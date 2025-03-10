#include "m4.h"
#include "../def.h"

void m4_id(m4 *m) {
	ASSERT(m, "matrix is null\n");

	m->c0r0 = 1;
	m->c0r1 = 0;
	m->c0r2 = 0;
	m->c0r3 = 0;

	m->c1r0 = 0;
	m->c1r1 = 1;
	m->c1r2 = 0;
	m->c1r3 = 0;

	m->c2r0 = 0;
	m->c2r1 = 0;
	m->c2r2 = 1;
	m->c2r3 = 0;

	m->c3r0 = 0;
	m->c3r1 = 0;
	m->c3r2 = 0;
	m->c3r3 = 1;
}

void m4_clone(m4 *m1, const m4 *m2) {
	ASSERT(m1, "matrix m1 is null\n");
	ASSERT(m2, "matrix m2 is null\n");

	m1->c0r0 = m2->c0r0;
	m1->c0r1 = m2->c0r1;
	m1->c0r2 = m2->c0r2;
	m1->c0r3 = m2->c0r3;

	m1->c1r0 = m2->c1r0;
	m1->c1r1 = m2->c1r1;
	m1->c1r2 = m2->c1r2;
	m1->c1r3 = m2->c1r3;

	m1->c2r0 = m2->c2r0;
	m1->c2r1 = m2->c2r1;
	m1->c2r2 = m2->c2r2;
	m1->c2r3 = m2->c2r3;

	m1->c3r0 = m2->c3r0;
	m1->c3r1 = m2->c3r1;
	m1->c3r2 = m2->c3r2;
	m1->c3r3 = m2->c3r3;
}

void m4_add(m4 *m1, const m4 *m2) {
	ASSERT(m1, "matrix m1 is null\n");
	ASSERT(m2, "matrix m2 is null\n");
	
	m1->c0r0 += m2->c0r0;
	m1->c0r1 += m2->c0r1;
	m1->c0r2 += m2->c0r2;
	m1->c0r3 += m2->c0r3;

	m1->c1r0 += m2->c1r0;
	m1->c1r1 += m2->c1r1;
	m1->c1r2 += m2->c1r2;
	m1->c1r3 += m2->c1r3;

	m1->c2r0 += m2->c2r0;
	m1->c2r1 += m2->c2r1;
	m1->c2r2 += m2->c2r2;
	m1->c2r3 += m2->c2r3;

	m1->c3r0 += m2->c3r0;
	m1->c3r1 += m2->c3r1;
	m1->c3r2 += m2->c3r2;
	m1->c3r3 += m2->c3r3;
}

void m4_mul(m4 *m1, const m4 *m2) {
	ASSERT(m1, "matrix m1 is null\n");
	ASSERT(m2, "matrix m2 is null\n");

	const f32 c0r0 = m1->c0r0 * m2->c0r0 + m1->c0r1 * m2->c1r0 + m1->c0r2 * m2->c2r0 + m1->c0r3 * m2->c3r0;
	const f32 c0r1 = m1->c0r0 * m2->c0r1 + m1->c0r1 * m2->c1r1 + m1->c0r2 * m2->c2r1 + m1->c0r3 * m2->c3r1;
	const f32 c0r2 = m1->c0r0 * m2->c0r2 + m1->c0r1 * m2->c1r2 + m1->c0r2 * m2->c2r2 + m1->c0r3 * m2->c3r2;
	const f32 c0r3 = m1->c0r0 * m2->c0r3 + m1->c0r1 * m2->c1r3 + m1->c0r2 * m2->c2r3 + m1->c0r3 * m2->c3r3;

	const f32 c1r0 = m1->c1r0 * m2->c0r0 + m1->c1r1 * m2->c1r0 + m1->c1r2 * m2->c2r0 + m1->c1r3 * m2->c3r0;
	const f32 c1r1 = m1->c1r0 * m2->c0r1 + m1->c1r1 * m2->c1r1 + m1->c1r2 * m2->c2r1 + m1->c1r3 * m2->c3r1;
	const f32 c1r2 = m1->c1r0 * m2->c0r2 + m1->c1r1 * m2->c1r2 + m1->c1r2 * m2->c2r2 + m1->c1r3 * m2->c3r2;
	const f32 c1r3 = m1->c1r0 * m2->c0r3 + m1->c1r1 * m2->c1r3 + m1->c1r2 * m2->c2r3 + m1->c1r3 * m2->c3r3;

	const f32 c2r0 = m1->c2r0 * m2->c0r0 + m1->c2r1 * m2->c1r0 + m1->c2r2 * m2->c2r0 + m1->c2r3 * m2->c3r0;
	const f32 c2r1 = m1->c2r0 * m2->c0r1 + m1->c2r1 * m2->c1r1 + m1->c2r2 * m2->c2r1 + m1->c2r3 * m2->c3r1;
	const f32 c2r2 = m1->c2r0 * m2->c0r2 + m1->c2r1 * m2->c1r2 + m1->c2r2 * m2->c2r2 + m1->c2r3 * m2->c3r2;
	const f32 c2r3 = m1->c2r0 * m2->c0r3 + m1->c2r1 * m2->c1r3 + m1->c2r2 * m2->c2r3 + m1->c2r3 * m2->c3r3;

	const f32 c3r0 = m1->c3r0 * m2->c0r0 + m1->c3r1 * m2->c1r0 + m1->c3r2 * m2->c2r0 + m1->c3r3 * m2->c3r0;
	const f32 c3r1 = m1->c3r0 * m2->c0r1 + m1->c3r1 * m2->c1r1 + m1->c3r2 * m2->c2r1 + m1->c3r3 * m2->c3r1;
	const f32 c3r2 = m1->c3r0 * m2->c0r2 + m1->c3r1 * m2->c1r2 + m1->c3r2 * m2->c2r2 + m1->c3r3 * m2->c3r2;
	const f32 c3r3 = m1->c3r0 * m2->c0r3 + m1->c3r1 * m2->c1r3 + m1->c3r2 * m2->c2r3 + m1->c3r3 * m2->c3r3;

	m1->c0r0 = c0r0;
	m1->c0r1 = c0r1;
	m1->c0r2 = c0r2;
	m1->c0r3 = c0r3;

	m1->c1r0 = c1r0;
	m1->c1r1 = c1r1;
	m1->c1r2 = c1r2;
	m1->c1r3 = c1r3;

	m1->c2r0 = c2r0;
	m1->c2r1 = c2r1;
	m1->c2r2 = c2r2;
	m1->c2r3 = c2r3;

	m1->c3r0 = c3r0;
	m1->c3r1 = c3r1;
	m1->c3r2 = c3r2;
	m1->c3r3 = c3r3;
}

