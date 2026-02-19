#ifndef __ENTITY_H__
#define __ENTITY_H__
#define ENTITY_LIMIT 1028

#include "../def.h"

typedef struct entity {
	int id;
	uint gen;
} entity;


void entities_init();
int entity_create(entity *e);
int entity_delete(const entity *e);
int entity_teardown();

#endif
