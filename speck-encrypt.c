#include <stdint.h>

#define ROR(x, r) ((x >> r) | (x << (64 - r)))
#define ROL(x, r) ((x << r) | (x >> (64 - r)))
#define R(x, y, k) (x = ROR(x, 8), x += y, x ^= k, y = ROL(y, 3), y ^= x)

void speck_encrypt(uint64_t *pt, uint64_t *ct, uint64_t *K)
{
    uint64_t i, B = K[1], A = K[0];
    ct[0] = pt[0]; ct[1] = pt[1];

    for(i = 0; i < 32; i++)
    {
        R(ct[1], ct[0], A);
        R(B, A, i);
    }
}
