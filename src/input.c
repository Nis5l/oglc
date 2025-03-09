#import "input.h"

void input_set_key_pressed(input *i, u8 key) {
	i->input[key] = 1;
}

void input_set_key_released(input *i, u8 key) {
	i->input[key] = 0;
}

u8 input_is_key_pressed(input *i, u8 key) {
	return i->input[key];
}
