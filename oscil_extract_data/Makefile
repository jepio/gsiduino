CXXFLAGS := -std=c++11 $(shell root-config --cflags) -Isource/
LDFLAGS := $(shell root-config --libs)
LD = g++

all: macro
	echo $(obj)

data := $(wildcard source/Esr*.h)
cxx := $(wildcard source/Esr*.cxx) 
obj := $(notdir $(cxx:.cxx=.o))

.PHONY: clean
clean:
	rm -rf macro *.o Dict.* *.d

macro: macro.o Dict.o $(obj)
	$(LD) $(LDFLAGS) -o $@ $^

Dict.cxx: $(data)
	rootcint -f $@ -c $^ source/Linkdef.h

%.o: %.cxx
	g++ $(CXXFLAGS) -c -o $@ $^

%.o: source/%.cxx
	g++ $(CXXFLAGS) -c -o $@ $^
