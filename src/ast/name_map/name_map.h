#ifndef NAME_MAP_H
#define NAME_MAP_H

typedef struct name
{
    char *name_string;
    int index;
    struct name *next;
} name_t;


typedef struct
{
    int count;
    name_t *head;
} name_map_t;


void init_name_map(name_map_t *name_map);

void free_name_map(name_map_t *name_map);

bool add_name_to_name_map(name_map_t *name_map, char *name, int index);

int get_name_index(name_map_t *name_map, char *name);

#endif

/*** end of file "name_map.h" ***/
