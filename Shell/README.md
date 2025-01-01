* 变量赋值不能有空格，用 `$` 读取变量，变量值都是字符串类型，一般字符串会加双引号标识

```sh
a="hello"
b="world"
c="${a} ${b}"

echo ${c} # hello world
```

* 和双引号不同，单引号内的变量不会被解析

```sh
a="hello"
b="world"
c='${a} ${b}'

echo ${c} # ${a} ${b}

```

* 变量可以重复赋值

```sh
a="abc"
a=123

echo ${a} # 123
```

* 用 unset 删除变量，unset 后变量为空

```sh
a=123
unset a

echo ${a} # 空
```

* 用 readonly 标记的变量不可修改也不可 unset

```sh
readonly a=123
unset a # 错误
```

* 用反引号或 `$()` 获取执行某个命令的结果

```sh
echo `pwd`
echo $(pwd)
```

* 用 `expr` 计算某个表达式的结果
7
```sh
a=1
b=2
c=$(expr ${a} + ${b})

echo $c # 3
```

* 用 `$[]` 或 `$(())` 计算某个表达式的结果

```sh
a=1
b=2
c=$(($a + $b))

echo $c # 3
```

* 用 let 计算某个表达式的结果

```sh
let "a = 1 + 2"
let "b = 3 + 4"
let "c = a + b"

echo $c # 10
```

* 进制转换

```sh
echo $((2#1111)) # 15
```

* 用 `#` 获取字符串长度

```sh
a="hello"

echo ${#a} # 5

# ps aux 会将过长用户名折叠，指定 user 显示宽度为用户名长度加 1 则不会被折叠
ps axo user:$[${#USER} + 1],pid,command | grep -w "$USER" | awk '{print $2}'
```

* 用 `${string:start:len}` 获取字符串子串

```sh
a="hello"
b=${a:0:3} # 从第 0 个字符开始的 3 个字符

echo ${b} # hel
```

* 用 `index` 查询字符串字串第一次出现在第几个字符，如果未找到则为 0

```sh
a="abcde"
echo $(expr index ${a} c)  # 3
echo $(expr index ${a} cd) # 3
echo $(expr index ${a} f)  # 0
```

* 用 `(v0 v1 v2 v3 v4)` 表示数组

```
a=(1 2 3 4 5)

echo ${a[0]} # 1
echo ${a[1]} # 2
```

* 用 `@` 作为下标获取数组所有元素

```sh
a=(1 2 3 4 5)

echo ${a[@]} # 1 2 3 4 5
```

* 数组元素数量

```sh
a=(1 2 3 4 5)

echo ${#a[@]} # 5
```

* 截取子数组

```sh
a=(0 1 2 3 4 5)
b=${a[@]:0:3} # 0 1 2

for i in ${b}; do
    echo ${i}
done
```

* if-then

```sh
a=123
b=456
c=789

if [[ $a -gt $b ]]; then # 注意双中括号的空格
    echo 1
elif [[ $b -gt $c ]]; then
    echo 2
else
    echo 3
fi
```

* 常用运算符如下

```
# 数值比较
-eq 数值相等
-ne 数值不等
-gt 数值大于
-ge 数值大于等于
-lt 数值小于
-le 数值小于等于

# 字符串比较
= 相同字符串
!= 不同字符串
-z 字符串长度为 0
-n 字符串长度不为 0
$ 字符串为空

# 文件属性测试
-b 块设备文件
-c 字符设备文件
-d 目录
-f 普通文件
-p 有名管道文件
-r 可读文件
-w 可写文件
-x 可执行文件
-s 文件大小大于 0
-e 文件存在
```

* for-in

```sh
a=(1 2 3 4 5)

for i in ${a[@]}; do
    echo $i
done
```

* while

```sh
a=0

while true; do
    echo $a
    a=$(expr $a + 1)
    sleep 0.5
done
```

* until

