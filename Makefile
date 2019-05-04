CC ?= gcc
NVCC ?= nvcc
CFLAGS ?= -O2 -march=native -fno-stack-protector -Wall -Wextra -Wpedantic -Werror
NVCCFLAGS ?= -O2 --compiler-options -Wall,-Wextra,-Werror

.PHONY: all
all: test/test_ref test/test_sse41 test/test_cuda test/bigcrush test/bigcrush_bitrev test/bigcrush_graycode

test/test_ref: test/test.c include/multfly_ref.h include/multfly_types.h
	$(CC) $(CFLAGS) -std=c99 -I./include $< -o $@

test/test_sse41: test/test.c include/multfly_sse41.h include/multfly_types.h
	$(CC) $(CFLAGS) -std=c99 -I./include -DSSE41 $< -o $@

test/test_cuda: test/test_cuda.cu include/multfly_device.cuh include/multfly_types.h
	$(NVCC) $(NVCCFLAGS) -std=c++11 -I./include $< -o $@

test/bigcrush: test/bigcrush.c include/multfly_ref.h include/multfly_types.h
	$(CC) $(CFLAGS) -std=c99 -I./include -ltestu01 -lprobdist -lmylib $< -o $@

test/bigcrush_bitrev: test/bigcrush.c include/multfly_ref.h include/multfly_types.h
	$(CC) $(CFLAGS) -std=c99 -I./include -ltestu01 -lprobdist -lmylib -DBITREV $< -o $@

test/bigcrush_graycode: test/bigcrush.c include/multfly_ref.h include/multfly_types.h
	$(CC) $(CFLAGS) -std=c99 -I./include -ltestu01 -lprobdist -lmylib -DGRAYCODE $< -o $@

.PHONY: clean
clean:
	rm -rf \
		test/test_ref \
		test/test_sse41 \
		test/test_cuda \
		test/bigcrush \
		test/bigcrush_bitrev \
		test/bigcrush_graycode
