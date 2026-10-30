/*
 * Project     : Lexical-Analyzer-Project
 * File        : lexical_analyzer.h
 * Author      : Supriya Velishetty
 * Description : Structure and function declarations.
 */

#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H
#include <stdio.h>

typedef struct
{
    char word[100];
    char type[30];
    int line;
} Token;

void analyze_file(FILE *file);

#endif
