## 介绍

* Redis 可以为每个键设置生存时间（Time To Live，TTL），生存时间到期后键会自动删除，配合出色的性能，Redis 常被用作缓存
* 作为缓存系统，Redis 还可以限制数据占用的最大内存空间，数据达到空间限制后，可以按一定规则自动淘汰不需要的键
* Redis 的列表类型可以用来实现队列，并支持阻塞式读取，可以很容易地实现一个高性能的优先级队列
* Redis 支持发布订阅的消息模式，可以基于此构建聊天室系统

## 安装

* Mac

```sh
brew install redis
```

* Ubuntu

```sh
sudo apt install redis
```

* 安装成功后将有如下可用命令行

```sh
redis-server
redis-cli
redis-benchmark
redis-check-aof # AOF 文件修复工具
redis-check-rdb # RDB 文件检查工具
redis-sentinel # Sential 服务器（仅 2.8 版以后）
```

## 运行

* 后台运行服务器

```sh
redis-server&
```

* 服务器默认使用 6379 端口（6379 对应手机键盘 MERZ，是一位意大利歌女的名字），通过 `--port` 参数可以自定义端口号

```sh
redis-server --port 6380
```

* 运行客户端，检测服务器是否正常运行

```sh
redis-cli

127.0.0.1:6379> ping
PONG
```

* 关闭服务器

```sh
127.0.0.1:6379> shutdown
414:M 13 Aug 2021 11:46:10.731 # User requested shutdown...
414:M 13 Aug 2021 11:46:10.731 * Saving the final RDB snapshot before exiting.
414:M 13 Aug 2021 11:46:10.737 * DB saved on disk
414:M 13 Aug 2021 11:46:10.737 # Redis is now ready to exit, bye bye...
```

## 配置

* 除了使用 `--port` 指定端口号，Redis 还支持其他选项，如果选项很多，更方便的做法是通过配置文件启动

```sh
redis-server redis.conf
```

* 如果同时指定配置文件和启动参数，则启动参数会配置文件中的对应选项

```sh
redis-server redis.conf --loglevel warning
```

* Redis 源码提供了 `redis/redis.conf` 作为配置文件模板
* 可以在 Redis 运行时通过 `CONFIG SET` 动态修改配置，但并非所有配置都能用此命令修改

```sh
127.0.0.1:6379> CONFIG SET loglevel warning
OK
127.0.0.1:6379> CONFIG GET loglevel
1) "loglevel"
2) "warning"
```

* maxmemory 参数可以设置 Redis 最大可用内存字节数，超出此限制时，Redis 将根据 maxmemory-policy 指定的策略删除不需要的键直到 Redis 占用内存小于指定内存，maxmemory-policy 支持如下规则

```sh
volatile-lru    # 使用 LRU 算法删除一个设置了过期时间的键
allkeys-lru     # 使用 LRU 算法删除一个键
volatile-random # 随机删除一个设置了过期时间的键
allkeys-random  # 随机删除一个键
volatile-ttl    # 删除过期时间最近的一个键
noeviction      # 不删除键，只返回错误
```

## 开机启动

* Linux 可以通过初始化脚本启动 Redis，Redis 源码提供了一个 `/redis/utils/redis_init_script` 脚本，内容如下

```sh
#!/bin/sh
#
# Simple Redis init.d script conceived to work on Linux systems
# as it does use of the /proc filesystem.

### BEGIN INIT INFO
# Provides:     redis_6379
# Default-Start:        2 3 4 5
# Default-Stop:         0 1 6
# Short-Description:    Redis data structure server
# Description:          Redis data structure server. See https://redis.io
### END INIT INFO

REDISPORT=6379
EXEC=/usr/local/bin/redis-server
CLIEXEC=/usr/local/bin/redis-cli

PIDFILE=/var/run/redis_${REDISPORT}.pid
CONF="/etc/redis/${REDISPORT}.conf"

case "$1" in
    start)
        if [ -f $PIDFILE ]
        then
                echo "$PIDFILE exists, process is already running or crashed"
        else
                echo "Starting Redis server..."
                $EXEC $CONF
        fi
        ;;
    stop)
        if [ ! -f $PIDFILE ]
        then
                echo "$PIDFILE does not exist, process is not running"
        else
                PID=$(cat $PIDFILE)
                echo "Stopping ..."
                $CLIEXEC -p $REDISPORT shutdown
                while [ -x /proc/${PID} ]
                do
                    echo "Waiting for Redis to shutdown ..."
                    sleep 1
                done
                echo "Redis stopped"
        fi
        ;;
    *)
        echo "Please use start or stop as first argument"
        ;;
esac
```

* 将该脚本拷到 `/etc/init.d` 中，文件名为 `redis_端口号`，并将脚本中的 `REDISPORT=6379` 改为此端口号
* 建立文件夹 `/etc/redis` 存放 Redis 配置文件，建立 `/var/redis/<port>` 存放 Redis 持久化文件
* 修改配置文件，将配置文件模板复制到 `/etc/redis`，以端口号命名，如 `6379.conf`，并修改如下参数

```sh
daemonize yes # 使 Redis 以守护进程模式运行
pidfile /var/run/redis_端口号.pid # 设置 Redis 的 PID 文件位置
port 端口号
dir /var/redis/端口号 # 设置持久化文件存放位置
```

