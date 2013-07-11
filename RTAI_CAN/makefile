CC=gcc
CFLAGS=-Wall
LDFLAGS=-ldl 
OBJ=RT_main.o limb.o myCan.o $(CAN_OBJ) 
EXECUTABLE=RT_main
CAN_OBJ=canerror.o ntcan.o

all: $(EXECUTABLE)
$(EXECUTABLE): $(OBJ) 
	gcc -o RT_main $(OBJ)

RT_main.o: limb.o RT_main.c

limb.o: myCan.o limb.c 

myCan.o: $(CAN_OBJ) myCan.c 

.PHONY:clean
clean:
	rm -f $(EXECUTABLE) myCan.o RT_main.o limb.o
