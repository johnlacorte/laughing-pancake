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

#endif
