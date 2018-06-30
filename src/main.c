#include <stdio.h>
#include <stdlib.h>
#include <pointer_store.h>

void copyOverEvens(void* valuePointer, void* secondListPointer){
    int value = *((int*)valuePointer);

    if(value % 2 == 0){
        int* copiedPointer = malloc(sizeof(int));
        *copiedPointer = value;
        push((STORE_HANDLE*) secondListPointer, copiedPointer);
    }
}

void printValue(void* valuePointer, void* notNeeded){
    int value = *((int*)valuePointer);

    printf("Value : %i\n", value);
}

int main(void){
    STORE_HANDLE* handle = create();

    for(int i = 0; i < 20; ++i){
        int* valuePointer = malloc(sizeof(int));
        *valuePointer = i;
        push(handle, valuePointer);
    }

    STORE_HANDLE* secondHandle = create();

    forEach(handle, &copyOverEvens, secondHandle);
    forEach(secondHandle, &printValue, NULL);

    cleanup(handle, &free);
    cleanup(secondHandle, &free);

    return 0;
}