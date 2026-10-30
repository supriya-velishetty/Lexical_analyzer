/*
 * Project     : Lexical-Analyzer-Project
 * File        : lexical_analyzer.c
 * Author      : Supriya Velishetty
 * Description : Read source character by character and classify basic C tokens.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexical_analyzer.h"

/* Check whether a word is a C keyword. */
int is_keyword(char word[])
{
    char keywords[32][12] = {
        "auto", "break", "case", "char", "const", "continue", "default", "do",
        "double", "else", "enum", "extern", "float", "for", "goto", "if",
        "int", "long", "register", "return", "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
    };
    int i;
    for (i = 0; i < 32; i++)
    {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

/* Print one token with its line number. */
void print_token(Token token)
{
    printf("%-8d %-25s %s\n", token.line, token.word, token.type);
}

/* Read a source file one character at a time. */
void analyze_file(FILE *file)
{
    Token token;
    int ch, next, previous;
    int index, line = 1;

    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
        {
            line++;
            continue;
        }
        if (isspace((unsigned char)ch))
            continue;

        /* Skip single-line and multi-line comments. */
        if (ch == '/')
        {
            next = fgetc(file);
            if (next == '/')
            {
                while ((ch = fgetc(file)) != EOF && ch != '\n') {}
                if (ch == '\n') line++;
                continue;
            }
            if (next == '*')
            {
                previous = 0;
                while ((ch = fgetc(file)) != EOF)
                {
                    if (ch == '\n') line++;
                    if (previous == '*' && ch == '/') break;
                    previous = ch;
                }
                if (ch == EOF) printf("Error: comment not closed.\n");
                continue;
            }
            if (next != EOF) ungetc(next, file);
        }

        token.line = line;
        index = 0;
        token.word[index++] = (char)ch;
        token.word[index] = '\0';

        /* Keywords and identifiers. */
        if (isalpha((unsigned char)ch) || ch == '_')
        {
            while ((next = fgetc(file)) != EOF &&
                   (isalnum((unsigned char)next) || next == '_'))
            {
                if (index < 99) token.word[index++] = (char)next;
            }
            token.word[index] = '\0';
            if (next != EOF) ungetc(next, file);
            if (is_keyword(token.word)) strcpy(token.type, "KEYWORD");
            else strcpy(token.type, "IDENTIFIER");
        }
        /* Simple decimal integers and floating-point values. */
        else if (isdigit((unsigned char)ch))
        {
            int dots = 0;
            while ((next = fgetc(file)) != EOF &&
                   (isdigit((unsigned char)next) || next == '.'))
            {
                if (next == '.') dots++;
                if (index < 99) token.word[index++] = (char)next;
            }
            token.word[index] = '\0';
            if (next != EOF) ungetc(next, file);
            if (dots > 1) strcpy(token.type, "INVALID NUMBER");
            else strcpy(token.type, "NUMBER");
        }
        /* Read strings and character literals, including escaped quotes. */
        else if (ch == '"' || ch == '\'')
        {
            int quote = ch;
            int escaped = 0;
            int closed = 0;
            while ((next = fgetc(file)) != EOF)
            {
                if (next == '\n') line++;
                if (index < 99) token.word[index++] = (char)next;
                if (next == quote && !escaped) { closed = 1; break; }
                if (next == '\\' && !escaped) escaped = 1;
                else escaped = 0;
                if (next == '\n') break;
            }
            token.word[index] = '\0';
            if (!closed) strcpy(token.type, "INVALID LITERAL");
            else if (quote == '"') strcpy(token.type, "STRING");
            else strcpy(token.type, "CHARACTER");
        }
        /* Common C operators; recognize two-character operators. */
        else if (strchr("+-*/%=!<>|&^~", ch) != NULL)
        {
            next = fgetc(file);
            if (next != EOF &&
                ((ch == '+' && (next == '+' || next == '=')) ||
                 (ch == '-' && (next == '-' || next == '=' || next == '>')) ||
                 (ch == '=' && next == '=') ||
                 (ch == '!' && next == '=') ||
                 (ch == '<' && (next == '=' || next == '<')) ||
                 (ch == '>' && (next == '=' || next == '>')) ||
                 (ch == '&' && (next == '&' || next == '=')) ||
                 (ch == '|' && (next == '|' || next == '=')) ||
                 (ch == '*' && next == '=') ||
                 (ch == '/' && next == '=') ||
                 (ch == '%' && next == '=')))
            {
                token.word[index++] = (char)next;
                token.word[index] = '\0';
            }
            else if (next != EOF) ungetc(next, file);
            strcpy(token.type, "OPERATOR");
        }
        else if (strchr("(){}[];,:.#?", ch) != NULL)
            strcpy(token.type, "SPECIAL SYMBOL");
        else
            strcpy(token.type, "UNKNOWN");
        print_token(token);
    }
}
