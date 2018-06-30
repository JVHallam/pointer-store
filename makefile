#Normal Variables.
CC = clang
include = -Iinclude
srcFiles = src/main.c
nonMain = src/pointer_store.c
flags = -g

#Test Variables.
TestCC = clang++
gtest = subs/googletest/googletest
TestInclude = -I$(gtest)/include $(include)
testSrc = tests/basic_store_tests.cpp
libs = $(gtest)/libgtest.a $(gtest)/libgtest_main.a -pthread

main: $(srcFiles)
	$(CC) $(srcFiles) $(include) -o bin/output $(nonMain);

test: $(testSrc)
	$(TestCC) $(testSrc) -o bin/test $(libs) $(TestInclude) $(nonMain) $(flags);

clean:
	rm bin/*