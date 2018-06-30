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
        }

        currentLength = length(store);
        EXPECT_EQ(currentLength, 0);
    }

    cleanup(store, &free);
}

void forEachTestFunction(void* valuePointer){
    *((int*)valuePointer) *= 2;
}

TEST(STORE_HANDLE, forEach){
    STORE_HANDLE* store = create();

    for(int i = 0; i < 100; ++i){
        int* value = (int*)malloc(sizeof(int));
        *value = i;
        push(store, value);
    }

    //This function doubles the value pointed to by each pointer.
    forEach(store, &forEachTestFunction);

    for(int i = 0; i < 100; ++i){
        int* valuePointer = (int*)skim(store);
        int value = *valuePointer;

        EXPECT_EQ(value, i * 2);

        free(valuePointer);
    }

    cleanup(store, &free);
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

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    int returnCode = RUN_ALL_TESTS();
    return returnCode;
}
