#include "stb_image.h"

#include "./texture_data.h"
#include "../ecs/components/texture.h"
#include "../gen.h"

packed_array texture_datas_pa;
texture_data texture_datas[TEXTURE_DATA_LIMIT] = {0};
i32 texture_data_map[TEXTURE_DATA_LIMIT] = {0};

void texture_data_init() {
    packed_array_init(&texture_datas_pa, texture_datas, sizeof(texture_data), TEXTURE_DATA_LIMIT, texture_data_map);
}

i32 texture_data_add(const char *path, texture_data_key *key) {
	ASSERT(path, "path is null\n");

	if(texture_datas_pa.count == TEXTURE_DATA_LIMIT) {
		eprintf("texture data is full\n");
		return 1;
	}

	texture_data *td = packed_array_add(&texture_datas_pa, texture_datas_pa.count, gen_counter++);
	if(!td) {
		eprintf("texture data packed array add failed\n");
		return 2;
	}

	packed_array_init(&td->entities_pa, td->entities, sizeof(entity), TEXTURE_DATA_ENTITY_LIMIT, td->entity_map);

	int width, height, channels;
	unsigned char *data = stbi_load(path, &width, &height, &channels, 4);
	if(!data) {
		eprintf("texture data error loading image %s\n", path);
		return 3;
	}

	glGenTextures(1, &td->tex);
	glBindTexture(GL_TEXTURE_2D, td->tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    td->width = width;
    td->height = height;
	dprintf("registered texture data [path:%s, id:%d, gen:%d, widht:%d, height:%d]\n", path, td->key.id, td->key.gen, width, height);

	*key = td->key;
	return 0;
}

i32 texture_data_remove(texture_data_key key) {
	ASSERT(key.id >= 0 && key.id < TEXTURE_DATA_LIMIT, "id [%d] not in range(0,%d)\n", key.id, TEXTURE_DATA_LIMIT);

	texture_data *td = packed_array_get(&texture_datas_pa, key.id, key.gen);
	if(!td) {
		eprintf("no texture_data with [id:%d, gen:%d]\n", key.id, key.gen);
		return 1;
	}

	glDeleteTextures(1, &td->tex); 

	i32 err = 0;
	//NOTE: deleted like this because the meshes unregister themseleves when removed
	while(td->entities_pa.count > 0) err += texture_component_remove(td->entities + td->entities_pa.count - 1);

	if(packed_array_remove(&texture_datas_pa, key.id, key.gen)) {
		eprintf("failed to remove texture_data [id:%d, gen:%d]\n", key.id, key.gen);
		return 1;
	}

	return 0;
}

i32 texture_data_register_entity(texture_data_key key, const entity *e) {
	texture_data *td = packed_array_get(&texture_datas_pa, key.id, key.gen);
	if(!td) return 1;

	if(!packed_array_add(&td->entities_pa, e->id, e->gen)) {
		eprintf("texture_data entities full or already contains entity [%d]\n", e->id);
		return 1;
	}

	return 0;
}

i32 texture_data_unregister_entity(texture_data_key key, const entity *e) {
	texture_data *td = packed_array_get(&texture_datas_pa, key.id, key.gen);
	if(!td) return 1;

	return packed_array_remove(&td->entities_pa, e->id, e->gen);
}

i32 texture_data_use(texture_data_key key) {
	texture_data *td = packed_array_get(&texture_datas_pa, key.id, key.gen);
	if(!td) return 1;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, td->tex);
	return 0;
}

i32 texture_data_teardown() {
	i32 err = 0;
	while(texture_datas_pa.count > 0) err += texture_data_remove(texture_datas[texture_datas_pa.count - 1].key);
	return err;
}
