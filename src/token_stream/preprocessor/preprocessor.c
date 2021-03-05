#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "char_stream/char_stream.h"
#include "preprocessor.h"

typedef struct
{
    char_stream_t *stream;
    int           status;
    char          *status_string;
    int           line;
    int           position;
    int           last_read;
} preproc_state_t;

preproc_t preproc_open_wat(char *filename)
{
    preproc_state_t *state = malloc(sizeof(preproc_state_t));
    if(state != NULL)
    {
        open_char_stream(state->stream, filename);
        if(is_char_stream_open(state->stream))
        {
            state->status = PREPROC_OK;
            state->status_string = "No Errors.";
        }

        else
        {
            state->status = PREPROC_ERROR;
            state->status_string = "Error opening file.";
        }
        state->line = 0;
        state->position = 0;
        state->last_read = ' ';
    }

    return (preproc_t)state;
}

void preproc_close_wat(preproc_t pre)
{
    preproc_state_t *state = (preproc_state_t*)pre;
    //change to close a char_stream_t
    close_char_stream(state->stream);
    free(state);
}

//I think I might use status for something else and instead return
//0 or if last_read was negative
int preproc_check_status(preproc_t pre)
{
    preproc_state_t *state = (preproc_state_t*)pre;
    return state->status;
}

//Is this supposed to return a const char* ?
char *preproc_error_msg(preproc_t pre)
{
    preproc_state_t *state = (preproc_state_t*)pre;
    return state->status_string;
}

int preproc_current_line(preproc_t pre)
{
    preproc_state_t *state = (preproc_state_t*)pre;
    return state->line;
}

int preproc_line_position(preproc_t pre)
{
    preproc_state_t *state = (preproc_state_t*)pre;
    return state->position;
}

int preproc_read_char(preproc_t pre)
{
    preproc_state_t *state = (preproc_state_t*)pre;
    //write this function
    return 0;
}

void preproc_read_name(preproc_t pre, char *buffer)
{

}

void preproc_read_extern_name(preproc_t pre, char *buffer)
{

}
void preproc_read_data(preproc_t pre, char_buffer_t *buffer)
{

}

char *preproc_read_string(preproc_t pre)
{
    //I'm probably going to return something other than a char*
    //like a struct with string length and status when finished
    preproc_state_t *state = (preproc_state_t*)pre;
    return 0;
}

//Maybe this should return a string telling what happenned
//or the status when it finished
void preproc_dump(preproc_t pre, char *filename)
{
    preproc_state_t *state = (preproc_state_t*)pre;
    //write this function
}

/* end of file "preprocessor.c" */

