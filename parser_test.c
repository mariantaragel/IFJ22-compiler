/**
 * @name parser_test.c
 * @brief Parser testing
 * @authors Marián Tarageľ
 * @date 29.10.2022
 */

#include "parser.h"
#include "error.h"
#include <stdio.h>

int test_num = 1;

void test(char *test, error_codes_t correct_return_value)
{
    freopen(test, "r", stdin);
    program();
    printf("%d.test: ", test_num);
    test_num++;
    if (error == correct_return_value) {
        printf("\033[0;32m");
        printf("passed\n\033[0m");
    }
    else {
        printf("\033[0;31m");
        printf("fail\n\033[0m");
    }
}

int main()
{
    test("tests/test1.php", OK);
    test("tests/test2.php", OK);
    test("tests/test3.php", SYNTAX_ERROR);
    test("tests/test4.php", SYNTAX_ERROR);
    test("tests/test5.php", SYNTAX_ERROR);
    test("tests/test6.php", SYNTAX_ERROR);

    return 0;
}