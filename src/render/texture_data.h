#ifndef __TEXTURE_DATA_H__
#define __TEXTURE_DATA_H__

#include <glad/glad.h>
#include "../def.h"
#include "../ecs/ecs.h"
#include "../data/packed_array.h"

#define TEXTURE_DATA_LIMIT ENTITY_LIMIT
#define TEXTURE_DATA_ENTITY_LIMIT ENTITY_LIMIT

typedef struct texture_data_key {
    int id;
    uint gen;
} texture_data_key;

typedef struct texture_data {
	texture_data_key key;

	GLuint tex;
	uint width;
	uint height;

	packed_array entities_pa;
	entity entities[TEXTURE_DATA_ENTITY_LIMIT];
	int entity_map[TEXTURE_DATA_ENTITY_LIMIT];
} texture_data;

void texture_data_init();
int texture_data_add(const char *path, texture_data_key *key);
int texture_data_remove(texture_data_key key);
int texture_data_teardown();
int texture_data_register_entity(texture_data_key key, const entity *e);
int texture_data_unregister_entity(texture_data_key key, const entity *e);
int texture_data_use(texture_data_key key);

#endif
