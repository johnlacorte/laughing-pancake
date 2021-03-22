#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "char_stream_status.h"
#include "char_stream.h"

void init_char_stream(char_stream_t *stream)
{
    stream->fp = NULL;
    stream->status = CHAR_STREAM_FAILED_TO_OPEN;
    stream->error_msg = "Reading from file where open_char_stream() was never called.";
}

#define BYTE_ORDER_MARKER 0xFEFF

#define READ_FROM_STREAM_NEXT 0

#define READ_FROM_NEXT_READ_NEXT 1

static int32_t read_utf8(char_stream_t *stream);

static void set_next_read(char_stream_t *stream, int32_t next_ch);

int open_char_stream(char_stream_t *stream, char *filename)
{
    stream->fp = fopen(filename, "r");
    if(stream->fp != NULL)
    {
        stream->status = CHAR_STREAM_OK;
        stream->error_msg = "char_stream okay.";
        stream->last_read = ' ';
        int32_t initial_read = read_utf8(stream);
        if(initial_read == BYTE_ORDER_MARKER)
        {
            stream->what_to_return_next = READ_FROM_STREAM_NEXT;
        }

        else
        {
            set_next_read(stream, initial_read);
        }

        return stream->status;
    }

    else
    {
        stream->status = CHAR_STREAM_FAILED_TO_OPEN;
        stream->error_msg = "Error opening file.";
        return stream->status;
    }
}

bool is_char_stream_open(char_stream_t *stream)
{
    //Change to return true/false
    if((stream->status == CHAR_STREAM_FAILED_TO_OPEN) ||
       (stream->status == CHAR_STREAM_FILE_CLOSED))
    {
        return 0;
    }

    else
    {
        return 1;
    }
}

void close_char_stream(char_stream_t *stream)
{
    fclose(stream->fp);
    stream->status = CHAR_STREAM_FILE_CLOSED;
    stream->error_msg = "File is closed.";
}

static int32_t convert_escape_sequences(char_stream_t *stream);

int32_t read_utf8_from_char_stream(char_stream_t *stream)
{
    int32_t ch = convert_escape_sequences(stream);
    if(ch != BYTE_ORDER_MARKER)
    {
        stream->last_read = ch;
        return ch;
    }

    else
    {
        stream->status = CHAR_STREAM_READ_FAILED;
        stream->error_msg = "Byte Order Marker read in file.";
        return stream->status;
    }
}

char *char_stream_error_msg(char_stream_t *stream)
{
    return stream->error_msg;
}

int pop_byte_from_char_stream(char_stream_t *stream)
{
    if(stream->status < 0)
    {
        return stream->status;
    }

    int ch = fgetc(stream->fp);
    if(ch == EOF)
    {
        stream->status = CHAR_STREAM_EOF;
        stream->error_msg = "Normal EOF reached.";
        return stream->status;
    }

    else
    {
        return ch;
    }
}

void char_stream_reset(char_stream_t *stream)
{
    stream->status = CHAR_STREAM_OK;
    stream->error_msg = "char_stream okay.";
}

/* ----Local Functions---- */

static int32_t is_code_point_valid(char_stream_t *stream, int32_t codepoint);

static int32_t read_utf8(char_stream_t *stream)
{
    if(stream->status < 0)
    {
        return stream->status;
    }

    int32_t ch = fgetc(stream->fp); //Casting int to int32_t
    if(ch == EOF)
    {
        stream->status = CHAR_STREAM_EOF;
        stream->error_msg = "Normal EOF reached.";
        return stream->status;
    }

    //Return 7bit character
    if(ch < 128)
    {
        return ch;
    }

    if(ch < 192 || ch > 247)
    {
        stream->status = CHAR_STREAM_READ_FAILED;
        stream->error_msg = "UTF8 sequence starts with invalid byte.";
        return stream->status;
    }

    //Read initial byte of multibyte sequence
    int32_t codepoint = 0;
    int number_of_additional_bytes = 0;
    if(ch < 224)
    {
        codepoint = ch & 31; //lowest 5 bits
        number_of_additional_bytes = 1;

        //The second byte of a two byte utf8 codepoint only holds six bits
        //so the bits taken from the first byte must be greater than 0.
        if(codepoint == 0)
        {
            stream->status = CHAR_STREAM_READ_FAILED;
            stream->error_msg = "Unicode codepoint is padded (first byte of two).";
            return stream->status;
        }
    }
    else
    {
        if(ch < 240)
        {
            codepoint = ch & 15; //lowest 4 bits
            number_of_additional_bytes = 2;
        }

        else
        {
            codepoint = ch & 7; //lowest 3 bits
            number_of_additional_bytes = 3;
        }
    }

    //Get additional bytes of multibyte sequence
    for(int i = 0; i < number_of_additional_bytes; i++)
    {
        ch = fgetc(stream->fp); //Casting int to int32_t
        if(ch < 0)
        {
            stream->status = CHAR_STREAM_READ_FAILED;
            stream->error_msg = "Unexpected EOF in UTF8 sequence.";
            return stream->status;
        }

        if(ch < 128 || ch > 191)
        {
            stream->status = CHAR_STREAM_READ_FAILED;
            stream->error_msg = "Invalid byte in UTF8 sequence.";
            return stream->status;
        }

        codepoint = codepoint << 6;
        codepoint = codepoint + (ch & 63);
        //After reading the first and second byte the codepoint should be more
        //than 0 in all cases unless it is padded with zeroes
        if(codepoint == 0)
        {
            stream->status = CHAR_STREAM_READ_FAILED;
            stream->error_msg = "Unicode codepoint is padded (second byte).";
            return stream->status;
        }
    }

    return is_code_point_valid(stream, codepoint);
}

