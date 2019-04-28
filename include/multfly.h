#ifndef MULTFLY_MULTFLY_H
#define MULTFLY_MULTFLY_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct multfly_key_ {
	uint32_t k[8];
} multfly_key;

multfly_key multfly_init(const uint32_t seed[8], uint64_t global_seed, uint64_t global_ctr);
multfly_key multfly_split(multfly_key *key);
void multfly_gen(const multfly_key *key, uint64_t ctr, uint32_t *array, uint64_t len);
void multfly_gen128(const multfly_key *key, uint64_t ctr, uint32_t *array[4], uint64_t len);

#ifdef __cplusplus
}
#endif

#endif
