#ifndef __RENDER_BATCH_H__
#define __RENDER_BATCH_H__

#include "../def.h"
#include "../ecs/ecs.h"
#include "./mesh_data.h"
#include "./texture_data.h"
#include "../window.h"

#define RENDER_BATCH_LIMIT ENTITY_LIMIT
#define RENDER_BATCH_ENTITY_LIMIT ENTITY_LIMIT

typedef struct render_batch_key {
    int id;
    uint gen;
} render_batch_key;

typedef struct render_batch {
	render_batch_key key;
	
	mesh_data_key md_key;
	texture_data_key td_key;

	packed_array entities_pa;
	entity entities[RENDER_BATCH_ENTITY_LIMIT];
	int entity_map[RENDER_BATCH_ENTITY_LIMIT];
} render_batch;

void render_batch_init();
int render_batch_check_add(const entity *e);
int render_batch_check_remove(const entity *e);
int render_batch_teardown();
void render_batch_draw(window_data *wd);

#endif
