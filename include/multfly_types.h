#ifndef MULTFLY_MULTFLY_TYPES_H_
#define MULTFLY_MULTFLY_TYPES_H_

#ifdef __CUDA_ARCH__
// stdint.h don't work with NVRTC
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
#else
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct multfly_ident_ {
	uint32_t v[8];
} multfly_ident;

typedef struct multfly_key_ {
	uint32_t v_[8];
} multfly_key;

#ifdef __cplusplus
}
#endif

#endif
