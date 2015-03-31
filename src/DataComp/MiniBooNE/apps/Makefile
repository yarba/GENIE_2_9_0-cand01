#
# GENIE Makefile 
#
# Author: Chris Barry, Liverpool 
#

SHELL = /bin/sh
NAME = all
MAKEFILE = Makefile

# Include machine specific flags and locations (inc. files & libs)
include $(GENIE)/src/make/Make.include

# Add extra libraries needed for these executables
GENIE_LIBS   = $(shell $(GENIE)/src/scripts/setup/genie-config --libs)
LIBRARIES   := $(GENIE_LIBS) $(LIBRARIES) $(CERN_LIBRARIES)

MBCCQE_SOURCES   := gVldMBCCQE.cxx 
MBCCQE_OBJECTS    = $(addsuffix .$(ObjSuf), $(basename $(MBCCQE_SOURCES)))
MBNC1PI0_SOURCES := gVldMBNC1pi0.cxx 
MBNC1PI0_OBJECTS  = $(addsuffix .$(ObjSuf), $(basename $(MBNC1PI0_SOURCES)))
MBCC1PI0_SOURCES := gVldMBCC1pi0.cxx 
MBCC1PI0_OBJECTS  = $(addsuffix .$(ObjSuf), $(basename $(MBCC1PI0_SOURCES)))
MBCC1PIP_SOURCES := gVldMBCC1pip.cxx 
MBCC1PIP_OBJECTS  = $(addsuffix .$(ObjSuf), $(basename $(MBCC1PIP_SOURCES)))

all: gVldMBCCQE \
     gVldMBCC1pip 
#    gVldMBNC1pi0 
#    gVldMBCC1pi0 

gVldMBCCQE: FORCE
	$(CXX) $(CXXFLAGS) -c $(MBCCQE_SOURCES) $(INCLUDES)
	$(LD) $(LDFLAGS) $(MBCCQE_OBJECTS) $(LIBRARIES) -o $(GENIE_BIN_PATH)/gvld_miniboone_ccqe

gVldMBCC1pip: FORCE
	$(CXX) $(CXXFLAGS) -c $(MBCC1PIP_SOURCES) $(INCLUDES)
	$(LD) $(LDFLAGS) $(MBCC1PIP_OBJECTS) $(LIBRARIES) -o $(GENIE_BIN_PATH)/gvld_miniboone_cc1pip

gVldMBNC1pi0: FORCE
	$(CXX) $(CXXFLAGS) -c $(MBNC1PI0_SOURCES) $(INCLUDES)
	$(LD) $(LDFLAGS) $(MBNC1PI0_OBJECTS) $(LIBRARIES) -o $(GENIE_BIN_PATH)/gvld_miniboone_nc1pi0

gVldMBCC1pi0: FORCE
	$(CXX) $(CXXFLAGS) -c $(MBCC1PI0_SOURCES) $(INCLUDES)
	$(LD) $(LDFLAGS) $(MBCC1PI0_OBJECTS) $(LIBRARIES) -o $(GENIE_BIN_PATH)/gvld_miniboone_cc1pi0

purge: FORCE
	$(RM) *.o *~ core 

clean: FORCE
	$(RM) *.o *~ core 
	$(RM) $(GENIE_BIN_PATH)/gvld_miniboone_ccqe
	$(RM) $(GENIE_BIN_PATH)/gvld_miniboone_cc1pip
#	$(RM) $(GENIE_BIN_PATH)/gvld_miniboone_nc1pi0
#	$(RM) $(GENIE_BIN_PATH)/gvld_miniboone_cc1pi0

distclean: FORCE
	$(RM) $(GENIE_BIN_INSTALLATION_PATH)/gvld_miniboone_ccqe
	$(RM) $(GENIE_BIN_INSTALLATION_PATH)/gvld_miniboone_cc1pip
#	$(RM) $(GENIE_BIN_INSTALLATION_PATH)/gvld_miniboone_nc1pi0
#	$(RM) $(GENIE_BIN_INSTALLATION_PATH)/gvld_miniboone_cc1pi0

FORCE:

# DO NOT DELETE
