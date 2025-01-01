## 流程控制

* if-then-else

```lua
local a, b = 1, 2
if a > b then
    print(1)
elseif a == b then
    print(0)
else
    print(-1)
end
```

* while

```lua
local i = 1
while i < 5 do
    print(i) -- 1 2 3 4
    i = i + 1
end
```

* repeat-until

```lua
local i = 1
repeat
    print(i) -- 1 2 3 4
    i = i + 1
until i == 5
```

* for

```lua
for i = 1, 5 do print(i) end -- 1 2 3 4 5
for i = 1, 10, 2 do print(i) end -- 1 3 5 7 9
for i = 1, math.huge do
    if i ^ 2 >= 100 then
        print(i) -- 10
        break
    end
end
```

* goto

```lua
local i = 1

::s1::
do
    print(i) -- 1 2 3 4 5
    if i < 5 then
        i = i + 1
        goto s1
    else
        goto s2
    end
end

print("unreachable code")

::s2::
```

## 变量类型

* 变量分为全局变量和局部变量，全局变量未赋值也可使用，默认值为 nil

```lua
print(a) -- nil
local b -- 声明局部变量 b，默认值为 nil
print(b) -- nil
```

* 局部变量的作用域为声明开始处至所在语句块末尾

```lua
x = 3
local i = 1

while i <= x do
    local x = i * 2
    print(x) -- 2 4 6
    i = i + 1
end

print(x) -- 3

local a, b = 1, 2
if a < b then
    print(a) -- 1
    local a
    print(a) -- nil
end

print(a, b) -- 1       2
```

* 一个惯用法是将全局变量赋值给同名的局部变量

```lua
local a = a
```

* 八种基本数据类型：nil、boolean、number、string、table、function、userdata、thread

```lua
type(nil) -- nil
type(true) -- boolean
type(10) -- number
type("hello") -- string
type({}) -- table
type(print) -- function
type(io.stdin) -- userdata
type(coroutine.create(function() print("hi") end)) -- thread
```

### nil

* 变量没有预定义类型，可以包含任意类型的值。nil 类型只有值 nil，比如全局变量在第一次赋值前默认为 nil，将全局变量赋值为 nil 相当于删除它

```lua
type(a) -- nil
a = 10
type(a) -- number
a = "hello"
type(a) -- string
a = nil
type(a) -- nil
```

### boolean

* boolean 类型有两个值：true 和 false。但是 boolean 不是唯一可以作为条件的值，在条件判断中，false、nil 被视为 false，其他值被视为 true。逻辑运算符有 and、or、not，对于 and，如果第一个操作数视为 false，则返回第一个操作数，否则返回第二个操作数

```lua
nil and 3 -- nil
false and 3 -- false
2 and 3 -- 3
0 and 3 -- 3
"" and 3 -- 3
nil or 3 -- 3
nil or false -- false
0 or 3 -- 0
not nil -- true
not false -- true
not 0 -- false
not not nil -- false
not not 0 -- true
```

### number

```lua
3.14 -- 3.14
0.1e3 -- 100.0
1e14 -- 1e+14
4.57e-3 -- 0.00457
0xff -- 255
0xffff -- 65535
0x0.1 -- 0.0625
0x1p-1 -- 0.5
0x1p-2 -- 0.25
0x2p-2 -- 0.5
math.type(3) -- integer
math.type(3.14) -- float
math.type(1e3) -- float
1 + 2 -- 3
1 + 3.14 -- 4.14
3.14 + 1 -- 4.14
3 / 2 -- 1.5
3 // 2 -- 1
3.0 / 2.0 -- 1.5
3.0 // 2 -- 1.0
-3 / 2 -- -1.5
-3 // 2 -- -2
-10 % 3 -- 2
10 % -3 -- -2
2 ^ 10 -- 1024.0
```

* 可以对小数求余

```lua
a = math.pi
a - a % 1 -- 3.0
a - a % 0.1 -- 3.1
a - a % 0.01 -- 3.14
a = -math.pi
a - a % 0.1 -- -3.2
a - a % 0.01 -- -3.15
```

