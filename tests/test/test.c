#include <stdio.h>

typedef struct
{
    char *desc;
    int (*test_function)();
} test_t;

int tester(int number_of_tests, test_t *tests)
{
    int result = 1;
    for(int i = 0; i < number_of_tests; i++)
    {
        result = tests[i].test_function();
        if(result)
        {
            printf("%s : Passed\n", tests[i].desc);
        }

        else
        {
            printf("%s : Failed\n", tests[i].desc);
            break;
        }
    }

    return result;
}

/* --- end of file "test_char_stream.c" --- */

