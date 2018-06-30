#ifndef POINTER_STORE_H_
#define POINTER_STORE_H_

#include <implementation.h>

#define STORE_HANDLE pointer_store

/*
    when cleanup is called, it will have the function cleanup the structure.
    It will not, however, clean up your stored pointers.
    This is where your_cleanup_function will come in. All pointers, in the structure
    will be passed to your_cleanup_function. There, you will be expected to call any cleanup function you wish.
    If you're just allocating things from the freestore, you can just pass in free, i will understand and
    respect your choice.

    Kill the handle and all memory associated with it. Don't reuse a handle after cleanup.
*/
void cleanup(STORE_HANDLE* userHandle, void(*yourCleanupFunction)(void*));

//Create a brand new handle.
STORE_HANDLE* create();

//=============================================================================================
//Functions for managing the items in the store.

//Insert an item at the end of the store.
void push(STORE_HANDLE*, void*);

//Remove the last element in the store.
void* pop(STORE_HANDLE*);

//Insert as the first item in the store
void stow(STORE_HANDLE*, void*);

//Remove the first item from the store
void* skim(STORE_HANDLE*);

//=============================================================================================
//Functions for getting descriptions of the handle.
int length(STORE_HANDLE*);

#endif