/**
 * @name parser_test.c
 * @brief Parser testing
 * @authors Marián Tarageľ
 * @date 16.11.2022
 */

#include "parser.h"
#include "error.h"
#include <stdio.h>

int test_num = 1;

void test(char *test, error_codes_t correct_return_value)
{
    freopen(test, "r", stdin);
    printf("\n[ %s ] \n", test);
    AST_node_t* root = program();
    AST_print(root, stdout);
    AST_free(root);
    fprintf(stderr, "EXPECTED ");
    error_print(correct_return_value);
    error_print(error);
    if (error == correct_return_value) {
        printf("\033[0;32m");
        printf("passed\n\033[0m");
    }
    else {
        printf("\033[0;31m");
        printf("fail\n\033[0m");
    }
    error = OK;
    fclose(stdin);
}

int main()
{
    test("tests/syntax/test1.php", OK);
    test("tests/syntax/test2.php", OK);
    test("tests/syntax/test3.php", SYNTAX_ERROR);
    test("tests/syntax/test4.php", SYNTAX_ERROR);
    test("tests/syntax/test5.php", SYNTAX_ERROR);
    test("tests/syntax/test6.php", SYNTAX_ERROR);
    test("tests/syntax/test7.php", SYNTAX_ERROR);
    test("tests/syntax/test8.php", SYNTAX_ERROR);
    test("tests/syntax/test9.php", SYNTAX_ERROR);
    test("tests/syntax/test10.php", OK);
    test("tests/syntax/test11.php", OK);
    test("tests/syntax/test12.php", OK);
    test("tests/syntax/test13.php", OK);
    test("tests/syntax/test14.php", SYNTAX_ERROR);
    test("tests/syntax/test15.php", OK);
    test("tests/syntax/test16.php", OK);
    test("tests/syntax/test17.php", OK);
    test("tests/syntax/test18.php", OK);
    test("tests/syntax/test19.php", OK);
    test("tests/syntax/test20.php", SYNTAX_ERROR);
    test("tests/syntax/test21.php", SYNTAX_ERROR);
    test("tests/syntax/test22.php", SYNTAX_ERROR);
    test("tests/syntax/test23.php", OK);
    test("tests/syntax/test24.php", OK);

    return 0;
}