```sh
a=0

until [[ $a -eq 5 ]]; do
    echo $a
    a=$(expr $a + 1)
done
```

* case-in

```sh
a="aaa"

case $a in
"aaa")
    echo 1
    ;;
"bbb")
    echo 2
    ;;
*)
    echo 3
    ;;
esac
```

* 用 `$n` 表示第 n 个参数

```sh
function f() {
    echo "first param: $1"
    echo "second param: $2"
}

f aaa bbb
```

* 用 `$?` 获取上一次调用的函数的返回值，函数返回值只能是 0 到 255 之间的整数，如果不指定返回值则为 0

```sh
function f() {
    return 1
}

function g() {
    return 2
}

f
echo $? # 1

g
echo $? # 2
```

* 用 `$#` 获取参数数量，用 `$@` 或 `$*` 获取所有参数组成的数组

```sh
function f() {
    echo "number of params: $#"

    for i in $@; do
        echo $i
    done

    for i in $*; do
        echo $i
    done
}

f aaa bbb ccc
```


* 特殊的是，如果在 `$*` 外加上双引号，则是一个包含所有参数的字符串

```sh
function f() {
    for i in "$*"; do
        echo $i
    done
}

f aaa bbb ccc
```

* 用 `$n` 获取第 n 个命令行参数，第 0 个命令行参数即为执行的脚本名

```sh
# test.sh
# bash test.sh

echo $0 # test.sh
```

* 用 `$$` 获取 pid

```sh
echo "current pid: $$"
```

* grep 正则表达式匹配

```
grep -P -n "[\x01]35=8[\x01]" <日志名> | grep -P -o "[\x01]58=[^\x01]*" | sort | uniq
```

* 删除当前目录下 7 天前的日志文件

```sh
find . -maxdepth 1 -name "*.log" -type f -mtime +7 -exec rm -f {}\;
```

* awk 对某一列求和

* awk 打印某列到最后一列

```sh
echo 1 2 3 4 5 | awk '{$1="";$2="";sub("  ", "");print}'
3 4 5
```

* awk 跳过空行

```sh
echo "" | awk '{if(!NF){next}}1'
```

* 对某列求和

```sh
echo -e "1,2,3\n4,5,6\n7,8,9" | awk -F',' '{sum += $2} END {print sum}'
15
```

* cut 按逗号分割获取第二列到最后一列

```sh
echo -e "1,2,3\n4,5,6\n7,8,9" | cut -d, -f 2-
2,3
5,6
8,9
```

* cut 删除 csv 文件第 10 列

```sh
cut --complement -f 10 -d, inputfile
```

* xargs 补充前缀

```sh
ls | xargs -i echo `pwd`/{}
```

* sed 保留指定部分

```sh
echo abcde | sed 's/\(.*\)c\(.*\)/\1C\2/'
abCde
```

* sed 在头部插入字符

```sh
echo a | sed 's/^/A/'
Aa
```

* sed 在尾部插入字符

```sh
echo a | sed 's/$/A/'
aA
```

* 字符串转时间戳

```sh
echo 'Thu Aug 19 12:25:33 UTC 2021' | date +%s -f -
1629375933
```

* jq 筛选多个 key

```sh
echo '{"a":1,"b":2,"c":3}' | jq '{a, b}'
{
  "a": 1,
  "b": 2
}
```

* jq 筛选并拼接多个 key 为字符串

```sh
echo '{"a":1,"b":2}' | jq '"a=\(.a), b=\(.b)"'
"a=1, b=2"
```

* jq 显示原生字符串

```sh
echo '{"a":1,"b":2}' | jq -r '"a=\(.a), b=\(.b)"'
a=1, b=2
```

* 创建一个 30 G 的空文件作为交换区

