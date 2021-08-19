#include <stdint.h>
#include "../token_stream/token_stream.h"
#include "../wasm_module/wasm_module.h"
#include "parser.h"

typedef struct
{
    token_stream_t token_stream;
    wasm_module_t wasm_module;
}parser_state_t;

void read_token(parser_state_t *parser_state, token_t *token);

int open_file_for_parsing(char *input_file, char *output_file, output_type_t output_type)
{
    parser_state_t parser_state;
    parser_state.token_stream = open_token_stream(input_file);
    parser_state.wasm_module = new_wasm_module(output_file);
    //token_stream_t token_stream = open_token_stream(input_file);
    token_t token;
    //Does read_token_from_stream() handle NULL and return an error?
    read_token(&parser_state, &token);
    //read_token_from_stream(token_stream, &token);
    switch(token.token_type)
    {
        case TOKEN_ERROR:
        case TOKEN_EOF:
        case TOKEN_NO_MATCH:
        case TOKEN_OPEN_PAREN:
            read_token(&parser_state, &token);
            //read_token_from_stream(token_stream, &token);
            switch(token.token_type)
            {
                case TOKEN_ERROR:
                case TOKEN_EOF:
                case TOKEN_NO_MATCH:
                case TOKEN_KEYWORD_MODULE:
                    //Probably safe to open a module
                    read_token(&parser_state, &token);
                    //read_token_from_stream(token_stream, &token);
                    switch(token.token_type)
                    {
                        case TOKEN_ERROR:
                        case TOKEN_EOF:
                        case TOKEN_NO_MATCH:
                        case TOKEN_CLOSE_PAREN:
                            read_token(&parser_state, &token);
                            //read_token_from_stream(token_stream, &token);
                            switch(token.token_type)
                            {
                                case TOKEN_ERROR:
                                case TOKEN_EOF:
                                    //write empty module
                                    return 0;
                                case TOKEN_NO_MATCH:
                                default:
                            }
                        default:
                    }
                default:
            }
        default:
    }

    return 1;
}

void read_token(parser_state_t *parser_state, token_t *token)
{
    read_token_from_stream(parser_state->token_stream, token);
    //perhaps print error message here
}

/*** end of file "parser.c" ***/