```sh
g++ -g xxx.cpp -o xxx # 没有 -g 则看不到函数名和变量名，只能看到地址
gdb # 打开 gdb
file xxx # 调试 xxx
gdb xxx # 相当结合上面两步，启动 gdb 调试 xxx
ctrl+x+a # TUI 模式
start # 开始
run # 运行，会停在断点
continue # 继续运行，直到遇到下一个断点
step # 步进，可以进函数，按回车会重复上次命令
bt # 查看函数堆栈
finish # 退出函数
next # 不进入函数而是执行下一句
break # 当前行打断点
break 20 # 在 line20 打断点
info break # 查看断点信息
clear # 删除当前行断点
clear 20 # 删除 line20 断点
delete # 删除所有断点
delete 3 # 删除编号为 3 的断点
print i # 打印变量 i 的值
set var i = 4 # 修改变量的值
quit # 退出 gdb

x/nfu # 查看内存，x 是 examine 的缩写
# n 表示显示内存单元数
# f 表示显示方式，可取如下值
x 十六进制
d 十进制
u 十进制无符号整型
o 八进制
t 二进制
a 十六进制
i 指令地址格式
c 字符
f 浮点数
# u 表示一个地址单元的长度，可取如下值
b 单字节
h 双字节
w 四字节
g 八字节
# 使用方式如 x/12xb

set p obj on # 按虚函数调用方法输出
# show print object 查看开关
set p pretty on # 按层次打印结构体
p *this
p /a (*(void ***)this)[0]@3 # 打印虚表
```
