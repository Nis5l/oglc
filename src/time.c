#include "time.h"

#include "def.h"

int game_clock_init(game_clock *clock) {
	ASSERT(clock, "clock is null\n");

    if (clock_gettime(CLOCK_MONOTONIC, &clock->start) == -1) {
        eprintf("clock_gettime error\n");
        return 1;
    }
    if (clock_gettime(CLOCK_MONOTONIC, &clock->end) == -1) {
        eprintf("clock_gettime error\n");
        return 2;
    }
    clock->dt = 0;
    return 0;
}

int game_clock_start(game_clock *clock) {
	ASSERT(clock, "clock is null\n");

	if(clock_gettime(CLOCK_MONOTONIC, &clock->start) == -1) {
        eprintf("clock_gettime error\n");
        return 1;
	}

	return 0;
}

int game_clock_end(game_clock *clock) {
	ASSERT(clock, "clock is null\n");

	if(clock_gettime(CLOCK_MONOTONIC, &clock->end) == -1) {
        eprintf("clock_gettime error\n");
        return 1;
	}

    clock->dt = (clock->end.tv_sec - clock->start.tv_sec) + (clock->end.tv_nsec - clock->start.tv_nsec) / 1000000000.0;

	return 0;
}