static void set_next_read(char_stream_t *stream, int32_t next_ch)
{
    if(stream->what_to_return_next == READ_FROM_STREAM_NEXT)
    {
        stream->next_read = next_ch;
        stream->what_to_return_next = READ_FROM_NEXT_READ_NEXT;
    }

    else
    {
        stream->status = CHAR_STREAM_PREPROC_DID_SOMETHING_WRONG;
        stream->error_msg = "char_stream tried to set read_next twice.";
    }
}

static int32_t convert_newlines(char_stream_t *stream);

static int32_t read_unicode_escape_sequence(char_stream_t *stream);

static int32_t convert_escape_sequences(char_stream_t *stream)
{
    int32_t ch = convert_newlines(stream);
    //Make sure escaped backslash is passed through and all escaped newlines
    //in a row are removed
    while((ch == '\\') && (stream->last_read != '\\'))
    {
        int32_t next_ch = convert_newlines(stream);
        if(next_ch == '\n')
        {
            ch = convert_newlines(stream);
        }

        else
        {
            if(next_ch == 'u')
            {
                return read_unicode_escape_sequence(stream);
            }

            else
            {
                set_next_read(stream, next_ch);
                return ch;
            }
        }
    }

    return ch;
}

static int32_t read_next(char_stream_t *stream);

static int32_t convert_newlines(char_stream_t *stream)
{
    int32_t ch = read_next(stream);
    if(ch == '\r')
    {
        int32_t next_ch = read_next(stream);
        if(next_ch != '\n')
        {
            set_next_read(stream, next_ch);
        }

        return '\n';
    }

    else
    {
        return ch;
    }
}

static int32_t hex_digit_to_int(int32_t ch);

static int32_t read_unicode_escape_sequence(char_stream_t *stream)
{
    //This should check if read_utf8() returns an error or EOF
    int32_t ch = read_utf8(stream);
    if(ch == '{')
    {
        int hex_counter = 0;
        int32_t codepoint = 0;
        ch = read_utf8(stream);
        do
        {
            int32_t hex = hex_digit_to_int(ch);
            if(hex >= 0)
            {
                hex_counter++;
                codepoint = (codepoint << 4) + hex;
            }

            else
            {
                stream->status = CHAR_STREAM_READ_FAILED;
                stream->error_msg = "Expected hex digit in unicode escape sequence.";
                return stream->status;
            }

            ch = read_utf8(stream);
        }while(ch != '}' && hex_counter < 7);

        if(ch == '}')
        {
            return is_code_point_valid(stream, codepoint);
        }

        else
        {
            stream->status = CHAR_STREAM_READ_FAILED;
            stream->error_msg = "A maximum of 6 hex digits are allowed in unicode escape sequences.";
            return stream->status;
        }
    }

    else
    {
        stream->status = CHAR_STREAM_READ_FAILED;
        stream->error_msg = "Expected \'{\' in unicode escape sequence.";
        return stream->status;
    }
}

static int32_t read_next(char_stream_t *stream)
{
    if(stream->what_to_return_next == READ_FROM_NEXT_READ_NEXT)
    {
        stream->what_to_return_next = READ_FROM_STREAM_NEXT;
        return stream->next_read;
    }

    else
    {
        return read_utf8(stream);
    }
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

static int32_t is_code_point_valid(char_stream_t *stream, int32_t codepoint)
{
    //There's other checks that might be generate an error. Things like
    //codepoints in the private use areas or noncharacters could be allowed
    //in the data section but nowhere else.

    //There's a gap in the basic multilingual plane to allow utf16 encodings
    //to encode the characters added later with two 16bit characters
    if(codepoint >= 0xd800 && codepoint <= 0xdfff)
    {
        stream->status = CHAR_STREAM_READ_FAILED;
        stream->error_msg = "Unicode codepoint is UTF16 surrogate.";
        return stream->status;
    }

    //utf8 can store a wider range of values than utf16 but there isn't any
    //need for additional codepoints anytime soon so 0x10ffff is the maximum
    //value used in Unicode 
    if(codepoint > 0x10ffff)
    {
        stream->status = CHAR_STREAM_READ_FAILED;
        stream->error_msg = "Unicode codepoint out of range.";
        return stream->status;
    }

    return codepoint;
}

/* --- end of file "char_stream.c" --- */

/* Notes
Unicode escape sequences can have escaped newlines in them but not unicode
escape sequences. That would be a mess to handle and you would have to type all
the characters you are escaping to do it.
*/
