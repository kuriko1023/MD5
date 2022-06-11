#include "uwp.h"
#include <ctime>
#include <string>
#include<iostream>
#include <cstring>
using std::string;

UWP1::UWP1() {}

void UWP1::init() {
    this->BUF[0] = 0x20190102;
    this->BUF[1] = 0x29202001;
    this->BUF[2] = 0x23539876;
    this->BUF[3] = 0x54321234;
}

string UWP1::hash(const string& message) {

    init();

    size_t len = message.length();
    uint8_t* ptr = (uint8_t*)message.c_str();
    int BlockNum = int((len + 8) / 64) + 1; // get the block number after padding

    // put the last block aside
    for (int i = 0; i < BlockNum - 1; i++) {
        memset(BLOCK, 0, 64);
        if (64 * (i + 1) <= len) {
            memcpy(BLOCK, ptr + 64 * i, 64);
        }
        else {
            int rest_len = len - 64 * i;
            memcpy(BLOCK, ptr + 64 * i, rest_len);
            BLOCK[rest_len] = 0x80;     // padding: first bit = 1, others = 0
        }
        transform(BLOCK);
    }

    // the last block
    memset(BLOCK, 0, 64);
    if (len % 64 == 0) BLOCK[0] = 0x80;     // in case that the padding is in the last block
    *(uint32_t*)(BLOCK + 56) = len;        // little endian
    transform(BLOCK);

    // take the first 10 bytes of BUF (total 16 bytes) as output
    memcpy(output, BUF, 10);

    // print to the terminal
    // printf("Hash = 0x");
    string res;
    for (int i = 0; i < sizeof(output); i++) {
        // printf("%.2x", output[i]);
        char* tmp = new char[5];
        sprintf(tmp, "%.2x", output[i]);
        res += string(tmp);
    }
    // printf("\n");
    return res;
}

