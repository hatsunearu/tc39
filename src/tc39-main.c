#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "speck-encrypt.c"
#include "tc39-io.c"

//prototypes
int internal_test();
void ctr_encode(uint64_t*, uint64_t*, uint64_t, uint64_t*, uint64_t*);
void base64_encode(uint64_t*, uint64_t, char*);
void block_encrypt(uint64_t*, uint64_t*, uint64_t, uint64_t*, uint64_t*);
void test();


int main(int argc, char** argv) {


    test();
    /*
    char* filename = "test.txt";

    char* bufptr = NULL;
 
    uint64_t blockcnt = open_file(filename, &bufptr, BLOCK) / (2*sizeof(uint64_t));
    
    if (blockcnt == 0) {
        return 1; //error while reading file
    }

    uint64_t* ctptr = malloc(blockcnt * 2*sizeof(uint64_t));

    uint64_t key[] = { 0x1122334455667788, 0x0123456789abceff };  
    uint64_t nonce[] = { 0x2384626433832795, 0x3141592653589793 }; 

    //printf("bufptr: \n%s", bufptr);

    block_encrypt((uint64_t*)bufptr, ctptr, blockcnt, key, nonce);

    printf("Plaintext:\n");
    for (int i = 0; i < blockcnt; i++) {
        printf("%016"PRIx64" %016"PRIx64"\n", ((uint64_t*)bufptr)[2*i+1], ((uint64_t*)bufptr)[2*i]);
    }

    printf("Ciphertext:\n");
    for (int i = 0; i < blockcnt; i++) {
        printf("%016"PRIx64" %016"PRIx64"\n", ctptr[2*i+1], ctptr[2*i]);
    }

    free(bufptr);
    free(ctptr);
    */
}

void test() {

    FILE* fpin;
    fpin = fopen("in.pt", "r");

    FILE* fpout;
    fpout = fopen("out.ct", "w");

    if (fpin == NULL) {
        perror("Error opening input");
        return;
    }
    if (fpout == NULL) {
        perror("Error opening output");
        return;
    }

    char c;
    uint64_t blockcount = 0;
    int charcount = 0;
    uint64_t pt[] = { 0, 0 };
    uint64_t ct[] = { 0, 0 };

    uint64_t key[] = { 0x1122334455667788, 0x0123456789abceff };  
    uint64_t nonce[] = { 0x2384626433832795, 0x3141592653589793 }; 
   
    c = fgetc(fpin);
    while (!feof(fpin)) { 
        pt[0] = 0;
        pt[1] = 0;
        for (charcount = 0; charcount < 16 && !feof(fpin); c = fgetc(fpin), charcount++) {
            if (charcount < 8) {
                pt[0] ^= ((uint64_t)c & 0xff) << (120 - charcount*8);
            }
            else {
                pt[1] ^= ((uint64_t)c & 0xff) << (56 - charcount*8);
            }
        }
        printf("PT: %05d [ %016" PRIx64 " %016" PRIx64 " ] \n", blockcount, pt[0], pt[1]);
        ctr_encode(pt, nonce, blockcount, key, ct);
        printf("CT: %05d [ %016" PRIx64 " %016" PRIx64 " ] \n\n", blockcount, ct[0], ct[1]);
        
        for (int i = 0; i < 8; i++) {
            fputc((char)(0xff & (ct[0] >> (56 - 8*i))), fpout);
        }
        for (int i = 0; i < 8; i++) {
            fputc((char)(0xff & (ct[1] >> (56 - 8*i))), fpout);
        }
        
        blockcount++;
    }
    fclose(fpin);
    fclose(fpout);

}

void block_encrypt(uint64_t* input, uint64_t* ciphertext, uint64_t blockcnt, uint64_t* key, uint64_t* nonce) {
    for (int i = 0; i < blockcnt; i++) {
        ctr_encode(input + 2*i, nonce, i, key, ciphertext + 2*i);
    }
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

    printf("Expected Ciphertext: %016" PRIx64 " %016" PRIx64 "\nComputed Ciphertext: %016" PRIx64 " %016" PRIx64 "\n", c1[1], c1[0], t1[1], t1[0]);
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

    printf("Expected Ciphertext: %016" PRIx64 " %016" PRIx64 " \nComputed Ciphertext: %016" PRIx64 " %016" PRIx64 "\n", c2[1], c2[0], t2[1], t2[0]);
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
    
    printf("Expected Plaintext:  %016" PRIx64 " %016" PRIx64 " \nComputed Plaintext:  %016" PRIx64 " %016" PRIx64 "\n", p3[1], p3[0], t3[1], t3[0]);
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

/*
void base64_encode(uint64_t* in, uint64_t len, char* out) {
    // 000 111 222 333 444 5 | 55 666 777 888 999 00 | 0 111 222 333 444 555 | ...
    
    uint64_t blockind = 0; //index of block 
    int ctr = 0;           //index inside each block
    uint64_t chrcnt = 0;   //index to character
    
    uint16_t b64char = 0x000;

    for (blockind = 0; blockind < len; blockind++) {
        for (; ctr < 64; ctr+=3) {
        }
    } 
}
*/
