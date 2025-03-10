#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <GLFW/glfw3.h>
#include "data/m4.h"
#include "input.h"
#include "ecs/ecs.h"

typedef struct window_data {
    m4 projection_m;
	input input;
	entity camera;
	i32 width;
	i32 height;
} window_data;

typedef struct window {
    GLFWwindow *window;
    window_data *data;
} window;

int window_init(window *window);
void window_process_input(window *w);
void window_teardown(window *window);
void window_swap_buffers(window *w);
void window_poll_events();
int window_should_close(window *w);
void window_set_camera(window *w, entity e);

#endif
