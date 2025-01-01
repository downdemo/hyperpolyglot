## 变量

```java
char a = '中';
boolean b = true;
final double c = Math.PI;  // final 表示常量
int i = (int)c;  // 类型转换，i 为 3
var x = 1;  // 自动推断类型，相当于 int x = 1
String s = "hi";
```

* 浮点数计算在除数为 0 时不会报错，而是返回特殊值

```java
double d = 0/0;  // 错误
double d1 = 0.0/0;  // NaN
double d2 = 1.0/0;  // Infinity
double d3 = -1.0/0;  // -Infinity
```

* 数组

```java
int[] a1 = new int[3];
a[0] = 3;
a[1] = 1;
a[2] = 2;
int[] a2 = { 3, 1, 2 };
// 遍历
for(int i = 0; i < a2.length; ++i)
{
    System.out.println(a2[i]);
}
for(int x : a2) System.out.println(x);

import java.util.Arrays;

Arrays.sort(a2);  // 按升序排序
System.out.println(Arrays.toString(a2));  // [1, 2, 3]
```

* 枚举

```java
enum Color { red, blue, white; }

Color x = Color.red;
System.out.println(x == Color.red);  // true
String s = Color.red.name();  // String s = "red"
int n = Color.blue.ordinal();  // 1
```

* [BigInterger](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/math/BigInteger.html)

```java
import java.math.BigInteger;

BigInterger n = new BigInterger("10").pow(10000);
System.out.println(n);  // 100000...（共 10000 个 0）
long n2 = n.longValueExact();  // 超出范围，抛出 ArithmeticException 异常
```

* [BigDecimal](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/math/BigDecimal.html)

```java
import java.math.BigDecimal;

BigDecimal d = new BigDecimal("10.10").pow(1000);
System.out.println(d.scale());  // 2000（小数位数）
BigDecimal d2 = d.stripTrailingZeros();  // 去除末尾的 0
System.out.println(d2.scale());  // 1000
System.out.println(d.compareTo(d2));  // 0（相等为 0，小于为负大于为正）
```

* [SecureRandom](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/security/SecureRandom.html)

```java
import java.security.SecureRandom;

SecureRandom sr = null;
try {
    sr = SecureRandom.getInstanceStrong();
}
catch (NoSuchAlgorithmException e) {
    sr = new SecureRandom();
}
System.out.println(sr.nextInt());  // 生成一个 int 随机数
System.out.println(x.nextDouble());  // 生成一个 double 随机数
System.out.println(x.nextBoolean());  // 生成一个 boolean 随机数
```

## [String](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/lang/String.html)

* 比较字符串应该用 equals 而 不应该用 ==

```java
String s1 = "hi";
String s2 = "HI".toLowerCase();
String s3 = "HI";
System.out.println(s1 == s2);  // false
System.out.println(s1.equals(s2));  // true
System.out.println(s1.equalsIgnoreCase(s3));  // true：比较时忽略大小写
```

* 索引

```java
"hello".contains("el");  // true
"hello".startsWith("he");  // true
"hello".endsWith("lo");  // true
"hello".indexOf("l");  // 2
"hello".lastIndexOf("l");  // 3
```

* 子串

```java
"hello".substring(1, 3);  // el
"hello".substring(1);  // ello
```

* 字符处理

```java
String s = "          heLLo    ".trim();  // 删除两侧空白字符
s.toUpperCase();  // HELLO
s.toLowerCase();  // hello
s.replace('L', 'l');  // hello：所有字符 'L' 替换为 'l'
s.replace("LL", "l");  // helo：所有子串 "LL" 替换为 "l"
s.replaceAll("\\w", "a");  // aaaaa：使用正则表达式，所有字符替换为 'a'
String[] ss = "1|2|3|4|5".split("\\|");  // String[] ss = { "1", "2", "3", "4", "5" }
String s2 = String.join(",", s2);  // String s2 = "1,2,3,4,5"
```

