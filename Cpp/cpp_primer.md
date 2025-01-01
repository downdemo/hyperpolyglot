## 程序的机器级表示

* 程序计数器（PC，x86-64 中用 % rip 表示）：给出下一要执行的指令的内存地址
* 整数寄存器文件：包含 16 个命名位置，分别存储 64 位值
* 条件寄存器：保存最近执行的算数逻辑指令的状态信息，比如 if 和 while
* 向量寄存器：存放一个或多个整数或浮点数值
* 由于是从 16 位体系结构扩展成 32 位的，Intel 用字（word）表示 16 位数据类型，32 位则是双字（double words），64 位则是四字（quad words）

```asm
;整数寄存器，从左到右依次为四字寄存器、双字寄存器、字寄存器、字节寄存器
63    31    15    7    0
%rax  %eax  %ax   %al       ;返回值
%rbx  %ebx  %bx   %bl       ;被调用者保存
%rcx  %ecx  %cx   %cl       ;第4个参数
%rdx  %edx  %dx   %dl       ;第3个参数
%rsi  %esi  %si   %sil      ;第2个参数
%rdi  %edi  %di   %dil      ;第1个参数
%rbp  %ebp  %bp   %bpl      ;返回值
%rsp  %esp  %sp   %spl      ;栈指针
%r8   %r8d  %r8w  %r8b      ;第5个参数
%r9   %r9d  %r9w  %r9b      ;第6个参数
%r10  %r10d %r10w %r10b     ;调用者保存
%r11  %r11d %r11w %r11b     ;调用者保存
%r12  %r12d %r12w %r12b     ;被调用者保存
%r13  %r13d %r13w %r13b     ;被调用者保存
%r14  %r14d %r14w %r14b     ;被调用者保存
%r15  %r15d %r15w %r15b     ;被调用者保存
```

* 指令操作数分为三种类型：立即数（常数值，$ 后接一个整数）、寄存器（某个寄存器内容，16 个寄存器的低位 1、2、4 或 8 字节中的一个作为操作数，r<sub>a</sub> 表示寄存器 a，R [r<sub>a</sub>] 表示其值）、内存引用（M<sub>b</sub>[Addr] 表示从 Addr 开始的 b 个字节值的引用）

```asm
;源类型和目的类型的五种可能组合
;movb 传字节、movw 传字、movl 传双字、movq 传四字、movabsq 传绝对的四字
movl $0x4050, %eax    ;立即数到寄存器，4字节
movw %bp, %sp         ;寄存器到寄存器，2字节
movb (%rdi,%rcx), %al ;内存到寄存器，1字节
movb $-17, (%rsp)     ;立即数到内存，1字节
movq %rax, -12(%rbq)  ;寄存器到内存，8字节
```

* 修改寄存器的值的例子

```asm
moveabsq $0x0011223344556677, %rax ;%rax = 0011223344556677
movb     $-1, %al                  ;%rax = 00112233445566FF（2个数1字节）
movw     $-1, %ax                  ;%rax = 001122334455FFFF
movl     $-1, %eax                 ;%rax = 00000000FFFFFFFF
movq     $-1, %rax                 ;%rax = FFFFFFFFFFFFFFFF
```

* 代码示例

```cpp
long swap(long* p, long x) {
  long tmp = *p;
  *p = x;
  return tmp;
}
```

* 对应汇编

```asm
swap:
  movq (%rdi), %rax ;获取第一个参数 p 存储的值 *p，作为返回值
  movq %rsi, (%rdi) ;把第二个参数 x 传给第一个参数 p 存储的值 *p
  ret
```

## enum

* 枚举属于字面值常量类型，C++11 引入了限定作用域的枚举类型，关键字为 enum class 或 enum struct，因此也可以简称为枚举类

```cpp
enum class open_modes { input, output, append };
```

* 定义不限定作用域的枚举类型省略关键字 class，不限作用域的枚举成员名称不能重复

```cpp
enum color { red, yellow, green };
enum color2 { green };  // 错误：重定义 green
auto green = bool;      // 错误，但有些编译器可能允许
// 未命名的，不限定作用域的枚举类型
enum { floatPrec = 6, doublePrec = 10, double_doublePrec = 10 };
```

* enum class 中，成员在枚举类型的作用域外不可访问。在 enum 中，成员作用域与枚举类型本身的作用域相同

```cpp
enum color { red, yellow, green };
enum class pappers { red, yellow, green };  // 正确：枚举成员被隐藏
color eyes = green;                         // 正确：有效作用域
pappers p = green;        // 错误：这是 color::green 而非 pappers::green
color hair = color::red;  // 正确
pappers p2 = pappers::red;  // 正确
```

* 默认情况枚举从 0 开始依次加 1，也可以指定值，枚举成员是 const，初始化枚举成员的初始值必须是常量表达式。如果没有提供初始值，当前枚举成员值等于前一个成员值加 1