```sh
dd if=/dev/zero of=~/swap bs=1GB count=30
30+0 records in
30+0 records out
30000000000 bytes (30 GB) copied, 59.0548 s, 508 MB/s

mkswap ～/swap
Setting up swapspace version 1, size = 29296868 KiB
no label, UUID=0615767e-455b-414b-8335-55f240741ad3

sudo swapon ~/swap
# 此时交换区将增加
swapon -s
free -m
             total       used       free     shared    buffers     cached
Mem:          3900       2452       1448          0         48       2226
-/+ buffers/cache:        177       3723
Swap:        30656         13      30642
# 如果要删除该交换区
sudo swapoff ~/swap
sudo rm ~/swap
```

* 传文件

```sh
# 接收方
nc -l 50002 > /tmp/test.cpp

# 发送方
nc -N 1.2.3.4 50002 < main.cpp
```

* 测网络吞吐量

```sh
# 接收方
nc -l 50002 > /dev/null

# sudo apt install pv
# nc -l 50002 | pv -W > /dev/null

# 发送方
dd if=/dev/zero bs=1MB count=10 | nc -N 1.2.3.4 50002
```

* 允许接受所有 UDP 端口的数据

```sh
sudo iptables -I INPUT 1 -p udp -j ACCEPT
```

* 添加组播地址到路由表

```sh
sudo route add -net 224.106.2.1 netmask 255.255.255.255 eth0
```

* 超线程技术：Intel CPU 的 2 个逻辑核运行在同一个物理核上，共享物理核的 L1 和 L2 缓存，平均性能提升 20% - 30%，即 32 核比不开超线程的 16 核性能高 30%。但反之，如果单线程使用，由于 L1 和 L2 被共享，将导致 cache miss 增多，性能变差
* 查看 CPU 的命令

```sh
cat /proc/cpuinfo

processor       : 0
vendor_id       : GenuineIntel
cpu family      : 6
model           : 85
model name      : Intel(R) Xeon(R) Silver 4110 CPU @ 2.10GHz
stepping        : 4
microcode       : 0x2000065
cpu MHz         : 2400.003
cache size      : 11264 KB
physical id     : 0
siblings        : 16
core id         : 0
cpu cores       : 8
apicid          : 0
initial apicid  : 0
fpu             : yes
fpu_exception   : yes
cpuid level     : 22
wp              : yes
flags           : fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf pni pclmulqdq dtes64 monitor ds_cpl vmx smx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid dca sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb cat_l3 cdp_l3 invpcid_single pti intel_ppin ssbd mba ibrs ibpb stibp tpr_shadow vnmi flexpriority ept vpid fsgsbase tsc_adjust bmi1 hle avx2 smep bmi2 erms invpcid rtm cqm mpx rdt_a avx512f avx512dq rdseed adx smap clflushopt clwb intel_pt avx512cd avx512bw avx512vl xsaveopt xsavec xgetbv1 xsaves cqm_llc cqm_occup_llc cqm_mbm_total cqm_mbm_local dtherm ida arat pln pts hwp hwp_act_window hwp_epp hwp_pkg_req pku ospke md_clear flush_l1d
bugs            : cpu_meltdown spectre_v1 spectre_v2 spec_store_bypass l1tf mds swapgs taa itlb_multihit
bogomips        : 4200.00
clflush size    : 64
cache_alignment : 64
address sizes   : 46 bits physical, 48 bits virtual
power management:
```

* 此台机器为 32 核，其中 physical id 表示真正的物理核 id，查看 id 数，可见真正的物理 CPU 数为 2

```sh
cat /proc/cpuinfo | grep 'physical id' | sort | uniq

physical id	: 0
physical id	: 1
```

* cpu cores 表示每个 CPU 有多少个物理核，结果均为 8 个。因此该机器 2 个物理 CPU，每个 CPU 8 核，物理核数为 16

```sh
cat /proc/cpuinfo | grep 'cpu cores' | uniq

cpu cores	: 8
```