* 使用 `/etc/init.d/redis_端口号 start` 启动 Redis，而后执行 `sudo update-rc.d redis_端口号 defaults` 使 Redis 随系统自动启动

* 插入与查询

```
127.0.0.1:6379> SET bar 1
OK
127.0.0.1:6379> GET bar
"1"
```

* 获取所有 key

```
127.0.0.1:6379> KEYS *
1) "bar"
```

* 判断 key 是否存在

```
127.0.0.1:6379> EXISTS bar
(integer) 1
127.0.0.1:6379> EXISTS foo
(integer) 0
```

* 删除 key

```
127.0.0.1:6379> DEL bar
(integer) 1
127.0.0.1:6379> DEL bar
(integer) 0
```

* 检查 key 对应值类型

```
127.0.0.1:6379> SET foo 1
OK
127.0.0.1:6379> type foo
string
127.0.0.1:6379> LPUSH bar 1
(integer) 1
127.0.0.1:6379> TYPE bar
list
```

* Redis 的数据类型有 string、hash、list、set、zset

## string

* string 类型最大能存储 512 MB 数据
* SET 赋值，GET 取值，APPEND 拼接字符串，STRLEN 获取字符串长度

```
127.0.0.1:6379> SET name hello
OK
127.0.0.1:6379> APPEND name " world"
(integer) 11
127.0.0.1:6379> STRLEN name
(integer) 11
127.0.0.1:6379> GET name
"hello world"
```

* 当存储的字符串是整数形式时，可以用 INCR 递增，如果 key 不存在则默认值为 0，INCR 后为 1，INCR 是原子操作。此外还可以用 DECR 递减，用 INCRBY 增加指定值，用 DECRBY 减少指定值，用 INCRBYFLOAT 增加指定浮点值

```
127.0.0.1:6379> SET age 12
OK
127.0.0.1:6379> INCR age
(integer) 13
127.0.0.1:6379> DECR age
(integer) 12
127.0.0.1:6379> INCRBY age 3
(integer) 15
127.0.0.1:6379> DECRBY age 3
(integer) 12
127.0.0.1:6379> INCRBYFLOAT age 1.5
"12.5"
127.0.0.1:6379> INCR age
(error) ERR value is not an integer or out of range
127.0.0.1:6379> INCRBYFLOAT age -1.5
"11"
127.0.0.1:6379> INCR age
(integer) 12
```

* 用 MSET 设置多个 key，用 MGET 获取多个 key

```
127.0.0.1:6379> MSET a 1 b 2 c 3
OK
127.0.0.1:6379> MGET a b c
1) "1"
2) "2"
3) "3"
```

* 用 GETBIT 获取指定位置二进制位值，如果超出索引范围则为 0，用 SETBIT 设定指定位置二进制位值，如果超出范围则将范围之间的二进制位设为 0

```
# 字符串 abc 对应的二进制表示为
# 0110 0001 0110 0010 0110 0011
127.0.0.1:6379> SET name abc
OK
127.0.0.1:6379> GET name
"abc"
127.0.0.1:6379> GETBIT name 0
(integer) 0
127.0.0.1:6379> GETBIT name 1
(integer) 1
127.0.0.1:6379> GETBIT name 2
(integer) 1
127.0.0.1:6379> GETBIT name 3
(integer) 0
127.0.0.1:6379> GETBIT name 23
(integer) 1
127.0.0.1:6379> GETBIT name 233
(integer) 1
127.0.0.1:6379> SETBIT name 23 0
(integer) 1
127.0.0.1:6379> GET name
"abb"
```

* 用 BITCOUNT 获取值为 1 的二进制位个数，BITCOUNT 可以指定字符下标的闭区间作为要查询的范围

```
# 字符串 abc 对应的二进制表示为
# 0110 0001 0110 0010 0110 0011
127.0.0.1:6379> SET name abc
OK
127.0.0.1:6379> GET name
"abc"
127.0.0.1:6379> BITCOUNT name
(integer) 10
127.0.0.1:6379> BITCOUNT name 0 0
(integer) 3
127.0.0.1:6379> BITCOUNT name 0 1
(integer) 6
127.0.0.1:6379> BITCOUNT name 0 2
(integer) 10
```

* 用 BITOP 对多个字符串做位运算

```
# a = 0110 0001
# b = 0110 0010
# c = 0110 0011
127.0.0.1:6379> SET a a
OK
127.0.0.1:6379> SET b b
OK
127.0.0.1:6379> BITOP OR res a b
(integer) 1
127.0.0.1:6379> GET res
"c"
```

* 用 BITPOS 获取二进制位值第一个为 0 或 1 的位置，BITPOS 可以额外用两个参数指定字符下标的闭区间作为要查询的范围。若未查询到则返回 -1，特殊的是如果所有二进制位都是 1，查询 0 时返回值长度的下一个字节的偏移量，因为 Redis 认为值长度后的二进制位都是 0

```
# 字符串 abc 对应的二进制表示为
# 0110 0001 0110 0010 0110 0011
127.0.0.1:6379> SET name abc
OK
127.0.0.1:6379> BITPOS name 0
(integer) 0
127.0.0.1:6379> BITPOS name 1
(integer) 1
127.0.0.1:6379> BITPOS name 0 1 1
(integer) 8
127.0.0.1:6379> BITPOS name 1 1 1
(integer) 9
127.0.0.1:6379> BITPOS name 0 3 3
(integer) -1
127.0.0.1:6379> SET name "\xff"
OK
127.0.0.1:6379> BITPOS name 0
(integer) 8
127.0.0.1:6379> SET name "\x00"
OK
127.0.0.1:6379> BITPOS name 1
(integer) -1
```

