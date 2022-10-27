/**
 * @name main.c
 * @brief Implementation of compiler main
 * @authors 
 * @date 27.10.2022
 */

#include "parser.h"
#include "error.h"
#include <stdio.h>

int main()
{
    program();
    if (!error) {
        printf("OK\n");
    } else {
        fprintf(stderr, "ERROR\n");
    }   
}