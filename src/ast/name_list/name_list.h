#ifndef NAME_LIST_H
#define NAME_LIST_H

typedef struct name
{
    /* data */
    char *name_string;
    struct name *next;
} name_t;


typedef struct
{
    int count;
    name_t *head;
} name_list_t;


void init_name_list(name_list_t *name_list);

bool add_name_to_name_list(name_list_t *name_list);

int get_name_index(name_list_t *name_list);
#endif
