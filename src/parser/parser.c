#include <stdint.h>
#include "../token_stream/token_stream.h"
#include "../wasm_module/wasm_module.h"
#include "parser.h"

typedef struct
{
    token_stream_t token_stream;
    wasm_module_t wasm_module;
    output_type_t output_type;
}parser_state_t;

void read_token(parser_state_t *parser_state, token_t *token);

void print_token_location(token_t *token);

void print_token_error(token_t *token);

void close_files_and_free_memory(parser_state_t *parser_state);

int open_file_for_parsing(char *input_file, char *output_file, output_type_t output_type)
{
    parser_state_t parser_state;
    parser_state.token_stream = open_token_stream(input_file);
    parser_state.wasm_module = new_wasm_module(output_file);
    parser_state.output_type = output_type;
    token_t token;
    read_token(&parser_state, &token);
    switch(token.token_type)
    {
        case TOKEN_ERROR:
            close_files_and_free_memory(&parser_state);
            return 1;
        case TOKEN_EOF:
            close_files_and_free_memory(&parser_state);
            return 1;
        case TOKEN_NO_MATCH:
            close_files_and_free_memory(&parser_state);
            return 1;
        case TOKEN_OPEN_PAREN:
            read_token(&parser_state, &token);
            switch(token.token_type)
            {
                case TOKEN_ERROR:
                    return 1;
                case TOKEN_EOF:
                case TOKEN_NO_MATCH:
                case TOKEN_KEYWORD_MODULE:
                    //Probably safe to open a module
                    read_token(&parser_state, &token);
                    //read_token_from_stream(token_stream, &token);
                    switch(token.token_type)
                    {
                        case TOKEN_ERROR:
                            return 1;
                        case TOKEN_EOF:
                        case TOKEN_NO_MATCH:
                        case TOKEN_OPEN_PAREN:
                            //I think I should pass either the line and
                            //position of the token or a pointer to the token
                            //to the read_entries function
                        case TOKEN_CLOSE_PAREN:
                            read_token(&parser_state, &token);
                            switch(token.token_type)
                            {
                                case TOKEN_ERROR:
                                    return 1;
                                case TOKEN_EOF:
                                    //write empty module
                                    return 0;
                                case TOKEN_NO_MATCH:
                                default:
                            }
                        default:
                            close_files_and_free_memory(&parser_state);
                            return 1;
                    }
                default:
                    close_files_and_free_memory(&parser_state);
                    return 1;
            }
        default:
            close_files_and_free_memory(&parser_state);
            return 1;
    }

    return 1;
}

void read_token(parser_state_t *parser_state, token_t *token)
{
    read_token_from_stream(parser_state->token_stream, token);
    //perhaps print error message here
}

void print_token_error(token_t *token)
{

}

void close_files_and_free_memory(parser_state_t *parser_state)
{
    free_token_stream(parser_state->token_stream);
    free_wasm_module(parser_state->wasm_module);
}
/*** end of file "parser.c" ***/