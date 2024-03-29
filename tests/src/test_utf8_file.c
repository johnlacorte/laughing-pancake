// Standard Library Header Files

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../test_lib/test_lib.h"

// Module Header Files
#include "../../src/preprocessor/utf8_file/utf8_file.h"

// Test Functions

// Global variables for the next couple functions.
utf8_file_t good_file;

bool pass_null_to_all_functions()
{
    return true;
}

bool test_no_file()
{
    good_file = open_utf8_file("nonexisting.txt");
    int32_t first_read = read_char_from_utf8_file(good_file);
    char *msg = get_utf8_file_error_msg(good_file);
    free_utf8_file(good_file);
    return
(
    (first_read == UTF8_FILE_ERROR) &&
    (!strcmp(msg, "open_utf8_file() error: unable to open file."))
);

}

/* good_utf8.txt
<BOM>!hello привет 你好\r\r\n
*/

bool test_open_good()
{
    good_file = open_utf8_file("good_utf8.txt");
    return (get_utf8_file_status(good_file) == UTF8_FILE_OK);
}

bool test_byte_order_mark_skipped()
{
    return (read_char_from_utf8_file(good_file) == '!');
}

bool test_good_read_hello()
{
    return
(
    (read_char_from_utf8_file(good_file) == 'h') &&
    (read_char_from_utf8_file(good_file) == 'e') &&
    (read_char_from_utf8_file(good_file) == 'l') &&
    (read_char_from_utf8_file(good_file) == 'l') &&
    (read_char_from_utf8_file(good_file) == 'o') &&
    (read_char_from_utf8_file(good_file) == ' ')
); 

}

bool test_good_read_privet()
{
    return
(
    (read_char_from_utf8_file(good_file) == 0x43f) && //п
    (read_char_from_utf8_file(good_file) == 0x440) && //р
    (read_char_from_utf8_file(good_file) == 0x438) && // и
    (read_char_from_utf8_file(good_file) == 0x432) && // в
    (read_char_from_utf8_file(good_file) == 0x435) && // е
    (read_char_from_utf8_file(good_file) == 0x442) && // т
    (read_char_from_utf8_file(good_file) == ' ')
);

}

bool test_good_read_ni_hao()
{
    return
(
    (read_char_from_utf8_file(good_file) == 0x4f60) && //你
    (read_char_from_utf8_file(good_file) == 0x597d) //好
);

}

bool test_cr()
{
    return (read_char_from_utf8_file(good_file) == '\n');
}

bool test_crlf()
{
    return (read_char_from_utf8_file(good_file) == '\n');
}

bool test_good_read_EOF()
{
    return (read_char_from_utf8_file(good_file) == UTF8_FILE_EOF);
}

bool test_good_close_file()
{
    free_utf8_file(good_file);

    return true;
}

/* bad_utf8.txt
65(normal 7bit character)
EF BB BF(BOM found in second read from file)
65(normal 7bit character to check stream error was reset)
80(bad initial byte)
C0 80(a two byte sequence starting with C0 is padded because all the bits read
from the first byte are 0 and the six bits in the second byte could have just
stored in a single byte)
C1 20(first byte is okay but second is not)
E0 80 BF(all bits from first and second byte in three byte sequence are 0)
ED A0 80(utf16 surrogate)
F7 BF BF BF(too large codepoint)
C1(first byte okay followed by EOF)
*/

// Global variables for the next couple functions.
utf8_file_t bad_file;

bool test_open_bad()
{
    bad_file = open_utf8_file("bad_utf8.txt");
    //This will read 0x65 at the beginning of the file
    return (read_char_from_utf8_file(bad_file) == 0x65);
}

bool test_byte_order_mark_error()
{
    int return_value = read_char_from_utf8_file(bad_file);
    char *msg = get_utf8_file_error_msg(bad_file);

    return
(
    (return_value == UTF8_FILE_ERROR) &&
    ((!strcmp(msg, "Byte Order Marker read in file.")))
);

}

bool test_char_stream_reset()
{
    //0x65
    reset_utf8_file_status(bad_file);

    return (read_char_from_utf8_file(bad_file) == 0x65);
}

