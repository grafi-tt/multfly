#include <inttypes.h>
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

int test_derive() {
	int ng = 0;

	multfly_key key0 = {0};
	key0 = multfly_derive(&key0, 0, 0);
	ng += check_number32("test_derive 0, 0, 0 [0]", key0.k[0], UINT32_C(0x91697D71));
	ng += check_number32("test_derive 0, 0, 0 [1]", key0.k[1], UINT32_C(0xCE9CA8CC));
	ng += check_number32("test_derive 0, 0, 0 [2]", key0.k[2], UINT32_C(0x3C633376));
	ng += check_number32("test_derive 0, 0, 0 [3]", key0.k[3], UINT32_C(0x611D7EC8));
	ng += check_number32("test_derive 0, 0, 0 [4]", key0.k[4], UINT32_C(0x4D1EFF6B));
	ng += check_number32("test_derive 0, 0, 0 [5]", key0.k[5], UINT32_C(0xD99BB1FE));
	ng += check_number32("test_derive 0, 0, 0 [6]", key0.k[6], UINT32_C(0x5B949C8E));
	ng += check_number32("test_derive 0, 0, 0 [7]", key0.k[7], UINT32_C(0x2903C1EE));

	multfly_key key0s42 = {0};
	key0s42 = multfly_derive(&key0s42, 42, 0);
	ng += check_number32("test_derive 0, 42, 0 [0]", key0s42.k[0], UINT32_C(0x926C3A4A));
	ng += check_number32("test_derive 0, 42, 0 [1]", key0s42.k[1], UINT32_C(0x47196DB4));
	ng += check_number32("test_derive 0, 42, 0 [2]", key0s42.k[2], UINT32_C(0x08CE7C1D));
	ng += check_number32("test_derive 0, 42, 0 [3]", key0s42.k[3], UINT32_C(0x6FD65C04));
	ng += check_number32("test_derive 0, 42, 0 [4]", key0s42.k[4], UINT32_C(0x3F86A67E));
	ng += check_number32("test_derive 0, 42, 0 [5]", key0s42.k[5], UINT32_C(0x9A959FF1));
	ng += check_number32("test_derive 0, 42, 0 [6]", key0s42.k[6], UINT32_C(0xDF30FF78));
	ng += check_number32("test_derive 0, 42, 0 [7]", key0s42.k[7], UINT32_C(0xBEE765F3));

	multfly_key key0c42 = {0};
	key0c42 = multfly_derive(&key0c42, 0, 42);
	ng += check_number32("test_derive 0, 0, 42 [0]", key0c42.k[0], UINT32_C(0xE62831AE));
	ng += check_number32("test_derive 0, 0, 42 [1]", key0c42.k[1], UINT32_C(0x3BD3A4F6));
	ng += check_number32("test_derive 0, 0, 42 [2]", key0c42.k[2], UINT32_C(0xAC915C09));
	ng += check_number32("test_derive 0, 0, 42 [3]", key0c42.k[3], UINT32_C(0x2E56733A));
	ng += check_number32("test_derive 0, 0, 42 [4]", key0c42.k[4], UINT32_C(0xAA2581F8));
	ng += check_number32("test_derive 0, 0, 42 [5]", key0c42.k[5], UINT32_C(0x3863CC20));
	ng += check_number32("test_derive 0, 0, 42 [6]", key0c42.k[6], UINT32_C(0xD31AC060));
	ng += check_number32("test_derive 0, 0, 42 [7]", key0c42.k[7], UINT32_C(0x925DFDA2));

	multfly_key key42z = {{42, 0, 0, 0, 0, 0, 0, 0}};
	key42z = multfly_derive(&key42z, 0, 0);
	ng += check_number32("test_derive {42, 0...}, 0, 0 [0]", key42z.k[0], UINT32_C(0x41673B29));
	ng += check_number32("test_derive {42, 0...}, 0, 0 [1]", key42z.k[1], UINT32_C(0x0E6A7922));
	ng += check_number32("test_derive {42, 0...}, 0, 0 [2]", key42z.k[2], UINT32_C(0x8EE4EB55));
	ng += check_number32("test_derive {42, 0...}, 0, 0 [3]", key42z.k[3], UINT32_C(0xD5C33D29));
	ng += check_number32("test_derive {42, 0...}, 0, 0 [4]", key42z.k[4], UINT32_C(0xE7CE936C));
	ng += check_number32("test_derive {42, 0...}, 0, 0 [5]", key42z.k[5], UINT32_C(0x6171A23B));
	ng += check_number32("test_derive {42, 0...}, 0, 0 [6]", key42z.k[6], UINT32_C(0x6FBBB413));
	ng += check_number32("test_derive {42, 0...}, 0, 0 [7]", key42z.k[7], UINT32_C(0xCCF58062));

	multfly_key keyz42 = {{0, 0, 0, 0, 0, 0, 0, 42}};
	keyz42 = multfly_derive(&keyz42, 0, 0);
	ng += check_number32("test_derive {0..., 42}, 0, 0 [0]", keyz42.k[0], UINT32_C(0x3FA7CF25));
	ng += check_number32("test_derive {0..., 42}, 0, 0 [1]", keyz42.k[1], UINT32_C(0x18647A4E));
	ng += check_number32("test_derive {0..., 42}, 0, 0 [2]", keyz42.k[2], UINT32_C(0xF344291A));
	ng += check_number32("test_derive {0..., 42}, 0, 0 [3]", keyz42.k[3], UINT32_C(0x2C48E2AB));
	ng += check_number32("test_derive {0..., 42}, 0, 0 [4]", keyz42.k[4], UINT32_C(0x2C4A7C75));
	ng += check_number32("test_derive {0..., 42}, 0, 0 [5]", keyz42.k[5], UINT32_C(0xE3F2EFF1));
	ng += check_number32("test_derive {0..., 42}, 0, 0 [6]", keyz42.k[6], UINT32_C(0xEFD54B97));
	ng += check_number32("test_derive {0..., 42}, 0, 0 [7]", keyz42.k[7], UINT32_C(0x9F55DFA4));

	return ng;
}

