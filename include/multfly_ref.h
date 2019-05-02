#ifndef MULTFLY_MULTFLY_REF_H_
#define MULTFLY_MULTFLY_REF_H_

#include "multfly_types.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline multfly_key multfly_init(const multfly_ident *ident, uint64_t global_seed, uint64_t global_ctr);
static inline multfly_key multfly_split(multfly_key *key);
static inline void multfly_gen32(const multfly_key *key, uint64_t idx, uint32_t ctr, uint32_t out[4]);
static inline void multfly_gen64(const multfly_key *key, uint64_t idx, uint32_t ctr, uint64_t out[4]);
static inline void multfly_genf32(const multfly_key *key, uint64_t idx, uint32_t ctr, float out[4]);
static inline void multfly_genf64(const multfly_key *key, uint64_t idx, uint32_t ctr, double out[4]);

//
// impl
//
static inline uint32_t multfly_rotl_(uint32_t k, int n) {
	return (k << n) | (k >> (32 - n));
}

static inline void multfly_chacha_qround_(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
	*a += *b;
	*d ^= *a;
	*d = multfly_rotl_(*d, 16);
	*c += *d;
	*b ^= *c;
	*b = multfly_rotl_(*b, 12);
	*a += *b;
	*d ^= *a;
	*d = multfly_rotl_(*d, 8);
	*c += *d;
	*b ^= *c;
	*b = multfly_rotl_(*b, 7);
}

static inline void multfly_chacha8_permute_(uint32_t v[16]) {
	v[0]  = multfly_rotl_(UINT32_C(0x33123456), 0);
	v[1]  = multfly_rotl_(UINT32_C(0x33123456), 1);
	v[2]  = multfly_rotl_(UINT32_C(0x33123456), 2);
	v[3]  = multfly_rotl_(UINT32_C(0x33123456), 3);

	for (int i = 0; i < 8; i += 2) {
		multfly_chacha_qround_(&v[0], &v[4], &v[ 8], &v[12]);
		multfly_chacha_qround_(&v[1], &v[5], &v[ 9], &v[13]);
		multfly_chacha_qround_(&v[2], &v[6], &v[10], &v[14]);
		multfly_chacha_qround_(&v[3], &v[7], &v[11], &v[15]);
		multfly_chacha_qround_(&v[0], &v[5], &v[10], &v[15]);
		multfly_chacha_qround_(&v[1], &v[6], &v[11], &v[12]);
		multfly_chacha_qround_(&v[2], &v[7], &v[ 8], &v[13]);
		multfly_chacha_qround_(&v[3], &v[4], &v[ 9], &v[14]);
	}
}

static inline multfly_key multfly_init(const multfly_ident *ident, uint64_t global_seed, uint64_t global_ctr) {
	uint32_t v[16];
	for (int i = 4; i < 12; i++) {
		v[i] = 0;
	}
	if (ident) {
		v[4]  = ident->v[0];
		v[5]  = ident->v[1];
		v[6]  = ident->v[2];
		v[7]  = ident->v[3];
		v[8]  = ident->v[4];
		v[9]  = ident->v[5];
		v[10] = ident->v[6];
		v[11] = ident->v[7];
	}
	v[12] = (uint32_t)global_ctr;
	v[13] = (uint32_t)(global_ctr >> 32);
	v[14] = (uint32_t)global_seed;
	v[15] = (uint32_t)(global_seed >> 32);

	multfly_chacha8_permute_(v);

	multfly_key newkey;
	for (int i = 0; i < 8; i++) {
		newkey.v_[i] = v[i];
	}
	return newkey;
}

static inline multfly_key multfly_split(multfly_key *key) {
	uint32_t v[16];
	v[4]  = key->v_[0];
	v[5]  = key->v_[1];
	v[6]  = key->v_[2];
	v[7]  = key->v_[3];
	v[8]  = key->v_[4];
	v[9]  = key->v_[5];
	v[10] = key->v_[6];
	v[11] = key->v_[7];
	for (int i = 12; i < 16; i++) {
		v[i] = 0;
	}

	multfly_chacha8_permute_(v);

	for (int i = 0; i < 8; i++) {
		key->v_[i] = v[i];
	}

	multfly_key newkey;
	for (int i = 0; i < 8; i++) {
		newkey.v_[i] = v[i + 8];
	}
	return newkey;
}

