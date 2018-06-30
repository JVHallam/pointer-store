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
void forEach(STORE_HANDLE* store, void(*yourFunction)(void*));

/*
    Functions that are needed in order of priority:
        Get pointer at index

        forEach
            important atm, as indexing isn't as intuitive as when using an array.

    Functions i really want, but aren't super useful atm:

        CastToArray
            Top of the list.

        filter
            Are we copying the filtered result into another list

        reduce
            Are we reducing and discarding the list?

        Map / Transform
            Are we mapping onto ourselves, or another list?
            If we have forEach, we kinda have this already.
            Which bumps it way down.

    Functions that might be nice later:

        Copy value at index

        Insert at index

        slicing
*/

#endif