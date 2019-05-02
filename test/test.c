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
	ng += check_number32("test_init 0, 0, 0 [0]", key0.v_[0], UINT32_C(0x7AB5532A));
	ng += check_number32("test_init 0, 0, 0 [1]", key0.v_[1], UINT32_C(0x89FC21C2));
	ng += check_number32("test_init 0, 0, 0 [2]", key0.v_[2], UINT32_C(0xD14CC927));
	ng += check_number32("test_init 0, 0, 0 [3]", key0.v_[3], UINT32_C(0x0D62FE75));
	ng += check_number32("test_init 0, 0, 0 [4]", key0.v_[4], UINT32_C(0xA5FEF694));
	ng += check_number32("test_init 0, 0, 0 [5]", key0.v_[5], UINT32_C(0xC251C80D));
	ng += check_number32("test_init 0, 0, 0 [6]", key0.v_[6], UINT32_C(0x5BBCBFED));
	ng += check_number32("test_init 0, 0, 0 [7]", key0.v_[7], UINT32_C(0xA71572DF));

	multfly_ident ident0 = {0};
	key0 = multfly_init(&ident0, 0, 0);
	ng += check_number32("test_init 0, 0, 0 [0]", key0.v_[0], UINT32_C(0x7AB5532A));
	ng += check_number32("test_init 0, 0, 0 [1]", key0.v_[1], UINT32_C(0x89FC21C2));
	ng += check_number32("test_init 0, 0, 0 [2]", key0.v_[2], UINT32_C(0xD14CC927));
	ng += check_number32("test_init 0, 0, 0 [3]", key0.v_[3], UINT32_C(0x0D62FE75));
	ng += check_number32("test_init 0, 0, 0 [4]", key0.v_[4], UINT32_C(0xA5FEF694));
	ng += check_number32("test_init 0, 0, 0 [5]", key0.v_[5], UINT32_C(0xC251C80D));
	ng += check_number32("test_init 0, 0, 0 [6]", key0.v_[6], UINT32_C(0x5BBCBFED));
	ng += check_number32("test_init 0, 0, 0 [7]", key0.v_[7], UINT32_C(0xA71572DF));

	multfly_key key0s42 = multfly_init(NULL, 42, 0);
	ng += check_number32("test_init 0, 42, 0 [0]", key0s42.v_[0], UINT32_C(0x845A343C));
	ng += check_number32("test_init 0, 42, 0 [1]", key0s42.v_[1], UINT32_C(0xB1F6244C));
	ng += check_number32("test_init 0, 42, 0 [2]", key0s42.v_[2], UINT32_C(0xB5518592));
	ng += check_number32("test_init 0, 42, 0 [3]", key0s42.v_[3], UINT32_C(0xB00AB9A6));
	ng += check_number32("test_init 0, 42, 0 [4]", key0s42.v_[4], UINT32_C(0x13F5E5CB));
	ng += check_number32("test_init 0, 42, 0 [5]", key0s42.v_[5], UINT32_C(0xD0E4BCCC));
	ng += check_number32("test_init 0, 42, 0 [6]", key0s42.v_[6], UINT32_C(0xDA9602BE));
	ng += check_number32("test_init 0, 42, 0 [7]", key0s42.v_[7], UINT32_C(0x8BA93664));

	multfly_key key0c42 = multfly_init(NULL, 0, 42);
	ng += check_number32("test_init 0, 0, 42 [0]", key0c42.v_[0], UINT32_C(0xD9C981C1));
	ng += check_number32("test_init 0, 0, 42 [1]", key0c42.v_[1], UINT32_C(0xF76078DB));
	ng += check_number32("test_init 0, 0, 42 [2]", key0c42.v_[2], UINT32_C(0xED3A0675));
	ng += check_number32("test_init 0, 0, 42 [3]", key0c42.v_[3], UINT32_C(0xD3C29F7E));
	ng += check_number32("test_init 0, 0, 42 [4]", key0c42.v_[4], UINT32_C(0x0494B964));
	ng += check_number32("test_init 0, 0, 42 [5]", key0c42.v_[5], UINT32_C(0x9639713B));
	ng += check_number32("test_init 0, 0, 42 [6]", key0c42.v_[6], UINT32_C(0x0907851C));
	ng += check_number32("test_init 0, 0, 42 [7]", key0c42.v_[7], UINT32_C(0x4494B507));

	multfly_ident ident42z = {{42, 0, 0, 0, 0, 0, 0, 0}};
	multfly_key key42z = multfly_init(&ident42z, 0, 0);
	ng += check_number32("test_init {42, 0...}, 0, 0 [0]", key42z.v_[0], UINT32_C(0xAFD828DE));
	ng += check_number32("test_init {42, 0...}, 0, 0 [1]", key42z.v_[1], UINT32_C(0xF48E42F3));
	ng += check_number32("test_init {42, 0...}, 0, 0 [2]", key42z.v_[2], UINT32_C(0xD8BB5DFD));
	ng += check_number32("test_init {42, 0...}, 0, 0 [3]", key42z.v_[3], UINT32_C(0x55D874C9));
	ng += check_number32("test_init {42, 0...}, 0, 0 [4]", key42z.v_[4], UINT32_C(0x039C1065));
	ng += check_number32("test_init {42, 0...}, 0, 0 [5]", key42z.v_[5], UINT32_C(0x9AFDABCA));
	ng += check_number32("test_init {42, 0...}, 0, 0 [6]", key42z.v_[6], UINT32_C(0x4A148931));
	ng += check_number32("test_init {42, 0...}, 0, 0 [7]", key42z.v_[7], UINT32_C(0x490662EA));

	multfly_ident identz42 = {{0, 0, 0, 0, 0, 0, 0, 42}};
	multfly_key keyz42 = multfly_init(&identz42, 0, 0);
	ng += check_number32("test_init {0..., 42}, 0, 0 [0]", keyz42.v_[0], UINT32_C(0x70B9D537));
	ng += check_number32("test_init {0..., 42}, 0, 0 [1]", keyz42.v_[1], UINT32_C(0xACFFBA4C));
	ng += check_number32("test_init {0..., 42}, 0, 0 [2]", keyz42.v_[2], UINT32_C(0x7FBAF7C8));
	ng += check_number32("test_init {0..., 42}, 0, 0 [3]", keyz42.v_[3], UINT32_C(0x767B398F));
	ng += check_number32("test_init {0..., 42}, 0, 0 [4]", keyz42.v_[4], UINT32_C(0xE2A72552));
	ng += check_number32("test_init {0..., 42}, 0, 0 [5]", keyz42.v_[5], UINT32_C(0x4837F7EC));
	ng += check_number32("test_init {0..., 42}, 0, 0 [6]", keyz42.v_[6], UINT32_C(0x10B3E1E8));
	ng += check_number32("test_init {0..., 42}, 0, 0 [7]", keyz42.v_[7], UINT32_C(0x11001F4E));

	return ng;
}

