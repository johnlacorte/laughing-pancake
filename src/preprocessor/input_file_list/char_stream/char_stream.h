#ifndef CHAR_STREAM_H
#define CHAR_STREAM_H

typedef struct
{
    FILE *fp;
    int  status;
    char *error_msg;
} char_stream_t;

//Call this function before using a char_stream_t
void init_char_stream(char_stream_t *stream);

//filename is full path returns CHAR_STREAM_OK or CHAR_STREAM_FAILED_TO_OPEN
int open_char_stream(char_stream_t *stream, char *filename);

//Returns true/false
int is_char_stream_open(char_stream_t *stream);

//Closes char_stream
void close_char_stream(char_stream_t *stream);

//Push a 7bit character back to the stream changes status to
//CHAR_STREAM_PREPROC_DID_SOMETHING_WRONG if you try to give it an int bigger
//than 7 bits
void push_7bit_char(char_stream_t *stream, int ch);

//Returns 7bit character or EOF or error
int pop_7bit_char(char_stream_t *stream);

//Decodes Unicode codepoint and returns that or negative value for EOF or error
int32_t pop_utf8(char_stream_t *stream);

//Returns a string explaining the status/errors in more detail
char *char_stream_error_msg(char_stream_t *stream);

//Resets status to CHAR_STREAM_OK so you can easily test all the errors can
//be triggered
void char_stream_reset(char_stream_t *stream);

#endif

/* --- end of file "char_stream.h" --- */

