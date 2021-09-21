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
    name_t *current = name_map->head;
    while(current != NULL)
    {
        name_t *next = current->next;
        free(current);
        current = next;
    }
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
            fprintf(stderr, "Failed to allocate memory for name.\n");
            return false;
        }
    }

    else
    {
        name_t *current = name_map->head;
        if(current->name_string == name)
        {
            free_name_map(name_map);
            fprintf(stderr, "Name \"%s\" already declared.\n", name);
            return false;
        }

        while(current->next != NULL)
        {
            if(current->name_string == name)
            {
                free_name_map(name_map);
                fprintf(stderr, "Name \"%s\" already declared.\n", name);
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
            fprintf(stderr, "Failed to allocate memory for name.\n");
            return false;
        }
    }
}

int get_name_index(name_map_t *name_map, char *name)
{
    name_t *current = name_map->head;
    while(current != NULL)
    {
        if(current->name_string == name)
        {
            return current->index;
        }

        current = current->next;
    }

    free_name_map(name_map);
    fprintf(stderr, "Unknown name \"%s\".\n");
    return -1;
}

name_t *new_name(char *name, int index)
{
    name_t *new = malloc(sizeof(name_t));
    if(new != NULL)
    {
        new->name_string = name;
        new->index = index;
        new->next = NULL;
    }

    return new;
}

/*** end of file "name_map.c" ***/
