#include "../test_lib/test_lib.h"

// Standard Library Header Files
// because char_stream_t has a FILE pointer in it.
#include <stdio.h>

// Module Header Files
#include "../../src/token_stream/preprocessor/char_stream/char_stream.h"

// Global Variables
char_stream_t good_file;
char_stream_t bad_file;

// Test Functions
int test_no_file()
{
    int return_value = open_char_stream(&good_file, "nonexisting.txt");

    return ((return_value == pop_7bit_char(&good_file)) &&
            (return_value == CHAR_STREAM_FAILED_TO_OPEN));
}

int test_open_good()
{
    int return_value = open_char_stream(&good_file, "good_utf8.txt");

    return (return_value == CHAR_STREAM_OK);
}

//int test_good_read_hello()
//int test_good_read_utf8()

int test_open_bad()
{
    int return_value = open_char_stream(&bad_file, "bad_utf8.txt");

    return (return_value == CHAR_STREAM_OK);
}

//Number Of Tests To Run
#define NUMBER_OF_TESTS 3

// Array Of Test Descriptions And Test Function Pointers
test_t tests[NUMBER_OF_TESTS] = 
{
    {"File not found error", test_no_file},
    {"Opening \"good_utf8.txt\"", test_open_good},
    {"Opening \"bad_utf8.txt\"", test_open_bad}
};

int main()
{
    return (!tester(NUMBER_OF_TESTS, tests,
            "Testing char_stream"));
}

/* --- end of "test_char_stream.c" --- */

