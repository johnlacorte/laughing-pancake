#ifndef INPUT_FILE_STACK_H
#define INPUT_FILE_STACK_H

typedef void* input_file_stack_t;

input_file_stack_t new_input_file_stack();

int open_input_file(input_file_stack_t stack, char *filename);

int is_input_file_open(input_file_stack_t stack);

char *input_file_name(input_file_stack_t stack);

void free_input_file_stack(input_file_stack_t stack);

int input_file_line(input_file_stack_t stack);

int input_file_line_position(input_file_stack_t stack);

int read_input_file_7bit(input_file_stack_t stack);

int32_t read_input_file_utf8(input_file_stack_t stack);

char *input_file_error_msg(input_file_stack_t stack);

#endif

/*** end of file "input_file_stack.h" ***/

