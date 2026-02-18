#ifndef __MESH_DATA_H__
#define __MESH_DATA_H__

#include "../def.h"
#include "../ecs/ecs.h"
#include "../data/packed_array.h"

#define MESH_DATA_LIMIT ENTITY_LIMIT
#define MESH_DATA_ENTITY_LIMIT ENTITY_LIMIT

typedef struct mesh_data_key {
    i32 id;
    u32 gen;
} mesh_data_key;

typedef struct mesh_data {
    mesh_data_key key;

    u32 vertex_count;
    u32 VBO;

    packed_array entities_pa;
    entity entities[MESH_DATA_ENTITY_LIMIT];
    i32 entity_map[MESH_DATA_ENTITY_LIMIT];
} mesh_data;

void mesh_data_init();
i32 mesh_data_add(const f32 *vertices, u32 vertex_count, mesh_data_key *md_key);
i32 mesh_data_use(mesh_data_key key);
i32 mesh_data_draw(mesh_data_key key);
i32 mesh_data_remove(mesh_data_key key);

i32 mesh_data_register_entity(mesh_data_key key, const entity *e);
i32 mesh_data_unregister_entity(mesh_data_key key, const entity *e);

i32 mesh_data_teardown();
void mesh_data_print();

extern u32 shaderProgram;
extern u32 VAO;

#endif
