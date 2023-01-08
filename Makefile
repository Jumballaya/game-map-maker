

CC = g++
BIN = MapMaker
DIR_OUT = dist/
FLAGS = -Wall -Wfatal-errors -std=c++17
LINKER = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua5.3
INCLUDE = -I"./libs/" -I"/usr/include/SDL2"
SOURCE_FILES = $(shell find src/ -type f -name '*.cpp')
INC_SOURCE_FILES = ./libs/imgui/*.cpp
SOURCE = $(SOURCE_FILES) $(INC_SOURCE_FILES)


build:
	mkdir -p $(DIR_OUT)
	$(CC) $(FLAGS) $(SOURCE) $(INCLUDE) $(LINKER) -o $(DIR_OUT)$(BIN)

run:
	./dist/MapMaker

clean:
	rm dist/MapMaker