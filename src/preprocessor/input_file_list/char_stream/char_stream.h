#ifndef CHAR_STREAM_H
#define CHAR_STREAM_H

/*#define CHAR_STREAM_OK 0
#define CHAR_STREAM_EOF -1
#define CHAR_STREAM_FAILED_TO_OPEN -2
#define CHAR_STREAM_PREPROC_DID_SOMETHING_WRONG -3
#define CHAR_STREAM_READ_FAILED -4
#define CHAR_STREAM_FILE_CLOSED -5 */

typedef struct
{
    FILE    *fp;
    int     status;
    char    *error_msg;
} char_stream_t;

//Call this function before using a char_stream_t
void init_char_stream(char_stream_t *stream);

//filename is full path returns CHAR_STREAM_OK or CHAR_STREAM_FAILED_TO_OPEN
int open_char_stream(char_stream_t *stream, char *filename);

//Returns true/false
int is_char_stream_open(char_stream_t *stream);

//Closes char_stream
void close_char_stream(char_stream_t *stream);

//Decodes Unicode codepoint and returns that or negative value for EOF or error
int32_t pop_utf8(char_stream_t *stream);

//Returns a string explaining the status/errors in more detail
char *char_stream_error_msg(char_stream_t *stream);

//Returns next byte of the stream mainly just to eat remaining bytes of a bad
//utf8 sequence in tests
int pop_byte(char_stream_t *stream);

//Resets status to CHAR_STREAM_OK so you can easily test all the errors can
//be triggered
void char_stream_reset(char_stream_t *stream);

#endif

/* --- end of file "char_stream.h" --- */

/* Notes
It might become better for each header file in the preprocessor to have its own
set of statuses for each step rather than using char_stream_status.h.

Using <stdbool.h> here wouldn't help much here but using it everywhere would
probably make tests more readable.
*/
