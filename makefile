CC=gcc-4.4
LDFLAGS=-ldl
OBJECTS=myTest.o ntcan.o canerror.o
EXECUTABLE=myTest

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)

myTest.o: ntcan.h

clean:
	rm -f $(EXECUTABLE)
