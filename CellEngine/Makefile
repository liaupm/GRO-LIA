ifeq ($(OS),Windows_NT)
	RM=del
else
	RM=rm -f
endif

ifeq ($(OS),Windows_NT)
	OS_DIR=win
	COMP = gcc
else
	UNAME_S := $(shell uname -s)
    	ifeq ($(UNAME_S),Linux)
        	OS_DIR=linux
		COMP = gcc
    	endif
    	ifeq ($(UNAME_S),Darwin)
        	OS_DIR=mac
		COMP = clang
    	endif
endif

ifeq ($(MODE),release)
	ifeq ($(COMP), gcc)
		C = gcc -Wall -O3 -c -I$(INCLUDE) -std=c99 #release
	else
		C = clang -Wall -O3 -c -I$(INCLUDE) -std=c11
	endif 
else
	ifeq ($(COMP), gcc)
		C = gcc -Wall -g -c -I$(INCLUDE) -std=c99 #debug centos
	else
		C = clang -Wall -g -c -I$(INCLUDE) -std=c11
	endif 
endif

ifeq ($(OS_DIR),mac)
	C += -mmacosx-version-min=10.8
endif

INCLUDE=include/
BUILD=build/$(OS_DIR)/$(MODE)/
SRC=src/

#C = gcc -Wall -g -c -I$(INCLUDE) -std=c99 #debug mac
#C = gcc -Wall -g -c -I$(INCLUDE) -std=c99 #debug centos
#C = gcc -Wall -d -c -I$(INCLUDE) -std=c99 #debug
#C = gcc -Wall -O3 -c -I$(INCLUDE) -std=c99 #release
#C = clang -Wall -O3 -c -I$(INCLUDE) #release
AR = ar -rs
#AR = /Applications//Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ar -rs

OBJECTS=$(BUILD)Body.o $(BUILD)Space.o $(BUILD)CellEngine.o
OUTPUT=$(BUILD)libcellengine.a
INCLUDES=$(INCLUDE)CESpace.h $(INCLUDE)private/CESpace_p.h $(INCLUDE)CEBody.h $(INCLUDE)private/CEBody_p.h $(INCLUDE)CEVector2.h $(INCLUDE)CellEngine.h $(INCLUDE)private/CellEngine_p.h

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(AR) $(OUTPUT) $(OBJECTS)

$(BUILD)Space.o: $(SRC)Space.c $(INCLUDES)
	$(C) -o $(BUILD)Space.o $(SRC)Space.c

$(BUILD)Body.o: $(SRC)Body.c $(INCLUDES)
	$(C) -o $(BUILD)Body.o $(SRC)Body.c

$(BUILD)CellEngine.o: $(SRC)CellEngine.c $(INCLUDES)
	$(C) -o $(BUILD)CellEngine.o $(SRC)CellEngine.c

.PHONY: clean

clean:
	$(RM) $(BUILD)*.o $(OUTPUT)
