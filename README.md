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

##随机性检测
* 去sts-2.1.2的folder下可以看见有assess
* ./assess <stream length> (以bit为单位）(数据段长度)(应该至少大于1M)
* 输入 0 (Input File)
* 然后打你的file
* 然后会遇到Enter Choice : 打0 
* 111010000011000 打这个
* 会遇到Select Test(0 to continue): 输入1 然后输入80， 再输入2， 然后输入80， 最后输入0
* how many bitstreams ? 要检测的数
* input format 的话？ 是 1 因为我们哈希值不只是只有 0 和 1 组成
* 可以去experiment folder -> Algorithm Testing folder -> 看到finalreportanalysis 还有freq.txt
