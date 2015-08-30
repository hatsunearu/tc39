#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <getopt.h>

#include "tc39-errorcodes.h"

#include "tc39-crypto.c"
#include "tc39-random.c"

int main(int argc, char** argv) {

  char c;
  char* input_file_name = NULL;
  char* output_file_name = NULL;
  int decode_flag = 0;
  int encode_flag = 0;

  while ((c = getopt(argc, argv, "edi:o:")) != -1) {
    switch (c) {
      case 'e':
        encode_flag = 1;
        break;
      case 'd':
        decode_flag = 1;
        break;
      case 'i':
        input_file_name = optarg;
        break;
      case 'o':
        output_file_name = optarg;
        break;
      case '?':
        if (optopt == 'i' || optopt == 'o') {
          fprintf(stderr, "You must provide a file name to %c.\n", optopt);
          return NO_FILE_NAME_ERROR;
        }
        else {
          fprintf(stderr, "Unknown options %c. \n", optopt);
          return UNKNOWN_OPTIONS_ERROR;
        }
    }
  }

  if (encode_flag == decode_flag) {
    fprintf(stderr, "Please select to encode or decode.");
    return NO_ACTION_ERROR;
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

  // replace with real procedure to get key
  uint64_t key[] = { 0x1122334455667788, 0x0123456789abceff };

  if (encode_flag) {
    uint64_t nonce[] = {0, 0};
    get_rand_128(nonce);
    encrypt(input_stream, output_stream, key, nonce);
  }
  else if (decode_flag) {
    decrypt(input_stream, output_stream, key);
  }

  fclose(input_stream);
  fclose(output_stream);
}

