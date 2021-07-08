//TODO
//build different read mode validation sets
//calls to utf8_file functions need to match the new prototypes
//rewrite open_preproc() and other functions to use a typecasted void pointer
//read_string needs to be combined with convert_escape_sequences() and return an int
//read_byte_escape_from_utf8_file() and read_byte_escape()
//compile
//testing
//build script
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "utf8_file/utf8_file.h"
#include "utf8_encoder/utf8_encoder.h"
#include "preprocessor.h"

typedef struct
{
    utf8_file_t    file;
    utf8_encoder_t encoder;
    int            status;
    int            read_mode;
    int32_t        last_read;
    bool           return_temp_next;
    int32_t        temp;
    char           *error_msg;
} preproc_state_t;

#define NORMAL_MODE 0

#define NAME_MODE 1

#define STRING_MODE 2

#define EXTERN_NAME_MODE 3

#define DATA_STRING_MODE 4

preproc_t open_preproc(char *filename)
{
    preproc_state_t *new = malloc(sizeof(preproc_state_t));
    if(new != NULL)
    {
        //open_utf8_file() declared in utf8_file/utf8_file.h
        new->file = open_utf8_file(filename);
        //init_utf8_encoder() declared in utf8_encoder/utf8_encoder.h
        init_utf8_encoder(&new->encoder);
        new->status = PREPROC_OK;
        new->read_mode = NORMAL_MODE;
        new->last_read = ' ';
        new->return_temp_next = false;
        new->temp = ' ';
        new->error_msg = "Preprocessor okay.";
        return (preproc_t)new;
    }

    else
    {
        return NULL;
    }
}

void free_preproc(preproc_t pre)
{
    if(pre != NULL)
    {
        preproc_state_t *state = (preproc_state_t*)pre;
        //free_utf8_file declared in utf8_encoder/utf8_encoder.h
        free_utf8_file(state->file);
        free(pre);
    }
}

int get_preproc_status(preproc_t pre)
{
    if(pre != NULL)
    {
        preproc_state_t *state = (preproc_state_t*)pre;
        return state->status;
    }

    else
    {
        return PREPROC_ERROR;
    }
}

char *get_preproc_error_msg(preproc_t pre)
{
    if(pre != NULL)
    {
        preproc_state_t *state = (preproc_state_t*)pre;
        return state->error_msg;
    }

    else
    {
        return "Failed to allocate memory for preprocessor.";
    }
}

int get_preproc_line_number(preproc_t pre)
{
    if(pre != NULL)
    {
        preproc_state_t *state = (preproc_state_t*)pre;
        //get_utf8_file_line_number() declared in utf8_file/utf8_file.h
        return get_utf8_file_line_number(state->file);
    }

    else
    {
        return -1;
    }
}

int get_preproc_line_position(preproc_t pre)
{
    if(pre != NULL)
    {
        preproc_state_t *state = (preproc_state_t*)pre;
        //get_utf8_file_line_position() declared in utf8_file/utf8_file.h
        return get_utf8_file_line_position(state->file);
    }

    else
    {
        return -1;
    }
}

void reset_preproc_status(preproc_t pre)
{
    //check if pointer is NULL
    //if I need to pass through any utf8_file_t error when returning a
    //preprocessor read error I may have to reset utf8_file_t as well
    //when testing that
}

//short description
static int32_t read_normal_mode_char(preproc_state_t *state);

//short description
static int32_t read_name_mode_char(preproc_state_t *state);

//short description
static int read_next_byte_from_encoder_helper(preproc_state_t *state);

