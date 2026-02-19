#include <stdlib.h>
#include <glad/glad.h>

#include "./mesh_data.h"
#include "./shader_data.h"
#include "../ecs/components/shader.h"
#include "../gen.h"
#include "../utils/file.h"

packed_array shader_datas_pa;
shader_data shader_datas[SHADER_DATA_LIMIT] = {0};
int shader_data_map[SHADER_DATA_LIMIT] = {0};

void shader_data_init() {
    packed_array_init(&shader_datas_pa, shader_datas, sizeof(shader_data), SHADER_DATA_LIMIT, shader_data_map, 0, 0);

    for(uint i = 0; i < shader_datas_pa.capacity; i++) {
        shader_data *sd = &shader_datas[i];
        packed_array_init(&sd->entities_pa, sd->entities, sizeof(entity), SHADER_DATA_ENTITY_LIMIT, sd->entity_map, 0, 0);
        packed_array_init(&sd->mesh_data_pa, sd->mesh_datas, sizeof(mesh_data_key), SHADER_DATA_MESH_DATA_LIMIT, sd->mesh_data_map, 0, 0);
    }
}

int shader_data_add(const char *vertex_shader_path, const char *fragment_shader_path, shader_data_key *key) {
	ASSERT(vertex_shader_path, "vertex_shader_path is null\n");
	ASSERT(fragment_shader_path, "fragment_shader_path is null\n");
	ASSERT(key, "shader_data_key is null\n");

    if(shader_datas_pa.count == SHADER_DATA_LIMIT) {
        eprintf("shader_data is full\n");
        return 1;
    }

	shader_data *sd = packed_array_add(&shader_datas_pa, shader_datas_pa.count, gen_counter++);
    if(!sd) {
        eprintf("shader data packed array add failed\n");
        return 2;
    }

	packed_array_init(&sd->entities_pa, sd->entities, sizeof(entity), SHADER_DATA_ENTITY_LIMIT, sd->entity_map, 0, 0);
	packed_array_init(&sd->mesh_data_pa, sd->mesh_datas, sizeof(mesh_data_key), SHADER_DATA_MESH_DATA_LIMIT, sd->mesh_data_map, 0, 0);

    glGenVertexArrays(1, &sd->VAO);

	unsigned int vertex_shader;
	{
		char *vertex_shader_source = read_file_text(vertex_shader_path);
		if(!vertex_shader_source) return 3;

		vertex_shader = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vertex_shader, 1, (const char * const *)&vertex_shader_source, NULL);
		glCompileShader(vertex_shader);

		//TODO
		//free(vertex_shader_source);

		int success;
		char infoLog[512];
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
			eprintf("creating vertex shader failed [%s]\n", infoLog);
			return 4;
		}
	}

	unsigned int fragment_shader;
	{
		char *fragment_shader_source = read_file_text(fragment_shader_path);
		if(!fragment_shader_source) return 5;

		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, (const char * const *)&fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		//TODO
		//free(fragment_shader_source);

		int  success;
		char infoLog[512];
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
			eprintf("creating fragment shader failed [%s]\n", infoLog);
			return 4;
		}
	}

	{
		sd->shaderProgram = glCreateProgram();
		dprintf("create shaderProgram [%d]\n", sd->shaderProgram);
		glAttachShader(sd->shaderProgram, vertex_shader);
		glAttachShader(sd->shaderProgram, fragment_shader);
		glLinkProgram(sd->shaderProgram);

		int success;
		char infoLog[512];
		glGetProgramiv(sd->shaderProgram, GL_LINK_STATUS, &success);
		if(!success) {
			glGetProgramInfoLog(sd->shaderProgram, 512, NULL, infoLog);
			eprintf("creating shader program failed [%s]\n", infoLog);
			return 5;
		}
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	dprintf("registered shader data [id:%d, gen:%d, f_source:%s, v_source:%s, shader_program:%d, VAO:%d]\n", sd->key.id, sd->key.gen, fragment_shader_path, vertex_shader_path, sd->shaderProgram, sd->VAO);

	*key = sd->key;

	return 0;
}

