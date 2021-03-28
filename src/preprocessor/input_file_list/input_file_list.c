//For PATH_MAX
#include <limits.h>

#include <stdbool.h>

//To return an int32_t for Unicode codepoints
#include <stdint.h>

//Because char_stream_t struct has a FILE * in it
#include <stdio.h>

//For malloc(), free(), and realpath()
#include <stdlib.h>

#include "char_stream/char_stream_status.h"
#include "char_stream/char_stream.h"
#include "input_file_list.h"

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

input_file_list_t new_input_file_list()
{
    input_file_t **new_list = malloc(sizeof(input_file_t**) * 2);
    if(new_list != NULL)
    {
        new_list[0] = NULL;
        new_list[1] = NULL;
    }

    return (input_file_list_t)new_list;
}

//Tries to allocate memory for an input_file_t, set up the state, and open file
//returns pointer to new input file or NULL if something goes wrong
static input_file_t *new_input_file(char *filename);

//Adds an input_file_t pointer to the end of the file list
static void add_input_file_to_tail(input_file_list_t file_list, input_file_t *new);

int open_input_file(input_file_list_t file_list, char *filename)
{
    if(file_list != NULL)
    {
        input_file_t *new = new_input_file(filename);
        if(new != NULL)
        {
            add_input_file_to_tail(file_list, new);
            return 1;
        }
    }

    return 0;
}

//Adds an input_file_t pointer to the beginning of the file list
static void add_input_file_to_head(input_file_list_t file_list, input_file_t *new);

int open_include_file(input_file_list_t file_list, char *filename)
{
    if(file_list != NULL)
    {
        input_file_t *new = new_input_file(filename);
        if(new != NULL)
        {
            add_input_file_to_head(file_list, new);
            return 1;
        }
    }

    return 0;
}

//Returns input_file_t pointer at the head of the list
//(the file to be read from)
static input_file_t *get_file_list_head(input_file_list_t file_list);

int is_input_file_open(input_file_list_t file_list)
{
    if(file_list != NULL)
    {
        input_file_t *input_file = get_file_list_head(file_list);
        if(input_file != NULL)
        {
            return is_char_stream_open(&input_file->stream);
        }
    }

    return 0;
}

//Removes input_file_t pointer from head of the list, closing files and freeing
//memory
static void pop_input_file(input_file_list_t file_list);

void free_input_file_list(input_file_list_t file_list)
{
    if(file_list != NULL)
    {
        while(get_file_list_head(file_list) != NULL)
        {
            pop_input_file(file_list);
        }

        free(file_list);
    }
}

char *input_file_name(input_file_list_t file_list)
{
    if(file_list != NULL)
    {
        input_file_t *input_file = get_file_list_head(file_list);
        if(input_file != NULL)
        {
            return input_file->short_filename;
        }

        return "NO FILE OPEN!!";
    }

    return "NULL POINTER PASSED TO input_file_name()!!";
}

int input_file_line(input_file_list_t file_list)
{
    if(file_list != NULL)
    {
        input_file_t *input_file = get_file_list_head(file_list);
        if(input_file != NULL)
        {
            return input_file->line;
        }
    }

    return 0;
}

int input_file_line_position(input_file_list_t file_list)
{
    if(file_list != NULL)
    {
        input_file_t *input_file = get_file_list_head(file_list);
        if(input_file != NULL)
        {
            return input_file->position;
        }
    }

    return 0;
}

int32_t read_utf8_from_input_file(input_file_list_t file_list)
{
    if(file_list != NULL)
    {
        input_file_t *input_file = get_file_list_head(file_list);
        //change loop to an if
        if(input_file != NULL)
        {
            int32_t ch = read_utf8_from_char_stream(&input_file->stream);
            if(ch != CHAR_STREAM_EOF)
            {
                if(ch == '\n')
                {
                    input_file->line++;
                    input_file->position = 0;
                }
        
                else
                {
                    input_file->position++;
                }
            }

            return ch;
        }

        return CHAR_STREAM_EOF;
    }

    return CHAR_STREAM_PREPROC_DID_SOMETHING_WRONG;
}

int32_t next_input_file(input_file_list_t file_list)
{
    if(file_list != NULL)
    {
        if(get_file_list_head(file_list) != NULL)
        {
            pop_input_file(file_list);
            if(get_file_list_head(file_list) != NULL)
            {
                return ' ';
            }
        }

        return CHAR_STREAM_EOF;
    }

    else
    {
        return CHAR_STREAM_PREPROC_DID_SOMETHING_WRONG;
    }
}

