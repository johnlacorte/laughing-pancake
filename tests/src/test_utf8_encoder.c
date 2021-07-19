// Standard Library Header Files
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../test_lib/test_lib.h"

// Module Header Files
#include "../../src/preprocessor/utf8_encoder/utf8_encoder.h"

// Global Variables
utf8_encoder_t encoder;

// Test Functions
bool test_init_encoder()
{
    init_utf8_encoder(&encoder);
    return (!strcmp(get_utf8_encoder_error_msg(), "Encoder okay.");
}

bool test_encoder_is_empty_after_init()
{
    return (is_utf8_encoder_empty(&encoder));
}

bool test_good_encode()
{

}

bool test_encode_negative()
{

}

bool test_codepoint_too_big()
{

}

bool test_encoder_not_empty()
{

}

bool test_read_when_empty()
{

}

//Number Of Tests To Run
#define NUMBER_OF_TESTS 2

// Array Of Test Descriptions And Test Function Pointers
test_t tests[NUMBER_OF_TESTS] =
{
    {"Init encoder", test_init_encoder},
    {"Encoder is empty", test_encoder_is_empty_after_init}
};

int main()
{
    return tester(NUMBER_OF_TESTS, tests, "Testing utf8_encoder");
}

/* --- end of "test_file_name.c" --- */

