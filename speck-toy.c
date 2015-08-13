#include <stdio.h>
#include <stdint.h>

#include "speck-encrypt.c"

//prototypes
int internal_test();

int main() {

    internal_test();


}

int internal_test() {

    int failure = 0;

    //test 1
    printf("Test 1: Single Block Test of Speck128/128\n");
    
    uint64_t k1[] = { 0x0706050403020100, 0x0f0e0d0c0b0a0908 };
    uint64_t p1[] = { 0x7469206564616d20, 0x6c61766975716520 };
    uint64_t c1[] = { 0x7860fedf5c570d18, 0xa65d985179783265 };

    uint64_t t1[] = {0, 0};

    speck_encrypt(p1, t1, k1);

    printf("Expected Ciphertext: %llx %llx\nComputed Ciphertext: %llx %llx\n", c1[1], c1[0], t1[1], t1[0]);
    if (c1[1] == t1[1] && c1[0] == t1[0]) {
        printf("Test 1 Success.\n");
    }
    else {
        failure = 1; 
        printf("Test 1 Failed.\n");
    }

    return failure;

}


