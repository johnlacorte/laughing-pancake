#include <stdio.h>

#include "char_stream.h"

int open_char_stream(char_stream_t *stream, char *filename)
{
    stream->fp = fopen(filename, "r");
    if(stream->fp != NULL)
    {
        stream->status = 0;
        stream->error_msg = "char_stream okay.";
        stream->expected_bytes = 0;
        return 1;
    }

    else
    {
        stream->status = CHAR_STREAM_FAILED_TO_OPEN;
        stream->error_msg = "Error opening file.";
        return 0;
    }
}

void close_char_stream(char_stream_t *stream)
{
    fclose(stream->fp);
    stream->status = CHAR_STREAM_FILE_CLOSED;
    stream->error_msg = "File is closed.";
    stream->expected_bytes = 0;
}

void push_7bit_char(char_stream_t *stream, int ch)
{
    if(stream->status >= 0)
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

int pop_utf8_byte(char_stream_t *stream)
{
    //does not check for padding, does not check if codepoints are for
    //surrogates or if they are outside of the range of the codepoints
    //or if it's a unicode control character or if the codepoint falls
    //into a range reserved for private or future use.
    if(stream->status < 0)
    {
        return stream->status;
    }

    else
    {
        int ch = fgetc(stream->fp);
        if(stream->expected_bytes > 0)
        {
            if(ch == EOF)
            {
                stream->status = CHAR_STREAM_READ_FAILED;
                stream->error_msg = "EOF before the end of a UTF8 character.";
                return stream->status;
            }

            if(ch < 128 || ch > 191)
            {
                stream->status = CHAR_STREAM_READ_FAILED;
                stream->error_msg = "Expected additional byte in UTF8 character was out of range.";
                return stream->status;
            }

            else
            {
                stream->expected_bytes--;
                return ch;
            }
        }

        else //Reading the first character of a sequence.
        {
            if(ch == EOF)
            {
                stream->status = CHAR_STREAM_EOF;
                stream->error_msg = "Normal EOF reached.";
                return stream->status;
            }

            else
            {
                //determine number of expected bytes or return an error.
                if(ch < 128)
                {
                    stream->expected_bytes = 0;
                    return ch;
                }

                else
                {
                    if(ch < 192)
                    {
                        stream->status = CHAR_STREAM_READ_FAILED;
                        stream->error_msg = "Unexpected additional byte at thestart of UTF8 character.";
                        return stream->status;
                    }

                    else
                    {
                        if(ch < 224)
                        {
                            stream->expected_bytes = 1;
                            return ch;
                        }

                        else
                        {
                            if(ch < 240)
                            {
                                stream->expected_bytes = 2;
                                return ch;
                            }

                            else
                            {
                                if(ch < 248)
                                {
                                    stream->expected_bytes = 3;
                                    return ch;
                                }

                                else
                                {
                                    stream->status = CHAR_STREAM_READ_FAILED;
                                    stream->error_msg = "Byte read was > 247.";
                                    return stream->status;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

int pop_7bit_char(char_stream_t *stream)
{
    if(stream->status < 0)
    {
        return stream->status;
    }

    else
    {
        if(stream->expected_bytes > 0)
        {
            stream->status = CHAR_STREAM_PREPROC_DID_SOMETHING_WRONG;
            stream->error_msg = "Unexpected call to pop_7bit_char() by preprocessor.";
            return stream->status;
        }

        else
        {
            int ch = fgetc(stream->fp);
            if(ch > 127)
            {
                stream->status = CHAR_STREAM_READ_FAILED;
                stream->error_msg = "Read byte > 127 while reading 7bit character.";
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
}

char *char_stream_error_msg(char_stream_t *stream)
{
    return stream->error_msg;
}

void char_stream_reset(char_stream_t *stream)
{
    stream->status = 0;
    stream->error_msg = "char_stream okay.";
    stream->expected_bytes = 0;
}

/* --- end of file "char_stream.c" --- */

