#include <stdlib.h>
#include <glad/glad.h>
#include "window.h"
#include "def.h"
#include "input.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	ASSERT(window, "window is null");

	window_data* data = (window_data*)glfwGetWindowUserPointer(window);
	dprintf("window resize [%d, %d]\n", width, height);

	data->width = width;
	data->height = height;
	m4_ortho(&data->projection_m, 0.0f, (f32)width, 0.0f, (f32)height, 0.1f, 1000.0f);

	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	ASSERT(window, "window is null");

	window_data* data = (window_data*)glfwGetWindowUserPointer(window);

	if(mods) return;

	switch(action) {
		case GLFW_PRESS: {
			input_set_key_pressed(&data->input, key);
		} break;
		case GLFW_RELEASE: {
			input_set_key_released(&data->input, key);
		} break;
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	ASSERT(window, "window is null");

	window_data* data = (window_data*)glfwGetWindowUserPointer(window);

	input_set_mouse_pos(&data->input, xpos, ypos);
}

void window_process_input(window *w) {
	ASSERT(w, "window is null");
	ASSERT(w->window, "window.window is null");

	if(glfwGetKey(w->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(w->window, 1);
}

int window_init(window *window) {
	ASSERT(window, "window is null\n");

	if(!glfwInit()) {
		eprintf("failed to initialize glfw\n");
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window->window = glfwCreateWindow(800, 600, "oglc", NULL, NULL);
	if(!window->window) {
		eprintf("failed to create glfw window\n");
		glfwTerminate();
		return 2;
	}
	window->data = malloc(sizeof(window_data));
	window->data->camera.id = -1;
	window->data->camera.gen = -1;
	window->data->width = 1920;
	window->data->height = 1080;
	input_init(&window->data->input);

	m4_id(&window->data->projection_m);

	glfwSetWindowUserPointer(window->window, window->data);
	glfwMakeContextCurrent(window->window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		eprintf("failed to initialize glad");
		return 3;
	}
	glViewport(0, 0, window->data->width, window->data->height);
	glfwSetFramebufferSizeCallback(window->window, framebuffer_size_callback);
	glfwSetKeyCallback(window->window, key_callback);
	glfwSetCursorPosCallback(window->window, cursor_position_callback);

	return 0;
}

void window_teardown(window *w) {
	ASSERT(w, "window is null");

	free(w->data);
	glfwDestroyWindow(w->window);
}


void window_swap_buffers(window *w) {
	ASSERT(w, "window is null");

	glfwSwapBuffers(w->window);
}

void window_poll_events() {
	glfwPollEvents();
}

int window_should_close(window *w) {
	ASSERT(w, "window is null");

	return glfwWindowShouldClose(w->window);
}

void window_set_camera(window *w, entity e) {
	ASSERT(w, "window is null");
	ASSERT(e.id != -1, "entity id is -1");
	
	w->data->camera = e;
}
