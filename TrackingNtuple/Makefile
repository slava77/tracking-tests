ifndef ROOTSYS
all:
	@echo "ROOTSYS is not set. Please set ROOT environment properly"; echo
else
all: 	build

BASEDIR = ./
MICTESTDIR = /home/cerati/mictest
INCLUDE = $(shell root-config --cflags) -I$(BASEDIR) -I$(MICTESTDIR)
ROOTLIBS = $(shell root-config --ldflags --cflags --libs) -lEG -lGenVector

MICTESTOBJECTS=$(MICTESTDIR)/Track.o $(MICTESTDIR)/Hit.o $(MICTESTDIR)/Matrix.o $(MICTESTDIR)/Config.o

MICTESTSOURCES=$(MICTESTOBJECTS:.o=.cc)
MICTESTSHEADER=$(MICTESTOBJECTS:.o=.h)

SOURCES = WriteMemoryFile.cc

EXE = writeMemoryFile

#
# how to make it
#

$(EXE): ${SOURCES} ${MICTESTSOURCES} ${MICTESTSHEADER}
	$(CXX) -o $@ ${SOURCES} $(MICTESTOBJECTS) $(ROOTLIBS)

build:  $(EXE)

b: build

clean: 
	rm writeMemoryFile

endif