void m4_mul_scalar(m4 *m, f32 f) {
	ASSERT(m, "matrix is null\n");

	m->c0r0 *= f;
	m->c0r1 *= f;
	m->c0r2 *= f;
	m->c0r3 *= f;

	m->c1r0 *= f;
	m->c1r1 *= f;
	m->c1r2 *= f;
	m->c1r3 *= f;

	m->c2r0 *= f;
	m->c2r1 *= f;
	m->c2r2 *= f;
	m->c2r3 *= f;

	m->c3r0 *= f;
	m->c3r1 *= f;
	m->c3r2 *= f;
	m->c3r3 *= f;
}

void m4_det(m4 *m, f32 *f_ret) {
	ASSERT(m, "matrix is null\n");
	ASSERT(f_ret, "return value f_ret is null\n");

	// Determinant of 4x4 matrix (using cofactor expansion, for simplicity)
	*f_ret = m->c0r0 * (m->c1r1 * m->c2r2 * m->c3r3 + m->c1r2 * m->c2r3 * m->c3r1 + m->c1r3 * m->c2r1 * m->c3r2 
		- m->c1r3 * m->c2r2 * m->c3r1 - m->c1r2 * m->c2r1 * m->c3r3 - m->c1r1 * m->c2r3 * m->c3r2);
}

void m4_set_position(m4 *m, const v4 *v) {
	ASSERT(m, "matrix is null\n");
	ASSERT(v, "vector is null\n");

	m->c0r3 = v->x;
	m->c1r3 = v->y;
	m->c2r3 = v->z;
	//m->c3r3 = v->w;
}

void m4_set_scale(m4 *m, const v4 *v) {
	ASSERT(m, "matrix is null\n");
	ASSERT(v, "vector is null\n");

	m->c0r0 = v->x;
	m->c1r1 = v->y;
	m->c2r2 = v->z;
	//m->c3r3 = v->w;
}

void m4_print(const m4 *m) {
    ASSERT(m, "matrix is null\n");

    dprintf("m4 [%f, %f, %f, %f]\n", m->c0r0, m->c0r1, m->c0r2, m->c0r3);
    dprintf("   [%f, %f, %f, %f]\n", m->c1r0, m->c1r1, m->c1r2, m->c1r3);
    dprintf("   [%f, %f, %f, %f]\n", m->c2r0, m->c2r1, m->c2r2, m->c2r3);
    dprintf("   [%f, %f, %f, %f]\n", m->c3r0, m->c3r1, m->c3r2, m->c3r3);
}

void m4_ortho(m4 *m, f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far) {
    ASSERT(m, "Matrix is null\n");

    m->c0r0 = 2.0f / (right - left);
    m->c0r1 = 0.0f;
    m->c0r2 = 0.0f;
    m->c0r3 = -(right + left) / (right - left);

    m->c1r0 = 0.0f;
    m->c1r1 = 2.0f / (top - bottom);
    m->c1r2 = 0.0f;
    m->c1r3 = -(top + bottom) / (top - bottom);

    m->c2r0 = 0.0f;
    m->c2r1 = 0.0f;
    m->c2r2 = -2.0f / (far - near);
    m->c2r3 = -(far + near) / (far - near);

    m->c3r0 = 0.0f;
    m->c3r1 = 0.0f;
    m->c3r2 = 0.0f;
    m->c3r3 = 1.0f;
}