* processor 表示逻辑核，可见共有 32 个逻辑核，processor 0 到 7、16 到 23 由 physical id 为 0 的 CPU 虚拟，processor 8 到 15、24 到 32 由 physical id 为 1 的 CPU 虚拟

```sh
cat /proc/cpuinfo | grep -E 'core id|processor|physical id'

processor	: 0
physical id	: 0
core id		: 0
processor	: 1
physical id	: 0
core id		: 1
processor	: 2
physical id	: 0
core id		: 2
processor	: 3
physical id	: 0
core id		: 3
processor	: 4
physical id	: 0
core id		: 4
processor	: 5
physical id	: 0
core id		: 5
processor	: 6
physical id	: 0
core id		: 6
processor	: 7
physical id	: 0
core id		: 7
processor	: 8
physical id	: 1
core id		: 0
processor	: 9
physical id	: 1
core id		: 1
processor	: 10
physical id	: 1
core id		: 2
processor	: 11
physical id	: 1
core id		: 3
processor	: 12
physical id	: 1
core id		: 4
processor	: 13
physical id	: 1
core id		: 5
processor	: 14
physical id	: 1
core id		: 6
processor	: 15
physical id	: 1
core id		: 7
processor	: 16
physical id	: 0
core id		: 0
processor	: 17
physical id	: 0
core id		: 1
processor	: 18
physical id	: 0
core id		: 2
processor	: 19
physical id	: 0
core id		: 3
processor	: 20
physical id	: 0
core id		: 4
processor	: 21
physical id	: 0
core id		: 5
processor	: 22
physical id	: 0
core id		: 6
processor	: 23
physical id	: 0
core id		: 7
processor	: 24
physical id	: 1
core id		: 0
processor	: 25
physical id	: 1
core id		: 1
processor	: 26
physical id	: 1
core id		: 2
processor	: 27
physical id	: 1
core id		: 3
processor	: 28
physical id	: 1
core id		: 4
processor	: 29
physical id	: 1
core id		: 5
processor	: 30
physical id	: 1
core id		: 6
processor	: 31
physical id	: 1
core id		: 7
```

* 现代 Intel CPU 架构中一般包含 L1、L2、L3 三级缓存，一个物理 CPU 的所有核共享一个 L3，可以直接用 lscpu 查看缓存大小，其中 `Socket(s)` 表示物理 CPU 数量为 2，`Core(s) per socket` 表示每个物理 CPU 8 核，因此共 16 物理核

```sh
lscpu

Architecture:        x86_64
CPU op-mode(s):      32-bit, 64-bit
Byte Order:          Little Endian
CPU(s):              32
On-line CPU(s) list: 0-31
Thread(s) per core:  2
Core(s) per socket:  8
Socket(s):           2
NUMA node(s):        2
Vendor ID:           GenuineIntel
CPU family:          6
Model:               85
Model name:          Intel(R) Xeon(R) Silver 4110 CPU @ 2.10GHz
Stepping:            4
CPU MHz:             2400.089
CPU max MHz:         3000.0000
CPU min MHz:         800.0000
BogoMIPS:            4200.00
Virtualization:      VT-x
L1d cache:           32K
L1i cache:           32K
L2 cache:            1024K
L3 cache:            11264K
NUMA node0 CPU(s):   0-7,16-23
NUMA node1 CPU(s):   8-15,24-31
Flags:               fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf pni pclmulqdq dtes64 monitor ds_cpl vmx smx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid dca sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb cat_l3 cdp_l3 invpcid_single pti intel_ppin ssbd mba ibrs ibpb stibp tpr_shadow vnmi flexpriority ept vpid fsgsbase tsc_adjust bmi1 hle avx2 smep bmi2 erms invpcid rtm cqm mpx rdt_a avx512f avx512dq rdseed adx smap clflushopt clwb intel_pt avx512cd avx512bw avx512vl xsaveopt xsavec xgetbv1 xsaves cqm_llc cqm_occup_llc cqm_mbm_total cqm_mbm_local dtherm ida arat pln pts hwp hwp_act_window hwp_epp hwp_pkg_req pku ospke md_clear flush_l1d
```

