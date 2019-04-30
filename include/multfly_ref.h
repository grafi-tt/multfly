#ifndef MULTFLY_MULTFLY_REF_H
#define MULTFLY_MULTFLY_REF_H

#include <stdint.h>

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
static inline uint32_t multfly_rotl_(uint32_t k, int n) {
	return (k << n) | (k >> (32 - n));
}

static inline void multfly_chacha_qround_(uint32_t v[16], int a, int b, int c, int d) {
	v[a] += v[b];
	v[d] = multfly_rotl_(v[d] ^ v[a], 16);
	v[c] += v[d];
	v[b] = multfly_rotl_(v[b] ^ v[c], 12);
	v[a] += v[b];
	v[d] = multfly_rotl_(v[d] ^ v[a], 8);
	v[c] += v[d];
	v[b] = multfly_rotl_(v[b] ^ v[c], 7);
}

static inline void multfly_chacha8_permute_(uint32_t v[16]) {
	for (int i = 0; i < 8; i += 2) {
		multfly_chacha_qround_(v, 0, 4,  8, 12);
		multfly_chacha_qround_(v, 1, 5,  9, 13);
		multfly_chacha_qround_(v, 2, 6, 10, 14);
		multfly_chacha_qround_(v, 3, 7, 11, 15);
		multfly_chacha_qround_(v, 0, 5, 10, 15);
		multfly_chacha_qround_(v, 1, 6, 11, 12);
		multfly_chacha_qround_(v, 2, 7,  8, 13);
		multfly_chacha_qround_(v, 3, 4,  9, 14);
	}
}

static inline multfly_key multfly_derive(const multfly_key *key, uint64_t global_seed, uint64_t global_ctr) {
	uint32_t v[16];
	v[0]  = multfly_rotl_(UINT32_C(0x33123456), 0);
	v[1]  = multfly_rotl_(UINT32_C(0x33123456), 1);
	v[2]  = multfly_rotl_(UINT32_C(0x33123456), 2);
	v[3]  = multfly_rotl_(UINT32_C(0x33123456), 3);
	v[4]  = key->k[0];
	v[5]  = key->k[1];
	v[6]  = key->k[2];
	v[7]  = key->k[3];
	v[8]  = key->k[4];
	v[9]  = key->k[5];
	v[10] = key->k[6];
	v[11] = key->k[7];
	v[12] = (uint32_t)global_ctr;
	v[13] = (uint32_t)(global_ctr >> 32);
	v[14] = (uint32_t)global_seed;
	v[15] = (uint32_t)(global_seed >> 32);

	multfly_chacha8_permute_(v);

	multfly_key newkey;
	for (int i = 0; i < 8; i++) {
		newkey.k[i] = v[i] ^ v[i + 8];
	}
	return newkey;
}

static inline void multfly_gen_qround_(uint32_t us[4], uint32_t vs[4], uint32_t lane) {
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

static inline void multfly_gen_impl_(const multfly_key *key, uint64_t ctr, uint32_t result[4]) {
	uint32_t us[4];
	uint32_t vs[4];

	for (uint32_t lane = 0; lane < 4; lane++) {
		us[lane] = key->k[lane] ^ ((uint32_t)ctr + lane);
		vs[lane] = key->k[lane + 4] ^ ((uint32_t)(ctr >> 32) + lane);
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

static inline void multfly_gen32(const multfly_key *key, uint64_t ctr, uint32_t *r, uint64_t len) {
	ctr &= -4;
	len &= -4;
	for (uint64_t i = 0; i < len; i += 4, ctr += 4) {
		multfly_gen_impl_(key, ctr, r + i);
	}
}

static inline void multfly_gen32x4(const multfly_key *key, uint64_t ctr, uint32_t *r0, uint32_t *r1, uint32_t *r2, uint32_t *r3, uint64_t len) {
	ctr &= -4;
	len &= -4;
	for (uint64_t i = 0; i < len; i += 4, ctr += 4) {
		multfly_gen_impl_(key, ctr + 0, r0 + i);
		multfly_gen_impl_(key, ctr + 1, r1 + i);
		multfly_gen_impl_(key, ctr + 2, r2 + i);
		multfly_gen_impl_(key, ctr + 3, r3 + i);
	}
}

static inline void multfly_gen64(const multfly_key *key, uint64_t ctr, uint64_t *r, uint64_t len) {
	ctr &= -4;
	len &= -4;
	uint32_t lo[4];
	uint32_t hi[4];
	for (uint64_t i = 0; i < len; i += 4, ctr += 4) {
		multfly_gen_impl_(key, ctr + 0, lo);
		multfly_gen_impl_(key, ctr + 1, hi);
		r[i + 0] = lo[0] | ((uint64_t)hi[0] << 32);
		r[i + 1] = lo[1] | ((uint64_t)hi[1] << 32);
		r[i + 2] = lo[2] | ((uint64_t)hi[2] << 32);
		r[i + 3] = lo[3] | ((uint64_t)hi[3] << 32);
	}
}

static inline void multfly_genf32(const multfly_key *key, uint64_t ctr, float *r, uint64_t len) {
	ctr &= -4;
	len &= -4;
	for (uint64_t i = 0; i < len; i += 4, ctr += 4) {
		uint32_t v[4];
		multfly_gen_impl_(key, ctr, v);
		r[i + 0] = (float)(v[0] >> 8) * (float)(1.0 / (UINT32_C(1 << 24)));
		r[i + 1] = (float)(v[1] >> 8) * (float)(1.0 / (UINT32_C(1 << 24)));
		r[i + 2] = (float)(v[2] >> 8) * (float)(1.0 / (UINT32_C(1 << 24)));
		r[i + 3] = (float)(v[3] >> 8) * (float)(1.0 / (UINT32_C(1 << 24)));
	}
}

static inline void multfly_genf64(const multfly_key *key, uint64_t ctr, double *r, uint64_t len) {
	ctr &= -4;
	len &= -4;
	for (uint64_t i = 0; i < len; i += 4, ctr += 4) {
		uint32_t lo[4];
		uint32_t hi[4];
		multfly_gen_impl_(key, ctr + 0, lo);
		multfly_gen_impl_(key, ctr + 1, hi);
		r[i + 0] = ((lo[0] >> 11) | ((uint64_t)hi[0] << 21)) * (1.0 / (UINT64_C(1) << 53));
		r[i + 1] = ((lo[1] >> 11) | ((uint64_t)hi[1] << 21)) * (1.0 / (UINT64_C(1) << 53));
		r[i + 2] = ((lo[2] >> 11) | ((uint64_t)hi[2] << 21)) * (1.0 / (UINT64_C(1) << 53));
		r[i + 3] = ((lo[3] >> 11) | ((uint64_t)hi[3] << 21)) * (1.0 / (UINT64_C(1) << 53));
	}
}

#ifdef __cplusplus
}
#endif

#endif
