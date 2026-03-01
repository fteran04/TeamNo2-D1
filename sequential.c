#include <stdio.h>

int dotProduct(int a[], int b[], int n){
    int ans = 0;
    int i;
    for(i = 0; i < n; i++){
        ans += a[i] * b[i];
    }
    return ans;
}

void printVec(int arr[], int n){
    printf("< ");
    for (int i = 0; i < n; i++){
        printf("%d, ", arr[i]);
    }
    printf(">");
    printf("\n");
}

int main(){
    int a[] = {4, 7, 2, 1, 9, 12, 5, 6};
    int b[] = {10, 5, 9, 4, 2, 7, 2, 8};
    int n = 8;

    printf("a = ");
    printVec(a, n);

    printf("b = ");
    printVec(b, n);

    printf("\n");

    int a_dot_b = dotProduct(a, b, n);

    printf("a • b = %d\n", a_dot_b);

    return 0;
}