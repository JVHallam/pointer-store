#Normal Variables.
CC = clang
include = -Iinclude
srcFiles = src/main.c

#Test Variables.
TestCC = clang++
gtest = subs/googletest/googletest
TestInclude = -I$(gtest)/include $(include)
#testSrc = tests/first.cpp
libs = $(gtest)/libgtest.a $(gtest)/libgtest_main.a -pthread

main: $(srcFiles)
	$(CC) $(srcFiles) $(include) -o bin/output;

test: $(testSrc)
	$(TestCC) $(testSrc) -o bin/test $(libs) $(TestInclude);