```cpp
enum class intTypes {
  charTyp = 8,
  shortTyp = 16,
  intTyp = 16,
  longTyp = 32,
  long_longTyp = 64
};
// 可以定义枚举类型的 constexpr 变量
constexpr intTypes charbits = intTypes::charTyp;
```

* enum 对象或枚举成员会自动隐式转换为整型，而 enum class 不会

```cpp
int i = color::red;  // 正确
int j = pappers::red;  // 错误：限定作用域的枚举类型不会进行隐式转换
```

* C++11 中可以在 enum 后加冒号以及要在 enum 中使用的类型，如果不指定，默认限定作用域的 enum 为 int，不限定作用域的枚举类型则不存在默认类型

```cpp
enum intValues : unsigned long long {
  charTyp = 255,
  shortTyp = 65535,
  intTyp = 65535,
  longTyp = 4294967295UL,
  long_longTyp = 18446744073709551615ULL
}
```

* C++11 中可以提前声明 enum

```cpp
enum intValues : unsigned long long;  // enum 必须指定成员类型
enum class open_modes;                // enum class 默认 int
```

* 必须使用 enum 类型的另一个对象或成员来初始化对应的 enum 对象，即使 int 值与枚举成员值相等，也不能作为函数的 enum 实参使用

```cpp
enum color { red = 2, blue = 1 };
void f(color);

color e = red;
f(red);  // f(color)
f(e);    // f(color)
f(2);    // 错误
```

* 但可以将 enum 对象或成员传给整型形参，此时 enum 值提升成 int 或更大的整型，提升结果由枚举类型的潜在类型决定

```cpp
enum color { red, blue };
void f(unsigned char);
void f(int);

unsigned char uc = red;
f(red);  // f(int)
f(uc);   // f(unsigned char)
```

## union

* union 是一种特殊的类，一个 union 可以有多个数据成员，但任意时刻只有一个数据成员可以有值。给 union 的某个成员赋值后，其他成员就变成未定义状态，如果使用错误的数据成员或为错误的数据成员赋值程序可能崩溃
* 分配给 union 的空间至少要容纳最大的数据成员
* union 不能含有引用
* C++11 中含有构造函数或析构函数的类类型可以作为 union 成员
* union 可以为成员指定 public、protected 和 private 等保护标记，默认情况下所有成员为 public
* union 可以方便地表示一组类型不同的互斥值

```cpp
union U {
  char c;
  int i;
  double d;
};

U x = {'a'};  // 初始化 c
U y;
y.c = 'z';
U* p = new U;
p->i = 42;
```

* 匿名 union 是一个未命名的 union，一旦定义匿名 union 编译器就会自动创建一个未命名对象。匿名 union 不能包含 protected 或 private 成员，也不能定义成员函数

```cpp
union {
  char c;
  int i;
  double d;
};  // 定义一个未命名对象，可以直接访问成员
c = 'c';
i = 42;
```

* 机器的存储规则中，低位在前的方式称为小端法，高位在前的方式称为大端法

```
int x = 0x01234567;  // 假设 x 地址为 0x100

// 大端
0x100: 01
0x101: 23
0x102: 45
0x103: 67
// 小端
0x100: 67
0x101: 45
0x102: 23
0x103: 01
```

* 判断大小端的方法

```cpp
bool is_little_endian() {
  int x = 1;
  return *reinterpret_cast<char*>(&x) == 1;
}

bool is_little_endian2() {
  union U {
    int i;
    char c;
  } u;
  u.i = 1;
  return u.c == 1;
}
```

## 位域（bit-field）

* 有时存储只需要几个二进制位，而不需要一个完整的字节，用于此目的的数据结构就是位域，连续定义的位域会尽可能压缩在内存相邻位
* 位域类型必须是整型或枚举类型，通常用无符号整型保存，位域同枚举一样不能取址

```cpp
struct A {
  unsigned a : 1;
  unsigned b : 1;
};

struct B {
  unsigned a;
  unsigned b;
};

static_assert(sizeof(A) == 4);
static_assert(sizeof(B) == 8);
```

* 位域宽度不能超过类型本身的大小

```cpp
struct A {
  int a : 33;  // 错误
  char b : 9;  // 错误
};
```

* 匿名的位域宽度可以设置为 0，表示空域，用于填充上一位域的剩余位置，下一位域将从新的边界开始对齐

```cpp
struct A {
  unsigned a : 1;
  unsigned : 0;  // 若 unsigned b : 0 则出错，命名的位域宽度不能为 0
  unsigned b : 1;
};

static_assert(sizeof(A) == 8);
```

* 位域的访问方式和其他成员一样

```cpp
struct A {
  char a : 4;
  char b : 4;
};

union U {
  A x;
  char c;
};

int main() {
  U u;
  u.x.a = 0b0001;  // 低 4 位：小端低地址保存低位（C++14 支持二进制字面值）
  u.x.b = 0b0100;    // 高 4 位
  std::cout << u.c;  // A：0b0100'0001 = 65，即 A 的 ASCII 码
}
```

