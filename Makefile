CC := clang

CFLAGS := -Wall -Wextra -Wpedantic -std=c11 -O0 -g -Iinclude
LDFLAGS :=

SRC := $(wildcard src/*.c)
TARGET := dist/ndarray

VALGRIND := valgrind \
	--leak-check=full \
	--show-leak-kinds=all \
	--track-origins=yes \
	--errors-for-leak-kinds=all \
	--error-exitcode=1

.PHONY: all test clean

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p dist
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

test: $(TARGET)
	$(VALGRIND) ./$(TARGET)

clean:
	rm -rf dist