int test_gen32() {
	int ng = 0;
	multfly_key key = multfly_init(NULL, 0, 0);
	uint32_t result[4];

	multfly_gen32(&key, 0, 0, result);
	ng += check_number32("test_gen32 [0]", result[0], UINT32_C(0x020269D1));
	ng += check_number32("test_gen32 [1]", result[1], UINT32_C(0x8BF8906F));
	ng += check_number32("test_gen32 [2]", result[2], UINT32_C(0x7F3D6574));
	ng += check_number32("test_gen32 [3]", result[3], UINT32_C(0xEB777B12));

	multfly_gen32(&key, 100, 0, result);
	ng += check_number32("test_gen32 [100]", result[0], UINT32_C(0x0C620D2A));
	ng += check_number32("test_gen32 [101]", result[1], UINT32_C(0x881462E9));
	ng += check_number32("test_gen32 [102]", result[2], UINT32_C(0x76E4DAEB));
	ng += check_number32("test_gen32 [103]", result[3], UINT32_C(0x3C6E6CC5));

	return ng;
}

int test_gen64() {
	int ng = 0;
	multfly_key key = multfly_init(NULL, 0, 0);
	uint64_t result[4];

	multfly_gen64(&key, 0, 0, result);
	ng += check_number64("test_gen64 [0]", result[0], UINT64_C(0xCCB2D11F020269D1));
	ng += check_number64("test_gen64 [1]", result[1], UINT64_C(0x291095538BF8906F));
	ng += check_number64("test_gen64 [2]", result[2], UINT64_C(0x4FF589D57F3D6574));
	ng += check_number64("test_gen64 [3]", result[3], UINT64_C(0x1D09C35AEB777B12));

	multfly_gen64(&key, 100, 0, result);
	ng += check_number64("test_gen64 [100]", result[0], UINT64_C(0x9EAD6FE40C620D2A));
	ng += check_number64("test_gen64 [101]", result[1], UINT64_C(0xA0CB1D27881462E9));
	ng += check_number64("test_gen64 [102]", result[2], UINT64_C(0x69AE8CEA76E4DAEB));
	ng += check_number64("test_gen64 [103]", result[3], UINT64_C(0xFE057E9A3C6E6CC5));

	return ng;
}

