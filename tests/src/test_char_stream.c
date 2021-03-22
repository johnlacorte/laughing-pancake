#include "../test_lib/test_lib.h"

// Standard Library Header Files

#include <stdbool.h>
// because char_stream_t has a FILE pointer in it.
#include <stdio.h>
#include <string.h>

// Module Header Files
#include "../../src/preprocessor/input_file_list/char_stream/char_stream_status.h"
#include "../../src/preprocessor/input_file_list/char_stream/char_stream.h"

// Test Functions

// Global variables for the next couple functions.
char_stream_t good_file;

int test_no_file()
{
    int return_value = open_char_stream(&good_file, "nonexisting.txt");

    return
(
    (return_value == read_utf8_from_char_stream(&good_file)) &&
    (return_value == CHAR_STREAM_FAILED_TO_OPEN)
);

}

int test_open_good()
{
    int return_value = open_char_stream(&good_file, "good_utf8.txt");

    return (return_value == CHAR_STREAM_OK);
}

int test_good_read_hello()
{
    return
(
    (read_utf8_from_char_stream(&good_file) == 'h') &&
    (read_utf8_from_char_stream(&good_file) == 'e') &&
    (read_utf8_from_char_stream(&good_file) == 'l') &&
    (read_utf8_from_char_stream(&good_file) == 'l') &&
    (read_utf8_from_char_stream(&good_file) == 'o') &&
    (read_utf8_from_char_stream(&good_file) == ' ')
); 

}
int test_good_read_privet()
{
    return
(
    (read_utf8_from_char_stream(&good_file) == 0x43f) && //п
    (read_utf8_from_char_stream(&good_file) == 0x440) && //р
    (read_utf8_from_char_stream(&good_file) == 0x438) && // и
    (read_utf8_from_char_stream(&good_file) == 0x432) && // в
    (read_utf8_from_char_stream(&good_file) == 0x435) && // е
    (read_utf8_from_char_stream(&good_file) == 0x442) && // т
    (read_utf8_from_char_stream(&good_file) == ' ')
);

}

int test_good_read_ni_hao()
{
    return
(
    (read_utf8_from_char_stream(&good_file) == 0x4f60) && //你
    (read_utf8_from_char_stream(&good_file) == 0x597d) //好
);

}

int test_good_read_EOF()
{
    return (read_utf8_from_char_stream(&good_file) == CHAR_STREAM_EOF);
}

int test_good_close_file()
{
    close_char_stream(&good_file);

    return
(
    (read_utf8_from_char_stream(&good_file) == CHAR_STREAM_FILE_CLOSED) &&
    (read_utf8_from_char_stream(&good_file) == CHAR_STREAM_FILE_CLOSED)
);

}

// Global variables for the next couple functions.
char_stream_t bad_file;

int test_open_bad()
{
    int return_value = open_char_stream(&bad_file, "bad_utf8.txt");
    //opening a char_stream reads the first character to check for BOM
    //a bad character will set the status to an error before it is returned

    //This will read 0x65 at the beginning of the file
    read_utf8_from_char_stream(&bad_file);
    return (return_value == CHAR_STREAM_OK);
}

int test_bad_utf8_bad_initial_byte()
{
    //0x80
    int return_value = read_utf8_from_char_stream(&bad_file);
    char *msg = char_stream_error_msg(&bad_file);

    return
(
    (return_value == CHAR_STREAM_READ_FAILED) &&
    ((!strcmp(msg, "UTF8 sequence starts with invalid byte.")))
);

}

int test_char_stream_reset()
{
    //0x65
    char_stream_reset(&bad_file);

    return (read_utf8_from_char_stream(&bad_file) == 0x65);
}

int test_first_character_in_two_character_sequence_padded()
{
    //0xc0 0x80
    int return_value = read_utf8_from_char_stream(&bad_file);
    char *msg = char_stream_error_msg(&bad_file);
    char_stream_reset(&bad_file);
    read_utf8_from_char_stream(&bad_file);
    char_stream_reset(&bad_file);

    return
(
    (return_value == CHAR_STREAM_READ_FAILED) &&
    ((!strcmp(msg, "Unicode codepoint is padded (first byte of two).")))
);

}

