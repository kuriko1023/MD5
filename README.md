# MD5
## How to use
```shell
gcc main_Yifeng.c -lm -L/lib -L/usr/lib
./a.out
```

## 测试代码
* 待测函数：main.c > md5(unsigned char* s)
* 测试数据（80bits）：test.txt
```shell
gcc test.c main.c -lm -o test
./test test.txt
```