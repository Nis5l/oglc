#ifndef __PACKED_ARRAY_H__
#define __PACKED_ARRAY_H__

#include <string.h>
#include "../def.h"

typedef struct packed_key {
    int id;
    uint gen;
} packed_key;

typedef int (*packed_array_cmp_fn)(const void *a, const void *b);

typedef struct packed_array {
    void   *data;
    size_t  elem_size;

    int *map;
    uint count;
    uint capacity;

	//TODO: use reverse index map for this also to keep performance, code gets disgusting, but just never touch it again.
	packed_array_cmp_fn cmp;
	int *sorted_index;
} packed_array;

void packed_array_init(packed_array *pa, void *data, size_t elem_size, uint capacity, int *map, int *sorted_index, packed_array_cmp_fn cmp);
void* packed_array_get(packed_array *pa, int id, uint gen);
void* packed_array_add(packed_array *pa, int id, uint gen);
int packed_array_remove(packed_array *pa, int id, uint gen);
packed_key* packed_array_key(void *elem);
void* packed_array_get_sorted(packed_array *pa, uint idx);

#endif
