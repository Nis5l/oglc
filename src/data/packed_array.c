#include "packed_array.h"

packed_key* packed_array_key(void *elem) {
    return (packed_key*)elem;
}

void packed_array_init(packed_array *pa, void *data, size_t elem_size, uint capacity, int *map) {
	ASSERT(pa, "packed array is null\n");
	ASSERT(data, "packed array data is null\n");
	ASSERT(elem_size > 0, "packed array element size <= 0\n");
	ASSERT(capacity > 0, "packed array capacity <= 0\n");
	ASSERT(map > 0, "packed array map is null\n");

    pa->data = data;
    pa->elem_size = elem_size;
    pa->map = map;
    pa->capacity = capacity;
    pa->count = 0;

    for(uint i = 0; i < capacity; i++) map[i] = -1;
}

void* packed_array_get(packed_array *pa, int id, uint gen) {
	ASSERT(pa, "packed array is null\n");

    if(id < 0 || id >= pa->capacity) return 0;

    int idx = pa->map[id];
    if(idx == -1) return 0;

    packed_key *elem_key = packed_array_key((u8*)pa->data + idx * pa->elem_size);
    if(elem_key->gen != gen) return 0;

    return elem_key;
}

void* packed_array_add(packed_array *pa, int id, uint gen) {
	ASSERT(pa, "packed array is null\n");

    if(id < 0 || id >= pa->capacity) return 0;
    if(pa->map[id] != -1) return 0;
    if(pa->count == pa->capacity) return 0;

    int idx = pa->count++;
    pa->map[id] = idx;

    packed_key *elem = (packed_key*)((u8*)pa->data + idx * pa->elem_size);
    memset(elem, 0, pa->elem_size);
    elem->id = id;
    elem->gen = gen;

    return elem;
}

int packed_array_remove(packed_array *pa, int id, uint gen) {
	ASSERT(pa, "packed array is null\n");

    if(id < 0 || id >= pa->capacity) return 1;

    int idx = pa->map[id];
    if(idx == -1) return 1;

    packed_key *elem_key = packed_array_key((u8*)pa->data + idx * pa->elem_size);
    if(elem_key->gen != gen) return 1;

    int last = pa->count - 1;
    void *dst = (u8*)pa->data + idx * pa->elem_size;
    void *src = (u8*)pa->data + last * pa->elem_size;

    if(idx != last) {
        memcpy(dst, src, pa->elem_size);
        packed_key *moved_key = packed_array_key(src);
        pa->map[moved_key->id] = idx;
    }

    pa->map[id] = -1;
    pa->count--;

    return 0;
}
