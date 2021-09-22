#ifndef HASH_MAP_H
#define HASH_MAP_H

typedef void* hash_map_t;

hash_map_t new_hash_map(int initial_size);

void free_hash_map(hash_map_t hash_map);

char *add_name_to_hash_map(hash_map_t hash_map, char *name);

#endif
