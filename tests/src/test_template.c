// Standard Library Header Files
#include <stdbool.h>

#include "../test_lib/test_lib.h"

// Module Header Files
//#include "../../path_to_header_file/header_file.h"

// Global Variables


// Test Functions
bool test_one()
{
    return (1 == 1);
}

bool test_two()
{
    return (2 == 2);
}

//Number Of Tests To Run
#define NUMBER_OF_TESTS 2

// Array Of Test Descriptions And Test Function Pointers
test_t tests[NUMBER_OF_TESTS] =
{
    {"One equals one", test_one},
    {"Two equals two", test_test_two}
};

int main()
{
    return tester(NUMBER_OF_TESTS, tests, "Testing assumptions about numbers");
}

/* --- end of "test_file_name.c" --- */

