-include config.mk

CXXFLAGS := -Wall -Wextra -Werror -Wfatal-errors -I $(CURDIR)

ifdef TINYALSA_DEBUG
CXXFLAGS := $(CXXFLAGS) -g
else
CXXFLAGS := $(CXXFLAGS) -Os -fno-rtti -fno-exceptions
endif

examples += examples/pcminfo
examples += examples/pcmlist

.PHONY: all
all: libtinyalsa-cxx.a

libtinyalsa-cxx.a: tinyalsa.o
	$(AR) $(ARFLAGS) $@ $^

tinyalsa.o: tinyalsa.cpp tinyalsa.hpp

.PHONY: examples
examples: $(examples)

examples/pcminfo: examples/pcminfo.o libtinyalsa-cxx.a

examples/pcminfo.o: examples/pcminfo.cpp tinyalsa.hpp

examples/pcmlist: examples/pcmlist.o libtinyalsa-cxx.a

examples/pcmlist.o: examples/pcmlist.cpp tinyalsa.hpp

examples/%: examples/%.o libtinyalsa-cxx.a
	$(CXX) $^ -o $@ libtinyalsa-cxx.a

.PHONY: clean
clean:
	$(RM) tinyalsa.o libtinyalsa-cxx.a $(examples) examples/*.o