## hash

* hash 是 key 到一批字段与其值的映射，用 HSET 设置值，用 HGET 获取值，用 HDEL 删除字段，用 HEXISTS 判断字段是否存在，一个 hash 类型键至多能包含 `2 ^ 32 - 1` 个字段

```
127.0.0.1:6379> HSET people name abc
(integer) 1
127.0.0.1:6379> HSET people age 12
(integer) 1
127.0.0.1:6379> HGET people name
"abc"
127.0.0.1:6379> HGET people age
"12"
127.0.0.1:6379> HEXISTS people name
(integer) 1
127.0.0.1:6379> HDEL people name
(integer) 1
127.0.0.1:6379> HEXISTS people name
(integer) 0
```

* 用 HMSET 设置多个字段，用 HMGET 获取多个字段，用 HKEYS 获取所有字段名，用 HVALS 获取所有字段值，用 HGETALL 获取所有字段和值，用 HLEN 获取字段数量

```
127.0.0.1:6379> HMSET people name abc age 12
OK
127.0.0.1:6379> HMGET people name age
1) "abc"
2) "12"
127.0.0.1:6379> HKEYS people
1) "age"
2) "name"
127.0.0.1:6379> HVALS people
1) "12"
2) "abc"
127.0.0.1:6379> HGETALL people
1) "name"
2) "abc"
3) "age"
4) "12"
127.0.0.1:6379> HLEN people
(integer) 2
```

* HSETNX 在字段不存在时赋值，在字段存在时不执行任何操作，HSETNX 是原子操作

```
127.0.0.1:6379> HMSET people name abc age 12
OK
127.0.0.1:6379> HSETNX people name aaa
(integer) 0
127.0.0.1:6379> HGET people name
"abc"
```

* 用 HINCRBY 为字段值增加指定整数

```
127.0.0.1:6379> HSET people age 12
(integer) 0
127.0.0.1:6379> HINCRBY people age 3
(integer) 15
127.0.0.1:6379> HGET people age
"15"
```

## list

* list 内部使用双向链表实现，两端添加元素时间复杂度为 `O(1)`，这意味着即使列表有几千万元素，获取头部或尾部的十条记录也非常快
* list 和 hash 最多能容纳的字段数一样，最多能容纳 `2 ^ 32 - 1` 个元素
* 用 LPUSH 往左侧添加元素，用 RPUSH 往右侧添加元素，如果有多个元素则依次对每个元素执行命令。用 LPOP 弹出左侧元素，用 RPOP 弹出右侧元素，用 LRANGE 获取指定范围内元素，LRANGE 支持负索引，-1 表示从右侧数第一个元素，-2 表示从右侧数第二个元素。用 LLEN 获取元素数，若 key 不存在则返回 0

```
127.0.0.1:6379> LPUSH nums 1 2 3
(integer) 3
127.0.0.1:6379> LLEN nums
(integer) 3
127.0.0.1:6379> LRANGE nums 0 -1
1) "3"
2) "2"
3) "1"
127.0.0.1:6379> LRANGE nums 0 1
1) "3"
2) "2"
127.0.0.1:6379> LRANGE nums 0 100
1) "3"
2) "2"
3) "1"
127.0.0.1:6379> LPOP nums
"3"
127.0.0.1:6379> RPOP nums
"1"
127.0.0.1:6379> RPOP nums
"2"
127.0.0.1:6379> LRANGE nums 0 -1
(empty list or set)
```

* LREM 删除链表中指定数量个目标值，命令为 `LREM key count value`，若 count 为 0 则删除所有指定值，若 count 大于 0 则从左侧开始删除 count 个目标值，若 count 小于 0 则从右侧开始删除 count 个目标值。LTRIM 删除指定范围外的所有元素

```
127.0.0.1:6379> RPUSH nums 1 0 2 0 3 0 4 0 5 0 6
(integer) 11
127.0.0.1:6379> LREM nums 2 0
(integer) 2
127.0.0.1:6379> LRANGE nums 0 -1
1) "1"
2) "2"
3) "3"
4) "0"
5) "4"
6) "0"
7) "5"
8) "0"
9) "6"
127.0.0.1:6379> LREM nums -1 0
(integer) 1
127.0.0.1:6379> LRANGE nums 0 -1
1) "1"
2) "2"
3) "3"
4) "0"
5) "4"
6) "0"
7) "5"
8) "6"
127.0.0.1:6379> LREM nums 0 0
(integer) 2
127.0.0.1:6379> LRANGE nums 0 -1
1) "1"
2) "2"
3) "3"
4) "4"
5) "5"
6) "6"
127.0.0.1:6379> LTRIM nums 0 0
OK
127.0.0.1:6379> LRANGE nums 0 -1
1) "1"
127.0.0.1:6379> LPOP nums
"1"
```

* LINDEX 获取指定索引元素，索引可以为负数，表示从右侧开始计算，LSET
修改指定索引的值