char *input_file_error_msg(input_file_list_t file_list)
{
    if(file_list != NULL)
    {
        input_file_t *input_file = get_file_list_head(file_list);
        if(input_file != NULL)
        {
            return char_stream_error_msg(&input_file->stream);
        }

        else
        {
            return "NO FILE OPEN!!";
        }
    }

    return "NULL POINTER PASSED TO input_file_error_msg()!!";
}

/*    ---- Local functions ----    */

//Returns true/false to indicate success/failure, tries to get the full path to
//a file and copies it to the input_file_t's long_filename array
static int get_full_path_to_file(input_file_t *new, char *filename);

//Sets the input_file_t's short_filename pointer to a character in its
//long_filename array pointing it to the start of just the file name
static void set_short_file_name(input_file_t *new);

static input_file_t *new_input_file(char *filename)
{
    input_file_t *new = malloc(sizeof(input_file_t));
    if(new != NULL)
    {
        init_char_stream(&new->stream);
        new->line = 1;
        new->position = 0;
        if(get_full_path_to_file(new, filename))
        {
            set_short_file_name(new);
            open_char_stream(&new->stream, new->long_filename);
            if(is_char_stream_open(&new->stream))
            {
                new->next = NULL;
                return new;
            }
        }

        //if either getting full path fails or opening file fails
        free(new);
    }

    return NULL;
}

//Returns last input_file_t pointer in list
static input_file_t *get_file_list_tail(input_file_list_t file_list);

//Sets file list head pointer to the new first input_file_t pointer
static void set_file_list_head(input_file_list_t file_list, input_file_t *new);

//Sets file list tail pointer to the new last input_file_t pointer
static void set_file_list_tail(input_file_list_t file_list, input_file_t *new);

static void add_input_file_to_tail(input_file_list_t file_list, input_file_t *new)
{
    input_file_t *tail = get_file_list_tail(file_list);
    if(tail != NULL)
    {
        tail->next = new;
    }

    else
    {
        //This is for the first input file added to list
        set_file_list_head(file_list, new);
    }

    set_file_list_tail(file_list, new);
}

static void add_input_file_to_head(input_file_list_t file_list, input_file_t *new)
{
    input_file_t *head = get_file_list_head(file_list);
    if(head != NULL)
    {
        new->next = get_file_list_head(file_list);
    }

    else
    {
        //This is for the first input file added to list
        set_file_list_tail(file_list, new);
    }

    set_file_list_head(file_list, new);
}

static input_file_t *get_file_list_head(input_file_list_t file_list)
{
    input_file_t **list = (input_file_t**)file_list;
    return list[0];
}

static void pop_input_file(input_file_list_t file_list)
{
    input_file_t *head = get_file_list_head(file_list);
    input_file_t *next = head->next;
    if(next == NULL)
    {
        set_file_list_tail(file_list, next);
    }

    set_file_list_head(file_list, next);
    close_char_stream(&head->stream);
    free(head);
}

//This function needs different versions for different OSes
static int get_full_path_to_file(input_file_t *new, char *filename)
{
    return (realpath(filename, new->long_filename) == new->long_filename);
}

//For strrchr()
#include <string.h>

static void set_short_file_name(input_file_t *new)
{
    char *last_slash = strrchr(new->long_filename, '/');
    if(last_slash == NULL)
    {
        new->short_filename = new->long_filename;
    }

    else
    {
        new->short_filename = last_slash + 1;
    }
}

static input_file_t *get_file_list_tail(input_file_list_t file_list)
{
    input_file_t **list = (input_file_t**)file_list;
    return list[1];
}

static void set_file_list_tail(input_file_list_t file_list, input_file_t *new)
{
    input_file_t **list = (input_file_t**)file_list;
    list[1] = new;
}

static void set_file_list_head(input_file_list_t file_list, input_file_t *new)
{
    input_file_t **list = (input_file_t**)file_list;
    list[0] = new;
}

/*** end of file "input_file_list.c" ***/

/*Notes
There's not really a standard way to get the full path to a file. I am
currently using realpath() which is a part of stdlib on BSD systems
but this means I need to compile this file with a different flag from the rest
of the source files. I don't think I would need to do this on Cygwin or Linux
if I instead used getcwd() from unistd but then I would have to deal with
gluing together the directory with whatever got passed in from the command
line and dealing with dots and things. Windows has GetFullPathName functions
(windows.h/fileapi.h/kernel32.dll) but I don't know what the situation is on
Mac, Android, BeOS, DOS, etc.
*/