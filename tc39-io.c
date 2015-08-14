#include <stdio.h>
#include <stdlib.h>

uint64_t open_file(char* filename, char** ptr) {

    FILE* fp = fopen(filename, "r");

    if (fp != NULL) {
        if (fseek(fp, 0L, SEEK_END) == 0) {
            uint64_t bufsize = (uint64_t)ftell(fp);
            if (bufsize == -1) {
                fprintf(stderr, "Error while seeking file.");
                return 0;
            }

            uint64_t buffersize = 0;

            //sized such that input data is already block-aligned
            if ((sizeof(char) * bufsize) % (2*sizeof(uint64_t)) == 0) {
                buffersize = bufsize * sizeof(char);
            }
            //sized such that input data is not block-aligned, must be padded
            else {
                buffersize = (bufsize * sizeof(char) / (2*sizeof(uint64_t)) + 1) * 2*sizeof(uint64_t);
            }

            //malloc a buffer
            *ptr = (char*)malloc(buffersize);

            //zero-pad stuff after the actual text data
            for (int i = bufsize * sizeof(char); i < buffersize; i++) {
                (*ptr)[i] = 0;
            }

            if (fseek(fp, 0L, SEEK_SET) != 0) { 
                fprintf(stderr, "Error while rewinding file.");
                return 0;
            }
            size_t newLen = fread(*ptr, sizeof(char), bufsize, fp);
            if (newLen == 0) {
                fprintf(stderr, "Error while allocating memory");
                return 0;
            }
            
            
            for (int i = 0; i < buffersize; i++) {
                printf("%c", (*ptr)[i]);
            }
            return buffersize;
        }
    }
    else {
        fprintf(stderr, "Error while accessing file.");
    }
    return 0;
}

int write_file(char* ptr) {
    return -1;
}

