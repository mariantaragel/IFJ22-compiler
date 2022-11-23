/**
 * @name parser_test.c
 * @brief Parser testing
 * @authors Marián Tarageľ
 * @date 23.11.2022
 */

#include "parser.h"
#include "error.h"
#include <stdio.h>

int num_of_tests = 0;
int correct_tests = 0;
int failed_tests = 0;

void test(char *test, error_codes_t correct_return_value)
{
    num_of_tests++;
    freopen(test, "r", stdin);
    printf("\n[ %s ] \n", test);
    AST_node_t* root = program();
    AST_print(root, stdout);
    AST_free(root);
    fprintf(stderr, "EXPECTED ");
    error_print(correct_return_value);
    error_print(error);
    if (error == correct_return_value) {
        correct_tests++;
        printf("\033[0;32m");
        printf("passed\n\033[0m");
    }
    else {
        failed_tests++;
        printf("\033[0;31m");
        printf("fail\n\033[0m");
    }
    error = OK;
    fclose(stdin);
}

int main()
{
    printf("Syntax Analyzer Tests\n");
    printf("=====================\n");

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
    test("tests/syntax/test25.php", LEXICAL_ERROR);
    test("tests/syntax/test26.php", LEXICAL_ERROR);
    test("tests/syntax/test27.php", LEXICAL_ERROR);
    test("tests/syntax/test28.php", LEXICAL_ERROR);
    test("tests/syntax/test29.php", LEXICAL_ERROR);
    test("tests/syntax/test30.php", LEXICAL_ERROR);
    test("tests/syntax/test31.php", OK);
    test("tests/syntax/test32.php", OK);
    test("tests/syntax/test33.php", OK);
    test("tests/syntax/test34.php", OK);
    test("tests/syntax/test35.php", OK);
    test("tests/syntax/test36.php", SYNTAX_ERROR);
    test("tests/syntax/test37.php", LEXICAL_ERROR);
    test("tests/syntax/test38.php", OK);
    test("tests/syntax/test39.php", OK);
    test("tests/syntax/test40.php", OK);
    test("tests/syntax/test41.php", OK);
    test("tests/syntax/test42.php", OK);
    test("tests/syntax/test43.php", OK);
    test("tests/syntax/test44.php", OK);

    printf("\n=====================\n");
    printf("TEST=%d \033[0;32m PASS=%d \033[0;31m FAIL=%d\033[0m\n", num_of_tests, correct_tests, failed_tests);

    return 0;
}