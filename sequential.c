#include <stdio.h>
#include <stdlib.h>

#include "dot_product.h"

int main() {
    int *a = NULL;
    int *b = NULL;
    int n = 0;

    if (!read_vectors_from_stdin(&a, &b, &n)) {
        fprintf(stderr, "Invalid input format. Expected: n, v1, v2\n");
        return 1;
    }

    if (!is_power_of_two(n)) {
        fprintf(stderr, "Vector length must be a power of two.\n");
        free(a);
        free(b);
        return 1;
    }

    printf("dot_product_sequential = %d\n", dotProduct(a, b, n));

    free(a);
    free(b);

    return 0;
}