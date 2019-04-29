#include <inttypes.h>
#include <stdio.h>

#include <multfly.h>

int check_number(char *title, uint32_t result, uint32_t expected) {
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

int test_derive_zero() {
	multfly_key key = {0};
	uint64_t global_seed = 0;
	uint64_t global_ctr = 0;
	key = multfly_derive(&key, global_seed, global_ctr);

	int ng = 0;
	ng += check_number("test_derive_zero [0]", key.k[0], UINT32_C(0x72E14C98));
	ng += check_number("test_derive_zero [1]", key.k[1], UINT32_C(0x416F21B9));
	ng += check_number("test_derive_zero [2]", key.k[2], UINT32_C(0x6753449F));
	ng += check_number("test_derive_zero [3]", key.k[3], UINT32_C(0x19566D45));
	ng += check_number("test_derive_zero [4]", key.k[4], UINT32_C(0xA3424A31));
	ng += check_number("test_derive_zero [5]", key.k[5], UINT32_C(0x01B086DA));
	ng += check_number("test_derive_zero [6]", key.k[6], UINT32_C(0xB8FD7B38));
	ng += check_number("test_derive_zero [7]", key.k[7], UINT32_C(0x42FE0C0E));
	return ng;
}

int main() {
	int ng = 0;
	ng += test_derive_zero();
	if (ng == 0) {
		puts("All tests succeeded.");
	} else {
		printf("%d tests failed.\n", ng);
	}
	return ng != 0;
}
