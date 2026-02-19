#include "../ecs/components/shader.h"
#include "../ecs/components/mesh.h"
#include "../ecs/components/texture.h"
#include "../ecs/components/transform.h"
#include "../ecs/components/camera.h"
#include "./render_batch.h"
#include "./mesh_data.h"
#include "./texture_data.h"
#include "../gen.h"

#include "shader_data.h"
packed_array render_batches_pa;
render_batch render_batches[RENDER_BATCH_LIMIT];
int render_batch_map[RENDER_BATCH_LIMIT];
int render_batches_sorted[RENDER_BATCH_LIMIT];

int render_batch_cmp(const void *a, const void *b) {
    const render_batch *ra = (const render_batch*)a;
    const render_batch *rb = (const render_batch*)b;

    if (ra->sd_key.id != rb->sd_key.id)
        return ra->sd_key.id - rb->sd_key.id;
    if (ra->md_key.id != rb->md_key.id)
        return ra->md_key.id - rb->md_key.id;

	return ra->td_key.id - rb->td_key.id;
}

void render_batch_init() {
    packed_array_init(&render_batches_pa, render_batches, sizeof(render_batch), RENDER_BATCH_LIMIT, render_batch_map, render_batches_sorted, render_batch_cmp);
}

render_batch *render_batch_find(shader_data_key sd_key, mesh_data_key md_key, texture_data_key td_key) {
    for(uint i = 0; i < render_batches_pa.count; i++) {
		render_batch *rb = render_batches + i;

        if(rb->md_key.id == md_key.id && rb->md_key.gen == md_key.gen && rb->td_key.id == td_key.id && rb->td_key.gen == td_key.gen && rb->sd_key.id == sd_key.id && rb->sd_key.gen == sd_key.gen) return rb;
    }
    return 0;
}

int render_batch_check_add(const entity *e) {
	ASSERT(e, "e is null\n");

	shader *s = 0;
	mesh *m = 0;
	texture *tex = 0;
	transform *t = 0;
	if(shader_component_get(e, &s)) return 0;
	if(mesh_component_get(e, &m)) return 0;
	if(texture_component_get(e, &tex)) return 0;
	if(transform_component_get(e, &t)) return 0;

	render_batch *rb = render_batch_find(s->sd_key, m->md_key, tex->td_key);

    if(!rb) {
        rb = packed_array_add(&render_batches_pa, render_batches_pa.count, gen_counter++);
        ASSERT(rb, "render batch add failed\n");

        rb->sd_key = s->sd_key;
        rb->md_key = m->md_key;
        rb->td_key = tex->td_key;

        packed_array_init(&rb->entities_pa, rb->entities, sizeof(entity), RENDER_BATCH_ENTITY_LIMIT, rb->entity_map, 0, 0);

        dprintf("created render batch [id:%d, gen:%d, sd_id:%d, sd_gen:%u, md_id:%d, md_gen:%u, td_id:%d, td_gen:%u]\n", rb->key.id, rb->key.gen, s->sd_key.id, s->sd_key.gen, m->md_key.id, m->md_key.gen, tex->td_key.id, tex->td_key.gen);
    }

    if(!packed_array_add(&rb->entities_pa, e->id, e->gen)) {
        return 0;
    }

    dprintf("added entity [id:%d, gen:%u] to render batch [id:%d, gen:%u, count:%d]\n", e->id, e->gen, rb->key.id, rb->key.gen, rb->entities_pa.count);

	return 0;
}

int render_batch_check_remove(const entity *e) {
	ASSERT(e, "entity is null\n");

    for(uint i = 0; i < render_batches_pa.count; i++) {
		render_batch *rb = render_batches + i;

        if(!packed_array_remove(&rb->entities_pa, e->id, e->gen)) {
            dprintf("removed entity [id:%d, gen:%u] from render batch [id:%d, gen:%u, count:%d]\n", e->id, e->gen, rb->key.id, rb->key.gen, rb->entities_pa.count);

            if(rb->entities_pa.count == 0) {
                packed_array_remove(&render_batches_pa, rb->key.id, rb->key.gen);

                dprintf("deleted empty render batch\n");
            }
            return 0;
        }
    }

	return 0;
}

int render_batch_teardown() {
	return 0;
}

//TODO: other file?
void render_batch_draw(window_data *wd) {
	camera *c;
	ASSERT(!camera_component_get(&wd->camera, &c), "camera not found for id [%d]\n", wd->camera.id);

	for(int i = 0; i < render_batches_pa.count; i++) {
		//TODO: check if still the same key if necessary
		render_batch *rb = packed_array_get_sorted(&render_batches_pa, i);

		shader_uniforms uniforms;

		ASSERT(!shader_data_use(rb->sd_key, &uniforms), "shader data use error\n");

		glUniformMatrix4fv(uniforms.projection_loc, 1, GL_TRUE, (GLfloat*)&wd->projection_m);
		glUniformMatrix4fv(uniforms.view_loc, 1, GL_TRUE, (GLfloat*)&c->m);

		ASSERT(!mesh_data_use(rb->md_key), "mesh data use error\n");

		ASSERT(!texture_data_use(rb->td_key), "texture data use error\n");

		glUniform1i(uniforms.texture_loc, 0);

		for(int j = 0; j < rb->entities_pa.count; j++) {
			entity *e = rb->entities + j;
			ASSERT(e->id != -1, "mesh data id is -1\n");

			transform *t = 0;
			texture *tex = 0;
			mesh *m = 0;
			ASSERT(!transform_component_get(e, &t), "transform not found for id [%d]\n", e->id);
			ASSERT(!mesh_component_get(e, &m), "mesh not found for id [%d]\n", e->id);
			ASSERT(!texture_component_get(e, &tex), "texture not found for id [%d]\n", e->id);

			glUniformMatrix4fv(uniforms.transform_loc, 1, GL_TRUE, (GLfloat*)&t->m);

			ASSERT(!mesh_data_draw(rb->md_key), "mesh data draw error\n");
		}
	}

	glBindVertexArray(0);
}