* 通常使用位运算符操作超过 1 位的位域

```cpp
#include <cassert>

class A {
 public:
  char a : 4;
  enum { READ = 01, WRITE = 02 };
  // 通常会为位域定义设置和检验的内联函数
  void setWrite() { a |= WRITE; }
  bool isWrite() { return a & WRITE; }
  bool test() {
    setWrite();
    return isWrite();
  }
};

int main() { assert(A{}.test()); }
```

## 数组

* array of pointers 是一个数组，数组元素都是指针

```cpp
int* p[10];  // [] 优先级高于 *，先结合为数组，有 p[0]、p[1]、p[2]... 等指针
```

* pointer to an array 是一个指针，指向的是一个数组，即数组首元素的指针

```cpp
int (*p)[10];  // p 是指针，指向 int 数组
```

* 可以引用一个数组

```cpp
int (&r)[10] = a;  // r 引用一个含有 42 个整数的数组
```

* 但数组元素不能是引用

```cpp
int& r[10];  // 错误：不存在元素是引用的数组
```

* 结合起来

```cpp
int* p[10];
int* (&r)[10] = p;  // r 是数组的引用，该数组含有 10 个指针
```

* 二维数组名本质上是一个指向数组的指针

```cpp
#include <cassert>
#include <iterator>

int main() {
  int a[2][3] = {1, 2, 3, 4, 5, 6};
  {
    int* p = a[0];  // 相当于把 a 看成有 6 个元素的一维数组 a[6]
    for (int i = 0; i < 2; ++i) {
      for (int j = 0; j < 3; ++j) {
        assert(*(p + i * 3 + j) == a[i][j]);
        assert(p[i * 3 + j] == a[i][j]);
      }
    }
  }
  {
    int(*q)[3] = a;  // q 指向的是 a 的首地址， q + i 相当于移动 i 行
    for (int i = 0; i < 2; ++i) {
      for (int j = 0; j < 3; ++j) {
        assert(*(*(q + i) + j) == a[i][j]);
        assert(q[i][j] == a[i][j]);
        assert(*(*(a + i) + j) == a[i][j]);
      }
    }
  }
  {
    int i = 1;
    for (auto p = a; p != a + 2; ++p) {
      for (auto q = *p; q != *p + 3; ++q) {
        assert(*q == i++);
      }
    }
  }
  {
    int i = 1;
    for (auto p = std::begin(a); p != std::end(a); ++p) {
      for (auto q = std::begin(*p); q != std::end(*p); ++q) {
        assert(*q == i++);
      }
    }
  }
  {
    using IA = int[3];
    int i = 1;
    for (IA* p = a; p != a + 2; ++p) {
      for (int* q = *p; q != *p + 3; ++q) {
        assert(*q == i++);
      }
    }
  }
}
```

## 函数

* 形参可以是数组，但是传递的实际是指向数组首元素的指针

```cpp
// 以下三种声明等价，形参都是 const int * 类型
void print(const int*);
void print(const int[]);
void print(const int[10]);
```

* 必须传数组则可以把数组引用当形参，但长度必须匹配
```cpp
void print(int (&a)[3]) {
  for (auto x : a) {
    std::cout << x << std::endl;
  }
}

int i[2] = {0, 1};
int j[10] = {0, 1, 2};
print(i);  // 错误：实参必须是含有 3 个整数的数组
print(j);  // OK
```

* 多维数组作参数传递的是数组首元素指针
```cpp
void print(int (*a)[10], int rowSize)）{}
// 等价定义，只是看起来像二维数组
void print(int a[][10], int rowSize) {}
```

## 参数入栈与求值顺序

