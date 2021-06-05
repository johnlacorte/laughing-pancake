#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "utf8_file.h"

void init_utf8_file(utf8_file_t *file)
{
    file->fp = NULL;
    file->status = UTF8_FILE_CLOSED;
    file->return_temp_next = false;
    file->temp = ' ';
    file->error_msg = "Preprocessor tried reading when no file is open.";
    file->line = 1;
    file->line_position = 0;
}

#define BYTE_ORDER_MARKER 0xFEFF

static int32_t read_utf8(utf8_file_t *file);

bool open_utf8_file(utf8_file_t *file, char *filename)
{
    //status should be UTF8_FILE_CLOSED if instead its 0 struct was never
    //initialized any other value the file already was open
    if(file->status == UTF8_FILE_CLOSED)
    {
        file->fp = fopen(filename, "r");
        if(file->fp != NULL)
        {
            reset_utf8_file_status(file);
            int32_t initial_read = read_utf8(file);
            if(initial_read != BYTE_ORDER_MARKER)
            {
                file->temp = initial_read;
                file->return_temp_next = true;
            }

            return true;
        }

        else
        {
            file->status = UTF8_FILE_ERROR;
            file->error_msg = "Prepocessor error: Error opening utf8_file_t.";
            return false;
        }
    }

    else
    {
        file->status = UTF8_FILE_ERROR;
        file->error_msg = "Preprocessor error: utf8_file_t already open.";
        return false;
    }
}

void close_utf8_file(utf8_file_t *file)
{
    fclose(file->fp);
    init_utf8_file(file);
}

int32_t get_utf8_file_status(utf8_file_t *file)
{
    return file->status;
}

char *get_utf8_file_error_msg(utf8_file_t *file)
{
    return file->error_msg;
}

int32_t set_utf8_file_read_error(utf8_file_t *file, char *msg)
{
    file->status = UTF8_FILE_ERROR;
    sprintf(file->error_msg_buffer,
            "On line %d, character %d:\n    %s\n",
            file->line,
            file->line_position,
            msg);
    return UTF8_FILE_ERROR;
}

int get_utf8_file_line_number(utf8_file_t *file)
{
    return file->line;
}

int get_utf8_file_line_position(utf8_file_t *file)
{
    return file->line_position;
}

int32_t pop_byte_from_utf8_file(utf8_file_t *file)
{
    if(file->status < 0)
    {
        if(file->status == UTF8_FILE_CLOSED)
        {
            file->status = UTF8_FILE_ERROR;
        }

        return file->status;
    }

    int32_t ch = fgetc(file->fp);
    if(ch == EOF)
    {
        file->status = UTF8_FILE_EOF;
        file->error_msg = "Normal EOF reached.";
        return file->status;
    }

    else
    {
        return ch;
    }
}

void reset_utf8_file_status(utf8_file_t *file)
{
    file->status = UTF8_FILE_OK;
    file->return_temp_next = false;
    file->temp = ' ';
    file->error_msg = "File open, no errors.";
    file->line = 1;
    file->line_position = 0;
}

static int32_t read_and_update_position(utf8_file_t *file);

int32_t read_char_from_utf8_file(utf8_file_t *file)
{
    int32_t ch = read_and_update_position(file);
    //open_char_stream() will eat the BOM at the beginning of the file if there
    //is any, afterwards BOM will generate an error
    if(ch != BYTE_ORDER_MARKER)
    {
        return ch;
    }

    else
    {
        return set_utf8_file_read_error(file,
                                        "Byte Order Marker read in file.");
    }
}

static int32_t hex_digit_to_int(int32_t ch);

static int32_t set_read_error_if_eof(utf8_file_t *file, int32_t ch, char *msg);

static int32_t is_code_point_valid(utf8_file_t *file, int32_t codepoint);