void m4_inv(m4 *out, const m4 *m) {
	f32 a2323 = m->c2r2 * m->c3r3 - m->c2r3 * m->c3r2;
	f32 a1323 = m->c2r1 * m->c3r3 - m->c2r3 * m->c3r1;
	f32 a1223 = m->c2r1 * m->c3r2 - m->c2r2 * m->c3r1;
	f32 a0323 = m->c2r0 * m->c3r3 - m->c2r3 * m->c3r0;
	f32 a0223 = m->c2r0 * m->c3r2 - m->c2r2 * m->c3r0;
	f32 a0123 = m->c2r0 * m->c3r1 - m->c2r1 * m->c3r0;
	f32 a2313 = m->c1r2 * m->c3r3 - m->c1r3 * m->c3r2;
	f32 a1313 = m->c1r1 * m->c3r3 - m->c1r3 * m->c3r1;
	f32 a1213 = m->c1r1 * m->c3r2 - m->c1r2 * m->c3r1;
	f32 a2312 = m->c1r2 * m->c2r3 - m->c1r3 * m->c2r2;
	f32 a1312 = m->c1r1 * m->c2r3 - m->c1r3 * m->c2r1;
	f32 a1212 = m->c1r1 * m->c2r2 - m->c1r2 * m->c2r1;
	f32 a0313 = m->c1r0 * m->c3r3 - m->c1r3 * m->c3r0;
	f32 a0213 = m->c1r0 * m->c3r2 - m->c1r2 * m->c3r0;
	f32 a0312 = m->c1r0 * m->c2r3 - m->c1r3 * m->c2r0;
	f32 a0212 = m->c1r0 * m->c2r2 - m->c1r2 * m->c2r0;
	f32 a0113 = m->c1r0 * m->c3r1 - m->c1r1 * m->c3r0;
	f32 a0112 = m->c1r0 * m->c2r1 - m->c1r1 * m->c2r0;

	f32 det = m->c0r0 * ( m->c1r1 * a2323 - m->c1r2 * a1323 + m->c1r3 * a1223 )
		- m->c0r1 * (m->c1r0 * a2323 - m->c1r2 * a0323 + m->c1r3 * a0223)
		+ m->c0r2 * (m->c1r0 * a1323 - m->c1r1 * a0323 + m->c1r3 * a0123)
		- m->c0r3 * (m->c1r0 * a1223 - m->c1r1 * a0223 + m->c1r2 * a0123);
	det = 1 / det;

   out->c0r0 = det *  (m->c1r1 * a2323 - m->c1r2 * a1323 + m->c1r3 * a1223);
   out->c0r1 = det * -(m->c0r1 * a2323 - m->c0r2 * a1323 + m->c0r3 * a1223);
   out->c0r2 = det *  (m->c0r1 * a2313 - m->c0r2 * a1313 + m->c0r3 * a1213);
   out->c0r3 = det * -(m->c0r1 * a2312 - m->c0r2 * a1312 + m->c0r3 * a1212);
   out->c1r0 = det * -(m->c1r0 * a2323 - m->c1r2 * a0323 + m->c1r3 * a0223);
   out->c1r1 = det *  (m->c0r0 * a2323 - m->c0r2 * a0323 + m->c0r3 * a0223);
   out->c1r2 = det * -(m->c0r0 * a2313 - m->c0r2 * a0313 + m->c0r3 * a0213);
   out->c1r3 = det *  (m->c0r0 * a2312 - m->c0r2 * a0312 + m->c0r3 * a0212);
   out->c2r0 = det *  (m->c1r0 * a1323 - m->c1r1 * a0323 + m->c1r3 * a0123);
   out->c2r1 = det * -(m->c0r0 * a1323 - m->c0r1 * a0323 + m->c0r3 * a0123);
   out->c2r2 = det *  (m->c0r0 * a1313 - m->c0r1 * a0313 + m->c0r3 * a0113);
   out->c2r3 = det * -(m->c0r0 * a1312 - m->c0r1 * a0312 + m->c0r3 * a0112);
   out->c3r0 = det * -(m->c1r0 * a1223 - m->c1r1 * a0223 + m->c1r2 * a0123);
   out->c3r1 = det *  (m->c0r0 * a1223 - m->c0r1 * a0223 + m->c0r2 * a0123);
   out->c3r2 = det * -(m->c0r0 * a1213 - m->c0r1 * a0213 + m->c0r2 * a0113);
   out->c3r3 = det *  (m->c0r0 * a1212 - m->c0r1 * a0212 + m->c0r2 * a0112);
}
