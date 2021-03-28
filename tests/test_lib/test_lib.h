#ifndef TEST_LIB_H
#define TEST_LIB_H

typedef struct
{
    char *desc;
    bool (*test_function)();
} test_t;

int tester(int number_of_tests, test_t *tests, char *test_name);

#endif

/* --- end of file "test_lib..h" --- */

