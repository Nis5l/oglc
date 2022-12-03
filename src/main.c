#include <stdio.h>
#include "def.h"
#include "ecs/ecs.h"
#include "ecs/components/transform.h"

int main(int argc, char **argv) {
	dprintf("initializing ecs\n");
	entities_init();
	transforms_init();

	entity e;
	entity_create(&e);
	transform_component_add(&e);

	return 0;
}
