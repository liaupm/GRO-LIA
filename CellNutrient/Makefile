ifeq ($(OS),Windows_NT)
	RM=del
else
	RM=rm -f
endif

ifeq ($(OS),Windows_NT)
        OS_DIR=win
	COMP = g++
else
        UNAME_S := $(shell uname -s)
        ifeq ($(UNAME_S),Linux)
                OS_DIR=linux
		COMP = g++
        endif
        ifeq ($(UNAME_S),Darwin)
                OS_DIR=mac
		COMP = clang++
        endif
endif

ifeq ($(MODE),release)
	ifeq ($(COMP), g++)
		CC = g++ -Wall -Wextra -O3 -c -I$(INCLUDE) -std=c++11
	else
        	CC = clang++ -std=c++11 -Wall -Wextra -O3 -c -I$(INCLUDE)
	endif
else
        ifeq ($(COMP), g++)
		CC = g++ -Wall -Wextra -g -c -I$(INCLUDE) #debug
	else
		CC = clang++ -std=c++11 -Wall -Wextra -g -c -I$(INCLUDE)
	endif
endif

ifeq ($(OS_DIR),mac)
        CC += -mmacosx-version-min=10.8
endif

INCLUDE=include
BUILD=build/$(OS_DIR)/$(MODE)
SRC=src

#CXXFLAGS = -m64
#CFLAGS = -m64
#LDFLAGS = -Wl,-m64,-arch,x86_64

#CC = g++ -Wall -Wextra -O3 -c -I$(INCLUDE) -std=c++11
#CC = g++ -Wall -Wextra -O3 -c -I$(INCLUDE)
#CC = g++ -Wall -Wextra -g -c -I$(INCLUDE) #debug
#CC = clang++ -std=c++11 -Wall -O3 -c -I$(INCLUDE) #release
#CC = clang++ -std=c++11 -Wall -O3 -c -I$(INCLUDE)
AR = ar -rs
#AR = /Applications//Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ar -rs

OBJECTS=$(BUILD)/CNvector.o $(BUILD)/CNGrid.o $(BUILD)/CNNutrients.o $(BUILD)/CellNutrient.o
OUTPUT=$(BUILD)/libcellnutrient.a
INCLUDES=$(INCLUDE)/CNvector.h $(INCLUDE)/CNGrid.h $(INCLUDE)/CNNutrients.h $(INCLUDE)/CellNutrient.h

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(AR) $(OUTPUT) $(OBJECTS)

$(BUILD)/%.o: $(SRC)/%.c
	$(CC) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(BUILD)/*.o $(OUTPUT)
