#include "v4.h"
#include "m4.h"

void v4_add(v4 *v1, const v4 *v2) {
	ASSERT(v1, "vector v1 is null\n");
	ASSERT(v2, "vector v2 is null\n");

	//TODO: handle v2.w:
	//set v1.w according to v2.w, ignore it or only set v1 at all if v2.w is set?
	v1->x += v2->x;
	v1->y += v2->y;
	v1->z += v2->z;
}

void v4_mul(v4 *v1, const v4 *v2) {
	ASSERT(v1, "vector v1 is null\n");
	ASSERT(v2, "vector v2 is null\n");

	//TODO: handle v2.w:
	//set v1.w according to v2.w, ignore it or only set v1 at all if v2.w is set?
	v1->x *= v2->x;
	v1->y *= v2->y;
	v1->z *= v2->z;
}

void v4_mul_m4(v4 *v, const m4 *m) {
    ASSERT(v, "vector v is null\n");
    ASSERT(m, "matrix m is null\n");

    v4 result;
    result.x = v->x * m->c0r0 + v->y * m->c0r1 + v->z * m->c0r2 + v->w * m->c0r3;
    result.y = v->x * m->c1r0 + v->y * m->c1r1 + v->z * m->c1r2 + v->w * m->c1r3;
    result.z = v->x * m->c2r0 + v->y * m->c2r1 + v->z * m->c2r2 + v->w * m->c2r3;
    result.w = v->x * m->c3r0 + v->y * m->c3r1 + v->z * m->c3r2 + v->w * m->c3r3;

    *v = result;
}

void v4_print(const v4 *v) {
    ASSERT(v, "vector is null\n");

    dprintf("v4 [%f, %f, %f, %f]\n", v->x, v->y, v->z, v->w);
}
