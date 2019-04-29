#ifndef MULTFLY_MULTFLY_REF_H
#define MULTFLY_MULTFLY_REF_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct multfly_key_ {
	uint32_t k[8];
} multfly_key;

multfly_key multfly_derive(const multfly_key *key, uint64_t global_seed, uint64_t global_ctr);
multfly_key multfly_split(multfly_key *key);
void multfly_gen(const multfly_key *key, uint64_t ctr, uint32_t *array, uint64_t len);
void multfly_gen128(const multfly_key *key, uint64_t ctr, uint32_t *array[4], uint64_t len);

//
// impl
//
static inline uint32_t multfly_rotl(uint32_t k, int n) {
	return (k << n) | (k >> (32 - n));
}

static inline void multfly_chacha_qround(uint32_t v[16], int a, int b, int c, int d) {
	v[a] += v[b];
	v[d] = multfly_rotl(v[d] ^ v[a], 16);
	v[c] += v[d];
	v[b] = multfly_rotl(v[b] ^ v[c], 12);
	v[a] += v[b];
	v[d] = multfly_rotl(v[d] ^ v[a], 8);
	v[c] += v[d];
	v[b] = multfly_rotl(v[b] ^ v[c], 7);
}

static inline void multfly_chacha8_permute(uint32_t v[16]) {
	for (int i = 0; i < 8; i += 2) {
		multfly_chacha_qround(v, 0, 4,  8, 12);
		multfly_chacha_qround(v, 1, 5,  9, 13);
		multfly_chacha_qround(v, 2, 6, 10, 14);
		multfly_chacha_qround(v, 3, 7, 11, 15);
		multfly_chacha_qround(v, 0, 5, 10, 15);
		multfly_chacha_qround(v, 1, 6, 11, 12);
		multfly_chacha_qround(v, 2, 7,  8, 13);
		multfly_chacha_qround(v, 3, 4,  9, 14);
	}
}

static inline multfly_key multfly_derive_impl(const multfly_key *key, uint64_t global_seed, uint64_t global_ctr, multfly_key *splitkey) {
	uint32_t v[16];
	// Use Chacha IV: "expand 32-byte k"
	v[0]  = UINT32_C(0x61707865);
	v[1]  = UINT32_C(0x3320646E);
	v[2]  = UINT32_C(0x79622D32);
	v[3]  = UINT32_C(0x6B206574);
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

	multfly_chacha8_permute(v);

	multfly_key newkey;
	for (int i = 0; i < 8; i++) {
		newkey.k[i] = v[i];
	}
	if (splitkey != 0) {
		for (int i = 0; i < 8; i++) {
			splitkey->k[i] = v[i + 8];
		}
	}
	return newkey;
}

inline multfly_key multfly_derive(const multfly_key *key, uint64_t global_seed, uint64_t global_ctr) {
	return multfly_derive_impl(key, global_seed, global_ctr, 0);
}

inline multfly_key multfly_split(multfly_key *key) {
	return multfly_derive_impl(key, 0, 0, key);
}

static inline void multfly_gen_qround(uint32_t us[4], uint32_t vs[4], uint32_t i) {
	uint32_t mulu = UINT32_C(2718281829);
	uint32_t mulv = UINT32_C(3141592653);
	uint32_t incr = UINT32_C(0x33123456);

	uint32_t u = us[i];
	uint32_t v = vs[i];

	u += multfly_rotl(incr, i);
	v += u;
	v ^= multfly_rotl(u, 8);
	v *= mulv;
	u ^= multfly_rotl(v, 9);
	u += v;
	u *= mulu;
	v ^= multfly_rotl(u, 16);
	v += u;

	us[i] = u;
	vs[i] = v;
}

static inline void multfly_gen_swap(uint32_t us[4], uint32_t i, uint32_t j) {
	uint32_t tmp = us[i];
	us[i] = us[j];
	us[j] = tmp;
}

static inline void multfly_gen_impl(const multfly_key *key, uint64_t ctr, uint32_t result[4]) {
	uint32_t us[4];
	uint32_t vs[4];

	for (uint32_t i = 0; i < 4; i++) {
		us[i] = key->k[i] ^ ((uint32_t)ctr + i);
		vs[i] = key->k[i + 4] ^ ((uint32_t)(ctr >> 32) + i);
	}

	multfly_gen_qround(us, vs, 0);
	multfly_gen_qround(us, vs, 1);
	multfly_gen_qround(us, vs, 2);
	multfly_gen_qround(us, vs, 3);

	multfly_gen_swap(vs, 0, 1);
	multfly_gen_swap(vs, 2, 3);

	multfly_gen_qround(us, vs, 0);
	multfly_gen_qround(us, vs, 1);
	multfly_gen_qround(us, vs, 2);
	multfly_gen_qround(us, vs, 3);

	multfly_gen_swap(vs, 0, 2);
	multfly_gen_swap(vs, 1, 3);

	multfly_gen_qround(us, vs, 0);
	multfly_gen_qround(us, vs, 1);
	multfly_gen_qround(us, vs, 2);
	multfly_gen_qround(us, vs, 3);

	result[0] = vs[0];
	result[1] = vs[1];
	result[2] = vs[2];
	result[3] = vs[3];
}

static inline void multfly_gen_loop(const multfly_key *key, uint64_t ctr, uint32_t *array, uint64_t len) {
	for (uint64_t i = 0; i < len; i += 4, ctr += 4) {
		multfly_gen_impl(key, ctr, array + i);
	}
}

inline void multfly_gen(const multfly_key *key, uint64_t ctr, uint32_t *array, uint64_t len) {
	ctr &= -4;
	len &= -4;

	multfly_gen_loop(key, ctr, array, len);
}

inline void multfly_gen128(const multfly_key *key, uint64_t ctr, uint32_t *array[4], uint64_t len) {
	ctr &= -4;
	len &= -4;

	multfly_gen_loop(key, ctr + 0, array[0], len);
	multfly_gen_loop(key, ctr + 1, array[1], len);
	multfly_gen_loop(key, ctr + 2, array[2], len);
	multfly_gen_loop(key, ctr + 3, array[3], len);
}


#ifdef __cplusplus
}
#endif

#endif
