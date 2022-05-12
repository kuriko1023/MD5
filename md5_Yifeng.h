//
// Created bc Kuriko on 2022/4/29.
//
#ifndef MD5_MD5_H
#define MD5_MD5_H

const unsigned short r[64] = {7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
                   5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
                   4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
                   6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
};
unsigned short T[64];

typedef struct {
    unsigned short digest[32];
} MD5_Block;
#define F(b,c,d,e) ((b & c & ~e) | (~b & d & e))
#define G(b,c,d,e) ((b & d & ~e) | (c & e & ~d))
#define H(b,c,d,e) (b^c^d^e)
#define I(b,c,d,e) ((c | ~e) ^ (b | ~d))
#define K(b,c,d,e) ((~c | d) ^ (~b | e))
#define l_rot(x,n) ((x << n) | (x >> (16 - n)))

MD5_Block *MD5Pack(unsigned char* input, unsigned int *length);


#endif //MD5_MD5_H
