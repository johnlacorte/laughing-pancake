// Standard Library Header Files
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../test_lib/test_lib.h"

// Module Header Files
#include "../../src/preprocessor/input_file_list/char_stream/char_stream_status.h"
#include "../../src/preprocessor/input_file_list/input_file_list.h"

// Test Functions
bool test_pass_null_to_functions()
{
    return
(
    (open_input_file(NULL, "input_file1.txt") == 0) &&
    (open_include_file(NULL, "input_file1.txt") == 0) &&
    (!is_input_file_open(NULL)) &&
    (!strcmp(input_file_name(NULL), "NULL POINTER PASSED TO input_file_name()!!")) &&
    (input_file_line(NULL) == 0) &&
    (input_file_line_position(NULL) == 0) &&
    (read_utf8_from_input_file(NULL) == CHAR_STREAM_PREPROC_DID_SOMETHING_WRONG) &&
    (next_input_file(NULL) == CHAR_STREAM_PREPROC_DID_SOMETHING_WRONG) &&
    (!strcmp(input_file_error_msg(NULL), "NULL POINTER PASSED TO input_file_error_msg()!!"))
);

}

input_file_list_t file_list;

bool test_pass_closed_file_to_functions()
{
    file_list = new_input_file_list();
    return
(
    (!is_input_file_open(file_list)) &&
    (!strcmp(input_file_name(file_list), "NO FILE OPEN!!")) &&
    (input_file_line(file_list) == 0) &&
    (input_file_line_position(file_list) == 0) &&
    (read_utf8_from_input_file(file_list) == CHAR_STREAM_EOF) &&
    (!strcmp(input_file_error_msg(file_list), "NO FILE OPEN!!"))
);

}

bool test_open_files()
{
    return
(
    (open_input_file(file_list, "input_file2.txt")) &&
    (open_input_file(file_list, "src/input_file3.txt")) &&
    (open_include_file(file_list, "input_file1.txt")) &&
    (is_input_file_open(file_list))
);

}

bool test_read_file1()
{
    //The three files together read "cat\ndog\nbird\n"<EOF>

    //The three read tests should probably be broken up into more tests
    //so I don't have to uncomment this block everytime I touch the code for
    //line and position numbers

/*    int ch = read_utf8_from_input_file(file_list);
    while(ch != CHAR_STREAM_EOF)
    {
        printf("File: %s [%d,%d] = %d\n", input_file_name(file_list),
               input_file_line(file_list),
               input_file_line_position(file_list), ch);
        ch = read_utf8_from_input_file(file_list);
    }
    return 0;*/

    return
(
    (read_utf8_from_input_file(file_list) == 'c') &&
    (!strcmp(input_file_name(file_list), "input_file1.txt")) &&
    (input_file_line(file_list) == 1) &&
    (input_file_line_position(file_list) == 1) &&
    (read_utf8_from_input_file(file_list) == 'a') &&
    (read_utf8_from_input_file(file_list) == 't') &&
    (input_file_line(file_list) == 1) &&
    (input_file_line_position(file_list) == 3) &&
    (read_utf8_from_input_file(file_list) == '\n') &&
    (input_file_line(file_list) == 2) &&
    (input_file_line_position(file_list) == 0) &&
    (read_utf8_from_input_file(file_list) == CHAR_STREAM_EOF) &&
    (next_input_file(file_list) == ' ')
);

}

bool test_read_file2()
{
    return
(
    (read_utf8_from_input_file(file_list) == 'd') &&
    (!strcmp(input_file_name(file_list), "input_file2.txt")) &&
    (input_file_line(file_list) == 1) &&
    (input_file_line_position(file_list) == 1) &&
    (read_utf8_from_input_file(file_list) == 'o') &&
    (read_utf8_from_input_file(file_list) == 'g') &&
    (input_file_line(file_list) == 1) &&
    (input_file_line_position(file_list) == 3) &&
    (read_utf8_from_input_file(file_list) == '\n') &&
    (input_file_line(file_list) == 2) &&
    (input_file_line_position(file_list) == 0) &&
    (read_utf8_from_input_file(file_list) == CHAR_STREAM_EOF) &&
    (next_input_file(file_list) == ' ')
);

}

bool test_read_file3()
{
    return
(
    (read_utf8_from_input_file(file_list) == 'b') &&
    (!strcmp(input_file_name(file_list), "input_file3.txt")) &&
    (input_file_line(file_list) == 1) &&
    (input_file_line_position(file_list) == 1) &&
    (read_utf8_from_input_file(file_list) == 'i') &&
    (read_utf8_from_input_file(file_list) == 'r') &&
    (read_utf8_from_input_file(file_list) == 'd') &&
    (input_file_line(file_list) == 1) &&
    (input_file_line_position(file_list) == 4) &&
    (read_utf8_from_input_file(file_list) == '\n') &&
    (input_file_line(file_list) == 2) &&
    (input_file_line_position(file_list) == 0) &&
    (read_utf8_from_input_file(file_list) == CHAR_STREAM_EOF) &&
    (next_input_file(file_list) == CHAR_STREAM_EOF)
);

}

//Number Of Tests To Run
#define NUMBER_OF_TESTS 6

// Array Of Test Descriptions And Test Function Pointers
test_t tests[NUMBER_OF_TESTS] =
{
    {"Pass NULL to functions with return values", test_pass_null_to_functions},
    {"Try functions when no files are open", test_pass_closed_file_to_functions},
    {"Open some files", test_open_files},
    {"Read from file1", test_read_file1},
    {"Read from file2", test_read_file2},
    {"Read from file3", test_read_file3},
};

int main()
{
    return tester(NUMBER_OF_TESTS, tests, "Testing input_file_stack");
}

/* --- end of "test_input_file_stack.c" --- */

