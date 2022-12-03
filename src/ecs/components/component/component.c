#include <stdlib.h>
#include "component.h"
#include "../../../def.h"

void components_init(void *components, size_t size, i32 *entity_map) {
	for(int i = 0; i < ENTITY_LIMIT; i++) {
		entity *e = (entity*)(components + i * size);
		e->id = -1;
		entity_map[i] = -1;
	}
}

int component_get(void *components, size_t size, i32 *entity_map, const entity *e, void **t_ret) {
	if(!e) {
		printf("entity is null\n");
		return 1;
	}
	if(!t_ret) {
		printf("component return is null\n");
		return 1;
	}

	int i = entity_map[e->id];
	if(i == -1) { 
		eprintf("entity with id [%d] not found", e->id);
		return 2;
	}

	entity *ec = (entity*)(components + size * i);

	if(ec->gen != e->gen) {
		eprintf("entity generations [%d] [%d] do not match", e->gen, ec->gen);
		return 3;
	}

	*t_ret = ec;

	return 0;
}

int component_add(void *components, size_t size, i32 *entity_map, const entity *e) {
	if(!e) {
		eprintf("entity is null\n");
		return 1;
	}

	if(entity_map[e->id] != -1) {
		eprintf("entity with id [%d] already has component\n", e->id);
		return 2;
	}

	for(int i = 0; i < ENTITY_LIMIT; i++) {
		entity *ce = (entity*)(components + i * size);
		if(ce->id == -1) {
			ce->id = e->id;
			ce->gen = e->gen;
			entity_map[e->id] = i;

			return 0;
		}
	}
	eprintf("component limit reached, this should never happen!\n");
	return 3;
}

int component_remove(void *components, size_t size, i32 *entity_map, i32 id) {
	if(id < 0 || id >= ENTITY_LIMIT) {
		eprintf("id [%d] not in range(0,%d)\n", id, ENTITY_LIMIT);
		return 1;
	}

	entity *e = (entity*)(components + id * size);

	if(e->id == -1) {
		eprintf("no component for entity with id [%d]\n", id);
		return 2;
	}

	e->id = -1;
	entity_map[id] = -1;

	return 0;
}
