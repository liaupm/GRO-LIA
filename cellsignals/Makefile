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
	ifeq ($(COMP),g++)
		CC = g++ -Wall -Wextra -O3 -c -I$(INCLUDE) -std=c++11
        else	
		CC = clang++ -std=c++11 -stdlib=libc++ -Wall -Wextra -O3 -I$(INCLUDE) 
	endif
else
        ifeq ($(COMP), g++)
		CC = g++ -Wall -Wextra -g -c -I$(INCLUDE) -std=c++11
        else
		CC = clang++ -std=c++11 -Wall -Wextra -g -I$(INCLUDE)
	endif
endif

INCLUDE=include
BUILD=build/$(OS_DIR)/$(MODE)/
#BUILD=build/$(OS_DIR)/debug/
SRC=src

#CXXFLAGS = -m64
#CFLAGS = -m64
#LDFLAGS = -Wl,-m64,-arch,x86_64

#CC = g++ -Wall -Wextra -O3 -c -I$(INCLUDE) -std=c++11
#CC = g++ -Wall -Wextra -g -c -I$(INCLUDE) -std=c++11 #Debug
#CC = g++ -Wall -Wextra -O3 -c -I$(INCLUDE)
#CC = clang++ -std=c++11 -Wall -O3 -c -I$(INCLUDE) #release
AR = ar -rs
#AR = x86_64-apple-darwin1X-ar -rs


OBJECTS=$(BUILD)/CSCell.o $(BUILD)/DBCell.o $(BUILD)/Grid.o $(BUILD)/Diffusion.o $(BUILD)/_structures.o
OUTPUT=$(BUILD)/libsignals.a
INCLUDES=$(INCLUDE)/GCell.h $(INCLUDE)/CSCell.h $(INCLUDE)/DBCell.h $(INCLUDE)/Grid.h $(INCLUDE)/GSignal.h $(INCLUDE)/GReaction.h $(INCLUDE)/_structures.h

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(AR) $(OUTPUT) $(OBJECTS)

$(BUILD)/%.o: $(SRC)/%.cpp
	$(CC) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(BUILD)/*.o $(OUTPUT)
