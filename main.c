/**
 * @name main.c
 * @brief Implementation of compiler main
 * @authors Marián Tarageľ
 * @date 29.10.2022
 */

#include "parser.h"
#include "error.h"
#include <stdio.h>

int main()
{
    program();
    if (error == OK) {
        printf("OK\n");
    }
    else {
        fprintf(stderr, "ERROR\n");
    }
}