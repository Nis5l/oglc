#include "transform.h"
#include "component/component.h"
#include "../../data/m4.h"
#include "../../render/render_batch.h"

packed_array transform_pa;
transform transforms[ENTITY_LIMIT];
i32 transform_entity_map[ENTITY_LIMIT];

void transform_components_init() {
    components_init(&transform_pa, transforms, sizeof(transform), transform_entity_map, ENTITY_LIMIT);
}

int transform_component_get(const entity *e, transform **t_ret) {
    ASSERT(e, "entity is null\n");
    ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);
    ASSERT(t_ret, "t_ret is null\n");

    return component_get(&transform_pa, e, (void**)t_ret);
}

int transform_component_add(const entity *e) {
    ASSERT(e, "entity is null\n");
    ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

    if(component_add(&transform_pa, e)) {
        eprintf("adding transform failed\n");
        return 1;
    }

    transform *t;
    ASSERT(!transform_component_get(e, &t), "transform for entity [%d] not found\n", e->id);

	t->pos.x = 0;
	t->pos.y = 0;
	t->pos.z = 0;
	t->pos.w = 1;
	t->scale.x = 1;
	t->scale.y = 1;
	t->scale.z = 1;
	//TODO: does scale need w or maybe v3?
	t->scale.w = 1;
	m4_id(&t->m);

    if(render_batch_check_add(e)) return 2;

    return 0;
}

int transform_component_remove(const entity *e) {
    ASSERT(e, "entity is null\n");
    ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

    if(component_remove(&transform_pa, e)) {
        //eprintf("removing transform for entity [%d] failed\n", e->id);
        return 1;
    }

    if(render_batch_check_remove(e)) return 2;

    return 0;
}

void transform_update_matrices() {
    for(u32 i = 0; i < transform_pa.count; i++) {
        transform *t = (transform*)((uint8_t*)transform_pa.data + i * transform_pa.elem_size);
        ASSERT(t->e.id != -1, "transform entity id is -1\n");
        m4_set_position(&t->m, &t->pos);
        m4_set_scale(&t->m, &t->scale);
    }
}

int transform_print_matrix(const entity *e) {
    ASSERT(e, "entity is null\n");
    ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

    transform *t;
    if(transform_component_get(e, &t)) {
        eprintf("transform not found, id [%d]\n", e->id);
        return 1;
    }

    dprintf("entity [%d]\n", e->id);
    m4_print(&t->m);

    return 0;
}

void transform_print_entities() {
    component_entities_print(&transform_pa);
}
