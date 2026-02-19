#ifndef __SHADER_H__
#define __SHADER_H__

#include "../ecs.h"
#include "../../data/rgba.h"
#include "../../render/shader_data.h"

typedef struct shader {
	entity e;

	shader_data_key sd_key;
	rgba color;
} shader;

void shader_components_init();
int shader_component_get(const entity *e, shader **m_ret);
int shader_component_add(const entity *e, shader_data_key md_key);
int shader_component_remove(const entity *e);

#endif
