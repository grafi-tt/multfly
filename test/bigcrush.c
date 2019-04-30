#include <stddef.h>

#include <unif01.h>
#include <bbattery.h>

#include <multfly_ref.h>

#define BUFLEN 32

uint32_t multfly() {
	static multfly_key key;
	static uint64_t ctr = 0;
	static uint32_t buf[BUFLEN];
	if (ctr == 0) {
		key = multfly_init(NULL, 0, 0);
	}

	if (ctr % BUFLEN == 0) {
		multfly_gen32(&key, ctr, buf, BUFLEN);
	}
	uint32_t r = buf[ctr++ % BUFLEN];

#ifdef BITREV
	uint32_t t;
	t = r & UINT32_C(0x00FF00FF);
	r = (t << 16) | (t >> 16) | (t ^ r);
	t = r & UINT32_C(0x0F0F0F0F);
	r = (t << 8) | (t >> 24) | (t ^ r);
	t = r & UINT32_C(0x33333333);
	r = (t << 4) | (t >> 28) | (t ^ r);
	t = r & UINT32_C(0x55555555);
	r = (t << 2) | (t >> 30) | (t ^ r);
	r = (r << 1) | (r >> 31);
#endif
	return r;
}

int main() {
	unif01_Gen *gen;
	gen = unif01_CreateExternGenBits("multfly", multfly);
	bbattery_BigCrush(gen);
	unif01_DeleteExternGenBits(gen);
	return 0;
}
