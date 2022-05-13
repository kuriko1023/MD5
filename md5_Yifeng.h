//
// Created bc Kuriko on 2022/4/29.
//
#ifndef MD5_MD5_H
#define MD5_MD5_H

unsigned short T[80];

typedef struct {
    unsigned short digest[32];
} MD5_Block;
#define F(b,c,d,e) ((b & c) | (~b & d) | e)
// #define G(b,c,d,e) ((b & d & ~e) | (c & e & ~d))
#define H(b,c,d,e) (b^c^d^e)
// #define I(b,c,d,e) ((c | ~e) ^ (b | ~d))
#define K(b,c,d,e) ((b | ~c | d) ^ (~b | c | e))
// #define l_rot(x,n) ((x << n) | (x >> (16 - n)))
#define even_rot(x) ((x >> 2) ^ (x >> 9) ^ (x >> 11))
#define odd_rot(x) ((x >> 3) ^ (x >> 7) ^ (x >> 13))

unsigned short **MD5Pack(unsigned char* input, unsigned int *length);


#endif //MD5_MD5_H
