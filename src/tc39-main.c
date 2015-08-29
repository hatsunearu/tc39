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
