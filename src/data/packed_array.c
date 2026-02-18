#include "packed_array.h"

packed_key* packed_array_key(void *elem) {
    return (packed_key*)elem;
}


void packed_array_init(packed_array *pa, void *data, size_t elem_size, uint capacity, int *map, int *sorted_index, packed_array_cmp_fn cmp) {
	ASSERT(pa, "packed array is null\n");
	ASSERT(data, "packed array data is null\n");
	ASSERT(elem_size > 0, "packed array element size <= 0\n");
	ASSERT(capacity > 0, "packed array capacity <= 0\n");
	ASSERT(map > 0, "packed array map is null\n");
	ASSERT((sorted_index && cmp) || (!sorted_index && !cmp), "packed array sorting only partially set\n");

    pa->data = data;
    pa->elem_size = elem_size;
    pa->map = map;
    pa->capacity = capacity;
    pa->count = 0;

    pa->sorted_index = sorted_index;
    pa->cmp = cmp;

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

static void insert_sorted_index(packed_array *pa, int idx) {
    if (!pa->sorted_index || !pa->cmp) return;

    void *new_elem = (u8*)pa->data + idx * pa->elem_size;
    uint left = 0;
    uint right = pa->count - 1;
    uint pos = pa->count - 1;

    while (left <= right) {
        uint mid = left + (right - left) / 2;
        void *mid_elem = (u8*)pa->data + pa->sorted_index[mid] * pa->elem_size;
        if (pa->cmp(new_elem, mid_elem) < 0) {
            if (mid == 0) {
                pos = 0;
                break;
            }
            right = mid - 1;
            pos = mid;
        } else {
            left = mid + 1;
        }
    }

    for (uint j = pa->count - 1; j > pos; j--) {
        pa->sorted_index[j] = pa->sorted_index[j - 1];
    }

    pa->sorted_index[pos] = idx;
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

	insert_sorted_index(pa, idx);

    return elem;
}

int packed_array_remove(packed_array *pa, int id, uint gen) {
    if (id < 0 || id >= (int)pa->capacity) return 1;

    int idx = pa->map[id];
    if (idx == -1) return 1;

    packed_key *elem = packed_array_key((u8*)pa->data + idx * pa->elem_size);
    if (elem->gen != gen) return 1;

    int last = pa->count - 1;

    if (idx != last) {
        void *dst = (u8*)pa->data + idx * pa->elem_size;
        void *src = (u8*)pa->data + last * pa->elem_size;
        memcpy(dst, src, pa->elem_size);

        packed_key *moved = packed_array_key(dst);
        pa->map[moved->id] = idx;

        if (pa->sorted_index) {
            for (uint i = 0; i < pa->count; i++) {
                if (pa->sorted_index[i] == last) {
                    pa->sorted_index[i] = idx;
                    break;
                }
            }
        }
    }

    pa->map[id] = -1;

    if (pa->sorted_index) {
        uint found = pa->count;
        for (uint i = 0; i < pa->count; i++) {
            if (pa->sorted_index[i] == last || pa->sorted_index[i] == idx) {
                found = i;
                break;
            }
        }
        if (found < pa->count) {
            for (uint i = found; i < pa->count - 1; i++)
                pa->sorted_index[i] = pa->sorted_index[i + 1];
        }
    }

    pa->count--;
    return 0;
}

void* packed_array_get_sorted(packed_array *pa, uint idx) {
    if (idx >= pa->count) return 0;

    if (!pa->sorted_index) {
        return (u8*)pa->data + idx * pa->elem_size;
    }

    return (u8*)pa->data + pa->sorted_index[idx] * pa->elem_size;
}
