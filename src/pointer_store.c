#include <pointer_store.h>
#include <stdlib.h>

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
        your_cleanup_function(walker->value);
        //move walker along
        node* temp = walker;
        walker = walker->next;
        free(temp);
    }

    free(userHandle);
}

void push(STORE_HANDLE* userHandle, void* newValue){
    node* nextNode = newNode();
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