/**
 * @file token_stream.h
 * @brief token_stream reads tokens from the preprocessor.
 *
 * After opening token_stream you can read from it by passing a pointer to a
 * token_t of your own and after the function returns you can examine the
 * different elements of it to decide what to do with it.
 */
#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H

/**
 * @brief Used to get back a result of the various read calls.
 *
 * You pass a pointer to a token_t that you own to the different read functions
 * and after the function returns you can examine what the values of the
 * elements were set to.
 */
typedef struct
{
    /// token_type is from token_types.h and keyword_matcher/keyword_tokens.h
    int    token_type;
    /// The line the first character of the token is on.
    int    token_line;
    /// The position in the line the first character of the token is.
    int    token_position;
    /// The length in bytes of the token string.
    size_t token_length;
    /**
     * @brief char pointer to the token.
     * 
     * A pointer to the buffer that holds the token, a pointer to an error
     * string, or a pointer to a data string allocated from the heap.
     */
    char   *token_string;
} token_t;

/**
 * @brief Type of value returned from open_token_stream().
 *
 * A void pointer to a structure that holds the state of a token stream.
 */
typedef void* token_stream_t;

/**
 * @brief Opens a file to read tokens from.
 *
 * Tries to open a preprocessor to read tokens from. The file name follows
 * rules for fopen(). It must either be the filename in the working directory
 * or full path to the file.
 * @param filename File to open.
 * @return token_stream_t
 */
token_stream_t open_token_stream(char *filename);

/**
 * @brief Frees token_strem.
 *
 * Closes file and frees any memory used to read from it.
 * @param token_stream
 */
void free_token_stream(token_stream_t token_stream);

/**
 * @brief Reads a token.
 *
 * The token_type is TOKEN_ERROR and the token_string is a pointer to a
 * constant string or a token_type from token_types.h or
 * keyword_matcher/keyword_tokens.h and token_string is a pointer to the
 * token_string buffer which must be copied from before another call to
 * read_token_from_stream() or read_extern_name_from_stream() if it is
 * something needed to keep like an identifier name or a number.
 * @param token_stream token_stream_t to read token from.
 * @param token token_t pointer that you own to get result back.
 */
void read_token_from_stream(token_stream_t token_stream, token_t *token);

/**
 * @brief Reads an external name string.
 *
 * The token_type is either TOKEN_ERROR and the token_string is a pointer to a
 * constant string or the token_type is TOKEN_EXTERNAL_NAME and the
 * token_string points to the token_stream buffer so it has to be copied
 * somewhere else before another call to read_token_from_stream() or
 * read_extern_name_from_stream().
 * @param token_stream token_stream_t to read external name from.
 * @param token token_t pointer that you own to get result back.
 */
void read_extern_name_from_stream(token_stream_t token_stream, token_t *token);

/**
 * @brief Reads data string.
 *
 * The token_type is either TOKEN_ERROR and the token_string is to a constant
 * string or the token_type is TOKEN_DATA_STRING and the token_string is a
 * pointer to a char array allocated from the heap (make sure to free it).
 * @param token_stream token_stream_t to read data string from.
 * @param token token_t pointer that you own to get result back.
 */
void read_data_from_stream(token_stream_t token_stream, token_t *token);

/**
 * @brief Not implemented, returns NULL.
 *
 * When this is finished, it will return the line from a file. This just returns
 * NULL at the moment. I think it will be needed for parser errors.
 * @param token_stream token_stream_t to get full line from.
 * @param line_number The line number to get from the file read from.
 * @return NULL
 */
char *get_token_stream_line_string(token_stream_t token_stream, int line_number);

#endif

// *** end of file "token_stream.h" ***
