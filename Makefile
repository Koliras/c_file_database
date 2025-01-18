SHELL := /bin/zsh
TARGET = bin/dbview

run: clean default

default: $(TARGET)
	${TARGET} test.db

build:
	gcc -o ${TARGET} src/**/*.c

clean:
	rm -f bin/*

$(TARGET):
	gcc -o bin/dbview src/**/*.c
