#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "def.h"
#include "window.h"
#include "time.h"
#include "render/mesh_data.h"
#include "render/texture_data.h"
#include "render/render_batch.h"
#include "ecs/ecs.h"
#include "ecs/components/transform.h"
#include "ecs/components/mesh.h"
#include "ecs/components/camera.h"
#include "ecs/components/texture.h"
#include "input.h"

int create_test_entity(entity *e, i32 md_id, i32 td_id) {
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
	if(mesh_component_add(e, md_id)) {
		eprintf("error adding mesh component\n");
		return 1;
	}

	dprintf("create entity: creating texture component\n");
	if(texture_component_add(e, td_id)) {
		eprintf("error adding texture component\n");
		return 1;
	}
	dprintf("TESTXX\n");

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
	dprintf("initializing texture_data\n");
	texture_data_init();
	dprintf("initializing render batches\n");
	render_batch_init();
	dprintf("initializing transform components\n");
	transform_components_init();
	dprintf("initializing mesh components\n");
	mesh_components_init();
	dprintf("initializing camera components\n");
	camera_components_init();
	dprintf("initializing texture components\n");
	texture_components_init();

	//i32 triangle_mesh_id, square_mesh_id;
	i32 square_mesh_id;
	/* {
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
	} */
	{
		dprintf("creating square mesh_data\n");
		/* f32 vertices[] = {
			-1.0f, -1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,

			-1.0f, -1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f 
		}; */

		f32 vertices[] = {
		//   x    y     z     u     v
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,

			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f
		};
		square_mesh_id = mesh_data_add(vertices, 6);
		if(square_mesh_id == -1) {
			eprintf("error creating mesh_data\n");
			return 1;
		}
	}

	int ball_tex_id = texture_data_add("./resources/ball.png");
	if(ball_tex_id < 0) {
		eprintf("error adding texture\n");
		return 1;
	}

	dprintf("TEST1\n");
	entity e1, mouse_cursor_entity, player_entity;
	if(create_test_entity(&e1, square_mesh_id, ball_tex_id)) {
		eprintf("error creating test entity\n");
		return 1;
	}
	dprintf("TEST2\n");
	if(create_test_entity(&mouse_cursor_entity, square_mesh_id, ball_tex_id)) {
		eprintf("error creating mouse cursor entity\n");
		return 1;
	}
	if(create_test_entity(&player_entity, square_mesh_id, ball_tex_id)) {
		eprintf("error creating player entity\n");
		return 1;
	}

	if(camera_component_add(&player_entity)) {
		eprintf("error adding camera\n");
		return 1;
	}
	window_set_camera(&window, player_entity);

	transform *player_transform = 0, *mouse_cursor_transform = 0;
	ASSERT(!transform_component_get(&player_entity, &player_transform), "error getting transform for id [%d]\n", player_entity.id);
	ASSERT(!transform_component_get(&mouse_cursor_entity, &mouse_cursor_transform), "error getting transform for id [%d]\n", mouse_cursor_entity.id);
	mouse_cursor_transform->scale.x = 10;
	mouse_cursor_transform->scale.y = 10;
	mouse_cursor_transform->scale.z = 10;
	player_transform->scale.x = 50;
	player_transform->scale.y = 50;
	player_transform->scale.z = 50;

	camera *c;
	camera_component_get(&player_entity, &c);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	i32 velocity = 1000;
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

		if(input_is_key_pressed(&window.data->input, GLFW_KEY_Q)) {
			player_transform->pos.z = player_transform->pos.z - velocity * gc.dt;
		}
		if(input_is_key_pressed(&window.data->input, GLFW_KEY_E)) {
			player_transform->pos.z = player_transform->pos.z + velocity * gc.dt;
		}
		if(input_is_key_pressed(&window.data->input, GLFW_KEY_W)) {
			player_transform->pos.y = player_transform->pos.y + velocity * gc.dt;
		}
		if(input_is_key_pressed(&window.data->input, GLFW_KEY_S)) {
			player_transform->pos.y = player_transform->pos.y - velocity * gc.dt;
		}
		if(input_is_key_pressed(&window.data->input, GLFW_KEY_A)) {
			player_transform->pos.x = player_transform->pos.x - velocity * gc.dt;
		}
		if(input_is_key_pressed(&window.data->input, GLFW_KEY_D)) {
			player_transform->pos.x = player_transform->pos.x + velocity * gc.dt;
		}

		camera_update_pos(window.data->width, window.data->height);

		mouse cursor_pos = camera_get_relative_mouse_pos(&player_entity, &window.data->input.mouse, window.data->width, window.data->height);
		mouse_cursor_transform->pos.x = cursor_pos.mouse_x;
		mouse_cursor_transform->pos.y = cursor_pos.mouse_y;

		transform_update_matrices();
		camera_update_matrices();
		//transform_print_matrix(&e);

		//mesh_draw(window.data);
		render_batch_draw(window.data);

		window_swap_buffers(&window);
		window_poll_events();

		ASSERT(!game_clock_end(&gc), "game clock end error\n");

		//printf("FPS: %.2f\n", 1.0 / gc.dt);
	}


	//TODO: entity teardown
	
	dprintf("tearing down mesh_data\n");
	mesh_data_teardown();

	dprintf("tearing down texture_data\n");
	texture_data_teardown();

	dprintf("tearing down render batch\n");
	render_batch_teardown();

	dprintf("tearing down window\n");
	window_teardown(&window);

	dprintf("terminating glfw\n");
	glfwTerminate();
	return 0;
}
