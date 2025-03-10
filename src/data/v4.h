#ifndef __V4_H__
#define __V4_H__

typedef struct m4 m4;
#include "../def.h"

typedef struct v4 {
	f32 x;
	f32 y;
	f32 z;
	f32 w;
} v4;

void v4_clone(v4 *v1, const v4 *v2);
void v4_add(v4 *v1, const v4 *v2);
void v4_mul(v4 *v1, const v4 *v2);
void v4_mul_m4(v4 *v, const m4 *m);
void v4_print(const v4 *v);

#endif
