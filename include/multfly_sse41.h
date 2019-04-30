#ifndef MULTFLY_MULTFLY_SSE41_H
#define MULTFLY_MULTFLY_SSE41_H

#include <stdint.h>

#include <emmintrin.h>
#include <smmintrin.h>
#include <tmmintrin.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MULTFLY_MULTFLY_KEY_STRUCT
#define MULTFLY_MULTFLY_KEY_STRUCT
typedef struct multfly_key_ {
	uint32_t k[8];
} multfly_key;
#endif

static inline multfly_key multfly_derive(const multfly_key *key, uint64_t global_seed, uint64_t global_ctr);
static inline void multfly_gen32(const multfly_key *key, uint64_t ctr, uint32_t *r, uint64_t len);
static inline void multfly_gen32x4(const multfly_key *key, uint64_t ctr, uint32_t *r0, uint32_t *r1, uint32_t *r2, uint32_t *r3, uint64_t len);
static inline void multfly_gen64(const multfly_key *key, uint64_t ctr, uint64_t *r, uint64_t len);
static inline void multfly_genf32(const multfly_key *key, uint64_t ctr, float *r, uint64_t len);
static inline void multfly_genf64(const multfly_key *key, uint64_t ctr, double *r, uint64_t len);

//
// impl
//
static inline void multfly_chacha_round_(__m128i *a, __m128i *b, __m128i *c, __m128i *d) {
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

static inline void multfly_chacha8_permute_(__m128i *a, __m128i *b, __m128i *c, __m128i *d) {
	for (int i = 0; i < 8; i += 2) {
		multfly_chacha_round_(a, b, c, d);
		*b = _mm_shuffle_epi32(*b, 0x39);
		*c = _mm_shuffle_epi32(*c, 0x4E);
		*d = _mm_shuffle_epi32(*d, 0x93);
		multfly_chacha_round_(a, b, c, d);
		*b = _mm_shuffle_epi32(*b, 0x93);
		*c = _mm_shuffle_epi32(*c, 0x4E);
		*d = _mm_shuffle_epi32(*d, 0x39);
	}
}

static inline multfly_key multfly_derive(const multfly_key *key, uint64_t global_seed, uint64_t global_ctr) {
	__m128i a = _mm_set_epi32(UINT32_C(0x9891A2B1), UINT32_C(0xCC48D158), UINT32_C(0x662468AC), UINT32_C(0x33123456));
	__m128i b = _mm_loadu_si128((const __m128i *)&key->k[0]);
	__m128i c = _mm_loadu_si128((const __m128i *)&key->k[4]);
	__m128i d = _mm_set_epi64x(global_seed, global_ctr);

	multfly_chacha8_permute_(&a, &b, &c, &d);

	multfly_key newkey;
	_mm_storeu_si128((__m128i *)&newkey.k[0], _mm_xor_si128(a, c));
	_mm_storeu_si128((__m128i *)&newkey.k[4], _mm_xor_si128(b, d));
	return newkey;
}

static inline void multfly_gen_round_(__m128i *u, __m128i *v) {
	__m128i mulu = _mm_set1_epi32(UINT32_C(2718281829));
	__m128i mulv = _mm_set1_epi32(UINT32_C(3141592653));
	__m128i incr = _mm_set_epi32(UINT32_C(0x9891A2B1), UINT32_C(0xCC48D158), UINT32_C(0x662468AC), UINT32_C(0x33123456));

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

static inline __m128i multfly_gen_impl_(const multfly_key *key, uint64_t ctr) {
	__m128i offset = _mm_set_epi32(3, 2, 1, 0);
	__m128i ctru = _mm_set1_epi32((uint32_t)ctr);
	__m128i ctrv = _mm_set1_epi32((uint32_t)(ctr >> 32));
	ctru = _mm_add_epi32(ctru, offset);
	ctrv = _mm_add_epi32(ctrv, offset);

	__m128i u = _mm_loadu_si128((const __m128i *)&key->k[0]);
	__m128i v = _mm_loadu_si128((const __m128i *)&key->k[4]);
	u = _mm_xor_si128(u, ctru);
	v = _mm_xor_si128(v, ctrv);

	multfly_gen_round_(&u, &v);
	v = _mm_shuffle_epi32(v, 0xB1);
	multfly_gen_round_(&u, &v);
	v = _mm_shuffle_epi32(v, 0x4E);
	multfly_gen_round_(&u, &v);

	return v;
}

static inline void multfly_gen32(const multfly_key *key, uint64_t ctr, uint32_t *r, uint64_t len) {
	ctr &= -4;
	len &= -4;
	for (uint64_t i = 0; i < len; i += 4, ctr += 4) {
		__m128i v = multfly_gen_impl_(key, ctr);
		_mm_storeu_si128((__m128i *)(r + i), v);
	}
}

static inline void multfly_gen32x4(const multfly_key *key, uint64_t ctr, uint32_t *r0, uint32_t *r1, uint32_t *r2, uint32_t *r3, uint64_t len) {
	ctr &= -4;
	len &= -4;
	for (uint64_t i = 0; i < len; i += 4, ctr += 4) {
		__m128i v;
		v = multfly_gen_impl_(key, ctr + 0);
		_mm_storeu_si128((__m128i *)(r0 + i), v);
		v = multfly_gen_impl_(key, ctr + 1);
		_mm_storeu_si128((__m128i *)(r1 + i), v);
		v = multfly_gen_impl_(key, ctr + 2);
		_mm_storeu_si128((__m128i *)(r2 + i), v);
		v = multfly_gen_impl_(key, ctr + 3);
		_mm_storeu_si128((__m128i *)(r3 + i), v);
	}
}

static inline void multfly_gen64(const multfly_key *key, uint64_t ctr, uint64_t *r, uint64_t len) {
	ctr &= -4;
	len &= -4;
	for (uint64_t i = 0; i < len; i += 4, ctr += 4) {
		__m128i lo = multfly_gen_impl_(key, ctr + 0);
		__m128i hi = multfly_gen_impl_(key, ctr + 1);
		__m128i fst = _mm_unpacklo_epi32(lo, hi);
		__m128i snd = _mm_unpackhi_epi32(lo, hi);
		_mm_storeu_si128((__m128i *)(r + i), fst);
		_mm_storeu_si128((__m128i *)(r + i + 2), snd);

	}
}

static inline void multfly_genf32(const multfly_key *key, uint64_t ctr, float *r, uint64_t len) {
	ctr &= -4;
	len &= -4;
	for (uint64_t i = 0; i < len; i += 4, ctr += 4) {
		__m128i v = multfly_gen_impl_(key, ctr);
		v = _mm_srli_epi32(v, 8);
		__m128 f = _mm_cvtepi32_ps(v);
		f = _mm_mul_ps(f, _mm_set1_ps((float)(1.0 / (UINT32_C(1 << 24)))));
		_mm_storeu_ps(r + i, f);
	}
}

static inline void multfly_genf64(const multfly_key *key, uint64_t ctr, double *r, uint64_t len) {
	ctr &= -4;
	len &= -4;
	for (uint64_t i = 0; i < len; i += 4, ctr += 4) {
		__m128i lo = multfly_gen_impl_(key, ctr + 0);
		__m128i hi = multfly_gen_impl_(key, ctr + 1);
		__m128i fst = _mm_unpacklo_epi32(lo, hi);
		__m128i snd = _mm_unpackhi_epi32(lo, hi);
		fst = _mm_srli_epi64(fst, 11);
		snd = _mm_srli_epi64(snd, 11);
		__m128d fst0 = _mm_cvtsi64_sd(_mm_setzero_pd(), _mm_cvtsi128_si64(fst));
		__m128d fst1 = _mm_cvtsi64_sd(_mm_setzero_pd(), _mm_cvtsi128_si64(_mm_bsrli_si128(fst, 8)));
		__m128d fstf = _mm_shuffle_pd(fst0, fst1, 0);
		fstf = _mm_mul_pd(fstf, _mm_set1_pd(1.0 / (UINT64_C(1) << 53)));
		__m128d snd0 = _mm_cvtsi64_sd(_mm_setzero_pd(), _mm_cvtsi128_si64(snd));
		__m128d snd1 = _mm_cvtsi64_sd(_mm_setzero_pd(), _mm_cvtsi128_si64(_mm_bsrli_si128(snd, 8)));
		__m128d sndf = _mm_shuffle_pd(snd0, snd1, 0);
		sndf = _mm_mul_pd(sndf, _mm_set1_pd(1.0 / (UINT64_C(1) << 53)));
		_mm_storeu_pd(r + i, fstf);
		_mm_storeu_pd(r + i + 2, sndf);
	}
}

#ifdef __cplusplus
}
#endif

#endif
