#include <stdio.h>
#include <gtest/gtest.h>
#include <pointer_store.h>

TEST(pointer_store, store_one){
    //Create a handle
    STORE_HANDLE* store = create();

    int inputValue = 401;

    //Store an object
    int pushResult = push(store, &inputValue);

    //remove it
    int* outputValue = (int*)pop(store);

    //Check that true's being returned when pushing
    EXPECT_EQ(pushResult, 1);

    //check it's value
    EXPECT_EQ(&inputValue, outputValue);

    cleanup(store, NULL);
}

TEST(pointer_store, store_many){
    STORE_HANDLE* store = create();

    int value = 401;

    for(int i = 0; i < 10; ++i){
        push(store, &value);
    }
    
    for(int i = 0; i < 10; ++i){
        int* poppedValue = (int*)pop(store);
        EXPECT_EQ(&value, poppedValue);
    }

    cleanup(store, NULL);
}

TEST(pointer_store, alternate_pushing_popping_evenly){
    STORE_HANDLE* store = create();

    for(int cycleIndex = 0; cycleIndex < 4; ++cycleIndex){
        //Storage
        for(int i = 0; i < 4; ++i){
            void* value = malloc(sizeof(int));
            *((int*)value) = i;
            push(store, value);
        }

        //Retrieval
        for(int i = 0; i < 4; ++i){
            void* retrieved = pop(store);
            //Check that the *retrieved = 3 - i;
            int expectedValue = 3 - i;
            int actualValue = *((int*)retrieved);

            if(retrieved){
                free(retrieved);
            }

            EXPECT_EQ(expectedValue, actualValue);
        }
    }

    cleanup(store, NULL);
}

TEST(pointer_store, alternate_pushing_popping_oddly){
    STORE_HANDLE* store = create(); 
    //Write this test, using the above code.

    for(int cycleIndex = 0; cycleIndex < 4; ++cycleIndex){

        int maxCycles = 5;
        //Storage
        for(int i = 0; i < maxCycles; ++i){
            void* value = malloc(sizeof(int));
            *((int*)value) = i;
            push(store, value);
        }

        //Retrieval
        //Run for 1 extra cycle per run.
        for(int i = 0; i <= maxCycles; ++i){
            void* retrieved = pop(store);

            if(i != maxCycles){
                //Check that the *retrieved = 3 - i;
                int expectedValue = maxCycles - 1 - i;
                int actualValue = *((int*)retrieved);

                if(retrieved){
                    free(retrieved);
                }

                EXPECT_EQ(expectedValue, actualValue);
            }
            else{
                void* expected = 0;
                EXPECT_EQ(retrieved, expected);
            }
        }
    }

    cleanup(store, NULL);
}

TEST(pointer_store, empty_list_popping){
    STORE_HANDLE* store = create();

    void* actualPop = pop(store);
    void* expectedPop = NULL;

    EXPECT_EQ(actualPop, expectedPop);

    cleanup(store, NULL);
}

//Testing the length function

TEST(pointer_store, length){
    STORE_HANDLE* store = create();
    int maxLength = 100;

    //Test growth
    for(int i = 0; i < maxLength; ++i){
        int currentLength = length(store);
        int expectedLength = i;
        void* currentValue = malloc(sizeof(int));

        EXPECT_EQ(currentLength, expectedLength);

        push(store, currentValue);
    }

    //Test reduction
    for(int i = 0; i < maxLength; ++i){
        int expectedLength = maxLength - i;
        int actualLength = length(store);

        EXPECT_EQ(expectedLength, actualLength);

        void* poppedValue = pop(store);
        if(poppedValue){
            free(poppedValue);
        }
    }

    //The list, theoretically, is now empty.
    void* poppedValue = pop(store);
    void* nullPointer = 0;
    EXPECT_EQ(poppedValue, nullPointer);
    
    int actualLength = length(store);
    EXPECT_EQ(actualLength, 0);

    cleanup(store, &free);
}

//Next Stow and skim
//Make sure we can run things without segfaults
TEST(pointer_store, stow_one){
    STORE_HANDLE* store = create();
    int value = 1;
    int stowResult = stow(store, &value);

    //Make sure it's telling us that it's being done.
    EXPECT_EQ(stowResult, 1);

    cleanup(store, NULL);
}

TEST(pointer_store, stow_many){
    STORE_HANDLE* store = create();

    for(int i = 0; i < 100; ++i){
        int currentLength = length(store);
        EXPECT_EQ(currentLength, i);
        
        void* valuePointer = malloc(sizeof(int));
        stow(store, valuePointer);
    }

    cleanup(store, &free);
}

