SRC = src/
BUILD = build/
BIN= bin/
INCLUDE= include/
CFLAGS = -Wall -Wextra -std=gnu18 -O9
SDL_FLAGS = $(shell echo `sdl2-config --cflags --libs`) -lSDL2_image
FILES= events color_fill thread_pool entity_rendering draw physics_solver entity grid
COMPILED_FILES = $(patsubst %,$(BUILD)%.o,$(FILES)) $(BUILD)main.o
MAIN_SOURCES = $(patsubst %,$(SRC)%.c,$(FILES)) $(SRC)main.c
HEADERS = $(patsubst %,$(SRC)%.h,$(FILES)) $(SRC)shapes.h $(SRC)dimensions.h $(SRC)SDL_perso.h

all: create_exec

.depend : $(ALL_FILES)
	@gcc $(SDL_FLAGS) $(CFLAGS) -MM $(MAIN_SOURCES) $(HEADERS) -I $(SRC)  | sed 's|\([^ ]*\.o\)|$(BUILD)\1|' > .depend

%.o: 
	gcc -c -Wall -Wextra $< -O9 -I $(SRC) -o $@

-include .depend

build:
	mkdir $(BUILD)

create_exec: build physicSolver

physicSolver: $(COMPILED_FILES)
	gcc $(CFLAGS) -pthread $^ -lm -o $@ $(SDL_FLAGS)

.PHONY: all create_exec clean

clean:
	@rm -rf build .depend physicSolver
