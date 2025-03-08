#include <stdlib.h>
#include <glad/glad.h>
#include "window.h"
#include "def.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	ASSERT(window, "window is null");

	window_data* data = (window_data*)glfwGetWindowUserPointer(window);
	dprintf("%d %d\n", width, height);
	m4_ortho(&data->projection_m, 0.0f, (f32)width, 0.0f, (f32)height, 0.1f, 100.0f);
	//m4_ortho(&data->projection_m, -1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

	glViewport(0, 0, width, height);
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
	glfwSetWindowUserPointer(window->window, window->data);
	glfwMakeContextCurrent(window->window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		eprintf("failed to initialize glad");
		return 3;
	}
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window->window, framebuffer_size_callback);

	return 0;
}

void window_teardown(window *window) {
	free(window->data);
	glfwDestroyWindow(window->window);
}
