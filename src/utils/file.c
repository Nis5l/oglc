#include <stdio.h>
#include <stdlib.h>

#include "../def.h"
#include "./file.h"

char* read_file_text(const char *path) {
    FILE *f = fopen(path, "rb");
    if(!f) {
        eprintf("failed to open file [%s]\n", path);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *data = malloc(size + 1);
    if(!data) {
        fclose(f);
        eprintf("malloc failed for file [%s]\n", path);
        return NULL;
    }

    fread(data, 1, size, f);
    data[size] = '\0';

    fclose(f);
    return data;
}
