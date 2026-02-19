#include <stdio.h>
#include <glad/glad.h>

#include "./mesh_data.h"
#include "../ecs/components/mesh.h"
#include "../gen.h"

packed_array mesh_datas_pa;
mesh_data mesh_datas[MESH_DATA_LIMIT] = {0};
int mesh_data_map[MESH_DATA_LIMIT] = {0};

void mesh_data_init() {
    packed_array_init(&mesh_datas_pa, mesh_datas, sizeof(mesh_data), MESH_DATA_LIMIT, mesh_data_map, 0, 0);

    for(int i = 0; i < mesh_datas_pa.capacity; i++) {
        mesh_data *md = &mesh_datas[i];
        packed_array_init(&md->entities_pa, md->entities, sizeof(entity), MESH_DATA_ENTITY_LIMIT, md->entity_map, 0, 0);
    }
}

int mesh_data_add(const f32 *vertices, uint vertex_count, mesh_data_key *md_key) {
	ASSERT(vertices, "vertices is null\n");
	ASSERT(vertex_count > 0, "vertiex count <= 0\n");

    if(mesh_datas_pa.count == MESH_DATA_LIMIT) {
        eprintf("mesh_data is full\n");
        return 1;
    }

	mesh_data *md = packed_array_add(&mesh_datas_pa, mesh_datas_pa.count, gen_counter++);
    if(!md) {
        eprintf("mesh data packed array add failed\n");
        return 3;
    }

	packed_array_init(&md->entities_pa, md->entities, sizeof(entity), MESH_DATA_ENTITY_LIMIT, md->entity_map, 0, 0);
	md->vertex_count = vertex_count;
    glGenVertexArrays(1, &md->VAO);
	glBindVertexArray(md->VAO);

    glGenBuffers(1, &md->VBO);

	glBindBuffer(GL_ARRAY_BUFFER, md->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(f32) * 5 /*x y z u v*/, vertices, GL_STATIC_DRAW);

	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)0);
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	dprintf("registered mesh data [id:%d, gen:%d, vc:%d, VAO:%d, VBO:%d]\n", md->key.id, md->key.gen, vertex_count, md->VAO, md->VBO);

	*md_key = md->key;
	return 0;
}

int mesh_data_use(mesh_data_key key) {
    ASSERT(key.id >= 0 && key.id < MESH_DATA_LIMIT, "id [%d] not in range(0,%d)\n", key.id, MESH_DATA_LIMIT);

    mesh_data *md = packed_array_get(&mesh_datas_pa, key.id, key.gen);
    if(!md) return 1;

	glBindVertexArray(md->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, md->VBO);

    return 0;
}

int mesh_data_draw(mesh_data_key key) {
    ASSERT(key.id >= 0 && key.id < MESH_DATA_LIMIT, "id [%d] not in range(0,%d)\n", key.id, MESH_DATA_LIMIT);

    mesh_data *md = packed_array_get(&mesh_datas_pa, key.id, key.gen);
    if(!md) return 1;

    glDrawArrays(GL_TRIANGLES, 0, md->vertex_count);

    return 0;
}

int mesh_data_remove(mesh_data_key key) {
    ASSERT(key.id >= 0 && key.id < MESH_DATA_LIMIT, "id [%d] not in range(0,%d)\n", key.id, MESH_DATA_LIMIT);

    mesh_data *md = packed_array_get(&mesh_datas_pa, key.id, key.gen);
    if(!md) {
        eprintf("no mesh_data with [id:%d, gen:%d]\n", key.id, key.gen);
        return 1;
    }

    glDeleteVertexArrays(1, &md->VAO);
    glDeleteBuffers(1, &md->VBO);

    int err = 0;
    while(md->entities_pa.count > 0) err += mesh_component_remove(md->entities + md->entities_pa.count - 1);

    if(packed_array_remove(&mesh_datas_pa, key.id, key.gen)) {
        eprintf("failed to remove mesh_data [id:%d, gen:%d] from packed array\n", key.id, key.gen);
        return 1;
    }

    dprintf("unregistered mesh data [id:%d gen:%d]\n", key.id, key.gen);
    return err;
}

int mesh_data_register_entity(mesh_data_key key, const entity *e) {
    ASSERT(e, "entity is null\n");
    ASSERT(key.id >= 0 && key.id < MESH_DATA_LIMIT, "mesh_data id [%d] out of range\n", key.id);

    mesh_data *md = packed_array_get(&mesh_datas_pa, key.id, key.gen);
    if(!md) {
        eprintf("mesh_data not found [id:%d gen:%d]\n", key.id, key.gen);
        return 1;
    }

    if(md->entities_pa.count == MESH_DATA_ENTITY_LIMIT) {
        eprintf("mesh_data entities full [id:%d]\n", key.id);
        return 1;
    }

    entity *added = packed_array_add(&md->entities_pa, e->id, e->gen);
    if(!added) {
        eprintf("failed to add entity [id:%d, gen:%d] to mesh_data [id:%d, gen:%d]\n", e->id, e->gen, key.id, key.gen);
        return 1;
    }

    dprintf("registered entity [id:%d, gen:%d] to mesh_data [id:%d, gen:%d, count:%d]\n", e->id, e->gen, md->key.id, md->key.gen, md->entities_pa.count);

    return 0;
}

int mesh_data_unregister_entity(mesh_data_key key, const entity *e) {
    ASSERT(e, "entity is null\n");
    ASSERT(key.id >= 0 && key.id < MESH_DATA_LIMIT, "mesh_data id [%d] out of range\n", key.id);

    mesh_data *md = packed_array_get(&mesh_datas_pa, key.id, key.gen);
    if(!md) {
        eprintf("mesh_data not found [id:%d gen:%d]\n", key.id, key.gen);
        return 1;
    }

    if(packed_array_remove(&md->entities_pa, e->id, e->gen)) {
        eprintf("entity [id:%d gen:%d] not found in mesh_data [id:%d gen:%d]\n", e->id, e->gen, md->key.id, md->key.gen);
        return 2;
    }

	dprintf("unregistered entity [id:%d gen:%d] from mesh_data [id:%d gen:%d]\n", e->id, e->gen, md->key.id, md->key.gen);
	return 0;
}

int mesh_data_teardown() {
    int err = 0;

    while(mesh_datas_pa.count > 0) err += mesh_data_remove(mesh_datas[mesh_datas_pa.count - 1].key);

    return err;
}

void mesh_data_print() {
    dprintf("mesh data:");
    for(uint i = 0; i < mesh_datas_pa.count; i++) {
        mesh_data *md = (mesh_data*)((u8*)mesh_datas_pa.data + i * mesh_datas_pa.elem_size);
        printf(" [%d,%d (", md->key.id, md->key.gen);

        for(uint j = 0; j < md->entities_pa.count; j++) {
            entity *e = (entity*)((u8*)md->entities_pa.data + j * md->entities_pa.elem_size);
            printf("[%d,%d]", e->id, e->gen);
        }

        printf(")]");
    }
    printf("\n");
}
