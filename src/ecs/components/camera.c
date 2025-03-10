#include "camera.h"
#include "component/component.h"
#include "transform.h"

u32 camera_component_count = 0;
camera cameras[ENTITY_LIMIT] = {0};
i32 camera_entity_map[ENTITY_LIMIT] = {0};

void camera_components_init() {
	components_init(cameras, sizeof(camera), camera_entity_map, &camera_component_count);
}

int camera_component_get(const entity *e, camera **c_ret) {
	ASSERT(e, "entity is null\n");
	ASSERT(c_ret, "c_ret is null\n");

	if(component_get(cameras, sizeof(camera), camera_entity_map, e, (void**)c_ret)) {
		eprintf("could not get amera\n");
		return 1;
	}

	return 0;
}

int camera_component_add(const entity *e) {
	ASSERT(e, "entity is null\n");

	if(component_add(cameras, sizeof(camera), camera_entity_map, &camera_component_count, e)) {
		eprintf("adding camera failed\n");
		return 1;
	}

	camera *c;
	ASSERT(!camera_component_get(e, &c), "camera for entity [%d] not found\n", e->id);

	m4_id(&c->m);

	return 0;
}

int camera_component_remove(const entity *e) {
	ASSERT(e, "entity is null\n");
	ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

	if(component_remove(cameras, sizeof(camera), camera_entity_map, &camera_component_count, e)) {
		eprintf("removing camera for entity [%d] failed\n", e->id);
		return 1;
	}
	return 0;
}

void camera_update_matrices(f32 window_width, f32 window_height) {
	for(int i = 0; i < camera_component_count; i++) {
		camera *c = cameras + i;
		ASSERT(c->e.id != -1, "transform entity id is -1\n");

		transform *t;
		ASSERT(!transform_component_get(&c->e, &t), "transform not found for camera %d\n", c->e.id);

		v4 pos = t->pos;
		pos.z += CAMERA_DISTANCE;
		pos.x -= window_width / 2.0f;
		pos.y -= window_height / 2.0f;
		m4 m;
		m4_id(&m);
		m4_set_position(&m, &pos);
		m4_inv(&c->m, &m);
	}
}
