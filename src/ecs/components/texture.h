#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "../ecs.h"

typedef struct texture {
	entity e;

	int td_id;
} texture;

void texture_components_init();
int texture_component_get(const entity *e, texture **t_ret);
int texture_component_add(const entity *e, i32 td_id);
int texture_component_remove(const entity *e);

#endif
