CC=gcc
CFLAGS=-Wall
LDFLAGS=-ldl
OBJECTS=myTest.o limb.o myCan.o ntcan.o canerror.o 
EXECUTABLE=myTest

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 


myTest.o: limb.o myTest.c

limb.o: myCan.o limb.c

myCan.o: canerror.o ntcan.o myCan.c

clean:
	rm -f $(EXECUTABLE) myTest.o limb.o myCan.o 