* math 库

```lua
math.huge -- inf
math.sin(math.pi / 2) -- 1.0
math.max(3.14, -3, 42) -- 42
math.floor(3.14) -- 3
math.floor(-3.14) -- -4
math.ceil(3.14) -- 4
math.ceil(-3.14) -- -3
math.floor(3.4 + 0.5) -- 3
math.floor(2.6 + 0.5) -- 3
math.modf(3.14) -- 3       0.14
math.modf(-3.14) -- -3      -0.14
math.random(1, 2) -- 1
math.random(1, 2) -- 2
math.maxinteger + 1 == math.mininteger -- true
math.mininteger - 1 == math.maxinteger -- true
-math.mininteger == math.mininteger -- true
math.mininteger // -1 == math.mininteger -- true
math.maxinteger -- 2 ^ 63 - 1 = 9223372036854775807
0x7fffffffffffffff -- 9223372036854775807
math.mininteger -- -2 ^ 63 = -9223372036854775808
0x8000000000000000 -- -9223372036854775808
math.maxinteger + 2 -- -9223372036854775807
math.maxinteger + 2.0 -- 9.2233720368548e+18
math.maxinteger + 2.0 == math.maxinteger + 1.0 -- true
1 + 0.0 -- 1.0，integer 转 float
3.0 | 0 -- 3，float 转 integer
-3.0 | 0 -- -3
3.2 | 0 -- 错误，要转换的浮点数必须能表示为整数
math.tointeger(-3.0) -- -3
math.tointeger(-3.14) -- nil
math.tointeger(2 ^ 64) -- nil，超出范围
```

### string

* string 用单引号或双引号均可，若使用双引号则内部的单引号无需转义，若使用单引号则内部的双引号无需转义

```lua
print("'''") -- '''
print("\"'") -- "'
print('"""') -- """
print('"\'') -- "'
```

* 长字符串

```lua
a = [[
<html>
<body>
  <p> Hello World</p>
</body>
</html>]]

-- 若字符串中嵌套 ]]
-- 则在最外层的 [[ 和 ]] 中插入任意等量的等于号来区分 
a = [=[
<html>
<body>
  <p> [[Hello World]]</p>
</body>
</html>]=]
```

* string 是不可变的，不能修改 string 中的某个字符

```lua
a = "one string"
b = string.gsub(a, "one", "another")
print(a) -- one string
print(b) -- another string
```

* 连接字符串

```lua
"hello" .. "world" .. 1 -- helloworld1
10 .. 20 -- 1020
```

* 对于加号运算符，string 会被转为 number

```lua
"10" + 1 -- 11
```

* 用 tonumber 将 string 转 number，若无法转换则返回 nil，可以用第二个参数指定字符串表示的进制（可以为 2 到 36），不指定则为十进制

```lua
tonumber("  -3  ") -- -3
tonumber(" 10e4 ") -- 100000.0
tonumber("10e") -- nil
tonumber("0x1.3p-4") -- 0.07421875
tonumber("1111", 2) -- 15
tonumber("2345", 2) -- nil
tonumber("ff", 16) -- 256
tonumber("-ZZ", 36) -- -1295
```

* 用 tostring 将 number 转 string

```lua
tostring(10) == "10" -- true
```

* string 库

