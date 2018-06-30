#ifndef IMPLEMENTATION_H_
#define IMPLEMENTATION_H_

typedef struct node_store{
    node_store* next;
    void* value;
}node;

//The struct to hold everything. IDK, what it'll look like yet.
//I just know it'll be nice.

typedef struct{
    //My handle for future use.
    node* head;
    node* tail;
    int length;
}pointer_store;

//Initialise a new node
//Allocates memory on the heap.
node* newNode();

#endif