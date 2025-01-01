## 08 不要让异常逃离析构函数

* 同时存在两个异常时，如果程序不结束执行就会导致不明确行为

```cpp
void f() {
  std::vector<A> v;  // 假设 A 的析构函数可能抛出一个异常
}  // v在此处被销毁，销毁其中所有的A，对每个A调用析构函数
// 如果有两个 A 的析构函数抛出异常，就会导致不明确行为
```

* 有两个办法来避免此问题，一是终止程序，二是吞下异常

```cpp
class A {
 public:
  void close();  // 假设这里可能抛出一个异常
};

class B {
 public:
  ~B();  // 析构函数中要对 a 调用 close，如果 close
         // 抛出异常且未被处理则导致上述问题
 private:
  A a;
};

// 一个方法是抛出异常则结束程序
B::~B() {
  try {
    a.close();
  } catch (...) {
    // 记录close调用失败的日志
    std::abort();
  }
}
// 另一个是吞下异常，但这通常也意味着吞掉了发生错误的信息
B::~B() {
  try {
    a.close();
  } catch (...) {
    // 记录close调用失败的日志
  }
}
```

* 但这两个方法都无法让用户对导致抛出异常的情况作出反应，一个较好的办法是重新设计接口，让客户有机会对可能出现的问题作出反应

```cpp
class B {
 public:
  void close() {  // 供用户使用的接口
    a.close();
    flag = true;
  }
  ~B() {
    if (!flag) {
      try {
        db.close();
      } catch (...) {
        // 记录close调用失败的日志
      }
    }
  }

 private:
  A a;
  bool flag;
};
```

## 09 不要在构造和析构过程中调用虚函数

* 基类构造期间调用虚函数时，不会调用派生类的虚函数

```cpp
class A {
 public:
  A() { f(); }  // 基类部分构造期间的对象类型是A，因此调用的是A::f而非B::f
  virtual void f() { std::cout << 1; }
};

class B : public A {
 public:
  virtual void f() { std::cout << 2; }
};

B b;  // 1：基类的构造函数不会调用派生类的虚函数
```

* 析构函数同理

```cpp
class A {
 public:
  ~A() { f(); }
  virtual void f() { std::cout << 1; }
};

class B : public A {
 public:
  ~B() { f(); }
  virtual void f() { std::cout << 2; };
};

{ B b; }  // 21
```

* 如果非要在基类的构造期间调用派生类函数，则可以在派生类中声明一个 static 函数用于传递信息给基类的构造函数

```cpp
class A {
 public:
  A(int) {}
  void f() { std::cout << 1; }
};

class B : public A {
 public:
  B() : A(g()) {}

 private:
  static int g() {
    std::cout << 1;
    return 1;
  }
};
```

## 33 避免覆盖继承而来的名称

* 在 public 继承下，派生类中不要覆盖基类名称，如果覆盖了可以通过 using 声明式提供可见性

```cpp
class A {
 public:
  virtual void f1() = 0;
  virtual void f1(int) {}
  virtual void f2() {}
  void f3() {}
  void f3(double) {}
};

class B : public A {
 public:
  using A::f1;
  using A::f3;
  virtual void f1() {}
  void f3() {}
};

int main() {
  B b;
  b.f1();  // B::f1
  int x = 1;
  b.f1(x);  // A::f1，如果没有 using 就会出错，因为被 B::f1 覆盖了找不到
  b.f2();  // A::f2
  b.f3();  // B::f3
  b.f3(x);  // A::f3，如果没有 using 就会出错，因为被 B::f3 覆盖了
  return 0;
}
```

## 36 不要重写继承而来的非虚函数

* 基类指针指向派生类对象时，调用非虚函数时会调用基类版本，所以不要重新定义继承而来的非虚函数。调用虚函数时会调用派生类重写版本，如果派生类没有重写则就往上调用基类的虚函数

