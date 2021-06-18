#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../preprocessor/preprocessor.h"
#include "keyword_matcher.h"
#include "token_types.h"
#include "token_stream.h"

#define MAX_TOKEN_LENGTH 128

typedef struct
{
    preproc_t preprocessor;
    token_t *token;
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

static void set_token_type(token_stream_state_t *state, int type);

static void set_token_string(token_stream_state_t *state, char *string);

void read_token_from_stream(token_stream_t token_stream, token_t *token)
{
    if(token_stream != NULL)
    {
        token_stream_state_t *state = (token_stream_state_t*)token_stream;
        if(new_token(state, token))
        {
            char *token_buffer = stream->token_buffer;
            if(token_buffer[0] == '$')
            {
                set_token_type(state, TOKEN_IDENTIFIER);
            }

            else
            {
                if(isdigit(token_buffer[0]))
                {
                    set_token_type(state, TOKEN_NUMBER);
                }

                else
                {
                    set_token_type(state, keyword_matcher(token_buffer));
                }
            }
        }
    }

    else
    {
        token->type = TOKEN_ERROR;
        token->token_string = "read_token_from_stream() passed NULL pointer.";
    }
}

static void set_token_line(token_stream_state_t *state, int line);

static void set_token_position(token_stream_state_t *state, int position);

void read_extern_name_from_stream(token_stream_t token_stream, token_t *token)
{
    if(token_stream != NULL)
    {
        token_stream_state_t *state = (token_stream_state_t*)token_stream;
        state->token = token;
        int ch = read_preproc_extern_name(state->preprocessor);
        token->line = get_preproc_line_number(state->preprocessor);
        token->position = get_preproc_line_position(state->preprocessor);
        //reads can be PREPROC_END_OF_STRING after the first read but not
        //PREPROC_EOF or PREPROC_ERROR
        if(ch >= 0)
        {
            int name_index;
            char *name_buffer = stream->token_buffer;
            token_buffer[0] = ch;
            for(name_index = 1; name_index < MAX_TOKEN_LENGTH; name_index++)
            {
                ch = read_preproc_extern_name(state->preprocessor);
                if(ch < 0)
                {
                    if(ch == PREPROC_END_OF_STRING)
                    {
                        name_buffer[name_index] = '\0';
                        token->token_length = name_index;
                        set_token_type(state, TOKEN_EXTERNAL_NAME);
                        set_token_string(state, name_buffer);
                        return;
                    }

                    else
                    {
                        set_token_type_for_negative_read(state, ch);
                        return;
                    }

                }

                else
                {
                    name_buffer[name_index] = ch;
                }
            }

            name_buffer[MAX_TOKEN_LENGTH - 1] = '/0';
            set_token_type(state, TOKEN_ERROR);
            set_token_string(state, "External name exceeded maximum length.");
            return;
        }

        else
        {
            //if first read is PREPROC_END_OF_STRING it should be an error
            if(ch == PREPROC_END_OF_STRING)
            {
                set_token_type(state, TOKEN_ERROR);
                set_token_string(state, "External name cannot be empty.");
            }

            else
            {
                set_token_type_for_negative_read(state, ch);
            }
        }
    }

    else
    {
        token->type = TOKEN_ERROR;
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
        state->token = token;
        int ch = read_preproc_extern_name(state->preprocessor);
        token->line = get_preproc_line_number(state->preprocessor);
        token->position = get_preproc_line_position(state->preprocessor);
        //Continue filling this in
    }

    else
    {
        token->type = TOKEN_ERROR;
        token->token_string = "read_data_from_stream() passed NULL pointer.";
    }
}

static void set_token_type_for_negative_read(token_stream_state_t *state, int ch);

static bool new_token(token_stream_state_t *state, token_t *token)
{
    state->token = token;
    int ch = read_preproc_char(state->preprocessor);
    token->line = get_preproc_line_number(state->preprocessor);
    token->position = get_preproc_line_position(state->preprocessor);
    if(ch >= 0)
    {
        //do I need to check if ch is a space? it shouldn't be
        int token_index;
        char *token_buffer = stream->token_buffer;
        token_buffer[0] = ch;
        for(token_index = 1; token_index < MAX_TOKEN_LENGTH; token_index++)
        {
            ch = read_preproc_char(state->preprocessor);
            if(ch < 0)
            {
                set_token_type_for_negative_read(state, ch);
                return false;
            }

            else
            {
                if(ch == ' ')
                {
                    token_buffer[token_index] = '\0';
                    token->token_length = token_index;
                    set_token_string(state, token_buffer);
                    return true;
                }

                else
                {
                    token_buffer[token_index] = ch;
                }
            }
        }

        token_buffer[MAX_TOKEN_LENGTH - 1] = '/0';
        set_token_type(state, TOKEN_ERROR);
        set_token_string(state, "Token exceeded maximum length.");
        return false;
    }

    else
    {
        //first read can be PREPROC_EOF
        if(ch == PREPROC_EOF)
        {
            set_token_type(state, TOKEN_EOF);
            return false;
        }

        else
        {
            set_token_type_for_negative_read(state, ch);
            return false;
        }
    }
}

static void set_token_type(token_stream_state_t *state, int type)
{
    token_t token = state->token;
    token->type = type;
}

static void set_token_string(token_stream_state_t *state, char *string)
{
    token_t token = state->token;
    token->string = string;
}

static void set_token_line(token_stream_state_t *state, int line)
{
    token_t token = state->token;
    token->line = line;
}

static void set_token_position(token_stream_state_t *state, int position)
{
    token_t token = state->token;
    token->position = position;
}

static void set_token_type_for_negative_read(token_stream_state_t *state, int ch)
{
    if(ch == PREPROC_EOF)
    {
        //Unexpected EOF error
        set_token_type(state, TOKEN_ERROR);
        set_token_string(state, "Unexpected EOF in token.");
    }

    else
    {
        set_token_type(state, TOKEN_ERROR);
        set_token_string(state, get_preproc_error_msg(state->preprocessor));
    }
}
