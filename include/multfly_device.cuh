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

__device__ static inline multfly_key multfly_device_init(const multfly_ident *ident, uint64_t global_seed, uint64_t global_ctr);
__device__ static inline multfly_key multfly_device_split(multfly_key *key);
__device__ static inline uint32_t multfly_device_gen32(const multfly_key *key, uint64_t idx, uint32_t ctr);
__device__ static inline uint64_t multfly_device_gen64(const multfly_key *key, uint64_t idx, uint32_t ctr);
__device__ static inline float multfly_device_genf32(const multfly_key *key, uint64_t idx, uint32_t ctr);
__device__ static inline double multfly_device_genf64(const multfly_key *key, uint64_t idx, uint32_t ctr);

#define multfly_device_init_by_literal(literal, global_seed, global_ctr) \
	multfly_device_init(&MULTFLY_IDENT_LITERAL(literal), global_seed, global_ctr)

//
// impl
//
__device__ static inline uint32_t multfly_device_rotl_(uint32_t k, int n) {
	return (k << n) | (k >> (32 - n));
}

__device__ static inline void multfly_device_chacha_round_(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
	*a += *b;
	*d = multfly_device_rotl_(*d ^ *a, 16);
	*c += *d;
	*b = multfly_device_rotl_(*b ^ *c, 12);
	*a += *b;
	*d = multfly_device_rotl_(*d ^ *a, 8);
	*c += *d;
	*b = multfly_device_rotl_(*b ^ *c, 7);
}

__device__ static inline void multfly_device_chacha8_permute_(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
	int lane = threadIdx.x & 3;
	static const uint32_t iv[4] = {2718281829u, 3141592653u, 1234567891u, 2345678910u};
	*a = iv[lane];

	for (int i = 0; i < 8; i += 2) {
		multfly_device_chacha_round_(a, b, c, d);
		*a = __shfl_sync(0xFFFFFFFF, *a, lane + 3, 4);
		*d = __shfl_xor_sync(0xFFFFFFFF, *d, 2);
		*c = __shfl_sync(0xFFFFFFFF, *c, lane + 1, 4);
		multfly_device_chacha_round_(a, b, c, d);
		*a = __shfl_sync(0xFFFFFFFF, *a, lane + 1, 4);
		*d = __shfl_xor_sync(0xFFFFFFFF, *d, 2);
		*c = __shfl_sync(0xFFFFFFFF, *c, lane + 3, 4);
	}
}

__device__ static inline multfly_key multfly_device_init(const multfly_ident *ident, uint64_t global_seed, uint64_t global_ctr) {
	int lane = threadIdx.x & 3;
	uint32_t a;
	uint32_t b = 0;
	uint32_t c = 0;
	if (ident) {
		b = ident->v[lane];
		c = ident->v[lane + 4];
	}
	uint64_t d64 = lane & 2 ? global_seed : global_ctr;
	uint32_t d = lane & 1 ? (uint32_t)(d64 >> 32) : (uint32_t)d64;

	multfly_device_chacha8_permute_(&a, &b, &c, &d);

	multfly_key newkey;
	newkey.v_[lane] = a;
	newkey.v_[lane + 4] = b;
	return newkey;
}

__device__ static inline multfly_key multfly_device_split(multfly_key *key) {
	int lane = threadIdx.x & 3;
	uint32_t a;
	uint32_t b = key->v_[lane];
	uint32_t c = key->v_[lane + 4];
	uint32_t d = 0;

	multfly_device_chacha8_permute_(&a, &b, &c, &d);

	key->v_[lane] = a;
	key->v_[lane + 4] = b;

	multfly_key newkey;
	newkey.v_[lane] = c;
	newkey.v_[lane + 4] = d;
	return newkey;
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

__device__ static inline uint32_t multfly_device_gen_impl_(const multfly_key *key, uint64_t idx, uint64_t ctr_funct) {
	int lane = threadIdx.x & 3;
	uint32_t a = key->v_[lane];
	uint32_t b = key->v_[lane + 4];
	uint64_t c64 = lane & 2 ? ctr_funct : idx;
	uint32_t c = lane & 1 ? (uint32_t)(c64 >> 32) : (uint32_t)c64;

	multfly_device_gen_round_(&a, &b, &c);
	a = __shfl_xor_sync(0xFFFFFFFF, a, 1);
	b = __shfl_xor_sync(0xFFFFFFFF, b, 2);
	multfly_device_gen_round_(&a, &b, &c);
	a = __shfl_xor_sync(0xFFFFFFFF, a, 1);
	b = __shfl_xor_sync(0xFFFFFFFF, b, 2);
	multfly_device_gen_round_(&a, &b, &c);

	return a + __shfl_xor_sync(0xFFFFFFFF, b, 3);
}

__device__ static inline uint32_t multfly_device_gen32(const multfly_key *key, uint64_t idx, uint32_t ctr) {
	return multfly_device_gen_impl_(key, idx, ctr);
}

__device__ static inline uint64_t multfly_device_gen64(const multfly_key *key, uint64_t idx, uint32_t ctr) {
	uint32_t lo = multfly_device_gen_impl_(key, idx, ctr);
	uint32_t hi = multfly_device_gen_impl_(key, idx, ctr | (1ull << 32));
	return lo | ((uint64_t)hi << 32);
}

__device__ static inline float multfly_device_genf32(const multfly_key *key, uint64_t idx, uint32_t ctr) {
	uint32_t v = multfly_device_gen_impl_(key, idx, ctr);
	return (float)(v >> 8) * (float)(1.0 / (1 << 24));
}

__device__ static inline double multfly_device_genf64(const multfly_key *key, uint64_t idx, uint32_t ctr) {
	uint32_t lo = multfly_device_gen_impl_(key, idx, ctr);
	uint32_t hi = multfly_device_gen_impl_(key, idx, ctr | (1ull << 32));
	return ((lo | ((uint64_t)hi << 32)) >> 11) * (1.0 / (1ull << 53));
}

#ifdef __cplusplus
}
#endif

#endif
