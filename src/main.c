#include <stdio.h>
#include <stdlib.h>
#include <pointer_store.h>
#include <string.h>

void combineStrings(void* firstValue, void* myValue){
    
}

int main(void){
    STORE_HANDLE* handle = create();

    push(handle, "This");
    push(handle, "Is");
    push(handle, "Not");
    push(handle, "My");
    push(handle, "Idea");
    push(handle, "Of");
    push(handle, "A");
    push(handle, "Good");
    push(handle, "Time");

    char* combinedString  = 0;

    forEach(handle, combineStrings, combinedString);

    return 0;
}
