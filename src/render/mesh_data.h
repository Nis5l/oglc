#ifndef __MESH_DATA_H__
#define __MESH_DATA_H__

#include "./shader_data.h"
#include "../def.h"
#include "../ecs/ecs.h"
#include "../data/packed_array.h"

#define MESH_DATA_LIMIT ENTITY_LIMIT
#define MESH_DATA_ENTITY_LIMIT ENTITY_LIMIT

typedef struct mesh_data {
    mesh_data_key key;

	shader_data_key sd_key;
    uint vertex_count;
    uint VBO;

    packed_array entities_pa;
    entity entities[MESH_DATA_ENTITY_LIMIT];
    int entity_map[MESH_DATA_ENTITY_LIMIT];
} mesh_data;

void mesh_data_init();
int mesh_data_add(const f32 *vertices, uint vertex_count, shader_data_key sd_key, mesh_data_key *md_key);
int mesh_data_use(mesh_data_key key);
int mesh_data_draw(mesh_data_key key);
int mesh_data_remove(mesh_data_key key);

int mesh_data_register_entity(mesh_data_key key, const entity *e);
int mesh_data_unregister_entity(mesh_data_key key, const entity *e);
int mesh_data_is_shader_data(mesh_data_key key, shader_data_key sd_key);

int mesh_data_teardown();
void mesh_data_print();

#endif
