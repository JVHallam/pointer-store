#include <stdio.h>
#include <gtest/gtest.h>
#include <pointer_store.h>

TEST(pointer_store, store_one){
    //Create a handle
    STORE_HANDLE* store = create();

    int a = 401;

    //Store an object
    push(store, &a);

    //remove it
    int* b = (int*)pop(store);

    //check it's value
    EXPECT_EQ(&a, b);

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
    EXPECT_EQ(1,2);

    //Write this test, using the above code.
}

TEST(pointer_store, empty_list_popping){
    STORE_HANDLE* store = create();

    void* actualPop = pop(store);
    void* expectedPop = NULL;

    EXPECT_EQ(actualPop, expectedPop);

    cleanup(store, NULL);
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);
    int returnCode = RUN_ALL_TESTS();
    return returnCode;
}
