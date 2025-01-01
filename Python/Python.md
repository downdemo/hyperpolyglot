* [Python Doc](https://docs.python.org/3)
* [PyPI](https://pypi.org/)

## 流程控制

* 输入、输出

```py
print('hi')
print(type('hi'))  # str
print(type(3.14))  # float

i = input('input value of i: ')  # input 返回值是 str 类型
if int(i) > 0:
    print(i)
```

* if

```py
i = -1
if i > 0:
    print(i)
elif i == 0:
    print(0)
else:
    print(-i)
```

* while

```py
res = 0
n = 10
while n > 0:
    res = res + n
    n = n - 1
    if n == 1:
        break
print(res)  # 54
```

* for...in

```py
from collections.abc import Iterable


x = 'hello'
if isinstance(x, Iterable):
    for i in x:
        print(i)
```

## 运算符

* [and、or](https://docs.python.org/3.9/library/stdtypes.html#boolean-operations-and-or-not) 相当于 &&、||

```py
# 如果含有 0，则返回 0，否则 and 返回后一个值
print(0 and 1)  # 0
print(1 and 2)  # 2

# or 返回第一个非 0 值，如果均为 0 则返回 0
print(0 or 0)  # 0
print(1 or 2)  # 1
```

* &、| 则是与非计算

```py
print(1 & 2)  # 0（0b01 & 0b10 = 0b00）
print(1 | 2)  # 3（0b01 | 0b10 = 0b11）
```

* 除法

```py
print(10 / 3)  # 3.3333333333333335
print(10 // 3)  # 3
```

* 不存在自增和自减运算符

```py
a = 1
--a  # --被视为两次取负（因此a--是错误的）
print(a)  # 1
```

## 变量

```py
a = True
b = False
print(a and b)  # False
print(a or b)  # True
print(not a)  # False

print(None)  # None是一种特殊的数据类型
print(type(None))  # NoneType

PI = 3.1415  # 一般用大写表示常量（只是表示，实际还是变量）
```

# 进制

```py
i = 21834
print(bin(i))  # 0b101010101001010
print(oct(i))  # 0o52512
print(hex(i))  # 0x554a

print(format(i, 'b'))  # 101010101001010
print(format(i, 'o'))  # 52512
print(format(i, 'x'))  # 554a
```

## [str](https://docs.python.org/3.9/library/stdtypes.html#text-sequence-type-str)

* Python3 的字符串用 Unicode 编码

```py
#!/usr/bin/env python3
# -*- coding: utf-8 -*-

print(ord('啊'))  # 21834
print(chr(21834))  # 啊
print('\u554a')  # 啊
```

* 转换为指定编码

```py
print('ABC'.encode('ascii'))  # b'ABC'
print('中文'.encode('utf-8'))  # b'\xe4\xb8\xad\xe6\x96\x87'

print(b'ABC'.decode('ascii')) # ABC
print(b'\xe4\xb8\xad\xe6\x96\x87'.decode('utf-8')) # 中文
print(b'\xe4\xb8\xad\xe6'.decode('utf-8', errors = 'ignore')) # 中（忽略不能编码的部分）
```

* 长度

```py
print(len('ABC'))  # 3
print(len('中文'))  # 2
print(len(b'ABC'))  # 3
print(len(b'\xe4\xb8\xad\xe6\x96\x87'))  # 6
```

* 转大小写

```py
print('AbC'.lower())  # abc
print('AbC'.upper())  # ABC
```

## [list](https://docs.python.org/3.9/library/stdtypes.html#list)

```py
x = ['b', 'c', 'd']
print(len(x))
print(x[0])
print(x[-1])
x.append('e') 
print(x.pop())  # 移除尾元素
x.insert(0, 'a')
print(x.pop(0))  # 移除第0个元素
x.extend([1, 2, 3])  # 添加另一个list的所有元素

for k, v in enumerate(x):  #  k索引，v值
    print(k, v)

for x, y in [('a', 0), ('b', 1), ('c', 2)]:
    print(x, y)
```

* list 元素类型可以不同

```py
x = [1, 'a', [1, 2], False]
```

* 切片（也适用于 str 和 tuple）

```py
x = [1, 2, 3, 4, 5, 6, 7, 8]
x[1:2]  # 返回范围[1, 2)中的元素组成的list
x[:2]  # 等价于x[0:2]
x[1:]  # 等价于x[1:len(x)]
x[-2:]  # 等价于x[len(x)-2, len(x)]
x[:]  # 等价于x

x[::2]  # 每两个元素取一个
x[::-1]  # 翻转list
```

* list comprehensions

```py
list(range(1, 4))  # [1, 2, 3]
list(range(4))  # [0, 1, 2, 3]
[x * x for x in range(1, 4)]  # [1, 4, 9]
[x + y for x in 'AB' for y in 'CD']  # ['AC', 'AD', 'BC', 'BD']
[k + '=' + v for k, v in {'a': 'apple', 'b': 'banana'}.items()]
```

## [tuple](https://docs.python.org/3.9/library/stdtypes.html#tuple)

* tuple 初始化后就不能修改

```py
x = (1, 2, 3)
print(len(x))  # 3
print(x[0])  # 1
print(x[-1])  # 3

for i in x:
    print(i)
```

* 但 tuple 中的 list 是可修改的

```py
x = (1, [2, 3])
x[1][0] = 0  # x现在是(1, [0, 3])
```

* 对于只有一个元素的 tuple

```py
x = (1)  # x是1
x = (1, )  # x是包含元素1的tuple
```

## [dict](https://docs.python.org/3.9/library/stdtypes.html#mapping-types-dict)

```py
x = {'a': 0, 'b': 1}
if 'a' in x:
    print(x['a'])
if x.get('b') != None:
    print(x['b'])
x['c'] = 2
if x.get('c', -1) != -1:
    print(x['c'])
print(x.pop('c'))  # 2
```

* 迭代

```py
x = {'a': 0, 'b': 1}
for k in x:
    print(k)
for v in x.values():
    print(v)
for k, v in x.items():
    print(k, v)
```

## [set](https://docs.python.org/3.9/library/stdtypes.html#set)

* set 用 list 初始化

```py
x = set([1, 2, 2, 3])  # x元素为1、2、3
x.add(4)
x.remove(4)

y = set([2, 3, 4])
print(x & y)  # 23
print(x | y)  # 1234
```

## [Enum](https://docs.python.org/3.9/library/enum.html#enum.Enum)

```py
from enum import Enum


x = Enum('Color', ('red', 'blue', 'white'))

for name, member in x.__members__.items():
    print(name, member)

print(x.red)  # Color.red
print(x['red'])  # Color.red
print(x.red.value)  # 1
print(x(1))  # Color.red
```

* 枚举值默认从 1 开始，如果要自定义值则可以从 Enum 派生

```py
from enum import Enum, unique


@unique  # 如果存在重复的自定义值则报错
class Color(Enum):
    red = 0
    blue = 1
    white = 2


print(Color.red == Color['red'])  # True
print(Color.red.value)  # 0
print(Color(1))  # Color.blue
```

## 正则表达式

```py
import re


if re.match(r'\d{3}', '123'):
    print(1)
```

* 分组

```py
import re


x = re.match(r'(\d+)-(\d)-(\d)', '1-2-3')
print(x.group(0))  # 1-2-3
print(x.group(1))  # 1
print(x.group(2))  # 2
print(x.group(3))  # 3
print(x.groups())  # ('1', '2', '3')
```

* 切分字符串

```py
import re


s = '1,  2 ,   3'
x = re.split(r'[\s\,]+', s)
print(x)  # ['1', '2', '3']
```

* 正则表达式使用时会被先编译，如果要多次使用同一正则表达式，可以预先编译并保存编译后的结果，这样只需要编译一次

```py
import re


r = re.compile(r'(\d+)-(\d)-(\d)')
x = r.match('1-2-3')
print(x.groups())  # ('1', '2', '3')
```

## 函数

```python
def f(x):
    return x


print(f(1))
g = f
print(g(1))
```

* 空函数

```python
def f():
    pass  # 什么都不做，相当于占位符
```

* 从其他文件导入函数

```python
# hi.py
#!/usr/bin/env python3
# -*- coding: utf-8 -*-


__author__ = 'downdemo'


_x = 'hi'


def f():
    print(_x)


if __name__ == '__main__':
    f()


# test.py
from hi import f


print(f(1))


# test2.py
import hi


print(hi.f(1))
```

* 类型检查

```python
def f(x):
    if not isinstance(x, (int, float)):
        raise TypeError('bad operand type')
    else:
        return x
```

* 返回多个值（实际是返回一个 tuple）

```python
def f():
    return 1, 2


x = f()  # x = (1, 2)
y, z = f()  # y = 1, z = 2
```

* 默认参数

```python
def f(x=1):
    print(x)


f()  # 1
```

* 跳过靠前的参数直接指定靠后的参数

```python
def f(x=1, y=1):
    print(y)


f(y=2)  # 2
```

* 默认参数的问题

```python
def f(x=[]):
    x.append(1)
    print(x)


f()  # [1]
f()  # [1, 1]
```

* 将默认参数设为不变对象

```python
def f(x=None):
    if x is None:
        x = []
    x.append(1)
    print(x)


f()  # [1]
f()  # [1]
```

* 可变参数

```python
def f(*x):
    res = 0
    for i in x:
        res = res + i
    return res


print(f(1, 2, 3))  # 6
x = [1, 2, 3]
print(f(*x))  # 6
```

* 关键字参数

```python
def f(**x):
    print(x)


f(a=1, b=2)  # {'a': 1, 'b': 2}
x = {'a': 1, 'b': 2}  # {'a': 1, 'b': 2}
f(**x)
```

* 关键字参数的检查

```python
def f(**x):
    if 'b' in x:
        print(x['b'])


f(a=1, b=2)  # 2
```

* 命名关键字参数

```python
def f(a, *, b, c):  # *之后的为命名关键字参数
    print(a, b, c)


# 必须传入相应的参数名
f(1, b=2, c=3)  # 1 2 3
f(1, c=3, b=2)  # 1 2 3
```

* 命名关键字参数可指定默认参数

```python
def f(a, *, b=2, c=3):
    print(a, b, c)


f(1)  # 1 2 3
```

* 可变参数之后的也是命名关键字参数

```python
def f(a, *x, b, c):
    print(a, b, c)


f(1, b=2, c=3)  # 1 2 3
```

* 以上参数的定义顺序是：必选参数、默认参数、可变参数、命名关键字参数、关键字参数

```python
def f(a, b=0, *x, c, **kw):
    print(a, b, x, c, kw)


f(1, 2, 0, c = 0, n = 42)  # 1 2 (0,) 0 {'n': 42}
x = (1, 2, 0)
kw = {'c': 0, 'n': 42}
f(*x, **kw)  # 1 2 (0,) 0 {'n': 42}
```

* 尾递归（返回值为函数本身）优化

```python
def f(x):
    return g(x, 1)


def g(x, n):
    if x == 1:
        return n
    return g(x-1, x*n)


print(f(1000))  # 仍会栈溢出，python解释器不支持尾递归优化
```

## 高阶函数

* 函数本身也可以作为参数

```py
def f(x, y, g):
    return g(x) + g(y)


print(f(1, -1, abs))  # 2
```

* 函数中也可以定义函数

```py
def f():
    def g():
        return 1

    return g()
```

* map

```py
def f(x):
    return x * x


list(map(f, [1, 2, 3]))  # [1, 4, 9]
list(map(lambda x: x * x, [1, 2, 3]))  # [1, 4, 9]
```

* filter

```py
list(filter(lambda x: x % 2 == 0, [1, 2, 3, 4, 5]))  # [1, 3, 5]
```

* reduce

```py
from functools import reduce


print(reduce(lambda x, y: x + y, [1, 2, 3, 4]))  # 10
```

* sorted

```py
# 按 key 的返回值排序
sorted([-2, 1, 3], key=abs, reverse=True)  # [3, -2, 1]
```

## 偏函数

* 偏函数的作用相当于设置默认参数

```py
import functools


def f(x, y):
    print(x, y)


f2 = functools.partial(f, 'a')
f2(1)  # a 1
# 相当于
f('a', 1)


f3 = functools.partial(f, y ='a')
f3(1)  # 1 a
# 相当于
kw = {'y': 'a'}
f(1, 'a')  # 1 a
```

## 惰性求值

```py
def f(*x):
    def g():
        res = 0
        for i in x:
            res = res + i
        return res

    return g


x = f(1, 2, 3, 4)  # 返回的是函数
x()  # 调用时才求值
```

* 惰性求值不是立即计算，因此使用的变量值改变时也会改变其延迟计算的结果

```py
def f():
    x = 1

    def g():
        return x
    x = x + 1
    return g


print(f()())  # 2
```

## decorator

* 装饰器的作用是，添加额外功能而不用修改函数的定义

```py
import functools


def log(f):
    @functools.wraps(f)  # 令wraps.__name__为f.__name__
    def wrapper(*x, **kw):
        print(1)
        return f(*x, **kw)

    return wrapper


@log
def f():
    pass


f()  # 1
print(f.__name__)  # f
```

* 带参数的 decorator

```py
import functools


def log(i):
    def decorator(f):
        @functools.wraps(f)
        def wrapper(*x, **kw):
            print(i)
            return f(*x, **kw)

        return wrapper

    return decorator


@log(1)
def f():
    pass


f()  # 1
```

## generator

```py
g = (x * x for x in range(5))

print(next(g))  # 0
print(next(g))  # 1
print(next(g))  # 4

for i in g:  # 继续调用next直到获取所有的值
    print(i)  # 9 16
```

* 包含 yield 的函数会被视为 generator

```py
def f():
    x = 0
    while x < 5:
        yield x * x
        x = x + 1


g = f()

print(next(g))  # 0（next返回第一个yield值）
print(next(g))  # 1（next返回第二个yield值）

for i in g:
    print(i)  # 4 9 16
```

* 获取 generator 函数的返回值

```py
def f():
    x = 0
    while x < 5:
        yield x
    return 'OK'


g = f()


while True:
    try:
        x = next(g)
    except StopIteration as e:
        print(e.value)  # OK
        break
```

* generator 对象是 Iterator 对象（可以被不断 next 计算下一个数据的对象）

```py
from collections.abc import Iterable
from collections.abc import Iterator


g = (x for x in range(5))
print(isinstance(g, Iterable))  # True
print(isinstance(g, Iterator))  # True
```

* send

```py
def f():
    a = yield 1
    print(a)
    b = yield 2


x = f()
print(x.send(None))  # 1（相当于next(x)）
y = x.send('hi')  # hi（传值给当前yield，即a，并调用next）
print(y)  # 2
```

* [asyncio](https://docs.python.org/3.9/library/asyncio.html)

```py
import asyncio


@asyncio.coroutine
def f():
    print(1)
    r = yield from asyncio.sleep(1)
    print(2)


x = asyncio.get_event_loop()
f1 = f()
f2 = f()
tasks = [f1, f2]
x.run_until_complete(asyncio.wait(tasks))
x.close()


# 输出如下
1
1
2  # 1秒后
2
```

* Python3.5 引入了 async 和 await 作为语法糖来简化代码

```py
import asyncio


async def f():
    print(1)
    r = await asyncio.sleep(1)
    print(2)


x = asyncio.get_event_loop()
tasks = [f(), f()]
x.run_until_complete(asyncio.wait(tasks))
x.close()


# 输出如下
1
1
2  # 1秒后
2
```

## 类

```py
class A:  # 即class A(object)
    def __init__(self, i):
        self.__i = i  # 私有变量__i被解释器视为_A__i

    def get_i(self):
        return self.__i

    def set_i(self, i):
        self.__i = i

    def f(self):
        print(self.__i)


class B(A):  # B是A的派生类
    def f(self):  # 重写基类方法
        print('a')

    def g(self):
        super().f()  # 调用基类方法（继承多个基类的同名方法时使用第一个基类的方法）


def g(x):
    x.f()


x = A(1)
x.f()  # 1
g(x)  # 1
print(x._A__i)  # 1
x.set_i(2)
print(x.get_i())  # 2


y = B(1)
print(isinstance(y, A))  # True
y.f()  # a
g(y)  # a
```

* 定义特殊方法

```py
class A:
    def __len__(self):
        return 42


print(len(A()))  # 42
```

* 操作属性和方法

```py
class A:
    def __init__(self):
        self.__i = 1

    def f(self):
        print(self.__i)


x = A()
print(dir(x))  # x的所有属性和方法
print(hasattr(x, 'f'))  # True
g = getattr(x, 'f')
g()  # 等价于x.f()

setattr(x, 'n', 42)  # 添加一个值为42的属性n
print(hasattr(x, 'n'))  # True
print(x.n)  # 42
print(getattr(x, 'n'))  # 42
print(getattr(x, 'm', 100))  # 属性m不存在则返回100
```

* 可以直接给对象添加属性和方法

```py
from types import MethodType


class A:
    __slots__ = ('i', 'f')  # 限制对象可添加的属性和方法


x = A()
x.i = 1
print(x.i)  # 1
del x.i
print(hasattr(x, 'i'))  # False


def f(self, i):
    print(i)


x.f = MethodType(f, x)
x.f(1)  # 1
```

* 也可以直接给类添加属性和方法

```py
class A:
    __slots__ = ('j')  # 仅限制对象，对类无影响


def f(self, i):
    print(i)


A.i = 1  # OK
A.f = f  # OK


x = A()
print(x.i)  # 1
x.f(2)  # 2
```

* 若派生类中未定义 `__slots__`，则基类的限制不起作用

```py
class A:
    __slots__ = ('i',)


class B(A):
    pass


x = B()
x.j = 1  # OK
```

* 若派生类也定义了 `__slots__`，则相当于加上了基类的 `__slots__`

```py
class A:
    __slots__ = ('i',)


class B(A):
    __slots__ = ('j',)


x = B()
x.i = 1  # OK
x.j = 1  # OK
```

* 用 `@property` 将方法变为属性

```py
class A:
    def __init__(self, i):
        self.__i = i

    @property
    def i(self):
        print('g')
        return self.__i

    @i.setter  # 可以不定义setter，则i就是只读属性
    def i(self, i):
        print('s')
        self.__i = i


x = A(1)
x.i = 2  # s（若未定义i.setter则不可修改x.i）
res = x.i  # g
print(res)  # 2
```

* 定制类

```py
class A:
    def __init__(self):
        self.__i = 0    

    def __str__(self):  # print(A()) 时调用
        return 'object of class A'

    __repr__ = __str__  # 在命令行执行 A() 时调用

    def __iter__(self):  # 提供 for...in 迭代的对象
        return self

    def __next__(self):  # 迭代时会不断调用此方法
        self.__i = self.__i + 1
        if self.__i > 10:
            raise StopIteration()
        return self.__i

    def __getitem__(self, n):  # 下标访问时调用，如A()[n]
        if isinstance(n, int):
            return n
        if isinstance(n, slice):
            print(n.start, n.stop, n.step)
            return []

    def __getattr__(self, attr):  # 访问不存在的属性时调用
        if attr == 'n':
            return 1
        if attr == 'f':
            return lambda: 'hi'
        raise AttributeError('class A has no %s' % attr)

    def __call__(self, *x, **kw):  # 将类对象作为函数调用时调用，如A()()
        print('call A()()')
```

* 类的类型是 `type`

```py
class A:
    pass


print(type(A))  # class type
print(type(A()))  # class A
```

* 用 `type()` 创建类

```py
def f(self):
    print(self.__i)


A = type('A', (object,), {'__i': '1', 'f': f})


x = A()
x.f()
```

* metaclass

```py
class A(type):
    def __new__(cls, name, bases, attrs):  # 当前类、类名（str）、基类集合（tuple）、类方法集合（dict）
        if cls == A:
            print(1)
        if name == 'MyList':
            print(2)
        if name == (list,):
            print(3)
        attrs['f'] = lambda self, x: self.append(x)
        attrs['g'] = lambda self: self.pop()
        return type.__new__(cls, name, bases, attrs)


class MyList(list, metaclass=A):  # 123（创建类时调用A.__new__，而非在创建类对象时）
    pass


x = MyList()
x.f(1)
print(x)  # [1]
print(x.g())  # 1
```

## [异常](https://docs.python.org/3.9/library/exceptions.html)

* 捕获异常

```py
import logging


try:
    x = 1 / 0
    print(x)
except ZeroDivisionError as e:
    logging.exception(e)
finally:
    print('done')
```

* 抛出异常

```py
def f(x):
    if x == 0:
        raise ValueError('invalid value')
    print(1 / x)


f(0)
```

* 断言

```py
def f(x):
    assert x != 0, 'invalid value'
    print(1 / x)


f(0)  # assert抛出AssertionError异常
```

## [I/O](https://docs.python.org/3.9/library/io.html)

* 文件读写

```py
# 写文件（无此文件则创建一个新文件）
f = open('test.txt', 'w')  # 覆盖写，追加写则使用'a'模式
f.write('hello\nworld');
f.close();

# 读文件（文件必须已存在）
f = open('test.txt', 'r')  # 若要读取二进制文件则使用'rb'模式
print(f.read(2))  # 读2个字符
print(f.readline(2))  # 读一行的2个字符
f.close()
```

* 用 with 语句在结束时自动调用 close

```py
with open('test.txt', 'r') as f:
    for x in f.readlines():  # readlines返回元素为行文本的list
        print(x.strip())  # 去掉末尾的换行符
```

* StringIO

```py
from io import StringIO


f1 = StringIO()
f1.write('hi')
print(f1.getvalue())

f2 = StringIO('123\n456\n789')
for x in f2.readlines():
    print(x.strip())
```

* BytesIO

```py
from io import BytesIO


f1 = BytesIO()
f1.write('中文'.encode('utf-8'))
print(f1.getvalue())  # b'\xe4\xb8\xad\xe6\x96\x87'

f2 = BytesIO(b'\xe4\xb8\xad\xe6\x96\x87')
print(f2.read().decode('utf-8'))  # 中文
```

* 文件操作

```py
import os


print(os.name)  # 系统名称，Windows为nt，Unix为posix
print(os.environ)  # 所有环境变量
print(os.environ.get('PATH'))  # PATH环境变量
print(os.environ.get('x', 'invalid'))  # 存在x环境变量则返回其值，否则返回第二个参数

curPath = os.path.abspath('.')  # 绝对路径
newPath = os.path.join(curPath, 'test')  # 用path.join拼接文件路径，不要直接拼接字符串
os.mkdir(newPath)  # 创建目录（已存在则无法创建）
os.rmdir(newPath)  # 移除目录

os.rename('test.txt', 'xxx.txt')  # 重命名文件
os.remove('xxx.txt')  # 删除文件

print(os.path.split('a/b/x.py'))  # 拆分出当前文件名：('a/b', 'x.py')
print(os.path.splitext('a/b/x.py'))  # 拆分出后缀名：('a/b/x', '.py')
```

* 序列化（Python 中称为 picking）

```py
import pickle


x = {'a': 0, 'b': 1}
with open('x.txt', 'wb') as f:
    pickle.dump(x, f)  # f.write(pickle.dumps(x))
with open('x.txt', 'rb') as f:
    y = pickle.load(f)  # y = pickle.loads(f.read())
assert x == y
```

* [json](https://docs.python.org/3.9/library/json.html#module-json)

```py
import json


x = {'a': 0, 'b': 1}
y = json.dumps(x)
z = json.loads(y)
assert x == z
```

* 对类的 json 序列化

```py
import json


class X:
    def __init__(self, a, b):
        self.a = a
        self.b = b


def f(x):  # 类到json的转换规则
    return {
        'a': x.a,
        'b': x.b,
    }


def g(x):  # json到类的转换规则
    return X(x['a'], x['b'])

x = X(1, 2)
y = json.dumps(x, default=f)  # f也可以换成lambda o: o.__dict__
z = json.loads(y, object_hook=g)
assert x.__dict__ == z.__dict__
```

## 多进程

* [os.fork()](https://docs.python.org/3.9/library/os.html#os.fork)

```py
# Unix 中才能调用 os.fork()
import os


pid = os.fork()  # 分别在父进程和子进程中返回
if pid == 0:  # 子进程
    print(os.getpid())  # 37：自身（子进程）ID
    print(os.getppid())  # 36：父进程ID
else:
    print(os.getpid())  # 36：自身（父进程）ID
    print(pid)  # 37：子进程ID
```

* [multiprocessing.Process](https://docs.python.org/3.9/library/multiprocessing.html#multiprocessing.Process)

```py
from multiprocessing import Process
import os


def f(x):  # 子进程任务
    print(x, os.getpid())


if __name__ == '__main__':
    print('parent id: %s' % os.getppid())  # 父进程
    p = Process(target=f, args=('hi',))
    p.start()
    p.join()


# 输出如下
parent id: 13924
hi 14128
```

* [Pool](https://docs.python.org/3.9/library/multiprocessing.html#using-a-pool-of-workers)

```py
import os
import time
from multiprocessing import Pool


def f(x):
    print(x, os.getpid())
    time.sleep(1)


if __name__ == '__main__':
    print('parent id: %s' % os.getpid())
    p = Pool(3)
    for i in range(10):
        p.apply_async(f, args=(i,))
    p.close()  # join之前要close
    p.join()


# 输出如下
parent id: 16176
0 12368
1 14224
2 13720
3 12368  # 1秒后
4 14224
5 13720
6 12368  # 1秒后
7 14224
8 13720
9 12368  # 1秒后
```

* [subprocess](https://docs.python.org/3.9/library/subprocess.html)

```py
#!/usr/bin/python3
import signal
import subprocess


def run_shell_cmd(cmd: str, match_str: str, timeout: int) -> bool:
    p = subprocess.Popen(
        cmd,
        shell=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        preexec_fn=lambda: signal.alarm(timeout),
    )
    for line in p.stdout:
        line_str = line.decode('utf-8').rstrip()
        print(line_str)
        if match_str in line_str:
            p.kill()
            return True
    return p.wait() == 0


if __name__ == '__main__':
    print(run_shell_cmd('telnet 127.0.0.1 12333', "Escape character is '^]'", 3))
```

* 进程间通信

```py
import os
import time
from multiprocessing import Process, Queue


def write(q):  # 每秒向队列写一个数
    print('w id: %s' % os.getpid())
    for i in [1, 2, 3]:
        q.put(i)
        time.sleep(1)


def read(q):  # 不停地从队列读取数
    print('r id: %s' % os.getpid())
    while True:
        i = q.get(True)
        print(i)


if __name__ == '__main__':
    q = Queue()
    a = Process(target=write, args=(q,))
    b = Process(target=read, args=(q,))
    a.start()
    b.start()
    a.join()
    b.terminate()  # 死循环，只能强制终止


# 输出如下
w id: 8088
r id: 13452
1
2  # 1秒后
3  # 1秒后
```

## 分布式进程

* serve.py

```py
import queue
from multiprocessing.managers import BaseManager

task_queue = queue.Queue()
result_queue = queue.Queue()


def re_task_queue():
    global task_queue
    return task_queue


def re_result_queue():
    global result_queue
    return result_queue


class QueueManager(BaseManager):
    pass


if __name__ == '__main__':
    QueueManager.register('get_task_queue', callable=re_task_queue)
    QueueManager.register('get_result_queue', callable=re_result_queue)
    m = QueueManager(address=('127.0.0.1', 5000), authkey=b'123')
    m.start()
    task = m.get_task_queue()
    result = m.get_result_queue()
    for i in range(10):
        task.put(i)
    for i in range(10):
        r = result.get(timeout=10)
        print(r)
    m.shutdown()
```

* client.py

```py
import time
from multiprocessing.managers import BaseManager


class QueueManager(BaseManager):
    pass


QueueManager.register('get_task_queue')
QueueManager.register('get_result_queue')

m = QueueManager(address=('127.0.0.1', 5000), authkey=b'123')
m.connect()
task = m.get_task_queue()
res = m.get_result_queue()
for i in range(10):
    n = task.get(timeout=1)
    print('cal %d * %d...' % (n, n))
    r = '%d * %d = %d' % (n, n, n * n)
    time.sleep(1)
    res.put(r)
```

* 先运行 serve.py，再运行 cilent.py，cilent.py 运行结果如下

```py
cal 0 * 0...
cal 1 * 1...  # 1秒后
cal 2 * 2...  # 1秒后
cal 3 * 3...  # 1秒后
cal 4 * 4...  # 1秒后
cal 5 * 5...  # 1秒后
cal 6 * 6...  # 1秒后
cal 7 * 7...  # 1秒后
cal 8 * 8...  # 1秒后
cal 9 * 9...  # 1秒后
```

* serve.py 运行结果如下

```py
0 * 0 = 0  # cilent.py显示cal 0 * 0...后
1 * 1 = 1  # cilent.py显示cal 1 * 1...后，以下同理
2 * 2 = 4
3 * 3 = 9
4 * 4 = 16
5 * 5 = 25
6 * 6 = 36
7 * 7 = 49
8 * 8 = 64
9 * 9 = 81
```

## 多线程

* [threading](https://docs.python.org/3.9/library/threading.html)

```py
import threading
import time


def f():
    print('worker thread name: %s' % threading.current_thread().name)
    for i in range(3):
        print(i, threading.current_thread().name)
        time.sleep(1)


print('main thread name: %s' % threading.current_thread().name)
t = threading.Thread(target=f, name='worker')
t.start()
t.join()


# 输出如下
main thread name: MainThread
worker thread name: worker
0 worker
1 worker  # 1秒后
2 worker  # 1秒后
```

* [threading.Lock](https://docs.python.org/3.9/library/threading.html#threading.Lock)

```py
import threading

n = 0
lk = threading.Lock()


def f(x):
    global n
    n = n + x
    n = n - x


def g(x):
    for i in range(100000):
        lk.acquire()
        try:
            f(x)
        finally:
            lk.release()


a = threading.Thread(target=g, args=(1,))
b = threading.Thread(target=g, args=(2,))
a.start()
b.start()
a.join()
b.join()
print(n)  # 一定为0（不加锁可能为-1、-2、1、2）
```

* [threading.local](https://docs.python.org/3.9/library/threading.html#thread-local-data)

```py
import threading

x = threading.local()  # 每个线程独有一份拷贝


def f():
    print(x.i, threading.current_thread().name)


def g(n):
    x.i = n
    f()


a = threading.Thread(target=g, args=(1,), name='A')
b = threading.Thread(target=g, args=(2,), name='B')
a.start()
b.start()
a.join()
b.join()


# 输出如下
1 A
2 B
```

* Python 的线程在执行前必须获取 GIL 锁（Global Interpreter Lock），每执行 100 条字节码就释放 GIL 锁给其他线程，因此即使是多核 CPU，也只能用 1 个核运行 Python 的多线程

```py
# 开与 CPU 数量相同的死循环线程，但只能跑满单个 CPU
import multiprocessing
import threading


def f():
    x = 0
    while True:
        x = x ^ 1


for i in range(multiprocessing.cpu_count()):
    t = threading.Thread(target=f)
    t.start()
```

## 线程池

```py
from concurrent.futures import ThreadPoolExecutor
from random import random
import time


class Worker:
    def __init__(self, id: int):
        self.id = id

    def work(self, sec: float) -> float:
        print(f{self.id}={sec}')
        time.sleep(sec)
        return sec

class Processor:
    def __init__(self):
        self.workers = [Worker(_) for _ in range(10)]

    def work(self) -> float:
        res = 0
        with ThreadPoolExecutor(max_workers=len(self.workers)) as executor:
            fs = executor.map(Worker.work, self.workers, [random() for _ in self.workers])
            res = sum(fs)
        return res


if __name__ == '__main__':
    processor = Processor()
    print(processor.work())
```

## 进程池

```py
from concurrent.futures import ProcessPoolExecutor, Future
import concurrent.futures
from random import random
import time

class Worker:
    def __init__(self, id: int):
        self.id = id

    def work(self, sec: float) -> bool:
        time.sleep(sec)
        print(self.id)
        return self.id < 10

class Processor:
    def __init__(self):
        self.workers = [Worker(_) for _ in range(10)]

    def work(self) -> bool:
        res = False
        with ProcessPoolExecutor(max_workers=len(self.workers)) as executor:
            fs: list[Future[bool]] = [executor.submit(_.work, random()) for _ in self.workers]
            res = all(_.result() for _ in concurrent.futures.as_completed(fs))
        return res


if __name__ == '__main__':
    processor = Processor()
    assert(processor.work())
```

## 包

* 导入当前目录内的其他包

```py
# 当前文件所在目录，有一个名为 package_a 的同级目录
# package_a/base.py 中定义了 A
# package_a/__init__.py 内容为 from package_a.base import A
import sys
sys.path.append(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))  # 添加当前文件所在目录的父目录到环境变量
from package_a import A  # noqa: E402
```

## socket

* 获取一个可用的端口号

```py
import socket


def get_free_port() -> int:
    sock: socket.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind(('localhost', 0))
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    port: int = sock.getsockname()[1]
    sock.close()
    return port


print(get_free_port())
```

## tempfile

```py
import os
import tempfile
import datetime
import shutil


tmp_dir: str = tempfile.mkdtemp()
shutil.rmtree(tmp_dir)
tmp_file: str = tempfile.mkstemp(suffix=f"_{datetime.date.today().strftime('%Y%m%d')}.log", prefix='jc_')[1]
os.remove(tmp_file)
```

## 调用 so

* 生成 so 文件

```cpp
// sample.cpp
// g++ sample.cpp -fpic --shared -o ~/test.so
#include <string>
#include <iostream>


namespace jc {
// C 方式导出生成同名函数符号，nm ~/test.so | grep print_data
extern "C" const char* print_data(const char* info, char* data, int len) {
  if (len == 0) {
    return nullptr;
  }
  static std::string res = "";
  std::cout << info << std::endl;
  res.clear();
  if (data) {
    res = std::string{data, data + len};
  }
  return res.c_str();
}

}  // namespace jc
```

* 导入 so 并调用符号

```py
from ctypes import *
import ctypes

api = ctypes.CDLL('~/test.so')
# 定义返回类型和参数类型，如果符号不存在就会报错 undefined symbol
api.print_data.restype = c_char_p
api.print_data.argtypes = (c_char_p, c_char_p, c_int)
# 用 bytes 作为 char* 调用
msg: str = '123\x000456789'  # \x00 表示 \0
print(msg)  # 123456789
print(len(msg))  # 10
data: bytes = msg.encode('utf-8')
print(data)  # b'123\x00456789'
print(len(data))  # 10
res: bytes = api.print_data(b'hello', data, len(data))
print(res)  # b'123'
print(res.decode('utf-8))  # 123
if api.print_data(b'', b'', 0) is None:
    print('so return nullptr')
```

## 生成类型

```py
import sys

class A:
    pass

a: dict[int, type] = {
    1: A,
    2: int,
}
assert(__name__ == '__main__')
assert(a[1] == getattr(sys.modules[__name__], 'A'))
assert(a[1] == A)
assert(a[1] == eval('A'))
assert(a[2] == eval('int')
```