void UWP1::transform(uint8_t block[64]) {

    uint8_t* buf = (uint8_t*)BUF;

    for (int step_num = 0; step_num < 16; step_num++) {

        memset(res, 0, sizeof(res));
        
        uint8_t* msg = block + 4 * step_num;

        uint8_t tab[6][6] = { \
            { *msg,       *buf,        *(msg + 1),       *(msg + 2), *(buf + 1),  *(msg + 3)  },
            { *(buf + 4), *(msg + 3),  *(buf + 8),       *(buf + 5), *(msg),      *(buf + 9)  },
            { *(msg + 1), *(buf + 12), *(msg + 2),       *(msg + 3), *(buf + 13), *(msg)      },
            { *(msg + 3), *(buf + 2),  *msg,             *(msg + 1), *(buf + 3),  *(msg + 2)  },
            { *(buf + 6), *(msg + 1),  *(buf + 10),      *(buf + 7), *(msg + 2),  *(buf + 11) },
            { *(msg + 2), *(buf + 14), *(msg + 3),       *(msg),     *(buf + 15), *(msg + 1)  },
        };

        uint8_t MAT_8_1[9][8] = {
            { tab[0][0], tab[0][1], tab[0][2], tab[0][3], tab[1][0], tab[1][1], tab[1][2], tab[1][3] },
            { tab[0][1], tab[0][2], tab[0][3], tab[0][4], tab[1][1], tab[1][2], tab[1][3], tab[1][4] },
            { tab[0][2], tab[0][3], tab[0][4], tab[0][5], tab[1][2], tab[1][3], tab[1][4], tab[1][5] },
            { tab[1][0], tab[1][1], tab[1][2], tab[1][3], tab[2][0], tab[2][1], tab[2][2], tab[2][3] },
            { tab[1][1], tab[1][2], tab[1][3], tab[1][4], tab[2][1], tab[2][2], tab[2][3], tab[2][4] },
            { tab[1][2], tab[1][3], tab[1][4], tab[1][5], tab[2][2], tab[2][3], tab[2][4], tab[2][5] },
            { tab[2][0], tab[2][1], tab[2][2], tab[2][3], tab[3][0], tab[3][1], tab[3][2], tab[3][3] },
            { tab[2][1], tab[2][2], tab[2][3], tab[2][4], tab[3][1], tab[3][2], tab[3][3], tab[3][4] },
            { tab[2][2], tab[2][3], tab[2][4], tab[2][5], tab[3][2], tab[3][3], tab[3][4], tab[3][5] }
        };

        uint8_t MAT_8_2[9][8] = {
            { tab[2][0], tab[2][1], tab[2][2], tab[2][3], tab[3][0], tab[3][1], tab[3][2], tab[3][3] },
            { tab[2][1], tab[2][2], tab[2][3], tab[2][4], tab[3][1], tab[3][2], tab[3][3], tab[3][4] },
            { tab[2][2], tab[2][3], tab[2][4], tab[2][5], tab[3][2], tab[3][3], tab[3][4], tab[3][5] },
            { tab[3][0], tab[3][1], tab[3][2], tab[3][3], tab[4][0], tab[4][1], tab[4][2], tab[4][3] },
            { tab[3][1], tab[3][2], tab[3][3], tab[3][4], tab[4][1], tab[4][2], tab[4][3], tab[4][4] },
            { tab[3][2], tab[3][3], tab[3][4], tab[3][5], tab[4][2], tab[4][3], tab[4][4], tab[4][5] },
            { tab[4][0], tab[4][1], tab[4][2], tab[4][3], tab[5][0], tab[5][1], tab[5][2], tab[5][3] },
            { tab[4][1], tab[4][2], tab[4][3], tab[4][4], tab[5][1], tab[5][2], tab[5][3], tab[5][4] },
            { tab[4][2], tab[4][3], tab[4][4], tab[4][5], tab[5][2], tab[5][3], tab[5][4], tab[5][5] }
        };

        uint64_t *MAT_64_1 = (uint64_t*)MAT_8_1;
        uint64_t *MAT_64_2 = (uint64_t*)MAT_8_2;
        uint64_t *res_64 = (uint64_t*)res;

        *res_64 = *(MAT_64_1) ^ conv_kernel_64[step_num][0][0] + *(MAT_64_1 + 1) ^ conv_kernel_64[step_num][0][1] + *(MAT_64_1 + 2) ^ conv_kernel_64[step_num][0][2] + *(MAT_64_1 + 3) ^ conv_kernel_64[step_num][1][0] + *(MAT_64_1 + 4) ^ conv_kernel_64[step_num][1][1] + *(MAT_64_1 + 5) ^ conv_kernel_64[step_num][1][2] + *(MAT_64_1 + 6) ^ conv_kernel_64[step_num][2][0] + *(MAT_64_1 + 7) ^ conv_kernel_64[step_num][2][1] + *(MAT_64_1 + 8) ^ conv_kernel_64[step_num][2][2];
        *(res_64 + 1) = *(MAT_64_2) ^ conv_kernel_64[step_num][0][0] + *(MAT_64_2 + 1) ^ conv_kernel_64[step_num][0][1] + *(MAT_64_2 + 2) ^ conv_kernel_64[step_num][0][2] + *(MAT_64_2 + 3) ^ conv_kernel_64[step_num][1][0] + *(MAT_64_2 + 4) ^ conv_kernel_64[step_num][1][1] + *(MAT_64_2 + 5) ^ conv_kernel_64[step_num][1][2] + *(MAT_64_2 + 6) ^ conv_kernel_64[step_num][2][0] + *(MAT_64_2 + 7) ^ conv_kernel_64[step_num][2][1] + *(MAT_64_2 + 8) ^ conv_kernel_64[step_num][2][2];

        BUF[0] = (uint32_t)((res[1][3] << 24) + (res[1][0] << 16) + (res[1][1] << 8) + res[1][2] + consts[step_num]) & 0xffffffff;
        BUF[1] = (res[2][2] << 24) + (res[2][3] << 16) + (res[2][0] << 8) + res[2][1];
        BUF[2] = (res[3][1] << 24) + (res[3][2] << 16) + (res[3][3] << 8) + res[3][0];
        BUF[3] = (res[0][0] << 24) + (res[0][1] << 16) + (res[0][2] << 8) + res[0][3];
    }
}