```
127.0.0.1:6379> RPUSH nums 1 2 3
(integer) 3
127.0.0.1:6379> LRANGE nums 0 -1
1) "1"
2) "2"
3) "3"
127.0.0.1:6379> LINDEX nums 0
"1"
127.0.0.1:6379> LINDEX nums -1
"3"
127.0.0.1:6379> LSET nums 1 5
OK
127.0.0.1:6379> LRANGE nums 0 -1
1) "1"
2) "5"
3) "3"
```

* `LINSERT key BEFORE|AFTER pivot value` 查找第一个值为 pivot 的元素，若元素不存在返回 -1，若元素存在则根据第二个参数为 BEFORE 或 AFTER 将 value 插入到元素的左侧或右侧

```
127.0.0.1:6379> LRANGE nums 0 -1
1) "1"
2) "0"
3) "2"
4) "0"
5) "3"
127.0.0.1:6379> LINSERT nums BEFORE 0 4
(integer) 6
127.0.0.1:6379> LRANGE nums 0 -1
1) "1"
2) "4"
3) "0"
4) "2"
5) "0"
6) "3"
127.0.0.1:6379> LINSERT nums AFTER 0 5
(integer) 7
127.0.0.1:6379> LRANGE nums 0 -1
1) "1"
2) "4"
3) "0"
4) "5"
5) "2"
6) "0"
7) "3"
```

* `RPOPLPUSH source destination` 将 source 的最右侧元素弹出，再插入到 destination 的左侧

```
127.0.0.1:6379> RPUSH nums1 1 2 3
(integer) 3
127.0.0.1:6379> RPOPLPUSH nums1 nums2
"3"
127.0.0.1:6379> LRANGE nums2 0 -1
1) "3"
```

## set

* set 键可以存储至多 `2 ^ 32 - 1` 个字符串，字符串无序但唯一
* SADD 添加元素，SREM 删除元素，两者返回操作成功的个数。SCARD 返回元素数量，SMEMBERS 返回所有元素，SISMEMBER 判断元素是否在集合中，SPOP 从集合中随机选一个元素弹出

```
127.0.0.1:6379> SADD words a b c
(integer) 3
127.0.0.1:6379> SCARD words
(integer) 3
127.0.0.1:6379> SMEMBERS words
1) "b"
2) "c"
3) "a"
127.0.0.1:6379> SISMEMBER words a
(integer) 1
127.0.0.1:6379> SISMEMBER words d
(integer) 0
127.0.0.1:6379> SREM words a b c
(integer) 3
127.0.0.1:6379> SADD words a b c
(integer) 3
127.0.0.1:6379> SPOP words
"a"
127.0.0.1:6379> SPOP words
"c"
127.0.0.1:6379> SPOP words
"b"
127.0.0.1:6379> SPOP words
(nil)
```

* SDIFF 计算差集，SINTER 计算交集，SUNION 计算并集，SDIFFSTORE、SINTERSTORE、SUNIONSTORE 将计算结果存储到目标键

```
127.0.0.1:6379> SADD setA 1 2 3
(integer) 3
127.0.0.1:6379> SADD setB 2 3 4
(integer) 3
127.0.0.1:6379> SADD setC 3 4 5
(integer) 3
127.0.0.1:6379> SDIFF setA setB
1) "1"
127.0.0.1:6379> SINTER setA setB setC
1) "3"
127.0.0.1:6379> SUNION setA setB setC
1) "1"
2) "2"
3) "3"
4) "4"
5) "5"
127.0.0.1:6379> SUNIONSTORE unionABC setA setB setC
(integer) 5
127.0.0.1:6379> SMEMBERS unionABC
1) "1"
2) "2"
3) "3"
4) "4"
5) "5"
```

* SRANDMEMBER 随机获取一个元素，`SRANDMEMBER key [count]` 随机获取多个元素，count 大于 0 则获取 count 个不重复元素，count 小于 0 则获取 -count 个元素但元素可能相同。可以发现返回的数据并非均匀分布，原因是 set 实现采用的哈希表由拉链法实现，随机获取元素时，先从所有桶中随机选择一个桶，再从桶的所有元素中随机选一个元素。元素所在桶的元素数量越少，随机选中的概率越大，元素为 abc 时，b 位于一个桶，ac 位于一个桶，因此 b 出现的概率大

```
127.0.0.1:6379> SMEMBERS words
1) "b"
2) "c"
3) "a"
127.0.0.1:6379> SRANDMEMBER words
"b"
127.0.0.1:6379> SRANDMEMBER words
"a"
127.0.0.1:6379> SRANDMEMBER words 0
(empty list or set)
127.0.0.1:6379> SRANDMEMBER words 2
1) "b"
2) "c"
127.0.0.1:6379> SRANDMEMBER words 2
1) "a"
2) "c"
127.0.0.1:6379> SRANDMEMBER words 4
1) "b"
2) "a"
3) "c"
127.0.0.1:6379> SRANDMEMBER words -2
1) "b"
2) "c"
127.0.0.1:6379> SRANDMEMBER words -2
1) "b"
2) "b"
127.0.0.1:6379> SRANDMEMBER words -10
 1) "b"
 2) "b"
 3) "c"
 4) "a"
 5) "b"
 6) "b"
 7) "b"
 8) "b"
 9) "c"
10) "b"
```

## zset

