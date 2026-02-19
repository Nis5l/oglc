#include "./mesh.h"
#include "./shader.h"
#include "./component/component.h"
#include "../../render/mesh_data.h"
#include "../../render/render_batch.h"

packed_array mesh_pa;
mesh meshs[ENTITY_LIMIT];
int meshs_entity_map[ENTITY_LIMIT];

void mesh_components_init() {
    components_init(&mesh_pa, meshs, sizeof(mesh), meshs_entity_map, ENTITY_LIMIT);
}

int mesh_component_get(const entity *e, mesh **m_ret) {
    ASSERT(e, "entity is null\n");
    ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);
    ASSERT(m_ret, "m_ret is null\n");

    return component_get(&mesh_pa, e, (void**)m_ret);
}

int mesh_component_add(const entity *e, mesh_data_key md_key) {
    ASSERT(e, "entity is null\n");
    ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

	shader *s;
	if(!shader_component_get(e, &s)) {
		if(!mesh_data_is_shader_data(md_key, s->sd_key)) {
			eprintf("mesh component shader data does not match shader component shader data\n");
			return 1;
		}
	}

    if(component_add(&mesh_pa, e)) {
        eprintf("adding mesh failed\n");
        return 1;
    }

    mesh *m;
    ASSERT(!mesh_component_get(e, &m), "mesh for entity [%d] not found\n", e->id);

    m->e = *e;
    m->md_key = md_key;

    if(mesh_data_register_entity(md_key, e)) return 2;
    if(render_batch_check_add(e)) return 3;

    return 0;
}

int mesh_component_remove(const entity *e) {
    ASSERT(e, "entity is null\n");
    ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

    mesh *m;
    if(mesh_component_get(e, &m)) {
        //eprintf("mesh for entity [id:%d, gen:%d] not found\n", e->id, e->gen);
        return 1;
    }

    if(mesh_data_unregister_entity(m->md_key, e)) return 2;

    ASSERT(!component_remove(&mesh_pa, e), "removing mesh for entity [id:%d, gen:%d] failed\n", e->id, e->gen);

    if(render_batch_check_remove(e)) return 3;

    return 0;
}

void mesh_print_entities() {
    component_entities_print(&mesh_pa);
}