bool test_bad_utf8_bad_initial_byte()
{
    //0x80
    int return_value = read_char_from_utf8_file(bad_file);
    char *msg = get_utf8_file_error_msg(bad_file);

    return
(
    (return_value == UTF8_FILE_ERROR) &&
    ((!strcmp(msg, "UTF8 sequence starts with invalid byte.")))
);

}

bool test_first_byte_in_two_byte_sequence_padded()
{
    //0xc0 0x80
    reset_utf8_file_status(bad_file);
    int return_value = read_char_from_utf8_file(bad_file);
    char *msg = get_utf8_file_error_msg(bad_file);
    reset_utf8_file_status(bad_file);
    read_char_from_utf8_file(bad_file);

    return
(
    (return_value == UTF8_FILE_ERROR) &&
    ((!strcmp(msg, "Unicode codepoint is padded (first byte of two).")))
);

}

bool test_bad_utf8_bad_additional_byte()
{
    //0xc1 0x20
    reset_utf8_file_status(bad_file);
    int return_value = read_char_from_utf8_file(bad_file);
    char *msg = get_utf8_file_error_msg(bad_file);

    return
(
    (return_value == UTF8_FILE_ERROR) &&
    ((!strcmp(msg, "Invalid byte in UTF8 sequence.")))
);

}

bool test_second_byte_padded()
{
    //0xeo 0x80 0xbf
    reset_utf8_file_status(bad_file);
    int return_value = read_char_from_utf8_file(bad_file);
    char *msg = get_utf8_file_error_msg(bad_file);
    reset_utf8_file_status(bad_file);
    pop_byte_from_utf8_file(bad_file);

    return
(
    (return_value == UTF8_FILE_ERROR) &&
    ((!strcmp(msg, "Unicode codepoint is padded (second byte).")))
);
}

bool test_read_utf16_surrogate()
{
    // 0xed 0xa0 0x80
    reset_utf8_file_status(bad_file);
    int return_value = read_char_from_utf8_file(bad_file);
    char *msg = get_utf8_file_error_msg(bad_file);

    return
(
    (return_value == UTF8_FILE_ERROR) &&
    ((!strcmp(msg, "Unicode codepoint is UTF16 surrogate.")))
);

}

bool test_codepoint_out_of_range()
{
    //0xf7 0xbf 0xbf 0xbf
    reset_utf8_file_status(bad_file);
    int return_value = read_char_from_utf8_file(bad_file);
    char *msg = get_utf8_file_error_msg(bad_file);

    return
(
    (return_value == UTF8_FILE_ERROR) &&
    ((!strcmp(msg, "Unicode codepoint out of range.")))
);

}

bool test_bad_utf8_eof_in_sequence()
{
    //0xc1 <EOF>
    reset_utf8_file_status(bad_file);
    int return_value = read_char_from_utf8_file(bad_file);
    char *msg = get_utf8_file_error_msg(bad_file);
    free_utf8_file(bad_file);

    return
(
    (return_value == UTF8_FILE_ERROR) &&
    ((!strcmp(msg, "Unexpected EOF in UTF8 sequence.")))
);

}

/*unicode_escapes.txt
\u{1111} (good escape sequence)
\u0x80 (0x80 is bad initial byte, this should return that error instead of an escape code error)
\ua (no open curly)
\u{g ('g' not a hex digit)
\u{1234567 (too many hex digits)
\u{aa (unexpected EOF)
*/

utf8_file_t escapes_file;

bool test_open_escapes_file()
{
    escapes_file = open_utf8_file("unicode_escapes.txt");

    return (get_utf8_file_status(escapes_file) == UTF8_FILE_OK);
}

bool test_good_escape_sequence()
{
    return 
(
    (read_char_from_utf8_file(escapes_file) == '\\') &&
    (read_char_from_utf8_file(escapes_file) == 'u') &&
    (read_unicode_escape_from_utf8_file(escapes_file) == 0x1111)
);

}

//Finish rewriting the rest from here

bool test_error_passthrough_in_escape()
{
    //0x80
    return
(
    (read_char_from_utf8_file(escapes_file) == '\\') &&
    (read_char_from_utf8_file(escapes_file) == 'u') &&
    (read_unicode_escape_from_utf8_file(escapes_file) == UTF8_FILE_ERROR) &&
    ((!strcmp(get_utf8_file_error_msg(escapes_file), "UTF8 sequence starts with invalid byte.")))
);

}

