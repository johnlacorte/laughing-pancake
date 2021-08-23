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

//reads a token from token stream and prints to stderr if it is of type
//TOKEN_ERROR
void read_token(parser_state_t *parser_state, token_t *token);

void close_files_and_free_memory(parser_state_t *parser_state);

int read_all_entrees(parser_state_t *parser_state, int line, int position);

void print_parser_error(token_t *token, char *msg);

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
        case TOKEN_NO_MATCH:
            //read_token() prints a message for these types automagically
            close_files_and_free_memory(&parser_state);
            return 1;
        case TOKEN_OPEN_PAREN:
            read_token(&parser_state, &token);
            switch(token.token_type)
            {
                case TOKEN_ERROR:
                case TOKEN_NO_MATCH:
                    close_files_and_free_memory(&parser_state);
                    return 1;
                case TOKEN_KEYWORD_MODULE:
                    read_token(&parser_state, &token);
                    switch(token.token_type)
                    {
                        case TOKEN_ERROR:
                        case TOKEN_NO_MATCH:
                            close_files_and_free_memory(&parser_state);
                            return 1;
                        case TOKEN_OPEN_PAREN:
                            //I think I should pass either the line and
                            //position of the token or a pointer to the token
                            //to the read_entries function
                            return read_all_entires(&parser_state,
                                                    token.token_line,
                                                    token.token_position);
                        case TOKEN_CLOSE_PAREN:
                            read_token(&parser_state, &token);
                            switch(token.token_type)
                            {
                                case TOKEN_ERROR:
                                case TOKEN_NO_MATCH:
                                    close_files_and_free_memory(&parser_state);
                                    return 1;
                                case TOKEN_EOF:
                                    //write empty module
                                    return 0;
                                default:
                                    //expected EOF
                                    print_parser_error(&token, "Expected EOF after \')\'.");
                                    close_files_and_free_memory(&parser_state);
                                    return 1;
                            }

                        case TOKEN_EOF:
                        default:
                            //expected '(' or ')'
                            print_parser_error(&token, "Expected \')\' or \'(\'.");
                            close_files_and_free_memory(&parser_state);
                            return 1;
                    }

                case TOKEN_EOF:
                default:
                    //expected module keyword
                    print_parser_error(&token, "Expected \'module\' keyword.");
                    close_files_and_free_memory(&parser_state);
                    return 1;
            }

        case TOKEN_EOF:
        default:
            //expected '(' message
            print_parser_error(&token, "Expected \'(\'.");
            close_files_and_free_memory(&parser_state);
            return 1;
    }
}

void print_token_error(token_t *token);

void print_no_match_error(token_t *token);

void read_token(parser_state_t *parser_state, token_t *token)
{
    read_token_from_stream(parser_state->token_stream, token);
    //perhaps print error message here
    if(token->token_type == TOKEN_ERROR)
    {
        print_token_error(token);
    }

    else
    {
        if(token->token_type == TOKEN_NO_MATCH)
        {
            print_no_match_error(token);
        }
    }
}

void close_files_and_free_memory(parser_state_t *parser_state)
{
    free_token_stream(parser_state->token_stream);
    free_wasm_module(parser_state->wasm_module);
}

bool read_entry(parser_state_t *parser_state, int line, int position);

int read_all_entrees(parser_state_t *parser_state, int line, int position)
{
    if(read_entry(parser_state, line, position))
    {
        token_t token;
        read_token(parser_state, &token);
        switch(token.token_type)
        {
            case TOKEN_ERROR:
            case TOKEN_NO_MATCH:
                close_files_and_free_memory(parser_state);
                return 1;
            case TOKEN_OPEN_PAREN:
                return read_all_entires(parser_state,
                                        token.token_line,
                                        token.token_position);
            case TOKEN_CLOSE_PAREN:
                read_token(parser_state, &token);
                switch(token.token_type)
                {
                    case TOKEN_ERROR:
                    case TOKEN_NO_MATCH:
                        close_files_and_free_memory(parser_state);
                        return 1;
                    case TOKEN_EOF:
                        //write module if EOF
                        return 0;
                    default:
                        print_parser_error(&token, "Expected EOF.");
                        close_files_and_free_memory(parser_state);
                        return 1;
                }
            case TOKEN_EOF:
            default:
                print_parser_error(&token, "Expected \')\' or \'(\'.");
                close_files_and_free_memory(parser_state);
                return 1;
        }
    }

    else
    {
        return 1;
    }
}

#include <stdio.h>

//prints to stderr a line indicating line and position of the token that caused
//an error
void print_token_location(token_t *token);

void print_parser_error(token_t *token, char *msg)
{

}

void print_token_error(token_t *token)
{

}

void print_no_match_error(token_t *token)
{

}

void print_token_location(token_t *token)
{

}

bool read_entry(parser_state_t *parser_state, int line, int position)
{

}

/*** end of file "parser.c" ***/