int read_preproc_char(preproc_t pre)
{
    if(pre != NULL)
    {
        preproc_state_t *state = (preproc_state_t*)pre;
        switch(state->status)
        {
            case PREPROC_OK:
                switch(state->read_mode)
                {
                    case NORMAL_MODE:
                        //is_utf8_encoder_empty() declared in 
                        if(is_utf8_encoder_empty(&state->encoder))
                        {
                            int32_t ch = read_normal_mode_char(state);
                            if(ch < 0)
                            {
                                return (int)ch;
                            }

                            else
                            {
                                state->last_read = ch;
                                //encode_codepoint_to_utf8() declared in 
                                encode_codepoint_to_utf8(&state->encoder, ch);
                                return
                                     read_next_byte_from_encoder_helper(state);
                            }
                        }

                        else
                        {
                            return read_next_byte_from_encoder_helper(state);
                        }

                    case NAME_MODE:
                        if(is_utf8_encoder_empty(&state->encoder))
                        {
                            int32_t ch = read_name_mode_char(state);
                            if(ch < 0)
                            {
                                return (int)ch;
                            }

                            else
                            {
                                state->last_read = ch;
                                encode_codepoint_to_utf8(&state->encoder, ch);
                                return
                                     read_next_byte_from_encoder_helper(state);
                            }
                        }

                        else
                        {
                            return read_next_byte_from_encoder_helper(state);
                        }

                    case STRING_MODE:
                        state->status = PREPROC_ERROR;
                        state->error_msg =
                     "read_preproc_char() called after double quote was read.";
                        return PREPROC_ERROR;

                    case EXTERN_NAME_MODE:
                        state->status = PREPROC_ERROR;
                        state->error_msg =
   "read_preproc_char() called before external name string was entirely read.";
                        return PREPROC_ERROR;

                    case DATA_STRING_MODE:
                        state->status = PREPROC_ERROR;
                        state->error_msg =
            "read_preproc_char() called before data string was entirely read.";
                        return PREPROC_ERROR;

                    default:
                        state->status = PREPROC_ERROR;
                        state->error_msg =
                                    "Preprocessor read mode was unknown value";
                        return PREPROC_ERROR;
                }

            case PREPROC_EOF:
                //returning an error might stop whatever loop the caller
                //is stuck in where it continued reading after PREPROC_EOF.
                state->status = PREPROC_ERROR;
                state->error_msg =
                              "read_preproc_char() called after EOF was read.";
                return PREPROC_ERROR;

            case PREPROC_END_OF_STRING:
                state->status = PREPROC_OK;
                state->read_mode = NORMAL_MODE;
                return read_preproc_char(pre);

            case PREPROC_ERROR:
                return PREPROC_ERROR;

            default:
                state->status = PREPROC_ERROR;
                state->error_msg =
                               "Preprocessor status was set to unknown value.";
                return PREPROC_ERROR;
        }
    }

    else//NULL pointer passed to function
    {
        return PREPROC_ERROR;
    }
}

//short description
static int read_string(preproc_state_t *state);

int read_preproc_extern_name(preproc_t pre)
{
    if(pre != NULL)
    {
        preproc_state_t *state = (preproc_state_t*)pre;
        switch(state->status)
        {
            case PREPROC_OK:
                switch(state->read_mode)
                {
                    case NORMAL_MODE:
                    case NAME_MODE:
                        state->status = PREPROC_ERROR;
                        state->error_msg = 
            "read_preproc_extern_name() called when no double quote was read.";
                        return PREPROC_ERROR;

                    case STRING_MODE:
                        state->read_mode = EXTERN_NAME_MODE;
                        return read_preproc_extern_name(pre);

                    case EXTERN_NAME_MODE:
                        return read_string(state);

                    case DATA_STRING_MODE:
                        state->status = PREPROC_ERROR;
                        state->error_msg = 
             "read_preproc_extern_name() called while reading a data string.";
                        return PREPROC_ERROR;

                    default:
                        state->status = PREPROC_ERROR;
                        state->error_msg =
                                    "Preprocessor read mode was unknown value";
                        return PREPROC_ERROR;
                }

            case PREPROC_EOF:
                //returning an error might stop whatever loop the caller
                //is stuck in where it continued reading after PREPROC_EOF.
                state->status = PREPROC_ERROR;
                state->error_msg =
                       "read_preproc_extern_name() called after EOF was read.";
                return PREPROC_ERROR;

            case PREPROC_END_OF_STRING:
                state->status = PREPROC_ERROR;
                state->error_msg =
             "read_preproc_extern_name() called after end of string was read.";
                return PREPROC_ERROR;

            case PREPROC_ERROR:
                return PREPROC_ERROR;

            default:
                state->status = PREPROC_ERROR;
                state->error_msg =
                               "Preprocessor status was set to unknown value.";
                return PREPROC_ERROR;
        }
    }

    else//NULL pointer passed to function
    {
        return PREPROC_ERROR;
    }
}

