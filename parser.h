/**
 * @name parser.h
 * @brief Interface of top-down parser
 * @authors Marián Tarageľ
 * @date 10.10.2022
 */

#ifndef _PARSER_H
#define _PARSER_H

#include "token.h"

#define TRUE 1
#define FALSE 0

int program();

int php_start();

int program_body();

int php_end();

#endif