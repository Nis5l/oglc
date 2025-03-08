#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "../ecs.h"
#include "../../data/v4.h"
#include "../../data/m4.h"

typedef struct transform {
	entity e;
	v4 pos, scale;
	m4 m;
} transform;

void transform_components_init();
int transform_component_get(const entity *e, transform **t_ret);
int transform_component_add(const entity *e);
int transform_component_remove(const entity *e);
void transform_update_matrices();
int transform_print_matrix(const entity *e);

#endif
