#ifndef UTF8_FILE_H
#define UTF8_FILE_H

#define UTF8_FILE_OK 1
#define UTF8_FILE_EOF -1
#define UTF8_FILE_ERROR -2
#define UTF8_FILE_CLOSED -4

#define MAX_ERROR_MSG_LENGTH 128

typedef struct
{
    FILE    *fp;
    int32_t status;
    bool    return_temp_next;
    int32_t temp;
    char    *error_msg;
    char    error_msg_buffer[MAX_ERROR_MSG_LENGTH];
    int     line;
    int     line_position;
} utf8_file_t;

void init_utf8_file(utf8_file_t *file);

bool open_utf8_file(utf8_file_t *file, char *filename);

void close_utf8_file(utf8_file_t *file);

int32_t get_utf8_file_status(utf8_file_t *file);

char *get_utf8_file_error_msg(utf8_file_t *file);

int32_t set_utf8_file_read_error(utf8_file_t *file, char *msg);

int get_utf8_file_line_number(utf8_file_t *file);

int get_utf8_file_line_position(utf8_file_t *file);

int32_t pop_byte_from_utf8_file(utf8_file_t *file);

void reset_utf8_file_status(utf8_file_t *file);

int32_t read_char_from_utf8_file(utf8_file_t *file);

int32_t read_unicode_escape_from_utf8_file(utf8_file_t *file);

#endif
