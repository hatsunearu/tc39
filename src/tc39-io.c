#include <stdio.h>
#include <stdlib.h>

typedef enum {BLOCK, CHAR} align_t;

uint64_t open_file(char* filename, char** ptr, align_t align) {

    FILE* fp = fopen(filename, "r");

    if (fp != NULL) {
        if (fseek(fp, 0L, SEEK_END) == 0) {
            uint64_t bufsize = (uint64_t)ftell(fp);
            if (bufsize == -1) {
                fprintf(stderr, "Error while seeking file.");
                fclose(fp);
                return 0;
            }

            uint64_t buffersize = 0;

            if (align == BLOCK) {
                printf("bufsize: %lld\n", bufsize);
                //sized such that input data is already block-aligned
                if (bufsize % (2*sizeof(uint64_t)) == 0) {
                    buffersize = bufsize;
                }
                //sized such that input data is not block-aligned, must be padded
                else {
                    buffersize = (bufsize / (2*sizeof(uint64_t)) + 1) * 2*sizeof(uint64_t);
                }
            }
            else if (align == CHAR) {
                buffersize = bufsize + 1; //extra for \0 at the end
            }

            //malloc a buffer
            *ptr = (char*)malloc(buffersize);

            //zero-pad stuff after the actual text data
            for (int i = bufsize; i < buffersize; i++) {
                (*ptr)[i] = 0;
            }

            if (fseek(fp, 0L, SEEK_SET) != 0) { 
                fprintf(stderr, "Error while rewinding file.");
                fclose(fp);
                return 0;
            }
            size_t newLen = fread(*ptr, sizeof(char), bufsize, fp);
            if (newLen == 0) {
                fprintf(stderr, "Error while allocating memory");
                fclose(fp);
                return 0;
            }
            
            
            for (int i = 0; i < buffersize; i++) {
                printf("%c", (*ptr)[i]);
            }
            fclose(fp);
            return buffersize;
        }
    }
    else {
        fprintf(stderr, "Error while accessing file.");
        fclose(fp);
    }
    fclose(fp);
    return 0;
}

int write_file(char* ptr) {
    return -1;
}

