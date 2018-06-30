#include <stdio.h>
#include <stdlib.h>
#include <pointer_store.h>

int main(void){
    STORE_HANDLE* handle = create();

    int MaxCycles = 5;

    for(int i = 0; i < MaxCycles; ++i){
        int* value = (int*)malloc(sizeof(int));
        (*value) = i;
        push(handle, value);
    }

    for(int i = MaxCycles - 1; i; --i){
        int* value = (int*)pop(handle);
        printf("Value : %i\n", *value);
    }

    return 0;
}