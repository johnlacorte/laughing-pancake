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

bool test_open_good()
{
    //I think open_preproc() should set status to PREPROC_ERROR if utf8_file
    //fails to open (it doesn't currently). 
    pre = open_preproc("good_preprocessor.txt");
    return (get_preproc_status(pre) == PREPROC_OK);
}

bool test_remove_comments()
{
    return (read_preproc_char(pre) == 'a');
}

bool test_tab_to_space()
{
    return
(
    (read_preproc_char(pre) == ' ') &&
    (read_preproc_char(pre) == 'b')
);

}

bool test_newline_to_space()
{
    return
(
    (read_preproc_char(pre) == ' ') &&
    (read_preproc_char(pre) == 'c')
);

}

bool test_remove_multiple_spaces()
{
    return
(
    (read_preproc_char(pre) == ' ') &&
    (read_preproc_char(pre) == 'd')
);

}

bool test_insert_space_before_open_paren()
{
    return
(
    (read_preproc_char(pre) == ' ') &&
    (read_preproc_char(pre) == '(')
);

}

bool test_insert_space_after_open_paren()
{
    return
(
    (read_preproc_char(pre) == ' ') &&
    (read_preproc_char(pre) == 'e')
);

}

bool test_space_before_identifier()
{
    return
(
    (read_preproc_char(pre) == ' ') &&
    (read_preproc_char(pre) == '$')
);

}

bool test_dollar_sign_in_identifier()
{
    return
(
    (read_preproc_char(pre) == 'f') &&
    (read_preproc_char(pre) == '$') &&
    (read_preproc_char(pre) == 'g')
);

}

bool test_insert_space_before_close_paren()
{
    return
(
    (read_preproc_char(pre) == ' ') &&
    (read_preproc_char(pre) == ')')
);

}

bool test_insert_space_after_close_paren()
{
    return (read_preproc_char(pre) == ' ');
}

bool test_eof()
{
    return (read_preproc_char(pre) == PREPROC_EOF);
}

bool test_close_good()
{
    free_preproc(pre);
    return true;
}

//Number Of Tests To Run
#define NUMBER_OF_TESTS 14

// Array Of Test Descriptions And Test Function Pointers
test_t tests[NUMBER_OF_TESTS] =
{
    {"Test passing null to functions", test_pass_null_to_functions},
    {"Open a file for preprocessing", test_open_good},
    {"Comments removed", test_remove_comments},
    {"Tabs converted to spaces", test_tab_to_space},
    {"Newlines converted to spaces", test_newline_to_space},
    {"Eat multiple spaces", test_remove_multiple_spaces},
    {"Insert space before open paren", test_insert_space_before_open_paren},
    {"Insert space after open paren", test_insert_space_after_open_paren},
    {"Insert space before identifer", test_space_before_identifier},
    {"Don\'t insert before dollar sign in identifier", test_dollar_sign_in_identifier},
    {"Insert space before close paren", test_insert_space_before_close_paren},
    {"Insert space after close paren", test_insert_space_after_close_paren},
    {"EOF", test_eof},
    {"Close file", test_close_good}
};

int main()
{
    return tester(NUMBER_OF_TESTS, tests, "Testing preprocessor");
}

/* --- end of "test_file_name.c" --- */