* CPU 信息位于 `/sys/devices/system/cpu`，这里的 CPU 均为逻辑核

```sh
ls /sys/devices/system/cpu
cpu0   cpu12  cpu16  cpu2   cpu23  cpu27  cpu30  cpu6  cpufreq       isolated    offline   present
cpu1   cpu13  cpu17  cpu20  cpu24  cpu28  cpu31  cpu7  cpuidle       kernel_max  online    smt
cpu10  cpu14  cpu18  cpu21  cpu25  cpu29  cpu4   cpu8  hotplug       microcode   possible  uevent
cpu11  cpu15  cpu19  cpu22  cpu26  cpu3   cpu5   cpu9  intel_pstate  modalias    power     vulnerabilities
```

* 查看 L1，index0、index1 均为 L1，大小均为 32 K，index0 为 Data 缓存，index1 为 Instruction 缓存，L1 命中时间一般为 1 到 4 个时钟周期

```sh
cd /sys/devices/system/cpu

ls cpu0/cache
index0  index1  index2  index3  power  uevent

ls cpu0/cache/index0
coherency_line_size  level           physical_line_partition  shared_cpu_list  size  uevent
id                   number_of_sets  power                    shared_cpu_map   type  ways_of_associativity

cat cpu0/cache/index0/coherency_line_size
64
cat cpu0/cache/index0/level
1
cat cpu0/cache/index0/size
32K
cat cpu0/cache/index0/shared_cpu_list
0,16
cat cpu0/cache/index0/type
Data

cat cpu0/cache/index1/coherency_line_size
64
cat cpu0/cache/index1/level
1
cat cpu0/cache/index1/size
32K
cat cpu0/cache/index1/shared_cpu_list
0,16
cat cpu0/cache/index1/type
Instruction
```

* 查看 L2，index2 为 L2 缓存，大小为 1024 K，L2 命中时间一般为 10 个时钟周期

```sh
cat cpu0/cache/index2/coherency_line_size
64
cat cpu0/cache/index2/level
2
cat cpu0/cache/index2/size
1024K
cat cpu0/cache/index2/shared_cpu_list
0,16
cat cpu0/cache/index2/type
Unified
```

* 查看 L3，index3 为 L3 缓存，大小为 11 M，L3 命中时间一般为 35 个时钟周期

```sh
cat cpu0/cache/index3/coherency_line_size
64
cat cpu0/cache/index3/level
3
cat cpu0/cache/index3/size
11264K
cat cpu0/cache/index3/shared_cpu_list
0-7,16-23
cat cpu0/cache/index3/type
Unified
```

* 上下文切换的开销一般在 2.7 到 5.48 微秒。查看上下文切换开销，每秒采集一次数据

```sh
vmstat 1
procs -----------memory---------- ---swap-- -----io---- -system-- ------cpu-----
 r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st
12  0      0 3595496 3418932 10385096    0    0     4    18    0    0 15  5 80  0  0
 7  0      0 3614872 3418932 10385772    0    0     0   432 122781 187683 23  7 70  0  0
17  0      0 3586344 3418932 10385924    0    0     0   236 119306 183010 23  7 70  0  0
24  0      0 3585084 3418932 10386620    0    0     4     0 117580 181276 22  6 71  0  0
13  0      0 3589388 3418932 10387400    0    0     0     0 122236 189284 22  7 72  0  0
 4  1      0 3703952 3418932 10388012    0    0     0   120 119607 185004 21  7 72  0  0
14  1      0 3693032 3418932 10385452    0    0     0  6780 120961 187345 23  8 67  2  0
 8  0      0 3698428 3418940 10385648    0    0     8   372 119126 181472 20  6 72  1  0
13  0      0 3694596 3418940 10384140    0    0     0     0 116128 181075 22  6 72  0  0
14  0      0 3692064 3418940 10384852    0    0     0     0 115432 180496 20  6 74  0  0
 6  0      0 3683828 3418944 10385424    0    0     0   156 118937 183939 22  7 71  0  0
 7  0      0 3689736 3418944 10385876    0    0     0     0 116807 181934 20  6 74  0  0
10  0      0 3684772 3418944 10386528    0    0     0   408 125014 190656 23  8 69  0  0
 7  0      0 3688664 3418944 10387160    0    0     0     0 113746 177164 20  6 75  0  0
```

