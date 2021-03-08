#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
//Questioning if I even need strinng.h
#include <string.h>

#include "char_stream/char_stream.h"
#include "preprocessor.h"

#define ERROR_MSG_MAX_LENGTH 128

typedef struct
{
    input_file_t  *next;
    int           line;
    int           position;
    char_stream_t stream;
    char          *short_filename;
    char          long_filename[PATH_MAX];
} input_file_t;

typedef struct
{
    char         *short_filename;
    input_file_t *input_file_stack;
    int          status;
    int          last_read;
    char         *error_msg;
    char         error_msg_buffer[ERROR_MSG_MAX_LENGTH];
} preproc_state_t;

static int open_input_file(input_file_t *stack, char *filename);

static int is_input_file_open(input_file *stack);

preproc_t preproc_open_wat(char *filename)
{
    preproc_state_t *state = malloc(sizeof(preproc_state_t));
    if(state != NULL)
    {
        if(open_input_file(state->input_file_stack, filename))
        {
            if(is_input_file_open(state->stream))
            {
                state->status = PREPROC_OK;
                state->error_msg = "No Errors.";
            }

            else
            {
                state->status = PREPROC_ERROR;
                //Build error msg with filename
                state->error_msg = "Error opening file.";
            }
            state->last_read = ' ';
        }
    }

    return (preproc_t)state;
}

static void close_input_files(input_file_t *stack);

void preproc_close_wat(preproc_t pre)
{
    preproc_state_t *state = (preproc_state_t*)pre;
    if(state != NULL)
    {
        close_input_files(state->input_file_stack);
    }

    //Possibly free'ing NULL pointer here if the caller of these functions
    //isn't checking for NULL pointers
    free(state);
}

int preproc_check_status(preproc_t pre)
{
    preproc_state_t *state = (preproc_state_t*)pre;
    if(state == NULL)
    {
        return PREPROC_ERROR;
    }

    else
    {
        return state->status;
    }
}

char *preproc_error_msg(preproc_t pre)
{
    preproc_state_t *state = (preproc_state_t*)pre;
    if(state == NULL)
    {
        return "Memory allocation for preprocessor failed. (Function was passed NULL pointer)";
    }

    else
    {
        return state->status_string;
    }
}

int preproc_current_line(preproc_t pre)
{
    //Check for NULL or if file is even open
    preproc_state_t *state = (preproc_state_t*)pre;
    return state->line;
}

int preproc_line_position(preproc_t pre)
{
    //check for NULL or if file is even open
    preproc_state_t *state = (preproc_state_t*)pre;
    return state->position;
}

char *preproc_file_name(preproc_t pre)
{
    preproc_state_t *state = (preproc_state_t*)pre;
    //return state->short_filename;
    return "filename.wat";
}

static int read_7bit(input_file_t *stack);

int preproc_read_char(preproc_t pre)
{
    preproc_state_t *state = (preproc_state_t*)pre;
    //write this function
    return 0;
}

int preproc_read_extern_name(preproc_t pre)
{
    return 0;
}

int preproc_read_data(preproc_t pre)
{
    return 0;
}

static int preproc_dump_string(preproc_t pre, FILE *fp);

//Maybe this should return a string telling what happenned
//or the status when it finished of maybe a comment?
void preproc_dump(preproc_t pre, char *filename)
{
    preproc_state_t *state = (preproc_state_t*)pre;
    //write this function
}

static int open_input_file(input_file_t *stack, char *filename)
{
    return 0;
}

static int is_input_file_open(input_file *stack)
{
    return 0;
}

static void close_input_files(input_file_t *stack)
{

}

static int read_7bit(input_file_t *stack)
{
    return 0;
}

static int read_utf8(input_file_t *stack)
{
    return 0;
}

int preproc_dump_string(preproc_t pre, FILE *fp)
{
    //I think I'm just going to read 
    preproc_state_t *state = (preproc_state_t*)pre;
    return 0;
}

/* end of file "preprocessor.c" */

