#include <stdio.h>
#include <stdlib.h>
#include <pointer_store.h>

void myFunction(void* valuePointer){
    int value = *((int*)valuePointer);

    printf("Value : %i\n", value);
}

int main(void){
    STORE_HANDLE* handle = create();

    for(int i = 0; i < 10; ++i){
        int* valuePointer = (int*)malloc(sizeof(int));
        *valuePointer = i;
        push(handle, valuePointer);
    }

    forEach(handle, &myFunction);

    return 0;
}