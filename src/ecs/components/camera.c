#include "camera.h"
#include "component/component.h"
#include "transform.h"

packed_array camera_pa;
camera cameras[ENTITY_LIMIT];
int camera_entity_map[ENTITY_LIMIT];

void camera_components_init() {
    components_init(&camera_pa, cameras, sizeof(camera), camera_entity_map, ENTITY_LIMIT);
}

int camera_component_get(const entity *e, camera **c_ret) {
    ASSERT(e, "entity is null\n");
    ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);
    ASSERT(c_ret, "c_ret is null\n");

    return component_get(&camera_pa, e, (void**)c_ret);
}

int camera_component_add(const entity *e) {
    ASSERT(e, "entity is null\n");
    ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

    if(component_add(&camera_pa, e)) {
        eprintf("adding camera failed\n");
        return 1;
    }

    camera *c;
    ASSERT(!camera_component_get(e, &c), "camera for entity [id:%d, gen:%d] not found\n", e->id, e->gen);

    c->e = *e;
    c->pos.x = 0;
    c->pos.y = 0;
    c->pos.z = 0;
    c->pos.w = 1;
    m4_id(&c->m);

    return 0;
}

int camera_component_remove(const entity *e) {
    ASSERT(e, "entity is null\n");
    ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

    if(component_remove(&camera_pa, e)) {
        //eprintf("removing camera for entity [id:%d, gen:%d] failed\n", e->id, e->gen);
        return 1;
    }
    return 0;
}

void camera_update_pos(f32 window_width, f32 window_height) {
    for(uint i = 0; i < camera_pa.count; i++) {
        camera *c = (camera*)((uint8_t*)camera_pa.data + i * camera_pa.elem_size);
        ASSERT(c->e.id != -1, "camera entity id is -1\n");

        transform *t;
        ASSERT(!transform_component_get(&c->e, &t), "transform not found for camera [id:%d, gen:%d]\n", c->e.id, c->e.gen);

        c->pos.x = t->pos.x - window_width / 2.0f;
        c->pos.y = t->pos.y - window_height / 2.0f;
        c->pos.z = t->pos.z + CAMERA_DISTANCE;
    }
}

void camera_update_matrices() {
    for(uint i = 0; i < camera_pa.count; i++) {
        camera *c = (camera*)((uint8_t*)camera_pa.data + i * camera_pa.elem_size);
        ASSERT(c->e.id != -1, "camera entity id is -1\n");

        m4 m;
        m4_id(&m);
        m4_set_position(&m, &c->pos);
        m4_inv(&c->m, &m);
    }
}

mouse camera_get_relative_mouse_pos(const entity *e, const mouse *m, f32 window_width, f32 window_height) {
    ASSERT(m, "mouse is null\n");
    ASSERT(e, "entity is null\n");
    ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

    camera *c;
    ASSERT(!camera_component_get(e, &c), "camera not found for entity [id:%d, gen:%d]\n", e->id, e->gen);

    mouse m_ret;
    m_ret.mouse_x = c->pos.x + m->mouse_x;
    m_ret.mouse_y = c->pos.y + window_height - m->mouse_y;

    return m_ret;
}

void camera_print_entities() {
    component_entities_print(&camera_pa);
}
