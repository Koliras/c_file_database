SHELL := /bin/zsh
TARGET = bin/dbview
SRC = $(wildcard src/**/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

default: $(TARGET)
	bin/dbview test.db

build:
	gcc -o bin/dbview src/**/*.c

clean:
	rm -f bin/*

$(TARGET):
	gcc -o bin/dbview src/**/*.c
