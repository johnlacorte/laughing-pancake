#ifndef CHAR_STREAM_H
#define CHAR_STREAM_H

#define CHAR_STREAM_EOF -1
#define CHAR_STREAM_FAILED_TO_OPEN -2
#define CHAR_STREAM_PREPROC_DID_SOMETHING_WRONG -3
#define CHAR_STREAM_READ_FAILED -4
#define CHAR_STREAM_FILE_CLOSED -5

typedef struct
{
    FILE *fp;
    int  status;
    char *error_msg;
    int expected_bytes;
} char_stream_t;

int open_char_stream(char_stream_t *stream, char *filename);

void close_char_stream(char_stream_t *stream);

void push_7bit_char(char_stream_t *stream, int ch);

int pop_utf8_byte(char_stream_t *stream);

int pop_7bit_char(char_stream_t *stream);

char *char_stream_error_msg(char_stream_t *stream);

void char_stream_reset(char_stream_t *stream);

#endif

/* --- end of file "char_stream.h" --- */

