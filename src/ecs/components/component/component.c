#include <stdio.h>
#include <string.h>

#include "./component.h"
#include "../../../def.h"

void components_init(packed_array *pa, void *components, size_t size, i32 *entity_map, u32 capacity) {
    packed_array_init(pa, components, size, capacity, entity_map);
}

int component_add(packed_array *pa, const entity *e) {
    void *ret = packed_array_add(pa, e->id, e->gen);
    if(!ret) {
        eprintf("component_add failed for entity [id:%d, gen:%d]\n", e->id, e->gen);
        return 1;
    }
    return 0;
}

int component_get(packed_array *pa, const entity *e, void **t_ret) {
    void *ret = packed_array_get(pa, e->id, e->gen);
    if(!ret) {
        //eprintf("component_get failed for entity [%d,%d]\n", e->id, e->gen);
        return 1;
    }
    *t_ret = ret;
    return 0;
}

int component_remove(packed_array *pa, const entity *e) {
    return packed_array_remove(pa, e->id, e->gen);
    /* int res = packed_array_remove(pa, e->id, e->gen);
    if(res) {
        eprintf("component_remove failed for entity [id:%d, gen:%d]\n", e->id, e->gen);
    }
    return res; */
}

void component_entities_print(packed_array *pa) {
    dprintf("components:");
    for(u32 i = 0; i < pa->count; i++) {
        entity *e = (entity*)((uint8_t*)pa->data + i * pa->elem_size);
        printf(" [%d, %d]", e->id, e->gen);
    }
    printf("\n");
}
