ifeq ($(OS),Windows_NT)
        RM=del
else
        RM=rm -f
endif

ifeq ($(OS),Windows_NT)
        OS_DIR=win
else
        UNAME_S := $(shell uname -s)
        ifeq ($(UNAME_S),Linux)
                OS_DIR=linux
        endif
        ifeq ($(UNAME_S),Darwin)
                OS_DIR=mac
        endif
endif

ifeq ($(MODE),release)
        CC = clang++ -std=c++11 -stdlib=libc++ -Wall -Wextra -O3 -I$(INCLUDE) 
else
        #CC = g++ -Wall -Wextra -g -c -I$(INCLUDE) -std=c++11 #Debug
        #CC = clang++ -std=c++11 -stdlib=libc++ -Wall -Wextra -g -I$(INCLUDE)
        CC = clang++ -std=c++11 -Wall -Wextra -g -I$(INCLUDE)
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
#CC = g++ -Wall -Wextra -g -c -I$(INCLUDE) -std=c++11 #Debug
#CC = g++ -Wall -Wextra -O3 -c -I$(INCLUDE)
#CC = clang++ -std=c++11 -Wall -O3 -c -I$(INCLUDE) #release
AR = ar -rs
#AR = x86_64-apple-darwin1X-ar -rs


OBJECTS=$(BUILD)/GCell.o $(BUILD)/Grid.o $(BUILD)/Core.o
OUTPUT=$(BUILD)/libsignals.a
INCLUDES=$(INCLUDE)/GCell.h $(INCLUDE)/Grid.h $(INCLUDE)/GSignal.h $(INCLUDE)/Core.h

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(AR) $(OUTPUT) $(OBJECTS)

$(BUILD)/%.o: $(SRC)/%.cpp
	$(CC) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(BUILD)/*.o $(OUTPUT)
