#include <stdio.h>
#include <stdlib.h>

#include "char_stream/char_stream.h"
#include "preprocessor.h"

typedef struct
{
    //change to using a char_stream_t
    //I might put this and the stuff that interacts with it in a
    //seperate module or possibly add some of this to char_stream.h
    FILE *fp;
    int  status;
    char *status_string;
    int  line;
    int  position;
    int last_read;
} preproc_state_t;

preproc_t preproc_open_wat(char *filename)
{
    preproc_state_t *state = malloc(sizeof(preproc_state_t));
    if(state != NULL)
    {
        //change to opening a char_stream_t
        state->fp = fopen(filename, "r");
        if(state->fp != NULL)
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

void preproc_free(preproc_t pre)
{
    preproc_state_t *state = (preproc_state_t*)pre;
    //change to close a char_stream_t
    fclose(state->fp);
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
char *preproc_get_status_string(preproc_t pre)
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

