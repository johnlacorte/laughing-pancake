#ifndef PREPOCESSOR_H
#define PREPROCESSOR_H

//So far everything is normal
#define PREPROC_OK 0

//Normal EOF
#define PREPROC_EOF -1

//Signals the end of an extern_name or data (where you have escape sequences)
#define PREPROC_END_OF_STRING -2

//Error opening file, error allocating memory, etc.
#define PREPROC_ERROR -3

//Some error trying to read the file (get current file, line, and line position)
#define PREPROC_FILE_ERROR -4

//Nobody needs to know the details of the state
typedef void* preproc_t;

//Allocate and initialize preprocessor state
preproc_t new_preproc();

//Opens one or more files to build a WebAssembly module from
int open_preproc_files(int number_of_files, char **filenames);

int is_preproc_open(preproc_t pre);

void free_preproc(preproc_t pre);

int preproc_status(preproc_t pre);

char *preproc_error_msg(preproc_t pre);

int preproc_line(preproc_t pre);

int preproc_line_position(preproc_t pre);

char *preproc_current_file_name(preproc_t pre);

int read_preproc_char(preproc_t pre);

int read_preproc_extern_name(preproc_t pre);

int read_preproc_data(preproc_t pre);

void preproc_dump(preproc_t pre, char *filename);

#endif

/* end of file "preprocessor.h" */

