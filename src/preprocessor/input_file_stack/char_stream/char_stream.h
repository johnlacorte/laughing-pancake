#ifndef CHAR_STREAM_H
#define CHAR_STREAM_H

typedef struct
{
    FILE *fp;
    int  status;
    char *error_msg;
} char_stream_t;

void init_char_stream(char_stream_t *stream);

int open_char_stream(char_stream_t *stream, char *filename);

int is_char_stream_open(char_stream_t *stream);

void close_char_stream(char_stream_t *stream);

//I probably won't use this.
void push_7bit_char(char_stream_t *stream, int ch);

int32_t pop_utf8(char_stream_t *stream);

int pop_7bit_char(char_stream_t *stream);

char *char_stream_error_msg(char_stream_t *stream);

void char_stream_reset(char_stream_t *stream);

#endif

/* --- end of file "char_stream.h" --- */

