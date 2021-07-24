// Standard Library Header Files
#include <stdbool.h>

#include "../test_lib/test_lib.h"

// Module Header Files
#include "../../src/preprocessor/preprocessor.h"

// Global Variables
preproc_t pre;

// Test Functions
bool test_pass_null_to_functions()
{
    return false;
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

