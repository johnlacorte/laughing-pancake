#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "name_map.h"

void init_name_map(name_map_t *name_map)
{
    name_map->count = 0;
    name_map->head = NULL;
}

void free_name_map(name_map_t *name_map)
{
    //free the name list
}

name_t *new_name(char *name, int index);

bool add_name_to_name_map(name_map_t *name_map, char *name, int index)
{
    if(name_map->head == NULL)
    {
        name_t *new = new_name(name, index);
        if(new != NULL)
        {
            name_map->head = new;
            return true;
        }

        else
        {
            free(new);
            free_name_map(name_map);
            //message
            return false;
        }
    }

    else
    {
        name_t *current = name_map->head;
        if(current->name_string == name)
        {
            free_name_map(name_map);
            //message
            return false;
        }

        while(current->next != NULL)
        {
            if(current->name_string == name)
            {
                free_name_map(name_map);
                //message
                return false;
            }

            else
            {
                current = current->next;
            }
        }

        name_t *new = new_name(name, index);
        if(new != NULL)
        {
            current->next = new;
            return true;
        }

        else
        {
            free(new);
            free_name_map(name_map);
            //message
            return false;
        }
    }
}

int get_name_index(name_map_t *name_map, char *name)
{
    return -1;
}

name_t *new_name(char *name, int index)
{
    return NULL;
}

/*** end of file "name_map.c" ***/
