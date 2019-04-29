CC ?= gcc
CXX ?= g++
CFLAGS ?= -O2 -march=native -fno-stack-protector -Wall -Wextra -Wpedantic -Werror

.PHONY: all
all: src/multfly_ref.so src/multfly_ssse3.so

src/multfly_ref.so: src/multfly_ref.o
	$(CC) -shared $^ -o $@

src/multfly_ssse3.so: src/multfly_ssse3.o
	$(CC) -shared $^ -o $@

src/multfly_ref.o: src/multfly_ref.c include/multfly.h
	$(CC) $(CFLAGS) -std=c99 -fPIC -I./include -c $< -o $@

src/multfly_ssse3.o: src/multfly_ssse3.c include/multfly.h
	$(CC) $(CFLAGS) -std=c99 -fPIC -I./include -c $< -o $@

.PHONY: test
test: test/bigcrush

test/bigcrush: src/multfly_ref.o test/bigcrush.o
	$(CC) $^ -ltestu01 -lprobdist -lmylib -o $@

test/bigcrush.o: test/bigcrush.c include/multfly.h
	$(CC) $(CFLAGS) -std=c99 -fPIC -I./include -c $< -o $@

.PHONY: clean
clean:
	rm -rf \
		src/multfly_ref.so \
		src/multfly_ssse3.so \
		src/multfly_ref.o \
		src/multfly_ssse3.o \
		test/bigcrush \
		test/bigcrush.o