```cpp
class Base {
 public:
  virtual int f();
};
chass D1 : public Base {
 public:
  int f(int);         // 隐藏了基类的 f，此 f (int) 不是虚函数
  virtual void f2();  // 新的虚函数
};
class D2 : public D1 {
 public:
  int f(int);  // 非虚函数，隐藏了 D1::f (int)
  int f();     // 覆盖 Base 的虚函数 f()
  void f2();   // 覆盖 D1 的虚函数 f2
};

Base b;
D1 d1;
D2 d2;

Base* bp1 = &b;
Base* bp2 = &d1;
Base* bp3 = &d2;
// 编译器在运行时确定虚函数版本，判断依据是该指针绑定对象的真实类型
bp1->f();  // 运行时调用 Base::f()
bp2->f();  // 运行时本来调用 D1::f()，但 D1 没有这个虚函数，往上调用 Base::f()
bp2->f2();  // 错误，Base 没有 f2()
bp3->f();   // 运行时调用 D2::f()

D1* d1p = &d1;
D2* d2p = &d2;
d1p->f2();  // 运行时调用 D1::f2()
d2p->f2();  // 运行时调用 D2::f2()

// 再看看对非虚函数 f (int) 的调用
Base* p1 = &d2;
D1* p2 = &d2;
D2* p3 = &d2;
p1->f(42);  // 错误，Base 没有 f (int)
p2->f(42);  // 静态绑定，调用 D1::f (int)
p3->f(42);  // 静态绑定，调用 D2::f (int)
```

* 具体规则是：先看左侧类型（静态绑定），在左侧类型的类中查找调用的函数，若不存在则出错，若存在
  * 为非虚函数，则直接调用左侧类型中的此函数
  * 为虚函数，则在右侧类型（动态绑定）中，调用此同名的虚函数，若不存在则在上一级基类中查找，直到找到同名虚函数并调用

## 46 类模板实例化也会实例化友元

* 如果函数形参是类模板，传入的实参就不会隐式转换为类模板的实例

```cpp
template <typename T>
class Rational {
 public:
  Rational(const T& numerator = 0, const T& denominator = 1);
  const T numerator() const;
  const T denominator() const;

 private:
  int n, d;
};

template <typename T>
const Rational<T> operator*(const Rational<T>& lhs, const Rational<T>& rhs) {
  return Rational<T>(lhs.numerator() * rhs.numerator(),
                     lhs.denominator() * rhs.denominator());
}

Rational<int> oneHalf(1, 2);
// 编译错误：由 oneHalf 推断 T 为 int，而 2 不会隐式转换为 Rational<int>
Rational<int> res = oneHalf * 2;
```

* 解决方法是，将非成员函数声明为友元函数，类模板实例化时也会实例化友元，所以编译器总是能在类模板实例化时得知 T

```cpp
template <typename T>
class Rational {
 public:
  friend const Rational operator*(const Rational& lhs, const Rational& rhs) {
    return Rational(lhs.numerator() * rhs.numerator(),
                    lhs.denominator() * rhs.denominator());
  }
};

template <typename T>
const Rational<T> operator*(const Rational<T>& lhs, const Rational<T>& rhs) {
  return Rational<T>(lhs.numerator() * rhs.numerator(),
                     lhs.denominator() * rhs.denominator());
}

// oneHalf 实例化 Rational<int>，友元函数被同时声明
Rational<int> oneHalf(1, 2);
// OK：此时友元函数相当于一个普通函数而非模板，2 可以隐式转换为 Rational<int>
Rational<int> res = oneHalf * 2;
```

* 类内定义的函数默认隐式 inline，将 inline 声明的开销最小化的做法是令函数不做任何事情，只调用一个类外定义的辅助函数

```cpp
template <typename T>
class Rational;

template <typename T>
const Rational<T> doMultiply(const Rational<T>& lhs, const Rational<T>& rhs) {
  return Rational<T>(lhs.numerator() * rhs.numerator(),
                     lhs.denominator() * rhs.denominator());
}

template <typename T>
class Rational {
 public:
  friend const Rational<T> operator*(const Rational<T>& lhs,
                                     const Rational<T>& rhs) {
    return doMultiply(lhs, rhs);
  }
};
```

## 47 traits

* STL 有五种迭代器分类
  * Input 迭代器只能向前移动，一次一步，只读，且只能读取一次，它们模仿指向输入文件的阅读指针，istream_iterator 是这一分类的代表
  * Output 迭代器类似，但一切只为输出，只能向前移动，一次一步，只能写，且只能涂写一次，它们模仿指向输出文件的涂写指针，ostream_iterator 是代表。这两种是威力最小的迭代器，它们只适合一次性操作算法
  * forward 迭代器，可以做前述两种分类所能做的每一件事，而且可以读或写其所指物一次以上，因此可施行于多次性操作算法。STL 未提供单向 linked_list，但某些程序库有，指入这种容器的迭代器就属于 forward 迭代器。
  * Bedirectional 迭代器比上一个威力更大：除了可以向前移动，还可以向后移动。STL 的 list，set，multiset，map 和 multimap 的迭代器都属于这一类
  * random access 迭代器威力最大，可以执行迭代器算术，常量时间内可以向前或向后跳跃任意距离。内置指针也可以当 random 迭代器用。vector，deque 和 string 提供的迭代器都是这一类
