#ifndef PREPOCESSOR_H
#define PREPROCESSOR_H

//So far everything is normal
#define PREPROC_OK 0

//Normal EOF
#define PREPROC_EOF -1

#define PREPROC_END_OF_STRING -2

//Error opening file, error allocating memory, etc.
#define PREPROC_ERROR -3

//Some sort of error trying to read the file
#define PREPROC_FILE_ERROR -4

//Nobody needs to know the details of the state
typedef void* preproc_t;

preproc_t new_preproc();

preproc_t open_preproc(char *include_path, char *filename);

int is_preproc_open(preproc_t pre);

void free_preproc(preproc_t pre);

int preproc_status(preproc_t pre);

char *preproc_error_msg(preproc_t pre);

int preproc_line(preproc_t pre);

int preproc_line_position(preproc_t pre);

char *preproc_file_name(preproc_t pre);

int read_preproc_char(preproc_t pre);

int read_preproc_extern_name(preproc_t pre, char *buffer);

int read_preproc_data(preproc_t pre, char_buffer_t *buffer);

void preproc_dump(preproc_t pre, char *filename);

#endif

/* end of file "preprocessor.h" */

