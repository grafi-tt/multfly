CC ?= gcc
CFLAGS ?= -O2 -march=native -fno-stack-protector -Wall -Wextra -Wpedantic -Werror

.PHONY: all
all: src/multfly_ref.so src/multfly_sse41.so

src/multfly_ref.so: src/multfly_ref.o
	$(CC) -shared $^ -o $@

src/multfly_sse41.so: src/multfly_sse41.o
	$(CC) -shared $^ -o $@

src/multfly_ref.o: src/multfly_ref.c include/multfly.h
	$(CC) $(CFLAGS) -std=c99 -fPIC -I./include -c $< -o $@

src/multfly_sse41.o: src/multfly_sse41.c include/multfly.h
	$(CC) $(CFLAGS) -std=c99 -fPIC -I./include -c $< -o $@

.PHONY: test
test: test/test_ref test/test_sse41 test/bigcrush test/bigcrush_bitrev

test/test_ref: src/multfly_ref.o test/test.o
	$(CC) $^ -o $@

test/test_sse41: src/multfly_sse41.o test/test.o
	$(CC) $^ -o $@

test/bigcrush: src/multfly_ref.o test/bigcrush.o
	$(CC) $^ -ltestu01 -lprobdist -lmylib -o $@

test/bigcrush_bitrev: src/multfly_ref.o test/bigcrush_bitrev.o
	$(CC) $^ -ltestu01 -lprobdist -lmylib -o $@

test/test.o: test/test.c include/multfly.h
	$(CC) $(CFLAGS) -std=c99 -fPIC -I./include -c $< -o $@

test/bigcrush.o: test/bigcrush.c include/multfly.h
	$(CC) $(CFLAGS) -std=c99 -fPIC -I./include -c $< -o $@

test/bigcrush_bitrev.o: test/bigcrush.c include/multfly.h
	$(CC) -DBITREV $(CFLAGS) -std=c99 -fPIC -I./include -c $< -o $@

.PHONY: clean
clean:
	rm -rf \
		src/multfly_ref.so \
		src/multfly_sse41.so \
		src/multfly_ref.o \
		src/multfly_sse41.o \
		test/test_ref \
		test/test_sse41 \
		test/bigcrush \
		test/bigcrush_bitrev \
		test/test.o \
		test/bigcrush.o \
		test/bigcrush_bitrev.o
