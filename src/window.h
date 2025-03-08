#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <GLFW/glfw3.h>
#include "data/m4.h"

typedef struct window_data {
    m4 projection_m;
} window_data;

typedef struct window {
    GLFWwindow *window;
    window_data *data;
} window;

int window_init(window *window);
void window_process_input(window *w);
void window_teardown(window *window);

#endif
