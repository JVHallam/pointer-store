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
These are ultimately the last 2 functions that i would need to deem this lib complete.
Anything else is just quality of life. Anything else can easily be done.
    Ordered Insert:
        I'm going to want to insert values, into locations, based on their values.

        If i have a list:
            1, 2, 4, 6, 8, 9, 10;

        I want it to insert 3, before 4 and after 2.

        We'll probably need something that takes a comparison function:

        orderedInsert(list, value, comparisonFunction);

        bool shouldBeInsertedBefore(void* listValue, void* yourValue){
            if the list Value is larger (or should come later) than your value,
            return true.

            if the list value is smaller, return false.
        }

        This is an essential function to implement. It also allows for insertion sorting by the user.
        A combination between this and forEach means that we can implement an insertion sort quite easily.

    Insert at index
        This can be added without much hassle

    Remove from index
        Remove a value from an index, shuffle the list to fit.

Then there's the world of splitting and joining:
    Concat(leftList, rightList);
        An explicit way of doing this will be nice. 
        Although it would just be a case of:
            push(a, Skim(b))

    Other functions:
        Due to the fact that forEach is pass by reference and 
        "Insert at Index" and "Remove from Index" will also exist,
        The rest of these functions may actually be uneeded in the implementation
        as of right now.

        They'd just be quality of life functions, rather than essentials.


        slicing:
            This would be a case of splitting the list into 2.
            The original list, minus the sliced section.
            It wouldn't be intuitive, so i might give this one a pass.

        filter
            Are we copying the filtered result into another list
            (This can be done with forEach, more or less)

        reduce
            Are we reducing and discarding the list?
            (forEach)

        CastToArray
            This is going to be awkard, imho.
            It will create a void* array, or a void** array.
            Making this function alot less useful.

            And with the fact it's storing void*, if you forget
            the size of the variables or the vars are mixed, it can lead
            to some serious issues.

        Map / Transform
            Are we mapping onto ourselves, or another list?
            If we have forEach, we kinda have this already.
            Which bumps it way down.

    Functions that might be nice later:
        Copy value at index
            This can be done manually. Doesn't really need a function.

        slicing
            You'll have your own slicing functions at this point.
            An implementation of this as a single function would be more efficient then the 
            "Insert at" and "Remove from" functions, but i don't know how i want that implemented yet.
*/

#endif