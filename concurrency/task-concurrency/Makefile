# Set you prefererred CFLAGS/compiler compiler here.
# Our github runner provides gcc-10 by default.
CC ?= cc
CFLAGS ?= -g -Wall -O2
CXX ?= c++
CXXFLAGS ?= -g -Wall -O2 -std=c++17
CARGO ?= cargo
RUSTFLAGS ?= -g
#DEBUGFLAGS ?= -D DEBUG_PRINT 

# this target should build all executables for all tests
#all:
#	@echo "Please set a concrete build command here"
#	false

.PHONY: all clean check

## Rust Example
#all:
#	cargo build

## C/C++ example
all: libcspinlock.so liblockhashmap.so liblockfreehashmap.so

libcspinlock.so: cspinlock.cpp
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) -shared -fPIC -ldl -o $@ $<

liblockhashmap.so: lockhashmap.cpp
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) -shared -fPIC -ldl -o $@ $<

liblockfreehashmap.so: lockfreehashmap.cpp
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) -shared -fPIC -ldl -o $@ $<

# Usually there is no need to modify this
check: all
	$(MAKE) -C tests check

clean:
	clear
	$(MAKE) -C tests clean
	rm -rf *.so* *.o
