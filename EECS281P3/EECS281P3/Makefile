# enables c++11 on CAEN
PATH := /usr/um/gcc-4.7.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-4.7.0/lib64
LD_RUN_PATH := /usr/um/gcc-4.7.0/lib64

# designate which compiler to use
CXX			= g++
# list of sources used in project
SOURCES 	= $(wildcard *.cpp)
# list of objects used in project
OBJECTS		= $(SOURCES:%.cpp=%.o)
# name of the executable produced by the top level
EXECUTABLE 	= market
# name of the tar ball created for submission
SUBMIT_FILE = submit.tar.gz

#Default Flags
CXXFLAGS = -std=c++11 -Wall -Werror -Wextra -pedantic 

# make release - will compile "all" with $(CXXFLAGS) and the -O3 flag
#				 also defines NDEBUG so that asserts will not check
release: CXXFLAGS += -O3 -DNDEBUG
release: clean all

# make debug - will compile "all" with $(CXXFLAGS) and the -g flag
#              also defines DEBUG so that "#ifdef DEBUG /*...*/ #endif" works
debug: CXXFLAGS += -g3 -DDEBUG
debug: clean all

# make profile - will compile "all" with $(CXXFLAGS) and the -pg flag
profile: CXXFLAGS += -pg
profile: clean all

# highest target; sews together all objects into executable
all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXECUTABLE)

# individual dependencies for objects
# YOU NEED TO ADD YOUR OWN DEPENDENCIES HERE!!!
# foo.o: foo.cpp bar.h 

# rule for creating objects
%.o:
	$(CXX) $(CXXFLAGS) -c $*.cpp

# make clean - remove .o files, executable, tarball
clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(SUBMIT_FILE) *.o *.output*

tltests: debug
	./$(EXECUTABLE) -g E_A < medium-TL-in.txt > medium-g-E_A-out.outputTL
	diff medium-g-E_A-out.outputTL medium-g-E_A-out.txt
	./$(EXECUTABLE) -i E_A < medium-TL-in.txt > medium-i-E_A-out.outputTL
	diff medium-i-E_A-out.outputTL medium-i-E_A-out.txt
	./$(EXECUTABLE) -m < medium-TL-in.txt > medium-m-out.outputTL
	diff medium-m-out.outputTL medium-m-out.txt
	./$(EXECUTABLE) -m -s -t -v < medium-TL-in.txt > medium-mstv-out.outputTL
	diff medium-mstv-out.outputTL medium-mstv-out.txt
	./$(EXECUTABLE) -s < medium-TL-in.txt > medium-s-out.outputTL
	diff medium-s-out.outputTL medium-s-out.txt
	./$(EXECUTABLE) -t < medium-TL-in.txt > medium-t-out.outputTL
	diff medium-t-out.outputTL medium-t-out.txt
	./$(EXECUTABLE) -v -i E_A -g E_A < medium-TL-in.txt > medium-v-i-E_A-g-E_A-out.outputTL
	diff medium-v-i-E_A-g-E_A-out.outputTL medium-v-i-E_A-g-E_A-out.txt
	./$(EXECUTABLE) -v -i E_A < medium-TL-in.txt > medium-v-i-E_A-out.outputTL
	diff medium-v-i-E_A-out.outputTL medium-v-i-E_A-out.txt
	./$(EXECUTABLE) -v < medium-TL-in.txt > medium-v-out.outputTL
	diff medium-v-out.outputTL medium-v-out.txt

prtests: debug
	./$(EXECUTABLE) -g E_A < medium-PR-in.txt > medium-g-E_A-out.outputPR
	diff medium-g-E_A-out.outputPR medium-g-E_A-out.txt
	./$(EXECUTABLE) -i E_A < medium-PR-in.txt > medium-i-E_A-out.outputPR
	diff medium-i-E_A-out.outputPR medium-i-E_A-out.txt
	./$(EXECUTABLE) -m < medium-PR-in.txt > medium-m-out.outputPR
	diff medium-m-out.outputPR medium-m-out.txt
	./$(EXECUTABLE) -m -s -t -v < medium-PR-in.txt > medium-mstv-out.outputPR
	diff medium-mstv-out.outputPR medium-mstv-out.txt
	./$(EXECUTABLE) -s < medium-PR-in.txt > medium-s-out.outputPR
	diff medium-s-out.outputPR medium-s-out.txt
	./$(EXECUTABLE) -t < medium-PR-in.txt > medium-t-out.outputPR
	diff medium-t-out.outputPR medium-t-out.txt
	./$(EXECUTABLE) -v -i E_A -g E_A < medium-PR-in.txt > medium-v-i-E_A-g-E_A-out.outputPR
	diff medium-v-i-E_A-g-E_A-out.outputPR medium-v-i-E_A-g-E_A-out.txt
	./$(EXECUTABLE) -v -i E_A < medium-PR-in.txt > medium-v-i-E_A-out.outputPR
	diff medium-v-i-E_A-out.outputPR medium-v-i-E_A-out.txt
	./$(EXECUTABLE) -v < medium-PR-in.txt > medium-v-out.outputPR
	diff medium-v-out.outputPR medium-v-out.txt
	./$(EXECUTABLE) -s -t -m -v < prtest1.txt > prtest1.outputPR
	diff prtest1.outputPR prspec-out.txt

gregtests: debug
	./$(EXECUTABLE) -g AAPL < test-13-g-AAPL.txt > gtest13.outputTL

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