TEST(pointer_store, stow_one_skim_one){
    STORE_HANDLE* store = create();
    int value = 1;
    stow(store, &value);

    int* skimmedPointer = (int*)(skim(store));

    EXPECT_EQ(skimmedPointer, &value);
    
    cleanup(store, NULL);
}

//Stowing and skimming whilst testing length.
TEST(pointer_store, stowing_and_skimming){
    STORE_HANDLE* store = create();
    int maxCycles = 5;

    for(int i = 0; i < maxCycles; ++i){
        int actualLength = length(store);
        int expectedLength = i;
        EXPECT_EQ(actualLength, expectedLength);

        int* valuePointer = (int*)(malloc(sizeof(int)));
        *valuePointer = i;
        //Check that it's actually telling you that it's storing it.
        int stowResult = stow(store, valuePointer);
        EXPECT_EQ(stowResult, 1);
    }

    for(int i = 0; i < maxCycles; ++i){
        int* valuePointer = (int*)(skim(store));
        void* nullPointer = 0;

        int expectedValue = maxCycles - i - 1;
        EXPECT_NE(valuePointer, nullPointer);
        EXPECT_EQ(*valuePointer, expectedValue);
        
        //These two should be identical at this point.
        int actualLength = length(store);
        EXPECT_EQ(expectedValue, actualLength);

        if(valuePointer){
            free(valuePointer);
        }
    }

    //Make sure that the length isn't effected by calling skim on an empty list.
    void* skimResult = skim(store);
    void* nullPointer = 0;
    int actualLength = length(store);

    EXPECT_EQ(skimResult, nullPointer);
    EXPECT_EQ(actualLength, 0);

    cleanup(store, NULL);
}

TEST(STORE_HANDLE, push_pop_stow_skim){
    STORE_HANDLE* store = create(); 
    for(int itterations = 0; itterations < 5; ++itterations){
        for(int i = 0; i < 4; ++i){
            void* currentValue = malloc(sizeof(int));
            push(store, currentValue);
        }

        for(int i = 0; i < 4; ++i){
            free(pop(store));
        }

        for(int i = 0; i < 4; ++i){
           void* currentValue = malloc(sizeof(int));
           stow(store, currentValue);
        }

        for(int i = 0; i < 4; ++i){
           free(skim(store));
        }

        //Now that we've shaken up the list, to make sure it works.
        //STOW, PUSH 5 values then pop them all off.
        for(int i = 0; i < 5; ++i){
            int* pushValuePointer = (int*)malloc(sizeof(int));
            int* stowValuePointer = (int*)malloc(sizeof(int));

            *pushValuePointer = *stowValuePointer = i;
            push(store, pushValuePointer);
            stow(store, stowValuePointer);
        }

        //The length of the list should now be 10.
        int currentLength = length(store);
        EXPECT_EQ(currentLength, 10);

        for(int i = 0; i < currentLength; ++i){
            int* value = (int*)pop(store);
            int expectedValue = 0;

            //The values should 4,3,2,1,0,0,1,2,3,4
            if(i < 5){
                expectedValue = 4 - i;
            }
            else{
                expectedValue = i - 5;
            }

            EXPECT_EQ(expectedValue, *value);
            free(value);
        }
        //length should now be zero
        currentLength = length(store);
        EXPECT_EQ(currentLength, 0);

        //Then STOW, PUSH 5 more value, then skim them all off.
        for(int i = 0; i < 5; ++i){
            int* pushValuePointer = (int*)malloc(sizeof(int));
            int* stowValuePointer = (int*)malloc(sizeof(int));

            *pushValuePointer = *stowValuePointer = i;
            push(store, pushValuePointer);
            stow(store, stowValuePointer);
        }

        //The length of the list should now be 10.
        currentLength = length(store);
        EXPECT_EQ(currentLength, 10);

        for(int i = 0; i < currentLength; ++i){
            int* value = (int*)skim(store);
            int expectedValue = 0;

            //The values should 4,3,2,1,0,0,1,2,3,4
            if(i < 5){
                expectedValue = 4 - i;
            }
            else{
                expectedValue = i - 5;
            }

            EXPECT_EQ(expectedValue, *value);
            free(value);
        }

        currentLength = length(store);
        EXPECT_EQ(currentLength, 0);
    }

    cleanup(store, &free);
}