int test_genf32() {
	int ng = 0;
	multfly_key key = multfly_init(NULL, 0, 0);
	float buf[4];
	uint32_t result[4];

	multfly_genf32(&key, 0, 0, buf);
	memcpy(result, buf, 16);
	ng += check_number32("test_genf32 [0]", result[0], UINT32_C(0x3C009A40));
	ng += check_number32("test_genf32 [1]", result[1], UINT32_C(0x3F0BF890));
	ng += check_number32("test_genf32 [2]", result[2], UINT32_C(0x3EFE7ACA));
	ng += check_number32("test_genf32 [3]", result[3], UINT32_C(0x3F6B777B));

	multfly_genf32(&key, 100, 0, buf);
	memcpy(result, buf, 16);
	ng += check_number32("test_genf32 [100]", result[0], UINT32_C(0x3D4620D0));
	ng += check_number32("test_genf32 [101]", result[1], UINT32_C(0x3F081462));
	ng += check_number32("test_genf32 [102]", result[2], UINT32_C(0x3EEDC9B4));
	ng += check_number32("test_genf32 [103]", result[3], UINT32_C(0x3E71B9B0));

	return ng;
}

int test_genf64() {
	int ng = 0;
	multfly_key key = multfly_init(NULL, 0, 0);
	double buf[4];
	uint64_t result[4];

	multfly_genf64(&key, 0, 0, buf);
	memcpy(result, buf, 32);
	ng += check_number64("test_genf64 [0]", result[0], UINT64_C(0x3FE9965A23E0404D));
	ng += check_number64("test_genf64 [1]", result[1], UINT64_C(0x3FC4884AA9C5FC48));
	ng += check_number64("test_genf64 [2]", result[2], UINT64_C(0x3FD3FD62755FCF58));
	ng += check_number64("test_genf64 [3]", result[3], UINT64_C(0x3FBD09C35AEB7778));

	multfly_genf64(&key, 100, 0, buf);
	memcpy(result, buf, 32);
	ng += check_number64("test_genf64 [100]", result[0], UINT64_C(0x3FE3D5ADFC818C41));
	ng += check_number64("test_genf64 [101]", result[1], UINT64_C(0x3FE41963A4F1028C));
	ng += check_number64("test_genf64 [102]", result[2], UINT64_C(0x3FDA6BA33A9DB936));
	ng += check_number64("test_genf64 [103]", result[3], UINT64_C(0x3FEFC0AFD3478DCD));

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
