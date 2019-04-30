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

	multfly_gen32(&key, 0, result);
	ng += check_number32("test_gen32 [0]", result[0], UINT32_C(0xB85CBB0E));
	ng += check_number32("test_gen32 [1]", result[1], UINT32_C(0x032FED01));
	ng += check_number32("test_gen32 [2]", result[2], UINT32_C(0x9D32B642));
	ng += check_number32("test_gen32 [3]", result[3], UINT32_C(0x99DD97BC));

	multfly_gen32(&key, 100, result);
	ng += check_number32("test_gen32 [100]", result[0], UINT32_C(0xB6F887BA));
	ng += check_number32("test_gen32 [101]", result[1], UINT32_C(0xCA489F22));
	ng += check_number32("test_gen32 [102]", result[2], UINT32_C(0xC048CAEF));
	ng += check_number32("test_gen32 [103]", result[3], UINT32_C(0x2D723783));

	return ng;
}

int test_gen32x4() {
	int ng = 0;
	multfly_key key = multfly_init(NULL, 0, 0);
	uint32_t result0[4];
	uint32_t result1[4];
	uint32_t result2[4];
	uint32_t result3[4];

	multfly_gen32x4(&key, 0, result0, result1, result2, result3);
	ng += check_number32("test_gen32x4 [0][0]", result0[0], UINT32_C(0xB85CBB0E));
	ng += check_number32("test_gen32x4 [0][1]", result0[1], UINT32_C(0x032FED01));
	ng += check_number32("test_gen32x4 [0][2]", result0[2], UINT32_C(0x9D32B642));
	ng += check_number32("test_gen32x4 [0][3]", result0[3], UINT32_C(0x99DD97BC));
	ng += check_number32("test_gen32x4 [1][0]", result1[0], UINT32_C(0x557E2BD0));
	ng += check_number32("test_gen32x4 [1][1]", result1[1], UINT32_C(0xF2B4ABB9));
	ng += check_number32("test_gen32x4 [1][2]", result1[2], UINT32_C(0x1947D90C));
	ng += check_number32("test_gen32x4 [1][3]", result1[3], UINT32_C(0xDD2EBD67));
	ng += check_number32("test_gen32x4 [2][0]", result2[0], UINT32_C(0x5A74F659));
	ng += check_number32("test_gen32x4 [2][1]", result2[1], UINT32_C(0xFCEEAE2D));
	ng += check_number32("test_gen32x4 [2][2]", result2[2], UINT32_C(0x9709A6FD));
	ng += check_number32("test_gen32x4 [2][3]", result2[3], UINT32_C(0x39A1B7B5));
	ng += check_number32("test_gen32x4 [3][0]", result3[0], UINT32_C(0xE415CEAB));
	ng += check_number32("test_gen32x4 [3][1]", result3[1], UINT32_C(0xFA73D217));
	ng += check_number32("test_gen32x4 [3][2]", result3[2], UINT32_C(0xFDEF4CBD));
	ng += check_number32("test_gen32x4 [3][3]", result3[3], UINT32_C(0xB59E5249));

	multfly_gen32x4(&key, 100, result0, result1, result2, result3);
	ng += check_number32("test_gen32x4 [0][100]", result0[0], UINT32_C(0xB6F887BA));
	ng += check_number32("test_gen32x4 [0][101]", result0[1], UINT32_C(0xCA489F22));
	ng += check_number32("test_gen32x4 [0][102]", result0[2], UINT32_C(0xC048CAEF));
	ng += check_number32("test_gen32x4 [0][103]", result0[3], UINT32_C(0x2D723783));
	ng += check_number32("test_gen32x4 [1][100]", result1[0], UINT32_C(0x19B89713));
	ng += check_number32("test_gen32x4 [1][101]", result1[1], UINT32_C(0xB6ACD472));
	ng += check_number32("test_gen32x4 [1][102]", result1[2], UINT32_C(0x16445CA5));
	ng += check_number32("test_gen32x4 [1][103]", result1[3], UINT32_C(0x1868DFEB));
	ng += check_number32("test_gen32x4 [2][100]", result2[0], UINT32_C(0xEB794173));
	ng += check_number32("test_gen32x4 [2][101]", result2[1], UINT32_C(0xC318E511));
	ng += check_number32("test_gen32x4 [2][102]", result2[2], UINT32_C(0x8F65E5BC));
	ng += check_number32("test_gen32x4 [2][103]", result2[3], UINT32_C(0x66CE67F6));
	ng += check_number32("test_gen32x4 [3][100]", result3[0], UINT32_C(0x33F8DFF6));
	ng += check_number32("test_gen32x4 [3][101]", result3[1], UINT32_C(0x9835F76C));
	ng += check_number32("test_gen32x4 [3][102]", result3[2], UINT32_C(0xD8A44034));
	ng += check_number32("test_gen32x4 [3][103]", result3[3], UINT32_C(0x44ECEB29));

	return ng;
}