void forEachTestFunction(void* valuePointer, void* myValue){
    *((int*)valuePointer) *= 2;
}

TEST(STORE_HANDLE, forEach_without_extra_arg){
    STORE_HANDLE* store = create();

    for(int i = 0; i < 100; ++i){
        int* value = (int*)malloc(sizeof(int));
        *value = i;
        push(store, value);
    }

    //This function doubles the value pointed to by each pointer.
    forEach(store, &forEachTestFunction, NULL);

    for(int i = 0; i < 100; ++i){
        int* valuePointer = (int*)skim(store);
        int value = *valuePointer;

        EXPECT_EQ(value, i * 2);

        free(valuePointer);
    }

    cleanup(store, &free);
}

//Value pointer being the pointer to the value in the original list
//secondList being a seperate list entirely.
void forEachSecondTestFunction(void* valuePointer, void* secondList){
    int value = *((int*)valuePointer);

    //If it's an even number
    if(value % 2 == 0){
        int* copiedValue = (int*)malloc(sizeof(int));
        *copiedValue = value;
        push(((STORE_HANDLE*)secondList), copiedValue);
    }
}

/*
    Create a list, from 0 to 99

    if a value is even, copy it into the second list.
*/
TEST(STORE_HANDLE, forEach_with_extra_args){
    STORE_HANDLE* store = create();

    for(int i = 0; i < 100; ++i){
        int* valuePointer = (int*)malloc(sizeof(int));

        *valuePointer = i;

        push(store, valuePointer);
    }

    STORE_HANDLE* evenStore = create();
    forEach(store, &forEachSecondTestFunction, evenStore);

    //Now, check that evenStore contains all even values from 1 to 99
    int expectedLength = 100 / 2;
    int actualLength = length(evenStore);
    EXPECT_EQ(expectedLength, actualLength);

    for(int i = 0; i < 100; i += 2){
        void* skimmedPointer = skim(evenStore);
        int skimmedValue = *((int*)skimmedPointer);

        EXPECT_EQ(skimmedValue, i);

        free(skimmedPointer);
    }

    cleanup(store, &free);
    cleanup(evenStore, &free);
}

//Now there's indexing.
TEST(STORE_HANDLE, indexing){
    STORE_HANDLE* store = create();

    //Create the large list
    for(int i = 0; i < 100; ++i){
        void* valuePointer = malloc(sizeof(int));
        *((int*)valuePointer) = i;

        push(store, valuePointer);
    }

    //Get the values, making sure that it doesn't alter the list.
    for(int i = 0; i < 100; ++i){
        void* valuePointer = getIndex(store, i);
        int value = *((int*)valuePointer);

        EXPECT_EQ(value, i);
    }

    int expectedLength = 100;
    int actualLength = length(store);
    EXPECT_EQ(expectedLength, actualLength);

    //Check everything is still in the list.
    for(int i = 0; i < 100; ++i){
        void* valuePointer = skim(store);
        int value = *((int*)valuePointer);

        EXPECT_EQ(value, i);
        free(valuePointer);
    }

    cleanup(store, &free);
}

//Check that we can't get anything outside of the list.
/*
    That means:
        Any negatives

        Anything larger than the list length
*/
TEST(STORE_HANDLE, outOfRangeHandling){
    STORE_HANDLE* store = create();

    //Check we can't get anything from the empty list
    void* indexedPointer = getIndex(store, 0);
    void* nullPointer = 0;
    EXPECT_EQ(indexedPointer, nullPointer);

    //Make a list
    for(int i = 0; i < 100; ++i){
        int* valuePointer = (int*)(malloc(sizeof(int)));
        *valuePointer = i;
        push(store, valuePointer);
    }

    //Now, check you can't negative index it.
    indexedPointer = getIndex(store, -1);
    EXPECT_EQ(indexedPointer, nullPointer);

    //Now check you can't go over
    //Baring in mind this is zero indexed.
    indexedPointer = getIndex(store, 100);
    EXPECT_EQ(indexedPointer, nullPointer);

    //Fin
    cleanup(store, &free);
}

/*

    returns 1 if the yourValuePointer SHOULD be inserted BEFORE this value.

    returns 0 if the yourValuePointer SHOULD NOT be insert BEFORE this value.
*/
int myOrderingFunction(void* listValuePointer, void* myValuePointer){
    //They're both ints.
    int listValue = *((int*)listValuePointer);
    int myValue = *((int*)myValuePointer);

    return (myValue < listValue);
}

