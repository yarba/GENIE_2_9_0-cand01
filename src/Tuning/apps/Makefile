#
# GENIE Makefile 
#
# Author: Costas Andreopoulos
#

SHELL = /bin/sh
NAME = all
MAKEFILE = Makefile

# Include machine specific flags and locations (inc. files & libs)
include $(GENIE)/src/make/Make.include

# Add extra libraries needed for these executables
GENIE_LIBS   = $(shell $(GENIE)/src/scripts/setup/genie-config --libs)
LIBRARIES   := $(GENIE_LIBS) $(LIBRARIES) $(CERN_LIBRARIES)

all: gTunePrototyping 

gTunePrototyping: FORCE
	$(CXX) $(CXXFLAGS) -c gTunePrototyping.cxx $(INCLUDES)
	$(LD) $(LDFLAGS) gTunePrototyping.$(ObjSuf) $(LIBRARIES) -o $(GENIE_BIN_PATH)/gtune_proto

purge: FORCE
	$(RM) *.o *~ core 

clean: FORCE
	$(RM) *.o *~ core 
	$(RM) $(GENIE_BIN_PATH)/gtune_proto

distclean: FORCE
	$(RM) $(GENIE_BIN_INSTALLATION_PATH)/gtune_proto

FORCE:

# DO NOT DELETE
