CXX = g++
CC := $(CXX)

CXXFLAGS += -Wall -O3 -std=c++11 -fopenmp $(shell root-config --cflags)

LDLIBS = -fopenmp $(shell root-config --libs) -lboost_filesystem -lboost_system
LDFLAGS += -Wl,-O1 -Wl,--as-needed $(shell root-config --ldflags)

all: oscilloscope

oscilloscope: oscilloscope.o filter.o

clean:
	rm -rf oscilloscope *.o
