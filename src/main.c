#include <stdio.h>
#include <stdlib.h>

void test(void(*v)(void*)){
    //Allocate room for an int on the heap.
    int* variable = (int*)malloc(sizeof(int));
    *variable = 101;
    v(variable);
}

int main(void){
    puts("Hello World");
    test(&free);
    return 0;
}