* 标准库为这五种迭代器提供了 tag struct 来区分，这些 struct 之间的继承关系是有效的 is-a 关系，所有 forward 迭代器都是 input 迭代器，依此类推

```cpp
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_iterator_tag : public bidirectional_iterator_tag {};
```

* 实现 advance

```cpp
template <typename IterT, typename DistT>
void advance(IterT& iter, DistT d) {
  if (iter is a random_access_iterator) {
    iter += d;  // 针对random_access迭代器使用迭代器算术运算
  } else {
    if (d >= 0) {
      while (d--) {
        ++iter;
      }
    } else {
      while (d++) {
        --iter;
      }
    }
  }
}
```

* 问题在于如何判断 iter 是否为 random_access 迭代器，这就是 traits 要做的事，即在编译期获取类型信息

```cpp
template <T>
class deque {
 public:
  class iterator {
   public:
    typedef random_access_iterator_tag iterator_category;
  };
};

template <T>
class list {
 public:
  class iterator {
   public:
    typedef bidirectional_iterator_tag iterator_category;
  };
};

template <typename IterT>
struct iterator_traits {
  typedef typename IterT::iterator_category iterator_category;
};

// 指针可以认为是随机访问迭代器，因此对指针做一个偏特化
template <typename IterT>
struct iterator_traits<TierT*> {
  typedef random_access_iterator_tag iterator_category;
};

// 给出 iterator_traits 即可作为判断条件
template <typename IterT, typename DistT>
void advance(IterT& iter, DistT d) {
  if (typeid(typename std::iterator_traits<IterT>::iterator_category) ==
      typeid(std::random_access_iterator_tag))
}
```

* 但 if 语句运行时才会确定，运行时才做编译期间就能做的事情不仅浪费了时间，还可能造成执行码膨胀。一个解决方法是标签分派，也就是根据不同标签设置重载函数

```cpp
template <typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, random_access_iterator_tag) {
  iter += d;
}

template <typename IterT, typename DistT>
void doAdcance(IterT& iter, DistT d, bidirectionl_iterator_tag) {
  if (d >= 0) {
    while (d--) ++iter;
  } else {
    while (d++) --iter;
  }
}

template <typename IterT, typename DistT>
void doAdvance(IterT& iter, DistT d, input_iterator_tag) {
  if (d < 0) throw out_of_range("Negative distance");
  while (d--) ++iter;
}
```

* 有了这些重载函数，advance 只要多指定一个参数转发给这些函数就行了

```cpp
template <typename IterT, typename DistT>
void advance(IterT& iter, DistT d) {
  doAdvance(iter, d, typename std::iterator_traits<IterT>::iterator_category);
}
```

## 49 new_handler

