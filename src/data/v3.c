#include "v3.h"
#include "../def.h"

int v3_add(v3 *v1, const v3 *v2) {
	if(!v1 || !v2) {
		eprintf("at least one vector is null\n");
		return 1;
	}

	//TODO: handle v2.w:
	//set v1.w according to v2.w, ignore it or only set v1 at all if v2.w is set?
	v1->x += v2->x;
	v1->y += v2->y;

	return 0;
}

int v3_mul(v3 *v1, const v3 *v2) {
	if(!v1 || !v2) {
		eprintf("at least one vector is null\n");
		return 1;
	}

	//TODO: handle v2.w:
	//set v1.w according to v2.w, ignore it or only set v1 at all if v2.w is set?
	v1->x += v2->x;
	v1->y += v2->y;

	return 0;
}