* 也可以用 sar 查看，这台工作负载较高的机器每秒上下文切换次数约为 20 万次

```sh
sar -w 1
Linux 4.15.0-74-generic (changsha1g-ctel-175-6-54-89) 	07/28/21 	_x86_64_	(32 CPU)

08:16:49       proc/s   cswch/s
08:16:50       510.00 199009.00
08:16:51       298.00 189711.00
08:16:52       344.00 193572.00
08:16:53       339.00 190027.00
08:16:54       363.00 191633.00
08:16:55       432.00 195819.00
08:16:56       425.00 209397.00
08:16:57       335.00 198127.00
08:16:58       340.00 195600.00
08:16:59       315.00 196776.00
08:17:00       734.00 196456.00
08:17:01       466.00 204334.00
08:17:02       377.00 199836.00
08:17:03       345.00 195774.00
```

* 查看进程对应的上下文切换次数

```sh
pidstat -w 1

Average:      UID       PID   cswch/s nvcswch/s  Command
Average:        0         1     49.21     38.59  systemd
Average:        0         8     10.62      1.39  ksoftirqd/0
Average:        0         9    150.89      1.69  rcu_sched
Average:        0        11      9.13      0.00  migration/0
Average:        0        12      0.20      0.00  watchdog/0
Average:        0        15      0.20      0.00  watchdog/1
Average:        0        16     29.17      0.00  migration/1
Average:        0        17     11.81      0.89  ksoftirqd/1
Average:        0        21      0.20      0.00  watchdog/2
Average:        0        22      4.76      0.00  migration/2
Average:        0        23      4.66      0.40  ksoftirqd/2
Average:        0        27      0.20      0.00  watchdog/3
Average:        0        28      4.76      0.00  migration/3
Average:        0        29     10.91      0.89  ksoftirqd/3
Average:        0        33      0.20      0.00  watchdog/4
Average:        0        34      2.48      0.00  migration/4
Average:        0        35     14.19      0.10  ksoftirqd/4
Average:        0        39      0.20      0.00  watchdog/5
Average:        0        40      2.18      0.00  migration/5
Average:        0        41      8.83      0.00  ksoftirqd/5
Average:        0        45      0.20      0.00  watchdog/6
Average:        0        46      2.28      0.00  migration/6
Average:        0        47      5.16      0.00  ksoftirqd/6
Average:        0        51      0.20      0.00  watchdog/7
Average:        0        52      3.17      0.00  migration/7
Average:        0        53     12.50      0.00  ksoftirqd/7
Average:        0        57      0.20      0.00  watchdog/8
Average:        0        58      5.26      0.00  migration/8
Average:        0        59      5.56      0.79  ksoftirqd/8
Average:        0        64      0.20      0.00  watchdog/9
Average:        0        65     19.05      0.00  migration/9
Average:        0        66      3.37      0.20  ksoftirqd/9
Average:        0        70      0.20      0.00  watchdog/10
Average:        0        71      4.17      0.00  migration/10
Average:        0        72      4.37      0.40  ksoftirqd/10
Average:        0        76      0.20      0.00  watchdog/11
Average:        0        77      4.46      0.00  migration/11
Average:        0        78      5.95      0.00  ksoftirqd/11
Average:        0        82      0.20      0.00  watchdog/12
Average:        0        83      3.37      0.00  migration/12
Average:        0        84      3.87      0.00  ksoftirqd/12
Average:        0        88      0.20      0.00  watchdog/13
Average:        0        89      4.56      0.00  migration/13
Average:        0        90      3.17      0.10  ksoftirqd/13
Average:        0        94      0.20      0.00  watchdog/14
Average:        0        95      4.86      0.00  migration/14
Average:        0        96      2.38      0.00  ksoftirqd/14
Average:        0       100      0.20      0.00  watchdog/15
Average:        0       101      5.26      0.00  migration/15
Average:        0       102      4.86      0.00  ksoftirqd/15
Average:        0       106      0.20      0.00  watchdog/16
Average:        0       107     15.18      0.00  migration/16
Average:        0       108      4.37      0.00  ksoftirqd/16
Average:        0       112      0.20      0.00  watchdog/17
Average:        0       113      3.97      0.00  migration/17
Average:        0       114      6.65      0.20  ksoftirqd/17
Average:        0       118      0.20      0.00  watchdog/18
Average:        0       119     25.10      0.00  migration/18
Average:        0       120      3.17      0.20  ksoftirqd/18
Average:        0       124      0.20      0.00  watchdog/19
Average:        0       125      4.17      0.00  migration/19
Average:        0       126      6.35      0.20  ksoftirqd/19
Average:        0       130      0.20      0.00  watchdog/20
Average:        0       131      2.08      0.00  migration/20
Average:        0       132     13.00      0.30  ksoftirqd/20
Average:        0       136      0.20      0.00  watchdog/21
Average:        0       137      2.58      0.00  migration/21
Average:        0       138      8.04      0.99  ksoftirqd/21
Average:        0       142      0.20      0.00  watchdog/22
Average:        0       143      2.68      0.00  migration/22
Average:        0       144      5.75      0.00  ksoftirqd/22
Average:        0       148      0.20      0.00  watchdog/23
Average:        0       149      2.68      0.00  migration/23
Average:        0       150      5.16      0.20  ksoftirqd/23
Average:        0       154      0.20      0.00  watchdog/24
Average:        0       155      5.36      0.00  migration/24
Average:        0       156      3.47      0.20  ksoftirqd/24
Average:        0       160      0.20      0.00  watchdog/25
Average:        0       161      3.57      0.00  migration/25
Average:        0       162      5.95      0.30  ksoftirqd/25
Average:        0       166      0.20      0.00  watchdog/26
Average:        0       167      4.76      0.00  migration/26
Average:        0       168      4.96      0.40  ksoftirqd/26
Average:        0       172      0.20      0.00  watchdog/27
Average:        0       173      3.67      0.00  migration/27
Average:        0       174      4.66      0.10  ksoftirqd/27
Average:        0       178      0.20      0.00  watchdog/28
Average:        0       179      2.48      0.00  migration/28
Average:        0       180      5.56      1.19  ksoftirqd/28
Average:        0       184      0.20      0.00  watchdog/29
Average:        0       185      3.67      0.00  migration/29
Average:        0       186      3.37      0.00  ksoftirqd/29
Average:        0       190      0.20      0.00  watchdog/30
Average:        0       191      3.57      0.00  migration/30
Average:        0       192      3.08      0.00  ksoftirqd/30
Average:        0       196      0.20      0.00  watchdog/31
Average:        0       197      6.05      0.00  migration/31
Average:        0       198      4.17      0.00  ksoftirqd/31
Average:     1001       579      0.20      0.00  sshd
Average:      999       628      0.10      0.00  bash
Average:      999       629      1.29      0.40  ntpdate
Average:      999       630      0.10      0.40  grep
Average:        0      1054      0.10      0.60  sh
Average:        0      1055      0.10      0.20  timeout
Average:        0      1056      0.10      0.20  grep
Average:        0      1057      0.10      0.20  grep
Average:        0      1058      0.10      0.00  awk
Average:        0      1060      5.65      2.68  docker
Average:        0      1066      0.10      0.99  sh
Average:        0      1067      5.95      4.56  docker
Average:        0      1069      0.10      0.00  grep
Average:        0      1070      0.10      0.20  grep
Average:        0      1071      0.10      0.20  grep
Average:        0      1072      0.10      0.10  grep
Average:        0      1073      0.10      0.20  wc
Average:        0      1097      0.10      0.60  sh
Average:        0      1104      0.10      0.10  timeout
Average:        0      1105      0.10      0.10  grep
Average:        0      1106      0.10      0.40  awk
Average:        0      1107      4.66      8.33  docker
Average:        0      1375    178.47      6.35  systemd-logind
Average:        0      1402      0.20      0.00  cron
Average:        0      1421   1545.04      0.00  irq/271-nvidia
Average:        0      1423      1.69      0.00  irq/273-nvidia
Average:      103      1478    100.40     15.67  dbus-daemon
```

