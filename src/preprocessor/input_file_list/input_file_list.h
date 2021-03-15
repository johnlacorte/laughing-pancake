#ifndef INPUT_FILE_LIST_H
#define INPUT_FILE_LIST_H

//A pointer to the state allocated at runtime
typedef void* input_file_list_t;

//Returns new input_file_list_t or NULL
input_file_list_t new_input_file_list();

//Takes a short or full path file name, tries to open it and adds it to the end
//of the list
int open_input_file(input_file_list_t file_list, char *filename);

//Takes a short or full path file name, tries to open it and adds it to the
//beginning of the list. Probably isn't going to be used for WebAssembly files.
int open_include_file(input_file_list_t file_list, char *filename);

//Returns true/false if any files are currently open
int is_input_file_open(input_file_list_t file_list);

//Closes any open files and free()'s any memory used
void free_input_file_list(input_file_list_t file_list);

//Returns short file name of file being read from
char *input_file_name(input_file_list_t file_list);

//Returns current line of file being read from
int input_file_line(input_file_list_t file_list);

//Returns position in current line in current file
int input_file_line_position(input_file_list_t file_list);

//Push a 7bit character back to the stream changes status to
//CHAR_STREAM_PREPROC_DID_SOMETHING_WRONG if you try to give it an int bigger
//than 7 bits
void push_7bit_char_to_input_file(input_file_list_t file_list, int ch);

//Returns 7bit character or EOF or error
int read_7bit_char_from_input_file(input_file_list_t file_list);

//Decodes Unicode codepoint and returns that or negative value for EOF or error
int32_t read_utf8_from_input_file(input_file_list_t file_list);

//Returns a string explaining the status/errors in more detail
char *input_file_error_msg(input_file_list_t file_list);

#endif

/*** end of file "input_file_list.h" ***/

