#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "utf8_file.h"

typedef struct
{
    FILE    *fp;
    int32_t status;
    bool    return_temp_next;
    int32_t temp;
    char    *error_msg;
    int     line;
    int     line_position;
} utf8_file_state_t;

#define BYTE_ORDER_MARKER 0xFEFF

//Read codepoint from file but does not cause an error for BOM or change
//position in file counters.
static int32_t read_utf8(utf8_file_state_t *state);

utf8_file_t open_utf8_file(char *filename)
{
    utf8_file_state_t *new = malloc(sizeof(utf8_file_state_t));
    if(new != NULL)
    {
        new->fp = fopen(filename, "r");
        if(new->fp != NULL)
        {
            reset_utf8_file_status((utf8_file_t)new);
            int32_t initial_read = read_utf8(new);
            if(initial_read != BYTE_ORDER_MARKER)
            {
                new->temp = initial_read;
                new->return_temp_next = true;
            }

            return (utf8_file_t)new;
        }

        else
        {
            new->status = UTF8_FILE_ERROR;
            new->error_msg = "open_utf8_file() error: unable to open file.";
            return (utf8_file_t)new;
        }
    }

    else
    {
        return NULL;
    }
}

void close_utf8_file(utf8_file_t file)
{
    if(file != NULL)
    {
        utf8_file_state_t *state = (utf8_file_state_t*)file;
        fclose(state->fp);
        free(file);
    }
}

int32_t get_utf8_file_status(utf8_file_t file)
{
    if(file != NULL)
    {
        utf8_file_state_t *state = (utf8_file_state_t*)file;
        return state->status;
    }

    else
    {
        return UTF8_FILE_ERROR;
    }    
}

char *get_utf8_file_error_msg(utf8_file_t file)
{
    if(file != NULL)
    {
        utf8_file_state_t *state = (utf8_file_state_t*)file;
        return state->error_msg;
    }

    else
    {
        return "Failed to allocate memory for utf8_file_t.";
    }
}

int get_utf8_file_line_number(utf8_file_t file)
{
    if(file != NULL)
    {
        utf8_file_state_t *state = (utf8_file_state_t*)file;
        return state->line;
    }

    else
    {
        return -1;
    }
}

int get_utf8_file_line_position(utf8_file_t file)
{
    if(file != NULL)
    {
        utf8_file_state_t *state = (utf8_file_state_t*)file;
        return state->line_position;
    }

    else
    {
        return -1;
    }
}

int32_t pop_byte_from_utf8_file(utf8_file_t file)
{
    if(file != NULL)
    {
        utf8_file_state_t *state = (utf8_file_state_t*)file;
        if(state->status < 0)
        {
            return state->status;
        }

        int32_t ch = fgetc(state->fp);
        if(ch == EOF)
        {
            state->status = UTF8_FILE_EOF;
            state->error_msg = "Normal EOF reached.";
            return state->status;
        }

        else
        {
            return ch;
        }
    }

    else
    {
        return UTF8_FILE_ERROR;
    }
}

void reset_utf8_file_status(utf8_file_t file)
{
    if(file != NULL)
    {
        utf8_file_state_t *state = (utf8_file_state_t*)file;
        state->status = UTF8_FILE_OK;
        state->return_temp_next = false;
        state->temp = ' ';
        state->error_msg = "File open, no errors.";
        state->line = 1;
        state->line_position = 0;
    }
}

//Calls read_utf8() and possibly increments file position counters before
//returning what was read.
static int32_t read_and_update_position(utf8_file_state_t *state);

//Sets status and error message and returns UTF8_FILE_ERROR.
static int32_t set_utf8_file_read_error(utf8_file_state_t *state, char *msg);

int32_t read_char_from_utf8_file(utf8_file_t file)
{
    if(file != NULL)
    {
        utf8_file_state_t *state = (utf8_file_state_t*)file;
        int32_t ch = read_and_update_position(state);
        //open_char_stream() will eat the BOM at the beginning of the file if there
        //is any, afterwards BOM will generate an error
        if(ch != BYTE_ORDER_MARKER)
        {
            return ch;
        }

        else
        {
            return set_utf8_file_read_error(state,
                                        "Byte Order Marker read in file.");
        }
    }

    else
    {
        return UTF8_FILE_ERROR;
    }
}

