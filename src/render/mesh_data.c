#include <stdio.h>
#include <string.h>
#include <glad/glad.h>
#include "mesh_data.h"
#include "../ecs/components/mesh.h"
#include "../ecs/components/transform.h"
#include "../ecs/components/camera.h"
#include "../window.h"

mesh_data mesh_datas[MESH_DATA_LIMIT] = {0};
i32 mesh_data_map[ENTITY_LIMIT] = {0};
u32 mesh_data_count = 0;

u32 shaderProgram;
u32 VAO;

void mesh_data_init() {
    glGenVertexArrays(1, &VAO);

	mesh_data_count = 0;
	for(int i = 0; i < MESH_DATA_LIMIT; i++) {
		mesh_datas[i].id = -1;
		mesh_datas[i].VBO = -1;
		mesh_datas[i].entity_count = 0;
		mesh_data_map[i] = -1;
		for(int j = 0; j < MESH_DATA_ENTITY_LIMIT; j++) {
			mesh_datas[i].entity_map[j] = -1;
			mesh_datas[i].entities[j].id = -1;
			mesh_datas[i].entities[j].gen = -1;
		}
	}

	//shader vertex
	unsigned int vertexShader;
	{
		const char *vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"layout (location = 1) in vec2 aUV;\n"
			"out vec2 TexCoord;\n"
			"uniform mat4 view;\n"
			"uniform mat4 transform;\n"
			"uniform mat4 projection;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = projection * view * transform * vec4(aPos, 1.0);\n"
			"	TexCoord = aUV;\n"
			//"   gl_Position = projection * transform * vec4(aPos, 1.0);\n"
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
			"in vec2 TexCoord;\n"
			"uniform sampler2D uTexture;\n"
			"void main()\n"
			"{\n"
			//"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); // RGB color\n"
			"	FragColor = texture(uTexture, TexCoord);\n"
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
		dprintf("create shaderProgram %d\n", shaderProgram);
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

	if(mesh_data_count == MESH_DATA_LIMIT) {
		eprintf("mesh_data is full\n");
		return -1;
	}

	int idx = mesh_data_count;
	mesh_data *md = mesh_datas + idx;
	ASSERT(md->id == -1, "mesh data id should be -1\n");
	md->id = idx;
	mesh_data_map[idx] = idx;

	mesh_data_count++;

	md->vertex_count = vertex_count;

    glGenBuffers(1, &md->VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, md->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(f32) * 5 /*x y z u v*/, vertices, GL_STATIC_DRAW);

	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)0);
	glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return md->id;
}

