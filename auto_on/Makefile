#*************************************************************
#*
#* Author :      SecuGen Corporation
#* Description : AUTO ON Makefile
#* Copyright(c): 2009 SecuGen Corporation, All rights reserved
#* date        person   comments
#* ======================================================
#* 12/10/2009  driley   initial release
#*
#*************************************************************

PORT = linux3
CFLAGS = -DLINUX3
CPP=g++
INCPATH = -I./ -I../include
LIBS =  -L../lib 

FDU03_TARGET = ../bin/$(PORT)/auto_on_fdu03
FDU04_TARGET = ../bin/$(PORT)/auto_on_fdu04
FDU05_TARGET = ../bin/$(PORT)/auto_on_fdu05
FDU06_TARGET = ../bin/$(PORT)/auto_on_fdu06

all : $(FDU06_TARGET) $(FDU05_TARGET) $(FDU04_TARGET) $(FDU03_TARGET)

$(FDU06_TARGET) :
	rm -rf $(PORT)/main.o
	$(CPP) $(CFLAGS) $(INCPATH) -D_FDU06 -o $@ main.cpp $(LIBS) -lsgfplib -lsgnfiq -lsgfpamx -lsgfdu06 -lusb -lpthread 

$(FDU05_TARGET) :
	rm -rf $(PORT)/main.o
	$(CPP) $(CFLAGS) $(INCPATH) -D_FDU05 -o $@ main.cpp $(LIBS) -lsgfplib -lsgnfiq -lsgfpamx -lsgfdu05 -lusb -lpthread 

$(FDU04_TARGET) :
	rm -rf $(PORT)/main.o
	$(CPP) $(CFLAGS) $(INCPATH) -D_FDU04 -o $@ main.cpp $(LIBS) -lsgfplib -lsgnfiq -lsgfpamx -lsgfdu04 -lusb -lpthread 

$(FDU03_TARGET) : 
	rm -rf $(PORT)/main.o
	$(CPP) $(CFLAGS) $(INCPATH) -D_FDU03 -o $@ main.cpp $(LIBS) -lsgfplib -lsgnfiq -lsgfpamx -lsgfdu03 -lusb -lpthread 

clean :
	rm -rf *.raw $(OBJS) $(FDU03_TARGET) $(FDU04_TARGET) $(FDU05_TARGET) $(FDU06_TARGET) core
	rm -rf $(PORT)
	mkdir $(PORT)

