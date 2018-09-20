# Pointer Store

This is the pointer_store. A project Written in C, to be a simple to use, generic, storage system.

It had to do the following (In order of priority):
* Be orderable and allow general insertions at any point
* Be adaptable (e.g. i could change the state of the store whenever i wanted)
* Use as little memory as it could (Within reason)
* Could store any types
* Be usable on embedded systems
* Have no dependencies

The reason we're storing pointers and keep the abstraction relatively low, is so that
the utility is still high, but the memory usage was low.
If i wanted it to be general purpose and be completely abstract (you'd push and pop values, not pointers),
i'd probably have to store general purpose unions, which may turn out to be a huge memory hog.

### Simplicity
One thing i've aimed to do with the naming conventions is keep things simple.

It's gotten to the point where most libraries i use have BIG_STONKIN_LONG_NAMES because of C's lack of namespace usages.
If all libraries are using super long names, there's free real estate in the shorter ones.

# Getting Started

At the minute, one must compile the project into your own. 
* include the "include" directory in your makefile
* compile your binary with pointer_store.c file

### Prerequisites

The main project has no dependencies.

### Testing

To run the C++ unit tests, you must pull the sub modules and build the google_test binaries.
Then, run them.
