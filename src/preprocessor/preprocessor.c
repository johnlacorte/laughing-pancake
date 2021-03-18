// backslash newline should be handled by input_file list!!

//start by removing comments and converting/eating whitespace
//then seperate tokens by spaces
//seperating tokens by spaces may require some rough number detection
//I might want to seperate multipart opcodes in a way I can remove it if I
//don't need it.
//then consider doing macro expansions, maybe bang out some quick code
//maybe I can just read them, add them to a symbol table then pass them back
//to expand them
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "input_file_list/char_stream/char_stream_status.h"
#include "input_file_list/input_file_list.h"
#include "preprocessor.h"

#define ERROR_MSG_MAX_LENGTH 128

typedef struct
{
    input_file_list_t  file_list;
    int                status;
    int                last_read;
    char               *error_msg;
    char               error_msg_buffer[ERROR_MSG_MAX_LENGTH];
} preproc_state_t;

static preproc_state_t *get_state(preproc_t pre)
{
    return (preproc_state_t *)pre;
}

preproc_t new_preproc()
{
    preproc_state_t *new = malloc(sizeof(preproc_state_t));
    if(new != NULL)
    {
        new->file_list = new_input_file_list();
        if(new->file_list != NULL)
        {
            new->status = PREPROC_OKAY;
            new->last_read = ' ';
            new->error_msg = "Preprocessor okay";
        }
        
        else
        {
            new->status = PREPROC_ERROR;
            new->last_read = ' ';
            new->error_msg = "Preprocessor failed to allocate memory";
        }
    }

    return (preproc_t)new;
}

int open_preproc_files(preproc_t pre, int number_of_files, char **filenames)
{
    preproc_state_t *state = get_state(pre);
    if(state != NULL)
    {
        for(int i = 0; i < number_of_files; i++)
        {
            if(!open_input_file(state->file_list, filenames[i]))
            {
                return 0;
            }
        }
        
        return 1;
    }
    
    return 0;
}

int is_preproc_open(preproc_t pre)
{
    preproc_state_t *state = get_state(pre);
    return 0;
}

void free_preproc(preproc_t pre)
{
    preproc_state_t *state = get_state(pre);
}

int preproc_status(preproc_t pre)
{
    preproc_state_t *state = get_state(pre);
    return 0;
}

char *preproc_error_msg(preproc_t pre)
{
    preproc_state_t *state = get_state(pre);
    return 0;
}

int preproc_line(preproc_t pre)
{
    preproc_state_t *state = get_state(pre);
    return 0;
}

int preproc_line_position(preproc_t pre)
{
    preproc_state_t *state = get_state(pre);
    return 0;
}

char *preproc_current_file_name(preproc_t pre)
{
    preproc_state_t *state = get_state(pre);
    return 0;
}

//One of these three is going to be called by read_preproc_char()
static int seperate_tokens(preproc_state_t *state);
static int break_up_opcodes(preproc_state_t *state);
static int expand_macros(preproc_state_t *state);

//I think I need to have stuff to do checking when reading numbers, insert spaces around
//parenthesis, underscores, forward slash, and period. Maybe before and after x in hex numbers
//maybe before and after e or p in floats maybe before and after sign
int read_preproc_char(preproc_t pre)
{
    preproc_state_t *state = get_state(pre);
    return 0;
}

int read_preproc_extern_name(preproc_t pre)
{
    preproc_state_t *state = get_state(pre);
    return 0;
}

int read_preproc_data(preproc_t pre)
{
    preproc_state_t *state = get_state(pre);
    return 0;
}

void preproc_dump(preproc_t pre, char *filename)
{
    preproc_state_t *state = get_state(pre);
}

//local functions

static int expand_macros(preproc_state_t *state)
{
    return 0;
}

static int break_up_opcodes(preproc_state_t *state)
{
    return 0;
}

static int eat_white_space(preproc_state_t *state);

static int seperate_tokens(preproc_state_t *state)
{
    return 0;
}

static int remove_comments(preproc_state_t *state);

static int eat_white_space(preproc_state_t *state)
{
    return 0;
}

static int remove_comments(preproc_state_t *state)
{
    return 0;
}

/* end of file "preprocessor.c" */

