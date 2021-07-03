#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../preprocessor/preprocessor.h"
#include "keyword_matcher/keyword_matcher.h"
#include "token_types.h"
#include "token_stream.h"

#define MAX_TOKEN_LENGTH 128

typedef struct
{
    preproc_t preprocessor;
    char token_buffer[MAX_TOKEN_LENGTH];
} token_stream_state_t;

token_stream_t open_token_stream(char *filename)
{
    token_stream_state_t *new_stream = malloc(sizeof(token_stream_state_t));
    if(new_stream != NULL)
    {
        new_stream->preprocessor = open_preproc(filename);
        if(new_stream->preprocessor != NULL)
        {
            return (token_stream_t)new_stream;
        }

        else
        {
            free(new_stream);
            return NULL;
        }
    }

    else
    {
        return NULL;
    }
}

void free_token_stream(token_stream_t token_stream)
{
    if(token_stream != NULL)
    {
        token_stream_state_t *state = (token_stream_state_t*)token_stream;
        free_preproc(state->preprocessor);
        free(token_stream);
    }
}

static bool new_token(token_stream_state_t *state, token_t *token);

void read_token_from_stream(token_stream_t token_stream, token_t *token)
{
    if(token_stream != NULL)
    {
        token_stream_state_t *state = (token_stream_state_t*)token_stream;
        if(new_token(state, token))
        {
            char *token_buffer = state->token_buffer;
            if(token_buffer[0] == '$')
            {
                token->token_type = TOKEN_IDENTIFIER;
                token->token_string = token_buffer;
            }

            else
            {
                if(isdigit(token_buffer[0]))
                {
                    token->token_type = TOKEN_NUMBER;
                    token->token_string = token_buffer;
                }

                else
                {
                    token->token_type = keyword_matcher(token_buffer);
                    token->token_string = NULL;
               }
            }
        }
    }

    else
    {
        token->token_type = TOKEN_ERROR;
        token->token_string = "read_token_from_stream() passed NULL pointer.";
    }
}

static void set_token_type_for_negative_read(token_stream_state_t *state, token_t *token, int ch);

void read_extern_name_from_stream(token_stream_t token_stream, token_t *token)
{
    if(token_stream != NULL)
    {
        token_stream_state_t *state = (token_stream_state_t*)token_stream;
        int ch = read_preproc_extern_name(state->preprocessor);
        token->token_line = get_preproc_line_number(state->preprocessor);
        token->token_position = get_preproc_line_position(state->preprocessor);
        //reads can be PREPROC_END_OF_STRING after the first read but not
        //PREPROC_EOF or PREPROC_ERROR
        if(ch >= 0)
        {
            char *name_buffer = state->token_buffer;
            name_buffer[0] = ch;
            for(size_t name_index = 1;
                name_index < MAX_TOKEN_LENGTH;
                name_index++)
            {
                ch = read_preproc_extern_name(state->preprocessor);
                if(ch < 0)
                {
                    if(ch == PREPROC_END_OF_STRING)
                    {
                        name_buffer[name_index] = '\0';
                        token->token_length = name_index;
                        token->token_type = TOKEN_EXTERNAL_NAME;
                        token->token_string = name_buffer;
                        return;
                    }

                    else
                    {
                        set_token_type_for_negative_read(state, token, ch);
                        return;
                    }

                }

                else
                {
                    name_buffer[name_index] = ch;
                }
            }

            name_buffer[MAX_TOKEN_LENGTH - 1] = '\0';
            token->token_type = TOKEN_ERROR;
            token->token_string = "External name exceeded maximum length.";
            return;
        }

        else
        {
            //if first read is PREPROC_END_OF_STRING it should be an error
            if(ch == PREPROC_END_OF_STRING)
            {
                token->token_type = TOKEN_ERROR;
                token->token_string = "External name cannot be empty.";
            }

            else
            {
                set_token_type_for_negative_read(state, token, ch);
            }
        }
    }

    else
    {
        token->token_type = TOKEN_ERROR;
        token->token_string =
                        "read_extern_name_from_stream() passed NULL pointer.";
    }
}

