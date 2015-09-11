#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <getopt.h>

#define DECODE_MODE 1
#define ENCODE_MODE 2

#define IMPORT_KEY 1
#define GENERATE_KEY 2

#include "tc39-errorcodes.h"

#include "tc39-crypto.c"
#include "tc39-random.c"

int main(int argc, char** argv) {

  char c;
  char* input_file_name = NULL;
  char* output_file_name = NULL;
  char* key_file_name = NULL;
  
  int mode_flag = 0;
  int key_flag = 0;

  while ((c = getopt(argc, argv, "e::d::o:K:k:")) != -1) {
    switch (c) {
      case 'e':
        if (mode_flag != 0) {
          fprintf(stderr, "Please choose to either encode or decode.");
          return MODE_ERROR;
        }
        mode_flag = ENCODE_MODE;
        input_file_name = optarg;
        break;
      case 'd':
        if (mode_flag != 0) {
          fprintf(stderr, "Please choose to either encode or decode.");
          return MODE_ERROR;
        }
        mode_flag = DECODE_MODE;
        input_file_name = optarg;
        break;
      case 'o':
        output_file_name = optarg;
        break;
      case 'k':
        if (key_flag != 0) {
          fprintf(stderr, "You are trying to both generate and import a key!");
          return KEY_IMPORT_ERROR;
        }
          key_flag = IMPORT_KEY;
          key_file_name = optarg;
        break;
      case 'K':
        if (key_flag != 0) {
          fprintf(stderr, "You are trying to both generate and import a key!");
          return KEY_IMPORT_ERROR;
        }
          key_flag = GENERATE_KEY;
          key_file_name = optarg;
        break;
      case '?':
        if (optopt == 'o') {
          fprintf(stderr, "You must provide a file name to %c.\n", optopt);
          return NO_FILE_NAME_ERROR;
        }
        else {
          fprintf(stderr, "Unknown options %c. \n", optopt);
          return UNKNOWN_OPTIONS_ERROR;
        }
    }
  }

  if (!mode_flag) {
    fprintf(stderr, "Please select to either encode or decode.");
    return MODE_ERROR;
  }
  if (!key_flag) {
    fprintf(stderr, "Please select to import or generate a new key.");
    return KEY_IMPORT_ERROR;
  }

  FILE* input_stream = stdin;
  FILE* output_stream = stdout;

  if (input_file_name != NULL) {
    input_stream = fopen(input_file_name, "r");
    if (input_stream == NULL) {
      perror("Error opening input");
      return FILE_OPEN_ERROR;
    }
  }
  if (output_file_name != NULL) {
    output_stream = fopen(output_file_name, "w");
    if (output_stream == NULL) {
      perror("Error opening output");
      return FILE_OPEN_ERROR;
    }
  }

  FILE* key_file;
  uint64_t key[] = {0, 0};
  if (key_flag == IMPORT_KEY) {
    key_file = fopen(key_file_name, "r");
    
    if (!key_file) {
        perror("Error opening key");
        return FILE_OPEN_ERROR;
    }
    
    for (int i=0; i<16; i++) {
      if (feof(key_file)) {
        perror("Key file is not long enough!");
        return KEY_TOO_LONG;
      }
      char c = fgetc(key_file);
      if (i < 8) {
        key[0] ^= ((uint64_t)c & 0xff) << (120 - i*8);
      }
      else {
        key[1] ^= ((uint64_t)c & 0xff) << (120 - i*8);
      }
    }
    fclose(key_file);
  }
  else if (key_flag == GENERATE_KEY) {
    perror("Not implemented");
    return 100;
  }

  // replace with real procedure to get key
  //uint64_t key[] = { 0x1122334455667788, 0x0123456789abceff };

  if (mode_flag == ENCODE_MODE) {
    uint64_t nonce[] = {0, 0};
    get_rand_128(nonce);
    tc39_ctr_encrypt(input_stream, output_stream, key, nonce);
  }
  else if (mode_flag == DECODE_MODE) {
    tc39_ctr_decrypt(input_stream, output_stream, key);
  }

  fclose(input_stream);
  fclose(output_stream);
}

