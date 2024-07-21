#include "wet3.h"

int main()
{
    printf("starting\n");
    int* x = (int*)__malloc(sizeof(int));
    *x = 0;
    printf("%d\n", *x);
    __free(x);
    return 0;
}
