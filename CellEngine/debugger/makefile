ifeq ($(OS),Windows_NT)
	BIN=bin/win/
	OUTPUT=$(BIN)debugger.exe
	RM=del

	#change to your paths
	INCLUDE_PATHS=-IC:/libs/freeglut-2.8.1/include -IL:/libs/freeglut-2.8.1/include -I../include -Iinclude
	LIB_PATHS=-LC:/libs/freeglut-2.8.1/lib/x64 -LL:/libs/freeglut-2.8.1/lib/x64 -L../build
	LIBS=-lfreeglut -lglu32 -lopengl32 -lcellengine
else
	BIN=bin/linux/
	OUTPUT=$(BIN)debugger
	RM=rm -f

	#change to your paths
	INCLUDE_PATHS=-I/usr/include/ -I../include -Iinclude
	LIB_PATHS=-L/usr/lib -L../build
	LIBS=-lglut -lGL -lcellengine -lm #-lm standard math lib on ubuntu 14.04
endif

INCLUDE=include/
BUILD=build/
SRC=src/

OBJECTS=$(BUILD)main.o
INCLUDES=

#C=gcc -Wall -d $(INCLUDE_PATHS) -std=c99 #debug
C=gcc -Wall -O3 $(INCLUDE_PATHS) -std=c99 #release

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS) $(INCLUDES)
	$(C) -o $(OUTPUT) $(OBJECTS) $(LIB_PATHS) $(LIBS)

$(BUILD)main.o: main.c $(INCLUDES)
	$(C) -c -o $(BUILD)main.o main.c $(LIB_PATHS) $(LIBS)

.PHONY: clean

clean:
	RM $(BIN)*.o $(OUTPUT)
