#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "../../ecs.h"

void components_init(void *components, size_t size, i32 *entity_map, u32 *component_count);
int component_get(void *components, size_t size, i32 *entity_map, const entity *e, void **t_ret);
int component_add(void *components, size_t size, i32 *entity_map, u32 *component_count, const entity *e);
int component_remove(void *components, size_t size, i32 *entity_map, u32 *component_count, const entity *e);
void component_entities_print(void *components, size_t size);

#endif
