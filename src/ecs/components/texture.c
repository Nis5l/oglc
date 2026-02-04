//TODO: rename texture
#include "texture.h"
#include "component/component.h"
#include "component/component.h"
#include "../../render/texture_data.h"
#include "../../render/render_batch.h"
//#include "./mesh.h"

u32 texture_component_count = 0;
texture textures[ENTITY_LIMIT] = {0};
i32 texture_entity_map[ENTITY_LIMIT] = {0};

void texture_components_init() {
	components_init(textures, sizeof(texture), texture_entity_map, &texture_component_count);

	//ASSERT(texture_mesh_id >= 0, "initializing textures error (mesh_data_add)\n");
}

int texture_component_get(const entity *e, texture **t_ret) {
	ASSERT(e, "entity is null\n");
	ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);
	ASSERT(t_ret, "t_ret is null\n");

	if(component_get(textures, sizeof(texture), texture_entity_map, e, (void**)t_ret)) {
		//eprintf("could not get texture\n");
		return 1;
	}

	return 0;
}

int texture_component_add(const entity *e, i32 td_id) {
	ASSERT(e, "entity is null\n");
	ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

	if(component_add(textures, sizeof(texture), texture_entity_map, &texture_component_count, e)) {
		eprintf("adding texture failed\n");
		return 1;
	}

	dprintf("add\n");
	texture *t;
	ASSERT(!texture_component_get(e, &t), "texture for entity [%d] not found\n", e->id);

	t->td_id = td_id;

	dprintf("register\n");
	if(texture_data_register_entity(td_id, e)) return 2;

	dprintf("render batch\n");
	if(render_batch_check_add(e)) return 3;

	dprintf("end\n");
	return 0;
}

int texture_component_remove(const entity *e) {
	ASSERT(e, "entity is null");
	ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

	texture *t;
	if(texture_component_get(e, &t)) {
		eprintf("texture for entity [%d] not found\n", e->id);
		return 1;
	}

	if(texture_data_unregister_entity(t->td_id, e)) return 2;

	ASSERT(!component_remove(textures, sizeof(texture), texture_entity_map, &texture_component_count, e), "removing texture for entity [%d] failed\n", e->id);

	if(render_batch_check_remove(e)) return 3;

	return 0;
}
