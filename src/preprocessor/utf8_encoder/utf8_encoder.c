#include <stdbool.h>
#include <stdint.h>
#include "utf8_encoder.h"

void init_utf8_encoder(utf8_encoder_t *encoder)
{
    encoder->status = ENCODER_OK;
    encoder->remaining_bytes = 0;
    encoder->bytes[0] = 0;
    encoder->bytes[1] = 0;
    encoder->bytes[2] = 0;
    encoder->bytes[3] = 0;
    encoder->error_msg = "Encoder okay.";
}

bool is_utf8_encoder_empty(utf8_encoder_t *encoder)
{
    //If there's an error this should return false so the error status can
    //be returned to the preprocessor.
    if(encoder->status == ENCODER_ERROR)
    {
        return false;
    }

    else
    {
        if(encoder->remaining_bytes < 0)
        {
            encoder->status = ENCODER_ERROR;
            return false;
        }

        else
        {
            return (encoder->remaining_bytes <= 0);
        }
    }
}

void convert_codepoint_to_utf8(utf8_encoder_t *encoder, int32_t ch)
{
    //do I need to cast anything to an int?
    if(encoder->status == ENCODER_OK)
    {
        if(is_utf8_encoder_empty(encoder))
        {
            if(ch < 0)
            {
                encoder->status = ENCODER_ERROR;
                encoder->error_msg =
                              "Encoder was given a negative number to encode.";
            }

            else
            {
                if(ch < 128)
                {
                    encoder->bytes[3] = ch;
                    encoder->remaining_bytes = 1;
                }

                else
                {
                    //128-191 = 10xxxxxx, 63 = 111111
                    encoder->bytes[3] = 128 + (ch & 63);
                    ch = ch >> 6;
                    if(ch < 32) //31 = 11111
                    {
                        encoder->remaining_bytes = 2;
                        encoder->bytes[2] = 192 + ch;
                    }

                    else
                    {
                        encoder->bytes[2] = 128 + (ch & 63);
                        ch = ch >> 6;
                        if(ch < 16) //15 = 1111
                        {
                            encoder->remaining_bytes = 3;
                            encoder->bytes[1] = 224 + ch; //224-239 = 1110xxxx
                        }

                        else
                        {
                            encoder->bytes[1] = 128 + (ch & 63);
                            ch = ch >> 6;
                            if(ch < 8) //7 = 111
                            {
                                encoder->remaining_bytes = 4;
                                encoder->bytes[0] = 240 + ch; //240-247 = 11110xxx
                            }

                            else
                            {
                                encoder->status = ENCODER_ERROR;
                                encoder->error_msg =
                                       "Encoder given codepoint out of range.";
                            }
                        }
                    }
                }
            }
        }
    }
}

int read_next_byte_from_encoder(utf8_encoder_t *encoder)
{
    if(encoder->remaining_bytes > 0)
    {
        int byte = 4 - encoder->bytes[encoder->remaining_bytes];
        encoder->remaining_bytes--;
        return byte;
    }

    else
    {
        encoder->status = ENCODER_ERROR;
        encoder->error_msg = "Preprocessor tried reading from empty utf8 encoder.";
        return ENCODER_ERROR; 
    }
}

char *get_encoder_error_msg(utf8_encoder_t *encoder)
{
    return encoder->error_msg;
}

// *** end of file "utf8_encoder.c" ***
