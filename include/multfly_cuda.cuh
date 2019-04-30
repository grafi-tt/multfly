#ifndef MULTFLY_MULTFLY_CUDA_CUH
#define MULTFLY_MULTFLY_CUDA_CUH

// stdint.h don't work with NVRTC
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

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
__device__ static inline uint32_t multfly_device_gen32(const multfly_key *key, uint64_t ctr);
__device__ static inline uint4 multfly_device_gen32x4(const multfly_key *key, uint64_t ctr);
__device__ static inline uint64_t multfly_device_gen64(const multfly_key *key, uint64_t ctr);
__device__ static inline float multfly_device_genf32(const multfly_key *key, uint64_t ctr);
__device__ static inline double multfly_device_genf64(const multfly_key *key, uint64_t ctr);

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
	*a = multfly_device_rotl_(0x33123456u, lane);

	for (int i = 0; i < 8; i += 2) {
		multfly_device_chacha_round_(a, b, c, d);
		*b = __shfl_sync(0xFFFFFFFF, *b, lane + 1, 4);
		*c = __shfl_xor_sync(0xFFFFFFFF, *c, 2);
		*d = __shfl_sync(0xFFFFFFFF, *d, lane + 3, 4);
		multfly_device_chacha_round_(a, b, c, d);
		*b = __shfl_sync(0xFFFFFFFF, *b, lane + 3, 4);
		*c = __shfl_xor_sync(0xFFFFFFFF, *c, 2);
		*d = __shfl_sync(0xFFFFFFFF, *d, lane + 1, 4);
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
	newkey.v_[lane] = a ^ c;
	newkey.v_[lane + 4] = b ^ d;
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

__device__ static inline void multfly_device_gen_round_(uint32_t *u, uint32_t *v) {
	int lane = threadIdx.x & 3;
	uint32_t mulu = 2718281829u;
	uint32_t mulv = 3141592653u;
	uint32_t incr = 0x33123456u;

	*u += multfly_device_rotl_(incr, lane);
	*v += *u;
	*v ^= multfly_device_rotl_(*u, 8);
	*v *= mulv;
	*u ^= multfly_device_rotl_(*v, 9);
	*u += *v;
	*u *= mulu;
	*v ^= multfly_device_rotl_(*u, 16);
	*v += *u;
}

__device__ static inline uint32_t multfly_device_gen_impl_(const multfly_key *key, uint64_t ctr) {
	int lane = threadIdx.x & 3;
	uint32_t u = key->v_[lane] ^ ((uint32_t)ctr + lane);
	uint32_t v = key->v_[lane + 4] ^ ((uint32_t)(ctr >> 32) + lane);

	multfly_device_gen_round_(&u, &v);
	v = __shfl_xor_sync(0xFFFFFFFF, v, 1);
	multfly_device_gen_round_(&u, &v);
	v = __shfl_xor_sync(0xFFFFFFFF, v, 2);
	multfly_device_gen_round_(&u, &v);

	return v;
}

__device__ static inline uint32_t multfly_device_gen32(const multfly_key *key, uint64_t ctr) {
	ctr &= (uint64_t)-4;
	return multfly_device_gen_impl_(key, ctr);
}

__device__ static inline uint4 multfly_device_gen32x4(const multfly_key *key, uint64_t ctr) {
	ctr &= (uint64_t)-4;
	uint32_t x = multfly_device_gen_impl_(key, ctr + 0);
	uint32_t y = multfly_device_gen_impl_(key, ctr + 1);
	uint32_t z = multfly_device_gen_impl_(key, ctr + 2);
	uint32_t w = multfly_device_gen_impl_(key, ctr + 3);
	uint4 r = {x, y, z, w};
	return r;
}

__device__ static inline uint64_t multfly_device_gen64(const multfly_key *key, uint64_t ctr) {
	ctr &= (uint64_t)-4;
	uint32_t lo = multfly_device_gen_impl_(key, ctr + 0);
	uint32_t hi = multfly_device_gen_impl_(key, ctr + 1);
	return lo | ((uint64_t)hi << 32);
}

__device__ static inline float multfly_device_genf32(const multfly_key *key, uint64_t ctr) {
	ctr &= (uint64_t)-4;
	uint32_t v = multfly_device_gen_impl_(key, ctr);
	return (float)(v >> 8) * (float)(1.0 / (1 << 24));
}

__device__ static inline double multfly_device_genf64(const multfly_key *key, uint64_t ctr) {
	ctr &= (uint64_t)-4;
	uint32_t lo = multfly_device_gen_impl_(key, ctr + 0);
	uint32_t hi = multfly_device_gen_impl_(key, ctr + 1);
	return ((lo | ((uint64_t)hi << 32)) >> 11) * (1.0 / (1ll << 53));
}

#ifdef __cplusplus
}
#endif

#endif
