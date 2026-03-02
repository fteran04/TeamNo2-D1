#include <stdio.h>
#include <stdlib.h>

int is_power_of_two(int value) {
    if (value <= 0) {
        return 0;
    }
    return (value & (value - 1)) == 0;
}

int main(int argc, char *argv[]) {
    int n = 8;

    if (argc > 1) {
        n = atoi(argv[1]);
    }

    if (!is_power_of_two(n)) {
        fprintf(stderr, "n must be a power of two\n");
        return 1;
    }

    printf("%d\n", n);

    for (int i = 0; i < n; i++) {
        printf("%d", i + 1);
        if (i < n - 1) {
            printf(", ");
        }
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("%d", i + n + 1);
        if (i < n - 1) {
            printf(", ");
        }
    }
    printf("\n");

    return 0;
}