#include <stdlib.h>
#include <string.h>
#include "component.h"
#include "../../../def.h"

void components_init(void *components, size_t size, i32 *entity_map) {
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
		eprintf("entity with id [%d] not found", e->id);
		return 1;
	}

	entity *ec = (entity*)((u8*)components + size * i);

	if(ec->gen != e->gen) {
		eprintf("entity generations [%d] [%d] do not match", e->gen, ec->gen);
		return 2;
	}

	*t_ret = ec;

	return 0;
}

int component_add(void *components, size_t size, i32 *entity_map, const entity *e) {
	ASSERT(e, "entity is null\n");

	if(entity_map[e->id] != -1) {
		eprintf("entity with id [%d] already has component\n", e->id);
		return 1;
	}

	for(int i = 0; i < ENTITY_LIMIT; i++) {
		entity *ce = (entity*)((u8*)components + i * size);
		if(ce->id == -1) {
			ce->id = e->id;
			ce->gen = e->gen;
			entity_map[e->id] = i;

			return 0;
		}
	}
	eprintf("component limit reached, this should never happen!\n");
	return 2;
}

int component_remove(void *components, size_t size, i32 *entity_map, const entity *e) {
	ASSERT(e, "entity is null");
	ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

	for(int i = 0; i < ENTITY_LIMIT; i++) {
		entity *e_s = (entity*)((u8*)components + i * size);
		if(e_s->id == -1) {
			eprintf("component with entity id [%d] gen [%d] not found\n", e->id, e->gen);
			return 1;
		}

		if(e_s->id == e->id && e_s->gen == e->gen) {
			entity *last_component = 0;
			for(int j = i + 1; j < ENTITY_LIMIT; j++) {
				entity *lc = (entity*)((u8*)components + j * size);
				if(lc->id == -1) break;
				last_component = lc;
			}
			if(last_component) {
				memcpy(e_s, last_component, size);
			} else {
				last_component = e_s;
			}

			last_component->id = -1;
			last_component->gen = -1;

			entity_map[e_s->id] = -1;
			return 0;
		}
	}

	return 1;
}
