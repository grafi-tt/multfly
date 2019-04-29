#include <emmintrin.h>
#include <smmintrin.h>
#include <tmmintrin.h>

#include <multfly.h>

static void multfly_chacha_round(__m128i *a, __m128i *b, __m128i *c, __m128i *d) {
	__m128i rotl8_shufvec = _mm_set_epi8(
			0x0E, 0x0D, 0x0C, 0x0F, 0x0A, 0x09, 0x08, 0x0B, 0x06, 0x05, 0x04, 0x07, 0x02, 0x01, 0x00, 0x03);
	__m128i rotl16_shufvec = _mm_set_epi8(
			0x0D, 0x0C, 0x0F, 0x0E, 0x09, 0x08, 0x0B, 0x0A, 0x05, 0x04, 0x07, 0x06, 0x01, 0x00, 0x03, 0x02);

	*a = _mm_add_epi32(*a, *b);
	*d = _mm_xor_si128(*d, *a);
	*d = _mm_shuffle_epi8(*d, rotl16_shufvec);
	*c = _mm_add_epi32(*c, *d);
	*b = _mm_xor_si128(*b, *c);
	*b = _mm_or_si128(_mm_slli_epi32(*b, 12), _mm_srli_epi32(*b, 20));
	*a = _mm_add_epi32(*a, *b);
	*d = _mm_xor_si128(*d, *a);
	*d = _mm_shuffle_epi8(*d, rotl8_shufvec);
	*c = _mm_add_epi32(*c, *d);
	*b = _mm_xor_si128(*b, *c);
	*b = _mm_or_si128(_mm_slli_epi32(*b, 7), _mm_srli_epi32(*b, 25));
}

static void multfly_chacha8_permute(__m128i *a, __m128i *b, __m128i *c, __m128i *d) {
	for (int i = 0; i < 8; i += 2) {
		multfly_chacha_round(a, b, c, d);
		*b = _mm_shuffle_epi32(*b, 0x39);
		*c = _mm_shuffle_epi32(*c, 0x4E);
		*d = _mm_shuffle_epi32(*d, 0x93);
		multfly_chacha_round(a, b, c, d);
		*b = _mm_shuffle_epi32(*b, 0x93);
		*c = _mm_shuffle_epi32(*c, 0x4E);
		*d = _mm_shuffle_epi32(*d, 0x39);
	}
}

static multfly_key multfly_derive_impl(const multfly_key *key, uint64_t global_seed, uint64_t global_ctr, multfly_key *splitkey) {
	__m128i a = _mm_set_epi32(UINT32_C(0x6B206574), UINT32_C(0x79622D32), UINT32_C(0x3320646E), UINT32_C(0x61707865));
	__m128i b = _mm_loadu_si128((const __m128i *)&key->k[0]);
	__m128i c = _mm_loadu_si128((const __m128i *)&key->k[4]);
	__m128i d = _mm_set_epi64x(global_seed, global_ctr);

	multfly_chacha8_permute(&a, &b, &c, &d);

	if (splitkey != 0) {
		_mm_storeu_si128((__m128i *)&splitkey->k[0], a);
		_mm_storeu_si128((__m128i *)&splitkey->k[4], b);
	}

	multfly_key newkey;
	_mm_storeu_si128((__m128i *)&newkey.k[0], c);
	_mm_storeu_si128((__m128i *)&newkey.k[4], d);
	return newkey;
}

multfly_key multfly_derive(const multfly_key *key, uint64_t global_seed, uint64_t global_ctr) {
	return multfly_derive_impl(key, global_seed, global_ctr, 0);
}

multfly_key multfly_split(multfly_key *key) {
	return multfly_derive_impl(key, 0, 0, key);
}

static void multfly_gen_round(__m128i *u, __m128i *v) {
	__m128i mulu = _mm_set1_epi32(UINT32_C(2718281829));
	__m128i mulv = _mm_set1_epi32(UINT32_C(3141592653));
	__m128i incr = _mm_set_epi32(UINT32_C(0x9891A2B1), UINT32_C(0xCC48d158), UINT32_C(0x662468AC), UINT32_C(0x33123456));

	__m128i rotl8_shufvec = _mm_set_epi8(
			0x0E, 0x0D, 0x0C, 0x0F, 0x0A, 0x09, 0x08, 0x0B, 0x06, 0x05, 0x04, 0x07, 0x02, 0x01, 0x00, 0x03);
	__m128i rotl16_shufvec = _mm_set_epi8(
			0x0D, 0x0C, 0x0F, 0x0E, 0x09, 0x08, 0x0B, 0x0A, 0x05, 0x04, 0x07, 0x06, 0x01, 0x00, 0x03, 0x02);

	*u = _mm_add_epi32(*u, incr);
	*v = _mm_add_epi32(*v, *u);
	*v = _mm_xor_si128(*v, _mm_shuffle_epi8(*u, rotl8_shufvec));
	*v = _mm_mullo_epi32(*v, mulv);
	*u = _mm_xor_si128(*u, _mm_slli_epi32(*v, 9));
	*u = _mm_xor_si128(*u, _mm_srli_epi32(*v, 23));
	*u = _mm_add_epi32(*u, *v);
	*u = _mm_mullo_epi32(*u, mulu);
	*v = _mm_xor_si128(*v, _mm_shuffle_epi8(*u, rotl16_shufvec));
	*v = _mm_add_epi32(*v, *u);
}

static void multfly_gen_impl(const multfly_key *key, uint64_t ctr, uint32_t result[4]) {
	__m128i offset = _mm_set_epi32(3, 2, 1, 0);
	__m128i ctru = _mm_set1_epi32((uint32_t)ctr);
	__m128i ctrv = _mm_set1_epi32((uint32_t)(ctr >> 32));
	ctru = _mm_add_epi32(ctru, offset);
	ctrv = _mm_add_epi32(ctrv, offset);

	__m128i u = _mm_loadu_si128((const __m128i *)&key->k[0]);
	__m128i v = _mm_loadu_si128((const __m128i *)&key->k[0] + 1);
	u = _mm_xor_si128(u, ctru);
	u = _mm_xor_si128(u, ctrv);

	multfly_gen_round(&u, &v);
	v = _mm_shuffle_epi32(v, 0xB1);
	multfly_gen_round(&u, &v);
	v = _mm_shuffle_epi32(v, 0x4E);
	multfly_gen_round(&u, &v);

	_mm_storeu_si128((__m128i *)result, v);
}

static void multfly_gen_loop(const multfly_key *key, uint64_t ctr, uint32_t *array, uint64_t len) {
	for (uint64_t i = 0; i < len; i += 4, ctr += 4) {
		multfly_gen_impl(key, ctr, array + i);
	}
}

void multfly_gen(const multfly_key *key, uint64_t ctr, uint32_t *array, uint64_t len) {
	ctr &= -4;
	len &= -4;

	multfly_gen_loop(key, ctr, array, len);
}

void multfly_gen128(const multfly_key *key, uint64_t ctr, uint32_t *array[4], uint64_t len) {
	ctr &= -4;
	len &= -4;

	multfly_gen_loop(key, ctr + 0, array[0], len);
	multfly_gen_loop(key, ctr + 1, array[1], len);
	multfly_gen_loop(key, ctr + 2, array[2], len);
	multfly_gen_loop(key, ctr + 3, array[3], len);
}
