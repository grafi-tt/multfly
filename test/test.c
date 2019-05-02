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

	multfly_key keyn = multfly_init(NULL, 0, 0);
	ng += check_number32("test_init NULL, 0, 0 [0]", keyn.v_[0], UINT32_C(0x5A7F760C));
	ng += check_number32("test_init NULL, 0, 0 [1]", keyn.v_[1], UINT32_C(0xE3ABAB0B));
	ng += check_number32("test_init NULL, 0, 0 [2]", keyn.v_[2], UINT32_C(0xECBCFCC9));
	ng += check_number32("test_init NULL, 0, 0 [3]", keyn.v_[3], UINT32_C(0x7CD8982B));
	ng += check_number32("test_init NULL, 0, 0 [4]", keyn.v_[4], UINT32_C(0xE428B5C8));
	ng += check_number32("test_init NULL, 0, 0 [5]", keyn.v_[5], UINT32_C(0xA1C1A56E));
	ng += check_number32("test_init NULL, 0, 0 [6]", keyn.v_[6], UINT32_C(0x0A66CADF));
	ng += check_number32("test_init NULL, 0, 0 [7]", keyn.v_[7], UINT32_C(0x16926033));

	multfly_ident ident0 = {0};
	multfly_key key0 = multfly_init(&ident0, 0, 0);
	ng += check_number32("test_init {0}, 0, 0 [0]", key0.v_[0], keyn.v_[0]);
	ng += check_number32("test_init {0}, 0, 0 [1]", key0.v_[1], keyn.v_[1]);
	ng += check_number32("test_init {0}, 0, 0 [2]", key0.v_[2], keyn.v_[2]);
	ng += check_number32("test_init {0}, 0, 0 [3]", key0.v_[3], keyn.v_[3]);
	ng += check_number32("test_init {0}, 0, 0 [4]", key0.v_[4], keyn.v_[4]);
	ng += check_number32("test_init {0}, 0, 0 [5]", key0.v_[5], keyn.v_[5]);
	ng += check_number32("test_init {0}, 0, 0 [6]", key0.v_[6], keyn.v_[6]);
	ng += check_number32("test_init {0}, 0, 0 [7]", key0.v_[7], keyn.v_[7]);

	multfly_key key = multfly_init_by_literal("abcdefghijklmnopqrstuvwxyz123456", 78, 90);
	ng += check_number32("test_init abcdefghijklmnopqrstuvwxyz123456, 78, 90 [0]", key.v_[0], UINT32_C(0x1FCE68FA));
	ng += check_number32("test_init abcdefghijklmnopqrstuvwxyz123456, 78, 90 [1]", key.v_[1], UINT32_C(0x01D594AD));
	ng += check_number32("test_init abcdefghijklmnopqrstuvwxyz123456, 78, 90 [2]", key.v_[2], UINT32_C(0x8FEC7E65));
	ng += check_number32("test_init abcdefghijklmnopqrstuvwxyz123456, 78, 90 [3]", key.v_[3], UINT32_C(0x0FFC302B));
	ng += check_number32("test_init abcdefghijklmnopqrstuvwxyz123456, 78, 90 [4]", key.v_[4], UINT32_C(0x0201AC2E));
	ng += check_number32("test_init abcdefghijklmnopqrstuvwxyz123456, 78, 90 [5]", key.v_[5], UINT32_C(0x5DE5F28B));
	ng += check_number32("test_init abcdefghijklmnopqrstuvwxyz123456, 78, 90 [6]", key.v_[6], UINT32_C(0xD44262CC));
	ng += check_number32("test_init abcdefghijklmnopqrstuvwxyz123456, 78, 90 [7]", key.v_[7], UINT32_C(0xA0F8A154));

	return ng;
}

int test_gen32() {
	int ng = 0;
	multfly_key key = multfly_init(NULL, 0, 0);
	uint32_t result[4];

	multfly_gen32(&key, 0, 0, result);
	ng += check_number32("test_gen32 [0]", result[0], UINT32_C(0x28DAC85C));
	ng += check_number32("test_gen32 [1]", result[1], UINT32_C(0x0D05143D));
	ng += check_number32("test_gen32 [2]", result[2], UINT32_C(0xF13F16D2));
	ng += check_number32("test_gen32 [3]", result[3], UINT32_C(0x5790CBA7));

	multfly_gen32(&key, 100, 0, result);
	ng += check_number32("test_gen32 [100]", result[0], UINT32_C(0xAC05F32E));
	ng += check_number32("test_gen32 [101]", result[1], UINT32_C(0xCAF615DD));
	ng += check_number32("test_gen32 [102]", result[2], UINT32_C(0x81CCDA48));
	ng += check_number32("test_gen32 [103]", result[3], UINT32_C(0xDC5C5E1A));

	return ng;
}

int test_gen64() {
	int ng = 0;
	multfly_key key = multfly_init(NULL, 0, 0);
	uint64_t result[4];

	multfly_gen64(&key, 0, 0, result);
	ng += check_number64("test_gen64 [0]", result[0], UINT64_C(0x1BDCBA9328DAC85C));
	ng += check_number64("test_gen64 [1]", result[1], UINT64_C(0x65985DB70D05143D));
	ng += check_number64("test_gen64 [2]", result[2], UINT64_C(0x483C27C0F13F16D2));
	ng += check_number64("test_gen64 [3]", result[3], UINT64_C(0xE764123A5790CBA7));

	multfly_gen64(&key, 100, 0, result);
	ng += check_number64("test_gen64 [100]", result[0], UINT64_C(0xDD5542F5AC05F32E));
	ng += check_number64("test_gen64 [101]", result[1], UINT64_C(0x5A190C48CAF615DD));
	ng += check_number64("test_gen64 [102]", result[2], UINT64_C(0x9AE4C7C581CCDA48));
	ng += check_number64("test_gen64 [103]", result[3], UINT64_C(0x27A5E2FCDC5C5E1A));

	return ng;
}

int test_genf32() {
	int ng = 0;
	multfly_key key = multfly_init(NULL, 0, 0);

	uint32_t u32_result[4];
	float f32_result[4];
	for (int idx = 0; idx < 100; idx++) {
		multfly_gen32(&key, idx, 0, u32_result);
		multfly_genf32(&key, idx, 0, f32_result);
		for (int lane = 0; lane < 4; lane++) {
			if (f32_result[lane] != (float)(u32_result[lane] >> 8) * (float)(1.0 / (UINT32_C(1) << 24))) {
				ng += 1;
			}
		}
	}

	puts("f32 correctness");
	if (!ng) {
		puts("ok");
	} else {
		printf("%d f32 samples failed\n", ng);
	}
	return ng;
}

int test_genf64() {
	int ng = 0;
	multfly_key key = multfly_init(NULL, 0, 0);

	uint64_t u64_result[4];
	double f64_result[4];
	for (int idx = 0; idx < 100; idx++) {
		multfly_gen64(&key, idx, 0, u64_result);
		multfly_genf64(&key, idx, 0, f64_result);
		for (int lane = 0; lane < 4; lane++) {
			if (f64_result[lane] != (u64_result[lane] >> 11) * (1.0 / (UINT64_C(1) << 53))) {
				ng += 1;
			}
		}
	}

	puts("f64 correctness");
	if (!ng) {
		puts("ok");
	} else {
		printf("%d f64 samples failed\n", ng);
	}
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
