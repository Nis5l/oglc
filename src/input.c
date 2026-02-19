#include "input.h"

void input_init(input *i) {
	ASSERT(i, "input is null");

	for(int j = 0; j < KEYS_BUFFER_LENGTH; j++) {
		i->keys_down[j] = 0;
		i->keys_pressed[j] = 0;
	}
}

void input_set_key_pressed(input *i, u8 key) {
	ASSERT(i, "input is null");

	i->keys_pressed[key] = 1;
	i->keys_down[key] = 1;
	i->run_update = 1;
}

void input_set_key_released(input *i, u8 key) {
	ASSERT(i, "input is null");

	i->keys_down[key] = 0;
}

void input_key_update(input *i) {
	if(!i->run_update) return;

	for(int j = 0; j < KEYS_BUFFER_LENGTH; j++)
		if(i->keys_pressed[j]) i->keys_pressed[j] = 0;

	i->run_update = 0;
}

u8 input_is_key_pressed(input *i, u8 key) {
	ASSERT(i, "input is null");

	//NOTE: idk, this is all retarded, maybe one day...
	return i->keys_pressed[key];
}

u8 input_is_key_down(input *i, u8 key) {
	ASSERT(i, "input is null");

	return i->keys_down[key];
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
