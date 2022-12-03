#include <stdlib.h>
#include "ecs.h"
#include "components/transform.h"

u32 gen_counter = 0;
entity entities[ENTITY_LIMIT];

void entities_init() {
	for(int i = 0; i < ENTITY_LIMIT; i++) {
		entities[i].id = -1;
		entities[i].gen = 0;
	}
}

int entity_create(entity *e) {
	for(int i = 0; i < ENTITY_LIMIT; i++) {
		if(entities[i].id == -1) {
			entities[i].gen = e->gen = gen_counter++;
			entities[i].id = e->id = i;
			return 0;
		}
	}

	eprintf("entity limit reached (%d)\n", ENTITY_LIMIT);
	return 1;
}

int entity_delete(i32 id) {
	if(id < 0 || id >= ENTITY_LIMIT) {
		eprintf("id [%d] not in range(0,%d)\n", id, ENTITY_LIMIT);
		return 1;
	}
	if(entities[id].id == -1) {
		eprintf("no entity with id [%d]\n", id);
		return 2;
	}

	int component_delete_err = 0;
	component_delete_err += transform_component_remove(id);

	entities[id].id = -1;
	entities[id].gen = 0;

	if(component_delete_err) {
		eprintf("error deleting componenents for entity [%d]\n", id);
		return 3;
	}
	return 0;
}
