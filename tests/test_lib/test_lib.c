#include <stdio.h>

#include "test_lib.h"

#define FG_YELLOW "\x1B[1;33m"
#define FG_GREEN "\x1B[1;32m"
#define FG_RED "\x1B[1;31m"
#define FG_NORMAL "\x1B[0m"

int tester(int number_of_tests, test_t *tests, char *test_name)
{
    int result = 1;
    printf("\n%s%s:%s\n", FG_YELLOW, test_name, FG_NORMAL);
    for(int i = 0; i < number_of_tests; i++)
    {
        result = tests[i].test_function();
        printf("  Test> ");
        if(result)
        {
            printf("%s: %sPassed%s\n", tests[i].desc, FG_GREEN, FG_NORMAL);
        }

        else
        {
            printf("%s: %sFailed%s\n", tests[i].desc, FG_RED, FG_NORMAL);
            printf("\n%s%d tests passed, test number %d failed.%s\n", FG_RED, i, (i + 1), FG_NORMAL);
            break;
        }
    }

    if(result)
    {
        printf("\n%sAll %d tests passed.%s\n", FG_GREEN, number_of_tests, FG_NORMAL);
    }

    return !result;
}

/* --- end of file "test_char_stream.c" --- */

