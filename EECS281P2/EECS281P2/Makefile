# enables c++11 on CAEN
PATH := /usr/um/gcc-4.7.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-4.7.0/lib64
LD_RUN_PATH := /usr/um/gcc-4.7.0/lib64

# designate which compiler to use
CXX			= g++
# list of sources used in project
SOURCES 	= project2.cpp
# list of objects used in project
OBJECTS		= $(SOURCES:%.cpp=%.o)
# name of the executable produced by the top level
EXECUTABLE 	= MineEscape
# name of the tar ball created for submission
SUBMIT_FILE = submit.tar.gz

#Default Flags
CXXFLAGS = -std=c++11 -Wall -Werror -Wextra -pedantic 

# make release - will compile "all" with $(CXXFLAGS) and the -O3 flag
#				 also defines NDEBUG so that asserts will not check
release: CXXFLAGS += -O3 -DNDEBUG
release: all

# make debug - will compile "all" with $(CXXFLAGS) and the -g flag
#              also defines DEBUG so that "#ifdef DEBUG /*...*/ #endif" works
debug: CXXFLAGS += -g3 -DDEBUG
debug: clean all

# make profile - will compile "all" with $(CXXFLAGS) and the -pg flag
profile: CXXFLAGS += -pg
profile: clean all

test1: debug
	./$(EXECUTABLE) -c POOR_MAN test-1-POOR_MAN.txt

test2: debug
	./$(EXECUTABLE) -c SORTED test—2-SORTED.txt

test3: debug
	./$(EXECUTABLE) -c POOR_MAN test-3-POOR_MAN.txt

test4: debug
	./$(EXECUTABLE) -c SORTED test—4-SORTED.txt

test5: debug
	./$(EXECUTABLE) -c BINARY test—1-POOR_MAN.txt

# highest target; sews together all objects into executable
all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXECUTABLE)

# individual dependencies for objects
# YOU NEED TO ADD YOUR OWN DEPENDENCIES HERE!!!
project2.o: project2.cpp project2.h

# rule for creating objects
%.o:
	$(CXX) $(CXXFLAGS) -c $*.cpp

# make clean - remove .o files, executable, tarball
clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(SUBMIT_FILE)

# make submit.tar.gz - cleans, runs dos2unix, creates tarball
MY_FILES=$(wildcard Makefile *.h *.cpp test*.txt)
$(SUBMIT_FILE): $(MY_FILES)
	dos2unix $(MY_FILES)
	tar -vczf $(SUBMIT_FILE) $(MY_FILES)

# shortcut for make submit.tar.gz (make submit)
submit: submit.tar.gz

# these targets do not create any files
.PHONY: all release debug profile clean submit
# disable built-in rules
.SUFFIXES:
