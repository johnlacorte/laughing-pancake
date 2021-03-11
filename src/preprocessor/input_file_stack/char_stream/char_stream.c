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

int open_char_stream(char_stream_t *stream, char *filename)
{
    stream->fp = fopen(filename, "r");
    if(stream->fp != NULL)
    {
        stream->status = CHAR_STREAM_OK;
        stream->error_msg = "char_stream okay.";
        return stream->status;
    }

    else
    {
        stream->status = CHAR_STREAM_FAILED_TO_OPEN;
        stream->error_msg = "Error opening file.";
        return stream->status;
    }
}

int is_char_stream_open(char_stream_t *stream)
{
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

void push_7bit_char(char_stream_t *stream, int ch)
{
    if(stream->status == CHAR_STREAM_EOF)
    {
        stream->status = CHAR_STREAM_OK;
    }

    if(stream->status >= CHAR_STREAM_OK)
    {
        if(ch < 0 || ch > 127)
        {
            stream->status = CHAR_STREAM_PREPROC_DID_SOMETHING_WRONG;
            stream->error_msg = "Character pushed to char_stream was out of range.";
        }

        else
        {
            ungetc(ch, stream->fp);
        }
    }
}

int32_t pop_utf8(char_stream_t *stream)
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

int pop_7bit_char(char_stream_t *stream)
{
    if(stream->status < 0)
    {
        return stream->status;
    }

    else
    {
        int ch = fgetc(stream->fp);
        if(ch > 127)
        {
            stream->status = CHAR_STREAM_READ_FAILED;
            stream->error_msg = "Read byte > 127 reading 7bit character.";
            return stream->status;
        }

        else
        {
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
    }
}

char *char_stream_error_msg(char_stream_t *stream)
{
    return stream->error_msg;
}

void char_stream_reset(char_stream_t *stream)
{
    stream->status = CHAR_STREAM_OK;
    stream->error_msg = "char_stream okay.";
}

/* --- end of file "char_stream.c" --- */

