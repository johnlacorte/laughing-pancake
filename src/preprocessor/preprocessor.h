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

//Nobody needs to know the details of the state
typedef void* preproc_t;

//Allocate and initialize preprocessor state
preproc_t open_preproc(char *filename);

void free_preproc(preproc_t pre);

int  get_preproc_status(preproc_t pre);

char *get_preproc_error_msg(preproc_t pre);

int  get_preproc_line_number(preproc_t pre);

int  get_preproc_line_position(preproc_t pre);

void reset_preproc_status(preproc_t pre);

int  read_preproc_char(preproc_t pre);

int  read_preproc_extern_name(preproc_t pre);

int  read_preproc_data(preproc_t pre);

#endif

/* end of file "preprocessor.h" */
