#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "../ecs.h"
#include "../../data/v4.h"
#include "../../data/m4.h"
#include "../../input.h"

#define CAMERA_DISTANCE 10

typedef struct camera {
	entity e;
	v4 pos;
	m4 m;
} camera;

void camera_components_init();
int camera_component_get(const entity *e, camera **c_ret);
int camera_component_add(const entity *e);
int camera_component_remove(const entity *e);
void camera_update_pos(f32 window_width, f32 window_height);
void camera_update_matrices();
mouse camera_get_relative_mouse_pos(const entity *e, const mouse *m, f32 window_width, f32 window_height);

#endif
