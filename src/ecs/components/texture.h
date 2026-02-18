#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "../ecs.h"
#include "../../render/texture_data.h"

typedef struct texture {
	entity e;

	texture_data_key td_key;
} texture;

void texture_components_init();
int texture_component_get(const entity *e, texture **t_ret);
int texture_component_add(const entity *e, texture_data_key td_key);
int texture_component_remove(const entity *e);

#endif
