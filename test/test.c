#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#ifdef SSE41
#include <multfly_sse41.h>
#else
#include <multfly_ref.h>
#endif

int check_number32(char *title, uint32_t result, uint32_t expected) {
	puts(title);
	if (result == expected) {
		puts("ok");
		return 0;
	} else {
		printf("Result: 0x%08"PRIX32", Expected: 0x%08"PRIX32"\n", result, expected);
		return 1;
	}
}

int check_number64(char *title, uint64_t result, uint64_t expected) {
	puts(title);
	if (result == expected) {
		puts("ok");
		return 0;
	} else {
		printf("Result: 0x%016"PRIX64", Expected: 0x%016"PRIX64"\n", result, expected);
		return 1;
	}
}

int test_init() {
	int ng = 0;

	multfly_key key0 = multfly_init(NULL, 0, 0);
	ng += check_number32("test_init 0, 0, 0 [0]", key0.v_[0], UINT32_C(0x91697D71));
	ng += check_number32("test_init 0, 0, 0 [1]", key0.v_[1], UINT32_C(0xCE9CA8CC));
	ng += check_number32("test_init 0, 0, 0 [2]", key0.v_[2], UINT32_C(0x3C633376));
	ng += check_number32("test_init 0, 0, 0 [3]", key0.v_[3], UINT32_C(0x611D7EC8));
	ng += check_number32("test_init 0, 0, 0 [4]", key0.v_[4], UINT32_C(0x4D1EFF6B));
	ng += check_number32("test_init 0, 0, 0 [5]", key0.v_[5], UINT32_C(0xD99BB1FE));
	ng += check_number32("test_init 0, 0, 0 [6]", key0.v_[6], UINT32_C(0x5B949C8E));
	ng += check_number32("test_init 0, 0, 0 [7]", key0.v_[7], UINT32_C(0x2903C1EE));

	multfly_ident ident0 = {0};
	key0 = multfly_init(&ident0, 0, 0);
	ng += check_number32("test_init 0, 0, 0 [0]", key0.v_[0], UINT32_C(0x91697D71));
	ng += check_number32("test_init 0, 0, 0 [1]", key0.v_[1], UINT32_C(0xCE9CA8CC));
	ng += check_number32("test_init 0, 0, 0 [2]", key0.v_[2], UINT32_C(0x3C633376));
	ng += check_number32("test_init 0, 0, 0 [3]", key0.v_[3], UINT32_C(0x611D7EC8));
	ng += check_number32("test_init 0, 0, 0 [4]", key0.v_[4], UINT32_C(0x4D1EFF6B));
	ng += check_number32("test_init 0, 0, 0 [5]", key0.v_[5], UINT32_C(0xD99BB1FE));
	ng += check_number32("test_init 0, 0, 0 [6]", key0.v_[6], UINT32_C(0x5B949C8E));
	ng += check_number32("test_init 0, 0, 0 [7]", key0.v_[7], UINT32_C(0x2903C1EE));

	multfly_key key0s42 = multfly_init(NULL, 42, 0);
	ng += check_number32("test_init 0, 42, 0 [0]", key0s42.v_[0], UINT32_C(0x926C3A4A));
	ng += check_number32("test_init 0, 42, 0 [1]", key0s42.v_[1], UINT32_C(0x47196DB4));
	ng += check_number32("test_init 0, 42, 0 [2]", key0s42.v_[2], UINT32_C(0x08CE7C1D));
	ng += check_number32("test_init 0, 42, 0 [3]", key0s42.v_[3], UINT32_C(0x6FD65C04));
	ng += check_number32("test_init 0, 42, 0 [4]", key0s42.v_[4], UINT32_C(0x3F86A67E));
	ng += check_number32("test_init 0, 42, 0 [5]", key0s42.v_[5], UINT32_C(0x9A959FF1));
	ng += check_number32("test_init 0, 42, 0 [6]", key0s42.v_[6], UINT32_C(0xDF30FF78));
	ng += check_number32("test_init 0, 42, 0 [7]", key0s42.v_[7], UINT32_C(0xBEE765F3));

	multfly_key key0c42 = multfly_init(NULL, 0, 42);
	ng += check_number32("test_init 0, 0, 42 [0]", key0c42.v_[0], UINT32_C(0xE62831AE));
	ng += check_number32("test_init 0, 0, 42 [1]", key0c42.v_[1], UINT32_C(0x3BD3A4F6));
	ng += check_number32("test_init 0, 0, 42 [2]", key0c42.v_[2], UINT32_C(0xAC915C09));
	ng += check_number32("test_init 0, 0, 42 [3]", key0c42.v_[3], UINT32_C(0x2E56733A));
	ng += check_number32("test_init 0, 0, 42 [4]", key0c42.v_[4], UINT32_C(0xAA2581F8));
	ng += check_number32("test_init 0, 0, 42 [5]", key0c42.v_[5], UINT32_C(0x3863CC20));
	ng += check_number32("test_init 0, 0, 42 [6]", key0c42.v_[6], UINT32_C(0xD31AC060));
	ng += check_number32("test_init 0, 0, 42 [7]", key0c42.v_[7], UINT32_C(0x925DFDA2));

	multfly_ident ident42z = {{42, 0, 0, 0, 0, 0, 0, 0}};
	multfly_key key42z = multfly_init(&ident42z, 0, 0);
	ng += check_number32("test_init {42, 0...}, 0, 0 [0]", key42z.v_[0], UINT32_C(0x41673B29));
	ng += check_number32("test_init {42, 0...}, 0, 0 [1]", key42z.v_[1], UINT32_C(0x0E6A7922));
	ng += check_number32("test_init {42, 0...}, 0, 0 [2]", key42z.v_[2], UINT32_C(0x8EE4EB55));
	ng += check_number32("test_init {42, 0...}, 0, 0 [3]", key42z.v_[3], UINT32_C(0xD5C33D29));
	ng += check_number32("test_init {42, 0...}, 0, 0 [4]", key42z.v_[4], UINT32_C(0xE7CE936C));
	ng += check_number32("test_init {42, 0...}, 0, 0 [5]", key42z.v_[5], UINT32_C(0x6171A23B));
	ng += check_number32("test_init {42, 0...}, 0, 0 [6]", key42z.v_[6], UINT32_C(0x6FBBB413));
	ng += check_number32("test_init {42, 0...}, 0, 0 [7]", key42z.v_[7], UINT32_C(0xCCF58062));

	multfly_ident identz42 = {{0, 0, 0, 0, 0, 0, 0, 42}};
	multfly_key keyz42 = multfly_init(&identz42, 0, 0);
	ng += check_number32("test_init {0..., 42}, 0, 0 [0]", keyz42.v_[0], UINT32_C(0x3FA7CF25));
	ng += check_number32("test_init {0..., 42}, 0, 0 [1]", keyz42.v_[1], UINT32_C(0x18647A4E));
	ng += check_number32("test_init {0..., 42}, 0, 0 [2]", keyz42.v_[2], UINT32_C(0xF344291A));
	ng += check_number32("test_init {0..., 42}, 0, 0 [3]", keyz42.v_[3], UINT32_C(0x2C48E2AB));
	ng += check_number32("test_init {0..., 42}, 0, 0 [4]", keyz42.v_[4], UINT32_C(0x2C4A7C75));
	ng += check_number32("test_init {0..., 42}, 0, 0 [5]", keyz42.v_[5], UINT32_C(0xE3F2EFF1));
	ng += check_number32("test_init {0..., 42}, 0, 0 [6]", keyz42.v_[6], UINT32_C(0xEFD54B97));
	ng += check_number32("test_init {0..., 42}, 0, 0 [7]", keyz42.v_[7], UINT32_C(0x9F55DFA4));

	return ng;
}

