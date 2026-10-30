/*
 * Project Name    : Lexical-Analyzer-Project
 * Author          : Supriya Velishetty
 * Register Number : 25034wp_0029
 * Course          : Advanced Embedded Systems - Emertxe
 * Description     : Open a C source file and analyze tokens.
 */
 
#include <stdio.h>
#include "lexical_analyzer.h"

int main(int argc, char *argv[])
{
    FILE *file;
    if (argc != 2)
    {
        printf("Usage: ./a.out sample.c\n");
        return 1;
    }
    file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Error: cannot open input file.\n");
        return 1;
    }
    printf("%-8s %-25s %s\n", "LINE", "TOKEN", "TYPE");
    analyze_file(file);
    fclose(file);
    return 0;
}
