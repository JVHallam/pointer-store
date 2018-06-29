#ifndef POINTER_STORE_H_
#define POINTER_STORE_H_

/*
    What this module should be able to do.
        STORE_HANDLE* my_handle = create_handle();

        void push(my_handle, void* value);

        void* pop(my_handle);


*/

typedef struct{
    //My handle for future use.
}pointer_store;

#define STORE_HANDLE pointer_store

void push(STORE_HANDLE, void*);

void* pop(STORE_HANDLE);

/*
    when cleanup is called, it will have the function cleanup the structure.
    It will not, however, clean up your stored pointers.
    This is where your_cleanup_function will come in. All pointers, in the structure
    will be passed to your_cleanup_function. There, you will be expected to call any cleanup function you wish.
    If you're just allocating things from the freestore, you can just pass in free, i will understand and
    respect your choice.
*/
void cleanup(STORE_HANDLE, void(*your_cleanup_function)(void*));

#endif