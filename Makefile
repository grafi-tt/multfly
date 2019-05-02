CC ?= gcc
CFLAGS ?= -O2 -march=native -fno-stack-protector -Wall -Wextra -Wpedantic -Werror

.PHONY: all
all: test/test_ref test/test_sse41 test/bigcrush test/bigcrush_bitrev test/bigcrush_uint64 test/bigcrush_uint64_bitrev

test/test_ref: test/test.c include/multfly_ref.h include/multfly_types.h
	$(CC) $(CFLAGS) -std=c99 -I./include $< -o $@

test/test_sse41: test/test.c include/multfly_sse41.h include/multfly_types.h
	$(CC) $(CFLAGS) -std=c99 -I./include -DSSE41 $< -o $@

test/bigcrush: test/bigcrush.c include/multfly_ref.h include/multfly_types.h
	$(CC) $(CFLAGS) -std=c99 -I./include -ltestu01 -lprobdist -lmylib $< -o $@

test/bigcrush_bitrev: test/bigcrush.c include/multfly_ref.h include/multfly_types.h
	$(CC) $(CFLAGS) -std=c99 -I./include -ltestu01 -lprobdist -lmylib -DBITREV $< -o $@

test/bigcrush_uint64: test/bigcrush.c include/multfly_ref.h include/multfly_types.h
	$(CC) $(CFLAGS) -std=c99 -I./include -ltestu01 -lprobdist -lmylib -DUINT64 $< -o $@

test/bigcrush_uint64_bitrev: test/bigcrush.c include/multfly_ref.h include/multfly_types.h
	$(CC) $(CFLAGS) -std=c99 -I./include -ltestu01 -lprobdist -lmylib -DUINT64 -DBITREV $< -o $@

.PHONY: clean
clean:
	rm -rf \
		test/test_ref \
		test/test_sse41 \
		test/bigcrush \
		test/bigcrush_bitrev \
		test/bigcrush_uint64 \
		test/bigcrush_uint64_bitrev
