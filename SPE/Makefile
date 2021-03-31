### C++ compiler
CXX           = g++
CXXFLAGS      = -O2 -Wall -fPIC -Wno-deprecated -g

### Library
ROOTCFLAGS := $(shell root-config --cflags)
ROOTLIBS   := $(shell root-config --libs )

CXXFLAGS   += $(ROOTCFLAGS)
LIBS        = $(ROOTLIBS) $(SYSLIBS)

### Targets
TARGS = txt2TTree  SPE_and_timing #createTree getTimeCFD

all: $(TARGS)

### Compilation
%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS) -lSpectrum

.PHONY: rmproper

rmproper:
	rm -vf $(TARGS)