int test_gen64() {
	int ng = 0;
	multfly_key key = multfly_init(NULL, 0, 0);
	uint64_t result[4];

	multfly_gen64(&key, 0, result);
	ng += check_number64("test_gen64 [0]", result[0], UINT64_C(0x557E2BD0B85CBB0E));
	ng += check_number64("test_gen64 [1]", result[1], UINT64_C(0xF2B4ABB9032FED01));
	ng += check_number64("test_gen64 [2]", result[2], UINT64_C(0x1947D90C9D32B642));
	ng += check_number64("test_gen64 [3]", result[3], UINT64_C(0xDD2EBD6799DD97BC));

	multfly_gen64(&key, 100, result);
	ng += check_number64("test_gen64 [100]", result[0], UINT64_C(0x19B89713B6F887BA));
	ng += check_number64("test_gen64 [101]", result[1], UINT64_C(0xB6ACD472CA489F22));
	ng += check_number64("test_gen64 [102]", result[2], UINT64_C(0x16445CA5C048CAEF));
	ng += check_number64("test_gen64 [103]", result[3], UINT64_C(0x1868DFEB2D723783));

	return ng;
}

int test_genf32() {
	int ng = 0;
	multfly_key key = multfly_init(NULL, 0, 0);
	float buf[4];
	uint32_t result[4];

	multfly_genf32(&key, 0, buf);
	memcpy(result, buf, 16);
	ng += check_number32("test_genf32 [0]", result[0], UINT32_C(0x3F385CBB));
	ng += check_number32("test_genf32 [1]", result[1], UINT32_C(0x3C4BFB40));
	ng += check_number32("test_genf32 [2]", result[2], UINT32_C(0x3F1D32B6));
	ng += check_number32("test_genf32 [3]", result[3], UINT32_C(0x3F19DD97));

	multfly_genf32(&key, 100, buf);
	memcpy(result, buf, 16);
	ng += check_number32("test_genf32 [100]", result[0], UINT32_C(0x3F36F887));
	ng += check_number32("test_genf32 [101]", result[1], UINT32_C(0x3F4A489F));
	ng += check_number32("test_genf32 [102]", result[2], UINT32_C(0x3F4048CA));
	ng += check_number32("test_genf32 [103]", result[3], UINT32_C(0x3E35C8DC));

	return ng;
}

int test_genf64() {
	int ng = 0;
	multfly_key key = multfly_init(NULL, 0, 0);
	double buf[4];
	uint64_t result[4];

	multfly_genf64(&key, 0, buf);
	memcpy(result, buf, 32);
	ng += check_number64("test_genf64 [0]", result[0], UINT64_C(0x3FD55F8AF42E172E));
	ng += check_number64("test_genf64 [1]", result[1], UINT64_C(0x3FEE5695772065FD));
	ng += check_number64("test_genf64 [2]", result[2], UINT64_C(0x3FB947D90C9D32B0));
	ng += check_number64("test_genf64 [3]", result[3], UINT64_C(0x3FEBA5D7ACF33BB2));

	multfly_genf64(&key, 100, buf);
	memcpy(result, buf, 32);
	ng += check_number64("test_genf64 [100]", result[0], UINT64_C(0x3FB9B89713B6F880));
	ng += check_number64("test_genf64 [101]", result[1], UINT64_C(0x3FE6D59A8E594913));
	ng += check_number64("test_genf64 [102]", result[2], UINT64_C(0x3FB6445CA5C048C8));
	ng += check_number64("test_genf64 [103]", result[3], UINT64_C(0x3FB868DFEB2D7230));

	return ng;
}

int main() {
	int ng = 0;
	ng += test_init();
	ng += test_gen32();
	ng += test_gen32x4();
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
