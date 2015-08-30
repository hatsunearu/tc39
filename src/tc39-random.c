#include <linux/random.h>

void get_rand_128(uint64_t*);

void get_rand_128(uint64_t* p) {
  // old test nonce
  // fix with real rng
  p[0] = 0x2384626433832795;
  p[1] = 0x3141592653589793;
}
