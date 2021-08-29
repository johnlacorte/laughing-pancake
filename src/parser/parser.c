#include <stdbool.h>
#include <stdint.h>
#include "../token_stream/token_stream.h"
#include "../wasm_module/wasm_constants.h"
#include "../ast/ast.h"
#include "parser.h"

typedef struct
{
    token_stream_t token_stream;
    ast_t ast;
    char *output_file;
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
    parser_state.ast = new_ast();
    parser_state.output_file = output_file;
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
    free_ast(parser_state->ast);
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
    print_token_location(token);
    fprintf(stderr, "%s\n\n", msg);
}

void print_token_error(token_t *token)
{
    print_token_location(token);
    fprintf(stderr, "%s\n\n", token->token_string);
}

void print_no_match_error(token_t *token)
{
    print_token_location(token);
    fprintf(stderr, "Unknown token \"%s\".\n\n");
}

void print_token_location(token_t *token)
{
    fprintf(stderr,
            "\n\nAt [%d, %d]\n\n",
            token->token_line,
            token->token_position);
}

bool read_type_entry(parser_state_t *parser_state, int line, int position);
bool read_import_entry(parser_state_t *parser_state, int line, int position);
bool read_func_entry(parser_state_t *parser_state, int line, int position);
bool read_table_entry(parser_state_t *parser_state, int line, int position);
bool read_memory_entry(parser_state_t *parser_state, int line, int position);
bool read_global_entry(parser_state_t *parser_state, int line, int position);
bool read_export_entry(parser_state_t *parser_state, int line, int position);
bool read_start_entry(parser_state_t *parser_state, int line, int position);
bool read_elem_entry(parser_state_t *parser_state, int line, int position);
bool read_code_entry(parser_state_t *parser_state, int line, int position);
bool read_data_entry(parser_state_t *parser_state, int line, int position);
bool read_name_entry(parser_state_t *parser_state, int line, int position);

bool read_entry(parser_state_t *parser_state, int line, int position)
{
    token_t token;
    read_token(parser_state, &token);
    switch(token.token_type)
    {
        case TOKEN_ERROR:
        case TOKEN_NO_MATCH:
            close_files_and_free_memory(parser_state);
            return false;
        case TOKEN_KEYWORD_TYPE:
            return read_type_entry(parser_state, line, position);
        case TOKEN_KEYWORD_IMPORT:
            return read_import_entry(parser_state, line, position);
        case TOKEN_KEYWORD_FUNC:
            return read_func_entry(parser_state, line, position);
        case TOKEN_KEYWORD_TABLE:
            return read_table_entry(parser_state, line, position);
        case TOKEN_KEYWORD_MEMORY:
            return read_memory_entry(parser_state, line, position);
        case TOKEN_KEYWORD_GLOBAL:
            return read_global_entry(parser_state, line, position);
        case TOKEN_KEYWORD_EXPORT:
            return read_export_entry(parser_state, line, position);
        case TOKEN_KEYWORD_START:
            return read_start_entry(parser_state, line, position);
        case TOKEN_KEYWORD_ELEM:
            return read_elem_entry(parser_state, line, position);
        case TOKEN_KEYWORD_CODE:
            return read_code_entry(parser_state, line, position);
        case TOKEN_KEYWORD_DATA:
            return read_data_entry(parser_state, line, position);
        case TOKEN_KEYWORD_NAME:
            return read_name_entry(parser_state, line, position);
        case TOKEN_EOF:
        default:
            print_parser_error(&token,
                               "Expected keyword indicating type of entry.");
            close_files_and_free_memory(parser_state);
            return false;
    }
}

char *copy_identifier_name(token_t *token);

//Calls read_all_type_results if result keyword is found 
bool read_all_type_params(parser_state_t *parser_state,
                     value_type_array_t params,
                     value_type_array_t results);

bool read_all_type_results(parser_state_t *parser_state,
                     value_type_array_t params,
                     value_type_array_t results);