int read_preproc_data_string(preproc_t pre)
{
    if(pre != NULL)
    {
        preproc_state_t *state = (preproc_state_t*)pre;
        switch(state->status)
        {
            case PREPROC_OK:
                switch(state->read_mode)
                {
                    case NORMAL_MODE:
                    case NAME_MODE:
                        state->status = PREPROC_ERROR;
                        state->error_msg = 
            "read_preproc_data_string() called when no double quote was read.";
                        return PREPROC_ERROR;

                    case STRING_MODE:
                        state->read_mode = DATA_STRING_MODE;
                        return read_preproc_data_string(pre);

                    case EXTERN_NAME_MODE:
                        state->status = PREPROC_ERROR;
                        state->error_msg = 
           "read_preproc_data_string() called while reading an external name.";
                        return PREPROC_ERROR;

                    case DATA_STRING_MODE:

                        return read_string(state);

                    default:
                        state->status = PREPROC_ERROR;
                        state->error_msg =
                                    "Preprocessor read mode was unknown value";
                        return PREPROC_ERROR;
                }

            case PREPROC_EOF:
                //returning an error might stop whatever loop the caller
                //is stuck in where it continued reading after PREPROC_EOF.
                state->status = PREPROC_ERROR;
                state->error_msg =
                       "read_preproc_data_string() called after EOF was read.";
                return PREPROC_ERROR;

            case PREPROC_END_OF_STRING:
                state->status = PREPROC_ERROR;
                state->error_msg =
             "read_preproc_data_string() called after end of string was read.";
                return PREPROC_ERROR;

            case PREPROC_ERROR:
                return PREPROC_ERROR;

            default:
                state->status = PREPROC_ERROR;
                state->error_msg =
                               "Preprocessor status was set to unknown value.";
                return PREPROC_ERROR;
        }
    }

    else//NULL pointer passed to function
    {
        return PREPROC_ERROR;
    }
}

char *get_preproc_line_string(preproc_t pre, int line_number)
{
    //Check if the return value of get_utf8_file_line_string() is NULL
    if(pre != NULL)
    {
        preproc_state_t *state = (preproc_state_t*)pre;
        //get_utf8_file_line_length() declared in 
        size_t line_length = get_utf8_file_line_length(state->file,
                                                       line_number);
        //get_utf8_file_line_string() declared in 
        int32_t *line_string = get_utf8_file_line_string(state->file,
                                                         line_number);
        if(line_length == 0 || line_string == NULL)
        {
            return NULL;
        }

        else
        {
            //convert_string_to_utf8() declared in 
            return convert_string_to_utf8(&state->encoder,
                                          line_length,
                                          line_string);
        }
    }

    else
    {
        return "get_preproc_line_string() called with NULL pointer.";
    }
}

// *** private functions ***

//short description
static int32_t remove_extra_spaces(preproc_state_t *state);

//short description
static int32_t set_preproc_read_error(preproc_state_t *state, char *msg);

static int32_t read_normal_mode_char(preproc_state_t *state)
{
    int32_t ch = remove_extra_spaces(state);
    if(ch < 128)
    {
        //Be sure what this checks for.
        if(iscntrl((int)ch))
        {
            return set_preproc_read_error(state,
                                              "Unexpected control character.");
        }

        else
        {
            return ch;
        }
    }

    else
    {
        return set_preproc_read_error(state,
                                              "Unexpected unicode character.");
    }
}

static int32_t read_name_mode_char(preproc_state_t *state)
{
    int32_t ch = remove_extra_spaces(state);
    if(ch < 128)
    {
        switch(ch)
        {

        }
        //name:   $(<letter> | <digit> | _ | . | + | - | * | / | \ | ^ | ~ | = | < | > | ! | ? | @ | # | $ | % | & | | | : | ' | `)+
        //space switches state->read_mode to NORMAL_MODE
    }

    else
    {
        return set_preproc_read_error(state,
                                              "Unexpected unicode character.");
    }
}

static int read_next_byte_from_encoder_helper(preproc_state_t *state)
{
    //read_next_byte_from_encoder() declared in 
    int byte = read_next_byte_from_encoder(&state->encoder);
    if(byte < 0)
    {
        state->status = PREPROC_ERROR;
        if(byte == ENCODER_ERROR)
        {
            //get_utf8_encoder_error_msg() declared in 
            state->error_msg = get_utf8_encoder_error_msg(&state->encoder);
        }

        else
        {
            state->error_msg =
              "read_next_byte_from_encoder() returned unknown negative value.";
        }

        return PREPROC_ERROR;
    }

    else
    {
        return byte;
    }
}

//short description
static int32_t convert_escape_sequences(preproc_state_t *state);

//short description
static int32_t set_read_error_if_eof(preproc_state_t *state,
                                     int32_t ch,
                                     char *msg);

