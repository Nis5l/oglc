#include "m3.h"
#include "../def.h"

int m3_id(m3 *m) {
	if(!m) {
		eprintf("matrix is null\n");
		return 1;
	}

	m->c0r0 = 1;
	m->c0r1 = 0;
	m->c0r2 = 0;

	m->c1r0 = 0;
	m->c1r1 = 1;
	m->c1r2 = 0;

	m->c2r0 = 0;
	m->c2r1 = 0;
	m->c2r2 = 1;

	return 0;
}

int m3_add(m3 *m1, const m3 *m2) {
	if(!m1 || !m2) {
		eprintf("at least one matrix is null\n");
		return 1;
	}
	
	m1->c0r0 += m2->c0r0;
	m1->c0r1 += m2->c0r1;
	m1->c0r2 += m2->c0r2;

	m1->c1r0 += m2->c1r0;
	m1->c1r1 += m2->c1r1;
	m1->c1r2 += m2->c1r2;

	m1->c2r0 += m2->c2r0;
	m1->c2r1 += m2->c2r1;
	m1->c2r2 += m2->c2r2;

	return 0;
}

int m3_mul(m3 *m1, const m3 *m2) {
	if(!m1 || !m2) {
		eprintf("at least one matrix is null\n");
		return 1;
	}

	const i32 c0r0 = m1->c0r0 * m2->c0r0 + m1->c0r1 * m2->c1r0 + m1->c0r2 * m2->c2r0;
	const i32 c0r1 = m1->c0r0 * m2->c0r1 + m1->c0r1 * m2->c1r1 + m1->c0r2 * m2->c2r1;
	const i32 c0r2 = m1->c0r0 * m2->c0r2 + m1->c0r1 * m2->c1r2 + m1->c0r2 * m2->c2r2;

	const i32 c1r0 = m1->c1r0 * m2->c0r0 + m1->c1r1 * m2->c1r0 + m1->c1r2 * m2->c2r0;
	const i32 c1r1 = m1->c1r0 * m2->c0r1 + m1->c1r1 * m2->c1r1 + m1->c1r2 * m2->c2r1;
	const i32 c1r2 = m1->c1r0 * m2->c0r2 + m1->c1r1 * m2->c1r2 + m1->c1r2 * m2->c2r2;

	const i32 c2r0 = m1->c2r0 * m2->c0r0 + m1->c2r1 * m2->c1r0 + m1->c2r2 * m2->c2r0;
	const i32 c2r1 = m1->c2r0 * m2->c0r1 + m1->c2r1 * m2->c1r1 + m1->c2r2 * m2->c2r1;
	const i32 c2r2 = m1->c2r0 * m2->c0r2 + m1->c2r1 * m2->c1r2 + m1->c2r2 * m2->c2r2;

	m1->c0r0 = c0r0;
	m1->c0r1 = c0r1;
	m1->c0r2 = c0r2;

	m1->c1r0 = c1r0;
	m1->c1r1 = c1r1;
	m1->c1r2 = c1r2;

	m1->c2r0 = c2r0;
	m1->c2r1 = c2r1;
	m1->c2r2 = c2r2;

	return 0;
}

int m3_mul_scalar(m3 *m, f32 f) {
	if(!m) {
		eprintf("matrix is null\n");
		return 1;
	}

	m->c0r0 *= f;
	m->c0r1 *= f;
	m->c0r2 *= f;

	m->c1r0 *= f;
	m->c1r1 *= f;
	m->c1r2 *= f;

	m->c2r0 *= f;
	m->c2r1 *= f;
	m->c2r2 *= f;

	return 0;
}

int m3_det(m3 *m, f32 *f_ret) {
	f32 d0 = m->c1r1 * m->c2r2 - m->c2r1 * m->c1r2;
	f32 d1 = m->c1r0 * m->c2r2 - m->c2r0 * m->c1r2;
	f32 d3 = m->c1r0 * m->c2r1 - m->c2r0 * m->c1r1;

	return m->c0r0 * d0 - m->c0r1 * d1 + m->c0r2 * d3;
}

int m3_set_position(m3 *m, const v3 *v) {
	if(!m || !v) {
		eprintf("matrix or vector is null\n");
		return 1;
	}

	m->c0r2 = v->x;
	m->c1r2 = v->y;
	m->c2r2 = v->w;

	return 0;
}

int m3_set_scale(m3 *m, const v3 *v) {
	if(!m || !v) {
		eprintf("matrix or vector is null\n");
		return 1;
	}

	m->c0r0 = v->x;
	m->c1r1 = v->y;
	m->c2r2 = v->w;

	return 0;
}
