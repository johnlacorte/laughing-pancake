//start by removing comments and converting/eating whitespace
//then seperate tokens by spaces
//seperating tokens by spaces may require some rough number detection
//I might want to seperate multipart opcodes in a way I can remove it if I
//don't need it.
//then consider doing macro expansions, maybe bang out some quick code
//maybe I can just read them, add them to a symbol table then pass them back
//to expand them
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "input_file_list/char_stream/char_stream_status.h"
#include "input_file_list/input_file_list.h"
#include "preprocessor.h"

#define ERROR_MSG_MAX_LENGTH 128

typedef int32_t utf8_encoder_state_t;

typedef struct
{
    input_file_list_t  file_list;
    int                status;
    int                read_mode;
    int                last_read;
    int                what_to_return_next;
    int32_t            next_read;
    char               *error_msg;
    char               error_msg_buffer[ERROR_MSG_MAX_LENGTH];
} preproc_state_t;

static preproc_state_t *get_state(preproc_t pre);

#define UTF8_ENCODER_EMPTY -1

#define NORMAL_MODE 0

#define EXTERN_NAME_MODE 1

#define DATA_MODE 2

#define READ_FROM_INPUT_FILE_NEXT 0

#define READ_FROM_NEXT_READ_NEXT 1

preproc_t new_preproc()
{
    preproc_state_t *new = malloc(sizeof(preproc_state_t));
    if(new != NULL)
    {
        new->file_list = new_input_file_list();
        if(new->file_list != NULL)
        {
            new->status = PREPROC_OKAY;
            new->read_mode = NORMAL_MODE;
            new->last_read = ' ';
            new->what_to_return_next = READ_FROM_INPUT_FILE_NEXT;
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

static void preproc_read_error_msg(preproc_state_t *pre, char *msg);

//I think I need to have stuff to do checking when reading numbers, insert spaces around
//parenthesis, underscores, forward slash, and period. Maybe before and after x in hex numbers
//maybe before and after e or p in floats maybe before and after sign
int read_preproc_char(preproc_t pre)
{
    //check status
    //Make sure to set last_read
    //Error for double quote
    //Error for values greater than 127
    //Error if not NORMAL_MODE
    preproc_state_t *state = get_state(pre);
    return 0;
}

static int read_string(preproc_t pre);

int read_preproc_extern_name(preproc_t pre)
{
    //if normal mode check for quote and set mode
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

static preproc_state_t *get_state(preproc_t pre)
{
    return (preproc_state_t *)pre;
}

static int read_string(preproc_t pre)
{
    return 0;
}

static void preproc_read_error_msg(preproc_state_t *pre, char *msg)
{
    //make sure format string is correct, maybe remove periods from error messages
    pre->status = PREPROC_READ_ERROR;
    sprintf(pre->error_msg_buffer,
            "%s at [%d:%d].",
            msg,
            input_file_line(pre->file_list),
            input_file_line_position(pre->file_list));
    pre->error_msg = pre->error_msg_buffer;
}

//For normal mode
static int32_t remove_extra_spaces(preproc_state_t *pre);

static int32_t insert_space_after_symbols(preproc_state_t *pre);

static int32_t convert_whitespace(preproc_state_t *pre);

static int32_t remove_comments(preproc_state_t *pre);

//All read modes use these
static int32_t remove_escaped_newlines(preproc_state_t *pre);

static void set_next_read(preproc_state_t *pre, int32_t next_ch);

static int32_t read_next(preproc_state_t *pre);

//For normal mode
static int32_t remove_extra_spaces(preproc_state_t *pre)
{
    int32_t ch = insert_space_after_symbols(pre);
    if(last_read == ' ')
    {
        while(ch == ' ')
        {
            ch = insert_space_after_symbols(pre);
        }
    }

    return ch;
}

static int32_t insert_space_after_symbols(preproc_state_t *pre)
{
    if(pre->last_read == '(' || pre->last_read == ')')
    {
        return ' ';
    }

    else
    {
        return convert_whitespace(pre);
    }
}

static int32_t convert_whitespace(preproc_state_t *pre)
{
    int32_t ch = remove_comments(pre);
    if(ch == '\n' || ch == '\t')
    {
        return ' ';
    }

    else
    {
        return ch;
    }
}

static int32_t insert_space_before_symbols(preproc_state_t *pre);

static int32_t remove_multiline_comment(preproc_state_t *pre);

static int32_t remove_line_comment(preproc_state_t *pre);

static int32_t remove_comments(preproc_state_t *pre)
{
    int32_t ch = insert_space_before_symbols(pre);
    if(ch == '(')
    {
        int32_t next_ch = remove_escaped_newlines(pre);
        if(next_ch == ';')
        {
            ch = remove_multiline_comment(pre);
        }

        else //Not a comment
        {
            set_next_read(pre, next_ch);
        }

        if(ch == ';')
        {
            int32_t next_ch = remove_escaped_newlines(pre);
            if(next_ch == ';')
            {
                ch = remove_line_comment(pre);
            }

            else
            {
                if(next_ch == ')')
                {
                    // ;) is a winky error
                    preproc_read_error_msg(pre, "Unexpected end of comment found");
                    return pre->status;
                }

                else
                {
                    //This probably should be an error, I don't think
                    //semicolons are used for anything else
                    set_next_read(pre, next_ch);
                }
            }
        }
    }

    return ch;
}

static int32_t remove_multiline_comment(preproc_state_t *pre)
{
    //Keep count of how deep the comments are nested
    int depth = 1;
    while(depth > 0)
    {
        int32_t ch = remove_escaped_newlines(pre);
        if(ch == CHAR_STREAM_EOF)
        {
            preproc_read_error_msg(pre, "End of file reached before end of comment");
            return pre->status;
        }

        if(ch == '(')
        {
            next_ch = remove_escaped_newlines(pre);
            if(next_ch == ';')
            {
                depth++;
            }

            else
            {
                set_next_read(pre, next_ch);
            }
        }

        if(ch == ';')
        {
            next_ch = remove_escaped_newlines(pre);
            if(next_ch == ')')
            {
                depth--;
            }

            else
            {
                set_next_read(pre, next_ch);
            }
        }
    }

    return insert_space_before_symbols(pre);
}

static int32_t remove_line_comment(preproc_state_t *pre)
{
    ch = remove_escaped_newlines(pre);
    while(ch != '\n')
    {
        if(ch == CHAR_STREAM_EOF)
        {
            return ch;
        }

        else
        {
            ch = remove_escaped_newlines(pre);
        }
    }

    return insert_space_before_symbols(pre);
}

static int32_t insert_space_before_symbols(preproc_state_t *pre)
{
    //spaces before '"' '$' '(' ')' ';'
    //a $ can appear in a name so I need to check that
    if(pre->last_read == ' ')
    {
        return remove_escaped_newlines(pre);
    }

    else
    {
        int32_t ch = remove_escaped_newlines(pre);
        switch(ch)
        {
            case '"':
            case '$':
            case '(':
            case ')':
            case ';':
                //emit space and push ch for next read
                set_next_read(pre, ch);
                return ' ';
            default:
                return ch;
        }
    }
}

//All read modes use these
static int32_t remove_escaped_newlines(preproc_state_t *pre)
{
    int32_t ch = read_next(pre);
    while(ch == '\\')
    {
        int32_t next_ch = read_next(pre);
        if(next_ch == '\n')
        {
            ch = read_next(pre);
        }

        else
        {
            set_next_read(pre, next_ch);
            return ch;
        }
    }

    return ch;
}

static void set_next_read(preproc_state_t *pre, int32_t next_ch)
{
    if(pre->what_to_return_next == READ_FROM_INPUT_FILE_NEXT)
    {
        pre->next_read = next_ch;
        pre->what_to_return_next = READ_FROM_NEXT_READ_NEXT;
    }

    else
    {
        pre->status = PREPROC_ERROR;
        pre->error_msg = "preprocessor tried to set read_next twice.";
    }
}

//since there are three read modes it might make more sense to check as the last step
//to generate the error message, it might be a good idea to convert the value to the
//preprocessor constant first before doing anything. It might be better to generate
//the error msg as early a possible too, I don't know.
static int32_t check_char_stream_error(preproc_state_t *pre);

static int32_t read_next(preproc_state_t *pre)
{
    if(pre->what_to_return_next == READ_FROM_NEXT_READ_NEXT)
    {
        pre->what_to_return_next = READ_FROM_INPUT_FILE_NEXT;
        return pre->next_read;
    }

    else
    {
        return check_char_stream_error(pre);
    }
}

static int32_t check_char_stream_error(preproc_state_t *pre)
{
    int32_t ch = read_utf8_from_input_file(pre->file_list);
    if(ch < 0)
    {
        if(ch == CHAR_STREAM_EOF)
        {
            return PREPROC_EOF;
        }

        else
        {
            if(ch == CHAR_STREAM_READ_FAILED)
            {
                preproc_read_error_msg(pre, input_file_error_msg(pre->file_list));
                return pre->status;
            }

            else
            {
                //PREPROC_ERROR + set pointer to msg
                pre->status = PREPROC_ERROR;
                pre->error_msg = input_file_error_msg(pre->file_list);
                return pre->status;
            }
        }
    }

    else
    {
        return ch;
    }
}

/* end of file "preprocessor.c" */