int test_bad_utf8_bad_additional_byte()
{
    //0xc1 0x20
    int return_value = read_utf8_from_char_stream(&bad_file);
    char *msg = char_stream_error_msg(&bad_file);
    char_stream_reset(&bad_file);

    return
(
    (return_value == CHAR_STREAM_READ_FAILED) &&
    ((!strcmp(msg, "Invalid byte in UTF8 sequence.")))
);

}

int test_second_character_padded()
{
    //0xeo 0x80 0xbf
    int return_value = read_utf8_from_char_stream(&bad_file);
    char *msg = char_stream_error_msg(&bad_file);
    char_stream_reset(&bad_file);
    pop_byte_from_char_stream(&bad_file);

    return
(
    (return_value == CHAR_STREAM_READ_FAILED) &&
    ((!strcmp(msg, "Unicode codepoint is padded (second byte).")))
);
}

int test_read_utf16_surrogate()
{
    // 0xed 0xa0 0x80
    int return_value = read_utf8_from_char_stream(&bad_file);
    char *msg = char_stream_error_msg(&bad_file);
    char_stream_reset(&bad_file);

    return
(
    (return_value == CHAR_STREAM_READ_FAILED) &&
    ((!strcmp(msg, "Unicode codepoint is UTF16 surrogate.")))
);

}

int test_codepoint_out_of_range()
{
    //0xf7 0xbf 0xbf 0xbf
    int return_value = read_utf8_from_char_stream(&bad_file);
    char *msg = char_stream_error_msg(&bad_file);
    char_stream_reset(&bad_file);

    return
(
    (return_value == CHAR_STREAM_READ_FAILED) &&
    ((!strcmp(msg, "Unicode codepoint out of range.")))
);

}

int test_bad_utf8_eof_in_sequence()
{
    //0xc1 <EOF>
    int return_value = read_utf8_from_char_stream(&bad_file);
    char *msg = char_stream_error_msg(&bad_file);

    return
(
    (return_value == CHAR_STREAM_READ_FAILED) &&
    ((!strcmp(msg, "Unexpected EOF in UTF8 sequence.")))
);

}

//Number Of Tests To Run
#define NUMBER_OF_TESTS 16

// Array Of Test Descriptions And Test Function Pointers
test_t tests[NUMBER_OF_TESTS] = 
{
    {"File not found error", test_no_file},
    {"Opening \"good_utf8.txt\"", test_open_good},
    {"Reading \"hello\"", test_good_read_hello},
    {"Reading \"привет\"", test_good_read_privet},
    {"Reading \"你好\"", test_good_read_ni_hao},
    {"Reading EOF", test_good_read_EOF},
    {"Close file", test_good_close_file},
    {"Opening \"bad_utf8.txt\"", test_open_bad},
    {"Trigger error \"UTF8 sequence starts with invalid byte.\"", test_bad_utf8_bad_initial_byte},
    {"char_stream reset", test_char_stream_reset},
    {"Trigger error \"Unicode codepoint is padded (first byte of two).\"", test_first_character_in_two_character_sequence_padded},
    {"Trigger error \"Invalid byte in UTF8 sequence.\"", test_bad_utf8_bad_additional_byte},
    {"Trigger error \"Unicode codepoint is padded (second byte).\"", test_second_character_padded},
    {"Trigger error \"Unicode codepoint is UTF16 surrogate\"", test_read_utf16_surrogate},
    {"Trigger error \"Unicode codepoint out of range.\"", test_codepoint_out_of_range},
    {"Trigger error \"Unexpected EOF in UTF8 sequence.\"", test_bad_utf8_eof_in_sequence}
};

int main()
{
    return tester(NUMBER_OF_TESTS, tests,
            "Testing char_stream");
}

/* --- end of "test_char_stream.c" --- */

