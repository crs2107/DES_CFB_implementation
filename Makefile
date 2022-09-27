SRC = $(shell pwd)
#_parentDIR := $(SRC)/..
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g -lpthread

#LIB_TYPE = {static,shared}
LIB_TYPE = $(LIB)

ifeq ($(LIB_TYPE),shared)
   LIB_EXT = so
else
   LIB_EXT = a
endif

OBJECTS = $(addsuffix .o, $(basename $(wildcard *.cpp)))
HEADERS = $(wildcard *.h)

ifeq ($(shell uname),Linux)
	OS = linux
endif
ifeq ($(shell uname | head -c 6),CYGWIN)
	OS = Cygwin
endif

TARGET := CFB_application-$(OS)

default : target

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -I. $<

$(OBJECTS) : $(HEADERS) 

target: $(OBJECTS)
	#creates executable. Using the following command
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(EXT_OBJECTS) 

clean:
	rm -rf $(TARGET) $(OBJECTS)

