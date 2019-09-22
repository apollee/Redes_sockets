#include <stdio.h>

int main(){
    int a = 10;
    int *b = &a;

    printf("a: %d\n", a);
    printf("a_addr: %p\n", &a);
    printf("b: %p\n", b);
    printf("b_value: %d\n", *b);

    printf("AF_INET: %d\n", AF_INET);

    return 0;
}