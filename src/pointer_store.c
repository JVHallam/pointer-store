#include <pointer_store.h>
#include <stdlib.h>

//Remember to remove this.
#include <stdio.h>

STORE_HANDLE* create(){
    STORE_HANDLE* userHandle = (STORE_HANDLE*)malloc(sizeof(STORE_HANDLE));

    userHandle->head = 0;
    userHandle->tail = 0;
    userHandle->length = 0;

    return userHandle;
}

//ps_node
void cleanup(STORE_HANDLE* userHandle, void(*your_cleanup_function)(void*)){
    //Go through the list.
    for(node* walker = userHandle->head; walker;){
        if(your_cleanup_function){
            your_cleanup_function(walker->value);
        }
        //move walker along
        node* temp = walker;
        walker = walker->next;
        free(temp);
    }

    free(userHandle);
}

int push(STORE_HANDLE* userHandle, void* newValue){
    node* nextNode = newNode();
    int wasSuccessful = 0;

    if(nextNode){
        nextNode->value = newValue;

        //Go to the tail.
        node** tailNode = 0;

        //God damn i love me a bodyless for loop.
        for(tailNode = &userHandle->tail; *tailNode; tailNode = &((*tailNode)->next));

        //Double check that this works.
        (*tailNode) = nextNode;

        if(!(userHandle->head)){
            userHandle->head = userHandle->tail;
        }

        wasSuccessful = 1;
        ++(userHandle->length);
    }

    return wasSuccessful;
}

void* pop(STORE_HANDLE* userHandle){
    node** holder = &(userHandle->head);
    void* returnValue = 0;

    //We want to walk the list until we hit the end
    for(node** walker = holder; (*walker); walker = &((*walker)->next)){
        holder = walker;
    }

    //Now, holder is pointing to the next to last node
    //Walker pointing at the last connector of the last node.
    if((*holder)){
        returnValue = (*holder)->value;
        free(*holder);
        *holder = 0;
        --(userHandle->length);
    }
    
    userHandle->tail = (node*)holder;
    return returnValue;
}

//Allocates memory on the heap.
node* newNode(){
    node* userNode = (node*)malloc(sizeof(node));
    //initialise it
    userNode->next = 0;
    userNode->value = 0;

    return userNode;
}

int length(STORE_HANDLE* store){
    return store->length;
}

int stow(STORE_HANDLE* store, void* valuePointer){
    int wasSuccessful = 0;
    node* nextNode = newNode();
    if(nextNode){
        nextNode->value = valuePointer;
        nextNode->next = store->head;
        store->head = nextNode;

        if(!(store->tail)){
            store->tail = store->head;
        }
        wasSuccessful = 1;
        ++(store->length);
    }

    return wasSuccessful;
}

void* skim(STORE_HANDLE* store){
    //We need to make the head point 1 past the current head
    void* valuePointer = 0;
    
    if(store->head){
        node* oldHead = store->head;
        store->head = store->head->next;

        valuePointer = oldHead->value;
        free(oldHead);
        --(store->length);
    }

    //If after the reduction, the head is now null.
    if(!(store->head)){
        store->tail = NULL;
    }
    
    return valuePointer;
}