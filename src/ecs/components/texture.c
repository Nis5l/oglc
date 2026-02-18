#include "texture.h"
#include "component/component.h"
#include "../../render/texture_data.h"
#include "../../render/render_batch.h"

packed_array texture_pa;
texture textures[ENTITY_LIMIT];
i32 texture_entity_map[ENTITY_LIMIT];

void texture_components_init() {
    components_init(&texture_pa, textures, sizeof(texture), texture_entity_map, ENTITY_LIMIT);
}

int texture_component_get(const entity *e, texture **t_ret) {
    ASSERT(e, "entity is null\n");
    ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);
    ASSERT(t_ret, "t_ret is null\n");

    return component_get(&texture_pa, e, (void**)t_ret);
}

int texture_component_add(const entity *e, texture_data_key td_key) {
    ASSERT(e, "entity is null\n");
    ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

    if(component_add(&texture_pa, e)) {
        eprintf("adding texture failed\n");
        return 1;
    }

    texture *t;
    ASSERT(!texture_component_get(e, &t), "texture for entity [%d] not found\n", e->id);

    t->e = *e;
    t->td_key = td_key;

    if(texture_data_register_entity(td_key, e)) return 2;
    if(render_batch_check_add(e)) return 3;

    return 0;
}

int texture_component_remove(const entity *e) {
    ASSERT(e, "entity is null\n");
    ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

    texture *t;
    if(texture_component_get(e, &t)) {
        //eprintf("texture for entity [%d] not found\n", e->id);
        return 1;
    }

    if(texture_data_unregister_entity(t->td_key, e)) return 2;

    ASSERT(!component_remove(&texture_pa, e), "removing texture for entity [%d] failed\n", e->id);
    if(render_batch_check_remove(e)) return 3;

    return 0;
}

void texture_print_entities() {
    component_entities_print(&texture_pa);
}