int shader_data_use(shader_data_key key, shader_uniforms *uniforms) {
    ASSERT(uniforms, "uniforms is null");
    ASSERT(key.id >= 0 && key.id < SHADER_DATA_LIMIT, "id [%d] not in range(0,%d)\n", key.id, SHADER_DATA_LIMIT);

    shader_data *sd = packed_array_get(&shader_datas_pa, key.id, key.gen);
    if(!sd) return 1;

    glUseProgram(sd->shaderProgram);
	glBindVertexArray(sd->VAO);

	uniforms->transform_loc = glGetUniformLocation(sd->shaderProgram, "transform");
	uniforms->projection_loc = glGetUniformLocation(sd->shaderProgram, "projection");
	uniforms->view_loc = glGetUniformLocation(sd->shaderProgram, "view");
	uniforms->texture_loc = glGetUniformLocation(sd->shaderProgram, "uTexture");

    return 0;
}

int shader_data_bind_vertex_array(shader_data_key key) {
    ASSERT(key.id >= 0 && key.id < SHADER_DATA_LIMIT, "id [%d] not in range(0,%d)\n", key.id, SHADER_DATA_LIMIT);

    shader_data *sd = packed_array_get(&shader_datas_pa, key.id, key.gen);
    if(!sd) return 1;

	//TODO: register mesh_data here, when deleted, delete mesh_data.
	glBindVertexArray(sd->VAO);

    return 0;
}

int shader_data_remove(shader_data_key key) {
    ASSERT(key.id >= 0 && key.id < SHADER_DATA_LIMIT, "id [%d] not in range(0,%d)\n", key.id, SHADER_DATA_LIMIT);

    shader_data *sd = packed_array_get(&shader_datas_pa, key.id, key.gen);
    if(!sd) {
        eprintf("no shader_data with [id:%d, gen:%d]\n", key.id, key.gen);
        return 1;
    }

    glDeleteVertexArrays(1, &sd->VAO);
    glDeleteProgram(sd->shaderProgram);

    int err = 0;
	//TODO: pointer or data?
    while(sd->entities_pa.count > 0) err += shader_component_remove(sd->entities + sd->entities_pa.count - 1);
    while(sd->mesh_data_pa.count > 0) err += mesh_data_remove(sd->mesh_datas[sd->mesh_data_pa.count - 1]);

    if(packed_array_remove(&shader_datas_pa, key.id, key.gen)) {
        eprintf("failed to remove shader_data [id:%d, gen:%d] from packed array\n", key.id, key.gen);
        return 1;
    }

    dprintf("unregistered shader data [id:%d gen:%d]\n", key.id, key.gen);
    return err;
}

int shader_data_register_entity(shader_data_key key, const entity *e) {
    ASSERT(e, "entity is null\n");
    ASSERT(key.id >= 0 && key.id < SHADER_DATA_LIMIT, "shader_data id [%d] out of range\n", key.id);

    shader_data *sd = packed_array_get(&shader_datas_pa, key.id, key.gen);
    if(!sd) {
        eprintf("shader_data not found [id:%d gen:%d]\n", key.id, key.gen);
        return 1;
    }

    if(sd->entities_pa.count == SHADER_DATA_ENTITY_LIMIT) {
        eprintf("shader_data entities full [id:%d]\n", key.id);
        return 1;
    }

    entity *added = packed_array_add(&sd->entities_pa, e->id, e->gen);
    if(!added) {
        eprintf("failed to add entity [id:%d, gen:%d] to shader_data [id:%d, gen:%d]\n", e->id, e->gen, key.id, key.gen);
        return 1;
    }

    dprintf("registered entity [id:%d, gen:%d] to shader_data [id:%d, gen:%d, count:%d]\n", e->id, e->gen, sd->key.id, sd->key.gen, sd->entities_pa.count);

    return 0;
}