```lua
string.len("hello") -- 5，返回字节数
#"hello" -- 5，等价于 string.len()
string.rep("abc", 3) -- abcabcabc
string.reverse("abc") -- cba
string.lower("Hello World") -- hello world
string.upper("Hello World") -- HELLO WORLD
string.sub("abcde", 1, 1) -- a
string.sub("abcde", 1, 3) -- abc
string.sub("abcde", 2, -1) -- bcde
string.char(97) -- a
string.char(97, 98, 99) -- abc
string.byte("abc") -- 97
string.byte("abc", 2) -- 98
string.byte("abc", -1) -- 99
string.byte("abc", 2, -1) -- 98      99
string.format("a = %d, b = %d", 1, 2) -- a = 1, b = 2
string.format("%x", 255) -- ff
string.format("%X", 255) -- FF
string.format("%f", 255) -- 255.000000
string.format("%s", "hello") -- hello
string.format("%.4f", math.pi) -- 3.1416
string.format("%04d-%02d-%02d", 2008, 8, 8) -- 2008-08-08
string.find("hello world", "llo") -- 3       5
string.find("hello world", "abc") -- nil
string.gsub("abcabc", "b", "B") -- aBcaBc  2
string.gsub("abcabc", "bc", "") -- aa      2
```

* utf8 库

```lua
#"中文" -- 6
utf8.len("中文") -- 2，返回 uft8 字符数
utf8.len("ab\x93") -- nil     3，发现无效字节序列则返回其第一个字节的位置
utf8.char(114, 233, 115, 117, 109, 233) --> résumé
utf8.codepoint("résumé", 3, 3) -- 错误，无效的 UTF-8 code
utf8.codepoint("résumé", 6, 7) -- 109     233
utf8.codepoint("résumé", utf8.offset("résumé", 3)) -- 115
string.sub("résumé", utf8.offset("résumé", -2)) -- mé
```

### table

* table 是引用语义

```lua
a = {}
a["x"] -- nil
a["x"] = 1
a["x"] -- 1
b = a
{} == {} -- false
a == b -- true
a.x = 2 -- a.x 等价于 a["x"]
a["x"] -- 2
b["x"] -- 2
a = nil -- b 仍指向 table
b.x -- 2
b.x = nil -- 删除 b["x"]
b = nil -- 删除 table
```

* integer 和 float 作为 table 的 index 时，若值相同则表示同一个 index

```lua
a = {}
a[2.0] = 1
a[2] -- 1
a[2] = 2
a[2.0] -- 2
```

* 初始化时构造 table

```lua
a = {"one", "two", "three"}
a[1] -- one
a[2] -- two
a[3] -- three

b = {x = 1, ["y"] = 2}
b.x -- 1
b.y -- 2

c = {x = 3.14, y = 42, "one", "two", "three", {x = 1, y = 2}}
c.x -- 3.14
c.y -- 42
c[1] -- one
c[2] -- two
c[3] -- three
c[4].x -- 1
c[4].y -- 2

i = 10
d = {[i + 0] = "a", [i + 1] = "b"}
d[10] -- a
d[11] -- b
```

* 当 table 是 list 时，用 `#` 操作符获取 list 长度

```lua
a = {"one", "two", "three"}
#a -- 3
a[1] -- one
a[2] -- two
a[3] -- three
a[#a + 1] = "four"
a[4] -- four
```

* 遍历 table

```lua
t = {10, print, x = 1, y = "hi"}
for k, v in pairs(t) do print(k, v) end
--[[
1       10
2       function: 0x5601bff679e0
x       1
y       hi
--]]
```

* 当 table 是 list 时

```lua
t = {10, print, 1, "hi"}
for k, v in ipairs(t) do print(k, v) end
--[[
1       10
2       function: 0x5601bff679e0
3       1
4       hi
--]]

for k = 1, #t do print(k, t[k]) end
--[[
1       10
2       function: 0x5601bff679e0
3       1
4       hi
--]]
```

* 获取多层 table 嵌套的某个 key

```lua
E = {}
(((a or E).b or E).c or E).d -- nil
a = {}
a.b = {}
a.b.c = {}
a.b.c.d = 1
(((a or E).b or E).c or E).d -- 1
```

* table 库

