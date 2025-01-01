* 想查询某个函数功能

```sh
whatis socket
socket (7)           - Linux socket interface
socket (2)           - create an endpoint for communication
```

* (7) 代表在 man 手册的第 7 部分有介绍，如果要查看的话详细信息的话

```sh
man 7 socket # 也可以用 info 手册

# man 分为下列部分
1 commands
2 system calls
3 library functions # 如 printf
4 devices and special Files # /dev 下的各种设备文件和特殊文件
5 file formats and conventions # 文件格式与约定
6 games for linux
7 miscellanea # 杂项
8 system management commands
9 other # 内核例程
```

* 快捷键

```sh
space # 下页
b # 上页

Enter # 下行
k # 上行

/<string> # 向下查询字符串
?<string> # 向上查询字符串

n # 下一个查到的字符串
N # 上一个
```

* 在线的 man 手册有 [The Linux man-pages project](https://www.kernel.org/doc/man-pages/)、[man.he.net](http://man.he.net/)、[man.cx](https://man.cx/)
* 在 [The Open Group](https://pubs.opengroup.org/onlinepubs/9699919799.2018edition/) 查看 POSIX 标准
