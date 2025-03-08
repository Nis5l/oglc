#include <stdio.h>
#include <string.h>
#include <glad/glad.h>
#include "mesh_data.h"
#include "ecs/components/mesh.h"
#include "ecs/components/transform.h"
#include "window.h"

unsigned int shaderProgram;
u32 VAO;
mesh_data mesh_datas[MESH_DATA_LIMIT] = {0};

void mesh_data_init() {
    glGenVertexArrays(1, &VAO);

	for(int i = 0; i < MESH_DATA_LIMIT; i++) {
		mesh_datas[i].id = -1;
		mesh_datas[i].VBO = -1;
		for(int j = 0; j < MESH_DATA_ENTITY_LIMIT; j++) {
			mesh_datas[i].entities[j].id = -1;
			mesh_datas[i].entities[j].gen = -1;
		}
	}

	//shader vertex
	unsigned int vertexShader;
	{
		const char *vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"uniform mat4 transform;\n"
			"uniform mat4 projection;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = projection * transform * vec4(aPos, 1.0);\n"
			//"   gl_Position = transform * vec4(aPos, 1.0);\n"
			//"   gl_Position = vec4(aPos, 1.0);\n"
			"}\0";

		vertexShader = glCreateShader(GL_VERTEX_SHADER);

		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			eprintf("creating vertex shader failed [%s]\n", infoLog);
		}
	}

	//shader fragment
	unsigned int fragmentShader;
	{
		const char *fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // RGB color\n"
			"}\0";
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		int  success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if(!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			eprintf("creating fragment shader failed [%s]\n", infoLog);
		}
	}

	{
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		int success;
		char infoLog[512];
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if(!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			eprintf("creating shader program failed [%s]\n", infoLog);
		}
	}
	//uglUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

i32 mesh_data_add(const f32 *vertices, u32 vertex_count) {
	ASSERT(vertices, "vertices is null\n");

	mesh_data *sd = 0;
	for(int i = 0; i < MESH_DATA_LIMIT; i++) {
		sd = mesh_datas + i;

		if(mesh_datas[i].id == -1) {
			mesh_datas[i].id = i;
			break;
		}
	}

	if(!sd) {
		eprintf("mesh_data is full\n");
		return -1;
	}

	sd->vertex_count = vertex_count;

    glGenBuffers(1, &sd->VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, sd->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(f32) * 3, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return sd->id;
}

i32 mesh_data_remove(i32 id) {
	ASSERT(id >= 0 && id < MESH_DATA_LIMIT, "id [%d] not in range(0,%d)\n", id, MESH_DATA_LIMIT);

	mesh_data *sd = mesh_datas + id;

	if(sd->id == -1) {
		eprintf("no mesh_data with id [%d]\n", id);
		return 1;
	}

    glDeleteBuffers(1, &sd->VBO);

	i32 err = 0;
	for(int i = 0; i < MESH_DATA_ENTITY_LIMIT; i++) {
		if(sd->entities[i].id == -1) break;

		err += mesh_component_remove(&sd->entities[i]);
	}

	mesh_data *last_sd = 0;
	for(int i = sd->id; i < MESH_DATA_ENTITY_LIMIT; i++) {
		if(mesh_datas[i].id == -1) break;
		last_sd = mesh_datas + i;
	}
	if(last_sd) {
		memcpy(sd, last_sd, sizeof(mesh_data));
	} else {
		last_sd = sd;
	}

	last_sd->id = -1;
	last_sd->VBO = -1;
	for(int j = 0; j < MESH_DATA_ENTITY_LIMIT; j++) {
		last_sd->entities[j].id = -1;
		last_sd->entities[j].gen = -1;
	}

	return err;
}

i32 mesh_data_register_entity(i32 id, const entity *e) {
	ASSERT(id >= 0 && id < MESH_DATA_LIMIT, "id [%d] not in range(0,%d)\n", id, MESH_DATA_LIMIT);

	mesh_data *sd = mesh_datas + id;

	if(sd->id == -1) {
		eprintf("no mesh_data with id [%d]\n", id);
		return 1;
	}

	for(int i = 0; i < MESH_DATA_ENTITY_LIMIT; i++) {
		if(sd->entities[i].id != -1) continue;

		sd->entities[i].id = e->id;
		sd->entities[i].gen = e->gen;

		if(i == MESH_DATA_ENTITY_LIMIT - 1) {
			eprintf("mesh_data entities full [%d]\n", id);
			return 1;
		}
		break;
	}

	return 0;
}

i32 mesh_data_unregister_entity(i32 id, const entity *e) {
	ASSERT(id >= 0 && id < MESH_DATA_LIMIT, "id [%d] not in range(0,%d)\n", id, MESH_DATA_LIMIT);

	mesh_data *sd = mesh_datas + id;

	if(sd->id == -1) {
		eprintf("no mesh_data with id [%d]\n", id);
		return 1;
	}

	for(int i = 0; i < MESH_DATA_ENTITY_LIMIT; i++) {
		if(sd->entities[i].id == -1) break;

		if(sd->entities[i].id == e->id && sd->entities[i].gen == e->gen) {
			entity *last_entity = 0;
			for(int j = i + 1; j < MESH_DATA_ENTITY_LIMIT; j++) {
				if(sd->entities[j].id == -1) break;
				last_entity = sd->entities + j;
			}
			if(last_entity) {
				memcpy(sd->entities + i, last_entity, sizeof(entity));
			} else {
				last_entity = sd->entities + i;
			}

			last_entity->id = -1;
			last_entity->gen = -1;
			return 0;
		}
	}

	eprintf("entity not found id [%d] [%d]\n", id, e->id);
	return 1;
}

i32 mesh_data_teardown() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);

	i32 err = 0;
	while(mesh_datas[0].id != -1) {
		err += mesh_data_remove(0);
	}

	return err;
}

void mesh_draw(window_data *wd) {
    glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	GLuint transform_loc = glGetUniformLocation(shaderProgram, "transform");
	GLuint projection_loc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projection_loc, 1, GL_TRUE, (GLfloat*)&wd->projection_m);

	for(int i = 0; i < MESH_DATA_LIMIT; i++) {
		if(mesh_datas[i].id == -1) break;

		glBindBuffer(GL_ARRAY_BUFFER, mesh_datas[i].VBO);

		for(int j = 0; j < MESH_DATA_ENTITY_LIMIT; j++) {
			if(mesh_datas[i].entities[j].id == -1) break;
			transform *t = 0;
			ASSERT(!transform_component_get(mesh_datas[i].entities + j, &t), "transform not found id [%d]\n", mesh_datas[i].entities[j].id);
			mesh *s = 0;
			ASSERT(!mesh_component_get(mesh_datas[i].entities + j, &s), "transform not found id [%d]\n", mesh_datas[i].entities[j].id);

			glUniformMatrix4fv(transform_loc, 1, GL_TRUE, (GLfloat*)&t->m);

			glDrawArrays(GL_TRIANGLES, 0, mesh_datas[i].vertex_count);
		}

		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glBindVertexArray(0);
}
