#ifndef __INPUT_H__
#define __INPUT_H__

#include <GLFW/glfw3.h>
#include "data/m4.h"

#define KEYS_BUFFER_LENGTH GLFW_KEY_MENU

typedef struct mouse {
	//TODO: probably use vector2 and also for camera get relative pos function
	double mouse_x, mouse_y;
} mouse;

typedef struct input {
	int run_update;
    u8 keys_pressed[KEYS_BUFFER_LENGTH];
	//NOTE: GLFW_KEY_MENU is the last key (348)
    u8 keys_down[KEYS_BUFFER_LENGTH];

	mouse mouse;
} input;

void input_init(input *i);
void input_set_key_pressed(input *i, u8 key);
void input_set_key_released(input *i, u8 key);
u8 input_is_key_down(input *i, u8 key);
u8 input_is_key_pressed(input *i, u8 key);
void input_set_mouse_pos(input *i, double xpos, double ypos);
void input_key_update(input *i);

#endif
