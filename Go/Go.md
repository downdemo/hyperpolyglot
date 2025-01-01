## 赋值语句

* 变量声明

```go
package main

import (
	"fmt"
)

func main() {
	a := 1
	var b = 1
	const c = 1
	var d int
	var e int = 1
	fmt.Println(a, b, c, d, e) // 1 1 1 0 1
}
```

* 冒号赋值只能在函数内部使用

```go
package main

import (
	"fmt"
)

x := 1    // 错误
var y = 1 // OK

func main() {
	fmt.Println(x, y)
}
```

* 一条语句中可以声明多个变量

```go
a, b, c := 1, 2, 3
var a, b, c int
var a, b, c = true, 1, "hi"
```

* 可以直接利用这种方式交换变量

```go
a, b := 1, 2
a, b = b, a
fmt.Println(a, b)  // 2 1
```

* 批量声明编译期常量

```go
const (
	e  = 2.71828182845904523536028747135266249775724709369995957496696763
	pi = 3.14159265358979323846264338327950288419716939937510582097494459
)
```

* 如果批量声明编译期常量未设置值，则使用前一个变量的值

```go
const (
	a = 2 // 第一个变量必须设置值
	b
	c = 1
	d
)

fmt.Println(a, b, c, d) // 2 2 1 1
```

* 可以用 iota 来生成递增值

```go
const (
	a = iota
	b
	c
	d
)

const (
	a2 = 1 << iota
	b2
	c2
	d2
)

fmt.Println(a, b, c, d)     // 1 2 3 4
fmt.Println(a2, b2, c2, d2) // 1 2 4 8
```

* 要忽略的值用下划线来表示

```go
const (
	_   = 1 << (10 * iota)
	KiB // 1024
	MiB // 1048576
	GiB // 1073741824
	TiB // 1099511627776 (exceeds 1 << 32)
	PiB // 1125899906842624
	EiB // 1152921504606846976
	ZiB // 1180591620717411303424 (exceeds 1 << 64)
	YiB // 1208925819614629174706176
)

fmt.Println(GiB / MiB) // 1024
```

## 流程控制

* if-else

```go
func main() {
	x := 42
	if x > 0 {
		fmt.Println(1)
	} else if x == 0 {
		fmt.Println(0)
	} else {
		fmt.Println(-1)
	}
}
```

* for

```go
func main() {
	for i := 0; i < 10; i++ { // go 没有前自增操作符，不支持 ++i
		fmt.Println(i)
	}
}
```

* Go 没有 while 语句，用 for 替代

```go
func main() {
	i := 0
	for i < 10 {
		fmt.Println(i)
		i++
	}
}
```

* switch-case

```go
func main() {
	x := 1
	switch x {
	case 1:
		fmt.Println(1)
	case 0:
		fmt.Println(0)
	default:
		fmt.Println(-1)
	}
}
```

* switch-case 可以不指定 switch 的初始条件

```go
func main() {
	x := 1
	switch {
	case x > 0:
		fmt.Println(1)
	case x == 0:
		fmt.Println(0)
	default:
		fmt.Println(-1)
	}
}
```

* switch-case 中默认每个 case 都带 break，如果要执行下一个 case 则要加上 fallthrough

```go
func main() {
	x := 1
	switch {
	case x > 0:
		fmt.Println(1)
		fallthrough
	case x == 0:
		fmt.Println(0)
		fallthrough
	default:
		fmt.Println(-1)
	}
}
```

## 基本类型

* 整型有 int（32 位 或 64 位，平台相关，与 int32、int64 均为不同类型）、int8、int16、int32、int64、uint（32 位 或 64 位，平台相关，与 uint32、uint64 均为不同类型）、uint8、uint16、uint32、uint64、byte（uint8 的别名）、rune（int32 的别名）

```go
var x byte = 1<<3 | 1<<5
y := 'a'                  // y 类型为 rune
fmt.Printf("%08b", x)     // 00101000
fmt.Printf("%T %T", x, y) // uin8 int32
```

* Printf 的部分 Verb 如下

```go
%d 十进制整数
%x 十六进制整数
%o 八进制整数
%b 二进制整数
%f, %g, %e 浮点数 3.141593 3.141592653589793 3.141593e+00
%t 布尔值
%c rune（Unicode 码点）
%s 字符串
%q 带双引号的字符串 "abc" 或带单引号的字符 'c'
%v natural format
%T 变量的类型
%% 字面上的百分号标志（无操作数）
```

* 浮点型有 float32、float64

```go
var x float32 = 3.14
var y int32 = int32(x) // 必须显式转换类型
fmt.Println(y) // 3
```

* 复数类型有 complex64、complex128

```go
var x complex128 = complex(2, 1)
fmt.Println(x)       // (2+1i)
fmt.Println(real(x)) // 2
fmt.Println(imag(x)) // 1
var y complex128 = complex(2, -1)
fmt.Println(x * y) // (5+0i)
```

* string

```go
var s string = "abcde"
fmt.Println(len(s)) // 5
fmt.Println(s[1:3]) // bc
fmt.Println(s[1:])  // bcde
fmt.Println(s[:3])  // abc
fmt.Println(s[:])   // abcde
```

