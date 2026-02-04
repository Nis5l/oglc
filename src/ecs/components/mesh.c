#include "mesh.h"
#include "component/component.h"
#include "../../render/mesh_data.h"
#include "../../render/render_batch.h"

u32 mesh_component_count = 0;
mesh meshs[ENTITY_LIMIT] = {0};
i32 meshs_entity_map[ENTITY_LIMIT] = {0};

void mesh_components_init() {
	components_init(meshs, sizeof(mesh), meshs_entity_map, &mesh_component_count);
}

int mesh_component_get(const entity *e, mesh **m_ret) {
	ASSERT(e, "entity is null");
	ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);
	ASSERT(m_ret, "s_ret is null");

	if(component_get(meshs, sizeof(mesh), meshs_entity_map, e, (void**)m_ret)) {
		//eprintf("could not get mesh\n");
		return 1;
	}

	return 0;
}

int mesh_component_add(const entity *e, i32 md_id) {
	ASSERT(e, "entity is null");
	ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

	if(component_add(meshs, sizeof(mesh), meshs_entity_map, &mesh_component_count, e)) {
		eprintf("adding mesh failed\n");
		return 1;
	}
	mesh *m;
	ASSERT(!mesh_component_get(e, &m), "mesh for entity [%d] not found\n", e->id);

	m->md_id = md_id;

	if(mesh_data_register_entity(md_id, e)) return 2;

	if(render_batch_check_add(e)) return 3;

	return 0;
}

int mesh_component_remove(const entity *e) {
	ASSERT(e, "entity is null");
	ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

	mesh *m;
	if(mesh_component_get(e, &m)) {
		eprintf("mesh for entity [%d] not found\n", e->id);
		return 1;
	}

	if(mesh_data_unregister_entity(m->md_id, e)) return 2;

	ASSERT(!component_remove(meshs, sizeof(mesh), meshs_entity_map, &mesh_component_count, e), "removing mesh for entity [%d] failed\n", e->id);

	if(render_batch_check_remove(e)) return 3;

	return 0;
}
