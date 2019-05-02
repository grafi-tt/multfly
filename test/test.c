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
	ng += check_number32("test_init 0, 0, 0 [0]", key0.v_[0], UINT32_C(0x5A7F760C));
	ng += check_number32("test_init 0, 0, 0 [1]", key0.v_[1], UINT32_C(0xE3ABAB0B));
	ng += check_number32("test_init 0, 0, 0 [2]", key0.v_[2], UINT32_C(0xECBCFCC9));
	ng += check_number32("test_init 0, 0, 0 [3]", key0.v_[3], UINT32_C(0x7CD8982B));
	ng += check_number32("test_init 0, 0, 0 [4]", key0.v_[4], UINT32_C(0xE428B5C8));
	ng += check_number32("test_init 0, 0, 0 [5]", key0.v_[5], UINT32_C(0xA1C1A56E));
	ng += check_number32("test_init 0, 0, 0 [6]", key0.v_[6], UINT32_C(0x0A66CADF));
	ng += check_number32("test_init 0, 0, 0 [7]", key0.v_[7], UINT32_C(0x16926033));

	multfly_key key0s42 = multfly_init(NULL, 42, 0);
	ng += check_number32("test_init 0, 42, 0 [0]", key0s42.v_[0], UINT32_C(0x837DEF60));
	ng += check_number32("test_init 0, 42, 0 [1]", key0s42.v_[1], UINT32_C(0x91EB7E46));
	ng += check_number32("test_init 0, 42, 0 [2]", key0s42.v_[2], UINT32_C(0x7DFE6454));
	ng += check_number32("test_init 0, 42, 0 [3]", key0s42.v_[3], UINT32_C(0xC8521F87));
	ng += check_number32("test_init 0, 42, 0 [4]", key0s42.v_[4], UINT32_C(0x4406CFDC));
	ng += check_number32("test_init 0, 42, 0 [5]", key0s42.v_[5], UINT32_C(0x8BD6B0DA));
	ng += check_number32("test_init 0, 42, 0 [6]", key0s42.v_[6], UINT32_C(0x82CB0B0C));
	ng += check_number32("test_init 0, 42, 0 [7]", key0s42.v_[7], UINT32_C(0xC017D37D));

	multfly_key key0c42 = multfly_init(NULL, 0, 42);
	ng += check_number32("test_init 0, 0, 42 [0]", key0c42.v_[0], UINT32_C(0x1D1D3930));
	ng += check_number32("test_init 0, 0, 42 [1]", key0c42.v_[1], UINT32_C(0x0F50F5F7));
	ng += check_number32("test_init 0, 0, 42 [2]", key0c42.v_[2], UINT32_C(0x79974889));
	ng += check_number32("test_init 0, 0, 42 [3]", key0c42.v_[3], UINT32_C(0x7C2BB2D3));
	ng += check_number32("test_init 0, 0, 42 [4]", key0c42.v_[4], UINT32_C(0xD3B290C1));
	ng += check_number32("test_init 0, 0, 42 [5]", key0c42.v_[5], UINT32_C(0x5D91717E));
	ng += check_number32("test_init 0, 0, 42 [6]", key0c42.v_[6], UINT32_C(0xCABC0AF1));
	ng += check_number32("test_init 0, 0, 42 [7]", key0c42.v_[7], UINT32_C(0xD1EB06C3));

	multfly_ident ident0 = {0};
	multfly_key keyz = multfly_init(&ident0, 0, 0);
	ng += check_number32("test_init {0}, 0, 0 [0]", keyz.v_[0], key0.v_[0]);
	ng += check_number32("test_init {0}, 0, 0 [0]", keyz.v_[1], key0.v_[1]);
	ng += check_number32("test_init {0}, 0, 0 [0]", keyz.v_[2], key0.v_[2]);
	ng += check_number32("test_init {0}, 0, 0 [0]", keyz.v_[3], key0.v_[3]);
	ng += check_number32("test_init {0}, 0, 0 [0]", keyz.v_[4], key0.v_[4]);
	ng += check_number32("test_init {0}, 0, 0 [0]", keyz.v_[5], key0.v_[5]);
	ng += check_number32("test_init {0}, 0, 0 [0]", keyz.v_[6], key0.v_[6]);
	ng += check_number32("test_init {0}, 0, 0 [0]", keyz.v_[7], key0.v_[7]);

	multfly_ident ident42z = {{42, 0, 0, 0, 0, 0, 0, 0}};
	multfly_key key42z = multfly_init(&ident42z, 0, 0);
	ng += check_number32("test_init {42, 0...}, 0, 0 [0]", key42z.v_[0], UINT32_C(0x96A14253));
	ng += check_number32("test_init {42, 0...}, 0, 0 [1]", key42z.v_[1], UINT32_C(0x57B8E697));
	ng += check_number32("test_init {42, 0...}, 0, 0 [2]", key42z.v_[2], UINT32_C(0xA27F6D34));
	ng += check_number32("test_init {42, 0...}, 0, 0 [3]", key42z.v_[3], UINT32_C(0x0809D9A3));
	ng += check_number32("test_init {42, 0...}, 0, 0 [4]", key42z.v_[4], UINT32_C(0xAD9DB6A7));
	ng += check_number32("test_init {42, 0...}, 0, 0 [5]", key42z.v_[5], UINT32_C(0x4ED9E9E6));
	ng += check_number32("test_init {42, 0...}, 0, 0 [6]", key42z.v_[6], UINT32_C(0x38B92312));
	ng += check_number32("test_init {42, 0...}, 0, 0 [7]", key42z.v_[7], UINT32_C(0x724588B5));

	multfly_ident identz42 = {{0, 0, 0, 0, 0, 0, 0, 42}};
	multfly_key keyz42 = multfly_init(&identz42, 0, 0);
	ng += check_number32("test_init {0..., 42}, 0, 0 [0]", keyz42.v_[0], UINT32_C(0x9419BF95));
	ng += check_number32("test_init {0..., 42}, 0, 0 [1]", keyz42.v_[1], UINT32_C(0x8CE202D4));
	ng += check_number32("test_init {0..., 42}, 0, 0 [2]", keyz42.v_[2], UINT32_C(0xF2C911B4));
	ng += check_number32("test_init {0..., 42}, 0, 0 [3]", keyz42.v_[3], UINT32_C(0x93A3BB37));
	ng += check_number32("test_init {0..., 42}, 0, 0 [4]", keyz42.v_[4], UINT32_C(0x88A5FE1B));
	ng += check_number32("test_init {0..., 42}, 0, 0 [5]", keyz42.v_[5], UINT32_C(0xC8181A07));
	ng += check_number32("test_init {0..., 42}, 0, 0 [6]", keyz42.v_[6], UINT32_C(0x9824A0C8));
	ng += check_number32("test_init {0..., 42}, 0, 0 [7]", keyz42.v_[7], UINT32_C(0x6BAD7B83));

	return ng;
}