* 字符转换

```java
String a = String.valueOf(3.14);  // String a = "3.14"
String b = String.valueOf(true);  // String b = "true"

int x = Integer.parseInt("42");  // int x = 42
boolean y = Boolean.parseBoolean("TrUe");  // boolean y = true
char[] z = "hello".toCharArray();
String s = new String(z);
```

* 使用 [StringBuilder](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/lang/StringBuilder.html) 拼接字符串

```java
StringBuilder sb = new StringBuilder(100);
for(int i = 0; i < 3; ++i) sb.append(i).append(',');
sb.delete(sb.length() - 1, sb.length());
String s = sb.toString();  // String s = "0,1,2"
```

* [StringJoiner](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/util/StringJoiner.html) 封装了 [StringBuilder](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/lang/StringBuilder.html)，拼接更方便。[String.join()](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/lang/String.html#join (java.lang.CharSequence,java.lang.CharSequence...)) 是对 [StringJoiner](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/util/StringJoiner.html) 的封装

```java
import java.util.StringJoiner;

StringJoiner sj = new StringJoiner(",", "A:", ";");  // 分隔符、前缀、后缀
for(int i = 0; i < 3; ++i) sj.add(String.valueOf(i));
String s = sj.toString();  // String s = "A:0,1,2;"
```

## [List](https://docs.oracle.com/en/java/javase/23/docs/api/java.desktop/java/awt/List.html)：[ArrayList](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/util/ArrayList.html)、[LinkedList](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/util/LinkedList.html)

```java
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

List<Integer> v = new ArrayList<>();
List<Integer> v2 = List.of(2, 1, 3);  // 快速创建，但不可改动
v.add(42);
v.add(43);
v.add(44);
v.remove(0);  // 移除索引为 0 的元素
System.out.println(v.contains(42));  // false
System.out.println(v.contains(43));  // true
System.out.println(v.get(0));  // 43
System.out.println(v.indexOf(44));  // 1

// 遍历
import java.util.Iterator;

for(Iterator<Integer> it = v.iterator(); it.hasNext();) {
    Integer n = it.next();
    System.out.println(n);
}

for(Integer x : v) System.out.println(x);

// List 和 Array 的互转
Integer[] a1 = v.toArray(new Integer[v.size()]);
Integer[] a2 = v.toArray(Integer[]::new);
List<Integer> x = List.of(a1);  // x 不可改动

// 其他操作
import java.util.Collections;

Collections.shuffle(v);  // 洗牌
Collections.sort(v);  // 排序
List<Integer> v3 = Collections.unmodifiableList(v);  // v3 不可改动，但改动 v 会影响 v3
v = null;  // 因此用这种方式创建不可变集合后，应该将原集合置空
```

## [Map](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/util/Map.html)：[HashMap](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/util/HashMap.html)、[EnumMap](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/util/EnumMap.html)、[SortedMap](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/util/SortedMap.html)-[TreeMap](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/util/TreeMap.html)

```java
import java.util.HashMap;
import java.util.Map;

Map<String, Integer> m = new HashMap<>();
m.put("a", 1);
m.put("b", 2);
m.put("c", 3);
m.remove("b");
System.out.println(m.get("a"));  // 1
System.out.println(m.get("b"));  // null
System.out.println(m.containsKey("b"));  // false
System.out.println(m.containsValue(3));  // true

// 遍历
for(String k : m.keySet()) {
    System.out.println(k + ":" + m.get(k));
}
for(Map.Entry<String, Integer> x : m.entrySet()) {
    System.out.println(x.getKey() + ":" + x.getValue());
}
```

## [Set](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/util/Set.html)：[HashSet](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/util/HashSet.html)、[SortedSet](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/util/SortedSet.html)-[TreeSet](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/util/TreeSet.html)

```java
import java.util.HashSet;
import java.util.Set;

Set<String> s = new HashSet<>();
System.out.println(s.add("a"));  // true
System.out.println(s.add("a"));  // false
System.out.println(s.add("b"));  // true
System.out.println(s.size());  // 2
System.out.println(s.remove("a"));  // true
System.out.println(s.contains("a"));  // false
for(String x : s) System.out.println(x);
```

## [Queue](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/util/Queue.html)：[LinkedList](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/util/LinkedList.html)、[PriorityQueue](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/util/PriorityQueue.html)

```java
import java.util.LinkedList;
import java.util.Queue;

Queue<Integer> q = new LinkedList<>();
q.add(1);  // 添加失败则抛出异常
q.offer(2);  // 添加成功返回 true，失败则返回 false
System.out.println(q.element());  // 1：返回首元素，队列为空抛出异常
System.out.println(q.peek());  // 1：返回首元素，队列为空则返回 null
System.out.println(q.remove());  // 1：移除并返回首元素，队列为空则抛出异常
System.out.println(q.poll());  // 2：移除并返回首元素，队列为空则返回 null
System.out.println(q.poll());  // null
System.out.println(q.peek());  // null
```


## [Deque](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/util/Deque.html)：[LinkedList](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/util/LinkedList.html)、[ArrayDeque](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/util/ArrayDeque.html)

```java
import java.util.Deque;
import java.util.LinkedList;

Deque<Integer> d = new LinkedList<>();
d.addFirst(1);  // 添加首元素，成功返回 true，失败则返回 false
d.offerFirst(2);  // 添加首元素，添加失败则抛出异常
d.addLast(3);  // 添加尾元素，
d.offerLast(4);  // 添加尾元素
for(Integer x : d) System.out.println(x);  // 2134
System.out.println(d.removeFirst());  // 2：移除并返回首元素，队列为空则抛出异常
System.out.println(d.pollLast());  // 4：移除并返回尾元素，队列为空则返回 null
System.out.println(d.getFirst());  // 1：返回首元素，队列为空则抛出异常
System.out.println(d.peekLast());  // 3：返回尾元素，队列为空则返回 null
```

* [Stack](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/util/Stack.html) 是遗留类，不应使用，可用 [Deque](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/util/Deque.html) 替代

```java
import java.util.Deque;
import java.util.LinkedList;

Deque<Integer> s = new LinkedList<>();
s.push(1);
s.push(2);
System.out.println(d.peek());  // 2：返回栈顶元素，栈为空则返回 null
System.out.println(d.pop());  // 2：移除并返回栈顶元素，栈为空则抛出异常
System.out.println(d.peek());  // 1
```

## [File](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/io/File.html)

```java
import java.io.File;

File f = new File("test.txt");  // 获取文件（文件可不存在）
System.out.println(f.isFile());  // 文件是否存在
System.out.println(f.isDirectory());  // 目录是否存在
System.out.println(f.getName());  // 文件名
System.out.println(f.getPath());  // 相对路径
System.out.println(f.getAbsolutePath());  // 绝对路径
System.out.println(f.canRead());  // 文件是否可读
System.out.println(f.canWrite());  // 文件是否可写
System.out.println(f.canExecute());  // 文件是否可执行
System.out.println(f.length());  // 文件字节数
File[] fs = new File(".\\").listFiles();  // 当前目录下所有文件和子目录
```

* [Path](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/nio/file/Path.html)：[Paths](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/nio/file/Paths.html)

```java
import java.nio.file.Path;
import java.nio.file.Paths;

Path p = Paths.get(".", "src", "test.txt");
Path p2 = p.toAbsolutePath();
Path p3 = p.normalize();

import java.io.File;
File f = p.toFile();
```

## [InputStream](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/io/InputStream.html)：[FileInputStream](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/io/FileInputStream.html)、[ByteArrayInputStream](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/io/ByteArrayInputStream.html)

```java
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

public void readFile() throws IOException {
    InputStream input = null;
    try {
        input = new FileInputStream("test.txt");
        int x;
        while((x = input.read()) != -1) System.out.println(x);
    } finally{
        if(input != null) input.close();
    }
}
```

* 等价的更简洁的写法

```java
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

public void readFile() throws IOException {
    try(InputStream input = new FileInputStream("test.txt")) {
        int x;
        while((x = input.read()) != -1) System.out.println(x);
    }
}
```

* 一次性读取

```java
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

public void readFile() throws IOException {
    try(InputStream input = new FileInputStream("test.txt")) {
        byte[] buffer = new byte[100];  // 存储读取的内容
        int sz;  // 读取的字节数
        while((sz = input.read(buffer)) != -1) System.out.println(sz);
    }
}
```

## [OutputStream](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/io/OutputStream.html)：[FileOutputStream](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/io/FileOutputStream.html)、[ByteArrayOutputStream](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/io/ByteArrayOutputStream.html)

```java
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

public void writeFile() throws IOException {
    try(OutputStream output = new FileOutputStream("test.txt")) {
        output.write('h');
        output.write('i');
        output.write("world".getBytes("UTF-8"));
    }
}
```

## [Reader](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/io/Reader.html)：[FileReader](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/io/FileReader.html)、[CharArrayReader](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/io/CharArrayReader.html)、[StringReader](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/io/StringReader.html)、[InputStreamReader](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/io/InputStreamReader.html)

```java
import java.io.FileReader;
import java.io.IOException;
import java.io.Reader;
import java.nio.charset.StandardCharsets;

public void readFile() throws IOException {
    try(Reader reader = new FileReader("test.txt", StandardCharsets.UTF_8)) {
        char[] buffer = new char[100];
        int sz;
        while((sz = reader.read(buffer)) != -1) System.out.println(sz);
    }
}
```

## [Writer](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/io/Writer.html)：[FileWriter](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/io/FileWriter.html)、[CharArrayWriter](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/io/CharArrayWriter.html)、[StringWriter](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/io/StringWriter.html)、[OutputStreamWriter](https://docs.oracle.com/en/java/javase/23/docs/api/java.base/java/io/OutputStreamWriter.html)

```java
import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import java.nio.charset.StandardCharsets;

public void writeFile() throws IOException {
    try(Writer writer = new FileWriter("test.txt", StandardCharsets.UTF_8)) {
        writer.write('h');
        writer.write('i');
        writer.write("cpp");
        writer.write("world".toCharArray());
    }
}
```

## 类

```java
public class Main {
    public static void main(String[] args) {
        Person p1 = new Person();
        System.out.println(p1.getName());  // AA
        System.out.println(p1.getAge());  // 1
        Person p2 = new Person("BB", 2);
        System.out.println(p2.getName());  // BB
        System.out.println(p2.getAge());  // 2
        Person p3 = new Person("CC");
        System.out.println(p3.getName());  // CC
        System.out.println(p3.getAge());  // 3
    }
}

class Person {
    private String name = "AA";
    private int age = 1;

    public Person() {}
    public Person(String name, int age) {
        this.name = name;
        this.age = age;
    }
    public Person(String name){
        this(name, 3);
    }
    public String getName() {
        return this.name;
    }
    public int getAge() {
        return this.age;
    }
}
```

* static

```java
public class Main {
    public static void main(String[] args) {
        A a = new A();
        A.f();  // 1
        a.f();  // 1
        a.x = 2;
        A b = new A();
        System.out.println(a.x);  // 2
        System.out.println(b.x);  // 2
        System.out.println(A.x);  // 2
    }
}

class A {
    private int i = 1;
    public static int x;  // static 表示被所有类实例共享

    public static void f() {
        System.out.println(1);
    }
    public static void g() {
        System.out.println(x);  // OK：注意不能是 this.x
        System.out.println(this.i);  // 错误：static 方法不能访问类成员
    }
}
```

## 继承

```java
public class Main {
    public static void main(String[] args) {
        Student s = new Student("AA", 1);
        System.out.println(s.getName());  // AA
        s.printName();  // AA
        System.out.println(s instanceof Student);  // true
        System.out.println(s instanceof Person);  // true
        s.f();  // 12（调用派生类的方法）
    }
}

class Person {
    protected String name;  // 要在派生类中使用则不能为 private

    public Person(String name) {
        this.name = name;
    }
    public String getName() {
        return this.name;
    }
    public void f() {
        System.out.println(2);
    }
}

class Student extends Person {
    private int score;

    public Student(String name, int score) {
        super(name);  // 调用基类构造函数
        this.score = score;
    }
    public void printName() {
        System.out.println(super.name);  // this.name 或 name 效果也一样
    }
    @Override // 加上这句让编译器检查基类的方法被重写
    public void f() { // 重写基类的 f
        super.f();  // 调用基类的 f
        System.out.println(2);
    }
}
```

* 如果希望类的成员初始化后不能被更改，则声明为 final

```java
class Person {
    private final String name;  // 一旦初始化就不能更改

    public Person(String name) {
        this.name = name;  // 可以在构造函数中初始化
    }
    public void setName(String name) {
        this.name = name;  // 错误：不能修改 final 成员
    }
    public String getName() {
        return this.name;
    }
}
```

* 禁止重写

```java
class A {
    public final void f() {} // 该方法不能被重写
}

class B extends A {
    public void f() {} // 错误：不能重写 final 方法
}
```

* 禁止继承

```java
final class A {} // 该类不能被继承
class B extends A {} // 错误：不能继承被 final 标记的类
```


## 抽象类

```java
public class Main {
    public static void main(String[] args) {
        A a = new X();
        A b = new Y();
        a.f();  // 1
        b.f();  // 2
    }
}

abstract class A { // 抽象类
    public abstract void f();  // 该方法必须在派生类中定义
}

class X extends A {
    public void f() {
        System.out.println(1);
    }
}

class Y extends A {
    public void f() {
        System.out.println(2);
    }
}
```


## 接口

```java
public class Main {
    public static void main(String[] args) {
        A a = new X();
        A b = new Y();
        a.f();  // 1
        b.f();  // 2
    }
}

interface A { // 接口
    void f();  // 接口方法默认为 public abstract，必须在实现类中定义
}

class X implements A {
    public void f() {
        System.out.println(1);
    }
}

class Y implements A {
    public void f() {
        System.out.println(2);
    }
}
```

* 接口的 default 方法

```java
public class Main {
    public static void main(String[] args) {
        A a = new X();
        a.f();  // 1（调用接口的 default 方法）
    }
}

interface A {
    default void f() { // 该方法不能被实现类定义
        System.out.println(1);
    }
}

class X implements A {}
```

* 接口可以被继承

```java
interface A {
    void f();
}

interface B extends A {
    void g();
}

class X implements B {
    public void f() {}
    public void g() {}
}
```

* 一个类不能继承多个类，但可以实现多个接口

```java
interface A {}
interface B {}
class X implements A, B {}
```

* 接口只能定义 static final 成员

```java
public class Main {
    public static void main(String[] args) {
        A a = new X();
        System.out.println(a.i);  // 1
    }
}

interface A {
    public static final int i = 1;
    int j = 2;  // 等价于 public static final int j = 2
}

class X implements A {}
```

## 包

```java
// src/test/A.java
package test;

public class A {
    public void f() {
        System.out.println(1);
    }
}

// src/com/downdemo/demo/Hello.java
package com.downdemo.demo;
import test.*;  // 导入 test 包中所有 class（不包括子包）

public class Hello {
    public static void main(String[] args) {
        A a = new A();  // 未导入包或存在同名类则要写为 test.A a = new test.A()
        a.f();  // 1
    }
}
```