static int read_string(preproc_state_t *state)
{
    //encoding to utf8 is done for everything except byte escapes meaning
    //the encoding needs to be done when checking for escape sequences or
    //the encoder needs some way to pass along a byte.
    if(is_utf8_encoder_empty(&state->encoder))
    {
        int32_t ch = convert_escape_sequences(state);
        if(ch < 0)
        {
            //cast to int or set state without this function
            return set_read_error_if_eof(state, ch,
                                                  "Unexpected EOF in string.");
        }

        else
        {
            convert_codepoint_to_utf8(&state->encoder, ch);
            return read_next_byte_from_encoder(&state->encoder)
        }
    }

    else
    {
        return read_next_byte_from_encoder(&state->encoder);
    }
}
static int read_extern_name_byte(preproc_state_t *state)
{
    if(is_utf8_encoder_empty(&state->encoder))
    {

    }

    else
    {
        return read_next_byte_from_encoder(&state->encoder);
    }
}

static int32_t read_extern_name_char(preproc_state_t *state)
{

}
//short description
static int32_t insert_space_after_symbols(preproc_state_t *state);

static int32_t remove_extra_spaces(preproc_state_t *state)
{
    int32_t ch = insert_space_after_symbols(state);
    if(state->last_read == ' ')
    {
        while(ch == ' ')
        {
            ch = insert_space_after_symbols(state);
        }
    }

    return ch;
}

static int32_t set_preproc_read_error(preproc_state_t *state, char *msg)
{
    state->status = PREPROC_ERROR;
    state->error_msg = msg;
    return PREPROC_ERROR;
}

//short description
static int32_t remove_escaped_newlines(preproc_state_t *state);

//short description
static int32_t read_byte_escape(preproc_state_t *state, int32 ch);

//short description
static int32_t read_unicode_escape(preproc_state_t *state);

static int32_t convert_escape_sequences(preproc_state_t *state)
{
    //This uses remove_escaped_newlines() for reading and converts whitespace
    //escapes and unicode escapes for both modes but only byte escapes for
    //DATA_MODE
    //check for end of string here too
    int32_t ch = remove_escaped_newlines(state);
    if(ch == '\"')
    {
        state->status = PREPROC_END_OF_STRING;
        return PREPROC_END_OF_STRING;
    }

    else
    {
        if(ch == '\\')
        {
            int32_t next_ch = remove_escaped_newlines(state);
            //set_preproc_read_error() in case next_ch < 0
            switch(next_ch)
            {
                case '\"':
                    return '\"';
                case '\'':
                    return '\'';
                case '\\':
                    return '\\';
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case 'A':
                case 'B':
                case 'C':
                case 'D':
                case 'E':
                case 'F':
                case 'a':
                case 'b':
                case 'c':
                case 'd':
                case 'e':
                case 'f':
                    return read_byte_escape(state, next_ch);
                case 'n':
                    return '\n';
                case 't':
                    return '\t';
                case 'u':
                    return read_unicode_escape(state);
                default:
                    return set_preproc_read_error(state,
                                                   "Unknown escape sequence.");
            } 
        }

        else
        {
            return ch;
        }
    }
}

static int32_t set_read_error_if_eof(preproc_state_t *state,
                                     int32_t ch,
                                     char *msg)
{
    if(ch == PREPROC_EOF)
    {
        return set_preproc_read_error(state, msg);
    }

    else
    {
        return ch;
    }
}

//short description
static int32_t convert_whitespace(preproc_state_t *state);

static int32_t insert_space_after_symbols(preproc_state_t *state)
{
    //Maybe add open square bracket
    switch(state->last_read)
    {
        case '\"':
            state->read_mode = STRING_MODE;
        case '(':
        case ')':
            return ' ';
        default:
            return convert_whitespace(state);
    }
}

//short description
static int32_t read_char(preproc_state_t *state);

//short description
static void set_next_read(preproc_state_t *state, int32_t next_ch);

static int32_t remove_escaped_newlines(preproc_state_t *state)
{
    int32_t ch = read_char(state);
    while(ch == '\\')
    {
        int32_t next_ch = read_char(state);
        if(next_ch == '\n')
        {
            ch = read_char(state);
        }

        else
        {
            set_next_read(state, next_ch);
            break;
        }
    }

    return ch;
}

static int32_t read_byte_escape(preproc_state_t *state, int32 ch)
{
    //This should probably return an int because the encoder might choke on this
    //read_byte_escape_from_utf8_file() should return an int too.
    if(state->read_mode == DATA_STRING_MODE)
    {
        int32_t byte =
        read_byte_escape_from_utf8_file(&state->file, ch);

        if(byte < 0)
        {
            state->error_msg = get_utf8_file_error_msg(&state->file);
            return PREPROC_ERROR;
        }

        else
        {
            return byte;
        }
    }

    else
    {
        return set_preproc_read_error(state,
                                 "Byte escape found in external name string.");
    }
}

