CXX = g++
CC := $(CXX)

CXXFLAGS += -Wall
CXXFLAGS += -fopenmp -O3 $(shell root-config --cflags) -std=c++11

LDFLAGS += -fopenmp -O3 -std=c++11 $(shell root-config --libs) -lboost_filesystem -lboost_system

all: oscilloscope

oscilloscope: oscilloscope.o filter.o

clean:
	rm -rf oscilloscope *.o