* C++ 一般默认使用 [__cdecl 调用模式](https://docs.microsoft.com/en-us/cpp/cpp/cdecl?view=vs-2019)，入栈顺序为从右往左

```cpp
#include <iostream>

int f1() {
  std::cout << 1;
  return 1;
}
int f2() {
  std::cout << 2;
  return 2;
}
int f3() {
  std::cout << 3;
  return 3;
}

void f(int, int, int) {}

int main() {
  f(f1(), f2(), f3());  // 321
}
```

* 调用模式详见 [MSDN](https://docs.microsoft.com/en-us/cpp/cpp/argument-passing-and-naming-conventions?view=vs-2019)

|Keyword|Stack cleanup|Parameter passing|
|:-:|:-:|:-:|
|[__cdecl](https://docs.microsoft.com/en-us/cpp/cpp/cdecl?view=vs-2019)|Caller|Pushes parameters on the stack, in reverse order (right to left)|
|[__clrcall](https://docs.microsoft.com/en-us/cpp/cpp/clrcall?view=vs-2019)|n/a|Load parameters onto CLR expression stack in order (left to right)|
|[__stdcall](https://docs.microsoft.com/en-us/cpp/cpp/stdcall?view=vs-2019)|Callee|Pushes parameters on the stack, in reverse order (right to left)|
|[__fastcall](https://docs.microsoft.com/en-us/cpp/cpp/fastcall?view=vs-2019)|Callee|Stored in registers, then pushed on stack|
|[__thiscall](https://docs.microsoft.com/en-us/cpp/cpp/thiscall?view=vs-2019)|Callee|Pushed on stack; this pointer stored in ECX|
|[__vectorcall](https://docs.microsoft.com/en-us/cpp/cpp/vectorcall?view=vs-2019)|Callee|Stored in registers, then pushed on stack in reverse order (right to left)|

## 函数指针

* 函数指针即指向函数的指针，指向函数而非对象，函数类型由返回类型和形参类型共同确定，与函数名无关，要声明一个指向此函数的指针只需要用指针替换函数名

```cpp
bool f(int, int);            // 类型为 bool (int, int)
bool (*pf)(int, int) = f;    // 注意 pf 加括号，否则它是一个函数
bool (*pf2)(int, int) = &f;  // 和上面的写法等价
auto pf3 = f;                // 用自动推断省去复杂的类型声明
decltype(f)* pf4 = f;  // decltype 返回函数类型而不是指针类型，得加个 *
```

* 用函数指针调用函数无须解引用

```cpp
bool b1 = pf(1, 2);
bool b2 = (*pf)(1, 2);  // 等价调用，解引用是不必要的
```

* 函数指针可以当形参，也可以把函数名作为实参，此时函数名自动转换成指向该函数的指针，使用类型别名可以简化代码

```cpp
// Func 和 Func2 是等价的类型，都是函数
typedef bool Func(int, int);
typedef decltype(f) Func2;

// FuncP 和 FuncP2 是等价的类型，都是函数指针
typedef bool (*FuncP)(int, int);
typedef delctype(f) * FuncP2;
```

* 返回函数指针时要把返回类型显式写成指针

```cpp
using F = int(int, int);
using PF = int (*)(int, int);
// 以下声明等价
PF f(int);
F* f(int);
int (*f(int))(int, int);
auto f(int) -> int (*)(int, int);
// 返回的函数已知，则可以直接使用 decltype
int g(int, int);
decltype(g)* f(int);  // decltype 返回函数类型而不是指针类型，所以要加 *
```

## static 函数

* static 函数只能在其所在的头文件和对应源文件中使用

```cpp
// a.h
#pragma once

static void f();
void g();

// a.cpp
#include "a.h"

void f() {}
void g() { f(); }

// main.cpp
#include "a.h"

int main() {
  g();  // OK
  f();  // 链接失败，function 'f' has internal linkage but is not defined
}
```

## 内存分布

* C++ 中内存分为 5 个区：数据段包含堆、栈、全局静态区、常量存储区，代码段包含代码区
  * 堆（heap）就是程序员自己分配的内存块，编译器不管释放，由程序员自己释放，如果没释放掉，程序结束后操作系统会自动回收。操作系统有一个记录空闲内存地址的链表，当系统收到程序的申请时，会遍历该链表，寻找第一个空间大于所申请空间的堆结点，然后将该结点从空闲结点链表中删除，并将该结点的空间分配给程序。堆是向高地址扩展的数据结构，是不连续的内存区域，链表遍历方向也是由低地址到高地址，由此可见堆获得的空间比较灵活也比较大，堆的大小受限于计算机系统中有效的虚拟内存（一般最大是 4G，试了试 VS2017，开辟 42919522 个 int 指针触发 std::bad_alloc 异常，大概 163.7MB）。堆一般速度比较慢，而且容易产生内存碎片，不过用起来最方便
  * 栈（stack）就是编译器根据需求自动分配的存储区，通常保存局部变量、函数参数等，程序结束时编译器自动释放。栈是向低地址扩展的数据结构，是一块连续的内存的区域，因此栈顶的地址和栈的最大容量是系统预先规定好的（一般是 1MB，试了试 char a[1031998] 刚好栈溢出）。只要栈的剩余空间大于所申请空间，系统将为程序提供内存，否则将报异常提示栈溢出。栈由系统自动分配，速度较快，但程序员无法控制
  * 全局静态区存放全局变量和静态变量，已初始化的全局和局部静态变量存储在一块区域（.data），未初始化的存储在另一块区域（.bss，不占用磁盘空间），程序结束时系统释放
  * 常量存储区（.rodata）存放常量字符串，程序结束后系统释放
  * 代码区（.text）存放函数体的二进制代码

* `g++ -c test.cpp && objdump -s test.o` 查看段信息

```cpp
// test.cpp
#include <iostream>

int a = 3;  // a 在 .data

int main() {
  std::cout << "hello world";  // hello world 在 .rodata
}
```

* 段信息为

```
Contents of section .text:
 0000 554889e5 be000000 00bf0000 0000e800  UH..............
 0010 000000b8 00000000 5dc35548 89e54883  ........].UH..H.
 0020 ec10897d fc8975f8 837dfc01 7527817d  ...}..u..}..u'.}
 0030 f8ffff00 00751ebf 00000000 e8000000  .....u..........
 0040 00ba0000 0000be00 000000bf 00000000  ................
 0050 e8000000 00c9c355 4889e5be ffff0000  .......UH.......
 0060 bf010000 00e8b0ff ffff5dc3           ..........].
Contents of section .data:
 0000 03000000                             ....
Contents of section .rodata:
 0000 68656c6c 6f20776f 726c6400           hello world.
Contents of section .init_array:
 0000 00000000 00000000                    ........
Contents of section .comment:
 0000 00474343 3a202855 62756e74 7520342e  .GCC: (Ubuntu 4.
 0010 382e342d 32756275 6e747531 7e31342e  8.4-2ubuntu1~14.
 0020 30342e34 2920342e 382e3400           04.4) 4.8.4.
Contents of section .eh_frame:
 0000 14000000 00000000 017a5200 01781001  .........zR..x..
 0010 1b0c0708 90010000 1c000000 1c000000  ................
 0020 00000000 1a000000 00410e10 8602430d  .........A....C.
 0030 06550c07 08000000 1c000000 3c000000  .U..........<...
 0040 00000000 3d000000 00410e10 8602430d  ....=....A....C.
 0050 06780c07 08000000 1c000000 5c000000  .x..........\...
 0060 00000000 15000000 00410e10 8602430d  .........A....C.
 0070 06500c07 08000000                    .P......
```

* `readelf -h test.o` 查看文件头

```
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              REL (Relocatable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x0
  Start of program headers:          0 (bytes into file)
  Start of section headers:          488 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           0 (bytes)
  Number of program headers:         0
  Size of section headers:           64 (bytes)
  Number of section headers:         15
  Section header string table index: 12
```

* `readelf -S test.o` 查看完整段表

```
There are 15 section headers, starting at offset 0x1e8:

Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .text             PROGBITS         0000000000000000  00000040
       000000000000006c  0000000000000000  AX       0     0     1
  [ 2] .rela.text        RELA             0000000000000000  00000890
       00000000000000d8  0000000000000018          13     1     8
  [ 3] .data             PROGBITS         0000000000000000  000000ac
       0000000000000004  0000000000000000  WA       0     0     4
  [ 4] .bss              NOBITS           0000000000000000  000000b0
       0000000000000001  0000000000000000  WA       0     0     1
  [ 5] .rodata           PROGBITS         0000000000000000  000000b0
       000000000000000c  0000000000000000   A       0     0     1
  [ 6] .init_array       INIT_ARRAY       0000000000000000  000000c0
       0000000000000008  0000000000000000  WA       0     0     8
  [ 7] .rela.init_array  RELA             0000000000000000  00000968
       0000000000000018  0000000000000018          13     6     8
  [ 8] .comment          PROGBITS         0000000000000000  000000c8
       000000000000002c  0000000000000001  MS       0     0     1
  [ 9] .note.GNU-stack   PROGBITS         0000000000000000  000000f4
       0000000000000000  0000000000000000           0     0     1
  [10] .eh_frame         PROGBITS         0000000000000000  000000f8
       0000000000000078  0000000000000000   A       0     0     8
  [11] .rela.eh_frame    RELA             0000000000000000  00000980
       0000000000000048  0000000000000018          13    10     8
  [12] .shstrtab         STRTAB           0000000000000000  00000170
       0000000000000072  0000000000000000           0     0     1
  [13] .symtab           SYMTAB           0000000000000000  000005a8
       00000000000001f8  0000000000000018          14    13     8
  [14] .strtab           STRTAB           0000000000000000  000007a0
       00000000000000ed  0000000000000000           0     0     1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), l (large)
  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)
  O (extra OS processing required) o (OS specific), p (processor specific)
```

## new 和 delete

* 内存耗尽时，new 不能分配所需求的内存空间，会抛出 std::bad_alloc 异常，可以使用 placement new 阻止抛出异常

```cpp
int* p = new int(42);  // 分配失败则抛出 std::bad_alloc 异常
int* q = new (std::nothrow) int(42);  // 分配失败则 new 返回空指针
```

* 重载的 `operator new` 和 `operator delete` 不是成员函数，而是 static 函数

```cpp
#include <iostream>

struct A {
  virtual ~A() = default;
  void operator delete(void*) {  // 默认为 static，不能声明为虚函数
    std::cout << 1;
  }
  void operator delete[](void*) { std::cout << 2; }
};
struct B : A {
  void operator delete(void*) { std::cout << 3; }
  void operator delete[](void*) { std::cout << 4; }
};

int main() {
  A* p = new B;
  delete p;  // 3
  A* q = new B[10];
  delete[] q;  // 不可预料的行为
  // 传递给 delete 的指针静态类型必须与动态类型一样
  using PF = void (A::*)(void*);
  PF pf = &A::operator delete;  // 错误，A::operator delete 不是成员函数类型
}
```

* 不要调用析构函数后再次分配原有内存

```cpp
#include <iostream>

struct A {
  ~A() { std::cout << 1; }
};

int main() {
  A a;
  a.~A();
  new (&a) A{};  // 此处重新构造，但可能构造失败
}  // 此处仍会调用 ~A()，如果后一次构造失败，此处析构就会出错
```

## 基类和派生类的定义

* 若基类中定义了 static 成员，则整个继承体系中只存在该成员的唯一定义，静态成员遵循通用的访问控制规则

```cpp
#include <cassert>

int i = 0;

struct A {
  static int f() { return ++i; }
};

struct B : A {
  void g(const B& obj) {
    assert(A::f() == 1);
    assert(B::f() == 2);
    assert(obj.f() == 3);
    assert(f() == 4);
  }
};

int main() {
  B b;
  b.g(b);
}
```

## 访问控制与继承

* 基类指针指向派生类对象时，调用非虚函数则调用基类方法，调用虚函数时会调用派生类重写的虚函数，如果没有重写就往上调用基类的虚函数

```cpp
#include <cassert>

struct A {
  virtual int f() { return 0; }
};

struct B : A {
  // 非虚函数，隐藏了 A::f
  int f(int) { return 1; }
  virtual int f2() { return 2; }
};

struct C : B {
  // 非虚函数，隐藏了 B::f(int)
  int f(int) { return 3; }
  // 虚函数，覆盖了 A::f()
  int f() { return 4; }
  // 虚函数，覆盖了 B::f2
  int f2() { return 5; }
};

int main() {
  A a;
  B b;
  C c;
  {
    A* p1 = &a;
    A* p2 = &b;
    A* p3 = &c;
    assert(p1->f() == 0);
    assert(p2->f() == 0);
    // p2->f2();  // 错误，没有 A::f2()
    assert(p3->f() == 4);
  }
  {
    B* pb = &b;
    C* pc = &c;
    assert(pb->f2() == 2);
    assert(pc->f2() == 5);
  }
  {
    // 对非虚函数 f(int) 的调用
    A* p1 = &c;
    B* p2 = &c;
    C* p3 = &c;
    // p1->f(42);  // 错误，没有 A::f(int)
    assert(p2->f(42) == 1);
    assert(p3->f(42) == 3);
  }
}
```

* 虚函数中调用基类实现的情况

```cpp
#include <cassert>

struct A {
  virtual int f1() {
    ++x;
    return 0;
  }
  int f2() {
    x += 5;
    return 1;
  }
  int x = 0;
};

struct B : A {
  int f1() {
    ++y;
    int t = A::f1();
    assert(t == 0);
    return 2;
  }
  int f2() {
    y -= 3;
    int t = A::f2();
    assert(t == 1);
    return 3;
  }
  int y = 10;
};

int main() {
  B b;
  assert(b.x == 0);
  assert(b.y == 10);
  A& a(b);
  assert(a.f2() == 1);
  assert(b.x == 5);
  assert(b.y == 10);
  assert(b.f2() == 3);
  assert(b.x == 10);
  assert(b.y == 7);
  assert(a.f1() == 2);
  assert(b.x == 11);
  assert(b.y == 8);
  assert(b.f1() == 2);
  assert(b.x == 12);
  assert(b.y == 9);
}
```

## 多重继承与虚继承

* 多重继承的情况下，如果名字在多个基类中被找到，而当前派生类没有覆盖此名字，或没有明确指出调用版本，则对该名字的使用具有二义性

```cpp
#include <cassert>

struct A {
  int i = 0;
};
struct B {
  int i = 1;
};
struct C : A, B {};
struct D : A, B {
  int i = 2;
};

int main() {
  C c;
  D d;
  //   c.i;  // 二义性错误，无法确定访问的是 A::i 还是 B::i
  assert(c.A::i == 0);
  assert(c.B::i == 1);
  assert(d.i == 2);
}
```

* 间接基类中找到也一样具有二义性错误

```cpp
#include <cassert>

struct A {
  int i = 0;
};
struct B {
  int i = 1;
};
struct C : B {};
struct D : A, C {};  // A 是直接基类，B 是间接基类

int main() {
  D d;
  //   d.i;  // 二义性错误，i 可能来自 A 或 B
  assert(d.A::i == 0);
  assert(d.B::i == 1);
  assert(d.C::i == 1);
}
```

* 即使来自同一个间接基类仍然属于二义性错误

```cpp
#include <cassert>

struct A {
  int i = 0;
};
struct B : A {};
struct C : A {};
struct D : B, C {};

int main() {
  D d;
  //   d.i;     // 二义性错误
  //   d.A::i;  // 二义性错误
  assert(d.B::i == 0);
  assert(d.C::i == 0);
}
```

* 因为 D 继承了两次 A，D 对象将包含 A 的多个子对象

```cpp
#include <iostream>

struct A {
  A() { std::cout << "a"; }
};
struct B : A {
  B() { std::cout << "b"; }
};
struct C : A {
  C() { std::cout << "c"; }
};
struct D : B, C {
  D() { std::cout << "d"; }
};

int main() {
  D d;  // abacd
}
```

* 使用虚继承可以解决此问题

```cpp
#include <iostream>

struct A {
  A() { std::cout << "a"; }
};
struct B : virtual public A {  // virtual 和 public 位置随意
  B() { std::cout << "b"; }
};
struct C : public virtual A {
  C() { std::cout << "c"; }
};
struct D : B, C {
  D() { std::cout << "d"; }
};

int main() {
  D d;  // abcd，B 和 C 都是虚继承自 A 所以 D 中只有一个 A 基类部分
}
```

* 此时如果 A 定义了一个成员 i，其在 D 的作用域中通过 D 的两个基类都是可见的，如果通过 D 对象使用 i
* 若 B 和 C 中都没有 i 的定义，i 被解析为 A 成员，不存在二义性，一个 D 对象只含有一个 A 实例

```cpp
#include <cassert>

struct A {
  int i = 0;
};
struct B : virtual A {};
struct C : virtual A {};
struct D : B, C {};

int main() {
  D d;
  assert(d.i == 0);
  assert(d.A::i == 0);
}
```

* 若 i 是 B 或 C 中的某一个成员，同样没有二义性，派生类的 i 比共享虚基类 A 的 i 优先级高

```cpp
#include <cassert>

struct A {
  int i = 0;
};
struct B : virtual A {
  int i = 1;
};
struct C : virtual A {};
struct D : B, C {};

int main() {
  D d;
  assert(d.i == 1);
}
```

* B 和 C 中都有 i 的定义，直接访问 i 产生二义性，最好的解决方法是在 D 中定义新的 i

```cpp
#include <cassert>

struct A {
  int i = 0;
};
struct B : virtual A {
  int i = 1;
};
struct C : virtual A {
  int i = 2;
};
struct D : B, C {};

int main() {
  D d;
  //   d.i;  // 二义性错误
}
```
  
* 虚继承中，虚基类由最低层的派生类初始化，创建 D 时，D 的构造函数独自控制 A 的初始化过程，因为如果应用普通规则，虚基类 A 会被 B 和 C 重复初始化。继承体系中的每个类都可能在某个时刻成为最低层的派生类，比如创建 B 对象时，B 对象就位于派生的最低层，因此 B 的构造函数将直接初始化 A 的基类部分
* 含有虚基类的对象的构造顺序与一般的顺序不同：先用最低层派生类的构造函数的初始值初始化虚基类子部分，然后按直接基类在派生列表中出现的顺序依次初始化

```cpp
struct A {};
struct B : virtual A {};
struct C : virtual A {};
struct X {};
struct D : B, C, X {
  int i;
};
```

* 对上述继承关系，创建一个 D 对象时，顺序为
  * 用 D 的构造函数初始值列表中提供的初始值构造虚基类 A 部分
  * 构造 B 部分
  * 构造 C 部分
  * 构造直接基类 X（虚基类总是先于非虚基类构造，与继承中的次序无关）
  * 构造 D 部分
* 因此 D 的构造函数初值列表按顺序写为

```cpp
D::D(...) : A(...), B(...), C(...), X(...), i(...) {}
```

* 如果 D 没有显式初始化 A，则 A 的默认构造函数将被调用，如果 A 没有默认构造函数则出错
* 一个类可以有多个虚基类，此时虚的子对象按它们在派生列表中的顺序从左向右依次构造

```cpp
#include <iostream>

struct A {
  A() { std::cout << "a"; }
};
struct B : A {
  B() { std::cout << "b"; }
};
struct C {
  C() { std::cout << "c"; }
};
struct D : C {
  D() { std::cout << "d"; }
};
struct E {
  E() { std::cout << "e"; }
};
struct F : virtual E {
  F() { std::cout << "f"; }
};
struct X : B, F, virtual D {
  X() { std::cout << "x"; }
};

int main() {
  X x;  // ecdabfx
}
```

* F 有两个虚基类：间接虚基类 E 和直接虚基类 D。编译器按直接基类的声明顺序依次检查，确定其中是否含有虚基类，如果有先构造虚基类，然后按声明顺序构造其他非虚基类，因此创建一个 F 对象，顺序为

```cpp
E();  // 直接基类F的虚基类
C();  // 直接虚基类D的基类
D();  // 直接虚基类
A();  // 第一个非虚基类B的基类
B();  // 第一个直接非虚基类B
F();  // 第二个直接非虚基类F
X();  // 最低层的派生类
```

* 合成的拷贝、赋值和移动构造函数顺序同上，销毁顺序则恰好相反
* 在 VS 中查看类的内存布局：`Properties - Configuration Properties - C/C++ - Command Line -
 Addtion Options - /d1 reportAllClassLayout`，查看单个类如类 A 的内存布局则添加 `/d1 reportSingleClassLayoutA`，`Build - Rebuild Solution` 后在 `Output` 中即可看到结果。比如对如下代码

```cpp
class A {
  virtual void f() = 0;
  virtual void g() {}
  virtual void h() {}
  int i;
  char a;
  double d;
};

class B : public A {
  virtual void g() {}
  virtual void f() {}
  int* p;
};

int main() {}
```

* VS 中设置 `/d1 reportAllClassLayout` 后，调试时可以在输出结果中找到如下内容

```
1>class A	size(24):
1>	+---
1> 0	| {vfptr}
1> 8	| i
1>12	| a
1>  	| <alignment member> (size=3)
1>16	| d
1>	+---
1>A::$vftable@:
1>	| &A_meta
1>	|  0
1> 0	| &A::f
1> 1	| &A::g
1> 2	| &A::h
1>A::f this adjustor: 0
1>A::g this adjustor: 0
1>A::h this adjustor: 0
1>class B	size(32):
1>	+---
1> 0	| +--- (base class A)
1> 0	| | {vfptr}
1> 8	| | i
1>12	| | a
1>  	| | <alignment member> (size=3)
1>16	| | d
1>	| +---
1>24	| p
1>	+---
1>B::$vftable@:
1>	| &B_meta
1>	|  0
1> 0	| &B::f
1> 1	| &B::g
1> 2	| &A::h
1>B::g this adjustor: 0
1>B::f this adjustor: 0
```

* 在 Linux 中查看类的内存布局，直接用下面的命令即可将 xxx.cpp 的内存布局导出到文件中

```sh
# g++ -fdump-class-hierarchy xxx.cpp # 生成文件 xxx.cpp.002t.class
g++ -fdump-lang-class xxx.cpp # g++ 8.1 之后，生成文件 a-xxx.cpp.001l.class
cat a-xxx.cpp.001l.class

Vtable for A
A::_ZTV1A: 5 entries
0     (int (*)(...))0
8     (int (*)(...))(& _ZTI1A)
16    (int (*)(...))__cxa_pure_virtual
24    (int (*)(...))A::g
32    (int (*)(...))A::h

Class A
   size=24 align=8
   base size=24 base align=8
A (0x0x7fc793d7b420) 0
    vptr=((& A::_ZTV1A) + 16)

Vtable for B
B::_ZTV1B: 5 entries
0     (int (*)(...))0
8     (int (*)(...))(& _ZTI1B)
16    (int (*)(...))B::f
24    (int (*)(...))B::g
32    (int (*)(...))A::h

Class B
   size=32 align=8
   base size=32 base align=8
B (0x0x7fc793c201a0) 0
    vptr=((& B::_ZTV1B) + 16)
A (0x0x7fc793d7b600) 0
      primary-for B (0x0x7fc793c201a0)
```

## 通过虚函数表访问 private 成员

```cpp
#include <cassert>
#include <cstdint>
#include <memory>

class A {
 private:
  virtual int f() { return 0; }
  virtual int g() { return 1; }
  virtual int h() { return 2; }
  int n = 42;
};

class B : public A {
 private:
  virtual int f() { return 3; }
  virtual int g() { return 4; }
  virtual int h() { return 5; }
};

int main() {
  std::unique_ptr<A> p = std::make_unique<B>();
  auto a = p.get();
  using F = int (*)();
  F pf = reinterpret_cast<F>(
      *(reinterpret_cast<intptr_t*>(*reinterpret_cast<intptr_t*>(&*a)) + 1));
  assert(pf() == 4);
  F pf2 = reinterpret_cast<F>(
      *(reinterpret_cast<intptr_t*>(*(intptr_t*)(&*a)) + 1));
  assert(pf2() == 4);
  F pf3 = reinterpret_cast<F>(
      *(reinterpret_cast<intptr_t*>(*(intptr_t*)(&*a) + sizeof(void*))));
  assert(pf3() == 4);
  int* member_var_n_addr =
      reinterpret_cast<int*>(reinterpret_cast<intptr_t*>(a) + 1);
  assert(*member_var_n_addr == 42);
  auto vtable_addr =
      reinterpret_cast<intptr_t*>(*reinterpret_cast<intptr_t*>(a));
  intptr_t vfunc1_addr = *vtable_addr;
  F pf4 = reinterpret_cast<F>(vfunc1_addr);
  assert(pf4() == 3);
  intptr_t vfunc2_addr = *(vtable_addr + 1);
  F pf5 = reinterpret_cast<F>(vfunc2_addr);
  assert(pf5() == 4);
  intptr_t vfunc3_addr = *(vtable_addr + 2);
  F pf6 = reinterpret_cast<F>(vfunc3_addr);
  assert(pf6() == 5);
}
```