static int32_t read_unicode_escape(preproc_state_t *state)
{
    int32_t ch = read_unicode_escape_from_utf8_file(&state->file);
    if(ch < 0)
    {
        state->status = PREPROC_ERROR;
        state->error_msg = get_utf8_file_error_msg(&state->file);
        return PREPROC_ERROR;
    }

    else
    {
        return ch;
    }
}

//short description
static int32_t remove_comments(preproc_state_t *state);

static int32_t convert_whitespace(preproc_state_t *state)
{
    int32_t ch = remove_comments(state);
    if(ch == '\n' || ch == '\t')
    {
        return ' ';
    }

    else
    {
        return ch;
    }
}

static int32_t read_char(preproc_state_t *state)
{
    if(state->return_temp_next)
    {
        state->return_temp_next = false;
        return state->temp;
    }

    else
    {
        int32_t ch = read_char_from_utf8_file(&state->file);
        if(ch < 0)
        {
            if(ch == UTF8_FILE_EOF)
            {
                state->status = PREPROC_EOF;
            }

            else
            {
                state->status = PREPROC_ERROR;
                state->error_msg = get_utf8_file_error_msg(&state->file);
            }

            return state->status;
        }

        else
        {
            return ch;
        }
    }
}

static void set_next_read(preproc_state_t *state, int32_t next_ch)
{
    if(state->return_temp_next == false)
    {
        state->temp = next_ch;
        state->return_temp_next = true;
    }

    else
    {
        state->status = PREPROC_ERROR;
        state->error_msg =
                       "Preprocessor tried to set preproc_state_t.temp twice.";
    }
}

//short description
static int32_t insert_space_before_symbols(preproc_state_t *state);

//short description
static int32_t remove_multiline_comment(preproc_state_t *state);

//short description
static int32_t remove_line_comment(preproc_state_t *state);

static int32_t remove_comments(preproc_state_t *state)
{
    int32_t ch = insert_space_before_symbols(state);
    if(ch == '(')
    {
        int32_t next_ch = read_char(state);
        if(next_ch == ';')
        {
            ch = remove_multiline_comment(state);
        }

        else //Not a comment
        {
            set_next_read(state, next_ch);
        }

        if(ch == ';')
        {
            int32_t next_ch = read_char(state);
            if(next_ch == ';')
            {
                ch = remove_line_comment(state);
            }

            else
            {
                if(next_ch == ')')
                {
                    // ;) is a winky error
                    return set_preproc_read_error(state,
                                                 "Unexpected end of comment.");
                }

                else
                {
                    //This probably should be an error, I don't think
                    //semicolons are used for anything else
                    set_next_read(state, next_ch);
                }
            }
        }
    }

    return ch;
}

static int32_t insert_space_before_symbols(preproc_state_t *state)
{
    //spaces before '"' '(' ')' ';'
    //I'd like to insert a space before names but a $ can appear in a name
    if(state->last_read == ' ')
    {
        return read_char(state);
    }

    else
    {
        int32_t ch = read_char(state);
        switch(ch)
        {
            case '$':
                if(state->read_mode == NAME_MODE)
                {
                    return ch;
                }
            case '"':
            case '(':
            case ')':
            case ';':
                //emit space and push ch for next read
                set_next_read(state, ch);
                return ' ';
            default:
                return ch;
        }
    }
}

static int32_t remove_multiline_comment(preproc_state_t *state)
{
    //Keep count of how deep the comments are nested
    int depth = 1;
    while(depth > 0)
    {
        int32_t ch = read_char(state);
        if(ch == PREPROC_EOF)
        {
            return set_preproc_read_error(state,
                                  "End of file reached before end of comment");
        }

        if(ch == '(')
        {
            int32_t next_ch = read_char(state);
            if(next_ch == ';')
            {
                depth++;
            }

            else
            {
                set_next_read(state, next_ch);
            }
        }

        if(ch == ';')
        {
            int32_t next_ch = read_char(state);
            if(next_ch == ')')
            {
                depth--;
            }

            else
            {
                set_next_read(state, next_ch);
            }
        }
    }

    return insert_space_before_symbols(state);
}

static int32_t remove_line_comment(preproc_state_t *state)
{
    int32_t ch = read_char(state);
    while(ch != '\n')
    {
        if(ch == UTF8_FILE_EOF)
        {
            return ch;
        }

        else
        {
            ch = read_char(state);
        }
    }

    return insert_space_before_symbols(state);
}

// *** end of file "preprocessor.c" ***
