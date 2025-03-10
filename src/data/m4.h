#ifndef __M4_H__
#define __M4_H__

#include "../def.h"
#include "v4.h"

typedef struct m4 {
    f32 c0r0, c0r1, c0r2, c0r3;
    f32 c1r0, c1r1, c1r2, c1r3;
    f32 c2r0, c2r1, c2r2, c2r3;
    f32 c3r0, c3r1, c3r2, c3r3;
} m4;

void m4_id(m4 *m);
void m4_add(m4 *m1, const m4 *m2);
void m4_mul(m4 *m1, const m4 *m2);
void m4_mul_scalar(m4 *m, f32 f);
void m4_set_position(m4 *m, const v4 *v);
void m4_set_scale(m4 *m, const v4 *v);
void m4_det(m4 *m, f32 *f_ret);
void m4_print(const m4 *m);
void m4_ortho(m4 *m, f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
void m4_inv(m4 *out, const m4 *m);
void m4_clone(m4 *m1, const m4 *m2);

#endif
