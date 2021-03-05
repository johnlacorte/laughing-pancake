#ifndef PREPOCESSOR_H
#define PREPROCESSOR_H

typedef struct
{
    int  length;
    int  max;
    int  status;
    char *buffer;
} char_buffer_t;

//So far everything is normal
#define PREPROC_OK 0

//Normal EOF
#define PREPROC_EOF -1

//Error opening file, error allocating memory, etc.
#define PREPROC_ERROR -2

//Some sort of error trying to read the file
#define PREPROC_FILE_ERROR -3

//Nobody needs to know the details of the state
typedef void* preproc_t;

//Allocates a structure from the heap and tries to open the file. Remember to
//check if it returns null and call preproc_free() when you are finished.
preproc_t preproc_open_wat(char *filename);

void preproc_free(preproc_t pre);

//Returns preprocessor status
int preproc_check_status(preproc_t pre);

char *preproc_error_msg(preproc_t pre);

int preproc_current_line(preproc_t pre);

int preproc_line_position(preproc_t pre);

//Returns a 7-bit character or status to indicate why it couldn't
int preproc_read_char(preproc_t pre);

void preproc_read_name(preproc_t pre, char *buffer);
void preproc_read_extern_name(preproc_t pre, char *buffer);
void preproc_read_data(preproc_t pre, char_buffer_t *buffer);
//Call this after reading a quotation mark and it will allocate a character
//array from the heap, check for valid utf-8 and convert escape sequences.
//Remember to free() this pointer when you are finished. I'm thinking of
//dumping the stuff that was read to a file in case of an error.
char *preproc_read_string(preproc_t pre);

void preproc_dump(preproc_t pre, char *filename);

#endif

/* end of file "preprocessor.h" */

