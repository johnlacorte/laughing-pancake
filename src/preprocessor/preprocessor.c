//I think that the code for reading byte escape sequences might be better
//in utf8_file.
//I think I might add a NAME_MODE and a read_preproc_name() function. If I add
//this insert_space_before_symbols() should add a space before $.
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "utf8_file/utf8_file.h"
#include "preprocessor.h"

typedef struct
{
    int remaining_bytes;
    int bytes[3];
} utf8_encoder_t;

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

#define STRING_MODE 1

#define EXTERN_NAME_MODE 2

#define DATA_MODE 3

static preproc_state_t *get_state(preproc_t pre);

static void init_utf8_encoder(preproc_state_t *state);

preproc_t open_preproc(char *filename)
{
    preproc_state_t *new = malloc(sizeof(preproc_state_t));
    if(new != NULL)
    {
        init_utf8_file(&new->file);
        if(open_utf8_file(&new->file, filename))
        {
            //Some of this should be moved to reset_preproc_status()
            init_utf8_encoder(new);
            new->status = PREPROC_OK;
            new->read_mode = NORMAL_MODE;
            new->last_read = ' ';
            new->return_temp_next = false;
            new->temp = ' ';
            new->error_msg = "Preprocessor okay.";
        }

        else
        {
            new->status = PREPROC_ERROR;
            new->read_mode = NORMAL_MODE;
            new->last_read = ' ';
            new->return_temp_next = false;
            new->temp = ' ';
            new->error_msg = get_utf8_file_error_msg(&new->file);
        }
    }

    return (preproc_t)new;
}

void free_preproc(preproc_t pre)
{
    if(pre != NULL)
    {
        preproc_state_t *state = get_state(pre);
        close_utf8_file(&state->file);
        free(pre);
    }
}

int get_preproc_status(preproc_t pre)
{
    if(pre != NULL)
    {
        preproc_state_t *state = get_state(pre);
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
        preproc_state_t *state = get_state(pre);
        return state->error_msg;
    }

    else
    {
        return "get_preproc_error_msg() passed NULL pointer.";
    }
}