//Returns the value of a hex digit or -1 if not hex digit.
static int32_t hex_digit_to_int(int32_t ch);

//EOF is an error in escape sequences, if ch is EOF it will set error_msg
//to msg, otherwise the negative value was another error and error_msg will
//stay the same.
static int32_t set_read_error_if_eof(utf8_file_state_t *state,
                                     int32_t ch,
                                     char *msg);

//Checks if codepoint falls in the correct ranges. If it is it returns the
//codepoint. If not it sets the error_msg and returns UTF8_FILE_ERROR.
static int32_t is_code_point_valid(utf8_file_state_t *state,
                                   int32_t codepoint);

int32_t read_unicode_escape_from_utf8_file(utf8_file_t file)
{
    if(file != NULL)
    {
        utf8_file_state_t *state = (utf8_file_state_t*)file;
        //Make sure read_char_from_utf8_file is used because it checks for BOM
        int32_t ch = read_char_from_utf8_file(file);
        if(ch == '{')
        {
            int32_t codepoint = 0;
            ch = read_utf8(state);
            int hex_counter = 1;
            do
            {
                if(ch < 0)
                {
                    return set_read_error_if_eof(state, ch,
                                 "Unexpected EOF in unicode escape sequence.");
                }

                else
                {
                    int32_t hex = hex_digit_to_int(ch);
                    if(hex >= 0)
                    {
                        hex_counter++;
                        codepoint = (codepoint << 4) + hex;
                    }

                    else
                    {
                        return set_utf8_file_read_error(state,
                             "Expected hex digit in unicode escape sequence.");
                    }
                }

                ch = read_utf8(state);
            }while(ch != '}' && hex_counter < 7);

            if(ch == '}')
            {
                return is_code_point_valid(state, codepoint);
            }

            else
            {
                return set_utf8_file_read_error(state,
         "A maximum of 6 hex digits are allowed in unicode escape sequences.");
            }
        }

        else //First read from escape sequence not '{'
        {
            if(ch < 0)
            {
                return set_read_error_if_eof(state, ch,
                                 "Unexpected EOF in unicode escape sequence.");
            }
        
            else
            {
                return set_utf8_file_read_error(state,
                                 "Expected \'{\' in unicode escape sequence.");
            }
        }
    }

    else
    {
        return UTF8_FILE_ERROR;
    }
}

int32_t read_byte_escape_from_utf8_file(utf8_file_t file, int32_t first_ch)
{
    if(file != NULL)
    {
        utf8_file_state_t *state = (utf8_file_state_t*)file;
        int32_t first_digit = hex_digit_to_int(first_ch);
        if(first_digit < 0)
        {
            return set_utf8_file_read_error(state,
"First character in byte escape wasn\'t hex digit (preprocessor did something wrong).");
        }

        else
        {
            int32_t second_ch = read_char_from_utf8_file(file);
            if(second_ch < 0)
            {
                return set_read_error_if_eof(state, second_ch,
                                    "Unexpected EOF in byte escape sequence.");
            }

            else
            {
                int32_t second_digit = hex_digit_to_int(second_ch);
                if(second_digit < 0)
                {
                    return set_utf8_file_read_error(state,
                         "Second character in byte escape wasn\'t hex digit.");
                }

                else
                {
                    return ((first_digit << 4) + second_digit);
                }
            }
        }
    }

    else
    {
        return UTF8_FILE_ERROR;
    }
}

size_t get_utf8_file_line_length(utf8_file_t file, int line_number)
{
    return 0;
}

int32_t *get_utf8_file_line_string(utf8_file_t file, int line_number)
{
    return NULL;
}

// *** private functions ***

