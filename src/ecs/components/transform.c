#include "transform.h"
#include "component/component.h"

transform transforms[ENTITY_LIMIT] = {0};
i32 transforms_entity_map[ENTITY_LIMIT] = {0};

void transforms_init() {
	components_init(transforms, sizeof(transform), transforms_entity_map);
}

int transform_component_get(const entity *e, transform **t_ret) {
	if(component_get(transforms, sizeof(transform), transforms_entity_map, e, (void**)t_ret)) {
		eprintf("could not get transform\n");
		return 1;
	}

	return 0;
}

int transform_component_add(const entity *e) {
	if(component_add(transforms, sizeof(transform), transforms_entity_map, e)) {
		eprintf("adding transform failed\n");
		return 1;
	}

	transform *t;
	if(transform_component_get(e, &t)) {
		eprintf("transform for entity [%d] not found, this should never happen!\n", e->id);
		return 2;
	}

	t->pos.x = 0;
	t->pos.y = 0;
	t->pos.w = 1;
	t->scale.x = 1;
	t->scale.y = 1;
	t->scale.w = 1;
	if(m3_id(&t->m)) {
		eprintf("could not set matrix to identity matrix, this should never happen");
		return 3;
	}

	return 0;
}

int transform_component_remove(i32 id) {
	if(component_remove(transforms, sizeof(transform), transforms_entity_map, id)) {
		eprintf("removing component for entity [%d] failed\n", id);
		return 1;
	}
	return 0;
}

int transform_update_matrix(transform *t) {
	if(!t) {
		eprintf("transform is null\n");
		return 1;
	}

	if(m3_set_position(&t->m, &t->pos)) {
		eprintf("could not set transform matrix position, this should never happen\n");
		return 2;
	}

	if(m3_set_scale(&t->m, &t->scale)) {
		eprintf("could not set transform matrix scale, this should never happen\n");
		return 3;
	}

	return 0;
}
