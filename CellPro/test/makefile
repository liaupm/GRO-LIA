#Select OS
ifeq ($(OS),Windows_NT)
    RM=del
else
    RM=rm -f
endif

ifeq ($(OS),Windows_NT)
    OS_NAME=windows
    OS_LIBS=-lopengl32
    COMPILER=g++
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        OS_NAME=linux
        OS_LIBS= -lGL -ldl
        COMPILER=g++
    endif
    ifeq ($(UNAME_S),Darwin)
        OS_NAME=osx
        OS_LIBS=-framework OpenGL
        COMPILER=clang++
    endif
endif

#Select MODE
MODE=debug
ifeq ($(MODE),release)
    MODE_FLAGS=-O3
else
	ifeq ($(MODE),debug)
		MODE_FLAGS=-g
	endif
endif

INCLUDE_PATH=-I. -I. -I../include -Iimgui-1.5/include
LIB_PATH=-Lbuild -Lbuild/imgui
LIBS=-lsfml-window -lsfml-system -limgui -lcellgene $(OS_LIBS)

TEMP=build
OUTPUT=build/GeneticTest

CPP=$(COMPILER) -std=c++11 -c -Wall $(MODE_FLAGS) $(INCLUDE_PATH) -o
CPP_L=$(COMPILER) -std=c++11 $(MODE_FLAGS) $(LIB_PATH) -o

all:
	$(CPP) build/IMGUI_SFML.o IMGUI_SFML.cpp
	$(CPP) build/GeneticTest.o GeneticTest.cpp
	$(CPP_L) $(OUTPUT) $(TEMP)/GeneticTest.o $(TEMP)/IMGUI_SFML.o $(LIBS)