```lua
t = {10, 20, 30}
table.concat(t) -- 102030
table.concat(t, ',') -- 10,20,30
table.concat(t, ',', 2, 3) -- 20,30
table.insert(t, 1, 40)
for k, v in ipairs(t) do print(k, v) end
--[[
1       40
2       10
3       20
4       30
--]]
table.sort(t)
for k, v in ipairs(t) do print(k, v) end
--[[
1       10
2       20
3       30
4       40
--]]
table.remove(t, #t) -- 40
for k, v in ipairs(t) do print(k, v) end
--[[
1       10
2       20
3       30
--]]
table.move(t, 1, #t, 2) -- {10, 10, 20, 30}
t[1] = 40 -- {40, 10, 20, 30}
for k, v in ipairs(t) do print(k, v) end
--[[
1       40
2       10
3       20
4       30
--]]
```

### function

* 如果函数只有一个参数且参数是字符串字面值或 table 构造器，则可以省略括号

```lua
print "hello" -- 等价于 print("hello")
type {} -- 等价于 tpye({})
f{x = 10, y = 20} -- 等价于 f({x = 10, y = 20})
```

* 函数声明

```lua
function add(a)
    local res = 0
    for i = 1, #a do res = res + a[i] end
    return res
end

print(add {1, 2, 3}) -- 6
```

* 调用函数时，缺少的参数会被视为 nil，多出的参数会被忽略

```lua
function f(a, b) print(a, b) end

f() -- nil nil
f(1) -- 1 nil
f(1, 2) -- 1 2
f(1, 2, 3) -- 1 2
```

* 利用缺少的参数被视为 nil 的特性，可以实现默认实参的机制

```lua
function f(a)
    a = a or 1 -- a 的默认实参为 1
    print(a)
end

f() -- 1
```

* 函数可以有多个返回值

```lua
function maximum(a)
    local index = 1
    local value = a[index]
    for i = 1, #a do
        if a[i] > value then
            index = i
            value = a[i]
        end
    end
    return value, index
end

print(maximum({11, 33, 55, 22, 44})) -- 55       3
```

* 多返回值函数赋值给多个变量时，缺少的返回值被视为 nil，多余的返回值会被丢弃

```lua
function f() return 1, 2 end

a = f() -- a = 1
a, b = f() -- a = 1, b = 2
a, b, c = f() -- a = 1, b = 2, c = nil
a, b, c = f(), 3 -- a = 1, b = 2, c = 3
```

* 多返回值函数出现在表达式中时，只会使用第一个返回值

```lua
function f() return "a", "b" end

print(f()) -- a       b
print(f(), "c") -- a       c
print(f() .. "c") -- ac
t = {f()} -- t = {"a", "b"}
t = {f(), "c"} -- t = {"a", "c"}
```

* 多返回值函数调用结果作为返回值时，返回值与该函数相同

```lua
function f() return "a", "b" end
function g() return f() end
function h() return (f()) end -- 加括号变成了表达式，只返回第一个值

print(g()) -- a       b
print(h()) -- a
```

* 可变参数函数

```lua
function add(...)
    local res = 0
    for _, v in ipairs {...} do res = res + v end
    return res
end

print(add(1, 2, 3)) -- 6
```

* 可变参数转 table

```lua
function f(...)
    local t = table.pack(...)
    for k, v in ipairs(t) do print(k, v) end
end

print(f("a", "b", "c"))
--[[
1       a
2       b
3       c
--]]
```

* table 转可变参数

```lua
print(table.unpack({1, 2, 3})) -- 1       2       3
a, b = table.unpack({1, 2, 3}) -- a = 1, b = 2
print(table.unpack({"a", "b", "c"}, 2, 3)) -- b       c
```

* 用 select 函数返回指定位置开始的可变参数

```lua
print(select(1, "a", "b", "c")) -- a       b       c
print(select(2, "a", "b", "c")) -- b       c
print(select(3, "a", "b", "c")) -- c
print(select(-2, "a", "b", "c")) -- b       c
print(select(-1, "a", "b", "c")) -- c
print(select("#", "a", "b", "c")) -- 3

function add(...)
    local res = 0
    for i = 1, select("#", ...) do res = res + select(i, ...) end
    return res
end

print(add(1, 2, 3)) -- 6
```