int test_gen32() {
	multfly_key key = {0};
	uint64_t global_seed = 0;
	uint64_t global_ctr = 0;
	key = multfly_derive(&key, global_seed, global_ctr);

	int ng = 0;
	uint32_t result[8];

	multfly_gen32(&key, 0, result, 8);
	ng += check_number32("test_gen32 [0]", result[0], UINT32_C(0xB85CBB0E));
	ng += check_number32("test_gen32 [1]", result[1], UINT32_C(0x032FED01));
	ng += check_number32("test_gen32 [2]", result[2], UINT32_C(0x9D32B642));
	ng += check_number32("test_gen32 [3]", result[3], UINT32_C(0x99DD97BC));
	ng += check_number32("test_gen32 [4]", result[4], UINT32_C(0xFF1DF474));
	ng += check_number32("test_gen32 [5]", result[5], UINT32_C(0x2E13540C));
	ng += check_number32("test_gen32 [6]", result[6], UINT32_C(0xE578B4D5));
	ng += check_number32("test_gen32 [7]", result[7], UINT32_C(0xCB498370));

	multfly_gen32(&key, 100, result, 8);
	ng += check_number32("test_gen32 [100]", result[0], UINT32_C(0xB6F887BA));
	ng += check_number32("test_gen32 [101]", result[1], UINT32_C(0xCA489F22));
	ng += check_number32("test_gen32 [102]", result[2], UINT32_C(0xC048CAEF));
	ng += check_number32("test_gen32 [103]", result[3], UINT32_C(0x2D723783));
	ng += check_number32("test_gen32 [104]", result[4], UINT32_C(0x33E4162E));
	ng += check_number32("test_gen32 [105]", result[5], UINT32_C(0xA84B4156));
	ng += check_number32("test_gen32 [106]", result[6], UINT32_C(0x0F0AEA9B));
	ng += check_number32("test_gen32 [107]", result[7], UINT32_C(0x9D8B5594));

	return ng;
}

