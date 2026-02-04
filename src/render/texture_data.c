#include <string.h>
#include <glad/glad.h>
#include "../ecs/components/texture.h"
#include "texture_data.h"
#include "stb_image.h"

//TODO: abstract?
texture_data texture_datas[TEXTURE_DATA_LIMIT] = {0};
i32 texture_data_map[ENTITY_LIMIT] = {0};
u32 texture_data_count = 0;

void texture_data_init() {
	texture_data_count = 0;
	for(int i = 0; i < TEXTURE_DATA_LIMIT; i++) {
		texture_datas[i].id = -1;
		texture_datas[i].width = -1;
		texture_datas[i].height = -1;
		texture_datas[i].tex = -1;
		texture_datas[i].entity_count = 0;
		texture_data_map[i] = -1;
		for(int j = 0; j < TEXTURE_DATA_ENTITY_LIMIT; j++) {
			texture_datas[i].entity_map[j] = -1;
			texture_datas[i].entities[j].id = -1;
			texture_datas[i].entities[j].gen = -1;
		}
	}
}

i32 texture_data_add(const char *path) {
	ASSERT(path, "path is null\n");

	if(texture_data_count == TEXTURE_DATA_LIMIT) {
		eprintf("texture_data is full\n");
		return -1;
	}

	int idx = texture_data_count;
	texture_data *td = texture_datas + idx;
	ASSERT(td->id == -1, "texture data id should be -1\n");
	td->id = idx;
	//TODO: is this even right, same for mesh?
	texture_data_map[idx] = idx;

	texture_data_count++;

	int width, height, channels;
	unsigned char* data = stbi_load(path, &width, &height, &channels, 4);
	if(!data) {
		eprintf("error loading image %s\n", path);
		return -1;
	}

	glGenTextures(1, &td->tex);
	glBindTexture(GL_TEXTURE_2D, td->tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    td->width = width;
    td->height = height;

	return td->id;
}

i32 texture_data_remove(i32 id) {
	ASSERT(id >= 0 && id < TEXTURE_DATA_LIMIT, "id [%d] not in range(0,%d)\n", id, TEXTURE_DATA_LIMIT);

	int idx = texture_data_map[id];
	if(idx == -1) {
		eprintf("no texture_data with id [%d]\n", id);
		return 1;
	}

	texture_data *td = texture_datas + idx;
	ASSERT(td->id != -1, "no texture_data with id [%d]\n", id);

	glDeleteTextures(1, &td->tex); 

	i32 err = 0;
	//NOTE: deleted like this because the meshes unregister themseleves when removed
	while(td->entity_count > 0) {
		entity e = td->entities[0];

		err += texture_component_remove(&e);
	}

	texture_data_map[id] = -1;

	texture_data *delete_td = td;
	if(texture_data_count > 1 && texture_data_count != idx + 1) {
		delete_td = texture_datas + texture_data_count - 1;
		ASSERT(delete_td->id != -1, "last entity does not exist\n");

		texture_data_map[delete_td->id] = idx;
		memcpy(td, delete_td, sizeof(texture_data));
	}

	delete_td->id = -1;
	delete_td->tex = -1;
	delete_td->width = -1;
	delete_td->height = -1;

	texture_data_count--;

	return 0;
}

i32 texture_data_register_entity(i32 id, const entity *e) {
	ASSERT(id >= 0 && id < TEXTURE_DATA_LIMIT, "id [%d] not in range(0,%d)\n", id, TEXTURE_DATA_LIMIT);

	int idx = texture_data_map[id];
	if(idx == -1) {
		eprintf("no mesh_data with id [%d]\n", id);
		return 1;
	}

	texture_data *td = texture_datas + idx;
	ASSERT(td->id != -1, "no texture_data with id [%d]\n", id);

	if(td->entity_count == TEXTURE_DATA_ENTITY_LIMIT) {
		eprintf("texture_data entities full [%d]\n", id);
		return 1;
	}

	int i = td->entity_count;
	td->entities[i].id = e->id;
	td->entities[i].gen = e->gen;
	td->entity_map[e->id] = i;
	td->entity_count++;

	return 0;
}

i32 texture_data_unregister_entity(i32 id, const entity *e) {
	ASSERT(id >= 0 && id < TEXTURE_DATA_LIMIT, "id [%d] not in range(0,%d)\n", id, TEXTURE_DATA_LIMIT);

	int idx = texture_data_map[id];
	if(idx == -1) {
		eprintf("no texture_data with id [%d]\n", id);
		return 1;
	}

	texture_data *td = texture_datas + idx;
	ASSERT(td->id != -1, "no texture_data with id [%d]\n", id);

	int entity_idx = td->entity_map[e->id];
	if(entity_idx == -1) {
		eprintf("entity not found [%d] [%d]\n", entity_idx, e->id);
		return 1;
	}

	entity *entity_delete = td->entities + entity_idx;
	ASSERT(entity_delete->id == e->id && entity_delete->gen == e->gen, "entity id or gen dont match [%d %d] [%d %d]\n", entity_delete->id, entity_delete->gen, e->id, e->gen);

	if(td->entity_count > 1 && td->entity_count != entity_idx + 1) {
		entity_delete = td->entities + td->entity_count - 1;
		ASSERT(entity_delete->id != -1, "last entity does not exist\n");

		td->entity_map[entity_delete->id] = idx;
		memcpy(td->entities + entity_idx, entity_delete, sizeof(entity));
	}

	entity_delete->id = -1;
	entity_delete->gen = -1;
	td->entity_count--;

	return 0;
}

i32 texture_data_teardown() {
	i32 err = 0;
	int i = 0;
	while(texture_data_count > 0 && i++ < 5) {
		err += texture_data_remove(texture_datas[0].id);
	}

	return err;
}

i32 texture_data_use(i32 md_id) {
	ASSERT(md_id >= 0 && md_id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", md_id, TEXTURE_DATA_LIMIT);

	i32 idx = texture_data_map[md_id];
	if(idx == -1) return 1;

	texture_data *td = texture_datas + idx;
	ASSERT(td->id != -1, "texture data id not set\n");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, td->tex);

	return 0;
}
