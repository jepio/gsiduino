CXXFLAGS += -fopenmp -O3 $(shell root-config --cflags) -std=c++11 

LDFLAGS += -fopenmp -O3 -std=c++11 $(shell root-config --libs) -lboost_filesystem -lboost_system

CC = g++

all: oscilloscope

oscilloscope: oscilloscope.cpp filter.cpp
	$(CC) $(CXXFLAGS) $(LDFLAGS) -o $@ $^
clean:
	rm -rf oscilloscope *.o
