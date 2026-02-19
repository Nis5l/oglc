#ifndef __SHADER_DATA_H__
#define __SHADER_DATA_H__

#include <glad/glad.h>
#include "../def.h"
#include "../ecs/ecs.h"
#include "../data/packed_array.h"

#define SHADER_DATA_LIMIT ENTITY_LIMIT
#define SHADER_DATA_ENTITY_LIMIT ENTITY_LIMIT
#define SHADER_DATA_MESH_DATA_LIMIT ENTITY_LIMIT

typedef struct shader_uniforms {
	GLuint transform_loc;
	GLuint projection_loc;
	GLuint view_loc;
	GLuint texture_loc;
} shader_uniforms;

typedef struct shader_data_key {
    int id;
    uint gen;
} shader_data_key;

typedef struct shader_data {
    shader_data_key key;

	uint shaderProgram;

    packed_array entities_pa;
    entity entities[SHADER_DATA_ENTITY_LIMIT];
    int entity_map[SHADER_DATA_ENTITY_LIMIT];
} shader_data;

void shader_data_init();
int shader_data_add(const char *vertex_shader_path, const char *fragment_shader_path, shader_data_key *key);
int shader_data_remove(shader_data_key key);
int shader_data_use(shader_data_key key, shader_uniforms *uniforms);

int shader_data_register_entity(shader_data_key key, const entity *e);
int shader_data_unregister_entity(shader_data_key key, const entity *e);

int shader_data_teardown();

#endif