TEST(STORE_HANDLE, ordered_insertion){
    STORE_HANDLE* store = create();

    //insert a bunch of odd values into a list
    int maxListValue = 10;
    for(int i = 1; i < maxListValue; i += 2){
        int* valuePointer = (int*)malloc(sizeof(int));
        *valuePointer = i;
        push(store, valuePointer);
    }

    //Now, ordered insert a bunch of values.
    for(int i = 0; i < maxListValue; i += 2){
        int* newValuePointer = (int*)malloc(sizeof(int));
        *newValuePointer = i;
        orderedInsert(store, &myOrderingFunction, newValuePointer);
    }

    //Now, we check that it has worked!
    for(int i = 0; i < maxListValue; ++i){
        int* valuePointer = (int*)skim(store);
        void* nullPointer = 0;
        EXPECT_NE(valuePointer, nullPointer);
        if(valuePointer){
            EXPECT_EQ(i, *valuePointer);
            free(valuePointer);
        }
    }

    cleanup(store, NULL);
}

//Ordered insert on an empty list
//Then popping that value
//then doing it again
//then 
TEST(STORE_HANDLE, ordered_insertion_on_empty_list){
    STORE_HANDLE* store = create();

    for(int i = 0; i < 3; ++i){
        int* valuePointer = (int*)malloc(sizeof(int));
        *valuePointer = i;
        orderedInsert(store, &myOrderingFunction, valuePointer);
        //Now pop it:
        int* poppedValuePointer = (int*)pop(store);

        free(valuePointer);

        EXPECT_EQ(poppedValuePointer, valuePointer);
    }

    //Now do the same with skim
    for(int i = 0; i < 3; ++i){
        int* valuePointer = (int*)malloc(sizeof(int));
        *valuePointer = i;
        orderedInsert(store, &myOrderingFunction, valuePointer);
        //Now pop it:
        int* poppedValuePointer = (int*)skim(store);

        free(valuePointer);

        EXPECT_EQ(poppedValuePointer, valuePointer);
    }

    cleanup(store, &free);
}


//Ordered insert on a list of 1 or 2
TEST(STORE_HANDLE, ordered_insertion_on_small_lists){
    STORE_HANDLE* store = create();

    for(int maxLength = 1; maxLength <= 3; ++maxLength){
        for(int i = 0; i < maxLength; ++i){
            int* valuePointer = (int*)malloc(sizeof(int));
            *valuePointer = i;
            orderedInsert(store, &myOrderingFunction,valuePointer);
        }

        //Now remove everything, checking that they're in order.
        for(int i = 0; i < maxLength; ++i){
            int* poppedValuePointer = (int*)skim(store);
            EXPECT_EQ(*poppedValuePointer, i);
            free(poppedValuePointer);
        }
    }

    cleanup(store, &free);
}

//insertBefore testing
//int insertBefore(STORE_HANDLE* store, void* valuePointer, int index);

/*
    Need to write tests for:
        Insertion on an empty list

        insertion to an index not stated

        insert to a list of lenghts:
            1, 2, 3
*/

//Best case scenario test.
TEST(STORE_HANDLE, basic_insertion){
    STORE_HANDLE* store = create();
    
    for(int i = 0; i < 10; ++i){
        int* valuePointer = (int*)malloc(sizeof(int));
        *valuePointer = i;
        push(store, valuePointer);
    }

    //Then insert at index 5.
    int* insertedValuePointer = (int*)malloc(sizeof(int));
    *insertedValuePointer = 100;
    int insertionResult = insertBefore(store, insertedValuePointer, 5);
    int expectedLength = 10 + 1;

    EXPECT_EQ(insertionResult, 1);
    EXPECT_EQ(length(store), expectedLength);

    //Retrieve it
    int* retrievedValuePointer = (int*)getIndex(store, 5);

    EXPECT_EQ(retrievedValuePointer, insertedValuePointer);

    cleanup(store, &free);
}

//Check that it's returning 0 on failure.
TEST(STORE_HANDLE, insert_on_empty_list){
    STORE_HANDLE* store = create();

    int* valuePointer = (int*)malloc(sizeof(int));

    int insertionResult = insertBefore(store, valuePointer, 5);
    EXPECT_EQ(insertionResult, 0);

    //The list should still be empty
    int listLength = length(store);
    EXPECT_EQ(listLength, 0);

    //Now try inserting the same value at index 0
    insertionResult = insertBefore(store, valuePointer, 0);
    EXPECT_EQ(insertionResult, 1);

    //The length should increase
    listLength = length(store);
    EXPECT_EQ(listLength, 1);

    //Cool
    cleanup(store, &free);
}



