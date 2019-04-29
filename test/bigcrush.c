#include <unif01.h>
#include <bbattery.h>

#include <multfly.h>

static const uint32_t seed[8] = {0};
static const uint64_t global_seed = 0;
static const uint64_t global_ctr = 0;

#define BUFLEN 32

uint32_t multfly() {
	static uint64_t ctr = 0;
	static multfly_key key;
	static uint32_t buf[BUFLEN];

	if (ctr == 0) {
		key = multfly_init(seed, global_seed, global_ctr);
	}

	if (ctr % BUFLEN == 0) {
		multfly_gen(&key, ctr, buf, BUFLEN);
	}
	return buf[ctr++ % BUFLEN];
}

int main() {
	unif01_Gen *gen;
	gen = unif01_CreateExternGenBits("multfly", multfly);
	bbattery_BigCrush(gen);
	unif01_DeleteExternGenBits(gen);
	return 0;
}
