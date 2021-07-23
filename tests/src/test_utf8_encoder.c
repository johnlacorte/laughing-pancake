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
    return (!strcmp(get_utf8_encoder_error_msg(&encoder), "Encoder okay."));
}

bool test_encoder_is_empty_after_init()
{
    return (is_utf8_encoder_empty(&encoder));
}

bool test_good_encode()
{
    //U+1f431 a cat emoji
    //011111 010000 110001
    //11110000 10011111 10010000 10110001
    //0xf0 0x9f 0x90 0xb1
    encode_codepoint_to_utf8(&encoder, 0x01f431);

    return
(
    (read_next_byte_from_encoder(&encoder) == 0xf0) &&
    (read_next_byte_from_encoder(&encoder) == 0x9f) &&
    (read_next_byte_from_encoder(&encoder) == 0x90) &&
    (read_next_byte_from_encoder(&encoder) == 0xb1) &&
    (is_utf8_encoder_empty(&encoder) == true)
);

}

bool test_encode_negative()
{
    encode_codepoint_to_utf8(&encoder, -1);
    return
(
    (read_next_byte_from_encoder(&encoder) == ENCODER_ERROR) &&
    (!strcmp(get_utf8_encoder_error_msg(&encoder), "Encoder was given a negative number to encode."))
);

}

bool test_codepoint_too_big()
{
    init_utf8_encoder(&encoder);
    encode_codepoint_to_utf8(&encoder, 0x77BFBFBF);
    return
(
    (read_next_byte_from_encoder(&encoder) == ENCODER_ERROR) &&
    (!strcmp(get_utf8_encoder_error_msg(&encoder), "Encoder given codepoint out of range."))
);

}

bool test_encoder_not_empty()
{
    init_utf8_encoder(&encoder);
    encode_codepoint_to_utf8(&encoder, 'a');
    encode_codepoint_to_utf8(&encoder, 'a');
    return
(
    (read_next_byte_from_encoder(&encoder) == ENCODER_ERROR) &&
    (!strcmp(get_utf8_encoder_error_msg(&encoder), "Encoder was given a codepoint when not empty."))
);

}

bool test_read_when_empty()
{
    init_utf8_encoder(&encoder);
    return
(
    (read_next_byte_from_encoder(&encoder) == ENCODER_ERROR) &&
    (!strcmp(get_utf8_encoder_error_msg(&encoder), "Preprocessor tried reading from empty utf8 encoder."))
);

}

//Number Of Tests To Run
#define NUMBER_OF_TESTS 7

// Array Of Test Descriptions And Test Function Pointers
test_t tests[NUMBER_OF_TESTS] =
{
    {"Init encoder", test_init_encoder},
    {"Encoder is empty", test_encoder_is_empty_after_init},
    {"Test if utf8_encoder properly encodes cat emoji", test_good_encode},
    {"Trigger error \"Encoder was given a negative number to encode.\"", test_encode_negative},
    {"Trigger error \"Encoder given codepoint out of range.\"", test_codepoint_too_big},
    {"Trigger error \"Encoder was given a codepoint when not empty.\"", test_encoder_not_empty},
    {"Trigger error \"Preprocessor tried reading from empty utf8 encoder.\"", test_read_when_empty}
};

int main()
{
    return tester(NUMBER_OF_TESTS, tests, "Testing utf8_encoder");
}

/* --- end of "test_file_name.c" --- */

