# 大作业报告

## 设计理念
FWMD（Five-Word based Message-Digest algorithm）算法是一种采用MD迭代结构、基于五个寄存器设计的哈希函数。FWMD算法的输入为任意长度的信息，输出为80位的消息摘要。利用移位、XOR、AND、OR、NOT等操作，FWMD从五个寄存器的初始值出发，利用预先设计的逻辑函数和输入消息的对应部分，生成最终的散列值。

在设计过程中，我们希望FWMD算法可以达到三点要求：（1）压缩性，（2）高效性，以及（3）安全性。为了实现压缩性，我们保证对于任意长度的输入信息，FWMD算法的输出均为长度为80位的消息摘要。为了实现高效性，FWMD算法采用了较为容易的计算方式，保证了预算效率。而对于安全性，FWMD算法采用了较为复杂的压缩函数设计和较多步数的运算，以期望获得较高的安全性。

## 算法组成

### 消息填充与打包

### 压缩函数
基于MD迭代结构，针对每一个512位的分组，FWMD算法使用了一个由三轮（96步）运算组成的压缩函数。压缩函数的整体结构如下图所示：

压缩函数的每一轮均由32步迭代组成，其中每一步迭代的过程如下：
* a ← e
* b ← l1(l2(a, s2) + F(b,c,d,e) + T[i] + X[i], s1) + b
* c ← b
* d ← l3(c, s3)
* e ← d

其中，a、b、c、d和e为FWMD算法使用的五个寄存器，而压缩函数就是利用不同的逻辑函数和消息数据，在这五个寄存器上进行迭代计算。

l1、l2和l3是专门将寄存器中的值进行移位操作的函数，设需要移动的位数为s，则：l1 = l3 = (x << s | x >> (16 - s))，l2 = (x >> s)。虽然移位操作是相对耗时的运算方式，但多次使用移位操作可以有效提升算法的复杂性，从而提升哈希函数的安全性。

F代表了FWMD算法所使用的逻辑函数。我们针对压缩函数的三轮运算设计了三种不同的逻辑函数，具体如下：
* F(b,c,d,e) = ((b & c) | (~b & d) | e)
* G(b,c,d,e) = ((b | ~c | d) ^ (~b | c | e))
* H(b,c,d,e) = (b ^ c ^ d ^ e)

在设计逻辑函数的过程中，我们有意识地多次运用了XOR、AND、OR、NOT等逻辑运算，以保证逻辑函数的复杂性。

沿用了MD5的设计，T[i]采用了正弦函数的构造方式，以实现对输入数据的规律性的消除。X[i]则代表了消息对应分组的第一个16位字。

## 效率测试

### 测试思路
硬件平台：Intel(R) Xeon(R) CPU E5-2630 v3 @ 2.40GHz (Linux)

样本数据：我们随机生成了10条长度为200000000字符的输入信息，分别计算哈希函数的运行时间（十次），最后计算平均效率。

测试算法实现：测试代码中读取样本数据文件后调用核心Hash函数并输出时间，结合样本数据长度得到算法的基本效率值。尽可能减少计时阶段如输入输出等不必要的操作，以进一步降低测试误差，通过改变样本数据内容及长度得到较平均的结果。整个测试过程仅涉及文件读取及Hash值计算，且计时阶段仅包括Hash算法的核心步骤（消息填充与打包、压缩函数），并无过多与操作系统、软件平台、开发语言相关的操作，故认为测试结果具有一定普遍性，且误差相对较小。

### 实验结果

实验表明，FWMD算法的运行效率约为 392.22 Mbps。具体的测试方式在代码的README文档中说明。

## 随机性检测

我们使用了NIST来对FWMD算法进行随机性检测，具体的实验结果在finalAnalysisReport.txt和freq.txt中。具体的测试方式在代码的README文档中说明。

根据freq.txt，我们可以清楚知道FWMD算法生成的散列值中的0和1的比例大约50:50，因此是随机的。

我们在随机性检测中所使用的参数如下图所示：

finalAnalysisReport.txt中有实验结果的详细描述，我们在此只对部分测试进行简要分析：

### Frequency(monobit) Test
此测试是为了测试0和1在此哈希值序列的占比，看0和1的占比是否接近1:1。如果p-value >= 0.01，就代表此序列0和1的数量都是各别占此序列的1/2。

在finalAnalysisReport.txt中，可以发现到frequency(monobit) test的p-value是0.191687，大于0.01。

### Frequency Test With Block
此测试是为了把哈希值序列分为M-bit blocks，然后观察1的占比。这里我们的M设为80，也就是每80个bit为一组，这是因为哈希值为80比特，所以相当于检测每一个哈希值中1的占比。理想情况下随机序列中1的占比应该为M/2。

在finalAnalaysisReport.txt中，可以发现frequency block test中的p-value为0.960583，大于0.01。

### Cumulative Sum Test
此测试是为了测试序列中的累积和（-1, +1)，是否太大还是太小。此测试就如一种random walk，理想情况下，对于一个随机序列累积和应该为0。

在finalAnalysisReport.txt中，可以发现cumulative frequency test中的p-value分别为0.494392和0.526105, 均大于0.01。

### Run Test
首先，先定义run，一个长度为k的run 也就代表它包含k个1或k个0。比如说0011110000,这里就共有三个run，而第一个run是长度为2的0’s run，第二个是长度为4的1’s run，第三个是长度为4的0’s run。他们都是0’s run, 1’s run交替而成的，但每个run的长度不限。

此测试是为了测试0’s run和1’s run的数量是不是所期望的，主要是判断0和1之间的振动是否太快还是太慢。

在finalAnalysisReport.txt中，可以发现run test中的p-value为0.426272，大于0.01。

### Longest Run
此测试和上面的run test类似，只是现在是M-bit blocks为一组。而M大小为多少取决于n也就是你的数据长度，这是NIST规定了。我们的每一组的长度是100000，也就是介于6272和750000之间，所以我们M是128。

在finalAnalysisReport.txt中，可以发现longest run test中的p-value为0.003224。

### Binary Matrix Rank Test
此测试主要是检测特定长度的子字符串的linear dependence。

在finalAnalaysisReport.txt中，可以发现Rank中的p-value为0.292519，大于0.01。

### FFT
此测试是用到DFT(Discrete Fourier Transform)，检测每个period的变化，是否存在同样的形式，还是说每个period变化很大。

在finalAnalysisReport.txt中，可以发现FFT中的p-value为0.146982，大于0.01。

### Serial
此测试是为了测试是否每m-bit pattern有同样的机会出现在其他m-bit pattern。和frequency test是等价的。

在finalAnalysisReport.txt中，可以发现倒数第三行和倒数第二行p-value分别为0.510153和0.215574, 大于0.01。

### Linear Complexity
此测试是为了测试此序列如果被认定为随机序列，是否过于复杂。主要是采用了LFSR(linear feedback shift register)。

在finalAnalysisReport.txt中，可以发现倒数第一行p-value为0.195864, 大于0.01。