void read_data_from_stream(token_stream_t token_stream, token_t *token)
{
    //This needs a string that grows
    //any read can be PREPROC_END_OF_STRING (make sure to set the length even 0)
    if(token_stream != NULL)
    {
        token_stream_state_t *state = (token_stream_state_t*)token_stream;
        int ch = read_preproc_data(state->preprocessor);
        token->token_line = get_preproc_line_number(state->preprocessor);
        token->token_position = get_preproc_line_position(state->preprocessor);
        if(ch >= 0)
        {
            size_t data_buffer_length = MAX_TOKEN_LENGTH;
            char *data_buffer = malloc(sizeof(char) * data_buffer_length);
            if(data_buffer != NULL)
            {
                data_buffer[0] = ch;
            }

            for(size_t data_index = 1; ; data_index++)
            {
                if(data_index >= data_buffer_length)
                {
                    data_buffer_length *= 2;
                    free(data_buffer);
                    data_buffer = malloc(sizeof(char) * data_buffer_length);
                }

                if(data_buffer == NULL)
                {
                    token->token_type = TOKEN_ERROR;
                    token->token_string =
        "Unable to allocate memory for data string in read_data_from_stream()";
                    return;
                }

                ch = read_preproc_data(state->preprocessor);
                if(ch < 0)
                {
                    if(ch == PREPROC_END_OF_STRING)
                    {
                        data_buffer[data_index] = '\0';
                        token->token_length = data_index;
                        token->token_type = TOKEN_EXTERNAL_NAME;
                        token->token_string = data_buffer;
                        return;
                    }

                    else
                    {
                        set_token_type_for_negative_read(state, token, ch);
                        return;
                    }

                }

                else
                {
                    data_buffer[data_index] = ch;
                }                
            }
        }

        else
        {
            if(ch == PREPROC_END_OF_STRING)
            {
                token->token_type = TOKEN_EMPTY_DATA_STRING;
            }

            else
            {
                set_token_type_for_negative_read(state, token, ch);
            }
        }
    }

    else
    {
        token->token_type = TOKEN_ERROR;
        token->token_string = "read_data_from_stream() passed NULL pointer.";
    }
}

char *get_token_stream_line_string(token_stream_t token_stream, int line_number)
{
    if(token_stream != NULL)
    {
        token_stream_state_t *state = (token_stream_state_t*)token_stream;
        return get_preproc_line_string(state->preprocessor, int line_number);
    }

    else
    {
        return NULL;
    }
}

// *** private functions ***

static bool new_token(token_stream_state_t *state, token_t *token)
{
    int ch = read_preproc_char(state->preprocessor);
    token->token_line = get_preproc_line_number(state->preprocessor);
    token->token_position = get_preproc_line_position(state->preprocessor);
    if(ch >= 0)
    {
        //do I need to check if ch is a space? it shouldn't be
        char *token_buffer = state->token_buffer;
        token_buffer[0] = ch;
        for(size_t token_index = 1;
            token_index < MAX_TOKEN_LENGTH;
            token_index++)
        {
            ch = read_preproc_char(state->preprocessor);
            if(ch < 0)
            {
                set_token_type_for_negative_read(state, token, ch);
                return false;
            }

            else
            {
                if(ch == ' ')
                {
                    token_buffer[token_index] = '\0';
                    token->token_length = token_index;
                    token->token_string = token_buffer;
                    return true;
                }

                else
                {
                    token_buffer[token_index] = ch;
                }
            }
        }

        token_buffer[MAX_TOKEN_LENGTH - 1] = '\0';
        token->token_type = TOKEN_ERROR;
        token->token_string = "Token exceeded maximum length.";
        return false;
    }

    else
    {
        //first read can be PREPROC_EOF
        if(ch == PREPROC_EOF)
        {
            token->token_type = TOKEN_EOF;
            return false;
        }

        else
        {
            set_token_type_for_negative_read(state, token, ch);
            return false;
        }
    }
}

//This needs to get the preprocessor used by state somehow
static void set_token_type_for_negative_read(token_stream_state_t *state, token_t *token, int ch)
{
    if(ch == PREPROC_EOF)
    {
        //Unexpected EOF error
        token->token_type = TOKEN_ERROR;
        token->token_string = "Unexpected EOF in token.";
    }

    else
    {
        token->token_type = TOKEN_ERROR;
        token->token_string = get_preproc_error_msg(state->preprocessor);
    }
}

// *** end of file "token_stream.c" ***
