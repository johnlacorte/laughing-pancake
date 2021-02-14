#include <stdio.h>
#include <stdlib.h>

#include "preprocessor.h"

typedef struct
{
    FILE *fp;
    int  status;
    char *status_string;
    int  line;
    int  position;
} preproc_state_t;

preproc_t preproc_open_wat(char *filename)
{

}

void preproc_free(preproc_t pre)
{

}

int preproc_check_status(preproc_t pre)
{
    return 0;
}

void preproc_print_status(preproc_t pre)
{

}

int preproc_read_char(preproc_t pre)
{
    return 0;
}

char *preproc_read_string(preproc_t pre)
{
    return 0;
}

int preproc_current_line(preproc_t, pre)
{
    return 0;
}

int preproc_line_position(preproc_t, pre)
{
    return 0;
}

void preproc_dump(preproc_t pre, char *filename)
{

}

/* end of file "preprocessor.c" */

