#ifndef __MESH_H__
#define __MESH_H__

#include "../../def.h"
#include "../ecs.h"
#include "../../data/rgba.h"

typedef struct mesh {
	i32 sd_id;
	entity e;
	rgba color;
} mesh;

void mesh_components_init();
int mesh_component_get(const entity *e, mesh **s_ret);
int mesh_component_add(const entity *e, i32 sd_id);
int mesh_component_remove(const entity *e);

#endif
