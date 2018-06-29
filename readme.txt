Description:
    The idea of this project is just to have somewhere that i can easily store pointers.

    You will have the pointer, already made, to push onto the object. It will NOT handle
    allocation of memory for variables that don't pertain to it's size.
    It will only handle itself.

    To elaborate, you will have to create the pointers. You will pass that pointer to the store.
    You can then get it back, then do whatever.
    If the pointer is pointing to something allocated on the heap, you will have to clean it up.
    The reason being, is i don't want to have to worry about discriminating between the heap and the stack, just in case.

    I will, however, want a clean up function.
    It will destroy the list and it's nodes, regardless of it's size. 
    It will take a function pointer, to a function that looks likes so:

        void your_cleanup_function(void* your_stored_variable);

    that way, the list can clean itself up and pass the pointers to your function that will clean up the variables.

    It's meant to be a relatively simple project, upon which i will build the more complex and abstract things.
   
The objectives of this project:
    To have a dynamically sized data structure

    To be written in pure C

    Something that can be used on an embedded system, with minimal hassle.

    It must, therefore, be pretty compact when compiled.

    It must have 0 outside dependencies.

    It must also be easy to move.

    It must be conservative with regards to RAM

    If it can, it must then be optimised to conserve CPU

Focuses in order of priority:
    Simple to use

    Portable

    Small when compiled.

    To be as conservative of memory as possible.

How we'll achieve this: 
    In the first iteration the structure will be developed using singularly linked lists.
    The reason being, they're super simple to program. And are relatively linear in everything they do.
    It's a safe data structure initially.

    On the later iterations, we'll try and incorporate something that mimics the C++ std library vector.

    There will be a focus on unit tests.

Testing:
    This project depends on the Google Test framework.
    To use the tests, it must be downloaded and the shared libs compiled.

Usages and paradigms:
    The idea will be to go with the opaque object style of doing things.
    E.g. You'll be given a handle to an object, and be expected to use functions to get values from it.

    Although, this project isn't going to be as complex as CURL, so i doubt this will really be a problem.

Other notes:
    As this is running on embedded systems, i SHOULD minimise CPU usage and make this
    as optimal as possible. On the other hand, i just need to pass the IOT module using this code.
    With that in mind, Memory, then CPU time.

Depth:
    This library will only be expected, for the time being, to handle single levels of depth.
    E.g. if you give it a void** it will only expect to have a void* so any functions that 
    deal with duplication or deletion will only act like so.

Goals:
    Top level priorities:
        Have a lib where i can create an object

        push values into the lists

        pop values from the list

        count the values in the list

        Clean up said object

    Second level priorities:
        Functional Programming style management of values:
            map values of the list, with respect to a function, onto another list
            Or back onto the list itself

            filter the list for values, into a new list.or back onto the original list.

            reduce the values in the list, into a single variable

        ToArray:
            It'd be nice to be able to cast the item into an array.