int test_gen32() {
	int ng = 0;
	multfly_key key = multfly_init(NULL, 0, 0);
	uint32_t result[4];

	multfly_gen32(&key, 0, 0, result);
	ng += check_number32("test_gen32 [0]", result[0], UINT32_C(0x9AE6ED79));
	ng += check_number32("test_gen32 [1]", result[1], UINT32_C(0x23718E06));
	ng += check_number32("test_gen32 [2]", result[2], UINT32_C(0x1DB7BE90));
	ng += check_number32("test_gen32 [3]", result[3], UINT32_C(0xBB9BA2AA));

	multfly_gen32(&key, 100, 0, result);
	ng += check_number32("test_gen32 [100]", result[0], UINT32_C(0xD30DA827));
	ng += check_number32("test_gen32 [101]", result[1], UINT32_C(0x56B9FDEB));
	ng += check_number32("test_gen32 [102]", result[2], UINT32_C(0x4D7B6C0C));
	ng += check_number32("test_gen32 [103]", result[3], UINT32_C(0xCEFCA104));

	return ng;
}

int test_gen64() {
	int ng = 0;
	multfly_key key = multfly_init(NULL, 0, 0);
	uint64_t result[4];

	multfly_gen64(&key, 0, 0, result);
	ng += check_number64("test_gen64 [0]", result[0], UINT64_C(0xDFDB426F9AE6ED79));
	ng += check_number64("test_gen64 [1]", result[1], UINT64_C(0x60BBBCDD23718E06));
	ng += check_number64("test_gen64 [2]", result[2], UINT64_C(0x92A4031B1DB7BE90));
	ng += check_number64("test_gen64 [3]", result[3], UINT64_C(0xB7BBB058BB9BA2AA));

	multfly_gen64(&key, 100, 0, result);
	ng += check_number64("test_gen64 [100]", result[0], UINT64_C(0x7A788E5ED30DA827));
	ng += check_number64("test_gen64 [101]", result[1], UINT64_C(0xEB53E60356B9FDEB));
	ng += check_number64("test_gen64 [102]", result[2], UINT64_C(0xC34E4E954D7B6C0C));
	ng += check_number64("test_gen64 [103]", result[3], UINT64_C(0xCFB5F70ECEFCA104));

	return ng;
}