* zset 实现原理是哈希表和跳表，读取元素的时间复杂度为 `O(log n)`
* zset 为每个元素关联一个分数，分数可以是浮点数。ZADD 添加元素，ZREM 删除元素，ZSCORE 获取元素的分数，ZINCRBY 增加元素分数，ZCARD 获取元素数量。ZRANGE 按分数从小到大顺序返回范围内元素，若分数相同则按 key 的字典序返回，在命令尾部加上 WITHSCORES 参数将返回元素及其分数，ZRANGE 时间复杂度为 `O(log n + m)`，n 为 zset 的基数，m 为返回的元素个数。ZREVRANGE 返回顺序与 ZRANGE 相反。ZRANGEBYSOCRE 返回指定分数范围内的元素，如果希望范围不包含指定分数，在分数前加上 `(` 即可，此外可以用 LIMIT 筛选结果中指定的范围。ZREVRANGEBYSCORE 返回顺序、输入范围的参数顺序与 ZRANGEBYSOCRE 相反

```
127.0.0.1:6379> ZADD scores 30 A 50 B 70 C +inf D -inf E
(integer) 5
127.0.0.1:6379> ZCARD scores
(integer) 5
127.0.0.1:6379> ZINCRBY scores 20 A
"50"
127.0.0.1:6379> ZSCORE scores A
"50"
127.0.0.1:6379> ZSCORE scores D
"inf"
127.0.0.1:6379> ZSCORE scores E
"-inf"
127.0.0.1:6379> ZRANGE scores 0 -1
1) "E"
2) "A"
3) "B"
4) "C"
5) "D"
127.0.0.1:6379> ZRANGE scores 0 -1 WITHSCORES
 1) "E"
 2) "-inf"
 3) "A"
 4) "50"
 5) "B"
 6) "50"
 7) "C"
 8) "70"
 9) "D"
10) "inf"
127.0.0.1:6379> ZREVRANGE scores 0 -1 WITHSCORES
 1) "D"
 2) "inf"
 3) "C"
 4) "70"
 5) "B"
 6) "50"
 7) "A"
 8) "50"
 9) "E"
10) "-inf"
127.0.0.1:6379> ZRANGEBYSCORE scores 50 70 WITHSCORES
1) "A"
2) "50"
3) "B"
4) "50"
5) "C"
6) "70"
127.0.0.1:6379> ZRANGEBYSCORE scores 50 (70 WITHSCORES
1) "A"
2) "50"
3) "B"
4) "50"
127.0.0.1:6379> ZRANGEBYSCORE scores (50 70 WITHSCORES
1) "C"
2) "70"
127.0.0.1:6379> ZRANGEBYSCORE scores (50 (70 WITHSCORES
(empty list or set)
127.0.0.1:6379> ZRANGEBYSCORE scores -inf inf WITHSCORES
 1) "E"
 2) "-inf"
 3) "A"
 4) "50"
 5) "B"
 6) "50"
 7) "C"
 8) "70"
 9) "D"
10) "inf"
127.0.0.1:6379> ZRANGEBYSCORE scores -inf inf WITHSCORES LIMIT 3 -1
1) "C"
2) "70"
3) "D"
4) "inf"
127.0.0.1:6379> ZREVRANGEBYSCORE scores 70 50 WITHSCORES
1) "C"
2) "70"
3) "B"
4) "50"
5) "A"
6) "50"
127.0.0.1:6379> ZREM scores A B C D E
(integer) 5
127.0.0.1:6379> ZRANGE scores 0 -1
(empty list or set)
```

* ZCOUNT 获取指定分数范围内元素数，ZRANK 获取指定元素按分数从小到大的排名，ZREVRANK 获取指定元素按分数从大到小的排名，ZREMRANGEBYSCORE 按分数范围删除元素，ZREMRANGEBYRANK 按排名删除范围内元素

```
127.0.0.1:6379> ZADD scores 10 A 20 B 30 C 40 D 50 E
(integer) 5
127.0.0.1:6379> ZRANGE scores 0 -1 WITHSCORES
 1) "A"
 2) "10"
 3) "B"
 4) "20"
 5) "C"
 6) "30"
 7) "D"
 8) "40"
 9) "E"
10) "50"
127.0.0.1:6379> ZCOUNT scores 30 40
(integer) 2
127.0.0.1:6379> ZRANK scores A
(integer) 0
127.0.0.1:6379> ZREVRANK scores A
(integer) 4
127.0.0.1:6379> ZREMRANGEBYSCORE scores 20 30
(integer) 2
127.0.0.1:6379> ZRANGE scores 0 -1 WITHSCORES
1) "A"
2) "10"
3) "D"
4) "40"
5) "E"
6) "50"
127.0.0.1:6379> ZREMRANGEBYRANK scores 1 1
(integer) 1
127.0.0.1:6379> ZRANGE scores 0 -1
1) "A"
2) "E"
127.0.0.1:6379> ZREMRANGEBYRANK scores 0 -1
(integer) 5
127.0.0.1:6379> ZRANGE scoers 0 -1
(empty list or set)
```

* `ZINTERSTORE destination numkeys key [key ...] [WEIGHTS weight] [AGGREGATE SUM|MIN|MAX]` 计算 zset 的交集，将结果存储到 destination，返回结果的元素数量，destination 中元素分数由 AGGREGATE 参数决定，AGGREGATE 参数默认为 SUM，表示分数为所有参与计算的该元素的分数和，MIN 和 MAX 分别表示所有参与计算的该元素的分数的最小值和最大值，WEIGHTS 参数用于设置每个集合的权重。ZUNIONSTORE 计算 zset 的并集，用法相同

