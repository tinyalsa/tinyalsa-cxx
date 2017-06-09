ifndef TOP
TOP = ..
endif

CXXFLAGS = -Wall -Wextra -Werror -Wfatal-errors
CXXFLAGS += -std=c++11
CXXFLAGS += -I $(TOP)/include
CXXFLAGS += -fPIC

VPATH += $(TOP)/include/tinyalsa

.PHONY: all
all: libtinyalsa-cxx.so

libtinyalsa-cxx.so: config.o pcm.o input-pcm.o
	$(CXX) -shared $^ -o $@

config.o: config.cpp config.hpp

pcm.o: pcm.cpp pcm.hpp

input-pcm.o: input-pcm.cpp input-pcm.hpp pcm.hpp

.PHONY: clean
clean:
	$(RM) libtinyalsa-cxx.sp
	$(RM) pcm.o input-pcm.o

