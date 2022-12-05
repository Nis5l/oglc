#ifndef __M3_H__
#define __M3_H__

#include "../def.h"
#include "v3.h"

struct m3 {
	i32 c0r0, c0r1, c0r2;
	i32 c1r0, c1r1, c1r2;
	i32 c2r0, c2r1, c2r2;
} typedef m3;

int m3_id(m3 *m);
int m3_add(m3 *m1, const m3 *m2);
int m3_mul(m3 *m1, const m3 *m2);
int m3_mul_scalar(m3 *m, f32 f);
int m3_set_position(m3 *m, const v3 *v);
int m3_set_scale(m3 *m, const v3 *v);

#endif
