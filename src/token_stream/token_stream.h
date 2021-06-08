#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H

typedef struct
{
    int type;
    int line;
    int position;
    int token_length;
    char *token_string;
} token_t;

typedef void* token_stream_t;

token_stream_t open_token_stream(char *filename);

void free_token_stream(token_stream_t stream);

void read_token_from_stream(token_stream_t stream, token_t *token);

void read_extern_name_from_stream(token_stream_t stream, token_t *token);

void read_data_from_stream(token_stream_t stream, token_t *token);

#endif
