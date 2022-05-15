# 使用FWMD算法
## 测试运行效率
* 我们随机生成了10条长度为200000000字符的输入信息，分别计算哈希函数的运行时间（十次），最后计算平均效率。
* 运行如下指令
```shell
gcc FWMD.c -lm
./a.out 0
```
## 生成随机性检测所需数据
* 在同目录下新建hash.txt文件，用于存放生成的数据。
* 运行如下指令 
```shell
gcc FWMD.c -lm
./a.out 1
```
* 生成结束后，在sts-2.1.2目录下运行如下指令进行随机性检测
```shell
(base) ➜ sts-2.1.2 git:(main) ✗ ./assess 100000
           G E N E R A T O R    S E L E C T I O N 
           ______________________________________
                                           
    [0] Input File                 [1] Linear Congruential
    [2] Quadratic Congruential I   [3] Quadratic Congruential II
    [4] Cubic Congruential         [5] XOR                                            
    [6] Modular Exponentiation     [7] Blum-Blum-Shub          
    [8] Micali-Schnorr             [9] G Using SHA-1
                                           
   Enter Choice: 0
                                                                                      

                User Prescribed Input File: 生成的hash.txt的绝对路径
                                                                                      
                S T A T I S T I C A L   T E S T S
                _________________________________
                                           
    [01] Frequency                       [02] Block Frequency
    [03] Cumulative Sums                 [04] Runs
    [05] Longest Run of Ones             [06] Rank
    [07] Discrete Fourier Transform      [08] Nonperiodic Template Matchings
    [09] Overlapping Template Matchings  [10] Universal Statistical
    [11] Approximate Entropy             [12] Random Excursions 
    [13] Random Excursions Variant       [14] Serial
    [15] Linear Complexity                                                            
                                                                                      
         INSTRUCTIONS
            Enter 0 if you DO NOT want to apply all of the
            statistical tests to each sequence and 1 if you DO.

   Enter Choice: 1

        P a r a m e t e r   A d j u s t m e n t s
        -----------------------------------------
    [1] Block Frequency Test - block length(M):         128
    [2] NonOverlapping Template Test - block length(m): 9
    [3] Overlapping Template Test - block length(m):    9
    [4] Approximate Entropy Test - block length(m):     10
    [5] Serial Test - block length(m):                  16
    [6] Linear Complexity Test - block length(M):       500

   Select Test (0 to continue): 1

   Enter Block Frequency Test block length: 80

        P a r a m e t e r   A d j u s t m e n t s
        -----------------------------------------
    [1] Block Frequency Test - block length(M):         80
    [2] NonOverlapping Template Test - block length(m): 9
    [3] Overlapping Template Test - block length(m):    9
    [4] Approximate Entropy Test - block length(m):     10
    [5] Serial Test - block length(m):                  16
    [6] Linear Complexity Test - block length(M):       500

   Select Test (0 to continue): 0

   How many bitstreams? 1000

   Input File Format:
    [0] ASCII - A sequence of ASCII 0's and 1's
    [1] Binary - Each byte in data file contains 8 bits of data

   Select input mode:  0

     Statistical Testing In Progress.........                                         
                                                                                      
     Statistical Testing Complete!!!!!!!!!!!!
```
## 针对给定输入生成哈希值
* 在同目录下新建test.txt文件，在其中写入一条输入信息（长度对应于FWMD.c中159行的buffer_size值，可进行修改）。
* 运行如下指令 
```shell
gcc FWMD.c -lm
./a.out 2
```