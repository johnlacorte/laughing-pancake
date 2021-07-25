// Standard Library Header Files
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../test_lib/test_lib.h"

// Module Header Files
#include "../../src/preprocessor/preprocessor.h"

// Global Variables
preproc_t pre;

// Test Functions
bool test_pass_null_to_functions()
{
    return
(
    (get_preproc_status(NULL) == PREPROC_ERROR)&&
    (!strcmp(get_preproc_error_msg(NULL), "Failed to allocate memory for preprocessor.")) &&
    (get_preproc_line_number(NULL) == -1) &&
    (get_preproc_line_position(NULL) == -1) &&
    (read_preproc_char(NULL) == PREPROC_ERROR) &&
    (read_preproc_extern_name(NULL) == PREPROC_ERROR) &&
    (read_preproc_data_string(NULL) == PREPROC_ERROR)
);

}

bool test_open_preprocessor()
{
    
    return false;
}

//Number Of Tests To Run
#define NUMBER_OF_TESTS 2

// Array Of Test Descriptions And Test Function Pointers
test_t tests[NUMBER_OF_TESTS] =
{
    {"Test passing null to functions", test_pass_null_to_functions},
    {"Open a file for preprocessing", test_open_preprocessor}
};

int main()
{
    return tester(NUMBER_OF_TESTS, tests, "Testing preprocessor");
}

/* --- end of "test_file_name.c" --- */

