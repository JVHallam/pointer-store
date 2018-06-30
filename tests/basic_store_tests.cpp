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

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    int returnCode = RUN_ALL_TESTS();
    return returnCode;
}