int test_gen32x4() {
	multfly_key key = {0};
	uint64_t global_seed = 0;
	uint64_t global_ctr = 0;
	key = multfly_derive(&key, global_seed, global_ctr);

	int ng = 0;
	uint32_t result0[8];
	uint32_t result1[8];
	uint32_t result2[8];
	uint32_t result3[8];

	multfly_gen32x4(&key, 0, result0, result1, result2, result3, 8);
	ng += check_number32("test_gen32x4 [0][0]", result0[0], UINT32_C(0xB85CBB0E));
	ng += check_number32("test_gen32x4 [0][1]", result0[1], UINT32_C(0x032FED01));
	ng += check_number32("test_gen32x4 [0][2]", result0[2], UINT32_C(0x9D32B642));
	ng += check_number32("test_gen32x4 [0][3]", result0[3], UINT32_C(0x99DD97BC));
	ng += check_number32("test_gen32x4 [0][4]", result0[4], UINT32_C(0xFF1DF474));
	ng += check_number32("test_gen32x4 [0][5]", result0[5], UINT32_C(0x2E13540C));
	ng += check_number32("test_gen32x4 [0][6]", result0[6], UINT32_C(0xE578B4D5));
	ng += check_number32("test_gen32x4 [0][7]", result0[7], UINT32_C(0xCB498370));
	ng += check_number32("test_gen32x4 [1][0]", result1[0], UINT32_C(0x557E2BD0));
	ng += check_number32("test_gen32x4 [1][1]", result1[1], UINT32_C(0xF2B4ABB9));
	ng += check_number32("test_gen32x4 [1][2]", result1[2], UINT32_C(0x1947D90C));
	ng += check_number32("test_gen32x4 [1][3]", result1[3], UINT32_C(0xDD2EBD67));
	ng += check_number32("test_gen32x4 [1][4]", result1[4], UINT32_C(0x0F194956));
	ng += check_number32("test_gen32x4 [1][5]", result1[5], UINT32_C(0xC9E9BA98));
	ng += check_number32("test_gen32x4 [1][6]", result1[6], UINT32_C(0x7AA2F53F));
	ng += check_number32("test_gen32x4 [1][7]", result1[7], UINT32_C(0x4E795F22));
	ng += check_number32("test_gen32x4 [2][0]", result2[0], UINT32_C(0x5A74F659));
	ng += check_number32("test_gen32x4 [2][1]", result2[1], UINT32_C(0xFCEEAE2D));
	ng += check_number32("test_gen32x4 [2][2]", result2[2], UINT32_C(0x9709A6FD));
	ng += check_number32("test_gen32x4 [2][3]", result2[3], UINT32_C(0x39A1B7B5));
	ng += check_number32("test_gen32x4 [2][4]", result2[4], UINT32_C(0xD753F2D1));
	ng += check_number32("test_gen32x4 [2][5]", result2[5], UINT32_C(0xD3062C2A));
	ng += check_number32("test_gen32x4 [2][6]", result2[6], UINT32_C(0x69BB3651));
	ng += check_number32("test_gen32x4 [2][7]", result2[7], UINT32_C(0xED240ED7));
	ng += check_number32("test_gen32x4 [3][0]", result3[0], UINT32_C(0xE415CEAB));
	ng += check_number32("test_gen32x4 [3][1]", result3[1], UINT32_C(0xFA73D217));
	ng += check_number32("test_gen32x4 [3][2]", result3[2], UINT32_C(0xFDEF4CBD));
	ng += check_number32("test_gen32x4 [3][3]", result3[3], UINT32_C(0xB59E5249));
	ng += check_number32("test_gen32x4 [3][4]", result3[4], UINT32_C(0x75E4B414));
	ng += check_number32("test_gen32x4 [3][5]", result3[5], UINT32_C(0x497DA16D));
	ng += check_number32("test_gen32x4 [3][6]", result3[6], UINT32_C(0x14FC6D4E));
	ng += check_number32("test_gen32x4 [3][7]", result3[7], UINT32_C(0x26CF8588));

	multfly_gen32x4(&key, 100, result0, result1, result2, result3, 8);
	ng += check_number32("test_gen32x4 [0][100]", result0[0], UINT32_C(0xB6F887BA));
	ng += check_number32("test_gen32x4 [0][101]", result0[1], UINT32_C(0xCA489F22));
	ng += check_number32("test_gen32x4 [0][102]", result0[2], UINT32_C(0xC048CAEF));
	ng += check_number32("test_gen32x4 [0][103]", result0[3], UINT32_C(0x2D723783));
	ng += check_number32("test_gen32x4 [0][104]", result0[4], UINT32_C(0x33E4162E));
	ng += check_number32("test_gen32x4 [0][105]", result0[5], UINT32_C(0xA84B4156));
	ng += check_number32("test_gen32x4 [0][106]", result0[6], UINT32_C(0x0F0AEA9B));
	ng += check_number32("test_gen32x4 [0][107]", result0[7], UINT32_C(0x9D8B5594));
	ng += check_number32("test_gen32x4 [1][100]", result1[0], UINT32_C(0x19B89713));
	ng += check_number32("test_gen32x4 [1][101]", result1[1], UINT32_C(0xB6ACD472));
	ng += check_number32("test_gen32x4 [1][102]", result1[2], UINT32_C(0x16445CA5));
	ng += check_number32("test_gen32x4 [1][103]", result1[3], UINT32_C(0x1868DFEB));
	ng += check_number32("test_gen32x4 [1][104]", result1[4], UINT32_C(0x9580D52E));
	ng += check_number32("test_gen32x4 [1][105]", result1[5], UINT32_C(0x90500066));
	ng += check_number32("test_gen32x4 [1][106]", result1[6], UINT32_C(0x065FD41C));
	ng += check_number32("test_gen32x4 [1][107]", result1[7], UINT32_C(0xE54C9A96));
	ng += check_number32("test_gen32x4 [2][100]", result2[0], UINT32_C(0xEB794173));
	ng += check_number32("test_gen32x4 [2][101]", result2[1], UINT32_C(0xC318E511));
	ng += check_number32("test_gen32x4 [2][102]", result2[2], UINT32_C(0x8F65E5BC));
	ng += check_number32("test_gen32x4 [2][103]", result2[3], UINT32_C(0x66CE67F6));
	ng += check_number32("test_gen32x4 [2][104]", result2[4], UINT32_C(0x2DCB1FA7));
	ng += check_number32("test_gen32x4 [2][105]", result2[5], UINT32_C(0xD9E6B414));
	ng += check_number32("test_gen32x4 [2][106]", result2[6], UINT32_C(0x773F6A7A));
	ng += check_number32("test_gen32x4 [2][107]", result2[7], UINT32_C(0x0A5218B6));
	ng += check_number32("test_gen32x4 [3][100]", result3[0], UINT32_C(0x33F8DFF6));
	ng += check_number32("test_gen32x4 [3][101]", result3[1], UINT32_C(0x9835F76C));
	ng += check_number32("test_gen32x4 [3][102]", result3[2], UINT32_C(0xD8A44034));
	ng += check_number32("test_gen32x4 [3][103]", result3[3], UINT32_C(0x44ECEB29));
	ng += check_number32("test_gen32x4 [3][104]", result3[4], UINT32_C(0x50FAB6B3));
	ng += check_number32("test_gen32x4 [3][105]", result3[5], UINT32_C(0x863EA149));
	ng += check_number32("test_gen32x4 [3][106]", result3[6], UINT32_C(0xD66B00BC));
	ng += check_number32("test_gen32x4 [3][107]", result3[7], UINT32_C(0x73E4B4BF));

	return ng;
}

