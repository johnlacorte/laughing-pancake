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
    char **slots;
    int number_of_names;
    int number_of_slots;
    name_t *name_list_head;
    name_t *name_list_tail;
} hash_map_state_t;

hash_map_t new_hash_map(int initial_size)
{
    hash_map_state_t *new = malloc(sizeof(hash_map_state_t));
    if(new != NULL)
    {
        new->slots = malloc(sizeof(char**) * initial_size);
        if(new->slots != NULL)
        {
            new->number_of_names = 0;
            new->number_of_slots = initial_size;
            new->name_list_head = NULL;
            new->name_list_tail = NULL;
            return (hash_map_t)new;
        }

        else
        {
            free(new);
            return NULL;
        }
    }

    else
    {
        return NULL;
    }
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
