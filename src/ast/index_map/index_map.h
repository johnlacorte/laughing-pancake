#ifndef INDEX_MAP_H
#define INDEX_MAP_H

typedef enum
{
    FIXED_SIZE,
    EXPANDS_AS_NEEDED
}index_map_type_t;

typedef struct
{
    index_map_type_t type;
    int current_size;
    int max_size;
    int *indexes;
    int *maps_to;
} index_map_t;

bool init_index_map(index_map_t *index_map, int initial_size, index_map_type_t type);

bool map_index(index_map_t *index_map, int index, int maps_to);

int maps_to(index_map_t *index_map, int index);

#endif