int mesh_data_use(i32 md_id) {
	ASSERT(md_id >= 0 && md_id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", md_id, MESH_DATA_LIMIT);

	i32 idx = mesh_data_map[md_id];
	if(idx == -1) return 1;

	mesh_data *md = mesh_datas + idx;
	ASSERT(md->id != -1, "mesh data id not set\n");
	glBindBuffer(GL_ARRAY_BUFFER, md->VBO);

	return 0;
}

int mesh_data_draw(i32 md_id) {
	ASSERT(md_id >= 0 && md_id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", md_id, MESH_DATA_LIMIT);

	i32 idx = mesh_data_map[md_id];
	if(idx == -1) return 1;

	mesh_data *md = mesh_datas + idx;
	ASSERT(md->id != -1, "mesh data id not set\n");
	glDrawArrays(GL_TRIANGLES, 0, md->vertex_count);

	return 0;
}

i32 mesh_data_remove(i32 id) {
	ASSERT(id >= 0 && id < MESH_DATA_LIMIT, "id [%d] not in range(0,%d)\n", id, MESH_DATA_LIMIT);

	int idx = mesh_data_map[id];
	if(idx == -1) {
		eprintf("no mesh_data with id [%d]\n", id);
		return 1;
	}

	mesh_data *md = mesh_datas + idx;
	ASSERT(md->id != -1, "no mesh_data with id [%d]\n", id);

    glDeleteBuffers(1, &md->VBO);

	i32 err = 0;
	//NOTE: deleted like this because the meshes unregister themseleves when removed
	while(md->entity_count > 0) {
		entity e = md->entities[0];

		err += mesh_component_remove(&e);
	}

	mesh_data_map[id] = -1;

	mesh_data *delete_md = md;
	if(mesh_data_count > 1 && mesh_data_count != idx + 1) {
		delete_md = mesh_datas + mesh_data_count - 1;
		ASSERT(delete_md->id != -1, "last entity does not exist\n");

		mesh_data_map[delete_md->id] = idx;
		memcpy(md, delete_md, sizeof(mesh_data));
	}

	delete_md->id = -1;
	delete_md->VBO = -1;
	delete_md->vertex_count = -1;
	for(int j = 0; j < delete_md->entity_count; j++) {
		delete_md->entities[j].id = -1;
		delete_md->entities[j].gen = -1;
	}
	mesh_data_count--;

	return err;
}

i32 mesh_data_register_entity(i32 id, const entity *e) {
	ASSERT(id >= 0 && id < MESH_DATA_LIMIT, "id [%d] not in range(0,%d)\n", id, MESH_DATA_LIMIT);

	int idx = mesh_data_map[id];
	if(idx == -1) {
		eprintf("no mesh_data with id [%d]\n", id);
		return 1;
	}

	mesh_data *md = mesh_datas + idx;
	ASSERT(md->id != -1, "no mesh_data with id [%d]\n", id);

	if(md->entity_count == MESH_DATA_ENTITY_LIMIT) {
		eprintf("mesh_data entities full [%d]\n", id);
		return 1;
	}

	int i = md->entity_count;
	md->entities[i].id = e->id;
	md->entities[i].gen = e->gen;
	md->entity_map[e->id] = i;
	md->entity_count++;

	return 0;
}

i32 mesh_data_unregister_entity(i32 id, const entity *e) {
	ASSERT(id >= 0 && id < MESH_DATA_LIMIT, "id [%d] not in range(0,%d)\n", id, MESH_DATA_LIMIT);

	int idx = mesh_data_map[id];
	if(idx == -1) {
		eprintf("no mesh_data with id [%d]\n", id);
		return 1;
	}

	mesh_data *md = mesh_datas + idx;
	ASSERT(md->id != -1, "no mesh_data with id [%d]\n", id);

	int entity_idx = md->entity_map[e->id];
	if(entity_idx == -1) {
		eprintf("entity not found [%d] [%d]\n", entity_idx, e->id);
		return 1;
	}

	entity *entity_delete = md->entities + entity_idx;
	ASSERT(entity_delete->id == e->id && entity_delete->gen == e->gen, "entity id or gen dont match [%d %d] [%d %d]\n", entity_delete->id, entity_delete->gen, e->id, e->gen);

	if(md->entity_count > 1 && md->entity_count != entity_idx + 1) {
		entity_delete = md->entities + md->entity_count - 1;
		ASSERT(entity_delete->id != -1, "last entity does not exist\n");

		md->entity_map[entity_delete->id] = idx;
		memcpy(md->entities + entity_idx, entity_delete, sizeof(entity));
	}

	entity_delete->id = -1;
	entity_delete->gen = -1;
	md->entity_count--;

	return 0;
}

i32 mesh_data_teardown() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);

	i32 err = 0;
	int i = 0;
	while(mesh_data_count > 0 && i++ < 5) {
		err += mesh_data_remove(mesh_datas[0].id);
	}

	return err;
}

void mesh_data_print() {
	dprintf("mesh data:");
	for(int i = 0; i < mesh_data_count; i++) {
		mesh_data *md = mesh_datas + i;
		//TODO: function without format
		printf(" [%d (", md->id);
		for(int j = 0; j < md->entity_count; j++) {
			printf("[%d, %d]", md->entities[j].id, md->entities[j].gen);
		}
		printf(")]");
	}
	printf("\n");
}
