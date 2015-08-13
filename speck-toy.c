#include <stdio.h>
#include <stdint.h>

#include "speck-encrypt.c"

//prototypes
int internal_test();
void ctr_encode(uint64_t*, uint64_t*, uint64_t, uint64_t*, uint64_t*);



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

    printf("Expected Ciphertext: %016llx %016llx\nComputed Ciphertext: %016llx %016llx\n", c1[1], c1[0], t1[1], t1[0]);
    if (c1[1] == t1[1] && c1[0] == t1[0]) {
        printf("Test 1 Success.\n\n");
    }
    else {
        failure = 1; 
        printf("Test 1 Failed.\n\n");
    }

    //test 2
    printf("Test 2: Single Block Encode\n");
    
    uint64_t k2[] = { 0x1122334455667788, 0x0123456789abceff }; //key
    uint64_t p2[] = { 0xdeadbeefdeadbeef, 0x39abcdef9abcedf9 }; //plaintext
    uint64_t n2[] = { 0x2384626433832795, 0x3141592653589793 }; //nonce
    
    uint64_t c2[] = { 0x78c91afb771243ce, 0x028f0074a570a58b }; //ciphertext

    uint64_t t2[] = {0, 0}; //test

    ctr_encode(p2, n2, 0, k2, t2);

    printf("Expected Ciphertext: %016llx %016llx \nComputed Ciphertext: %016llx %016llx\n", c2[1], c2[0], t2[1], t2[0]);
    if (c2[1] == t2[1] && c2[0] == t2[0]) {
        printf("Test 2 Success.\n\n");
    }
    else {
        failure = 1; 
        printf("Test 2 Failed.\n\n");
    }

    //test 3
    printf("Test 3: Single Block Decode\n");

    uint64_t k3[] = { 0xf5a3c444cef0cc3f, 0xf3762bcf8dc0715d };
    uint64_t n3[] = { 0x7182818284590452, 0x3536028747135266 };
    uint64_t c3[] = { 0xe3929aa79634d795, 0xaff93fc834064f98 };

    uint64_t p3[] = { 0x85b9de0189e353ba, 0x2618d185c4791b74 };

    uint64_t t3[] = { 0, 0 }; 

    ctr_encode(c3, n3, 0, k3, t3);
    
    printf("Expected Plaintext:  %016llx %016llx \nComputed Plaintext:  %016llx %016llx\n", p3[1], p3[0], t3[1], t3[0]);
    if (p3[1] == t3[1] && p3[0] == t3[0]) {
        printf("Test 3 Success.\n\n");
    }
    else {
        failure = 1; 
        printf("Test 3 Failed.\n\n");
    }

    
    

    return failure;

    

}

void ctr_encode(uint64_t* pt_block, uint64_t* nonce, uint64_t count, uint64_t* key, uint64_t* output) {

    uint64_t ctr_block[] = { nonce[1], nonce[0] ^ count };

    speck_encrypt(ctr_block, output, key);
    output[0] ^= pt_block[0];
    output[1] ^= pt_block[1];

}
