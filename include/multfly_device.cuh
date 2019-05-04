#ifndef MULTFLY_MULTFLY_DEVICE_CUH_
#define MULTFLY_MULTFLY_DEVICE_CUH_

#include "multfly_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#if __CUDACC_VER_MAJOR__ < 9
#define __shfl_sync(mask, ...) __shfl(__VA_ARGS__)
#define __shfl_xor_sync(mask, ...) __shfl_xor(__VA_ARGS__)
#endif

__device__ static void multfly_device_initkey(multfly_key *key, const multfly_name *name, uint64_t global_seed, uint64_t global_ctr);
__device__ static void multfly_device_splitkey(multfly_key *key, multfly_key *newkey);
__device__ static inline uint32_t multfly_device_gen32(const multfly_key *key, uint64_t idx, uint32_t ctr);
__device__ static inline uint64_t multfly_device_gen64(const multfly_key *key, uint64_t idx, uint32_t ctr);
__device__ static inline float multfly_device_genf32(const multfly_key *key, uint64_t idx, uint32_t ctr);
__device__ static inline double multfly_device_genf64(const multfly_key *key, uint64_t idx, uint32_t ctr);

#define multfly_device_initkey_fromliteral(key, literal, global_seed, global_ctr) \
	multfly_device_initkey(key, &MULTFLY_NAME_LITERAL(literal), global_seed, global_ctr)

//
// impl
//
__device__ static inline uint32_t multfly_device_rotl_(uint32_t k, int n) {
	return (k << n) | (k >> (32 - n));
}

__device__ static inline void multfly_device_chacha_qround_(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
	*a += *b;
	*d ^= *a;
	*d = multfly_device_rotl_(*d, 16);
	*c += *d;
	*b ^= *c;
	*b = multfly_device_rotl_(*b, 12);
	*a += *b;
	*d ^= *a;
	*d = multfly_device_rotl_(*d, 8);
	*c += *d;
	*b ^= *c;
	*b = multfly_device_rotl_(*b, 7);
}

__device__ static inline void multfly_device_chacha8_permute_(uint32_t v[16]) {
	v[0] = 2718281829u;
	v[1] = 3141592653u;
	v[2] = 1234567891u;
	v[3] = 2345678910u;

	for (int i = 0; i < 8; i += 2) {
		multfly_device_chacha_qround_(&v[0], &v[4], &v[ 8], &v[12]);
		multfly_device_chacha_qround_(&v[1], &v[5], &v[ 9], &v[13]);
		multfly_device_chacha_qround_(&v[2], &v[6], &v[10], &v[14]);
		multfly_device_chacha_qround_(&v[3], &v[7], &v[11], &v[15]);
		multfly_device_chacha_qround_(&v[0], &v[5], &v[10], &v[15]);
		multfly_device_chacha_qround_(&v[1], &v[6], &v[11], &v[12]);
		multfly_device_chacha_qround_(&v[2], &v[7], &v[ 8], &v[13]);
		multfly_device_chacha_qround_(&v[3], &v[4], &v[ 9], &v[14]);
	}
}

__device__ static inline void multfly_device_initkey(multfly_key *key, const multfly_name *name, uint64_t global_seed, uint64_t global_ctr) {
	uint32_t v[16];
#pragma unroll
	for (int i = 0; i < 8; i++) {
		v[i + 4] = 0;
	}
	if (name) {
#pragma unroll
		for (int i = 0; i < 8; i++) {
			v[i + 4] = name->v[i];
		}
	}
	v[12] = (uint32_t)global_ctr;
	v[13] = (uint32_t)(global_ctr >> 32);
	v[14] = (uint32_t)global_seed;
	v[15] = (uint32_t)(global_seed >> 32);

	multfly_device_chacha8_permute_(v);

#pragma unroll
	for (int i = 0; i < 8; i++) {
		key->v_[i] = v[i];
	}
}

__device__ static inline void multfly_device_splitkey(multfly_key *key, multfly_key *newkey) {
	uint32_t v[16];
#pragma unroll
	for (int i = 0; i < 8; i++) {
		v[i + 4] = key->v_[i];
	}
	v[12] = 0;
	v[13] = 0;
	v[14] = 0;
	v[15] = 0;

	multfly_device_chacha8_permute_(v);

#pragma unroll
	for (int i = 0; i < 8; i++) {
		key->v_[i] = v[i];
		newkey->v_[i] = v[i + 8];
	}
}

__device__ static inline void multfly_device_gen_round_(uint32_t *a, uint32_t *b, uint32_t *c) {
	*c += *a;
	*c = multfly_device_rotl_(*c, 8);
	*b ^= *c;
	*b *= 2718281829u;
	*a += *b;
	*a = multfly_device_rotl_(*a, 11);
	*c ^= *a;
	*c *= 3141592653u;
	*b += *c;
	*b = multfly_device_rotl_(*b, 16);
	*a ^= *b;
}

__device__ static inline uint32_t multfly_device_gen_impl_(const multfly_key *key, uint64_t tid, uint64_t ctr_funct) {
	uint64_t idx = tid >> 2;
	int lane = tid & 3;
	uint32_t a = key->v_[lane];
	uint32_t b = key->v_[lane + 4];
	uint64_t c64 = lane & 2 ? ctr_funct : idx;
	uint32_t c = lane & 1 ? (uint32_t)(c64 >> 32) : (uint32_t)c64;

	multfly_device_gen_round_(&a, &b, &c);
	b = __shfl_xor_sync(0xFFFFFFFF, b, 1);
	c = __shfl_xor_sync(0xFFFFFFFF, c, 2);
	multfly_device_gen_round_(&a, &b, &c);
	b = __shfl_xor_sync(0xFFFFFFFF, b, 1);
	c = __shfl_xor_sync(0xFFFFFFFF, c, 2);
	multfly_device_gen_round_(&a, &b, &c);

	return a + __shfl_xor_sync(0xFFFFFFFF, b, 3);
}

__device__ static inline uint32_t multfly_device_gen32(const multfly_key *key, uint64_t tid, uint32_t ctr) {
	return multfly_device_gen_impl_(key, tid, ctr);
}

__device__ static inline uint64_t multfly_device_gen64(const multfly_key *key, uint64_t tid, uint32_t ctr) {
	uint32_t lo = multfly_device_gen_impl_(key, tid, ctr);
	uint32_t hi = multfly_device_gen_impl_(key, tid, ctr | (1ull << 32));
	return lo | ((uint64_t)hi << 32);
}

__device__ static inline float multfly_device_genf32(const multfly_key *key, uint64_t tid, uint32_t ctr) {
	uint32_t v = multfly_device_gen_impl_(key, tid, ctr);
	return (float)(v >> 8) * (float)(1.0 / (1 << 24));
}

__device__ static inline double multfly_device_genf64(const multfly_key *key, uint64_t tid, uint32_t ctr) {
	uint32_t lo = multfly_device_gen_impl_(key, tid, ctr);
	uint32_t hi = multfly_device_gen_impl_(key, tid, ctr | (1ull << 32));
	return ((lo | ((uint64_t)hi << 32)) >> 11) * (1.0 / (1ull << 53));
}

#ifdef __cplusplus
}
#endif

#endif