int32_t read_unicode_escape_from_utf8_file(utf8_file_t *file)
{
    //Make sure read_char_from_utf8_file is used because it checks for BOM
    int32_t ch = read_char_from_utf8_file(file);
    if(ch == '{')
    {
        int32_t codepoint = 0;
        ch = read_utf8(file);
        int hex_counter = 1;
        do
        {
            if(ch < 0)
            {
                return set_read_error_if_eof(file, ch,
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
                    return set_utf8_file_read_error(file,
                             "Expected hex digit in unicode escape sequence.");
                }
            }

            ch = read_utf8(file);
        }while(ch != '}' && hex_counter < 7);

        if(ch == '}')
        {
            return is_code_point_valid(file, codepoint);
        }

        else
        {
            return set_utf8_file_read_error(file,
         "A maximum of 6 hex digits are allowed in unicode escape sequences.");
        }
    }

    else //First read from escape sequence not '{'
    {
        if(ch < 0)
        {
            return set_read_error_if_eof(file, ch,
                                 "Unexpected EOF in unicode escape sequence.");
        }
        
        else
        {
            return set_utf8_file_read_error(file,
                                 "Expected \'{\' in unicode escape sequence.");
        }
    }
}

//private functions

static int32_t read_utf8(utf8_file_t *file)
{
    if(file->status < 0)
    {
        if(file->status == UTF8_FILE_CLOSED)
        {
            file->status = UTF8_FILE_ERROR;
        }

        return file->status;
    }

    int32_t ch = fgetc(file->fp); //Casting int to int32_t
    if(ch == EOF)
    {
        file->status = UTF8_FILE_EOF;
        file->error_msg = "Normal EOF reached.";
        return file->status;
    }

    //Return 7bit character
    if(ch < 128)
    {
        return ch;
    }

    if(ch < 192 || ch > 247) //128-191 = 10xxxxxx
    {
        return set_utf8_file_read_error(file,
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
            return set_utf8_file_read_error(file,
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
        ch = fgetc(file->fp); //Casting int to int32_t
        if(ch < 0)
        {
            return set_utf8_file_read_error(file,
                                           "Unexpected EOF in UTF8 sequence.");
        }

        if(ch < 128 || ch > 191) //128-191 = 10xxxxxx
        {
            return set_utf8_file_read_error(file,
                                            "Invalid byte in UTF8 sequence.");
        }

        codepoint = codepoint << 6;
        codepoint = codepoint + (ch & 63); //63 = 111111
        //After reading the first and second byte the codepoint should be more
        //than 0 in all cases unless it is padded with zeroes
        if(codepoint == 0)
        {
            return set_utf8_file_read_error(file,
                                 "Unicode codepoint is padded (second byte).");
        }
    }

    return is_code_point_valid(file, codepoint);
}

static int32_t convert_newlines(utf8_file_t *file);

static int32_t read_and_update_position(utf8_file_t *file)
{
    //This should probably check the status to increment the line_position
    //exactly once when a read error is encountered
    int32_t ch = convert_newlines(file);
    if(ch >= 0)
    {
        if(ch == '\n')
        {
            file->line++;
            file->line_position = 0;
        }

        else
        {
            file->line_position++;
        }
    }

    return ch;
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

static int32_t set_read_error_if_eof(utf8_file_t *file, int32_t ch, char *msg)
{
    if(ch == UTF8_FILE_EOF)
    {
        return set_utf8_file_read_error(file, msg);
    }

    else
    {
        //Pass on any read_utf8 errors
        return ch;
    }
}

static int32_t is_code_point_valid(utf8_file_t *file, int32_t codepoint)
{
    //There's other checks that might be generate an error. Things like
    //codepoints in the private use areas or noncharacters could be allowed
    //in the data section but nowhere else.

    //There's a gap in the basic multilingual plane to allow utf16 encodings
    //to encode the characters added later with two 16bit characters
    if(codepoint >= 0xd800 && codepoint <= 0xdfff)
    {
        return set_utf8_file_read_error(file,
                                      "Unicode codepoint is UTF16 surrogate.");
    }

    //utf8 can store a wider range of values than utf16 but there isn't any
    //need for additional codepoints anytime soon so 0x10ffff is the maximum
    //value used in Unicode 
    if(codepoint > 0x10ffff)
    {
        return set_utf8_file_read_error(file,
                                        "Unicode codepoint out of range.");
    }

    return codepoint;
}

static int32_t convert_newlines(utf8_file_t *file)
{
    int32_t ch;
    if(file->return_temp_next)
    {
        ch = file->temp;
        file->return_temp_next = false;
    }

    else
    {
        ch = read_utf8(file);
    }

    if(ch == '\r')
    {
        int32_t next_ch = read_utf8(file);
        if(next_ch != '\n')
        {
            file->temp = next_ch;
            file->return_temp_next = true;
        }

        return '\n';
    }

    else
    {
        return ch;
    }
}

