#include "speck-encrypt.c"

void tc39_ctr_encrypt(FILE* fpin, FILE* fpout, uint64_t* key, uint64_t* nonce);
void tc39_ctr_decrypt(FILE* fpin, FILE* fpout, uint64_t* key);
void tc39_ctr_encode(FILE* fpin, FILE* fpout, uint64_t* key, uint64_t* nonce); 
void block_encode(uint64_t*, uint64_t*, uint64_t*, uint64_t, uint64_t*);

void tc39_ctr_encrypt(FILE* fpin, FILE* fpout, uint64_t* key, uint64_t* nonce) {

  //output nonce
  for (int i=0; i<8; i++) {
    fputc((char)(0xff & (nonce[0] >> (56 - 8*i))), fpout);
  } 
  for (int i=8; i<16; i++) {
    fputc((char)(0xff & (nonce[1] >> (56 - 8*i))), fpout); 
  }

  tc39_ctr_encode(fpin, fpout, key, nonce);
}

void tc39_ctr_decrypt(FILE* fpin, FILE* fpout, uint64_t* key) {

  //read nonce
  uint64_t nonce[] = {0,0};
  char c = 0;

  for (int i = 0; i < 16 && !feof(fpin); i++) {
    c = fgetc(fpin);
    if (i < 8) {
      nonce[0] ^= ((uint64_t)c & 0xff) << (120 - i*8);
    }
    else {
      nonce[1] ^= ((uint64_t)c & 0xff) << (56 - i*8);
    }
  } 
 
  tc39_ctr_encode(fpin, fpout, key, nonce); 

}

void tc39_ctr_encode(FILE* fpin, FILE* fpout, uint64_t* key, uint64_t* nonce) {

  char c;
  uint64_t blockcount = 0;
  int charcount = 0;
  uint64_t in[] = {0,0};
  uint64_t out[] = {0,0};

  //start encryption
  c = fgetc(fpin);
  while (!feof(fpin)) { 
    in[0] = 0;
    in[1] = 0;
    for (charcount = 0; charcount < 16 && !feof(fpin); c = fgetc(fpin), charcount++) {
      if (charcount < 8) {
        in[0] ^= ((uint64_t)c & 0xff) << (120 - charcount*8);
      }
      else {
        in[1] ^= ((uint64_t)c & 0xff) << (56 - charcount*8);
      }
    }
    //printf("I: %05d [ %016" PRIx64 " %016" PRIx64 " ] \n", blockcount, in[0], in[1]);
    block_encode(key, in, nonce, blockcount, out);
    //printf("O: %05d [ %016" PRIx64 " %016" PRIx64 " ] \n\n", blockcount, out[0], out[1]);

    for (int i = 0; i < 8; i++) {
      fputc((char)(0xff & (out[0] >> (56 - 8*i))), fpout);
    }
    for (int i = 0; i < 8; i++) {
      fputc((char)(0xff & (out[1] >> (56 - 8*i))), fpout);
    }
    blockcount++;
  }
}


void block_encode(uint64_t* key, uint64_t* pt_block, uint64_t* nonce, uint64_t count, uint64_t* output) {

  uint64_t ctr_block[] = { nonce[1], nonce[0] ^ count };

  speck_encrypt(ctr_block, output, key);
  output[0] ^= pt_block[0];
  output[1] ^= pt_block[1];

}
