ifndef TOP
TOP = ..
endif

VPATH += $(TOP)/src
VPATH += $(TOP)/include/tinyalsa

CXXFLAGS = -Wall -Wextra -Werror -Wfatal-errors
CXXFLAGS += -std=c++11
CXXFLAGS += -I $(TOP)/include

.PHONY: all
all: input

input: input.o -ltinyalsa-cxx

input.o: input.cpp

.PHONY: clean
clean:
	$(RM) input input.o

