#ifndef __MESH_H__
#define __MESH_H__

#include "../../def.h"
#include "../ecs.h"
#include "../../data/rgba.h"

typedef struct mesh {
	entity e;
	i32 sd_id;
	rgba color;
} mesh;

void mesh_components_init();
int mesh_component_get(const entity *e, mesh **m_ret);
int mesh_component_add(const entity *e, i32 md_id);
int mesh_component_remove(const entity *e);

#endif
