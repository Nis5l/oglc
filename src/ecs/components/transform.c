#include "transform.h"
#include "component/component.h"
#include "../../data/m4.h"

u32 transform_component_count = 0;
transform transforms[ENTITY_LIMIT] = {0};
i32 transforms_entity_map[ENTITY_LIMIT] = {0};

void transform_components_init() {
	components_init(transforms, sizeof(transform), transforms_entity_map, &transform_component_count);
}

int transform_component_get(const entity *e, transform **t_ret) {
	ASSERT(e, "entity is null");
	ASSERT(t_ret, "t_ret is null");
	if(component_get(transforms, sizeof(transform), transforms_entity_map, e, (void**)t_ret)) {
		eprintf("could not get transform\n");
		return 1;
	}

	return 0;
}

int transform_component_add(const entity *e) {
	ASSERT(e, "entity is null");
	if(component_add(transforms, sizeof(transform), transforms_entity_map, &transform_component_count, e)) {
		eprintf("adding transform failed\n");
		return 1;
	}

	transform *t;
	ASSERT(!transform_component_get(e, &t), "transform for entity [%d] not found\n", e->id);

	t->pos.x = 0;
	t->pos.y = 0;
	t->pos.z = 0;
	t->pos.w = 1;
	t->scale.x = 1;
	t->scale.y = 1;
	t->scale.z = 1;
	//TODO: does scale need w or maybe v3?
	t->scale.w = 1;
	m4_id(&t->m);

	return 0;
}

int transform_component_remove(const entity *e) {
	ASSERT(e, "entity is null");
	ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);
	if(component_remove(transforms, sizeof(transform), transforms_entity_map, &transform_component_count, e)) {
		eprintf("removing transform for entity [%d] failed\n", e->id);
		return 1;
	}
	return 0;
}

void transform_update_matrices() {
	for(int i = 0; i < ENTITY_LIMIT; i++) {
		if(transforms[i].e.id == -1) break;
		m4_set_position(&transforms[i].m, &transforms[i].pos);
		m4_set_scale(&transforms[i].m, &transforms[i].scale);
	}
}

int transform_print_matrix(const entity *e) {
	ASSERT(e, "entity is null");
	ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

	transform *t;
	if(transform_component_get(e, &t)) {
		eprintf("transform not found, id [%d]", e->id);
		return 1;
	}

	dprintf("entity [%d]\n", e->id);
	m4_print(&t->m);

	return 0;
}
