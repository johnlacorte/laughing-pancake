#include "../token_stream/token_stream.h"
//#include "../wasm_module/wasm_module.h"
#include "parser.h"

int open_file_for_parsing(char *input_file, char *output_file, output_type_t output_type)
{
    token_stream_t token_stream = open_token_stream(input_file);
    token_t token;
    //Does read_token_from_stream() handle NULL and return an error?
    read_token_from_stream(token_stream, &token);
    switch(token.token_type)
    {
        case TOKEN_ERROR:
        case TOKEN_EOF:
        case TOKEN_NO_MATCH:
        case TOKEN_OPEN_PAREN:
            read_token_from_stream(token_stream, &token);
            switch(token.token_type)
            {
                case TOKEN_ERROR:
                case TOKEN_EOF:
                case TOKEN_NO_MATCH:
                case TOKEN_KEYWORD_MODULE:
                    //Probably safe to open a module 
                    read_token_from_stream(token_stream, &token);
                    switch(token.token_type)
                    {
                        case TOKEN_ERROR:
                        case TOKEN_EOF:
                        case TOKEN_NO_MATCH:
                        case TOKEN_CLOSE_PAREN:
                            read_token_from_stream(token_stream, &token);
                            switch(token.token_type)
                            {
                                case TOKEN_ERROR:
                                case TOKEN_EOF:
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

/*** end of file "parser.c" ***/