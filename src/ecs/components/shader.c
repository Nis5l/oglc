#include "./shader.h"
#include "./component/component.h"
#include "../../render/shader_data.h"
#include "../../render/render_batch.h"

packed_array shader_pa;
shader shaders[ENTITY_LIMIT];
int shaders_entity_map[ENTITY_LIMIT];

void shader_components_init() {
    components_init(&shader_pa, shaders, sizeof(shader), shaders_entity_map, ENTITY_LIMIT);
}

int shader_component_get(const entity *e, shader **s_ret) {
    ASSERT(e, "entity is null\n");
    ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);
    ASSERT(s_ret, "s_ret is null\n");

    return component_get(&shader_pa, e, (void**)s_ret);
}

int shader_component_add(const entity *e, shader_data_key sd_key) {
    ASSERT(e, "entity is null\n");
    ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

    if(component_add(&shader_pa, e)) {
        eprintf("adding shader failed\n");
        return 2;
    }

    shader *s;
    ASSERT(!shader_component_get(e, &s), "mesh for entity [%d] not found\n", e->id);

    s->e = *e;
    s->sd_key = sd_key;

    if(shader_data_register_entity(sd_key, e)) return 2;
    if(render_batch_check_add(e)) return 3;

    return 0;
}

int shader_component_remove(const entity *e) {
    ASSERT(e, "entity is null\n");
    ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

    shader *m;
    if(shader_component_get(e, &m)) {
        //eprintf("mesh for entity [id:%d, gen:%d] not found\n", e->id, e->gen);
        return 1;
    }

    if(shader_data_unregister_entity(m->sd_key, e)) return 2;

    ASSERT(!component_remove(&shader_pa, e), "removing shader for entity [id:%d, gen:%d] failed\n", e->id, e->gen);

    if(render_batch_check_remove(e)) return 3;

    return 0;
}
