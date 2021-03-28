#ifndef INPUT_FILE_LIST_H
#define INPUT_FILE_LIST_H

//These constants are either going to be expanded and put into use to add
//additional features or just deleted. Looking things like this helps me come
//up with good names.
/*#define INPUT_FILE_OK 0
#define INPUT_FILE_EOF -1
#define INPUT_FILE_FAILED_TO_OPEN -2
#define INPUT_FILE_PREPROC_DID_SOMETHING_WRONG -3
#define INPUT_FILE_READ_FAILED -4
#define INPUT_FILE_FILE_CLOSED -5 */

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

//Decodes Unicode codepoint and returns that or negative value for EOF or error
int32_t read_utf8_from_input_file(input_file_list_t file_list);

//Returns a space or CHAR_STREAM_EOF if there are no more files
int32_t next_input_file(input_file_list_t file_list);

//Returns a string explaining the status/errors in more detail
char *input_file_error_msg(input_file_list_t file_list);

#endif

/*** end of file "input_file_list.h" ***/

/* Notes
If input_file_list ends up handling escape sequences other than escaped
newlines it might make more sense to have its own status and set of constants
to go with them instead of just passing the char_stream status along. Having
its own set of constants for return values makes the documentation in my
imagination look neater as well.

Position in line is currently the number of codepoints. Better than number of
bytes but not as good as number of graphemes. It's good enough for most people. 
*/