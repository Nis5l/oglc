#ifndef __TEXTURE_DATA_H__
#define __TEXTURE_DATA_H__

#include <glad/glad.h>
#include "../def.h"
#include "../ecs/ecs.h"

#define TEXTURE_DATA_LIMIT 128
#define TEXTURE_DATA_ENTITY_LIMIT 128

typedef struct texture_data {
	//TODO: maybe gen
	i32 id;

	GLuint tex;
	u32 width;
	u32 height;

	u32 entity_count;
	i32 entity_map[TEXTURE_DATA_ENTITY_LIMIT];
	entity entities[TEXTURE_DATA_ENTITY_LIMIT];
} texture_data;

void texture_data_init();
i32 texture_data_add(const char *path);
i32 texture_data_remove(i32 id);
i32 texture_data_teardown();
i32 texture_data_register_entity(i32 id, const entity *e);
i32 texture_data_unregister_entity(i32 id, const entity *e);
i32 texture_data_use(i32 md_id);
/* void mesh_draw(window_data *wd);
void mesh_data_print(); */

#endif
