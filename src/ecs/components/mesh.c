#include "mesh.h"
#include "component/component.h"
#include "../../mesh_data.h"

mesh meshs[ENTITY_LIMIT] = {0};
i32 meshs_entity_map[ENTITY_LIMIT] = {0};

void mesh_components_init() {
	components_init(meshs, sizeof(mesh), meshs_entity_map);
}

int mesh_component_get(const entity *e, mesh **s_ret) {
	ASSERT(e, "entity is null");
	ASSERT(s_ret, "s_ret is null");

	if(component_get(meshs, sizeof(mesh), meshs_entity_map, e, (void**)s_ret)) {
		eprintf("could not get mesh\n");
		return 1;
	}

	return 0;
}

int mesh_component_add(const entity *e, i32 sd_id) {
	ASSERT(e, "entity is null");
	if(component_add(meshs, sizeof(mesh), meshs_entity_map, e)) {
		eprintf("adding mesh failed\n");
		return 1;
	}

	mesh *s;
	ASSERT(!mesh_component_get(e, &s), "mesh for entity [%d] not found\n", e->id);

	s->sd_id = sd_id;

	if(mesh_data_register_entity(sd_id, e)) return 2;

	return 0;
}

int mesh_component_remove(const entity *e) {
	ASSERT(e, "entity is null");
	ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

	mesh *s;
	if(mesh_component_get(e, &s)) {
		eprintf("mesh for entity [%d] not found\n", e->id);
		return 1;
	}

	if(mesh_data_unregister_entity(s->sd_id, e)) return 2;

	ASSERT(!component_remove(meshs, sizeof(mesh), meshs_entity_map, e), "removing mesh for entity [%d] failed\n", e->id);

	return 0;
}
