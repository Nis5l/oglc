#ifndef __MESH_H__
#define __MESH_H__

#include "../ecs.h"
#include "../../data/rgba.h"
#include "../../render/mesh_data.h"

typedef struct mesh {
	entity e;

	mesh_data_key md_key;
	rgba color;
} mesh;

void mesh_components_init();
int mesh_component_get(const entity *e, mesh **m_ret);
int mesh_component_add(const entity *e, mesh_data_key md_key);
int mesh_component_remove(const entity *e);

#endif