bool read_type_entry(parser_state_t *parser_state, int line, int position)
{
    char *name = NULL;
    value_type_array_t params = new_value_type_array();
    value_type_array_t results = new_value_type_array();
    token_t token;
    read_token(parser_state, &token);
    if(token.token_type == TOKEN_IDENTIFIER)
    {
        name = copy_identifier_name(&token);
        if(name == NULL)
        {
            free_value_type_array(params);
            free_value_type_array(results);
            close_files_and_free_memory(parser_state);
            return false;
        }

        else
        {
            read_token(parser_state, &token);
        }
    }

    switch(token.token_type)
    {
        case TOKEN_ERROR:
        case TOKEN_NO_MATCH:
            free_value_type_array(params);
            free_value_type_array(results);
            close_files_and_free_memory(parser_state);
            return false;
        case TOKEN_OPEN_PAREN:
            read_token(parser_state, &token);
            switch(token.token_type)
            {
                case TOKEN_ERROR:
                case TOKEN_NO_MATCH:
                    free_value_type_array(params);
                    free_value_type_array(results);
                    close_files_and_free_memory(parser_state);
                    return false;
                case TOKEN_KEYWORD_FUNC:
                    read_token(parser_state, &token);
                    switch(token.token_type)
                    {
                        case TOKEN_ERROR:
                        case TOKEN_NO_MATCH:
                            free_value_type_array(params);
                            free_value_type_array(results);
                            close_files_and_free_memory(parser_state);
                            return false;
                        case TOKEN_OPEN_PAREN:
                            read_token(parser_state, &token);
                            switch(token.token_type)
                            {
                                case TOKEN_ERROR:
                                case TOKEN_NO_MATCH:
                                    free_value_type_array(params);
                                    free_value_type_array(results);
                                    close_files_and_free_memory(parser_state);
                                    return false;
                                case TOKEN_KEYWORD_PARAM:
                                    return read_all_type_params(parser_state,
                                                            params,
                                                            results);
                                case TOKEN_KEYWORD_RESULT:
                                    return read_all_type_results(parser_state,
                                                                params,
                                                                results);
                                case TOKEN_EOF:
                                default:
                                    print_parser_error(&token,
                                    "Expected keyword \'param\' or \'result\'.");
                                    free_value_type_array(params);
                                    free_value_type_array(results);
                                    close_files_and_free_memory(parser_state);
                                    return false;
                            }
                        case TOKEN_EOF:
                        default:
                            print_parser_error(&token, "Expected \'(\'.");
                            free_value_type_array(params);
                            free_value_type_array(results);
                            close_files_and_free_memory(parser_state);
                            return false;
                    }

                case TOKEN_EOF:
                default:
                    print_parser_error(&token, "Expected keyword \'func\'.");
                    free_value_type_array(params);
                    free_value_type_array(results);
                    close_files_and_free_memory(parser_state);
                    return false;
            }

        case TOKEN_CLOSE_PAREN:
            print_parser_error(&token,
                                "Type entry missing params and return types;\n"
                                " ( type <name>? ( func <param>* <result>* ) )");
        case TOKEN_EOF:
        default:
            print_parser_error(&token,
                                   "Expected \'(\'.");
            free_value_type_array(params);
            free_value_type_array(results);
            close_files_and_free_memory(parser_state);
            return false;
    }
}

bool read_import_entry(parser_state_t *parser_state, int line, int position)
{
    return false;
}

bool read_func_entry(parser_state_t *parser_state, int line, int position)
{
    return false;
}

bool read_table_entry(parser_state_t *parser_state, int line, int position)
{
    return false;
}

bool read_memory_entry(parser_state_t *parser_state, int line, int position)
{
    return false;
}

bool read_global_entry(parser_state_t *parser_state, int line, int position)
{
    return false;
}

bool read_export_entry(parser_state_t *parser_state, int line, int position)
{
    return false;
}

bool read_start_entry(parser_state_t *parser_state, int line, int position)
{
    return false;
}

bool read_elem_entry(parser_state_t *parser_state, int line, int position)
{
    return false;
}

bool read_code_entry(parser_state_t *parser_state, int line, int position)
{
    return false;
}

bool read_data_entry(parser_state_t *parser_state, int line, int position)
{
    return false;
}

bool read_name_entry(parser_state_t *parser_state, int line, int position)
{
    return false;
}

char *copy_identifier_name(token_t *token)
{
    return NULL;
}

bool read_all_type_params(parser_state_t *parser_state,
                     value_type_array_t params,
                     value_type_array_t results)
{
    return false;
}

bool read_all_type_results(parser_state_t *parser_state,
                     value_type_array_t params,
                     value_type_array_t results)
{
    return false;
}

/*** end of file "parser.c" ***/
