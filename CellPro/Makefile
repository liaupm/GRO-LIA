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
        	CC = g++ -Wall -Wextra -O3  -I$(INCLUDE) -std=gnu++11 #Release
	else

        	CC = clang++ -std=c++11 -stdlib=libc++ -Wall -Wextra -O3 -I$(INCLUDE) 
	endif
else
        ifeq ($(COMP), g++)
		CC = g++ -Wall -Wextra -g -I$(INCLUDE) -std=gnu++11 #Debug
	else
		CC = clang++ -std=c++11 -stdlib=libc++ -Wall -Wextra -g -I$(INCLUDE)
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
#CC = g++ -Wall -Wextra -g -c -I$(INCLUDE) -std=c++11 #Debug
#CC = g++ -Wall -Wextra -O3 -c -I$(INCLUDE)
#CC = clang++ -std=c++11 -Wall -O3 -c -I$(INCLUDE) #release
AR = ar -rs
#AR = x86_64-apple-darwin1X-ar -rs


OBJECTS=$(BUILD)/GenCell.o $(BUILD)/GenListPlasmid.o $(BUILD)/GenPlasmid.o $(BUILD)/GenOperon.o $(BUILD)/GenPromoter.o $(BUILD)/erreenege.o
OUTPUT=$(BUILD)/libcellpro.a
#OUTPUT=$(BUILD)/cellgene
INCLUDES=$(INCLUDE)/GenCell.h $(INCLUDE)/GenListPlasmid.h $(INCLUDE)/GenPlasmid.h $(INCLUDE)/GenOperon.h $(INCLUDE)/GenPromoter.h $(INCLUDE)/erreenege.h

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(AR) $(OUTPUT) $(OBJECTS)

#$(OUTPUT): $(OBJECTS)
#	$(CC) -o $(OUTPUT) $(OBJECTS)

$(BUILD)/%.o: $(SRC)/%.cpp
	$(CC) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(BUILD)/*.o $(OUTPUT)

