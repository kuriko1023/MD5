#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "md5_Yifeng.h"

#define BUFFER_SIZE 10000

unsigned short h0 = 0x6745;
unsigned short h1 = 0xefcd;
unsigned short h2 = 0x98ba;
unsigned short h3 = 0x1032;
unsigned short h4 = 0xdcfe;

/*第78行的字符串为加密内容
 * */

/*填充串
输入初始字符串，输入补位和填充长度后的Md5块（512 bits)*/
unsigned short **MD5Pack(unsigned char* input, unsigned int *length) {
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
        unsigned short* md5Block = (unsigned short*)malloc(sizeof(unsigned short) * 32);
        unsigned int k = 0;
        unsigned int j = i * 64;
        unsigned int j_ = (i + 1) * 64;
        for(j; j < j_; j+=4) {
            unsigned int md5_block_512 = input[j] | input[j + 1] << 8 | input[j + 2] << 16 | input[j + 3] << 24;
            md5Block[k] = (unsigned short)md5_block_512;
            md5Block[k+1] = (unsigned short)(md5_block_512>>4);
            k = k+2;
        }
        output[i] = md5Block;
    }

    return output;
}

/*转化为小端编码*/
unsigned int little_e(unsigned short x) {
    unsigned short r = 0;
    for(unsigned int i = 0; i < sizeof(x); i++) {
        r = (r << 8) | (x & 0xff);
        x = x >> 8;
    }
    return r;
}


void hash(unsigned char* buf) {
    unsigned int len = 0;
    unsigned short** md5Blocks = MD5Pack(buf, &len);

    //每个512 bits的块
    for(unsigned int i = 0; i < len; i++) {
        unsigned short a = h0;
        unsigned short b = h1;
        unsigned short c = h2;
        unsigned short d = h3;
        unsigned short e = h4;
        unsigned short* md5Block = md5Blocks[i];

        for(unsigned int j = 0; j < 80; j++) {
            unsigned short f = 0;
            unsigned int g = 0;
            if (j <= 31) {
                f = F(b, c, d, e);
                g = (5 * j + 1) & 0x1f;
            }
            else if (j >= 32 && j <= 63) {
                f = K(b, c, d, e);
                g = (11 * j + 7) & 0x1f;
            }
            else if (j >= 64 && j <= 79) {
                f = H(b, c, d, e);
                g = 2*j & 0x1f;
            }
            
            unsigned short tmp = e;
            e = d;
            d = c;
            c = b;
            if(j % 2 == 0) {
                b = even_rot(a + f + T[j] + md5Block[g]) + b;
            }
            else {
                b = odd_rot(a + f + T[j] + md5Block[g]) + b;
            }
            a = tmp;
        }
        h0 = h0 + a;
        h1 = h1 + b;
        h2 = h2 + c;
        h3 = h3 + d;
        h4 = h4 + e;
    }
}


int main(int argc, char **argv) {
    long long t_32 = 0x100000000;
    for(unsigned int i = 0; i < 80; i++) {
        T[i] = floor(fabs(sin(i + 1)) * t_32);
    }

    FILE *p=fopen(argv[1], "r");
    if (p == NULL) {
        printf("open %s failed!\n", argv[1]);
        return -1;
    }
 
    unsigned char *buf = (unsigned char *)malloc(BUFFER_SIZE);
    if (buf == NULL) {
        printf("malloc %d failed!\n", BUFFER_SIZE);
        fclose(p);
        return -1;
    }

    fread(buf, 1, BUFFER_SIZE, p);

    clock_t start = clock();

    hash(buf);

    clock_t end = clock();

    //输出密文
    double time = ((double)BUFFER_SIZE / (end - start) * CLOCKS_PER_SEC) / 125000;
    printf("Speed: %f Mbps\n", time);
    printf("%04x%04x%04x%04x%04x\n", little_e(h0), little_e(h1), little_e(h2), little_e(h3), little_e(h4));
    return 0;
}
