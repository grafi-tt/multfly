#include <stddef.h>

#include <unif01.h>
#include <bbattery.h>

#include <multfly_ref.h>

#ifdef UINT64
#define BUFLEN 8
#else
#define BUFLEN 4
#endif

uint32_t multfly() {
	static multfly_key key;
	static uint64_t id = 0;
	static uint32_t buf[BUFLEN];
	static int pos = 0;
	if (pos == 0) {
		key = multfly_init(NULL, 0, 0);
		pos = BUFLEN;
	}
	if (pos == BUFLEN) {
		pos = 0;
#ifdef UINT64
		multfly_gen64(&key, id++, 0, (uint64_t *)buf);
#else
		multfly_gen32(&key, id++, 0, buf);
#endif
	}
	uint32_t r = buf[pos++];

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
