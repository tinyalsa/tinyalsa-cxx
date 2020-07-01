-include config.mk

CXXFLAGS := -Wall -Wextra -Werror -Wfatal-errors -I $(CURDIR)

ifdef TINYALSA_DEBUG
CXXFLAGS := $(CXXFLAGS) -g
else
CXXFLAGS := $(CXXFLAGS) -Os -fno-rtti
endif

examples += examples/pcminfo

.PHONY: all
all: libtinyalsa-cxx.a

libtinyalsa-cxx.a: tinyalsa.o
	$(AR) $(ARFLAGS) $@ $^

tinyalsa.o: tinyalsa.cpp

.PHONY: examples
examples: $(examples)

examples/pcminfo: examples/pcminfo.o libtinyalsa-cxx.a

examples/pcminfo.o: examples/pcminfo.cpp tinyalsa.hpp

examples/%: examples/%.o libtinyalsa-cxx.a
	$(CXX) $^ -o $@ libtinyalsa-cxx.a

.PHONY: clean
clean:
	$(RM) tinyalsa.o libtinyalsa-cxx.a