int test_gen32() {
	int ng = 0;
	multfly_key key = multfly_init(NULL, 0, 0);
	uint32_t result[4];

	multfly_gen32(&key, 0, 0, result);
	ng += check_number32("test_gen32 [0]", result[0], UINT32_C(0x19654FAC));
	ng += check_number32("test_gen32 [1]", result[1], UINT32_C(0xDBCD9879));
	ng += check_number32("test_gen32 [2]", result[2], UINT32_C(0x8AE67409));
	ng += check_number32("test_gen32 [3]", result[3], UINT32_C(0xC11CFA8A));

	multfly_gen32(&key, 100, 0, result);
	ng += check_number32("test_gen32 [100]", result[0], UINT32_C(0xA7CFA710));
	ng += check_number32("test_gen32 [101]", result[1], UINT32_C(0xCABB6031));
	ng += check_number32("test_gen32 [102]", result[2], UINT32_C(0x1F710068));
	ng += check_number32("test_gen32 [103]", result[3], UINT32_C(0x21EB7B9E));

	return ng;
}

int test_gen64() {
	int ng = 0;
	multfly_key key = multfly_init(NULL, 0, 0);
	uint64_t result[4];

	multfly_gen64(&key, 0, 0, result);
	ng += check_number64("test_gen64 [0]", result[0], UINT64_C(0x98A55BED19654FAC));
	ng += check_number64("test_gen64 [1]", result[1], UINT64_C(0x6EBC4103DBCD9879));
	ng += check_number64("test_gen64 [2]", result[2], UINT64_C(0x3C10CE098AE67409));
	ng += check_number64("test_gen64 [3]", result[3], UINT64_C(0xB19D51F0C11CFA8A));

	multfly_gen64(&key, 100, 0, result);
	ng += check_number64("test_gen64 [100]", result[0], UINT64_C(0x39A8B1DBA7CFA710));
	ng += check_number64("test_gen64 [101]", result[1], UINT64_C(0x25F08229CABB6031));
	ng += check_number64("test_gen64 [102]", result[2], UINT64_C(0x9AF900EB1F710068));
	ng += check_number64("test_gen64 [103]", result[3], UINT64_C(0x6FA3902B21EB7B9E));

	return ng;
}

