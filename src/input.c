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
