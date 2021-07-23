#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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
            //This should never happen nor is there a way to trigger this.
            encoder->error_msg =
                        "remaining_bytes in utf8_encoder is a negative value.";
            return false;
        }

        else
        {
            return (encoder->remaining_bytes == 0);
        }
    }
}

void encode_codepoint_to_utf8(utf8_encoder_t *encoder, int32_t ch)
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
                //int ch_as_int = (int)ch;
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

        else
        {
            encoder->status = ENCODER_ERROR;
            encoder->error_msg =
                               "Encoder was given a codepoint when not empty.";
        }
    }
}

int read_next_byte_from_encoder(utf8_encoder_t *encoder)
{
    if(encoder->status == ENCODER_ERROR)
    {
        return ENCODER_ERROR;
    }

    else
    {
        if(encoder->remaining_bytes > 0)
        {
            int byte = encoder->bytes[4 - encoder->remaining_bytes];
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
}

char *get_utf8_encoder_error_msg(utf8_encoder_t *encoder)
{
    return encoder->error_msg;
}

static int copy_utf8_to_char_array(utf8_encoder_t *encoder,
                               int32_t ch,
                               char *dest);

char *convert_string_to_utf8(utf8_encoder_t *encoder,
                             size_t length,
                             int32_t *string)
{
    //If I don't replace this with print_line() the branches that return NULL
    //need to set an error message.
    if(string != NULL)
    {
        if(length > 0)
        {
            char *return_string = malloc(sizeof(char) * length);
            if(return_string != NULL)
            {
                int char_index = 0;
                for(size_t int32_t_index = 0;
                    int32_t_index < length;
                    int32_t_index++)
                {
                    int bytes_copied =
                        copy_utf8_to_char_array(encoder,
                                                string[int32_t_index],
                                                &return_string[char_index]);
                    if(bytes_copied <= 0)
                    {
                        free(return_string);
                        return NULL;
                    }

                    else
                    {
                        char_index += bytes_copied;
                    }
                }

                if(return_string[char_index - 1] == '\0')
                {
                    return return_string;
                }

                else
                {
                    free(return_string);
                    return NULL;
                }
            }

            else
            {
                return NULL;
            }
        }

        else
        {
            return NULL;
        }
    }

    else
    {
        return NULL;
    }

    return NULL;
}

// *** private functions ***

//Tries to encode a codepoint to utf8 and write the resulting bytes to an array
//of chars returning the number of chars written.
static int copy_utf8_to_char_array(utf8_encoder_t *encoder,
                               int32_t ch,
                               char *dest)
{
    if(is_utf8_encoder_empty(encoder))
    {
        encode_codepoint_to_utf8(encoder, ch);
        int counter = 0;
        while(!is_utf8_encoder_empty(encoder))
        {
            int byte = read_next_byte_from_encoder(encoder);
            if(byte == ENCODER_ERROR)
            {
                return -1;
            }

            *dest = byte;
            dest++;
            counter++;
        }

        return counter;
    }

    return -1;
}

// *** end of file "utf8_encoder.c" ***
