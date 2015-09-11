#include <stdlib.h>
#include <stdio.h>

void get_rand_128(uint64_t*);

void get_rand_128(uint64_t* p) {

  FILE* fp = fopen("/dev/urandom", "r");
  for (int i=0; i<8; i++) {
    p[0] = (p[0] << 8) ^ (((uint64_t)fgetc(fp)) & 0xff);
    p[1] = (p[1] << 8) ^ (((uint64_t)fgetc(fp)) & 0xff);
  }
  fclose(fp);
}
