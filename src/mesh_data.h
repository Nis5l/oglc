#ifndef __MESH_DATA_H__
#define __MESH_DATA_H__

#include "def.h"
#include "window.h"
#include "ecs/ecs.h"

#define MESH_DATA_LIMIT 128
#define MESH_DATA_ENTITY_LIMIT 128

typedef struct mesh_data {
	//TODO: maybe gen
	i32 id;

	u32 vertex_count;
	u32 VBO;

	u32 entity_count;
	i32 entity_map[MESH_DATA_ENTITY_LIMIT];
	entity entities[MESH_DATA_ENTITY_LIMIT];
} mesh_data;

void mesh_data_init();
i32 mesh_data_add(const f32 *vertices, u32 vertex_count);
i32 mesh_data_remove(i32 id);
i32 mesh_data_teardown();
i32 mesh_data_register_entity(i32 id, const entity *e);
i32 mesh_data_unregister_entity(i32 id, const entity *e);
void mesh_draw(window_data *wd);
void mesh_data_print();

#endif
