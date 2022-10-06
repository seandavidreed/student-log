#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "in_out.h"

// My own function idea to replace fgets():
// I don't want that pesky newline character in the string!
int get_text(char *str, int size) {
    int i = -1;
    while ((str[++i] = getc(stdin)) != '\n') {
        if (i == size) break;
    }
    str[i] = '\0';
    return i;
}

// Similar to get_text() but with the added benefit
// of supporting any size null-terminated string
int get_var_text(char **str) {
    int qty = 100;
    *str = (char *)malloc(qty * sizeof(char));
    char c;
    int i = 0;
    while ((c = getc(stdin)) != '\n') {
        (*str)[i++] = c;
        if (i == qty) {
            qty += 50;
            *str = (char *)realloc(*str, qty);
        }
    }
    (*str)[i] = '\0';
    return i;
}

// Use this after taking char input with scanf()
void clear_stdin() {
    while (1) {
        int c = getc(stdin);
        if (c == EOF || c == '\n') break;
    }
}

void text_wrap(char *str, int wrap) {
    int i = -1, limit = 0;
	while (str[++i]) {
        limit = (i % wrap == 0 && i != 0) ? 1 : limit;
        if (limit == 1 && str[i] == ' ') {
            str[i] = '\n';
            limit = 0;
        }
    }
}


int str_to_int(char *number) {
    int i = -1;
    int buffer[10];
    int result = 0;
    while (number[++i] != '\0') {
        buffer[i] = (int)(number[i] - '0'); //haven't tested this change yet (int)
    }
    for (int j = 0, k = i; j < k; j++) {
        result += buffer[j] * pow(10, --i);
    }
    return result;
}