```
127.0.0.1:6379> ZADD Z1 10 A 20 B 30 C
(integer) 3
127.0.0.1:6379> ZADD Z2 40 B 50 C 60 D
(integer) 3
127.0.0.1:6379> ZRANGE Z1 0 -1 WITHSCORES
1) "A"
2) "10"
3) "B"
4) "20"
5) "C"
6) "30"
127.0.0.1:6379> ZRANGE Z2 0 -1 WITHSCORES
1) "B"
2) "40"
3) "C"
4) "50"
5) "D"
6) "60"
127.0.0.1:6379> ZINTERSTORE Z3 2 Z1 Z2
(integer) 2
127.0.0.1:6379> ZRANGE Z3 0 -1 WITHSCORES
1) "B"
2) "60"
3) "C"
4) "80"
127.0.0.1:6379> ZINTERSTORE Z3 2 Z1 Z2 WEIGHTS 1 2
(integer) 2
127.0.0.1:6379> ZRANGE Z3 0 -1 WITHSCORES
1) "B"
2) "100"
3) "C"
4) "130"
127.0.0.1:6379> ZINTERSTORE Z3 2 Z1 Z2 AGGREGATE MIN
(integer) 2
127.0.0.1:6379> ZRANGE Z3 0 -1 WITHSCORES
1) "B"
2) "20"
3) "C"
4) "30"
127.0.0.1:6379> ZINTERSTORE Z3 2 Z1 Z2 WEIGHTS 2 1 AGGREGATE MIN
(integer) 2
127.0.0.1:6379> ZRANGE Z3 0 -1 WITHSCORES
1) "B"
2) "40"
3) "C"
4) "50"
127.0.0.1:6379> ZUNIONSTORE Z4 2 Z1 Z2 WEIGHTS 2 1
(integer) 4
127.0.0.1:6379> ZRANGE Z4 0 -1 WITHSCORES
1) "A"
2) "20"
3) "D"
4) "60"
5) "B"
6) "80"
7) "C"
8) "110"
```

## 事务

* 如果有多个命令，希望所有命令要么都执行要么都不执行，则需要使用事务，事务以 MULTI 开始，EXEC 结束

```
127.0.0.1:6379> MULTI
OK
127.0.0.1:6379> LPUSH nums 1
QUEUED
127.0.0.1:6379> LPUSH nums 2
QUEUED
127.0.0.1:6379> LRANGE nums 0 -1
QUEUED
127.0.0.1:6379> EXEC
1) (integer) 1
2) (integer) 2
3) 1) "2"
   2) "1"
```

* 如果事务中某条命令有语法错误，则事务中所有命令都不会执行

```
127.0.0.1:6379> LRANGE nums 0 -1
1) "2"
2) "1"
127.0.0.1:6379> MULTI
OK
127.0.0.1:6379> LPUSH nums 3
QUEUED
127.0.0.1:6379> A
(error) ERR unknown command `A`, with args beginning with: 
127.0.0.1:6379> LPUSH nums 4
QUEUED
127.0.0.1:6379> EXEC
(error) EXECABORT Transaction discarded because of previous errors.
127.0.0.1:6379> LRANGE nums 0 -1
1) "2"
2) "1"
```

* 如果事务中的命令在执行时才会出错，比如用 hash 的命令操作 string，事务中所有命令均会执行

```
127.0.0.1:6379> MULTI
OK
127.0.0.1:6379> SET age 12
QUEUED
127.0.0.1:6379> SADD age 1
QUEUED
127.0.0.1:6379> INCR age
QUEUED
127.0.0.1:6379> EXEC
1) OK
2) (error) WRONGTYPE Operation against a key holding the wrong kind of value
3) (integer) 13
127.0.0.1:6379> GET age
"13"
```

* 用 WATCH 监控 key，一旦 WATCH 的 key 被修改或删除，则之后的事务不会执行，直到执行 EXEC 命令后，所有的 WATCH 将被取消

```
127.0.0.1:6379> SET age 12
OK
127.0.0.1:6379> WATCH age
OK
127.0.0.1:6379> INCR age
(integer) 13
127.0.0.1:6379> MULTI
OK
127.0.0.1:6379> SET age 15
QUEUED
127.0.0.1:6379> EXEC
(nil)
127.0.0.1:6379> GET age
"13"
127.0.0.1:6379> MULTI
OK
127.0.0.1:6379> SET age 15
QUEUED
127.0.0.1:6379> EXEC
1) OK
127.0.0.1:6379> GET age
"15"
```

## 过期时间

* EXPIRE 指定某个 key 的过期时间，时间单位为秒，到期后将删除 key，设置成功返回 1，若 key 不存在或设置失败返回 0，如果 WATCH 设置了过期时间的 key，key 过期不会被 WATCH 认为发生改变

```
127.0.0.1:6379> LPUSH nums 1 2 3
(integer) 3
127.0.0.1:6379> EXPIRE nums 10
(integer) 1
# 10 秒后将删除 nums
127.0.0.1:6379> LRANGE nums 0 -1
(empty list or set)
127.0.0.1:6379> KEYS *
(empty list or set)
```

