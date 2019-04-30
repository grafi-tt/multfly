#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#ifdef SSE41
#include <multfly_sse41.h>
#else
#include <multfly_ref.h>
#endif

int check_number32(char *title, uint32_t result, uint32_t expected) {
	int ng;
	puts(title);
	if (result == expected) {
		puts("ok");
		ng = 0;
	} else {
		printf("Result: 0x%08"PRIX32", Expected: 0x%08"PRIX32"\n", result, expected);
		ng = 1;
	}
	puts("");
	return ng;
}

int check_number64(char *title, uint64_t result, uint64_t expected) {
	int ng;
	puts(title);
	if (result == expected) {
		puts("ok");
		ng = 0;
	} else {
		printf("Result: 0x%016"PRIX64", Expected: 0x%016"PRIX64"\n", result, expected);
		ng = 1;
	}
	puts("");
	return ng;
}

int test_derive_zero() {
	multfly_key key = {0};
	uint64_t global_seed = 0;
	uint64_t global_ctr = 0;
	key = multfly_derive(&key, global_seed, global_ctr);

	int ng = 0;
	ng += check_number32("test_derive_zero [0]", key.k[0], UINT32_C(0x91697D71));
	ng += check_number32("test_derive_zero [1]", key.k[1], UINT32_C(0xCE9CA8CC));
	ng += check_number32("test_derive_zero [2]", key.k[2], UINT32_C(0x3C633376));
	ng += check_number32("test_derive_zero [3]", key.k[3], UINT32_C(0x611D7EC8));
	ng += check_number32("test_derive_zero [4]", key.k[4], UINT32_C(0x4D1EFF6B));
	ng += check_number32("test_derive_zero [5]", key.k[5], UINT32_C(0xD99BB1FE));
	ng += check_number32("test_derive_zero [6]", key.k[6], UINT32_C(0x5B949C8E));
	ng += check_number32("test_derive_zero [7]", key.k[7], UINT32_C(0x2903C1EE));
	return ng;
}

int test_gen32_ctr0() {
	multfly_key key = {0};
	uint64_t global_seed = 0;
	uint64_t global_ctr = 0;
	key = multfly_derive(&key, global_seed, global_ctr);
	uint32_t result[8];
	multfly_gen32(&key, 0, result, 8);

	int ng = 0;
	ng += check_number32("test_gen32_ctr0 [0]", result[0], UINT32_C(0xB85CBB0E));
	ng += check_number32("test_gen32_ctr0 [1]", result[1], UINT32_C(0x032FED01));
	ng += check_number32("test_gen32_ctr0 [2]", result[2], UINT32_C(0x9D32B642));
	ng += check_number32("test_gen32_ctr0 [3]", result[3], UINT32_C(0x99DD97BC));
	ng += check_number32("test_gen32_ctr0 [4]", result[4], UINT32_C(0xFF1DF474));
	ng += check_number32("test_gen32_ctr0 [5]", result[5], UINT32_C(0x2E13540C));
	ng += check_number32("test_gen32_ctr0 [6]", result[6], UINT32_C(0xE578B4D5));
	ng += check_number32("test_gen32_ctr0 [7]", result[7], UINT32_C(0xCB498370));
	return ng;
}

int test_gen64_ctr0() {
	multfly_key key = {0};
	uint64_t global_seed = 0;
	uint64_t global_ctr = 0;
	key = multfly_derive(&key, global_seed, global_ctr);
	uint64_t result[8];
	multfly_gen64(&key, 0, result, 8);

	int ng = 0;
	ng += check_number64("test_gen64_ctr0 [0]", result[0], UINT64_C(0x557E2BD0B85CBB0E));
	ng += check_number64("test_gen64_ctr0 [1]", result[1], UINT64_C(0xF2B4ABB9032FED01));
	ng += check_number64("test_gen64_ctr0 [2]", result[2], UINT64_C(0x1947D90C9D32B642));
	ng += check_number64("test_gen64_ctr0 [3]", result[3], UINT64_C(0xDD2EBD6799DD97BC));
	ng += check_number64("test_gen64_ctr0 [4]", result[4], UINT64_C(0x0F194956FF1DF474));
	ng += check_number64("test_gen64_ctr0 [5]", result[5], UINT64_C(0xC9E9BA982E13540C));
	ng += check_number64("test_gen64_ctr0 [6]", result[6], UINT64_C(0x7AA2F53FE578B4D5));
	ng += check_number64("test_gen64_ctr0 [7]", result[7], UINT64_C(0x4E795F22CB498370));
	return ng;
}

int test_genf32_ctr0() {
	multfly_key key = {0};
	uint64_t global_seed = 0;
	uint64_t global_ctr = 0;
	key = multfly_derive(&key, global_seed, global_ctr);
	float tmp[8];
	multfly_genf32(&key, 0, tmp, 8);
	uint32_t result[8];
	memcpy(result, tmp, 32);

	int ng = 0;
	ng += check_number32("test_genf32_ctr0 [0]", result[0], UINT32_C(0x3F385CBB));
	ng += check_number32("test_genf32_ctr0 [1]", result[1], UINT32_C(0x3C4BFB40));
	ng += check_number32("test_genf32_ctr0 [2]", result[2], UINT32_C(0x3F1D32B6));
	ng += check_number32("test_genf32_ctr0 [3]", result[3], UINT32_C(0x3F19DD97));
	ng += check_number32("test_genf32_ctr0 [4]", result[4], UINT32_C(0x3F7F1DF4));
	ng += check_number32("test_genf32_ctr0 [5]", result[5], UINT32_C(0x3E384D50));
	ng += check_number32("test_genf32_ctr0 [6]", result[6], UINT32_C(0x3F6578B4));
	ng += check_number32("test_genf32_ctr0 [7]", result[7], UINT32_C(0x3F4B4983));
	return ng;
}

int test_genf64_ctr0() {
	multfly_key key = {0};
	uint64_t global_seed = 0;
	uint64_t global_ctr = 0;
	key = multfly_derive(&key, global_seed, global_ctr);
	double tmp[8];
	multfly_genf64(&key, 0, tmp, 8);
	uint64_t result[8];
	memcpy(result, tmp, 64);

	int ng = 0;
	ng += check_number64("test_genf64_ctr0 [0]", result[0], UINT64_C(0x3FD55F8AF42E172E));
	ng += check_number64("test_genf64_ctr0 [1]", result[1], UINT64_C(0x3FEE5695772065FD));
	ng += check_number64("test_genf64_ctr0 [2]", result[2], UINT64_C(0x3FB947D90C9D32B0));
	ng += check_number64("test_genf64_ctr0 [3]", result[3], UINT64_C(0x3FEBA5D7ACF33BB2));
	ng += check_number64("test_genf64_ctr0 [4]", result[4], UINT64_C(0x3FAE3292ADFE3BE0));
	ng += check_number64("test_genf64_ctr0 [5]", result[5], UINT64_C(0x3FE93D375305C26A));
	ng += check_number64("test_genf64_ctr0 [6]", result[6], UINT64_C(0x3FDEA8BD4FF95E2C));
	ng += check_number64("test_genf64_ctr0 [7]", result[7], UINT64_C(0x3FD39E57C8B2D260));
	return ng;
}

int main() {
	int ng = 0;
	ng += test_derive_zero();
	ng += test_gen32_ctr0();
	ng += test_gen64_ctr0();
	ng += test_genf32_ctr0();
	ng += test_genf64_ctr0();

	if (ng == 0) {
		puts("All tests succeeded.");
	} else {
		printf("%d tests failed.\n", ng);
	}
	return ng != 0;
}