//Test that it works for 1, 2, 3
//We're essentially pushing a value 
//I don't have a damn clue how this test works.
TEST(STORE_HANDLE, insert_on_growing_list){
    STORE_HANDLE* store = create();

    for(int i = 0; i < 10; ++i){
        int* valuePointer = (int*)malloc(sizeof(int));
        *valuePointer = i;

        //You have to insert at 0, twice.
        int insertionIndex = (length(store)) ? 0 : i - 1;

        int result = insertBefore(store, valuePointer, insertionIndex);

        EXPECT_EQ(result, 1);
    }

    for(int i = 0; i < 10; ++i){
        int* skimmedPointer = (int*)pop(store);
        EXPECT_EQ(*skimmedPointer, i);
        if(skimmedPointer){
            free(skimmedPointer);
        }
    }

    cleanup(store, &free);
}

//Remove at index testing
//void* removeAtIndex(STORE_HANDLE* store, int index);
//Try removing from a normal list
TEST(STORE_HANDLE, remove_from_best_case){
    //Create the list, storing it's length
    STORE_HANDLE* store = create();
    for(int i = 0; i < 10; ++i){
        int* valuePointer = (int*)malloc(sizeof(int));
        *valuePointer = i;
        push(store, valuePointer);
    }
    int listLength = length(store);

    //Remove a value.
    int* removedPointer = (int*)removeAtIndex(store, 5);
    EXPECT_EQ(*removedPointer, 5);
    free(removedPointer);

    //Check the list lengths
    int newListLength = length(store);
    EXPECT_EQ(listLength - 1, newListLength);

    // Clean up
    cleanup(store, &free);
}

TEST(STORE_HANDLE, remove_from_list_middle){    
    STORE_HANDLE* store = create();

    for(int i = 0; i < 20; ++i){
        int* valuePointer = (int*)malloc(sizeof(int));
        *valuePointer = i;
        push(store, valuePointer);
    }
    //The list should be [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19];

    //WE want to remove the middle of the list.
    //Remove index 5, 10 times.
    //We're expecting the value to be 5, 6, 7, 8, 9... 5 + i
    int targetIndex = 5;
    for(int i = 0; i < 3; ++i){
        int* removedPointer = (int*)removeAtIndex(store, targetIndex);
        int expectedValue = targetIndex + i;
        EXPECT_EQ(expectedValue, *removedPointer);
        free(removedPointer);
    }

    //Then clean up
    cleanup(store, &free);
}
//=MEMORY LEAKS END==========================================================

//Try removing from an empty list
//Try removing from an index that doesn't exist
TEST(STORE_HANDLE, remove_from_non_existant_index){
    STORE_HANDLE* store = create();

    void* removedPointer = removeAtIndex(store, 0);
    void* nullPointer = 0;

    EXPECT_EQ(removedPointer, nullPointer);

    //Now make a list and try again
    for(int i = 0; i < 10; ++i){
        int* valuePointer = (int*)malloc(sizeof(int));
        *valuePointer = i;
        push(store, valuePointer);
    }

    //Now try and remove from 10.
    removedPointer = removeAtIndex(store, 10);
    EXPECT_EQ(removedPointer, nullPointer);

    cleanup(store, &free);
}

//Try removing the last 3 values from a list
//      Then push new values in
//      Then remove them again.
TEST(STORE_HANDLE, remove_at_index_general){
    STORE_HANDLE* store = create();

    int maxLength = 10;
    void* nullPointer = 0;

    for(int i = 0; i <= maxLength; ++i){
        //Make the list maxLength in length
        for(int j = 0; j < i; ++j){
            int* valuePointer = (int*)malloc(sizeof(int));
            *valuePointer = j;
            push(store, valuePointer);
        }

        int listLength = length(store);
        EXPECT_EQ(i, listLength);

        //Now, remove all the items from the list.
        for(int j = 0; j < i; ++j){
            int lastIndex = length(store) - 1;
            int* valuePointer = (int*)removeAtIndex(store, lastIndex);
            EXPECT_NE(valuePointer, nullPointer);
            if(valuePointer){
                free(valuePointer);
            }
        }
    }
    cleanup(store, &free);
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    int returnCode = RUN_ALL_TESTS();
    return returnCode;
}
