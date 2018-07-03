#include <pointer_store.h>
#include <stdlib.h>

//Just initalise a node
STORE_HANDLE* create(){
    STORE_HANDLE* userHandle = (STORE_HANDLE*)malloc(sizeof(STORE_HANDLE));
    userHandle->head = 0;
    userHandle->tail = 0;
    userHandle->length = 0;
    return userHandle;
}

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
        //Why am i using a forloop here? Why?
        for(tailNode = &(userHandle->tail); *tailNode; tailNode = &((*tailNode)->next));
        
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

void forEach(STORE_HANDLE* store, void(*myFunction)(void*, void*), void* yourArgument){
    if(myFunction && store->head){
        for(node* walker = store->head; walker; walker = walker->next){
            myFunction(walker->value, yourArgument);
        }
    }
}

/*
    You can't negative index the list.
    It's final index is also store->length - 1;
*/
void* getIndex(STORE_HANDLE* store, int index){
    void* valuePointer = 0;

    if( (index >= 0) && (index < store->length)){
        node* walker = store->head;

        for(int indexTracker = 0; indexTracker != index; ++indexTracker){
            walker = walker->next;
        }

        valuePointer = walker->value;
    }
    
    return valuePointer;
}

/*
    Go through the list

    apply ordering function to everything
        int your_ordering_function(void* currentListValuePointer, void* yourValuePointer);

        returns 1 if the yourValuePointer SHOULD be inserted BEFORE this value.

        returns 0 if the yourValuePointer SHOULD NOT be insert BEFORE this value.
*/
int orderedInsert(STORE_HANDLE* store,  int(*orderingFunction)(void*, void*), void* yourValuePointer){
    int wasSuccessful = 0;

    if(store->head){
        for(node** walker = &(store->head); (*walker); walker = &((*walker)->next)){
            //We're walking
            int shouldValueBeInsertedNow = orderingFunction((*walker)->value, yourValuePointer);

            if(shouldValueBeInsertedNow){
                node* insertionNode = newNode();
                if(insertionNode){
                    insertionNode->value = yourValuePointer;
                    insertionNode->next = (*walker);
                    (*walker) = insertionNode;
                    wasSuccessful = 1;
                    ++(store->length);
                }

                break;
            }
        }   
    }

    //If we haven't inserted it yet, it goes on the end.
    if(!wasSuccessful){
        wasSuccessful = push(store, yourValuePointer);
    }

    return wasSuccessful;
}


int insertBefore(STORE_HANDLE* store, void* yourValuePointer, int index){
    int wasSuccessful = 0;
    int maxIndex = (length(store)) ? length(store) - 1 : 0;

    if(
        maxIndex >= index
    ){
        //If we're inserting on an empty list.
        if(!(store->head)){
            wasSuccessful = push(store, yourValuePointer);
        }
        else{
            int walkerIndex = 0;
            for(node** walker = &(store->head); (*walker); walker = &((*walker)->next)){
                
                if(walkerIndex == index){
                    node* insertionNode = newNode();
                    if(insertionNode){
                        insertionNode->value = yourValuePointer;
                        insertionNode->next = (*walker);
                        (*walker) = insertionNode;
                        wasSuccessful = 1;
                        ++(store->length);
                    }
                    break;
                }
                else{
                    ++walkerIndex;
                }
            } 
        }  
    }

    return wasSuccessful;
}

//Remove the pointer at the index, then return it.
//Else, return 0;
void* removeAtIndex(STORE_HANDLE* store, int index){
    void* removedPointer = 0;
    int storeLength = length(store);
    int maxIndex = storeLength - 1;

    if(storeLength && (maxIndex >= index)){
        //If it's 0, we're just skimming
        if(!index){
            removedPointer = skim(store);
        }
        //If it's the end, it's just popping
        else if(index == maxIndex){
            removedPointer = pop(store);
        }
        else{
            //This is the index of the next value in the list, the one after walker.
            int nextIndex = 1;
            for(node* walker = store->head; walker; walker = walker->next){
                if(nextIndex == index){
                    //Then we remove the next node.
                    node* temp = walker->next;
                    walker->next = walker->next->next;
                    removedPointer = temp->value;
                    free(temp);
                    --(store->length);
                    break;
                }
                else{
                    ++nextIndex;
                }
            }

        }
    }

    return removedPointer;
}