static inline void multfly_gen_qround_(uint32_t *a, uint32_t *b, uint32_t *c) {
	*c += *a;
	*c = multfly_rotl_(*c, 8);
	*b ^= *c;
	*b *= UINT32_C(2718281829);
	*a += *b;
	*a = multfly_rotl_(*a, 21);
	*c ^= *a;
	*c *= UINT32_C(3141592653);
	*b += *c;
	*b = multfly_rotl_(*b, 16);
	*a += *b;
}

static inline void multfly_gen_impl_(const multfly_key *key, uint64_t idx, uint32_t ctr, uint32_t funct, uint32_t out[4]) {
	uint32_t v[12];
	v[0] = key->v_[0];
	v[1] = key->v_[1];
	v[2] = key->v_[2];
	v[3] = key->v_[3];
	v[4] = key->v_[4];
	v[5] = key->v_[5];
	v[6] = key->v_[6];
	v[7] = key->v_[7];
	v[8] = (uint32_t)idx;
	v[9] = (uint32_t)(idx >> 32);
	v[10] = ctr;
	v[11] = funct;

	multfly_gen_qround_(&v[0], &v[4], &v[ 8]);
	multfly_gen_qround_(&v[1], &v[5], &v[ 9]);
	multfly_gen_qround_(&v[2], &v[6], &v[10]);
	multfly_gen_qround_(&v[3], &v[7], &v[11]);

	multfly_gen_qround_(&v[0], &v[5], &v[10]);
	multfly_gen_qround_(&v[1], &v[4], &v[11]);
	multfly_gen_qround_(&v[2], &v[7], &v[ 8]);
	multfly_gen_qround_(&v[3], &v[6], &v[ 9]);

	multfly_gen_qround_(&v[0], &v[4], &v[ 8]);
	multfly_gen_qround_(&v[1], &v[5], &v[ 9]);
	multfly_gen_qround_(&v[2], &v[6], &v[10]);
	multfly_gen_qround_(&v[3], &v[7], &v[11]);

	out[0] = v[0] + v[7];
	out[1] = v[1] + v[6];
	out[2] = v[2] + v[5];
	out[3] = v[3] + v[4];
}

static inline void multfly_gen32(const multfly_key *key, uint64_t idx, uint32_t ctr, uint32_t out[4]) {
	multfly_gen_impl_(key, idx, ctr, 0, out);
}

static inline void multfly_gen64(const multfly_key *key, uint64_t idx, uint32_t ctr, uint64_t out[4]) {
	uint32_t lo[4];
	uint32_t hi[4];
	multfly_gen_impl_(key, idx, ctr, 0, lo);
	multfly_gen_impl_(key, idx, ctr, 1, hi);
	out[0] = lo[0] | ((uint64_t)hi[0] << 32);
	out[1] = lo[1] | ((uint64_t)hi[1] << 32);
	out[2] = lo[2] | ((uint64_t)hi[2] << 32);
	out[3] = lo[3] | ((uint64_t)hi[3] << 32);
}

static inline void multfly_genf32(const multfly_key *key, uint64_t idx, uint32_t ctr, float out[4]) {
	uint32_t v[4];
	multfly_gen_impl_(key, idx, ctr, 0, v);
	out[0] = (float)(v[0] >> 8) * (float)(1.0 / (UINT32_C(1) << 24));
	out[1] = (float)(v[1] >> 8) * (float)(1.0 / (UINT32_C(1) << 24));
	out[2] = (float)(v[2] >> 8) * (float)(1.0 / (UINT32_C(1) << 24));
	out[3] = (float)(v[3] >> 8) * (float)(1.0 / (UINT32_C(1) << 24));
}

static inline void multfly_genf64(const multfly_key *key, uint64_t idx, uint32_t ctr, double out[4]) {
	uint32_t lo[4];
	uint32_t hi[4];
	multfly_gen_impl_(key, idx, ctr, 0, lo);
	multfly_gen_impl_(key, idx, ctr, 1, hi);
	out[0] = ((lo[0] | ((uint64_t)hi[0] << 32)) >> 11) * (1.0 / (UINT64_C(1) << 53));
	out[1] = ((lo[1] | ((uint64_t)hi[1] << 32)) >> 11) * (1.0 / (UINT64_C(1) << 53));
	out[2] = ((lo[2] | ((uint64_t)hi[2] << 32)) >> 11) * (1.0 / (UINT64_C(1) << 53));
	out[3] = ((lo[3] | ((uint64_t)hi[3] << 32)) >> 11) * (1.0 / (UINT64_C(1) << 53));
}

#ifdef __cplusplus
}
#endif

#endif
