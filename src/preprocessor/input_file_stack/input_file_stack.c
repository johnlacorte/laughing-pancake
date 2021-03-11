#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "char_stream/char_stream_status.h"
#include "char_stream/char_stream.h"
#include "input_file_stack.h"

typedef struct
{
    //input_file_t  *next;
    void          *next;
    char_stream_t stream;
    int           line;
    int           position;
    char          *short_filename;
    char          long_filename[PATH_MAX];
} input_file_t;

static input_file_t *get_stack_head(input_file_stack_t stack);

static void set_stack_head(input_file_stack_t stack, input_file_t *head);

input_file_stack_t new_input_file_stack()
{
    input_file_t **new_stack = malloc(sizeof(input_file_t**));
    if(new_stack != NULL)
    {
        set_stack_head((input_file_stack_t)new_stack, NULL);
    }

    return (input_file_stack_t)new_stack;
}

static int get_full_path_to_file(char *buffer, char *filename);

static void set_short_file_name(input_file_t *stack);

int open_input_file(input_file_stack_t stack, char *filename)
{
    input_file_t *new = malloc(sizeof(input_file_t));
    if(new != NULL)
    {
        init_char_stream(&new->stream);
        new->line = 0;
        new->position = 0;
        if(get_full_path_to_file(new->long_filename, filename))
        {
            set_short_file_name(new);
            open_char_stream(&new->stream, new->long_filename);
            if(is_char_stream_open(&new->stream))
            {
                new->next = get_stack_head(stack);
                set_stack_head(stack, new);
                return 1;
            }
        }
    }

    return 0;
}

static int get_include_path_to_file(char *buffer, char *include_path, char *filename);

int open_include_file(input_file_stack_t stack, char *include_path, char *filename)
{
    input_file_t *new = malloc(sizeof(input_file_t));
    if(new != NULL)
    {
        init_char_stream(&new->stream);
        new->line = 0;
        new->position = 0;
        if(get_include_path_to_file(new->long_filename, include_path, filename))
        {
            set_short_file_name(new);
            open_char_stream(&new->stream, new->long_filename);
            if(is_char_stream_open(&new->stream))
            {
                new->next = get_stack_head(stack);
                set_stack_head(stack, new);
                return 1;
            }
        }
    }

    return 0;
}

int is_input_file_open(input_file_stack_t stack)
{
    input_file_t *input_file = get_stack_head(stack);
    if(input_file != NULL)
    {
        return is_char_stream_open(&input_file->stream);
    }

    else
    {
        return 0;
    }
}

char *input_file_name(input_file_stack_t stack)
{
    input_file_t *input_file = get_stack_head(stack);
    if(input_file != NULL)
    {
        return input_file->short_filename;
    }

    else
    {
        return "NO FILE OPEN!!";
    }
}

static void pop_input_file(input_file_stack_t stack);

void free_input_file_stack(input_file_stack_t stack)
{
    while(get_stack_head(stack) != NULL)
    {
        pop_input_file(stack);
    }

    free(stack);
}

int input_file_line(input_file_stack_t stack)
{
    input_file_t *input_file = get_stack_head(stack);
    if(input_file != NULL)
    {
        return input_file->line;
    }

    else
    {
        return 0;
    }
}

int input_file_line_position(input_file_stack_t stack)
{
    return 0;
    input_file_t *input_file = get_stack_head(stack);
    if(input_file != NULL)
    {
        return input_file->position;
    }

    else
    {
        return 0;
    }
}

int read_input_file_7bit(input_file_stack_t stack)
{
    //Change this to update position and line, maybe char_stream error instead of 0
    input_file_t *input_file = get_stack_head(stack);
    if(input_file != NULL)
    {
        return pop_7bit_char(&input_file->stream);
    }

    else
    { //return EOF?
        return 0;
    }
}

int32_t read_input_file_utf8(input_file_stack_t stack)
{
    //same as above
    input_file_t *input_file = get_stack_head(stack);
    if(input_file != NULL)
    {
        return pop_utf8(&input_file->stream);
    }

    else
    {
        return 0;
    }
}

char *input_file_error_msg(input_file_stack_t stack)
{
    //error msg for NULL pointer
    input_file_t *input_file = get_stack_head(stack);
    if(input_file != NULL)
    {
        return char_stream_error_msg(&input_file->stream);
    }

    else
    { //file closed?
        return 0;
    }
}

// Local functions

static input_file_t **get_stack_pointer(input_file_stack_t stack)
{
    return (input_file_t**)stack;
}

static input_file_t *get_stack_head(input_file_stack_t stack)
{
    return *get_stack_pointer(stack);
}

static void set_stack_head(input_file_stack_t stack, input_file_t *head)
{
    input_file_t **pointer = get_stack_pointer(stack);
    *pointer = head;
}

static int get_full_path_to_file(char *buffer, char *filename)
{
    return (realpath(filename, buffer) == buffer);
}

static void set_short_file_name(input_file_t *stack)
{
    char *last_slash = strrchr(stack->long_filename, '/');
    if(last_slash == NULL)
    {
        stack->short_filename = stack->long_filename;
    }

    else
    {
        stack->short_filename = last_slash + 1;
    }
}

static int get_include_path_to_file(char *buffer, char *include_path, char *filename)
{
    char temp[PATH_MAX];
    sprintf(temp, "%s%s", include_path, filename);
    return (realpath(temp, buffer) == buffer);
}

static void pop_input_file(input_file_stack_t stack)
{
    input_file_t *head = get_stack_head(stack);
    input_file_t *next = head->next;
    set_stack_head(stack, head);
    free(head);
}

/*** end of file "input_file_stack.c" ***/

