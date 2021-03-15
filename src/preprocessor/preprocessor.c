#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "input_file_list/char_stream/char_stream_status.h"
#include "input_file_list.h"
#include "preprocessor.h"

#define ERROR_MSG_MAX_LENGTH 128

typedef struct
{
    char               *include_path;
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
    return 0;
}

preproc_t open_preproc(char *include_path, char *filename)
{
    return 0;
}

int is_preproc_open(preproc_t pre)
{
    return 0;
}

void free_preproc(preproc_t pre)
{

}

int preproc_status(preproc_t pre)
{
    return 0;
}

char *preproc_error_msg(preproc_t pre)
{
    return 0;
}

int preproc_line(preproc_t pre)
{
    return 0;
}

int preproc_line_position(preproc_t pre)
{
    return 0;
}

char *preproc_file_name(preproc_t pre)
{
    return 0;
}

//I think I need to have stuff to do checking when reading numbers, insert spaces around
//parenthesis, underscores, forward slash, and period. Maybe before and after x in hex numbers
//maybe before and after e or p in floats maybe before and after sign
int read_preproc_char(preproc_t pre)
{
    return 0;
}

int read_preproc_extern_name(preproc_t pre, char *buffer)
{
    return 0;
}

int read_preproc_data(preproc_t pre, char_buffer_t *buffer)
{
    return 0;
}

void preproc_dump(preproc_t pre, char *filename)
{

}

/* end of file "preprocessor.c" */

