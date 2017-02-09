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


OBJECTS=$(BUILD)/Protein.o $(BUILD)/Gene.o $(BUILD)/Promoter.o $(BUILD)/Operon.o $(BUILD)/Plasmid.o $(BUILD)/PlasmidPool.o $(BUILD)/Genome.o

AR = ar -rs

all:
	$(CC) -c -o $(BUILD)/Protein.o src/cg/Protein.cpp
	$(CC) -c -o $(BUILD)/Gene.o src/cg/Gene.cpp
	$(CC) -c -o $(BUILD)/Promoter.o src/cg/Promoter.cpp
	$(CC) -c -o $(BUILD)/Operon.o src/cg/Operon.cpp
	$(CC) -c -o $(BUILD)/Plasmid.o src/cg/Plasmid.cpp
	$(CC) -c -o $(BUILD)/PlasmidPool.o src/cg/PlasmidPool.cpp
	$(CC) -c -o $(BUILD)/Genome.o src/cg/Genome.cpp

	$(AR) $(BUILD)/libcellpro.a $(OBJECTS)


.PHONY: clean

clean:
	rm -f $(BUILD)/*.o $(OUTPUT)