bool test_no_open_curly_in_escape()
{
    reset_utf8_file_status(escapes_file);
    return
(
    (read_char_from_utf8_file(escapes_file) == '\\') &&
    (read_char_from_utf8_file(escapes_file) == 'u') &&
    (read_unicode_escape_from_utf8_file(escapes_file) == UTF8_FILE_ERROR) &&
    ((!strcmp(get_utf8_file_error_msg(escapes_file), "Expected \'{\' in unicode escape sequence.")))
);

}

bool test_not_hex_digit_in_escape()
{
    reset_utf8_file_status(escapes_file);
    return
(
    (read_char_from_utf8_file(escapes_file) == '\\') &&
    (read_char_from_utf8_file(escapes_file) == 'u') &&
    (read_unicode_escape_from_utf8_file(escapes_file) == UTF8_FILE_ERROR) &&
    ((!strcmp(get_utf8_file_error_msg(escapes_file), "Expected hex digit in unicode escape sequence.")))
);

}

bool test_too_long_escape()
{
    reset_utf8_file_status(escapes_file);
    return
(
    (read_char_from_utf8_file(escapes_file) == '\\') &&
    (read_char_from_utf8_file(escapes_file) == 'u') &&
    (read_unicode_escape_from_utf8_file(escapes_file) == UTF8_FILE_ERROR) &&
    ((!strcmp(get_utf8_file_error_msg(escapes_file), "A maximum of 6 hex digits are allowed in unicode escape sequences.")))
);

}

bool test_eof_in_escape()
{
    reset_utf8_file_status(escapes_file);
    return
(
    (read_char_from_utf8_file(escapes_file) == '\\') &&
    (read_char_from_utf8_file(escapes_file) == 'u') &&
    (read_unicode_escape_from_utf8_file(escapes_file) == UTF8_FILE_ERROR) &&
    ((!strcmp(get_utf8_file_error_msg(escapes_file), "Unexpected EOF in unicode escape sequence.")))
);

}

bool test_close_unicode_escape()
{
    free_utf8_file(escapes_file);
    return true;
}

/* byte_escapes.txt
\ff
\f(byte 0x80)
\g
\fg
\f(EOF)
 */

bool test_open_bytes_escapes()
{
    escapes_file = open_utf8_file("byte_escapes.txt");

    return (get_utf8_file_status(escapes_file) == UTF8_FILE_OK);
}

bool test_good_byte_escape()
{
/*
    int32_t slash = read_char_from_utf8_file(escapes_file);
    int32_t first_digit = read_char_from_utf8_file(escapes_file);
    int32_t return_value = read_byte_escape_from_utf8_file(escapes_file, first_digit);
    char *msg = get_utf8_file_error_msg(escapes_file);
    printf("\nslash=%d\nfirst_digit=%d\nreturn_value=%d\nerror_message=%s\n", slash, first_digit, return_value, msg);
    return false;
*/
    return
(
    (read_char_from_utf8_file(escapes_file) == '\\') &&
    (read_byte_escape_from_utf8_file(escapes_file, read_char_from_utf8_file(escapes_file)) == 0xff)
);

}

bool test_encoding_error_in_byte_escape()
{
    return
(
    (read_char_from_utf8_file(escapes_file) == '\\') &&
    (read_byte_escape_from_utf8_file(escapes_file, read_char_from_utf8_file(escapes_file)) == UTF8_FILE_ERROR) &&
    ((!strcmp(get_utf8_file_error_msg(escapes_file), "UTF8 sequence starts with invalid byte.")))
);

}

bool test_first_digit_not_hex_in_byte_escape()
{
    reset_utf8_file_status(escapes_file);
    return
(
    (read_char_from_utf8_file(escapes_file) == '\\') &&
    (read_byte_escape_from_utf8_file(escapes_file, read_char_from_utf8_file(escapes_file)) == UTF8_FILE_ERROR) &&
    ((!strcmp(get_utf8_file_error_msg(escapes_file), "First character in byte escape wasn\'t hex digit (preprocessor did something wrong).")))
);

}

bool test_second_digit_not_hex_in_byte_escape()
{
    reset_utf8_file_status(escapes_file);
    return
(
    (read_char_from_utf8_file(escapes_file) == '\\') &&
    (read_byte_escape_from_utf8_file(escapes_file, read_char_from_utf8_file(escapes_file)) == UTF8_FILE_ERROR) &&
    ((!strcmp(get_utf8_file_error_msg(escapes_file), "Second character in byte escape wasn\'t hex digit.")))
);

}