* 查看某个进程具体的上下文切换情况

```sh
grep ctxt /proc/4739/status

voluntary_ctxt_switches:	57264
nonvoluntary_ctxt_switches:	1358
```

* C 函数调用开销约为 0.4 纳秒，系统调用的开销一般约为 200 纳秒，多的可能到十多微秒，对每个系统调用大概需要 1000 条 CPU 指令。用 strace 查看统计某个进程的系统调用耗时

```sh
sudo strace -cp 947
strace: Process 947 attached
^Cstrace: Process 947 detached
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 55.22    0.001597           6       259           epoll_wait
  7.68    0.000222           8        29           writev
  6.74    0.000195          13        15        15 connect
  4.84    0.000140           2        88           epoll_ctl
  4.15    0.000120           6        19           close
  3.98    0.000115          14         8           sendmsg
  2.97    0.000086           6        15           socket
  2.87    0.000083           3        29         1 readv
  2.59    0.000075           8        10           sendto
  2.42    0.000070           3        22         8 read
  1.87    0.000054           2        29           ioctl
  1.28    0.000037           9         4           openat
  1.07    0.000031           2        15           getsockopt
  0.97    0.000028           1        30           fcntl
  0.52    0.000015           1        13           getpeername
  0.41    0.000012           1        13           getsockname
  0.24    0.000007           1        10           getpid
  0.07    0.000002           1         2           fstat
  0.07    0.000002           1         2           times
  0.03    0.000001           1         2           getrusage
------ ----------- ----------- --------- --------- ----------------
100.00    0.002892                   614        24 total
```

* 对于高负载的情况，系统调用开销可能高达毫秒

```sh
strace -cp 30665
strace: Process 30665 attached
^Cstrace: Process 30665 detached
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 99.66    1.878155        2627       715           epoll_wait
  0.06    0.001135           6       192           epoll_ctl
  0.04    0.000692          14        48           close
  0.04    0.000663          10        64           writev
  0.03    0.000491          15        32        32 connect
  0.02    0.000451           7        62           readv
  0.02    0.000413          11        37           sendto
  0.02    0.000365           6        62           ioctl
  0.02    0.000332           5        62        17 read
  0.02    0.000311          10        32           socket
  0.02    0.000286          17        17           sendmsg
  0.01    0.000217           3        64           fcntl
  0.01    0.000215           5        42           getpid
  0.01    0.000201           6        32           getsockopt
  0.01    0.000154           6        28           getsockname
  0.01    0.000154          10        16           openat
  0.01    0.000112           4        28           getpeername
  0.01    0.000107           9        12           write
  0.00    0.000030           6         5           futex
  0.00    0.000015           3         5           fstat
  0.00    0.000011           2         5           times
  0.00    0.000008           2         5           getrusage
------ ----------- ----------- --------- --------- ----------------
100.00    1.884518                  1565        49 total
```
