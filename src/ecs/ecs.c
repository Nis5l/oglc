#include <stdlib.h>
#include "ecs.h"
#include "components/transform.h"
#include "components/mesh.h"

u32 gen_counter = 0;
entity entities[ENTITY_LIMIT];

void entities_init() {
	for(int i = 0; i < ENTITY_LIMIT; i++) {
		entities[i].id = -1;
		entities[i].gen = 0;
	}
}

int entity_create(entity *e) {
	ASSERT(e, "entity is null");
	//ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

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

int entity_delete(const entity *e) {
	ASSERT(e, "entity is null");
	ASSERT(e->id >= 0 && e->id < ENTITY_LIMIT, "id [%d] not in range(0,%d)\n", e->id, ENTITY_LIMIT);

	if(entities[e->id].id != e->id || entities[e->id].gen != e->gen) {
		eprintf("no entity with id or gen doesnt match [%d]\n", e->id);
		return 1;
	}

	//TODO: err when there really is none
	int component_delete_err = 0;
	component_delete_err += transform_component_remove(e);
	component_delete_err += mesh_component_remove(e);

	entities[e->id].id = -1;
	entities[e->id].gen = 0;

	if(component_delete_err) {
		eprintf("error deleting componenents for entity [%d]\n", e->id);
		return 2;
	}
	return 0;
}
