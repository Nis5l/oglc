#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "../ecs.h"
#include "../../data/v3.h"

struct transform {
	entity e;
	v3 pos;
	v3 scale;
} typedef transform;

void transforms_init();
int transform_component_get(const entity *e, transform **t);
int transform_component_add(const entity *e);
int transform_component_remove(i32 id);

#endif
