#ifndef HASH_MAP_H
#define HASH_MAP_H

typedef struct
{
    int number_of_names;
    int number_of_slots;
    char **slots;
} hash_map_t;

bool init_hash_map(hash_map_t *hash_map);

char *add_name_to_hash_map(hash_map_t *hash_map, char *name);

#endif