* [operator new](https://en.cppreference.com/w/cpp/memory/new/operator_new)
* [operator delete](https://en.cppreference.com/w/cpp/memory/new/operator_delete)
* [set_new_handler](https://en.cppreference.com/w/cpp/memory/new/set_new_handler)
* 如果 operator new 分配内存失败，在抛出异常之前，它会先调用一个 new_handler 函数用以处理内存不足的错误，这个错误处理函数通过 std::set_new_handler 指定
* set_new_handler 做三件事
  * 尝试使更多内存可用
  * 终止程序（如调用 std::terminate）
  * 抛出 std::bad_alloc 或 std::bad_alloc 的派生类异常

```cpp
namespace std {
using new_handler = void (*)();
new_handler set_new_handler(new_handler new_p) noexcept;
}  // namespace std
```

* set_new_handler 的参数是个函数指针，指向 operator new 无法分配足够内存时该调用的 new_handler 函数，如果是空指针，默认的分配函数直接抛 std::bad_alloc 异常
* set_new_handler 的返回值也是个指针，指向 set_new_handler 被调用前正在执行的那个 new_handler 函数
* 可以这样使用 set_new_handler 函数

```cpp
void f() {  // new_handler函数
  std::cerr << "Unable to satisfy request for memory\n";
  std::abort();  // 分配失败则终止程序
}

int main() {
  // operator 无法满足内存申请时就会不断调用 f，直到分配成功
  std::set_new_handler(f);
  while (true) {
    new char[1024 * 1024 * 1024];  // 循环分配 1GB 内存，分配失败时调用 f
  }
};
```

* 一个设计良好的 new_handler 函数必须做到以下事情
  * 让更多的内存可以被使用。一个做法是程序一开始执行就分配一大块内存，而后当 new_handler 第一次被调用，将它们释放给程序使用
  * 安装另一个 new_handler。如果目前这个 new_handler 无法取得更多内存，或许它知道另外哪个有此能力，这样目前这个就可以调用 set_new_handler 安装另外那个替换自己
  * 卸载 new_handler。也就是将空指针传给 set_new_handler，一旦没安装任何 new_handler，operator new 会在内存分配不成功时抛出异常
  * 抛出 std::bad_alloc（或其派生类）异常。这样的异常不会被 operator new 捕捉，因此会被传播到内存索求处
  * 不返回而是终止程序。通常调用 std::abort

```cpp
void f() {
  std::cout << "Memory allocation failed, terminating\n";
  std::set_new_handler(nullptr);  // 分配失败则抛 std::bad_alloc 异常
}

int main() {
  std::set_new_handler(f);
  try {
    while (true) {
      new int[100000000ul];
    }
  } catch (const std::bad_alloc& e) {
    std::cout << e.what() << std::endl;
  }
}
```

* 要对类对象处理内存分配失败情况，需要声明一个类型为 new_handler 的 static 成员，用以指向类的 new_handler，用于 operator new 无法为对象分配足够内存时调用

```cpp
class A {
 public:
  static std::new_handler set_new_handler(std::new_handler p) noexcept;
  static void* opreator new (std::size_t size) throw(std::bad_alloc);

 private:
  static std::new_handler currentHandler;
};

std::new_handler A::currentHandler = 0;

std::new_handler A::set_new_handler(std::new_handler p) noexcept {
  std::new_handler oldHandler = currentHandler;  // 存储当前指针
  currentHandler = p;  // 将当前指针设为 new_handler
  return oldHandler;   // 返回旧指针
}
```

* 最后，operator new 做以下事情
  * 调用标准 set_new_handler，告知类的错误处理函数，即将 new_handler 安装为 global new_handler
  * 调用全局 operator new，执行实际分配内存。如果分配失败，全局 operator new 会调用类的 new_handler（上一步刚被安装为全局 new_handler）
  * 如果全局 operator new 最终无法分配足够内存，会抛出一个 std::bad_alloc 异常，在此情况下类的 operator new 必须恢复为原本的全局 new_handler 然后再传播该异常
  * 如果全局 operator new 能够分配足够一个类对象所用的内存，类的 operator new 会返回一个指针，指向分配所得。析构函数会自动恢复 operator new 被调用前的全局 new_handler
* 下面通过代码详细解释。使用基础 RAII 操作，构造时存储原有的 new_handler，析构时将通过调用 set_new_handler 安装原有的 new_handler

```cpp
class A {
 public:
  static std::new_handler set_new_handler(std::new_handler p) noexcept;
  static void* opreator new (std::size_t size);

 private:
  static std::new_handler currentHandler;
};
std::new_handler A::currentHandler = 0;
std::new_handler A::set_new_handler(std::new_handler p) noexcept {
  std::new_handler oldHandler = currentHandler;
  currentHandler = p;
  return oldHandler;
}

class NewHandlerHolder {
 public:
  explicit NewHandlerHolder(std::new_handler nh) : handler(nh) {}
  ~NewHandlerHolder() { std::set_new_handler(handelr); }
  NewHandlerHolder(const NewHandlerHolder&) = delete;
  NewHandlerHolder& operator=(const NewHandlerHolder&) = delete;

 private:
  std::new_handler handlder;
};

// 这样 operator new 的实现就很简单
void* A::operator new(std::size_t size) {
  // A 安装 new_handler
  NewHandlerHolder h(std::set_new_handler(currentHandler));
  // 返回值为A之前拥有的currentHandler，即旧的指针被传给NewHandlerHolder存储
  return ::operator new(size);  // 分配内存或者抛出异常
};  // NewHandlerHolder 执行析构函数，安装存储的旧指针，从而恢复全局 new_handler

// A 的用户应该类似这样使用其 new_handler
void outOfMem();  // 函数声明，此函数在对象分配失败时被调用
A::set_new_handler(outOfMem);  // 设定 outOfMem 为类的 new_handler 函数
A* p1 = new A;                 // 如果内存分配失败，调用 outOfMem
// 如果内存分配失败，调用 global new_handler 函数
std::string* ps = new std::string;
A::set_new_handler(nullptr);  // 设定类专属的 new_handler 函数为空
A* p2 = new A;                // 如果内存分配失败，立刻抛出异常
```

* 上述实现对所有类都一样，因此创建类模板 new_handler 是个好想法

```cpp
template <typename T>
class NewHandlerSupport {
 public:
  static std::new_handler set_new_handler(std::new_handler p) noexcept;
  static void* operator new(std::size_t size);

 private:
  static std::new_handler currentHandler;
};

template <typename T>
std::new_handler NewHandlerSupport<T>::set_new_handler(
    std::new_handler p) noexcept {
  std::new_handler oldHandler = currentHandler;
  currentHandler = p;
  return oldHandler;
}

template <typename T>
void* NewHandlerSupport<T>::operator new(std::size_t size) {
  NewHandlerHolder h(std::set_new_handler(currentHandlder));
  return ::operator new(size);
}

template <typename T>
std::new_handler NewHandlerSupport<T>::currentHandler = nullptr;

// 有了这个 class template，为 A 添加 set_new_handler 就很容易了
class A : public NewHandlderSupport<A> {
  // 和先前一样，但不必声明set_new_handler或者operator new
};
```

* 上述做法就是 CRTP，看起来模板参数 T 从未被使用，实际上也不需要使用。这种做法只是希望每个派生类拥有实例互异的 NewHandlerSupport 复件，更确切地说是其 static 成员 currentHandler，T 只是用来区分不同的派生类，模板机制会为每个 T 生成一份 currentHandler

## 50 需要重载 new 和 delete 的场景

* 替换标准库提供的 operator new 或 operator delete 通常基于以下三个理由
  * 用来检测运行上的错误。new 的内存 delete 失败会导致内存泄露，多次对同一块 new 的内存 delete 会导致未定义行为。如果 operator new 持有一串动态分配所得地址，而 operator delete 将地址从中移走，很容易检测出上述错误。各式各样的编程错误会导致数据 overrun（写入点在分配区块末端之后）或 underrun（写入点在分配区块起点之前）。如果自定义一个 operator new 便可超额分配内存，以额外空间放置特定的 byte pattern（即签名，signature）。operator delete 便可以检查上述签名是否变化，若发生变化就表示在分配区的某个生命时间点发生 overrun 或 underrun，这时 operator delete 可以 log 那个事实以及进行非法操作的指针
  * 为了强化效能。标准库所提供的 operator new 和 operator delete 主要用于一般目的，它们不但可被长时间执行的程序（如网页服务器）接受，也可被执行时间小于一秒的程序接受。它们必须处理一系列需求，包括大块内存、小块内存、大小混合内存。它们必须接纳各种分配形态，范围从程序存活期间的少量区块动态分配，到大数量短命对象的持续分配和归还。它们必须考虑破碎问题，这最终会导致程序无法满足大区块内存要求。由于对内存管理器的要求多种多样，因此标准库所提供的 operator new 和 operator delete 采取中庸之道。如果对自己程序的动态内存运行形态有深刻的了解，就会发现定制版的 operator new 和 operator delete 性能胜过默认版本，它们比较快，需要的内存比较少
  * 为了收集使用上的统计数据。自行定义 operator new 和 operator delete 可以帮助收集软件内存区块大小分布、寿命分布、内存归还次序、最大动态分配量等信息
* 以下是一个初阶段全局 operator new 的例子，它能够促进并协助检测 overrun 和 underrun

```cpp
static const int signature = 0xDEADBEEF;
using Byte = unsigned char;
// 这段代码还有若干小错误，详下
void* operator new(std::size_t size) {
  std::size_t realSize = size + 2 * sizeof(int);
  void* p = std::malloc(realSize);
  if (!p) throw std::bad_alloc();
  // 将signature写入内存的最前段落和最后段落
  *(static_cast<int*>(p)) = signatrue;
  *(reinterpret_cast<int*>(static_cast<Byte*>(p) + realSize - sizeof(int))) =
      signature;
  // 返回指针，指向恰位于第一个signature之后的内存位置
  return static_cast<Byte*>(p) + sizeof(int);
}
```

* 这个 operator new 的主要缺点在于它忽略了其应该具备的坚持 C++ 规则的态度，比如条款 51 提到所有 operator new 都应该内含一个循环，反复调用某个 new_handler 函数，这里却没有。此外，还有一个更加微妙的主题：内存对齐（alignment）
* 许多计算机体系结构要求特定类型必须放在特定的内存地址上，例如指针的地址必须是 4 的倍数，double 的地址必须是 8 的倍数。如果不满足这个条件可能会导致运行期硬件异常。有些体系结构如 Intel x86 架构的 double 可被对其于任何边界，但如果它是 8 字节齐位，其访问速度将会快许多
* 在定制 operator new 的过程中，内存对齐意义重大，因为 C++ 要求所有 operator new 返回的指针都有适当的对齐。malloc 就是在这样的要求下工作，所以令 malloc 返回一个得自 malloc 的指针是安全的。然而上述 operator new 并未提供这样的保证，而是返回一个得自 malloc 且偏移一个 int 大小的指针，并不安全。如果客户端调用 operator new 来获取给一个 double 所用的内存，同时在一部 int 为 4 字节且 double 必须为 8 字节齐位的机器上运行，可能会获得一个未有适当齐位的指针，这可能会导致程序崩溃或执行速度变慢。因此写一个能优良运作的内存管理器可能并不容易，许多平台已有可以替代编译器自带之内存管理器的商业产品，开放源码领域的内存管理器（如 Boost 库的 Pool）也都可用，因而可能并不需要自己定制 operator new 和 operator delete
* 总结下来，定制 operator new 和 operator delete 的作用
  * 为了检测运行错误
  * 为了收集动态分配内存之使用统计信息
  * 为了增加分配和归还的速度
  * 为了降低缺省内存管理器所带来的额外开销
  * 为了弥补缺省分配器中的非最佳齐位
  * 为了将相关对象成簇集中
  * 为了获得非传统行为

## 51 new 和 delete 的实现

* operator new 必须返回正确的值，内存不足时必须调用 new_handler 函数，要有对付零内存需求的准备，避免不小心覆盖正常形式的 new，虽然这比较偏近类接口的要求而非实现要求
* operator new 如果申请内存成功，就返回指向那块内存的指针，否则抛出 std::bad_alloc 异常。operator new 实际上不只一次尝试内存分配，并在每次失败后都调用 new_handler 函数。这里假设 new_handler 函数能做某些动作将某些内存释放出来，只有指向 new_handler 函数的指针为空 operator new 才会抛出异常。C++ 规定即使用户要求 0 字节，operator new 也要返回一个合法指针，这种看似诡异的行为其实是为了简化语言其他部分，下面是个非成员 operator new 的伪代码

```cpp
void* operator new(std::size_t size) {
  if (size == 0) size = 1;  // 将 0 字节申请其视为 1 字节申请
  while (true) {
    if (try_malloc(size_bytes)) {
      return malloc_ptr;
    }
    // 分配失败，找到当前的 new_handler 函数
    std::new_handler globalHandler = std::set_new_handler(nullptr);
    std::set_new_handler(globalHandler);
    if (globalHandler) {
      (*globalHandler)();
    } else {
      throw std::bad_alloc();
    }
  }
}
```

* 如果在多线程环境下，还需要某种锁机制，以便处理 new_handler 函数背后的全局数据结构。上面包含一个无穷循环，退出此循环的唯一办法是：内存分配成功或 new_handler 做了一件描述于条款 49 中的事：让更多内存可用、安装另一个 new_handler、卸载 new_handler、抛出 std::bad_alloc 异常（或其派生类），或承认失败直接 return。上面的 operator new 成员函数可能会被继承。函数尝试分配 size 字节，这很合理，因为 size 是函数接收的实参。条款 50 提到，定制内存分配器往往是为了最优化特定的类对象分配行为，而不是为了该类的派生类

```cpp
class Base {
 public:
  static void* operator new(std::size_t size);
};

class Derived : public Base {};  // 假设重新定义 operator new
Derived* p = new Derived;        // 这里调用了 Base::operator new
```

* 如果基类专属的 operator new 并非被设计用来对付上述情况，处理此情势的最佳做法是将内存申请量错误的调用行为改采标准 operator new

```cpp
void* Base::operator new(std::size_t size) {
  if (size != sizeof(Base)) {
    return ::operator new(size);
  }
}
```

* 如果希望控制类专属版的数组内存分配，那么需要实现 operator new []。编写 operator new [] 时，唯一要做的一件事就是分配一块未加工内存，因为无法对数组之内迄今尚未存在的元素对象做任何事，无法知道这个数组含有多少个元素对象。可能不知道每个对象多大，因为基类的 operator new [] 有可能经由继承被调用，将内存分配给派生类对象数组使用。因此不能在 Base::operator new [] 中假设每个元素对象大小是 sizeof (Base)，这也意味着不能假设数组元素个数是 `申请字节数 /sizeof (Base)`。此外，传递给 operator new [] 的 size_t 参数，其值有可能比将被将要填给对象的内存数量更多，因为动态分配的数组可能包含额外空间用来存放元素个数
* operator delete 情况就简单很多，唯一需要记住的就是保证删除空指针永远安全。下面是非成员 operator delete 的伪代码

```cpp
void operator delete(void* p) noexcept {
  if (!p) {
    return;
  }
  free_memory(p);
}
```

* 这个函数的成员函数版本也很简单，只需多加一个检查删除数量

```cpp
class Base {
 public:
  static void* operator new(std::size_t);
  static void operator delete(void*, std::size_t) noexcept;
};

void Base::operator delete(void* p, std::size_t size) noexcept {
  if (!p) return;
  if (size != sizeof(Base)) {
    ::operator delete(p);
    return;
  }
  free_memory(p);
  return;
}
```

* operator new 应该内含一个无穷循环，并在其中尝试分配内存，如果它无法满足内存需求，就该调用 new_handler。它也应该有能力处理 0 字节申请。类专属版本则还应该处理比正确大小更大的（错误）申请
* operator delete 应该在收到空指针时不做任何事。类专属版本则还应该处理比正确大小更大的（错误）申请

## 52 placement new 和 placement delete

* placement new 和 placement delete 是 C++ 经常用到但是却不常见的两个操作符。placement new 的用法如下

```cpp
int* p = new int[100];
int* q = new (p) int(42);  // 利用 p 的空间
std::cout << p[0];         // 42
delete[] p;                // q 的空间会被析构，注意不要 delete q
```

* 比如先申请了一个 100 个字节的空间，再用于一个 int 数组

```cpp
char* p = new char[100];
int* q = new (p) int[100 / sizeof(int)];  // 利用 p 的空间
```

* 当我们使用 new 创建一个对象时

```cpp
A* p = new A;
```

* 有两个函数被调用，第一个函数就是用以分配内存的 operator new，第二个是类的默认构造函数。如果第一个函数调用成功，但是第二个函数抛出异常，这时需要释放第一步分配的内存，否则就造成了内存泄露。此时用户无法归还内存，因为构造函数抛出异常，p 尚未被赋值，用户手中没有指向要被归还的内存的指针
* 释放内存的任务落到了 C++ 运行期系统身上，运行期系统会调用第一个函数 operator new 所对应的 operator delete 版本，operator delete 可能有多个版本，因此它必须知道该调用的版本

```cpp
void* operator new(std::size_t);
void operator delete(void*) noexcept;  // 全局作用域中的正常声明
void operator delete(void*, std::size_t) noexcept;  // 类作用域中的典型声明
```

* 正常的 operator new 对应于正常的 operator delete，如果使用正常的 operator new 和 operator delete，运行期系统可以找到释放 new 开辟内存的 delete 函数。但是如果使用非正常形式的 operator new 就会有问题了。假设有一个类专属的 operator new，它接受一个用来记录相关分配信息的 ostream，同时又写了一个正常形式的类专属的 operator delete

```cpp
class A {
 public:
  static void* operator new(std::size_t,
                            std::ostream&);  // 非正常形式的 new
  static void operator delete(void*,
                              std::size_t) noexcept;  // 正常的类专属 delete
};
```

* 这个设计有问题，在讨论问题前先说明若干术语。operator new 除了必需的 size_t，还接受其他参数，这就是所谓的 placement new。因此，上述的 operator new 是个 placement 版本
* 特别有用的一个 placement new 版本会接受一个指向对象构造之处的指针，形式如下

```cpp
void* operator new(std::size_t, void*) noexcept;  // placement new
```

* 这个版本的 new 已被纳入标准库，它的用途之一是负责在 vector 的未使用空间上创建对象，它同时也是最早的 placement new 版本，谈到 placement new 时通常指的就是这一特定版本
* 现在再来看一下类的声明式，这个类会引起微妙的内存泄露。例如动态创建一个类对象时将分配信息记录于 cerr

```cpp
A* p = new (std::cerr) A;  // 调用 operator new，并传递 cerr 作为 ostream 实参
// 这个动作会在构造函数抛出异常时泄露内存
```

* 如果内存分配成功，而构造函数抛出异常，运行期系统负责释放 operator new 分配的内存，但运行期系统不知道真正被调用的 operator new 行为。于是运行期系统寻找参数个数和类型都与 operator new 相同的 operator delete，这里的 operator new 接受 ostream & 类型的额外实参，对应的 operator delete 就应该是

```cpp
void operator delete(void*, std::ostream&) noexcept;
```

* 类似于 new 的 placement 版本，接收额外参数的 operator delete 称为 placement delete。由于类没有声明 placement delete，运行期系统不知道如何取消并恢复对 placement new 的调用，于是什么都不做，因此类必须声明一个对应的 placement delete

```cpp
class A {
 public:
  static void* operator new(std::size_t, std::ostream&);
  static void operator delete(void*) noexcept;
  static void operator delete(void*, std::ostream&) noexcept;
};
```

* 这样，如果下面的代码导致构造函数抛出异常，就会调用对应的 placement delete

```cpp
A* p = new (std::cerr) A;  // 一如既往，但这次不再泄漏
```

* 但如果没有抛出异常（通常如此），即使有对应的 placement delete，调用的也是正常形式的 operator delete，因为只有伴随 placement new 调用的构造函数抛出异常，对应的 placement delete 才会被调用
* 成员函数的名称会覆盖其外围作用域中的相同名称，必须避免类专属的 new 覆盖外围作用域的 new。比如一个类中声明了唯一的 placement operator new，客户将无法使用正常形式的 new

```cpp
class Base {
 public:
  static void* operator new(std::size_t,
                            std::ostream&);  // 覆盖了正常形式的 new
};
Base* pb1 = new Base;  // 错误：正常形式的 operator new 被覆盖
Base* pb2 = new (std::cerr) Base;  // 正确：调用 Base 的 placement new
```

* 同理，派生类中的 operator new 会覆盖全局版本和继承版本

```cpp
class Derived : public Base {
 public:
  static void* operator new(std::size_t);  // 重新声明正常形式的 new
};

Derived* pd1 = new (std::clog) Derived;  // 错误：Base 的 placement new 被覆盖
Derived* pd2 = new Derived;  // 正确：调用 Derived 的 operator new
```

* 以下标准形式 operator new 在全局作用域中默认提供

```cpp
void* operator new(std::size_t);                  // normal new
void* operator new(std::size_t, void*) noexcept;  // placement new
void* operator new(std::size_t, const std::nothrow_t&) noexcept;  // nothrow new
```

* 类内声明任何形式的 operator new 都会覆盖这些标准形式，对于每个可用的 operator new 必须提供对应的 operator delete。一个简单做法是建立一个包含所有正常形式的 new 和 delete 的类

```cpp
class StadardNewDeleteForms {
 public:
  // normal new/delete
  static void* operator new(std::size_t n) { return ::operator new(n); }
  static void operator delete(void* p) noexcept { ::operator delete(p); }
  // placement new/delete
  static void* operator new(std::size_t n, void* p) noexcept {
    return ::operator new(n, p);
  }
  static void operator delete(void* p, void* q) noexcept {
    return ::operator delete(p, q);
  }
  // nothrow new/delete
  static void* operator new(std::size_t n, const std::nothrow_t& t) noexcept {
    return ::operator new(n, t);
  }
  static void operator delete(void* p, const std::nothrow_t&) noexcept {
    ::operator delete(p);
  }
};
```

* 使用继承机制和 using 声明就能以自定义方式扩充标准形式

```cpp
class A : public StandardNewDeleteForms {
 public:
  using StandardNewDeleteForms::operator new;
  using StandardNewDeleteForms::operator delete;
  // 添加自定义的placement new
  static void* operator new(std::size_t, std::ostream&);
  // 添加自定义的placement delete
  static void operator detele(void*, std::ostream&) noexcept;
};
```
