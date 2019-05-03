#ifndef MULTFLY_MULTFLY_SSE41_H_
#define MULTFLY_MULTFLY_SSE41_H_

#include <emmintrin.h>
#include <smmintrin.h>
#include <tmmintrin.h>

#include "multfly_types.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline void multfly_initkey(multfly_key *key, const multfly_name *name, uint64_t global_seed, uint64_t global_ctr);
static inline void multfly_splitkey(multfly_key *key, multfly_key *newkey);
static inline void multfly_gen32(const multfly_key *key, uint64_t idx, uint32_t ctr, uint32_t out[4]);
static inline void multfly_gen64(const multfly_key *key, uint64_t idx, uint32_t ctr, uint64_t out[4]);
static inline void multfly_genf32(const multfly_key *key, uint64_t idx, uint32_t ctr, float out[4]);
static inline void multfly_genf64(const multfly_key *key, uint64_t idx, uint32_t ctr, double out[4]);

#define multfly_initkey_fromliteral(key, literal, global_seed, global_ctr) \
	multfly_initkey(key, &MULTFLY_NAME_LITERAL(literal), global_seed, global_ctr)

//
// impl
//
static inline void multfly_chacha_round_(__m128i *a, __m128i *b, __m128i *c, __m128i *d) {
	__m128i rotl8_shufvec = _mm_set_epi8(
			0x0E, 0x0D, 0x0C, 0x0F, 0x0A, 0x09, 0x08, 0x0B, 0x06, 0x05, 0x04, 0x07, 0x02, 0x01, 0x00, 0x03);
	__m128i rotl16_shufvec = _mm_shuffle_epi8(rotl8_shufvec, rotl8_shufvec);

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
	*a = _mm_set_epi32(UINT32_C(2345678910), UINT32_C(1234567891), UINT32_C(3141592653), UINT32_C(2718281829));

	for (int i = 0; i < 8; i += 2) {
		multfly_chacha_round_(a, b, c, d);
		*a = _mm_shuffle_epi32(*a, 0x93);
		*d = _mm_shuffle_epi32(*d, 0x4E);
		*c = _mm_shuffle_epi32(*c, 0x39);
		multfly_chacha_round_(a, b, c, d);
		*a = _mm_shuffle_epi32(*a, 0x39);
		*d = _mm_shuffle_epi32(*d, 0x4E);
		*c = _mm_shuffle_epi32(*c, 0x93);
	}
}

static inline void multfly_initkey(multfly_key *key, const multfly_name *name, uint64_t global_seed, uint64_t global_ctr) {
	__m128i a;
	__m128i b = _mm_setzero_si128();
	__m128i c = _mm_setzero_si128();
	if (name) {
		b = _mm_loadu_si128((const __m128i *)&name->v[0]);
		c = _mm_loadu_si128((const __m128i *)&name->v[4]);
	}
	__m128i d = _mm_set_epi64x(global_seed, global_ctr);

	multfly_chacha8_permute_(&a, &b, &c, &d);

	_mm_storeu_si128((__m128i *)&key->v_[0], a);
	_mm_storeu_si128((__m128i *)&key->v_[4], b);
}

static inline void multfly_splitkey(multfly_key *key, multfly_key *newkey) {
	__m128i a;
	__m128i b = _mm_loadu_si128((const __m128i *)&key->v_[0]);
	__m128i c = _mm_loadu_si128((const __m128i *)&key->v_[4]);
	__m128i d = _mm_setzero_si128();

	multfly_chacha8_permute_(&a, &b, &c, &d);

	_mm_storeu_si128((__m128i *)&key->v_[0], a);
	_mm_storeu_si128((__m128i *)&key->v_[4], b);
	_mm_storeu_si128((__m128i *)&newkey->v_[0], c);
	_mm_storeu_si128((__m128i *)&newkey->v_[4], d);
}

