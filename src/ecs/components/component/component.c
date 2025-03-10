#include <stdlib.h>
#include <string.h>
#include "component.h"
#include "../../../def.h"

void components_init(void *components, size_t size, i32 *entity_map, u32 *component_count) {
	*component_count = 0;

	for(int i = 0; i < ENTITY_LIMIT; i++) {
		entity *e = (entity*)((u8*)components + i * size);
		e->id = -1;
		entity_map[i] = -1;
	}
}

int component_get(void *components, size_t size, i32 *entity_map, const entity *e, void **t_ret) {
	ASSERT(e, "entity is null\n");
	ASSERT(t_ret, "component return is null\n");

	int i = entity_map[e->id];
	if(i == -1) { 
		eprintf("entity with id [%d] not found\n", e->id);
		return 1;
	}

	entity *ec = (entity*)((u8*)components + size * i);

	if(ec->gen != e->gen) {
		eprintf("entity generations [%d %d] [%d %d] do not match\n", e->id, e->gen, ec->id, ec->gen);
		return 2;
	}

	*t_ret = ec;

	return 0;
}

int component_add(void *components, size_t size, i32 *entity_map, u32 *component_count, const entity *e) {
	ASSERT(e, "entity is null\n");

	if(entity_map[e->id] != -1) {
		eprintf("entity with id [%d] already has component\n", e->id);
		return 1;
	}

	if(*component_count == ENTITY_LIMIT) {
		eprintf("component limit reached, this should never happen!\n");
		return 2;
	}

	int idx = *component_count;
	entity *ce = (entity*)((u8*)components + idx * size);
	ASSERT(ce->id == -1, "entity id is not -1 (%d)\n", ce->id);
	ce->id = e->id;
	ce->gen = e->gen;
	entity_map[e->id] = idx;

	(*component_count)++;

	return 0;
}

int component_remove(void *components, size_t size, i32 *entity_map, u32 *component_count, const entity *e) {
	ASSERT(e, "entity is null");
	ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

	int idx = entity_map[e->id];
	if(idx == -1) {
		eprintf("component with entity id [%d] not found\n", e->id);
		return 1;
	}

	entity *e_s = (entity*)((u8*)components + idx * size);
	if(e_s->id != e->id || e_s->gen != e->gen) {
		eprintf("component with entity id [%d] gen [%d] not found\n", e->id, e->gen);
		return 1;
	}

	entity_map[e_s->id] = -1;

	entity *component_delete = e_s;
	if(*component_count > 1 && *component_count != idx + 1) {
		int i = (*component_count - 1);
		component_delete = (entity*)((u8*)components + i * size);
		ASSERT(component_delete->id != -1, "last component does not exist\n");

		entity_map[component_delete->id] = idx;
		memcpy(e_s, component_delete, size);
	}

	component_delete->id = -1;
	component_delete->gen = -1;

	(*component_count)--;

	return 0;
}

void component_entities_print(void *components, size_t size) {
	dprintf("components:");
	for(int i = 0; i < ENTITY_LIMIT; i++) {
		entity *e = (entity*)((u8*)components + i * size);
		if(e->id == -1) break;
		//TODO: function without format
		printf(" [%d, %d]", e->id, e->gen);
	}
	printf("\n");
}
