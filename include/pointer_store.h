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
//return 1 if successful, else return 0.
int push(STORE_HANDLE* store, void* valuePointer);

//Remove the last element in the store.
void* pop(STORE_HANDLE* store);

//Insert as the first item in the store
//return 1 if successful, else return 0.
int stow(STORE_HANDLE* store, void* valuePointer);

//Remove the first item from the store
void* skim(STORE_HANDLE* store);

//=============================================================================================
//Functions for getting descriptions of the handle.
int length(STORE_HANDLE* store);

//=============================================================================================
//Functions that manipulate the list:

//Get the value at an index.
void* getIndex(STORE_HANDLE* store, int wantedIndex);

//Takes a pointer to your store and a function that looks like:
//void yourFunction(void* listItem);
//Your function will be called with every item in the list.
//your Value pointer is a pointer to a value that you'll want passed into the yourFunction
//Alongside the values from the list.
//For example, if you wanted to take values from one list and put them in another
//You'd need to pass that list in as an arg to forEach.
void forEach(STORE_HANDLE* store, void(*yourFunction)(void*, void*), void* yourValuePointer);

/*
    Returns 1 if successful, 0 otherwise.
    parameters:
        You Store handle
    
    ordering function
        int your_ordering_function(void* currentListValuePointer, void* yourValuePointer);

        returns 1 if the yourValuePointer SHOULD be inserted BEFORE this value.

        returns 0 if the yourValuePointer SHOULD NOT be insert BEFORE this value.

    yourValuePointer:
        The value that you're inserting.
*/
int orderedInsert(STORE_HANDLE* store,  int(*orderingFunction)(void*, void*), void* yourValuePointer);

/*
    Insert a value BEFORE and index so it becomes the value at that index
    [0, 1, 2, 3, 4, 5, 6]

    If you insert the value 10 before index = 4;

    [0, 1, 2, 3, 10, 4, 5, 6]

    You're inserting a value before an index, so it becomes that index.

    You CANNOT use this function to push a value onto the end of a list.
*/
int insertBefore(STORE_HANDLE* store, void* valuePointer, int index);

/*
    Remove a value from the index specified.
    If it's successful, return the value pointer, else return null.
*/
void* removeAtIndex(STORE_HANDLE* store, int index);

#endif