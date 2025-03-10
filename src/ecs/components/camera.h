#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "../ecs.h"
#include "../../data/v4.h"
#include "../../data/m4.h"

#define CAMERA_DISTANCE 10

typedef struct camera {
	entity e;
	m4 m;
} camera;

void camera_components_init();
int camera_component_get(const entity *e, camera **c_ret);
int camera_component_add(const entity *e);
int camera_component_remove(const entity *e);
void camera_update_matrices(f32 window_width, f32 window_height);

#endif
