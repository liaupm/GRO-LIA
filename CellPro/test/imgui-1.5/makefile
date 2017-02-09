#Select OS
ifeq ($(OS),Windows_NT)
    OS_NAME=windows
    COMPILER=g++
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        OS_NAME=linux
        COMPILER=g++
    endif
    ifeq ($(UNAME_S),Darwin)
        OS_NAME=osx
        COMPILER=clang++
    endif
endif

INCLUDE=-Iinclude/IMGUI
TEMP=../build/imgui
BUILD=../build/imgui

CPP=$(COMPILER) -std=c++11 -O3 -c $(INCLUDE)
AR = ar -rs

all:
	$(CPP) -o $(TEMP)/imgui.o src/imgui.cpp
	$(CPP) -o $(TEMP)/draw.o src/draw.cpp
	$(CPP) -o $(TEMP)/demo.o src/demo.cpp
	$(AR) $(BUILD)/libimgui.a $(TEMP)/imgui.o $(TEMP)/draw.o $(TEMP)/demo.o