static inline void multfly_gen_round_(__m128i *a, __m128i *b, __m128i *c) {
	__m128i rotl8_shufvec = _mm_set_epi8(
			0x0E, 0x0D, 0x0C, 0x0F, 0x0A, 0x09, 0x08, 0x0B, 0x06, 0x05, 0x04, 0x07, 0x02, 0x01, 0x00, 0x03);
	__m128i rotl16_shufvec = _mm_shuffle_epi8(rotl8_shufvec, rotl8_shufvec);

	*c = _mm_add_epi32(*c, *a);
	*c = _mm_shuffle_epi8(*c, rotl8_shufvec);
	*b = _mm_xor_si128(*b, *c);
	*b = _mm_mullo_epi32(*b, _mm_set1_epi32(UINT32_C(2718281829)));
	*a = _mm_add_epi32(*a, *b);
	*a = _mm_or_si128(_mm_slli_epi32(*a, 11), _mm_srli_epi32(*a, 21));
	*c = _mm_xor_si128(*c, *a);
	*c = _mm_mullo_epi32(*c, _mm_set1_epi32(UINT32_C(3141592653)));
	*b = _mm_add_epi32(*b, *c);
	*b = _mm_shuffle_epi8(*b, rotl16_shufvec);
	*a = _mm_xor_si128(*a, *b);
}

static inline __m128i multfly_gen_impl_(const multfly_key *key, uint64_t idx, uint64_t ctr_funct) {
	__m128i a = _mm_loadu_si128((const __m128i *)&key->v_[0]);
	__m128i b = _mm_loadu_si128((const __m128i *)&key->v_[4]);
	__m128i c = _mm_set_epi64x(ctr_funct, idx);

	multfly_gen_round_(&a, &b, &c);
	b = _mm_shuffle_epi32(b, 0xB1);
	c = _mm_shuffle_epi32(c, 0x4E);
	multfly_gen_round_(&a, &b, &c);
	b = _mm_shuffle_epi32(b, 0xB1);
	c = _mm_shuffle_epi32(c, 0x4E);
	multfly_gen_round_(&a, &b, &c);

	return _mm_add_epi32(a, _mm_shuffle_epi32(b, 0x1B));
}

static inline void multfly_gen32(const multfly_key *key, uint64_t idx, uint32_t ctr, uint32_t out[4]) {
	__m128i v = multfly_gen_impl_(key, idx, ctr);
	_mm_storeu_si128((__m128i *)out, v);
}

static inline void multfly_gen64(const multfly_key *key, uint64_t idx, uint32_t ctr, uint64_t out[4]) {
	__m128i lo = multfly_gen_impl_(key, idx, ctr);
	__m128i hi = multfly_gen_impl_(key, idx, ctr | (UINT64_C(1) << 32));
	__m128i fst = _mm_unpacklo_epi32(lo, hi);
	__m128i snd = _mm_unpackhi_epi32(lo, hi);
	_mm_storeu_si128((__m128i *)&out[0], fst);
	_mm_storeu_si128((__m128i *)&out[2], snd);
}

static inline void multfly_genf32(const multfly_key *key, uint64_t idx, uint32_t ctr, float out[4]) {
	__m128i v = multfly_gen_impl_(key, idx, ctr);
	v = _mm_srli_epi32(v, 8);
	__m128 f = _mm_cvtepi32_ps(v);
	f = _mm_mul_ps(f, _mm_set1_ps((float)(1.0 / (UINT32_C(1) << 24))));
	_mm_storeu_ps(out, f);
}

static inline void multfly_genf64(const multfly_key *key, uint64_t idx, uint32_t ctr, double out[4]) {
	__m128i lo = multfly_gen_impl_(key, idx, ctr);
	__m128i hi = multfly_gen_impl_(key, idx, ctr | (UINT64_C(1) << 32));
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
	_mm_storeu_pd(&out[0], fstf);
	_mm_storeu_pd(&out[2], sndf);
}

#ifdef __cplusplus
}
#endif

#endif
