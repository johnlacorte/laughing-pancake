//Do I really need to check for EOF seperately if default does the same thing?
//Make sure if I'm printing a line of source due to an error that it can handle
//the error being an unexpected EOF and print properly
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "../token_stream/token_stream.h"
#include "../wasm_module/wasm_constants.h"
#include "../ast/ast.h"
#include "parser.h"

typedef struct
{
    token_stream_t token_stream;
    ast_t ast;
    int current_line;
    int current_position;
}parser_state_t;

//reads a token from token stream and prints to stderr if it is of type
//TOKEN_ERROR or TOKEN_NO_MATCH. Changes the token type from TOKEN_NO_MATCH
//to TOKEN_ERROR after the no match message is printed. Closes input file
//and frees memory. If it is an identifier, it adds the name to the ast
//and changes the string pointer from the token_stream buffer over to that.
void read_token(parser_state_t *parser_state, token_t *token);

//I was thinking that this could take a char pointer to the error message
//because a token error would otherwise print the token error then the
//else branch would print the expected message. Then an else branch would
//only need to free any additional data structures used and return false;
bool next_token_is(parser_state_t *parser_state, int token_type, char *error_msg);

//This probably should return a bool as well
bool read_all_entrees(parser_state_t *parser_state);

void print_parser_error(parser_state_t *parser_state, char *msg);

//read_all_entrees() doesn't need line and position passed anymore
bool open_file_for_parsing(char *input_file, ast_t ast)
{
    parser_state_t parser_state;
    parser_state.token_stream = open_token_stream(input_file);
    parser_state.ast = ast;
    if(next_token_is(&parser_state, TOKEN_OPEN_PAREN, "Expected \'(\'."))
    {
        if(next_token_is(&parser_state, TOKEN_KEYWORD_MODULE, "Expected \'module\' keyword."))
        {
            token_t token;
            read_token(&parser_state, &token);
            switch(token.token_type)
            {
                case TOKEN_ERROR:
                    break;
                case TOKEN_OPEN_PAREN:
                    return read_all_entrees(&parser_state);
                case TOKEN_CLOSE_PAREN:
                    if(next_token_is(&parser_state, TOKEN_EOF, "Expected EOF."))
                    {
                        return true;
                    }

                    break;
                default:
                    print_parser_error(&parser_state, "Expected \')\' or \'(\'.");
            }
        }
    }

    return false;
}

void print_token_error(token_t *token);

void close_files_and_free_memory(parser_state_t *parser_state);

void print_no_match_error(token_t *token);

void read_token(parser_state_t *parser_state, token_t *token)
{
    //set location in parser state
    read_token_from_stream(parser_state->token_stream, token);
    parser_state->current_line = token->token_line;
    parser_state->current_position = token->token_position;
    switch(token->token_type)
    {
        case TOKEN_ERROR:
            print_token_error(token);
            close_files_and_free_memory(parser_state);
            break;
        case TOKEN_NO_MATCH:
            print_no_match_error(token);
            token->token_type = TOKEN_ERROR;
            close_files_and_free_memory(parser_state);
            break;
        case TOKEN_IDENTIFIER:
            token->token_string = add_identifier_name(parser_state->ast,
                                                      token->token_string);
            if(token->token_string == NULL)
            {
                token->token_type = TOKEN_ERROR;
                close_files_and_free_memory(parser_state);
            }

            break;
    }
}

bool next_token_is(parser_state_t *parser_state, int token_type, char *error_msg)
{
    token_t token;
    read_token(parser_state, &token);
    if(token.token_type != TOKEN_ERROR)
    {
        if(token.token_type == token_type)
        {
            return true;
        }

        else
        {
            print_parser_error(parser_state, error_msg);
        }
    }

    return false;
}

bool read_entry(parser_state_t *parser_state);

bool read_all_entrees(parser_state_t *parser_state)
{
    //There's something wrong here. When this is called an open paren has
    //already been matched so I imagine this can be combined with the
    //read_entry() function below. Like this should just call read_entry()
    //and if the return value is true, check for an open paren and call
    //itself or if it is a close paren check for EOF and write the module
    if(read_entry(parser_state))
    {
        token_t token;
        read_token(parser_state, &token);
        switch(token.token_type)
        {
            case TOKEN_ERROR:
                return false;
            case TOKEN_OPEN_PAREN:
                return read_all_entires(parser_state);
            case TOKEN_CLOSE_PAREN:
                read_token(parser_state, &token);
                switch(token.token_type)
                {
                    case TOKEN_ERROR:
                        return false;
                    case TOKEN_EOF:
                        return true;
                    default:
                        print_parser_error(parser_state, "Expected EOF.");
                        return false;
                }

            default:
                print_parser_error(parser_state, "Expected \')\' or \'(\'.");
                return false;
        }
    }

    else
    {
        return false;
    }
}

