#include <string.h>
#include <glad/glad.h>
#include "../ecs/components/mesh.h"
#include "../ecs/components/texture.h"
#include "../ecs/components/transform.h"
#include "../ecs/components/camera.h"
#include "render_batch.h"
#include "mesh_data.h"
#include "stb_image.h"

#include "texture_data.h"
//TODO: SORT!!
render_batch render_batches[RENDER_BATCH_LIMIT] = {0};
i32 render_batch_map[ENTITY_LIMIT] = {0};
u32 render_batch_count = 0;

void render_batch_init() {
	render_batch_count = 0;
	for(int i = 0; i < RENDER_BATCH_LIMIT; i++) {
		render_batches[i].id = -1;
		render_batches[i].md_id = -1;
		render_batches[i].td_id = -1;
		render_batch_map[i] = -1;
		render_batches[i].entity_count = 0;
		for(int j = 0; j < RENDER_BATCH_ENTITY_LIMIT; j++) {
			render_batches[i].entity_map[j] = -1;
			render_batches[i].entities[j].id = -1;
			render_batches[i].entities[j].gen = -1;
		}
	}
}


i32 render_batch_check_add(const entity *e) {
	ASSERT(e, "e is null\n");

	mesh *m = 0;
	texture *tex = 0;
	transform *t = 0;
	if(mesh_component_get(e, &m)) return 0;
	if(texture_component_get(e, &tex)) return 0;
	if(transform_component_get(e, &t)) return 0;

	if(render_batch_map[e->id] != -1) return 0;

	dprintf("ADD render batch\n");
	render_batch *rb = 0;
	for(i32 i = 0; i < RENDER_BATCH_LIMIT; i++) {
		if(render_batches[i].id == -1) break;
		if(render_batches[i].md_id != m->md_id) continue;
		if(render_batches[i].td_id != tex->td_id) continue;

		rb = render_batches + i;
		render_batch_map[e->id] = i;
		break;
	}

	if(!rb) {
		dprintf("NEW\n");
		if(render_batch_count == RENDER_BATCH_LIMIT) {
			eprintf("render batch is full\n");
			return -1;
		}

		int idx = render_batch_count;
		rb = render_batches + idx;
		ASSERT(rb->id == -1, "render batch id should be -1\n");
		rb->id = idx;
		render_batch_map[e->id] = idx;
		render_batch_count++;
		rb->td_id = tex->td_id;
		rb->md_id = tex->td_id;
	}

	//dprintf("NEW ADDED %p %d\n", rb, rb->id);
	dprintf("NEW ADDED %p\n", rb);
	dprintf("A %p %d %d\n", e, e->id, e->gen);
	ASSERT(rb->entity_map[e->id] == -1, "render batch entity map should not be set\n");
	dprintf("B\n");
	int e_idx = rb->entity_count;
	entity *rb_e = rb->entities + e_idx;
	ASSERT(rb_e->id == -1, "render batch entity id should be -1\n");
	ASSERT(rb_e->gen == -1, "render batch entity gen should be -1\n");
	rb_e->id = e->id;
	rb_e->gen = e->gen;
	dprintf("C\n");
	rb->entity_map[e->id] = e_idx;
	dprintf("D\n");
	rb->entity_count++;
	dprintf("XXXX\n");

	return 0;
}

i32 render_batch_check_remove(const entity *e) {
	ASSERT(e, "e is null\n");

	i32 idx = render_batch_map[e->id];
	if(idx == -1) return 0;

	render_batch_map[e->id] = -1;

	render_batch *rb = render_batches + idx;
	i32 e_idx = rb->entity_map[e->id];
	ASSERT(e_idx != -1, "render batch entity map should be set\n");
	ASSERT(rb->entities[e_idx].id == e->id, "render batch entity id should match\n");

	entity *delete_e = rb->entities + e_idx;
	if(rb->entity_count > 1 && rb->entity_count != e_idx + 1) {
		delete_e = rb->entities + rb->entity_count - 1;
		ASSERT(delete_e->id != -1, "last entity does not exist\n");

		rb->entity_map[delete_e->id] = e_idx;
		memcpy(rb->entities + e_idx, delete_e, sizeof(entity));
	}

	delete_e->id = -1;
	delete_e->gen = -1;

	rb->entity_count--;

	if(rb->entity_count > 0) return 0;

	render_batch *delete_rb = rb;
	if(render_batch_count > 1 && render_batch_count != idx + 1) {
		delete_rb = render_batches + render_batch_count - 1;
		ASSERT(delete_rb->id != -1, "last entity does not exist\n");

		render_batch_map[delete_rb->id] = idx;
		memcpy(rb, delete_rb, sizeof(render_batch));
	}

	return 0;
}

i32 render_batch_teardown() {
	return 0;
}

//TODO: other file?
void render_batch_draw(window_data *wd) {
	dprintf("shaderProgram %d\n", shaderProgram);
    glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	camera *c;

	ASSERT(!camera_component_get(&wd->camera, &c), "camera not found for id [%d]\n", wd->camera.id);

	GLuint transform_loc = glGetUniformLocation(shaderProgram, "transform");
	GLuint projection_loc = glGetUniformLocation(shaderProgram, "projection");
	GLuint view_loc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(projection_loc, 1, GL_TRUE, (GLfloat*)&wd->projection_m);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, (GLfloat*)&c->m);

	dprintf("draw %d\n", render_batch_count);
	for(int i = 0; i < render_batch_count; i++) {
		render_batch *rb = render_batches + i;
		ASSERT(rb->id != -1, "render batch id is -1\n");

		ASSERT(!mesh_data_use(rb->md_id), "mesh data use error\n");

		ASSERT(!texture_data_use(rb->td_id), "texture data use error\n");
		glUniform1i(glGetUniformLocation(shaderProgram, "uTexture"), 0);

		dprintf("rb ec %d\n", rb->entity_count);
		for(int j = 0; j < rb->entity_count; j++) {
			entity *e = rb->entities + j;
			ASSERT(e->id != -1, "mesh data id is -1\n");

			transform *t = 0;
			texture *tex = 0;
			mesh *m = 0;
			ASSERT(!transform_component_get(e, &t), "transform not found for id [%d]\n", e->id);
			ASSERT(!mesh_component_get(e, &m), "mesh not found for id [%d]\n", e->id);
			ASSERT(!texture_component_get(e, &tex), "texture not found for id [%d]\n", e->id);

			glUniformMatrix4fv(transform_loc, 1, GL_TRUE, (GLfloat*)&t->m);

			dprintf("d\n");
			ASSERT(!mesh_data_draw(rb->md_id), "mesh data draw error\n");
		}

		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glBindVertexArray(0);
}