* 用 [strconv](https://go-zh.org/pkg/strconv/) 包进行 string 与其他类型的转换

```go
var s string = strconv.Itoa(7)                      // int => string, 7
s = strconv.FormatInt(int64(7), 2)                  // int64 => string, 111
s = strconv.FormatUint(uint64(7), 10)               // uint64 => string, 7
s = strconv.FormatFloat(float64(3.14), 'f', -1, 64) // float64 => string
a, _ := strconv.Atoi(s)                             // string => int
b, _ := strconv.ParseInt(s, 10, 64)                 // string => in64
c, _ := strconv.ParseUint(s, 10, 64)                // string => uint64
d, _ := strconv.ParseFloat(s, 64)                   // string => float64
```

* string 底层的字符类型是 byte 或 rune

```go
var s string = "中文"
for i := 0; i < len(s); i++ { // s[i] 类型为 byte
	fmt.Printf("%v(%c) ", s[i], s[i]) // 228(ä) 184(¸) 173(­) 230(æ) 150() 135()
}
for _, r := range s { // r 类型为 rune
	fmt.Printf("%v(%c) ", r, r) // 20013(中) 25991(文)
}
```

* string 是不可改变的，必须先转为 []byte 或 []rune（对于 UTF-8 字符），再转为 string，注意这会导致重新分配内存和拷贝数据

```go
var s string = "英文"
r := []rune(s)
r[0] = '中'
s = string(r)
```

## 复合类型

### 数组（Array）

```go
var a [3]int = [3]int{1, 2, 3}
var b [4]int = [4]int{1, 2, 3} // 1 2 3 0
var c [3]int = [...]int{1, 2, 3}
var d [3]int = [...]int{0: 1, 2: 3} // 1 0 3
```

* 多维数组

```go
a := [2][3]int{{1, 2, 3}, {1, 2, 3}}
b := [...][3]int{{1, 2, 3}, {1, 2, 3}}
```

* 遍历数组

```go
a := [3]int{100, 200, 300}
for i, v := range a {
	fmt.Println(i, v)
}
```

### 切片（Slice）

* 用数组创建 slice

```go
var a [3]int = [...]int{1, 2, 3}
var b []int = a[1:2]
```

* 用 make 直接创建 slice

```go
var a []int = make([]int, 2, 3)
fmt.Println(len(a), cap(a)) // 2 3
fmt.Println(a)              // [0 0]
a[0], a[1] = 1, 2
fmt.Println(a) // [1 2]
```

* slice 在 cap 不足时会进行扩容

```go
a := make([]int, 0, 1)
a = append(a, 1, 2)
fmt.Println(cap(a)) // 2
a = append(a, 3, 4, 5)
fmt.Println(cap(a)) // 6
a = append(a, 6, 7, 8)
fmt.Println(cap(a)) // 12
```

* slice 是引用类型

```go
a := [...]int{0, 1, 2}
b := a[:]
b[0] = 1
fmt.Println(a) // [1 1 2]
```

* 用 append 深拷贝 slice

```go
a := []int{1, 2}
b := []int{3, 4}
c := append(a, b...)
fmt.Println(c) // 1 2 3 4
c[0] = 2
fmt.Println(a) // 1 2
```

* 用 copy 深拷贝 slice

```go
from := []int{1, 2, 3}
to := make([]int, 2, 3)
copy(to, from)
fmt.Println(to) // [1 2]

a := []int{0, 1, 2, 3}
copy(a[1:], a[:])
fmt.Println(a) // [0 0 1 2]
```

* int 排序

```go
a := []int{3, 1, 2}
sort.Ints(a)
fmt.Println(a) // [1 2 3]
```

* float64 排序

```go
a := []float64{3.3, 1.1, 2.2}
sort.Float64s(a)
fmt.Println(a) // [1.1 2.2 3.3]
```

* string 排序

```go
a := []string{"c", "a", "b"}
sort.Strings(a)
fmt.Println(a) // [a b c]
```

* 通用的类型排序

```go
a := []uint32{3, 1, 2}
sort.Slice(a, func(i, j int) bool { return a[i] < a[j] })
fmt.Println(a) // [1 2 3]
```

### 指针

```go
x := 1
var p *int = &x
*p = 2
fmt.Println(x) // 2

a := [...]int{1, 2, 3}
var q *[3]int = &a
fmt.Println(*q) // [1 2 3]
fmt.Println(q[0], q[1], q[2]) // 1 2 3
```

* 空指针为 nil

```go
var p *int
if p == nil {
	fmt.Println(1)
}
```

* 可以用 new 函数生成指针类型变量

```go
p := new(int)
if *p == 0 {
	fmt.Println(1)
}
```

### Map

```go
var m map[string]int = map[string]int{
	"a": 1,
	"b": 2,
}
fmt.Println(m["a"], m["b"]) // 1 2
delete(m, "a")
v, ok := m["a"]
if !ok {
	fmt.Println(v) // 0
}
m["c"] = 3
fmt.Println(m) // map[b:2 c:3]
```

* 用 make 创建 map

```go
m := make(map[string]int)
fmt.Println(m["a"]) // 0
fmt.Println(m)      // map[]
m["a"]++
fmt.Println(m) // map[a:1]
```

* map 中的元素不是变量，不能对其取址，因为 map 扩容会重新分配内存空间，导致原来的地址失效

```go
m := make(map[string]int)
m["a"] = 1
p := &m["a"] // 错误
```

* map 类型的零值是 nil

```go
var m1 map[string]int
m2 := make(map[string]int)
fmt.Println(m1 == nil) // true
fmt.Println(m2 == nil) // false
```

* 注意零值的 map 不能直接使用

```go
var m map[string]int
m["a"] = 1 // 错误
```

* 遍历 map

```go
m := make(map[string]int)
m["a"] = 1
m["b"] = 2
for k, v := range m {
	fmt.Println(k, v)
}
for k := range m {
	fmt.Println(k)
}
```

* map 是乱序的，每次遍历的结果可能相同也可能不同，如果希望有序遍历，要显式对 key 排序

```go
package main

import (
	"fmt"
	"sort"
)

func main() {
	m := make(map[string]int)
	m["a"] = 1
	m["b"] = 2
	var a []string
	for k := range m {
		a = append(a, k)
	}
	sort.Strings(a)
	for _, x := range a {
		fmt.Println(m[x])
	}
}
```

### struct

* 自定义类型和类型别名

```go
package main

import (
	"fmt"
)

type Number1 float64   // 自定义类型
type Number2 = float64 // 类型别名

func main() {
	var a Number1 = 1.0
	var b Number2 = 1.0
	fmt.Printf("%T %T", a, b) // main.Number1 float64
}
```

* struct

```go
type Person struct {
	Age           uint8
	Name, Address string
}

var x Person
x.Age = 12
x.Name = "ABC"
fmt.Println(x.Age, x.Name) // 12 ABC
```

* 匿名 struct

```go
var x struct {
	Age  uint8
	Name string
}
x.Age = 12
x.Name = "ABC"
fmt.Println(x.Age, x.Name) // 12 ABC
```

* 用 new 创建指针类型 struct

```go
type Person struct {
	Age  uint8
	Name string
}

var x *Person = new(Person)
x.Age = 12
x.Name = "ABC"
fmt.Println(x.Age, x.Name) // 12 ABC
```

* 可以直接在初始化时按顺序设置成员值，使用这种方式必须指定所有成员

```go
type Person struct {
	Age           uint8
	Name, Address string
}

x := Person{12, "ABC", ""}
fmt.Println(x.Age, x.Name) // 12 ABC
```

* 指针类型 struct 也可以使用这种方式

```go
type Person struct {
	Age  uint8
	Name string
}

x := &Person{12, "ABC"}
fmt.Println(x.Age, x.Name) // 12 ABC
```

* 注意如果使用分行的格式，每行值的后面都要加上逗号

```go
type Person struct {
	Age  uint8
	Name string
}

x := &Person{
	12,
	"ABC",
}
fmt.Println(x.Age, x.Name) // 12 ABC
```

* 如果不想设置所有成员，可以通过指定 key 的初始化方式仅设置指定成员，未设置的成员为零值

```go
type Person struct {
	Age  uint8
	Name string
}

x := &Person{
	Name: "ABC",
}
fmt.Println(x.Age, x.Name) // 0 ABC
```

* 在 struct 中嵌入匿名字段，可以从外层直接访问匿名字段的成员

```go
type A struct {
	a int
}

type B struct {
	A
	b int
}

type C struct {
	B
	c int
}

var x C
x.a, x.b, x.c = 1, 2, 3
fmt.Println(x.a, x.b, x.c)       // 1 2 3
fmt.Println(x.B.A.a, x.B.b, x.c) // 1 2 3
```

* 如果有同名成员，必须显式指定其所属的匿名字段

```go
type A struct {
	i int
}

type B struct {
	A
	i int
}

type C struct {
	B
	i int
}

var x C
x.A.i, x.B.i, x.i = 1, 2, 3
fmt.Println(x.A.i, x.B.i, x.i) // 1 2 3
```

* 对于匿名字段，如果用值初始化必须嵌套指定

```go
type A struct {
	a int
}

type B struct {
	A
	b int
}

type C struct {
	B
	c int
}

x := C{B{A{1}, 2}, 3}
fmt.Println(x.a, x.b, x.c)       // 1 2 3
fmt.Println(x.B.A.a, x.B.b, x.c) // 1 2 3
```

* 如果用 key 初始化也必须嵌套指定

```go
type A struct {
	a int
	x int
}

type B struct {
	A
	b int
}

type C struct {
	B
	c int
}

x := C{
	B: B{
		A: A{
			a: 1,
		},
		b: 2,
	},
	c: 3,
}
fmt.Println(x.a, x.b, x.c)       // 1 2 3
fmt.Println(x.B.A.a, x.B.b, x.c) // 1 2 3
```

* 匿名字段也可以是指针类型

```go
type A struct {
	a int
}

type B struct {
	*A
	b int
}

type C struct {
	*B
	c int
}

x := C{&B{&A{1}, 2}, 3}
fmt.Println(x.a, x.b, x.c) // 1 2 3
```

* map 中的元素不是变量，不能对其取址，因此如果 map 的 value 是 struct，不能直接修改 struct 的成员

```go
type A struct {
	i int
}

m := map[int]A{0: A{1}}
fmt.Println(m[0].i) // 1
m[0].i = 2          // 错误
```

* 只能把一个新的 value 赋值给 map

```go
type A struct {
	i int
}

m := map[int]A{0: A{1}}
fmt.Println(m[0].i) // 1
t := m[0]
t.i = 2
m[0] = t
fmt.Println(m[0].i) // 2
```

* 如果 map 的 value 是 struct 的指针则可以直接修改成员

```go
type A struct {
	i int
}

m := map[int]*A{0: &A{1}}
fmt.Println(m[0].i) // 1
m[0].i = 2          // OK
fmt.Println(m[0].i) // 2
```

## json

```go
package main

import (
	"encoding/json"
	"fmt"
	"log"
)

func main() {
	type Person struct {
		Name string `json:"Id"`
		Age  uint8
	}

	a := []Person{
		{
			Name: "A",
		},
		{
			Name: "B",
			Age:  12,
		},
	}

	data, err := json.Marshal(a) // data 类型为 []byte
	if err != nil {
		log.Fatalf("JSON marshaling failed: %s", err)
	}
	fmt.Printf("%s\n", data) // [{"Id":"A","Age":0},{"Id":"B","Age":12}]

	var b []Person
	err = json.Unmarshal(data, &b)
	if err != nil {
		log.Fatalf("JSON unmarshaling failed: %s", err)
	}
	fmt.Println(b) // [{A 0} {B 12}]
}
```

* 注意小写字母开头的成员是未导出的，在序列化时会被忽略

```go
package main

import (
	"encoding/json"
	"fmt"
	"log"
)

func main() {
	type Person struct {
		name string `json:"Id"`
		age  uint8
	}

	a := []Person{
		{
			name: "A",
		},
		{
			name: "B",
			age:  12,
		},
	}

	data, err := json.Marshal(a) // s 类型为 []byte
	if err != nil {
		log.Fatalf("JSON marshaling failed: %s", err)
	}
	fmt.Printf("%s\n", data) // [{},{}]

	var b []Person
	err = json.Unmarshal(data, &b)
	if err != nil {
		log.Fatalf("JSON unmarshaling failed: %s", err)
	}
	fmt.Println(b) // [{ 0} { 0}]
}
```

* 仅修改特定 key，其他 key 保持原有值，解析为 `map[string]interface{}` 即可

```go
package main

import (
	"encoding/json"
	"fmt"
)

func main() {
	s := []byte(`{
		"a": 1,
		"b": {
			"c": 2,
			"d": 3
		}
	}`)
	m := &map[string]interface{}{}
	json.Unmarshal(s, m)
	b := (*m)["b"].(map[string]interface{})
	b["c"] = "hi"
	data, _ := json.Marshal(m)
	fmt.Println(string(data)) // {"a":1,"b":{"c":"hi","d":3}}
}
```

## func

* 传值

```go
func f(x int) {
	x = 1
}

x := 0
f(x)
fmt.Println(x) // 0
```

* 传指针

```go
func f(x *int) {
	*x = 1
}

x := 0
f(&x)
fmt.Println(x) // 1、
```

* 数组作为函数参数传递时是值语义

```go
func f(a [3]int) {
	a[0]++
}

func g(a *[3]int) {
	a[0]++
}

x := [3]int{1, 2, 3}
f(x)
fmt.Println(x) // 1 2 3
g(&x)
fmt.Println(x) // 2 2 3
```

* 如果希望通过函数修改数组，传递 slice 即可

```go
func f(a []int) {
	a[0]++
}

x := [3]int{1, 2, 3}
f(x[:])
fmt.Println(x) // 2 2 3
```

* 可变参数

```go
func f(ts ...int) int {
	var res int
	for _, v := range ts {
		res += v
	}
	return res
}

fmt.Println(f(1, 2, 3)) // 6
a := []int{1, 2, 3}
fmt.Println(f(a...)) // 6
```

* 函数可以有多个返回值

```go
func f(a, b, c int) (int, int, int) {
	return a, b, c
}

a, b, c := f(1, 2, 3)
fmt.Println(a, b, c) // 1 2 3
```

* 可以直接命名返回值

```go
func f() (a, b, c int) {
	a, b, c = 1, 2, 3
	return
}

a, b, c := f()
fmt.Println(a, b, c) // 1 2 3
```

* func 是基本类型，可以作为参数传递

```go
func f(x int) int {
	return x
}

var g func(int) int
if g == nil {
	g = f
}
fmt.Println(g(1)) // 1
```

* 匿名函数

```go
func f(x, y int, g func(int, int) int) int {
	return g(x, y)
}

fmt.Println(f(1, 2, func(x, y int) int { return x + y })) // 3
```

* 闭包

```go
func f(x int) func(int) int {
	return func(y int) int {
		return x + y
	}
}

fmt.Println(f(1)(2)) // 3
```

* defer 函数会在函数返回后执行

```go
func f() {
	defer func() {
		fmt.Print(1)
	}()
	fmt.Print(2)
}

f() // 21
```

* 如果有多个 defer，后定义的会先执行

```go
func f() {
	defer func() {
		fmt.Print(1)
	}()

	defer func() {
		fmt.Print(2)
	}()
}

f() // 21
```

* 用 panic 抛异常

```go
func f() {
	panic("hello")
}

f() // 报错 panic: hello
```

* 用 recover 捕获异常

```go
func f() {
	defer func() {
		if err := recover(); err != nil {
			fmt.Println(1)
		}
	}()
	panic("error!")
}

f() // 1
```

* 如果有多个 defer 函数，panic 没被 recover 则会一直向后传递，直到被 recover

```go
func f() {
	defer func() {
		recover()
		fmt.Println("ok")
	}()

	defer func() {
		panic("2")
	}()

	panic("1")
}

f() // ok
```

* recover 必须在 defer 函数中被直接调用

```go
func f() {
	defer func() {
		defer func() {
			recover()
			fmt.Println("ok")
		}()
	}()

	panic("1")
}

f() // panic: 1
```

## 方法

* 方法是与某个类型绑定的函数，这个类型的实例称为接收器（receiver），写在函数名前。方法不支持重载，用指针和非指针的调用方式相同

```go
type A struct {
	x, y int
}

func (a A) sum() int {
	return a.x + a.y
}

func (a *A) mul() int {
	return a.x * a.y
}

a := A{1, 2}
fmt.Println(a.sum())    // 3
fmt.Println((&a).sum()) // 3
fmt.Println(a.mul())    // 2
fmt.Println((&a).mul()) // 2
```

* 接收器实际上会被作为参数传递

```go
type A struct {
	i int
}

func (a A) f() {
	a.i = 1
}

func (a *A) g() {
	a.i = 1
}

x := A{0}
x.f()
fmt.Println(x.i) // 0
x.g()
fmt.Println(x.i) // 1
```

* 对于指针类型，传入的接收器必须能取址

```go
type A struct{}

func (a *A) f() {}

A{}.f() // 错误
```

* 方法和函数一样，可以被作为参数传递

```go
type A struct {
	i int
}

func (a *A) f(x int) {
	a.i = x
}

x := &A{0}
var fFromA func(int) = x.f
fFromA(1)
fmt.Println(x.i) // 1
```

* 任何非内置类型都可以声明方法，比如给自定义类型声明方法

```go
type Int int

func (a Int) f() {
	fmt.Println(1)
}

var x Int
x.f() // 1
```

* 嵌套成员的方法也可以直接访问

```go
type A struct {
	a int
}

type B struct {
	A
	b int
}

type C struct {
	B
	c int
}

func (a A) af() {
	fmt.Println(a.a)
}

func (b *B) bf() {
	fmt.Println(b.a)
}

x := C{B{A{1}, 2}, 3}
x.af() // 1
x.bf() // 1
```

* 调用嵌套成员的同名方法时，编译器会从外层向内层查找，调用第一个找到的同名方法

```go
type A struct {
	a int
}

type B struct {
	A
	b int
}

type C struct {
	B
	c int
}

func (a A) f() {
	fmt.Println(1)
}

func (b *B) f() {
	fmt.Println(2)
}

func (c *C) f() {
	fmt.Println(3)
}

x := C{B{A{1}, 2}, 3}
x.f()   // 3
x.B.f() // 2
x.A.f() // 1
```

## 接口

* 接口内规定了需要实现的函数，如果一个类型实现了接口中所有函数，这个类型就实现了此接口

```go
type X interface {
	f()
	g()
}

type A struct{}

func (a A) f() {
	fmt.Println(1)
}

func (a A) g() {
	fmt.Println(2)
}

var x X // x 为 nil
a := A{}
x = a
x.f() // 1
x.g() // 2
```

* 注意如果接收器是指针类型，对应的指针类型才算实现了接口

```go
type X interface {
	f()
}

type A struct{}

func (a *A) f() {}

var x X
var x X = &A{} // OK：*A 实现了接口 X
var x X = A{} // 错误：A 没有实现接口 X
```

* 接口可以组合

```go
type X interface {
	f() int
}

type Y interface {
	g() int
}

type Z interface {
	X
	Y
}

type A struct{}

func (a A) f() int {
	return 1
}

func (a A) g() int {
	return 2
}

a := A{}
var x Z = a
fmt.Println(x.f(), x.g()) // 1 2
```

* 用类型断言可以检查接口当前的动态类型

```go
type X interface {
	f()
}

type A struct{}

func (a *A) f() {
	fmt.Println(1)
}

type B struct{}

func (a *B) f() {
	fmt.Println(2)
}

var x X
x = &A{}
if a, ok := x.(*A); ok {
	a.f() // 1
}
x = &B{}
if b, ok := x.(*B); ok {
	b.f() // 2
}
```

## Goroutine

* goroutine 的栈空间开始生命周期时只需要 2 KB，栈大小随需要变化，最大可以有 1 GB，使用时只需要在调用函数前加 go 关键字，主协程退出后其他 goroutine 会直接退出

```go
package main

import (
	"fmt"
	"time"
)

func fib(x int) int {
	if x <= 1 {
		return x
	}
	return fib(x-1) + fib(x-2)
}

func main() {
	go func() {
		for {
			fmt.Print("-")
			var t time.Duration = 1000 * time.Millisecond
			time.Sleep(t)
		}
	}()
	fmt.Println(fib(45)) // 计算需要一段时间
} // main 函数结束后，goroutine 将停止
```

* goroutine 可能延后执行，但传入的值不会被改变

```go
package main

import (
	"fmt"
	"time"
)

func main() {
	x := 1
	go func(x int) {
		time.Sleep(100 * time.Millisecond)
		fmt.Println(x) // 仍然为 1
	}(x)
	x = 2
	fmt.Println(x) // 2
	time.Sleep(200 * time.Millisecond) // 等待足够的时间让 goroutine 执行完成
}
```

## Channel

* 通过 channel 可以在 goroutine 之间同步数据，chan 是引用类型

```go
package main

import (
	"fmt"
	"time"
)

func main() {
	var ch chan int = make(chan int)
	go func() {
		time.Sleep(1000 * time.Millisecond)
		ch <- 1
	}()
	var x int = <-ch // 阻塞至 goroutine 给 ch 发送值 1
	fmt.Println(x)   // 一定会在 goroutine 完成后执行
}
```

* 串联多个 goroutine

```go
package main

import (
	"fmt"
	"time"
)

func main() {
	a := make(chan int)
	b := make(chan int)

	go func() {
		for i := 0; ; i++ {
			a <- i // 把 0 1 2 3 ... 依次传入 a
		}
	}()

	go func() {
		for {
			x := <-a   // 把 a 中的数读出来
			b <- x * x // 再把其平方传给 b
		}
	}()

	for {
		time.Sleep(1000 * time.Millisecond)
		fmt.Println(<-b) // 每秒从 b 中取一个数
	}
}
```

* 提前关闭 channel

```go
package main

import (
	"fmt"
	"time"
)

func main() {
	a := make(chan int)
	b := make(chan int)

	go func() {
		for i := 0; i < 10; i++ {
			a <- i
		}
		close(a) // close 后不能往其中写数据，继续读取 <-a 则值为 0
	}()

	go func() {
		for x := range a { // a 中的所有数据
			b <- x * x
		}
		close(b)
	}()

	for x := range b {
		time.Sleep(100 * time.Millisecond)
		fmt.Println(x)
	}
}
```

* 判断 chan 是否关闭

```go
package main

import (
	"fmt"
	"time"
)

func main() {
	a := make(chan int)

	go func() {
		for i := 0; i < 10; i++ {
			a <- i
		}
		close(a)
	}()

	for {
		if x, ok := <-a; ok {
			time.Sleep(100 * time.Millisecond)
			fmt.Println(x)
		} else {
			return
		}
	}
}
```

* 关闭 chan 会发送写的通知

```go
package main

import (
	"fmt"
	"time"
)

func main() {
	a := make(chan int)
	go func() {
		time.Sleep(1000 * time.Millisecond)
		close(a)
	}()
	<-a // goroutine 中 close 后会解除阻塞
	fmt.Println(1)
}
```

* 上述的 chan 都是双向类型，既可以接受数据，又可以从其中读取数据，可以更详细地指定 chan 为单向类型，<-chan 为 receive-only（只能从 chan 中读数据），chan<- 为 send-only（只能往 chan 中发数据）

```go
package main

import (
	"fmt"
	"time"
)

func main() {
	a := make(chan int)
	b := make(chan int)

	go func(a chan<- int) { // a send-only
		for i := 0; i < 10; i++ {
			a <- i
		}
		close(a)
	}(a)

	go func(a <-chan int, b chan<- int) { // a receive-only，b send-only
		for x := range a {
			b <- x * x
		}
		close(b)
	}(a, b)

	func(b <-chan int) { // b receive-only
		for x := range b {
			time.Sleep(100 * time.Millisecond)
			fmt.Println(x)
		}
	}(b)
}
```

* 不能 close receive-only channel

```go
var a <-chan int = make(chan int)
close(a) // 错误
```

* channel 可以指定容量，如果未指定则默认为 0

```go
a := make(chan int, 3)
a <- 42
fmt.Println(len(a), cap(a)) // 1 3
```

* 对于无缓存的 channel，向其中写数据必须马上被读，否则会被阻塞，比如如下代码会导致死锁

```go
package main

import "fmt"

func main() {
	a := make(chan int)
	a <- 1
	fmt.Println(<-a)
}
```

* 指定容量则可以解决此问题

```go
package main

import "fmt"

func main() {
	a := make(chan int, 1)
	a <- 1
	fmt.Println(<-a)
}
```

* select 可以监听多个 chan，只要有就绪的 chan 就执行对应的操作

```go
package main

import (
	"fmt"
	"strconv"
	"time"
)

func main() {
	a := make(chan string)
	b := make(chan string)
	go func() {
		for i := 0; ; i++ {
			time.Sleep(1000 * time.Millisecond)
			a <- "a" + strconv.Itoa(i)
		}
	}()

	go func() {
		time.Sleep(500 * time.Millisecond)
		for i := 0; ; i++ {
			time.Sleep(1000 * time.Millisecond)
			b <- "b" + strconv.Itoa(i)
		}
	}()

	for {
		select {
		case x := <-a:
			fmt.Println(x)
		case x := <-b:
			fmt.Println(x)
		}
	}
}
```

* select 只会选择一个满足条件的分支执行，如果所有分支都不满足条件，则 select 会阻塞，可以通过指定 default 分支避免阻塞

```go
package main

import (
	"fmt"
	"strconv"
	"time"
)

func main() {
	a := make(chan string)
	b := make(chan string)
	go func() {
		for i := 0; ; i++ {
			time.Sleep(1000 * time.Millisecond)
			a <- "a" + strconv.Itoa(i)
		}
	}()

	go func() {
		time.Sleep(500 * time.Millisecond)
		for i := 0; ; i++ {
			time.Sleep(1000 * time.Millisecond)
			b <- "b" + strconv.Itoa(i)
		}
	}()

	for {
		select {
		case x := <-a:
			fmt.Println(x)
		case x := <-b:
			fmt.Println(x)
		default:
		}
		fmt.Println("out of select")
		time.Sleep(100 * time.Millisecond)
	}
}
```

## Sync

* 用 sync.WaitGroup 阻塞一组 goroutine

```go
package main

import (
	"fmt"
	"sync"
)

func main() {
	var wg sync.WaitGroup
	wg.Add(2)

	go func() {
		defer wg.Done() // 减少一个计数
		fmt.Println(1)
	}()

	go func() {
		defer wg.Done() // 减少一个计数
		fmt.Println(2)
	}()

	wg.Wait() // 阻塞直至计数为零
	fmt.Println("done")
}
```

* 用 sync.Mutex 保证同时只有一个 goroutine 访问临界区

```go
package main

import (
	"fmt"
	"sync"
)

type A struct {
	sync.Mutex
	x int
}

func (a *A) f() {
	a.Lock() // 加锁保证连续输出 5 个递增值
	defer a.Unlock()
	for i := 0; i < 5; i++ {
		a.x++
		fmt.Println(a.x)
	}
}

func main() {
	var wg sync.WaitGroup
	wg.Add(2)

	a := A{x: 0}

	go func() {
		a.f()
		wg.Done()
	}()

	go func() {
		a.f()
		wg.Done()
	}()

	wg.Wait()
}
```

* 用读写锁 sync.RWMutex 优化多读少写

```go
package main

import (
	"fmt"
	"sync"
	"time"
)

type A struct {
	sync.RWMutex
}

func (a *A) read() {
	a.RLock() // 锁定读
	defer a.RUnlock()
	time.Sleep(1 * time.Millisecond) // 读耗时 1 毫秒
}

func (a *A) write() {
	a.Lock() // 锁定写
	defer a.Unlock()
	time.Sleep(10 * time.Millisecond) // 写耗时 10 毫秒
}

func main() {
	start := time.Now()
	var wg sync.WaitGroup

	a := A{}

	for i := 0; i < 5; i++ {
		wg.Add(1)
		go func() {
			a.write() // 同时只能有一个写，共需要 50 毫秒
			wg.Done()
		}()
	}

	for i := 0; i < 1000; i++ {
		wg.Add(1)
		go func() {
			a.read() // 同时多个读
			wg.Done()
		}()
	}

	wg.Wait()
	end := time.Now()
	fmt.Println(end.Sub(start)) // 略多于 50 毫秒，如果把读锁换成写锁就要 1 秒多
}
```

* 用 sync.Once 保证仅初始化一次

```go
package main

import (
	"fmt"
	"sync"
)

var setOnce sync.Once
var m map[int]string

func set() {
	fmt.Println(1)
	m = make(map[int]string)
	m[0] = "aaa"
	m[1] = "bbb"
}

func get(x int) string {
	setOnce.Do(set) // 如果多次调用 get，其中只有一次会调用 set
	return m[x]
}

func main() {
	var wg sync.WaitGroup

	for i := 0; i < 5; i++ {
		wg.Add(1)
		go func() {
			fmt.Println(get(0))
			wg.Done()
		}()
	}

	wg.Wait()
}
```

* go 提供了工具来检测运行时的 race condition，比如对如下 `main.go`，执行 `go run -race main.go` 将提示 `WARNING: DATA RACE`

```go
package main

import (
	"sync"
)

func main() {
	var wg sync.WaitGroup
	var x int64 = 0
	wg.Add(2)

	go func() {
		x++
		wg.Done()
	}()

	go func() {
		x++
		wg.Done()
	}()

	wg.Wait()
}
```

* map 不是并发安全的，可以用上述方式检测如下代码在写 map 时存在 race condition

```go
package main

import (
	"sync"
)

func main() {
	var wg sync.WaitGroup
	m := make(map[int]int)
	for i := 0; i < 5; i++ {
		wg.Add(1)
		go func(i int) {
			m[i] = i
			fmt.Println(m[i])
			wg.Done()
		}(i)
	}
	wg.Wait()
}
```

* sync.Map 是并发安全的

```go
package main

import (
	"fmt"
	"sync"
)

func main() {
	var wg sync.WaitGroup
	var m sync.Map
	for i := 0; i < 5; i++ {
		wg.Add(1)
		go func(i int) {
			m.Store(i, i)
			if v, ok := m.Load(i); ok {
				fmt.Println(v)
			}
			wg.Done()
		}(i)
	}
	wg.Wait()
}
```

* atomic

```go
package main

import (
	"sync"
	"sync/atomic"
)

func main() {
	var wg sync.WaitGroup
	var x int64 = 0
	wg.Add(2)

	go func() {
		atomic.AddInt64(&x, 1)
		wg.Done()
	}()

	go func() {
		atomic.AddInt64(&x, 1)
		wg.Done()
	}()

	wg.Wait()
}
```

* 可以通过 runtime.GOMAXPROCS 获取核数，通过 runtime.Goexit 强制结束某个 goroutine 而不影响其他 goroutine

```go
package main

import (
	"fmt"
	"runtime"
	"time"
)

func main() {
	go func() {
		var n int = runtime.GOMAXPROCS(0)
		fmt.Println(n) // 8
		time.Sleep(1000 * time.Millisecond)
		runtime.Goexit()
	}()
}
```

* 如果在 main 中直接使用 runtime.Goexit，它会在其他任务结束后让进程崩溃

```go
package main

import (
	"fmt"
	"runtime"
	"time"
)

func main() {
	go func() {
		var n int = runtime.GOMAXPROCS(0)
		fmt.Println(n) // 8
		time.Sleep(1000 * time.Millisecond)
	}()
	runtime.Goexit()
}
```

## 包

* 创建一个文件夹作为项目根目录，文件夹名任意，假设为 `go-demo`
* 进入项目根目录，执行 `go mod init demo-project`，表示该项目名为 `demo-project`，根目录下将生成一个 `go.mod` 文件，内容如下

```
module demo-project

go 1.15
```

* 创建 `go-demo/src/algo` 文件夹作为自定义包，要使用该自定义包，只需要在代码中 `import"demo-project/src/algo"`
* 创建 `go-demo/src/algo/aaa.go`（文件名任意），内容如下，注意要导出的函数的首字母为大写

```go
package algo

func Sum(x, y int) int {
	return x + y
}
```

* 创建 `go-demo/src/main/main.go`，使用自定义包

```go
package main

import (
	"demo-project/src/algo"
	"fmt"
)

func main() {
	fmt.Println(algo.Sum(1, 2))
}
```

* 通常包名与文件夹名称相同，假设将 `go-demo/src/algo/aaa.go` 中的 package 改名为 `al`

```go
package al

func Sum(x, y int) int {
	return x + y
}
```

* 则 `go-demo/src/main/main.go` 中 import 的方式不变，但调用代码时必须使用真正的 package 名

```go
package main

import (
	"demo-project/src/algo"
	"fmt"
)

func main() {
	fmt.Println(al.Sum(1, 2))
}
```

* 可以在导入包时起别名，调用时使用别名即可

```go
package main

import (
	algorithm "demo-project/src/algo"
	"fmt"
)

func main() {
	fmt.Println(algorithm.Sum(1, 2))
}
```

* 在包中可以指定 `init()` 函数，`init()` 函数会在包被导入时自动调用，比如 `go-demo/src/algo/aaa.go` 内容如下

```go
package al

import "fmt"

func init() {
	fmt.Println("import algo")
}

func Sum(x, y int) int {
	return x + y
}
```

* `go-demo/src/main/main.go` 中导入该包，运行即会调用 `algo` 包中的 `init()`，如果有多个包，则按包的导入顺序调用每个包的 `init()` 函数。注意对于未显式使用的包，需要将别名设置为 `_`

```go
package main

import (
	_ "demo-project/src/algo"
)

func main() {
}
```

## 测试

* a.go

```go
package a

func sum(x, y int) int {
	return x + y
}
```

* a_test.go

```go
package a

import "testing"

func TestSum(t *testing.T) {
	if sum(1, 2) != 3 {
		t.Error(`sum(1+2) != 3`)
	}

	if sum(3, 4) != 7 {
		t.Error(`sum(3+4) != 7`)
	}
}
```

* 执行 `go test -v -cover`（v 显示每个测试套件执行时间，cover 显示覆盖率）

```
=== RUN   TestSum
--- PASS: TestSum (0.00s)
PASS
coverage: 100.0% of statements
ok      go-demo 0.347s
```

* 如果将 a.go 改为

```go
package a

func sum(x, y int) int {
	return x + y + 1
}
```

* 执行 `go test -v -cover`

```
=== RUN   TestSum
    main_test.go:7: sum(1+2) != 3
    main_test.go:11: sum(3+4) != 7
--- FAIL: TestSum (0.00s)
FAIL
coverage: 100.0% of statements
exit status 1
FAIL    go-demo 0.148s
```

* testing.T 有以下方法

```go
func (c *T) Error(args ...interface{})
func (c *T) Errorf(format string, args ...interface{})
func (c *T) Fail()
func (c *T) FailNow()
func (c *T) Failed() bool
func (c *T) Fatal(args ...interface{})
func (c *T) Fatalf(format string, args ...interface{})
func (c *T) Log(args ...interface{})
func (c *T) Logf(format string, args ...interface{})
func (c *T) Name() string
func (t *T) Parallel()
func (t *T) Run(name string, f func(t *T)) bool
func (c *T) Skip(args ...interface{})
func (c *T) SkipNow()
func (c *T) Skipf(format string, args ...interface{})
func (c *T) Skipped() bool
```

## 交叉编译

* GOOS 指定目标平台，可以为 darwin、freebsd、linux、windows
* GOARCH 指定目标平台架构体系，可以为 386、amd64、arm
* 交叉编译不支持 CGO，需要设置 CGO_ENABLED 为 0 来禁用
* Mac 下编译 Linux 可执行程序

```
CGO_ENABLED=0 GOOS=linux GOARCH=amd64 go build
```

* Mac 下编译依赖 CGO 的 Linux 可执行程序

```
# brew install FiloSottile/musl-cross/musl-cross
CGO_ENABLED=1 GOOS=linux GOARCH=amd64 CC=x86_64-linux-musl-gcc CGO_LDFLAGS="-static" go build
```

* Mac 下编译 Windows 可执行程序

```
CGO_ENABLED=0 GOOS=windows GOARCH=amd64 go build
```

* Linux 下编译 Mac 可执行程序

```
CGO_ENABLED=0 GOOS=darwin GOARCH=amd64 go build
```

* Linux 下编译 Windows 可执行程序

```
CGO_ENABLED=0 GOOS=windows GOARCH=amd64 go build
```

* Windows 下编译 Mac 可执行程序

```
SET CGO_ENABLED=0
SET GOOS=darwin
SET GOARCH=amd64
go build
```

* Windows 下编译 Linux 可执行程序

```
SET CGO_ENABLED=0
SET GOOS=linux
SET GOARCH=amd64
go build
```