* TTL 返回 key 的剩余过期时间，若未设置过期时间则返回 -1，若 key 不存在则返回 -2

```
127.0.0.1:6379> TTL age
(integer) -2
127.0.0.1:6379> SET age 12
OK
127.0.0.1:6379> TTL age
(integer) -1
127.0.0.1:6379> EXPIRE age 10
(integer) 1
127.0.0.1:6379> TTL age
(integer) 7
# 过期后
127.0.0.1:6379> TTL age
(integer) -2
```

* 再次使用 EXPIRE 会重置过期时间

```
127.0.0.1:6379> EXPIRE age 10
(integer) 1
127.0.0.1:6379> TTL age
(integer) 8
127.0.0.1:6379> EXPIRE age 20
(integer) 1
127.0.0.1:6379> TTL age
(integer) 18
```

* PERSIST 取消 key 的过期时间设置，使用 SET 或 GETSET 也会取消 key 的过期时间设置，但其他只对值进行的操作（如 INCR、LPUSH、HSET、ZREM）不会影响过期时间

```
127.0.0.1:6379> SET age 12
OK
127.0.0.1:6379> EXPIRE age 10
(integer) 1
127.0.0.1:6379> TTL age
(integer) 8
127.0.0.1:6379> PERSIST age
(integer) 1
127.0.0.1:6379> TTL age
(integer) -1
```

* EXPIRE 的参数必须是整数，最小单位是一秒。PEXPIRE 的时间单位是毫秒，PTTL 查看单位为毫秒的剩余过期时间

```
127.0.0.1:6379> set age 12
OK
127.0.0.1:6379> PEXPIRE age 10000
(integer) 1
127.0.0.1:6379> PTTL age
(integer) 7063
127.0.0.1:6379> TTL age
(integer) 3
```

## 排序

* SORT 返回对 list 排序的结果

```
127.0.0.1:6379> RPUSH nums 1 3 2
(integer) 3
127.0.0.1:6379> LRANGE nums 0 -1
1) "1"
2) "3"
3) "2"
127.0.0.1:6379> SORT nums
1) "1"
2) "2"
3) "3"
127.0.0.1:6379> SORT nums DESC
1) "3"
2) "2"
3) "1"
```

* SORT 排序 set，对字符排序要指定 ALPHA 参数

```
127.0.0.1:6379> SADD words a c b
(integer) 3
127.0.0.1:6379> SMEMBERS words
1) "b"
2) "c"
3) "a"
127.0.0.1:6379> SORT words
(error) ERR One or more scores can't be converted into double
127.0.0.1:6379> SORT words ALPHA
1) "a"
2) "b"
3) "c"
127.0.0.1:6379> SORT words ALPHA DESC
1) "c"
2) "b"
3) "a"
```

* SORT 排序 zset 会忽略元素的分数，只对元素的值排序

```
127.0.0.1:6379> ZADD scores 10 a 20 c 30 b 40 d
(integer) 4
127.0.0.1:6379> ZRANGE scores 0 -1
1) "a"
2) "c"
3) "b"
4) "d"
127.0.0.1:6379> SORT scores ALPHA
1) "a"
2) "b"
3) "c"
4) "d"
```

* SORT 可以用 BY 指定排序参考的键，用每个元素替换参考键中的第一个 `*` 并获取值，再以该值进行排序，如果值相同则再比较元素本身的值来决定顺序，如果未设置则认为值为 0。如果参考键中没有 `*` 则所有要比较的值都是一样的，Redis 认为这种情况没有意义，不执行排序操作。SORT 的 GET 参数可以使用排序返回的结果，可以有多个 GET 参数，用 `GET #` 可以返回元素本身值。用 STORE 参数保存排序结果为 list，返回值为元素数，STORE 的结果常用于结合 EXPIRE 命令作为排序的缓存

```
127.0.0.1:6379> SADD words a b c d e
(integer) 5
127.0.0.1:6379> SMEMBERS words
1) "b"
2) "d"
3) "c"
4) "a"
5) "e"
127.0.0.1:6379> SET item:a -1
OK
127.0.0.1:6379> SET item:b 1
OK
127.0.0.1:6379> SET item:c 3
OK
127.0.0.1:6379> SET item:d 2
OK
127.0.0.1:6379> SORT words BY item:*
1) "a"
2) "e"
3) "b"
4) "d"
5) "c"
127.0.0.1:6379> SET dest:a 1
OK
127.0.0.1:6379> SET dest:b 2
OK
127.0.0.1:6379> SET dest:c 3
OK
127.0.0.1:6379> SET dest:d 4
OK
127.0.0.1:6379> SET dest:e 5
OK
127.0.0.1:6379> SORT words BY item:* GET dest:*
1) "1"
2) "5"
3) "2"
4) "4"
5) "3"
127.0.0.1:6379> SORT words BY item:* GET dest:* GET #
 1) "1"
 2) "a"
 3) "5"
 4) "e"
 5) "2"
 6) "b"
 7) "4"
 8) "d"
 9) "3"
10) "c"
127.0.0.1:6379> SORT words BY item:* GET dest:* GET # STORE res
(integer) 10
127.0.0.1:6379> LRANGE res 0 -1
 1) "1"
 2) "a"
 3) "5"
 4) "e"
 5) "2"
 6) "b"
 7) "4"
 8) "d"
 9) "3"
10) "c"
```