int test_genf32() {
	int ng = 0;
	multfly_key key = multfly_init(NULL, 0, 0);
	float buf[4];
	uint32_t result[4];

	multfly_genf32(&key, 0, 0, buf);
	memcpy(result, buf, 16);
	ng += check_number32("test_genf32 [0]", result[0], UINT32_C(0x3F1AE6ED));
	ng += check_number32("test_genf32 [1]", result[1], UINT32_C(0x3E0DC638));
	ng += check_number32("test_genf32 [2]", result[2], UINT32_C(0x3DEDBDF0));
	ng += check_number32("test_genf32 [3]", result[3], UINT32_C(0x3F3B9BA2));

	multfly_genf32(&key, 100, 0, buf);
	memcpy(result, buf, 16);
	ng += check_number32("test_genf32 [100]", result[0], UINT32_C(0x3F530DA8));
	ng += check_number32("test_genf32 [101]", result[1], UINT32_C(0x3EAD73FA));
	ng += check_number32("test_genf32 [102]", result[2], UINT32_C(0x3E9AF6D8));
	ng += check_number32("test_genf32 [103]", result[3], UINT32_C(0x3F4EFCA1));

	return ng;
}

int test_genf64() {
	int ng = 0;
	multfly_key key = multfly_init(NULL, 0, 0);
	double buf[4];
	uint64_t result[4];

	multfly_genf64(&key, 0, 0, buf);
	memcpy(result, buf, 32);
	ng += check_number64("test_genf64 [0]", result[0], UINT64_C(0x3FEBFB684DF35CDD));
	ng += check_number64("test_genf64 [1]", result[1], UINT64_C(0x3FD82EEF3748DC62));
	ng += check_number64("test_genf64 [2]", result[2], UINT64_C(0x3FE254806363B6F7));
	ng += check_number64("test_genf64 [3]", result[3], UINT64_C(0x3FE6F7760B177374));

	multfly_genf64(&key, 100, 0, buf);
	memcpy(result, buf, 32);
	ng += check_number64("test_genf64 [100]", result[0], UINT64_C(0x3FDE9E2397B4C36A));
	ng += check_number64("test_genf64 [101]", result[1], UINT64_C(0x3FED6A7CC06AD73F));
	ng += check_number64("test_genf64 [102]", result[2], UINT64_C(0x3FE869C9D2A9AF6D));
	ng += check_number64("test_genf64 [103]", result[3], UINT64_C(0x3FE9F6BEE1D9DF94));

	return ng;
}

int main() {
	int ng = 0;
	ng += test_init();
	ng += test_gen32();
	ng += test_gen64();
	ng += test_genf32();
	ng += test_genf64();

	if (ng == 0) {
		puts("All tests succeeded.");
	} else {
		printf("%d tests failed.\n", ng);
	}
	return ng != 0;
}
