#####################################################################
ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)

EXE	=	../GGMpp
MON	=	monitor
CXX	=	g++ -g -w
CXXFLAGS      = -O -fbounds-check
LD	=	g++
LDFLAGS = 	-g
COPTS	=	-ggdb -fPIC -DLINUX -Wall -O0 
#COPTS	=	-g -fPIC -DLINUX -Wall
FLAGS	=	-ggdb -Wall -s -O0
#FLAGS	=	-Wall
DEPLIBS	=       -l CAENVME -l curses -lSpectrum
CXXFLAGS     += $(ROOTCFLAGS)
LIBS	=	$(ROOTLIBS)
GLIBS   =       $(ROOTGLIBS)
INCLUDEDIR =	-I.
OBJS	=	ggmpp.o console.o GGMpulser.o Send.o ArduinoRead.o PayloadSimulator.o PayloadProcess.o Print.o ReadConf.o OutputFile.o #PlotEffi.o
INCLUDES =	CAENVMElib.h CAENVMEtypes.h CAENVMEoslib.h console.h INIReader.h ReadConf.h

MON_OBJS	=	monitor.o ReadConf.o
MON_INCLUDES =	monitor.h ReadConf.h
#########################################################################

all	:	$(EXE) $(MON)

clean	:
		/bin/rm -f $(OBJS) $(EXE)

$(EXE)	:	$(OBJS)
		/bin/rm -f $(EXE)
		$(LD) $(LDFLAGS) $(FLAGS) -o $(EXE) $(OBJS) $(DEPLIBS) $(LIBS) $(GLIBS)

$(MON)	:	$(MON_OBJS)
		/bin/rm -f $(MON)
		$(LD) $(LDFLAGS) $(FLAGS) -o $(MON) $(MON_OBJS) $(DEPLIBS) $(LIBS) $(GLIBS)

$(OBJS)	:	$(INCLUDES) Makefile

$(MON_OBJS) :	$(MON_INCLUDES) Makefile

%.o	:	%.c
		$(CXX) -c $(CXXFLAGS) $(COPTS) $(INCLUDEDIR) -c -o $@ $< #-std=c++0x

