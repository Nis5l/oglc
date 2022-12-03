#ifndef __V3_H__
#define __V3_H__

#include "../def.h"

struct v3 {
	f32 x;
	f32 y;
	f32 w;
} typedef v3;

int v3_add(v3 *v1, const v3 *v2);
int v3_mul(v3 *v1, const v3 *v2);

#endif