int test_gen64() {
	multfly_key key = {0};
	uint64_t global_seed = 0;
	uint64_t global_ctr = 0;
	key = multfly_derive(&key, global_seed, global_ctr);

	int ng = 0;
	uint64_t result[8];

	multfly_gen64(&key, 0, result, 8);
	ng += check_number64("test_gen64 [0]", result[0], UINT64_C(0x557E2BD0B85CBB0E));
	ng += check_number64("test_gen64 [1]", result[1], UINT64_C(0xF2B4ABB9032FED01));
	ng += check_number64("test_gen64 [2]", result[2], UINT64_C(0x1947D90C9D32B642));
	ng += check_number64("test_gen64 [3]", result[3], UINT64_C(0xDD2EBD6799DD97BC));
	ng += check_number64("test_gen64 [4]", result[4], UINT64_C(0x0F194956FF1DF474));
	ng += check_number64("test_gen64 [5]", result[5], UINT64_C(0xC9E9BA982E13540C));
	ng += check_number64("test_gen64 [6]", result[6], UINT64_C(0x7AA2F53FE578B4D5));
	ng += check_number64("test_gen64 [7]", result[7], UINT64_C(0x4E795F22CB498370));

	multfly_gen64(&key, 100, result, 8);
	ng += check_number64("test_gen64 [100]", result[0], UINT64_C(0x19B89713B6F887BA));
	ng += check_number64("test_gen64 [101]", result[1], UINT64_C(0xB6ACD472CA489F22));
	ng += check_number64("test_gen64 [102]", result[2], UINT64_C(0x16445CA5C048CAEF));
	ng += check_number64("test_gen64 [103]", result[3], UINT64_C(0x1868DFEB2D723783));
	ng += check_number64("test_gen64 [104]", result[4], UINT64_C(0x9580D52E33E4162E));
	ng += check_number64("test_gen64 [105]", result[5], UINT64_C(0x90500066A84B4156));
	ng += check_number64("test_gen64 [106]", result[6], UINT64_C(0x065FD41C0F0AEA9B));
	ng += check_number64("test_gen64 [107]", result[7], UINT64_C(0xE54C9A969D8B5594));

	return ng;
}