int test_genf32() {
	int ng = 0;
	multfly_key key = multfly_init(NULL, 0, 0);
	float buf[4];
	uint32_t result[4];

	multfly_genf32(&key, 0, 0, buf);
	memcpy(result, buf, 16);
	ng += check_number32("test_genf32 [0]", result[0], UINT32_C(0x3DCB2A78));
	ng += check_number32("test_genf32 [1]", result[1], UINT32_C(0x3F5BCD98));
	ng += check_number32("test_genf32 [2]", result[2], UINT32_C(0x3F0AE674));
	ng += check_number32("test_genf32 [3]", result[3], UINT32_C(0x3F411CFA));

	multfly_genf32(&key, 100, 0, buf);
	memcpy(result, buf, 16);
	ng += check_number32("test_genf32 [100]", result[0], UINT32_C(0x3F27CFA7));
	ng += check_number32("test_genf32 [101]", result[1], UINT32_C(0x3F4ABB60));
	ng += check_number32("test_genf32 [102]", result[2], UINT32_C(0x3DFB8800));
	ng += check_number32("test_genf32 [103]", result[3], UINT32_C(0x3E07ADEC));

	return ng;
}

int test_genf64() {
	int ng = 0;
	multfly_key key = multfly_init(NULL, 0, 0);
	double buf[4];
	uint64_t result[4];

	multfly_genf64(&key, 0, 0, buf);
	memcpy(result, buf, 32);
	ng += check_number64("test_genf64 [0]", result[0], UINT64_C(0x3FE314AB7DA32CA9));
	ng += check_number64("test_genf64 [1]", result[1], UINT64_C(0x3FDBAF1040F6F366));
	ng += check_number64("test_genf64 [2]", result[2], UINT64_C(0x3FCE086704C57338));
	ng += check_number64("test_genf64 [3]", result[3], UINT64_C(0x3FE633AA3E18239F));

	multfly_genf64(&key, 100, 0, buf);
	memcpy(result, buf, 32);
	ng += check_number64("test_genf64 [100]", result[0], UINT64_C(0x3FCCD458EDD3E7D0));
	ng += check_number64("test_genf64 [101]", result[1], UINT64_C(0x3FC2F84114E55DB0));
	ng += check_number64("test_genf64 [102]", result[2], UINT64_C(0x3FE35F201D63EE20));
	ng += check_number64("test_genf64 [103]", result[3], UINT64_C(0x3FDBE8E40AC87ADE));

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