bool test_eof_in_byte_escape()
{
    reset_utf8_file_status(escapes_file);
    return
(
    (read_char_from_utf8_file(escapes_file) == '\\') &&
    (read_byte_escape_from_utf8_file(escapes_file, read_char_from_utf8_file(escapes_file)) == UTF8_FILE_ERROR) &&
    ((!strcmp(get_utf8_file_error_msg(escapes_file), "Unexpected EOF in byte escape sequence.")))
);

}

bool test_close_byte_escapes()
{
    free_utf8_file(escapes_file);
    return true;
}

//Number Of Tests To Run
#define NUMBER_OF_TESTS 36

// Array Of Test Descriptions And Test Function Pointers
test_t tests[NUMBER_OF_TESTS] = 
{
    {"Pass NULL to all functions", pass_null_to_all_functions},
    {"File not found error", test_no_file},
    {"Opening \"good_utf8.txt\"", test_open_good},
    {"Byte order mark detected", test_byte_order_mark_skipped},
    {"Reading \"hello\"", test_good_read_hello},
    {"Reading \"привет\"", test_good_read_privet},
    {"Reading \"你好\"", test_good_read_ni_hao},
    {"Convert \\r to \\n", test_cr},
    {"Convert \\r\\n to \\n", test_crlf},
    {"Reading EOF", test_good_read_EOF},
    {"Close file", test_good_close_file},
    {"Opening \"bad_utf8.txt\"", test_open_bad},
    {"Trigger error \"Byte Order Marker read in file.\"", test_byte_order_mark_error},
    {"char_stream reset", test_char_stream_reset},
    {"Trigger error \"UTF8 sequence starts with invalid byte.\"", test_bad_utf8_bad_initial_byte},
    {"Trigger error \"Unicode codepoint is padded (first byte of two).\"", test_first_byte_in_two_byte_sequence_padded},
    {"Trigger error \"Invalid byte in UTF8 sequence.\"", test_bad_utf8_bad_additional_byte},
    {"Trigger error \"Unicode codepoint is padded (second byte).\"", test_second_byte_padded},
    {"Trigger error \"Unicode codepoint is UTF16 surrogate\"", test_read_utf16_surrogate},
    {"Trigger error \"Unicode codepoint out of range.\"", test_codepoint_out_of_range},
    {"Trigger error \"Unexpected EOF in UTF8 sequence.\"", test_bad_utf8_eof_in_sequence},
    {"Opening \"unicode_escapes.txt\"", test_open_escapes_file},
    {"Reading \\u{1111}", test_good_escape_sequence},
    {"Trigger error \"UTF8 sequence starts with invalid byte.\"", test_error_passthrough_in_escape},
    {"Trigger error \"Expected \'{\' in unicode escape sequence.\"", test_no_open_curly_in_escape},
    {"Trigger error \"Expected hex digit in unicode escape sequence.\"", test_not_hex_digit_in_escape},
    {"Trigger error \"A maximum of 6 hex digits are allowed in unicode escape sequences.\"", test_too_long_escape},
    {"Trigger error \"Unexpected EOF in unicode escape sequence.\"", test_eof_in_escape},
    {"Close file", test_close_unicode_escape},
    {"Opening \"byte_escapes.txt\"", test_open_bytes_escapes},
    {"Reading \\ff", test_good_byte_escape},
    {"Trigger error \"UTF8 sequence starts with invalid byte.\"", test_encoding_error_in_byte_escape},
    {"Trigger error \"First character in byte escape wasn\'t hex digit (preprocessor did something wrong).\"", test_first_digit_not_hex_in_byte_escape},
    {"Trigger error \"Second character in byte escape wasn\'t hex digit.\"", test_second_digit_not_hex_in_byte_escape},
    {"Trigger error \"Unexpected EOF in byte escape sequence.\"", test_eof_in_byte_escape},
    {"Close file", test_close_byte_escapes}
};

int main()
{
    return tester(NUMBER_OF_TESTS, tests,
            "Testing char_stream");
}

/* --- end of "test_utf8_file.c" --- */

