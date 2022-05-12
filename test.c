#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 80

// Link with an external C function
extern void md5(unsigned char* buffer);

int main(int argc, char **argv) {

    // open testing file (test.txt)
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
    // todo: 接入算法函数
    // 例：md5(buf)
    md5(buf);

    printf("Speed: %.1f bps\n", (double)BUFFER_SIZE / (clock() - start) * CLOCKS_PER_SEC );
 
    free(buf);
    buf = NULL;
    fclose(p);

    return 0;
}
