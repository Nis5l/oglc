#ifndef __RENDER_BATCH_H__
#define __RENDER_BATCH_H__

//#include <glad/glad.h>
#include "../def.h"
#include "../ecs/ecs.h"
#include "../window.h"

#define RENDER_BATCH_LIMIT 128
#define RENDER_BATCH_ENTITY_LIMIT 128

typedef struct render_batch {
	//TODO: maybe gen
	i32 id;
	
	i32 md_id;
	i32 td_id;

	u32 entity_count;
	i32 entity_map[RENDER_BATCH_ENTITY_LIMIT];
	entity entities[RENDER_BATCH_ENTITY_LIMIT];
} render_batch;

void render_batch_init();
i32 render_batch_check_add(const entity *e);
i32 render_batch_check_remove(const entity *e);
i32 render_batch_teardown();
void render_batch_draw(window_data *wd);

#endif
