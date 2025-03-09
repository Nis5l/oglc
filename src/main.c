#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "def.h"
#include "window.h"
#include "time.h"
#include "mesh_data.h"
#include "ecs/ecs.h"
#include "ecs/components/transform.h"
#include "ecs/components/mesh.h"
#import "input.h"

int create_test_entity(entity *e, i32 mesh_data_id) {
	dprintf("creating entity\n");
	if(entity_create(e)) {
		eprintf("error creating entity\n");
		return 1;
	}

	dprintf("create entity: creating transform component\n");
	if(transform_component_add(e)) {
		eprintf("error adding transform component\n");
		return 1;
	}

	transform *t = 0;
	ASSERT(!transform_component_get(e, &t), "error getting transform id [%d]\n", e->id);

	t->pos.x = 100;
	t->pos.y = 100;
	t->pos.z = -100;
	t->pos.w = 1;
	t->scale.x = 100;
	t->scale.y = 100;
	t->scale.z = 100;
	t->scale.w = 1;

	dprintf("create entity: creating mesh component\n");
	if(mesh_component_add(e, mesh_data_id)) {
		eprintf("error adding mesh component\n");
		return 1;
	}

	return 0;
}

int main() {
	dprintf("initializing opengl\n");
	window window;
	if(window_init(&window)) {
		eprintf("initializing opengl failed\n");
		return 1;
	}

	dprintf("initializing glad\n");
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
		eprintf("failed to initialize GLAD\n");
        return 1;
    }

	dprintf("initializing ecs\n");
	entities_init();
	dprintf("initializing mesh_data\n");
	mesh_data_init();
	dprintf("initializing transform components\n");
	transform_components_init();
	dprintf("initializing mesh components\n");
	mesh_components_init();

	i32 triangle_mesh_id, square_mesh_id;
	{
		dprintf("creating triangle mesh_data\n");
		f32 vertices[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};
		triangle_mesh_id = mesh_data_add(vertices, 3);
		if(triangle_mesh_id == -1) {
			eprintf("error creating mesh_data\n");
			return 1;
		}
	}
	{
		dprintf("creating square mesh_data\n");
		f32 vertices[] = {
			-1.0f, -1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,

			-1.0f, -1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f 
		};
		square_mesh_id = mesh_data_add(vertices, 6);
		if(square_mesh_id == -1) {
			eprintf("error creating mesh_data\n");
			return 1;
		}
	}


	entity e1, e2, e3;
	if(create_test_entity(&e1, triangle_mesh_id)) {
		eprintf("error creating test entity\n");
		return 1;
	}
	if(create_test_entity(&e2, triangle_mesh_id)) {
		eprintf("error creating test entity\n");
		return 1;
	}
	if(create_test_entity(&e3, square_mesh_id)) {
		eprintf("error creating test entity\n");
		return 1;
	}

	transform *t1 = 0, *t2 = 0, *t3 = 0;
	ASSERT(!transform_component_get(&e1, &t1), "error getting transform id [%d]\n", e1.id);
	ASSERT(!transform_component_get(&e2, &t2), "error getting transform id [%d]\n", e2.id);
	ASSERT(!transform_component_get(&e3, &t3), "error getting transform id [%d]\n", e3.id);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	i32 velocity = 100;

	game_clock gc;
	if(game_clock_init(&gc)) {
		eprintf("error initializing game clock\n");
		return 1;
	}
	while(!window_should_close(&window)) {
		ASSERT(!game_clock_start(&gc), "game clock start error\n");
		window_process_input(&window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if(input_is_key_pressed(&window.data->input, GLFW_KEY_W)) {
			t3->pos.y = t3->pos.y + velocity * gc.dt;
		}
		if(input_is_key_pressed(&window.data->input, GLFW_KEY_S)) {
			t3->pos.y = t3->pos.y - velocity * gc.dt;
		}
		if(input_is_key_pressed(&window.data->input, GLFW_KEY_A)) {
			t3->pos.x = t3->pos.x - velocity * gc.dt;
		}
		if(input_is_key_pressed(&window.data->input, GLFW_KEY_D)) {
			t3->pos.x = t3->pos.x + velocity * gc.dt;
		}

		t1->pos.x = t1->pos.x + 100 * gc.dt;
		t1->pos.y = t1->pos.y + 100 * gc.dt;

		t2->pos.x = t2->pos.x + 200 * gc.dt;
		t2->pos.y = t2->pos.y + 100 * gc.dt;

		transform_update_matrices();
		//transform_print_matrix(&e);

		mesh_draw(window.data);

		window_swap_buffers(&window);
		window_poll_events(&window);

		ASSERT(!game_clock_end(&gc), "game clock end error\n");

		//printf("FPS: %.2f\n", 1.0 / gc.dt);
	}


	//TODO: entity teardown
	
	dprintf("tearing down mesh_data\n");
	mesh_data_teardown();

	dprintf("tearing down window\n");
	window_teardown(&window);

	dprintf("terminating glfw\n");
	glfwTerminate();
	return 0;
}