int get_preproc_line_number(preproc_t pre)
{
    if(pre != NULL)
    {
        preproc_state_t *state = get_state(pre);
        return get_utf8_file_line_number(&state->file);
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
        preproc_state_t *state = get_state(pre);
        return get_utf8_file_line_position(&state->file);
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

static int set_preproc_read_error(preproc_state_t *state, char *msg);

static int32_t is_char_normal_mode_char(preproc_state_t *state);

int read_preproc_char(preproc_t pre)
{
    if(pre != NULL)
    {
        preproc_state_t *state = get_state(pre);
        if(state->status == PREPROC_OK)
        {
            if(state->read_mode == NORMAL_MODE || state->read_mode == STRING_MODE)
            {
                int32_t ch = is_char_normal_mode_char(state);
                state->last_read = ch;
                //cast to int
                return ch;
            }

            else
            {
                return set_preproc_read_error(state,
                "read_preproc_char() called before string was entirely read.");
            }
        }

        else
        {
            if(state->status == PREPROC_END_OF_STRING)
            {
                state->status = PREPROC_OK;
                return read_preproc_char(pre);
            }

            else
            {
                return state->status;
            }
        }
    }

    else//NULL pointer passed to function
    {
        return PREPROC_ERROR;
    }
}

static int read_string(preproc_state_t *state);

int read_preproc_extern_name(preproc_t pre)
{
    if(pre != NULL)
    {
        preproc_state_t *state = get_state(pre);
        if(state->status == PREPROC_OK)
        {
            //if normal mode check for quote and set mode
            if(state->read_mode == NORMAL_MODE)
            {
                //find initial quote
                state->read_mode = STRING_MODE;
                //By switching to STRING_MODE here read_preproc_char() doesn't
                //return an error for a double quote. 
                int32_t ch = read_preproc_char(pre);
                if(ch == '\"')
                {
                    state->read_mode = EXTERN_NAME_MODE;
                }

                else
                {
                    return set_preproc_read_error(state, "Expected string.");
                }
            }

            //check if read_mode is DATA_MODE
            else
            {
                if(state->read_mode == EXTERN_NAME_MODE)
                {
                    return read_string(state);
                }

                else
                {
                    return set_preproc_read_error(state,
             "read_preproc_extern_name() called while reading a data string.");
                }
            }
        }

        else
        {
            return state->status;
        }
    }

    else
    {
        return PREPROC_ERROR;
    }
}

int read_preproc_data(preproc_t pre)
{
    if(pre != NULL)
    {
        preproc_state_t *state = get_state(pre);
        if(state->status == PREPROC_OK)
        {
            //if normal mode check for quote and set mode
            if(state->read_mode == NORMAL_MODE)
            {
                //find initial quote
                state->read_mode = STRING_MODE;
                int32_t ch = read_preproc_char(pre);
                if(ch == '\"')
                {
                    state->read_mode = DATA_MODE;
                }

                else
                {
                    return set_preproc_read_error(state, "Expected string.");
                }
            }

            //check if read_mode is DATA_MODE
            else
            {
                if(state->read_mode == DATA_MODE)
                {
                    return read_string(state);
                }

                else
                {
                    return set_preproc_read_error(state,
             "read_preproc_data() called while reading an external name.");
                }
            }
        }

        else
        {
            return state->status;
        }
    }

    else
    {
        return PREPROC_ERROR;
    }
}

//local functions

static preproc_state_t *get_state(preproc_t pre)
{
    return (preproc_state_t *)pre;
}

static void init_utf8_encoder(preproc_state_t *state)
{
    utf8_encoder_t *encoder = &state->encoder;
    encoder->remaining_bytes = 0;
    encoder->bytes[0] = 0;
    encoder->bytes[1] = 0;
    encoder->bytes[2] = 0;
}

static int set_preproc_read_error(preproc_state_t *state, char *msg)
{
    set_utf8_file_read_error(&state->file, msg);
    state->status = PREPROC_ERROR;
    state->error_msg = get_utf8_file_error_msg(&state->file);
    return PREPROC_ERROR;
}

#include <ctype.h>

static int32_t remove_extra_spaces(preproc_state_t *state);

static int32_t is_char_normal_mode_char(preproc_state_t *state)
{
    int32_t ch = remove_extra_spaces(state);
    if(ch < 128)
    {
        //STRING_MODE exist just to have a way to read the first double quote
        //of a string using read_preproc_char()
        if(state->read_mode == NORMAL_MODE)
        {
            if(ch == '\"')
            {
                return set_preproc_read_error(state,
                                                         "Unexpected string.");
            }
        }

        if(iscntrl(ch))
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

static bool is_utf8_encoder_empty(preproc_state_t *state);

static int32_t convert_escape_sequences(preproc_state_t *state);

static int32_t set_read_error_if_eof(preproc_state_t *state,
                                     int32_t ch,
                                     char *msg);

static int convert_codepoint_to_utf8(preproc_state_t *state, int32_t ch);

static int read_next_byte_from_encoder(preproc_state_t *state);

static int read_string(preproc_state_t *state)
{
    if(is_utf8_encoder_empty(state))
    {
        int32_t ch = convert_escape_sequences(state);
        if(ch < 0)
        {
            //cast to int
            return set_read_error_if_eof(state, ch,
                                                  "Unexpected EOF in string.");
        }

        else
        {
            return convert_codepoint_to_utf8(state, ch);
        }
    }

    else
    {
        return read_next_byte_from_encoder(state);
    }
}

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

static bool is_utf8_encoder_empty(preproc_state_t *state)
{
    //To be safe I might be better returning true if it is less than zero too
    utf8_encoder_t *encoder = &state->encoder;
    return (encoder->remaining_bytes <= 0);
}

static int32_t remove_escaped_newlines(preproc_state_t *state);

static int32_t hex_to_byte(preproc_state_t *state, int32_t first_digit, int32_t second_digit);

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
                    return hex_to_byte(state, next_ch,
                                         remove_escaped_newlines(state));
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

static int convert_codepoint_to_utf8(preproc_state_t *state, int32_t ch)
{
    //should probably check if encoder is empty and return an error if not
    //do I need to cast anything to an int?
    if(ch < 128)
    {
        return ch;
    }

    else
    {
        utf8_encoder_t *encoder = &state->encoder;
        encoder->bytes[2] = 128 + (ch & 63); //128-191 = 10xxxxxx, 63 = 111111
        ch = ch >> 6;
        if(ch < 32) //31 = 11111
        {
            encoder->remaining_bytes = 1;
            return 192 + ch; //192-223 = 110xxxxx
        }

        else
        {
            encoder->bytes[1] = 128 + (ch & 63);
            ch = ch >> 6;
            if(ch < 16) //15 = 1111
            {
                encoder->remaining_bytes = 2;
                return 224 + ch; //224-239 = 1110xxxx
            }

            else
            {
                encoder->bytes[0] = 128 + (ch & 63);
                ch = ch >> 6;
                if(ch < 8) //7 = 111
                {
                    encoder->remaining_bytes = 3;
                    return 240 + ch; //240-247 = 11110xxx
                }

                else
                {
                    //codepoint too big?
                    state->status = PREPROC_ERROR;
                    state->error_msg = "";
                    //set state->error_msg
                }
            }
        }
    }
}

static int read_next_byte_from_encoder(preproc_state_t *state)
{
    utf8_encoder_t *encoder = &state->encoder;
    if(encoder->remaining_bytes > 0)
    {
        int byte = 3 - encoder->bytes[encoder->remaining_bytes];
        encoder->remaining_bytes--;
        return byte;
    }

    else
    {
        state->status = PREPROC_ERROR;
        state->error_msg = "Preprocessor tried reading from empty utf8 encoder.";
        //set state->error_msg
        return PREPROC_ERROR; 
    }
}

static int32_t convert_whitespace(preproc_state_t *state);

static int32_t insert_space_after_symbols(preproc_state_t *state)
{
    //I think I maybe should remove the quote from here. If the last character
    //read before reading a string was a quote the next character read after
    //will be a space and instead I want the next character to be the next
    //nonwhitespace character, the first character of the next token. 
    if(state->last_read == '(' || state->last_read == ')' || state->last_read == '\"')
    {
        return ' ';
    }

    else
    {
        return convert_whitespace(state);
    }
}

static int32_t read_char(preproc_state_t *state);

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

static int32_t hex_value(preproc_state_t *state, int32_t ch);

static int32_t hex_to_byte(preproc_state_t *state, int32_t first_digit, int32_t second_digit)
{
    if(state->read_mode == DATA_MODE)
    {
        int32_t byte = 0;
        int32_t digit = hex_value(state, first_digit);
        if(digit >= 0)
        {
            byte = digit * 16;
            digit = hex_value(state, second_digit);
            if(digit >= 0)
            {
                byte = byte + digit;
                return byte;
            }
        }

        //returns first digit that is negative. Should be PREPROC_ERROR.
        return digit;
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

static int32_t hex_value(preproc_state_t *state, int32_t ch)
{
    if(ch < 0)
    {
        return ch;
    }

    else
    {
        if(ch >= '0')
        {
            if(ch <= '9')
            {
                return (ch - '0');
            }

            else
            {
                if(ch >= 'A')
                {
                    if(ch <= 'F')
                    {
                        return (ch - 'A' + 9);
                    }

                    else
                    {
                        if(ch >= 'a')
                        {
                            if(ch <= 'f')
                            {
                                return (ch - 'a' + 9);
                            }
                        }
                    }
                }
            }
        }

        return set_preproc_read_error(state, "Character that isn't a hex digit in byte escape.");
    }
}

static int32_t insert_space_before_symbols(preproc_state_t *state);

static int32_t remove_multiline_comment(preproc_state_t *state);

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

/* end of file "preprocessor.c" */
