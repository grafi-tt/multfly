#ifndef MULTFLY_MULTFLY_REF_H
#define MULTFLY_MULTFLY_REF_H

#include <stdint.h>

#include "multfly_types.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline multfly_key multfly_init(const multfly_ident *ident, uint64_t global_seed, uint64_t global_ctr);
static inline multfly_key multfly_split(multfly_key *key);
static inline void multfly_gen32(const multfly_key *key, uint64_t id, uint64_t ctr, uint32_t r[4]);
static inline void multfly_gen64(const multfly_key *key, uint64_t id, uint64_t ctr, uint64_t r[4]);
static inline void multfly_genf32(const multfly_key *key, uint64_t id, uint64_t ctr, float r[4]);
static inline void multfly_genf64(const multfly_key *key, uint64_t id, uint64_t ctr, double r[4]);

//
// impl
//
static inline uint32_t multfly_rotl_(uint32_t k, int n) {
	return (k << n) | (k >> (32 - n));
}

static inline void multfly_chacha_qround_(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
	*a += *b;
	*d = multfly_rotl_(*d ^ *a, 16);
	*c += *d;
	*b = multfly_rotl_(*b ^ *c, 12);
	*a += *b;
	*d = multfly_rotl_(*d ^ *a, 8);
	*c += *d;
	*b = multfly_rotl_(*b ^ *c, 7);
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
		newkey.v_[i] = v[i] ^ v[i + 8];
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

static inline void multfly_gen_qround_(uint32_t us[4], uint32_t vs[4], int lane) {
	uint32_t mulu = UINT32_C(2718281829);
	uint32_t mulv = UINT32_C(3141592653);
	uint32_t incr = UINT32_C(0x33123456);

	uint32_t u = us[lane];
	uint32_t v = vs[lane];

	u += multfly_rotl_(incr, lane);
	v += u;
	v ^= multfly_rotl_(u, 8);
	v *= mulv;
	u ^= multfly_rotl_(v, 9);
	u += v;
	u *= mulu;
	v ^= multfly_rotl_(u, 16);
	v += u;

	us[lane] = u;
	vs[lane] = v;
}

static inline void multfly_gen_swap_(uint32_t us[4], uint32_t i, uint32_t j) {
	uint32_t tmp = us[i];
	us[i] = us[j];
	us[j] = tmp;
}

static inline void multfly_gen32(const multfly_key *key, uint64_t id, uint64_t ctr, uint32_t result[4]) {
	uint32_t us[4];
	uint32_t vs[4];

	uint32_t id_lo = (uint32_t)id;
	uint32_t id_hi = (uint32_t)(id >> 32);
	uint32_t ctr_lo = (uint32_t)ctr;
	uint32_t ctr_hi = (uint32_t)(ctr >> 32);

	for (int lane = 0; lane < 4; lane++) {
		uint32_t nonce_lo = id_lo + multfly_rotl_(ctr_lo, lane * 8) + lane;
		uint32_t nonce_hi = id_hi + multfly_rotl_(ctr_hi, lane * 8) + lane;
		us[lane] = key->v_[lane] ^ nonce_lo;
		vs[lane] = key->v_[lane + 4] ^ nonce_hi;
	}

	multfly_gen_qround_(us, vs, 0);
	multfly_gen_qround_(us, vs, 1);
	multfly_gen_qround_(us, vs, 2);
	multfly_gen_qround_(us, vs, 3);

	multfly_gen_swap_(vs, 0, 1);
	multfly_gen_swap_(vs, 2, 3);

	multfly_gen_qround_(us, vs, 0);
	multfly_gen_qround_(us, vs, 1);
	multfly_gen_qround_(us, vs, 2);
	multfly_gen_qround_(us, vs, 3);

	multfly_gen_swap_(vs, 0, 2);
	multfly_gen_swap_(vs, 1, 3);

	multfly_gen_qround_(us, vs, 0);
	multfly_gen_qround_(us, vs, 1);
	multfly_gen_qround_(us, vs, 2);
	multfly_gen_qround_(us, vs, 3);

	result[0] = vs[0];
	result[1] = vs[1];
	result[2] = vs[2];
	result[3] = vs[3];
}

static inline void multfly_gen64(const multfly_key *key, uint64_t id, uint64_t ctr, uint64_t r[4]) {
	uint32_t lo[4];
	uint32_t hi[4];
	multfly_gen32(key, id, ctr + 0, lo);
	multfly_gen32(key, id, ctr + 1, hi);
	r[0] = lo[0] | ((uint64_t)hi[0] << 32);
	r[1] = lo[1] | ((uint64_t)hi[1] << 32);
	r[2] = lo[2] | ((uint64_t)hi[2] << 32);
	r[3] = lo[3] | ((uint64_t)hi[3] << 32);
}

static inline void multfly_genf32(const multfly_key *key, uint64_t id, uint64_t ctr, float r[4]) {
	uint32_t v[4];
	multfly_gen32(key, id, ctr, v);
	r[0] = (float)(v[0] >> 8) * (float)(1.0 / (UINT32_C(1) << 24));
	r[1] = (float)(v[1] >> 8) * (float)(1.0 / (UINT32_C(1) << 24));
	r[2] = (float)(v[2] >> 8) * (float)(1.0 / (UINT32_C(1) << 24));
	r[3] = (float)(v[3] >> 8) * (float)(1.0 / (UINT32_C(1) << 24));
}

static inline void multfly_genf64(const multfly_key *key, uint64_t id, uint64_t ctr, double r[4]) {
	uint32_t lo[4];
	uint32_t hi[4];
	multfly_gen32(key, id, ctr + 0, lo);
	multfly_gen32(key, id, ctr + 1, hi);
	r[0] = ((lo[0] >> 11) | ((uint64_t)hi[0] << 21)) * (1.0 / (UINT64_C(1) << 53));
	r[1] = ((lo[1] >> 11) | ((uint64_t)hi[1] << 21)) * (1.0 / (UINT64_C(1) << 53));
	r[2] = ((lo[2] >> 11) | ((uint64_t)hi[2] << 21)) * (1.0 / (UINT64_C(1) << 53));
	r[3] = ((lo[3] >> 11) | ((uint64_t)hi[3] << 21)) * (1.0 / (UINT64_C(1) << 53));
}

#ifdef __cplusplus
}
#endif

#endif
