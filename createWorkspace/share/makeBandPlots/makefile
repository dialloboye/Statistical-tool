ROOTCFLAGS = $(shell root-config --cflags)
ROOTLIB    = $(shell root-config --libs) 

CXXFLAGS   = -g -I.
CXXFLAGS  += -Wno-long-long -fPIC
CXXFLAGS  += $(shell root-config --cflags)

LDFLAGS    = $(ROOTLIB)

OBJS := $(wildcard *.C)

CXXFLAGS   += -I${ROOTSYS}/include -L${ROOTSYS}/lib

%.o: %.cxx
	g++ -c $(CXXFLAGS) -o $@ $<

all: main.exe     # this is the default executable

%.exe: %.o $(OBJS)
	echo $@
	g++ $(CXXFLAGS) -o $@ $(patsubst %.exe,%.o,$@) $(OBJS) $(LDFLAGS)
