#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "../../ecs.h"
#include "../../../data/packed_array.h"
#include "../../../def.h"

void components_init(packed_array *pa, void *components, size_t size, int *entity_map, uint capacity);
int component_add(packed_array *pa, const entity *e);
int component_get(packed_array *pa, const entity *e, void **t_ret);
int component_remove(packed_array *pa, const entity *e);
void component_entities_print(packed_array *pa);

#endif