int shader_data_unregister_entity(shader_data_key key, const entity *e) {
    ASSERT(e, "entity is null\n");
    ASSERT(key.id >= 0 && key.id < SHADER_DATA_LIMIT, "shader_data id [%d] out of range\n", key.id);

    shader_data *sd = packed_array_get(&shader_datas_pa, key.id, key.gen);
    if(!sd) {
        eprintf("shader_data not found [id:%d gen:%d]\n", key.id, key.gen);
        return 1;
    }

    if(packed_array_remove(&sd->entities_pa, e->id, e->gen)) {
        eprintf("entity [id:%d gen:%d] not found in shader_data [id:%d gen:%d]\n", e->id, e->gen, sd->key.id, sd->key.gen);
        return 2;
	}

	dprintf("unregistered entity [id:%d gen:%d] from shader_data [id:%d gen:%d]\n", e->id, e->gen, sd->key.id, sd->key.gen);
	return 0;
}

int shader_data_register_mesh_data(shader_data_key key, mesh_data_key md_key) {
    ASSERT(key.id >= 0 && key.id < SHADER_DATA_LIMIT, "shader_data id [%d] out of range\n", key.id);

    shader_data *sd = packed_array_get(&shader_datas_pa, key.id, key.gen);
    if(!sd) {
        eprintf("shader_data not found [id:%d gen:%d]\n", key.id, key.gen);
        return 1;
    }

    if(sd->mesh_data_pa.count == SHADER_DATA_MESH_DATA_LIMIT) {
        eprintf("shader_data entities full [id:%d]\n", key.id);
        return 1;
    }

    mesh_data_key *added = packed_array_add(&sd->mesh_data_pa, md_key.id, md_key.gen);
    if(!added) {
        eprintf("failed to add mesh_data [id:%d, gen:%d] to shader_data [id:%d, gen:%d]\n", md_key.id, md_key.gen, key.id, key.gen);
        return 1;
    }

    dprintf("registered mesh_data [id:%d, gen:%d] to shader_data [id:%d, gen:%d, count:%d]\n", md_key.id, md_key.gen, sd->key.id, sd->key.gen, sd->mesh_data_pa.count);

    return 0;
}

int shader_data_unregister_mesh_data(shader_data_key key, mesh_data_key md_key) {
    ASSERT(key.id >= 0 && key.id < SHADER_DATA_LIMIT, "shader_data id [%d] out of range\n", key.id);

    shader_data *sd = packed_array_get(&shader_datas_pa, key.id, key.gen);
    if(!sd) {
        eprintf("shader_data not found [id:%d gen:%d]\n", key.id, key.gen);
        return 1;
    }

    if(packed_array_remove(&sd->mesh_data_pa, md_key.id, md_key.gen)) {
        eprintf("mesh_data [id:%d gen:%d] not found in shader_data [id:%d gen:%d]\n", md_key.id, md_key.gen, sd->key.id, sd->key.gen);
        return 2;
	}

	dprintf("unregistered mesh_data [id:%d gen:%d] from shader_data [id:%d gen:%d]\n", md_key.id, md_key.gen, sd->key.id, sd->key.gen);
	return 0;
}

int shader_data_has_mesh_data(shader_data_key key, mesh_data_key md_key) {
    ASSERT(key.id >= 0 && key.id < SHADER_DATA_LIMIT, "shader_data id [%d] out of range\n", key.id);

    shader_data *sd = packed_array_get(&shader_datas_pa, key.id, key.gen);
    if(!sd) {
        eprintf("shader_data not found [id:%d gen:%d]\n", key.id, key.gen);
        return 0;
    }

    return !packed_array_get(&sd->entities_pa, md_key.id, md_key.gen);
}

int shader_data_teardown() {
    int err = 0;

    while(shader_datas_pa.count > 0) err += shader_data_remove(shader_datas[shader_datas_pa.count - 1].key);

    return err;
}
