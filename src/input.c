#include "input.h"

void input_init(input *i) {
	ASSERT(i, "input is null");

	for(int j = 0; j < KEYS_BUFFER_LENGTH; j++) {
		i->keys[j] = 0;
	}
}

void input_set_key_pressed(input *i, u8 key) {
	ASSERT(i, "input is null");

	i->keys[key] = 1;
}

void input_set_key_released(input *i, u8 key) {
	ASSERT(i, "input is null");

	i->keys[key] = 0;
}

u8 input_is_key_pressed(input *i, u8 key) {
	ASSERT(i, "input is null");

	return i->keys[key];
}

void input_set_mouse_pos(input *i, double xpos, double ypos) {
	ASSERT(i, "input is null");

	i->mouse.mouse_x = xpos;
	i->mouse.mouse_y = ypos;
}

mouse input_get_mouse_pos(input *i) {
	ASSERT(i, "input is null");

	return i->mouse;
}
