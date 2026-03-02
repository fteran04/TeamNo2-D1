#include "dot_product.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dotProduct(const int a[], const int b[], int n) {
    int ans = 0;

    for (int i = 0; i < n; i++) {
        ans += a[i] * b[i];
    }

    return ans;
}

int is_power_of_two(int value) {
    if (value <= 0) {
        return 0;
    }
    return (value & (value - 1)) == 0;
}

int parse_int_csv_line(const char *line, int out[], int expected) {
    char *buffer = (char *)malloc(strlen(line) + 1);
    int count = 0;
    char *token;

    if (buffer == NULL) {
        return 0;
    }

    strcpy(buffer, line);
    token = strtok(buffer, ",");

    while (token != NULL && count < expected) {
        while (isspace((unsigned char)*token)) {
            token++;
        }

        out[count] = (int)strtol(token, NULL, 10);
        count++;
        token = strtok(NULL, ",");
    }

    free(buffer);
    return count == expected;
}

int read_vectors_from_stdin(int **a, int **b, int *n) {
    *a = NULL;
    *b = NULL;
    *n = 0;

    if (scanf(" %d", n) != 1) {
        return 0;
    }

    if (*n <= 0) {
        return 0;
    }

    *a = (int *)malloc((size_t)(*n) * sizeof(int));
    *b = (int *)malloc((size_t)(*n) * sizeof(int));

    if (*a == NULL || *b == NULL) {
        free(*a);
        free(*b);
        return 0;
    }

    for (int i = 0; i < *n; i++) {
        if (scanf(" %d", &(*a)[i]) != 1) {
            free(*a);
            free(*b);
            *a = NULL;
            *b = NULL;
            return 0;
        }
        if (i < *n - 1) {
            char sep = '\0';
            if (scanf(" %c", &sep) != 1 || sep != ',') {
                free(*a);
                free(*b);
                *a = NULL;
                *b = NULL;
                return 0;
            }
        }
    }

    for (int i = 0; i < *n; i++) {
        if (scanf(" %d", &(*b)[i]) != 1) {
            free(*a);
            free(*b);
            *a = NULL;
            *b = NULL;
            return 0;
        }
        if (i < *n - 1) {
            char sep = '\0';
            if (scanf(" %c", &sep) != 1 || sep != ',') {
                free(*a);
                free(*b);
                *a = NULL;
                *b = NULL;
                return 0;
            }
        }
    }

    return 1;
}