int test_genf32() {
	multfly_key key = {0};
	uint64_t global_seed = 0;
	uint64_t global_ctr = 0;
	key = multfly_derive(&key, global_seed, global_ctr);

	int ng = 0;
	float buf[8];
	uint32_t result[8];

	multfly_genf32(&key, 0, buf, 8);
	memcpy(result, buf, 32);
	ng += check_number32("test_genf32 [0]", result[0], UINT32_C(0x3F385CBB));
	ng += check_number32("test_genf32 [1]", result[1], UINT32_C(0x3C4BFB40));
	ng += check_number32("test_genf32 [2]", result[2], UINT32_C(0x3F1D32B6));
	ng += check_number32("test_genf32 [3]", result[3], UINT32_C(0x3F19DD97));
	ng += check_number32("test_genf32 [4]", result[4], UINT32_C(0x3F7F1DF4));
	ng += check_number32("test_genf32 [5]", result[5], UINT32_C(0x3E384D50));
	ng += check_number32("test_genf32 [6]", result[6], UINT32_C(0x3F6578B4));
	ng += check_number32("test_genf32 [7]", result[7], UINT32_C(0x3F4B4983));

	multfly_genf32(&key, 100, buf, 8);
	memcpy(result, buf, 32);
	ng += check_number32("test_genf32 [100]", result[0], UINT32_C(0x3F36F887));
	ng += check_number32("test_genf32 [101]", result[1], UINT32_C(0x3F4A489F));
	ng += check_number32("test_genf32 [102]", result[2], UINT32_C(0x3F4048CA));
	ng += check_number32("test_genf32 [103]", result[3], UINT32_C(0x3E35C8DC));
	ng += check_number32("test_genf32 [104]", result[4], UINT32_C(0x3E4F9058));
	ng += check_number32("test_genf32 [105]", result[5], UINT32_C(0x3F284B41));
	ng += check_number32("test_genf32 [106]", result[6], UINT32_C(0x3D70AEA0));
	ng += check_number32("test_genf32 [107]", result[7], UINT32_C(0x3F1D8B55));

	return ng;
}

int test_genf64() {
	multfly_key key = {0};
	uint64_t global_seed = 0;
	uint64_t global_ctr = 0;
	key = multfly_derive(&key, global_seed, global_ctr);

	int ng = 0;
	double buf[8];
	uint64_t result[8];

	multfly_genf64(&key, 0, buf, 8);
	memcpy(result, buf, 64);
	ng += check_number64("test_genf64 [0]", result[0], UINT64_C(0x3FD55F8AF42E172E));
	ng += check_number64("test_genf64 [1]", result[1], UINT64_C(0x3FEE5695772065FD));
	ng += check_number64("test_genf64 [2]", result[2], UINT64_C(0x3FB947D90C9D32B0));
	ng += check_number64("test_genf64 [3]", result[3], UINT64_C(0x3FEBA5D7ACF33BB2));
	ng += check_number64("test_genf64 [4]", result[4], UINT64_C(0x3FAE3292ADFE3BE0));
	ng += check_number64("test_genf64 [5]", result[5], UINT64_C(0x3FE93D375305C26A));
	ng += check_number64("test_genf64 [6]", result[6], UINT64_C(0x3FDEA8BD4FF95E2C));
	ng += check_number64("test_genf64 [7]", result[7], UINT64_C(0x3FD39E57C8B2D260));

	multfly_genf64(&key, 100, buf, 8);
	memcpy(result, buf, 64);
	ng += check_number64("test_genf64 [100]", result[0], UINT64_C(0x3FB9B89713B6F880));
	ng += check_number64("test_genf64 [101]", result[1], UINT64_C(0x3FE6D59A8E594913));
	ng += check_number64("test_genf64 [102]", result[2], UINT64_C(0x3FB6445CA5C048C8));
	ng += check_number64("test_genf64 [103]", result[3], UINT64_C(0x3FB868DFEB2D7230));
	ng += check_number64("test_genf64 [104]", result[4], UINT64_C(0x3FE2B01AA5C67C82));
	ng += check_number64("test_genf64 [105]", result[5], UINT64_C(0x3FE20A000CD50968));
	ng += check_number64("test_genf64 [106]", result[6], UINT64_C(0x3F997F50703C2BA0));
	ng += check_number64("test_genf64 [107]", result[7], UINT64_C(0x3FECA99352D3B16A));

	return ng;
}

int main() {
	int ng = 0;
	ng += test_derive();
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
