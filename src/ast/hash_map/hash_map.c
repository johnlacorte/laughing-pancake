#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_map.h"

typedef struct name
{
    char *name;
    struct name *next;
} name_t;

typedef struct
{
    char **slots;
    unsigned int number_of_names;
    unsigned int number_of_slots;
    name_t *name_list_head;
    name_t *name_list_tail;
} hash_map_state_t;

hash_map_t new_hash_map(unsigned int initial_size)
{
    hash_map_state_t *new = malloc(sizeof(hash_map_state_t));
    if(new != NULL)
    {
        //slots needs to be set to NULL
        new->slots = calloc(initial_size, sizeof(char**));
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

static void free_name_list(name_t *name_list_head);

void free_hash_map(hash_map_t hash_map)
{
    hash_map_state_t *state = (hash_map_state_t*)hash_map;
    free_name_list(state->name_list_head);
    free(state->slots);
    free(state);
}

static unsigned int hash_function(char *name, unsigned int number_of_slots);

static char *add_name_to_name_list(name_t *name_list_head, char *name);

char *add_name_to_hash_map(hash_map_t hash_map, char *name)
{
    //check for null pointer
    if(hash_map != NULL)
    {
        hash_map_state_t *state = (hash_map_state_t*)hash_map;
        unsigned int slot_number = hash_function(name, state->number_of_slots);
        //search to end of array
        for(unsigned int i = slot_number; i < state->number_of_slots; i++)
        {
            if(state->slots[i] == NULL)
            {
                //add to name list and slot[i]
                state->slots[i] = add_name_to_name_list(state->name_list_head,
                                                        name);
                if(state->slots[i] == NULL)
                {
                    //free hash map, assume an error message was printed to stderr
                    free_hash_map(hash_map);
                    return NULL;
                }

                else
                {
                    return state->slots[i];
                }
            }

            else
            {
                //compare strings
                if(strcmp(name, state->slots[i]) == 0)
                {
                    return state->slots[i];
                }
            }
        }

        //search staring from beginning of array
        for(unsigned int i = 0; i < slot_number; i++)
        {
            if(state->slots[i] == NULL)
            {
                //add to name list and slot[i]
                state->slots[i] = add_name_to_name_list(state->name_list_head,
                                                        name);
                if(state->slots[i] == NULL)
                {
                    //free hash map, assume an error message was printed to stderr
                    free_hash_map(hash_map);
                    return NULL;
                }

                else
                {
                    return state->slots[i];
                }
            }

            else
            {
                //compare strings
                if(strcmp(name, state->slots[i]) == 0)
                {
                    return state->slots[i];
                }
            }
        }

        //For some reason all slots are full print error message to stderr
        return NULL;
    }

    else
    {
        //failed to allocate memory for hash map print error message to stderr
        return NULL;
    }
}

static void free_name_list(name_t *name_list_head)
{

}

static unsigned int hash_function(char *name, unsigned int number_of_slots)
{
    return 0;
}

static char *add_name_to_name_list(name_t *name_list_head, char *name)
{
    return NULL;
}

/*** end of file "hash_map.c" ***/