static int32_t read_utf8(utf8_file_state_t *state)
{
    if(state->status < 0)
    {
        return state->status;
    }

    int32_t ch = fgetc(state->fp); //Casting int to int32_t
    if(ch == EOF)
    {
        state->status = UTF8_FILE_EOF;
        state->error_msg = "Normal EOF reached.";
        return state->status;
    }

    //Return 7bit character
    if(ch < 128)
    {
        return ch;
    }

    if(ch < 192 || ch > 247) //128-191 = 10xxxxxx
    {
        return set_utf8_file_read_error(state,
                                   "UTF8 sequence starts with invalid byte.");
    }

    //Read initial byte of multibyte sequence
    int32_t codepoint = 0;
    int number_of_additional_bytes = 0;
    if(ch < 224) //192-223 = 110xxxxx
    {
        codepoint = ch & 31; //31 = 11111
        number_of_additional_bytes = 1;

        //The second byte of a two byte utf8 codepoint only holds six bits
        //so the bits taken from the first byte must be greater than 0.
        if(codepoint == 0)
        {
            return set_utf8_file_read_error(state,
                           "Unicode codepoint is padded (first byte of two).");
        }
    }
    else
    {
        if(ch < 240) //224-239 = 1110xxxx
        {
            codepoint = ch & 15; //1111
            number_of_additional_bytes = 2;
        }

        else
        {
            codepoint = ch & 7; //7 = 111
            number_of_additional_bytes = 3;
        }
    }

    //Get additional bytes of multibyte sequence
    for(int i = 0; i < number_of_additional_bytes; i++)
    {
        ch = fgetc(state->fp); //Casting int to int32_t
        if(ch < 0)
        {
            return set_utf8_file_read_error(state,
                                           "Unexpected EOF in UTF8 sequence.");
        }

        if(ch < 128 || ch > 191) //128-191 = 10xxxxxx
        {
            return set_utf8_file_read_error(state,
                                            "Invalid byte in UTF8 sequence.");
        }

        codepoint = codepoint << 6;
        codepoint = codepoint + (ch & 63); //63 = 111111
        //After reading the first and second byte the codepoint should be more
        //than 0 in all cases unless it is padded with zeroes
        if(codepoint == 0)
        {
            return set_utf8_file_read_error(state,
                                 "Unicode codepoint is padded (second byte).");
        }
    }

    return is_code_point_valid(state, codepoint);
}

static int32_t convert_newlines(utf8_file_state_t *state);

static int32_t read_and_update_position(utf8_file_state_t *state)
{
    //This should probably check the status to increment the line_position
    //exactly once when a read error is encountered
    int32_t ch = convert_newlines(state);
    if(ch >= 0)
    {
        if(ch == '\n')
        {
            state->line++;
            state->line_position = 0;
        }

        else
        {
            state->line_position++;
        }
    }

    return ch;
}

static int32_t set_utf8_file_read_error(utf8_file_state_t *state, char *msg)
{
    state->status = UTF8_FILE_ERROR;
    state->error_msg = msg;
    return UTF8_FILE_ERROR;
}

static int32_t hex_digit_to_int(int32_t ch)
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

    return -1;
}

static int32_t set_read_error_if_eof(utf8_file_state_t *state,
                                     int32_t ch,
                                     char *msg)
{
    if(ch == UTF8_FILE_EOF)
    {
        return set_utf8_file_read_error(state, msg);
    }

    else
    {
        //Pass on any read_utf8 errors
        return ch;
    }
}

static int32_t is_code_point_valid(utf8_file_state_t *state, int32_t codepoint)
{
    //There's other checks that might be generate an error. Things like
    //codepoints in the private use areas or noncharacters could be allowed
    //in the data section but nowhere else.

    //There's a gap in the basic multilingual plane to allow utf16 encodings
    //to encode the characters added later with two 16bit characters
    if(codepoint >= 0xd800 && codepoint <= 0xdfff)
    {
        return set_utf8_file_read_error(state,
                                      "Unicode codepoint is UTF16 surrogate.");
    }

    //utf8 can store a wider range of values than utf16 but there isn't any
    //need for additional codepoints anytime soon so 0x10ffff is the maximum
    //value used in Unicode 
    if(codepoint > 0x10ffff)
    {
        return set_utf8_file_read_error(state,
                                        "Unicode codepoint out of range.");
    }

    return codepoint;
}

static int32_t convert_newlines(utf8_file_state_t *state)
{
    int32_t ch;
    if(state->return_temp_next)
    {
        ch = state->temp;
        state->return_temp_next = false;
    }

    else
    {
        ch = read_utf8(state);
    }

    if(ch == '\r')
    {
        int32_t next_ch = read_utf8(state);
        if(next_ch != '\n')
        {
            state->temp = next_ch;
            state->return_temp_next = true;
        }

        return '\n';
    }

    else
    {
        return ch;
    }
}

// *** end of file "utf8_file.c" ***
