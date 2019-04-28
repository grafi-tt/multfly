CC ?= gcc
CXX ?= g++
CFLAGS ?= -O2 -fno-stack-protector -Wall -Wextra -Wpedantic -Werror

src/multfly.so: src/multfly.o
	$(CC) -shared $^ -o $@

test/bigcrush: src/multfly.o test/bigcrush.o
	$(CC) $^ -ltestu01 -lprobdist -lmylib -o $@

src/multfly.o: src/multfly.c include/multfly.h
	$(CC) $(CFLAGS) -std=c99 -fPIC -I./include -c $< -o $@

test/bigcrush.o: test/bigcrush.c include/multfly.h
	$(CC) $(CFLAGS) -std=c99 -fPIC -I./include -c $< -o $@

.PHONY: clean
clean:
	rm -rf src/multfly.so test/bigcrush src/multfly.o test/bigcrush.o
