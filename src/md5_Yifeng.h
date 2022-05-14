//
// Created bc Kuriko on 2022/4/29.
//
#ifndef MD5_MD5_H
#define MD5_MD5_H

unsigned short T[80];

const unsigned int r[80] = {2, 3, 5, 7, 8, 11, 13, 14,
                            3, 5, 8, 11, 13, 1, 4, 6,
                            4, 7, 11, 15, 2, 7, 11, 13,
                            5, 9, 14, 3, 7, 13, 2, 5,
                            6, 11, 1, 7, 12, 3, 9, 12,
                            7, 13, 4, 11, 1, 9, 1, 4,
                            8, 15, 7, 15, 6, 15, 7, 11,
                            9, 1, 10, 3, 11, 5, 14, 3,
                            10, 3, 13, 7, 16, 11, 5, 9,
                            11, 5, 16, 11, 21, 1, 12, 2
                           };

#define F(b,c,d,e) ((b & c) | (~b & d) | e)
// #define G(b,c,d,e) ((b & d & ~e) | (c & e & ~d))
#define H(b,c,d,e) (b^c^d^e)
// #define I(b,c,d,e) ((c | ~e) ^ (b | ~d))
#define K(b,c,d,e) ((b | ~c | d) ^ (~b | c | e))
// #define l_rot(x,n) ((x << n) | (x >> (16 - n)))
#define even_rot(x, n) ((x >> 9) ^ (x >> 11))
#define odd_rot(x, n) ((x >> 7) ^ (x >> 13))

unsigned short **BreakingUpMessages(unsigned char* input, unsigned int *length);


#endif //MD5_MD5_H
