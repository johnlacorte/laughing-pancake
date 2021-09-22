#include <stdbool.h>
#include <stdlib.h>
#include "hash_map.h"

typedef struct name
{
    /* data */
    char *name;
    struct name *next;
} name_t;

typedef struct
{
    int number_of_names;
    int number_of_slots;
    //need an array of name_t and head and tail of a list of all names added
    //for rehashing and freeing
    char **slots;
} hash_map_state_t;

hash_map_t new_hash_map(int initial_size)
{
    return NULL;
}

void free_hash_map(hash_map_t hash_map)
{

}

char *add_name_to_hash_map(hash_map_t hash_map, char *name)
{
    return NULL;
}

char *add_name_to_hash_map(hash_map_t *hash_map, char *name)
{
    return NULL;
}

/*** end of file "hash_map.c" ***/
