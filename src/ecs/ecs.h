#ifndef __ENTITY_H__
#define __ENTITY_H__
#define ENTITY_LIMIT 1028

#include "../def.h"

struct entity {
	i32 id;
	u32 gen;
} typedef entity;


void entities_init();
int entity_create(entity *e);
int entity_delete(i32 id);

#endif
