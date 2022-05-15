#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "FWMD.h"

unsigned short h0 = 0x6745;
unsigned short h1 = 0xefcd;
unsigned short h2 = 0x98ba;
unsigned short h3 = 0x1032;
unsigned short h4 = 0xdcfe;

unsigned short **BreakingUpMessages(unsigned char* input, unsigned int *length) {
    unsigned int len = strlen(input);
    unsigned int t = len  & 0x3f;   //bits mod 512
    unsigned int pad = t > 56 ? 120 - t : 56 - t;  // 补位
    unsigned int len_p = len + pad + 8;
    input = (unsigned char*)realloc(input, len_p);

    input[len] = 0x80;  // 10000000
    for (unsigned int i = 1; i < pad; i++){
        input[len + i] = 0;
    }

    //the length
    unsigned int j = 0;
    unsigned int len_bit = len * 8;
    for(unsigned int i = 0; i < 4; i++) {
        input[len + pad + i] = (len_bit >> j) & 0xff;  // j = 0, 8, 16, 24
        j += 8;
    }
    for(unsigned int i = 0; i < 4; i++) {
        input[len + pad + 4 + i] = 0;
    }

    *length = len_p / 64;
    unsigned short **output = (unsigned short**)malloc(sizeof(unsigned short*) * (len_p / 64));
    for (unsigned int i = 0; i < *length; i++) {
        unsigned short* block = (unsigned short*)malloc(sizeof(unsigned short) * 32);
        unsigned int k = 0;
        unsigned int j = i * 64;
        unsigned int j_ = (i + 1) * 64;
        for(j; j < j_; j+=4) {
            unsigned int block_512 = input[j] | input[j + 1] << 8 | input[j + 2] << 16 | input[j + 3] << 24;
            block[k] = (unsigned short)block_512;
            block[k+1] = (unsigned short)(block_512>>4);
            k = k+2;
        }
        output[i] = block;
    }

    return output;
}

unsigned int little_e(unsigned short x) {
    unsigned short r = 0;
    for(unsigned int i = 0; i < sizeof(x); i++) {
        r = (r << 8) | (x & 0xff);
        x = x >> 8;
    }
    return r;
}

void outputBinary(unsigned short h, FILE *fp){
    unsigned short x = h;
    unsigned short* output = (unsigned short*)malloc(sizeof(unsigned short) * 16);
    for(int i = 0; i < 16; ++i){
        output[15-i] = x % 2;
        x = x/2;
    }
    for(int i=0;i<16;i++) {
        fprintf(fp, "%d", output[i]);
    }
    free(output);
}


void hash(unsigned char* buf) {
    unsigned int len = 0;
    unsigned short** blocks = BreakingUpMessages(buf, &len);

    //每个512 bits的块
    for(unsigned int i = 0; i < len; i++) {
        unsigned short a = h0;
        unsigned short b = h1;
        unsigned short c = h2;
        unsigned short d = h3;
        unsigned short e = h4;
        unsigned short* block = blocks[i];

        for(unsigned int j = 0; j < 96; j++) {
            unsigned short f = 0;
            unsigned int g = 0;
            if (j <= 31) {
                f = F(b, c, d, e);
                g = j;
            }
            else if (j >= 32 && j <= 63) {
                f = G(b, c, d, e);
                g = (11 * j + 7) & 0x1f;
            }
            else if (j >= 64 && j <= 95) {
                f = H(b, c, d, e);
                g = (5 * j + 1) & 0x1f;
            }
            
            unsigned short tmp = e;
            e = d;
            d = (c << r[j]) | (c >> (16-r[j]));
            c = b;
            b = l_rot((a >> r[j]) + f + T[j] + block[g], r[j]) + b;
            a = tmp;
        }
        h0 = h0 + a;
        h1 = h1 + b;
        h2 = h2 + c;
        h3 = h3 + d;
        h4 = h4 + e;
    }

    for(int i=0;i<len;i++) {
        free(blocks[i]);
    }
    free(blocks);
}



void random_string_into(unsigned char * buf, int len)
{
    unsigned char * possible_chars = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int possible_chars_len = -1;

    if (possible_chars_len == -1) {
        possible_chars_len = strlen(possible_chars);
    }

    for (int s = 0; s < len; s++) {
        int index_p = rand() % possible_chars_len;
        buf[s] = possible_chars[index_p];
    }
}

int main(int argc, char **argv) {
    int test_mode; // 0: for efficiency test; 1: for NIST test; 2: for normal use
    if (strcmp(argv[1], "0") == 0) {
        test_mode = 0;
    }
    else if (strcmp(argv[1], "1") == 0) {
        test_mode = 1;
    }
    else if (strcmp(argv[1], "2") == 0) {
        FILE *p=fopen("test.txt", "r");
        if (p == NULL) {
            printf("open %s failed!\n", argv[1]);
            return -1;
        }
        int buffer_size = 20;
        unsigned char *buf = (unsigned char *)malloc(buffer_size);
        fread(buf, 1, buffer_size, p);
        h0 = 0x6745;
        h1 = 0xefcd;
        h2 = 0x98ba;
        h3 = 0x1032;
        h4 = 0xdcfe;
        hash(buf);
        printf("%04x%04x%04x%04x%04x\n", little_e(h0), little_e(h1), little_e(h2), little_e(h3), little_e(h4));
        return 0;
    }
    else {
        exit(-1);
    }

    long long t_16 = 0x10000000;
    for(unsigned int i = 0; i < 96; i++) {
        T[i] = floor(fabs(sin(i + 1)) * t_16);
    }
    
    FILE *fp=fopen("hash.txt","w");
    double total_time = 0.0;
    int test_case_num, buffer_size;
    if (test_mode == 0) {
        test_case_num = 10;
        buffer_size = 200000000;
    }
    else if (test_mode == 1) {
        test_case_num = 1250000;
        buffer_size = 10000;
    }

    for(int i=0;i<test_case_num;i++) {
        h0 = 0x6745;
        h1 = 0xefcd;
        h2 = 0x98ba;
        h3 = 0x1032;
        h4 = 0xdcfe;
        unsigned char *buf = (unsigned char *)malloc(buffer_size);
        random_string_into(buf, buffer_size);
        clock_t start = clock();
        hash(buf);
        clock_t end = clock();
        free(buf);
        total_time = total_time + ((double)buffer_size/ (end - start) * CLOCKS_PER_SEC) / 125000.0;
        if (test_mode == 1) {
            outputBinary(little_e(h0), fp);
            outputBinary(little_e(h1), fp);
            outputBinary(little_e(h2), fp);
            outputBinary(little_e(h3), fp);
            outputBinary(little_e(h4), fp);
            if(i>0 && i%1000==0) {
                printf("%d\n", i);
            }
        }
        if (test_mode == 0) {
            printf("%04x%04x%04x%04x%04x\n", little_e(h0), little_e(h1), little_e(h2), little_e(h3), little_e(h4));
        }
    }

    fclose(fp);

    if (test_mode == 0) {
        double time = total_time / (double)test_case_num;
        printf("Speed: %f Mbps\n", time);
    }
    return 0;
}