//prints to stderr a line indicating line and position of the token that caused
//an error
void print_token_location(int line, int position);

void print_parser_error(parser_state_t *parser_state, char *msg)
{
    //print_token_location
    fprintf(stderr, "%s\n\n", msg);
    close_files_and_free_memory(parser_state);
}

void print_token_error(token_t *token)
{
    print_token_location(token->token_line, token->token_position);
    fprintf(stderr, "%s\n\n", token->token_string);
}

void close_files_and_free_memory(parser_state_t *parser_state)
{
    free_token_stream(parser_state->token_stream);
    free_ast(parser_state->ast);
}

void print_no_match_error(token_t *token)
{
    print_token_location(token->token_line, token->token_position);
    fprintf(stderr, "Unknown token \"%s\".\n\n");
}

void print_token_location(int line, int position)
{
    fprintf(stderr,
            "\n\nAt [%d, %d]\n\n",
            line,
            position);
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

bool read_entry(parser_state_t *parser_state)
{
    token_t token;
    read_token(parser_state, &token);
    //get line and position from parser state
    int line = parser_state->current_line;
    int position = parser_state->current_position;
    switch(token.token_type)
    {
        case TOKEN_ERROR:
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
        default:
            print_parser_error(parser_state,
                               "Expected keyword indicating type of entry.");
            return false;
    }
}

//If naming type entries the name pointer has to be passed along to these
//functions, however it seems like a sort of useless thing
//Calls read_all_type_results if result keyword is found 
bool read_all_type_params(parser_state_t *parser_state,
                          char *type_name,
                          value_type_array_t params,
                          value_type_array_t results);

//pass name pointer here too
bool read_all_type_results(parser_state_t *parser_state,
                           char *type_name,
                           value_type_array_t params,
                           value_type_array_t results);

bool read_type_entry(parser_state_t *parser_state, int line, int position)
{
    char *type_name = NULL;
    value_type_array_t params = new_value_type_array();
    value_type_array_t results = new_value_type_array();
    token_t token;
    read_token(parser_state, &token);
    //read_token copies string from buffer to the ast
    if(token.token_type == TOKEN_IDENTIFIER)
    {
        type_name = token.token_string;
        read_token(parser_state, &token);
    }

    switch(token.token_type)
    {
        case TOKEN_ERROR:
        case TOKEN_NO_MATCH:
            free_value_type_array(params);
            free_value_type_array(results);
            return false;
        case TOKEN_OPEN_PAREN:
            if(next_token_is(parser_state, TOKEN_KEYWORD_FUNC, "Expected keyword \'func\'."))
            {
                if(next_token_is(parser_state, TOKEN_OPEN_PAREN, "Expected \'(\'."))
                {
                    read_token(parser_state, &token);
                    switch(token.token_type)
                    {
                        case TOKEN_ERROR:
                        case TOKEN_NO_MATCH:
                            free_value_type_array(params);
                            free_value_type_array(results);
                            return false;
                        case TOKEN_KEYWORD_PARAM:
                            return read_all_type_params(parser_state,
                                                        type_name,
                                                        params,
                                                        results);
                        case TOKEN_KEYWORD_RESULT:
                            return read_all_type_results(parser_state,
                                                        type_name,
                                                        params,
                                                        results);
                        case TOKEN_EOF:
                        default:
                            print_parser_error(parser_state,
                                  "Expected keyword \'param\' or \'result\'.");
                            free_value_type_array(params);
                            free_value_type_array(results);
                            return false;
                    }
                }

                else
                {
                    //expected open paren
                    free_value_type_array(params);
                    free_value_type_array(results);
                    return false;
                }
            }

            else
            {
                //expected func
                free_value_type_array(params);
                free_value_type_array(results);
                return false;
            }

        case TOKEN_CLOSE_PAREN:
            print_parser_error(parser_state,
                                "Type entry missing params and return types;\n"
                              " ( type <name>? ( func <param>* <result>* ) )");
        default:
            print_parser_error(parser_state, "Expected \'(\'.");
            free_value_type_array(params);
            free_value_type_array(results);
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

bool read_value_type(parser_state_t *parser_state, value_type_t *value_type);

bool read_all_type_params(parser_state_t *parser_state,
                          char *type_name,
                          value_type_array_t params,
                          value_type_array_t results)
{
    //I think this needs any name passed to this function
    //I might add a special message if a name is given to a param
    //make sure this and read_all_results() stop after the second close paren
    //because read_all_entries will check the stuff after.
    token_t token;
    value_type_t value_type;
    if(read_value_type(parser_state, &value_type))
    {
        if(next_token_is(parser_state, TOKEN_CLOSE_PAREN))
        {
            //I think this was supposed to add a new param
            if(add_type_entry(parser_state->ast, NULL, params, results) >= 0)
            {
                //) or (
                read_token(parser_state, &token);
                switch(token.token_type)
                {
                    case TOKEN_ERROR:
                    case TOKEN_NO_MATCH:
                        free_value_type_array(params);
                        free_value_type_array(results);
                        return false;
                    case TOKEN_OPEN_PAREN:
                        read_token(parser_state, &token);
                        switch(token.token_type)
                        {
                            case TOKEN_ERROR:
                            case TOKEN_NO_MATCH:
                                free_value_type_array(params);
                                free_value_type_array(results);
                                return false;
                            case TOKEN_KEYWORD_PARAM:
                                return read_all_type_params(parser_state,
                                                            type_name,
                                                            params,
                                                            results);
                            case TOKEN_KEYWORD_RESULT:
                                return read_all_type_results(parser_state,
                                                                type_name,
                                                                params,
                                                                results);
                            case TOKEN_EOF:
                            default:
                                print_parser_error(parser_state,
                                  "Expected keyword \'param\' or \'result\'.");
                                free_value_type_array(params);
                                free_value_type_array(results);
                                return false;
                        }
                    case TOKEN_CLOSE_PAREN:
                        //End of type entry
                        
                    case TOKEN_EOF:
                    default:
                        print_parser_error(parser_state,
                         "Expected \'(\' or \')\'.");
                        free_value_type_array(params);
                        free_value_type_array(results);
                        return false;
                }
            }

            else
            {
                //adding type to ast failed it should print an error message
                //and free the value type arrays
                close_files_and_free_memory(parser_state);
                return false;
            }
        }

        else
        {
            //expected )
        }
    }

    else
    {
        print_parser_error(parser_state,
             "Expected value type keyword.");
        free_value_type_array(params);
        free_value_type_array(results);
        return false;
    }
    //the old code

    read_token(parser_state, &token);
    switch(token.token_type)
    {
        case TOKEN_ERROR:
        case TOKEN_NO_MATCH:
            free_value_type_array(params);
            free_value_type_array(results);
            return false;
        case TOKEN_CLOSE_PAREN:
            //add to the array and see what's next
            if(add_type_entry(parser_state->ast, NULL, params, results) >= 0)
            {
                //check for another
                read_token(parser_state, &token);
                switch(token.token_type)
                {
                    case TOKEN_ERROR:
                    case TOKEN_NO_MATCH:
                        free_value_type_array(params);
                        free_value_type_array(results);
                        return false;
                    case TOKEN_OPEN_PAREN:
                        read_token(parser_state, &token);
                        switch(token.token_type)
                        {
                            case TOKEN_ERROR:
                            case TOKEN_NO_MATCH:
                                free_value_type_array(params);
                                free_value_type_array(results);
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
                                print_parser_error(parser_state,
                                  "Expected keyword \'param\' or \'result\'.");
                                free_value_type_array(params);
                                free_value_type_array(results);
                                return false;
                        }
                    case TOKEN_CLOSE_PAREN:
                        //End of type entry
                    case TOKEN_EOF:
                    default:
                        print_parser_error(parser_state,
                         "Expected \'(\' or \')\'.");
                        free_value_type_array(params);
                        free_value_type_array(results);
                        return false;
                }
            }

            else
            {
                //assume add_type_entry() printed an error message and freed
                //value type arrays
                close_files_and_free_memory(parser_state);
                return false;
            }
        case TOKEN_EOF:
        default:
            print_parser_error(parser_state, "Expected \')\'.");
            free_value_type_array(params);
            free_value_type_array(results);
            return false;
    }
}

bool read_all_type_results(parser_state_t *parser_state,
                           char *type_name,
                           value_type_array_t params,
                           value_type_array_t results)
{
    //I think this needs any name passed to this function
    return false;
}

bool read_value_type(parser_state_t *parser_state, value_type_t *value_type)
{
    token_t token;
    read_token(parser_state, &token);
    switch(token.token_type)
    {
        case TOKEN_ERROR:
        case TOKEN_NO_MATCH:
            return false;
        case TOKEN_KEYWORD_I32:
            value_type = VALUE_I32;
            return true;
        case TOKEN_KEYWORD_I64:
            value_type = VALUE_I64;
            return true;
        case TOKEN_KEYWORD_F32:
            value_type = VALUE_F32;
            return true;
        case TOKEN_KEYWORD_F64:
            value_type = VALUE_F64;
            return true;
        default:
            return false;
    }
}

/*** end of file "parser.c" ***/
