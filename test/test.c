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
	ng += check_number("test_derive_zero [0]", key.k[0], UINT32_C(0xCE7E87D9));
	ng += check_number("test_derive_zero [1]", key.k[1], UINT32_C(0xA31FFB1B));
	ng += check_number("test_derive_zero [2]", key.k[2], UINT32_C(0x6F562E4D));
	ng += check_number("test_derive_zero [3]", key.k[3], UINT32_C(0x3684A3AB));
	ng += check_number("test_derive_zero [4]", key.k[4], UINT32_C(0xC30E842C));
	ng += check_number("test_derive_zero [5]", key.k[5], UINT32_C(0x3B7F9ACE));
	ng += check_number("test_derive_zero [6]", key.k[6], UINT32_C(0x88E11B18));
	ng += check_number("test_derive_zero [7]", key.k[7], UINT32_C(0x1E1A71EF));
	return ng;
}

int test_gen_first() {
	multfly_key key = {0};
	uint64_t global_seed = 0;
	uint64_t global_ctr = 0;
	key = multfly_derive(&key, global_seed, global_ctr);
	uint32_t result[8];
	multfly_gen(&key, 0, result, 8);

	int ng = 0;
	ng += check_number("test_gen_first [0]", result[0], UINT32_C(0x5DD1F6C8));
	ng += check_number("test_gen_first [1]", result[1], UINT32_C(0x864710EF));
	ng += check_number("test_gen_first [2]", result[2], UINT32_C(0x1A8033E1));
	ng += check_number("test_gen_first [3]", result[3], UINT32_C(0x4AF0CB98));
	ng += check_number("test_gen_first [4]", result[4], UINT32_C(0x6771F377));
	ng += check_number("test_gen_first [5]", result[5], UINT32_C(0x1A5BE782));
	ng += check_number("test_gen_first [6]", result[6], UINT32_C(0xF073227A));
	ng += check_number("test_gen_first [7]", result[7], UINT32_C(0x085DF090));
	return ng;
}

int main() {
	int ng = 0;
	ng += test_derive_zero();
	ng += test_gen_first();

	if (ng == 0) {
		puts("All tests succeeded.");
	} else {
		printf("%d tests failed.\n", ng);
	}
	return ng != 0;
}
