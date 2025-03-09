#ifndef __TIME_H__
#define __TIME_H__

#include <time.h>

typedef struct game_clock {
	struct timespec start, end;
	double dt;
} game_clock;

int game_clock_start(game_clock *clock);
int game_clock_end(game_clock *clock);
int game_clock_init(game_clock *clock);

#endif