* SORT 是 Redis 最强大复杂的命令之一，使用不好很容易成为性能瓶颈。SORT 的时间复杂度是 `O(n + m * log m)`，n 为排序目标的元素数，m 为返回的元素个数，n 较大时 SORT 性能较低，并且 Redis 排序前会建立一个长度为 n 的容器存储待排序元素，如果同时进行较多的大数据量排序操作则会严重影响性能，因此开发时使用 SORT 要注意以下几点
  * 尽量减少待排序目标的元素数量，使 n 尽可能小
  * 使用 LIMIT 参数只获取需要的数据，使 m 尽可能小
  * 如果要排序的数据数量较大，尽可能用 STORE 参数将结果缓存

## 消息通知

* BRPOP 从 list 右侧弹出一个元素，在 list 没有元素时，会阻塞至有新元素加入或超时，若超时时间为 0 则将一直阻塞直到有新元素。BLPOP 从左侧弹出元素，用法和 BRPOP 一致

```
127.0.0.1:6379> LPUSH nums 1
(integer) 1
127.0.0.1:6379> LRANGE nums 0 -1
1) "1"
127.0.0.1:6379> BRPOP nums 0
1) "nums"
2) "1"
127.0.0.1:6379> BRPOP nums 3
(nil)
(3.10s)
127.0.0.1:6379> BRPOP nums 0
# 在另一个 redis-cli 执行 LPUSH nums 1
1) "nums"
2) "1"
(3.09s)
```

* BRPOP 可以同时接收多个 list，如果所有 list 都无元素则阻塞，如果其中一个有元素则弹出元素，如果多个 list 都有元素则从传入的第一个有元素的 list 弹出。把要优先取元素的 list 写在前面，即可实现分优先级的任务队列的功能

```
127.0.0.1:6379> LRANGE nums1 0 -1
1) "1"
127.0.0.1:6379> LRANGE nums2 0 -1
1) "2"
127.0.0.1:6379> BRPOP nums1 nums2 0
1) "nums1"
2) "1"
127.0.0.1:6379> BRPOP nums1 nums2 0
1) "nums2"
2) "2"
127.0.0.1:6379> BRPOP nums1 nums2 0
# 在另一个 redis-cli 执行 LPUSH nums2 1
1) "nums2"
2) "1"
(13.94s)
```

* SUBSCRIBE 订阅一个频道，返回值有三个，第一个返回值是消息类型，可以为 subscribe、unsubscribe、message，第二个参数是频道名，若消息类型是 subscribe 或 unsubscribe 则第三个参数是当前客户端订阅的频道数，若消息类型是 message 则第三个参数是消息内容。PUBLISH 往频道发消息后所有订阅者都会收到消息，返回值为接受消息的订阅者数量。UNSUBSCRIBE 取消订阅指定频道，若不指定频道则取消订阅所有频道

```
# redis-cli 1
127.0.0.1:6379> SUBSCRIBE channel.1
Reading messages... (press Ctrl-C to quit)
1) "subscribe"
2) "channel.1"
3) (integer) 1
# redis-cli 2 执行 PUBLISH 后显示如下信息
1) "message"
2) "channel.1"
3) "hi"

# redis-cli 2
127.0.0.1:6379> PUBLISH channel.1 hi
(integer) 1
```

* PSUBSCRIBE 和 PUNSUBSCRIBE 支持通配符匹配频道名

```
127.0.0.1:6379> PSUBSCRIBE channel.?*
Reading messages... (press Ctrl-C to quit)
1) "psubscribe"
2) "channel.?*"
3) (integer) 1
# 在另外的 redis-cli 执行 PUBLISH channel.1 hi
1) "pmessage"
2) "channel.?*"
3) "channel.1"
4) "hi"
# 在另外的 redis-cli 执行 PUBLISH channel.233 hi
1) "pmessage"
2) "channel.?*"
3) "channel.233"
4) "hi"
```

* PSUBSCRIBE 可以重复订阅相同频道

```
127.0.0.1:6379> PSUBSCRIBE channel.? channel.?*
Reading messages... (press Ctrl-C to quit)
1) "psubscribe"
2) "channel.?"
3) (integer) 1
1) "psubscribe"
2) "channel.?*"
3) (integer) 2
# 在另外的 redis-cli 执行 PUBLISH channel.1 hi，PUBLISH 返回值为 2
1) "pmessage"
2) "channel.?"
3) "channel.1"
4) "hi"
1) "pmessage"
2) "channel.?*"
3) "channel.1"
4) "hi"
```

## 管道

* Redis 的客户端和服务器通过 TCP 连接，客户端发送命令到收到服务器返回结果的总耗时称为往返时延，一般到本地回环地址的往返时延在数量级上相当于 Redis 处理一条简单命令（如 LPUSH nums 1 2 3）的时间，如果执行较多命令，每个命令的往返时延累计起来对性能是有影响的
* 执行多个命令时，每条命令都要等待上一条命令执行完才能执行，即时命令不需要上一条的执行结果。Redis 的底层通信协议支持管道，通过管道可以一次性发送多条命令并在执行完成后一次性将结果返回，当一组命令中每条命令都不依赖之前命令的执行结果，就可以将这一组命令一起通过管道发出
