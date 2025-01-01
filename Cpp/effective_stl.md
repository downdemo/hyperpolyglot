## 06 C++ Most Vexing Parse

* C++11 引入了花括号初始化方式语法解决此问题

```cpp
#include <cassert>
#include <fstream>
#include <iterator>
#include <vector>

int main() {
  const std::vector<int> data{12, 34, 56, 78};
  std::string filepath = "ints.data";
  {
    std::ofstream fstream{filepath, std::ios_base::trunc};
    for (const auto& x : data) {
      fstream << x << " ";
    }
  }
  std::ifstream fstream(filepath);
  std::vector<int> dst(
      std::istream_iterator<int>{fstream},  // 用小括号会被视为函数声明编译失败
      std::istream_iterator<int>());  // 用小括号无歧义
  assert(dst == data);
}
```

## 09 不同容器的删除方法

* 删除指定值元素的方法因容器类型不同而不同

```cpp
// 连续内存容器（vector、deque、string）最好的删除方法是 erase-remove 惯用法
c.erase(std::remove(c.begin(), c.end(), 1963), c.end());
// list 的成员函数直接 remove 更高效
c.remove(1963);
// 关联容器没有 remove，方法是调用 erase，只花费对数时间（序列容器为线性时间）
c.erase(1963);
```

* 关联容器有两种方法

```cpp
// 低效方法用 remove_copy_if
// 把需要的值拷贝到一个新容器，然后把原容器的内容和新的交换
std::remove_copy_if(c.begin(), c.end(), std::inserter(c2, c2.end()), f);
c.swap(c2);
// 高效方法遍历关联容器删除
for (auto it = c.begin(); it != c.end();) {
  if (f(*it)) {
    std::cout << "Erasing " << *it << std::endl;
    c.erase(it++);
  } else {
    ++it;
  }
}
```

* vector、string 和 deque 不能再使用 erase-remove 惯用法，也不能仿照关联容器用 erase，因为 erase 会使序列容器的被删元素和之后所有的元素迭代器失效，再自增会出错。解决方法是使用 erase 的返回值保持迭代器有效

```cpp
for (auto it = c.begin(); it != c.end();) {
  if (f(*it)) {
    std::cout << "Erasing " << *it << std::endl;
    it = c.erase(i);
  } else {
    ++it;
  }
}
```

* 对于 list，关联容器和序列容器的做法都可行
* C++20 引入了 ranges 彻底解决此类语法不统一的问题

## 10 分配器的协定和约束

* 分配器最初被设想为抽象内存模型，C++ 标准中类型 T 的对象的默认分配器（`allocator<T>`）提供 `typedef allocator<T>::pointer` 和 `allocator<T>::reference`，而且也希望用户定义的分配器也提供这些 typedef
* 标准允许 STL 实现认为相同类型的分配器等价，原因是，假设有两个容器 v1 和 v2，把 v2 赋值到 v1，销毁 v1 时 v1 的分配器要能回收由 v2 的分配器分配的内存，如果两者不等价，接合操作就很难实现
* 相同类型的分配器等价是十分严厉的约束，这代表如果要可移植，分配器就不能有状态，即不能有任何非静态数据成员，这表明不能有从两个不同的堆分配的 `SpecialAllocator<int>`，它们是不等价的
* 分配器在分配原始内存方面类似 operator new，但它们的接口不同，两者都带有一个指定要分配多少内存的参数，但对于 operator new，这个参数指定的是字节数，而对于 `allocator<T>::allocate` 指定的是内存里能容纳多少个 T 对象。在 `sizeof (int) == 4` 的平台上，容纳一个 int 的内存得把 4 传给 operator new，把 1 传给 `allocator<int>::allocate`

```cpp
void* operator new(size_t bytes);
pointer allocator<T>::allocate(size_type numObjects);
```

* 大多数标准容器从未调用它们例示的分配器，list、set、multiset、map、multimap 都是如此。原因是这些容器的节点结构不是 T 而是基于 T 的结构体，分配器无法提供分配方案，比如添加一个新节点到 list 时，需要从分配器获取的不是 T 的内存而是包含了一个 T 的 ListNode 的内存，分配器不能提供 list 需要的，因此 list 不让 Allocator 做任何分配

```cpp
// list 的可能实现
template <typename T, typename Allocator = allocator<T>>
class list {
 private:
  Allocator alloc;   // 用于 T 类型对象的分配器
  struct ListNode {  // 链表里的节点
    T data : ListNode *prev;
    ListNode *next;
  };
};
```

* 分配器模板 A（例如，std::allocator，SpecialAllocator，等）都有一个如下的 rebind 的内嵌结构体模板。`list<T>` 可以通过 `Allocator::rebind<ListNode>::other` 获取对应的 ListNode 分配器

```cpp
template <typename T>
class allocator {
 public:
  template <typename U>
  struct rebind {
    using other = allocator<U>;
  }
};
```

* 如果要自定义分配器，必须
  * 把分配器做成一个模板，模板参数 T 代表要分配内存的对象类型
  * 提供 pointer 和 reference 的 typedef，让 pointer 是 `T*`，reference 是 `T&`
  * 不要给你的分配器对象状态，分配器不能有非静态的数据成员
  * 传给分配器的 allocate 成员函数需要分配的对象个数而不是字节数，函数返回 `T*` 指针（通过 pointer typedef），即使还没有 T 对象被构造
  * 提供标准容器依赖的内嵌 rebind 模板

## 11 自定义分配器的正确用法

* 假如有一个仿效 malloc 和 free 的程序，用于管理共享内存的堆

```cpp
void* mallocShared(size_t bytesNeeded);
void freeShared(void* ptr);
```

* 希望能把 STL 容器的内容放在共享内存中

```cpp
template <typename T>
class SharedMemoryAllocator {
 public:
  pointer allocate(size_type numObiects, const void *localityHint = 0) {
    return static_cast<pointer>(mallocShared(numObiects * sizeof(T)));
  }
  void deallocate(pointer ptrToMemory, size_ type numObjects) {
    freeShared(ptrToMiemory);
  }
};
```

* 使用 SharedMemoryAllocator

```cpp
using SharedDoubleVec = vector<double, SharedMemoryAllocator<double>>;
SharedDoubleVec v;  // 建立一个元素在共享内存中的 vector
```

* v 元素的内存来自共享内存，但 v 本身在堆上而非共享内存。如果要把 v 的内容放进共享内存则要用 placement-new

```cpp
void *pVectorMemory = mallocShared(sizeof(SharedDoubleVec));
if (!pVectorMemory) {
  throw;
}
SharedDoubleVec *pv = new (pVectorMemory) SharedDoubleVec;
pv->~SharedDoubleVec();     // 销毁共享内存中的对象
freeShared(pVectorMemory);  // 销毁原来的共享内存块
```

* 再举一个例子，有两个堆，命名为 Heap1 和 Heap2 类。每个堆类有用于进行分配和回收的静态成员函数

```cpp
class Heap1 {
 public:
  static void *alloc(size_t numBytes, const void *memoryBlockToBeNear);
  static void dealloc(void *ptr);
};

class Heap2 {
 public:
  static void *alloc(size_t numBytes, const void *memoryBlockToBeNear);
  static void dealloc(void *ptr);
};
```

* 把 STL 容器元素按需放到指定堆

```cpp
template <typenameT, typename Heap>
class SpecificHeapAllocator {
 public:
  pointer allocate(size_type numObjects, const void* localityHint = nullptr) {
    return static_cast<pointer>(
        Heap::alloc(numObjects * sizeof(T), localityHint));
  }
  void deallocate(pointer ptrToMemory, size_type numObjects) {
    Heap::dealloc(ptrToMemory);
  }
};
// ab 的元素放进 Heap1
// cd 的元素放进 Heap2
std::vector<int, SpecificHeapAllocator<int, Heap1>> a;
std::set<int, SpecificHeapAllocator<int, Heap1>> b;
std::list<Widget, SpecificHeapAllocator<Widget, Heap2>> c;
std::map<int, std::string, std::less<int>,
         SpecificHeapAllocator<std::pair<const int, std::string>, Heap2>>
    d;
```

* C++20 几乎没有自定义分配器的必要

## 15 string 的多种实现

* string 对象的大小可能是 1 到至少 7 倍 `char*` 指针的大小，为了理解存在差别的原因，必须知道 string 可能存的数据和保存的位置
* 实际上每个 string 实现都容纳了下面的信息，不同的 string 实现以不同的方式把这些信息放在一起
  * 字符串的大小
  * 容纳字符串字符的内存容量
  * 这个字符串的值，即构成这个字符串的字符
  * 一个 string 可能容纳它的配置器的拷贝
  * 依赖引用计数的 string 实现包含了这个值的引用计数
* 实现 A 有一次动态分配

```cpp
struct Node {
  size_t ref_cnt;
  T value;
};
// 使用默认配置器的字符串对象是指针大小的四倍
// 如果使用自定义配置器，string 对象会随配置器对象的增大而变大
class string {
  Allocator alloc;
  size_t sz;
  size_t capacity;
  Node* value;
};
```

* 实现 B 有两次动态分配，如果关心动态分配和回收内存的次数和开销，避开这个实现

```cpp
struct Node {
  size_t sz;
  size_t capacity;
  size_t ref_cnt;
  T* value;
  Other other;  // 多线程系统中与并发控制有关的一些附加数据 6 倍指针大小
};

class string {  // 使用默认分配器不占空间
  Node* value;
};
```

* 实现 C 有一次动态分配

```cpp
struct Node {
  size_t sz;
  size_t capacity;
  size_t ref_cnt;
  X x;      // 一些关于值可共享性的数据
  T value;  // 不支持动态分配
};

class string {
  Node* value;
};
```

* 实现 D 没有动态分配，每个 string 包含了一个足以表现最多 15 个字符的字符串值的内部缓冲区，小字符串整个保存在 string 对象中（即 SSO），当一个容量超过 15 时，缓冲器的第一部分被用作指向动态分配内存的一个指针，而字符串的值存放在那块内存中。在 VS 中空 string 的 size 就是 15，sizeof(string) 是 28

```cpp
// capacity <= 15
class string_capacity_le_15 {
  Allocator alloc;
  Other other;
  size_t sz;
  size_t capacity;
};

// capacity > 15
class string_capacity_gt_15 {
  Allocator alloc;
  T* value;
  char unused_buffer[];
  size_t sz;
  size_t capacity;
};
```

## 19 相等和等价的区别

* std::find 和 set::insert 是判断两个值是否相同的函数代表，std::find 基于 `operator==` 对相同的定义是相等即 `a == b`，set::insert 基于 `operator<` 对相同的定义是等价即 `!(a < b) && !(b < a)`

```cpp
#include <algorithm>
#include <cassert>
#include <cctype>
#include <set>
#include <string>

struct InsensitiveCompare {
  bool operator()(const std::string& lhs, const std::string& rhs) const {
    std::size_t less_len = std::min(lhs.size(), rhs.size());
    for (std::size_t i = 0; i < less_len; ++i) {
      if (std::tolower(lhs[i]) < std::tolower(rhs[i])) {
        continue;
      }
      return false;
    }
    return true;
  }
};

int main() {
  std::set<std::string, InsensitiveCompare> s;
  std::string str1 = "Abc";
  std::string str2 = "abc";
  // 成员函数忽略大小写
  s.insert(str1);
  s.insert(str2);
  assert(s.size() == 1);
  assert(*s.begin() == str1);
  assert(s.find(str1) != s.end());
  assert(s.find(str2) != s.end());
  assert(s.count(str1));
  assert(s.count(str2));
  // std::find 仍匹配大小写
  assert((std::find(s.begin(), s.end(), str1) != s.end()));
  assert(!(std::find(s.begin(), s.end(), str2) != s.end()));
}
```

## 20 元素为指针的关联容器的比较器

* 元素类型为指针时，默认的排序方式是按指针地址大小，如果要按指针解引用的值排序则需要自定义比较函数

```cpp
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <string>

struct StringCompare {
  bool operator()(const std::string* lhs, const std::string* rhs) const {
    return *lhs < *rhs;
  }
};

struct Dereference {
  template <typename T>
  const T& operator()(const T* p) const {
    return *p;
  }
};

int main() {
  std::set<std::string*, StringCompare> s;
  auto str1 = std::make_unique<std::string>("b");
  auto str2 = std::make_unique<std::string>("a");
  auto str3 = std::make_unique<std::string>("c");
  s.emplace(str1.get());
  s.emplace(str2.get());
  s.emplace(str3.get());
  std::transform(s.begin(), s.end(),
                 std::ostream_iterator<std::string>(std::cout, "\n"),
                 Dereference());  // a\nb\nc\n
}
```

## 21 让比较器对相等的值返回 false

* 如果让比较器对相等的值 true，则能插入相同值，且无法查找到，因为等价的定义变成了 `!(a <= b) && !(b <= a)`，相同值不等价所以可以插入，而查找时则由于不等价查找结果为空

```cpp
#include <cassert>
#include <set>

int main() {
  std::set<int, std::less_equal<int>> s;
  s.emplace(1);
  s.emplace(1);
  assert(s.size() == 2);
  assert(!s.count(1));
  std::multiset<int, std::less_equal<int>> s2;
  s2.emplace(1);
  s2.emplace(1);
  s2.emplace(1);
  assert(s2.size() == 3);
  const auto [l, r] = s2.equal_range(1);
  int cnt = 0;
  for (auto it = l; it != r; ++it) {
    ++cnt;
  }
  assert(cnt == 0);
}
```

## 28 了解如何通过 reverse_iterator 的 base 得到 iterator

* LegacyOutputIterator：该迭代器与其他迭代器不同。因为这是一个单纯用于写出的迭代器，其只能增加，并且将对应内容写入文件当中。如果要读取这个迭代中的数据，那么读取到的值就是未定义的
* LegacyInputIterator：只能用来读取指向的值。当该迭代器自加时，之前指向的值就不可访问。也就是说，不能使用这个迭代器在一个范围内遍历多次。std::istream_iterator 就是这样的迭代器
* LegacyForwardIterator：类似于输入迭代器，不过其可以在指示范围内迭代多次。`std::forward_list` 的迭代器就是这样的。就像一个单向链表一样，只能向前遍历，不能向后遍历，但可以反复迭代
* LegacyBidirectionalIterator：从名字就能看出来，这个迭代器可以自增，也可以自减，迭代器可以向前或向后迭代。`std::list`，`std::set` 和 `std::map` 都支持双向迭代器
* LegacyRandomAccessIterator：与其他迭代器不同，随机访问迭代器一次可以跳转到任何容器中的元素上，而非之前的迭代器，一次只能移动一格。`std::vector` 和 `std::deque` 的迭代器就是这种类型
* LegacyContiguousIterator：这种迭代器具有前述几种迭代器的所有特性，不过需要容器内容在内存上是连续的，类似一个数组或 `std::vector`
* reverse_iterator::base 指向反向迭代器的 base iterator，即当前所在位置往右一个位置，reverse_iterator 不能用于 erase 或 insert，必须先转为 iterator

```cpp
#include <algorithm>
#include <cassert>
#include <vector>

int main() {
  std::vector<int> v{1, 2, 3, 4, 5};
  std::vector<int>::reverse_iterator r_it = std::find(v.rbegin(), v.rend(), 3);
  assert(*r_it == 3);
  assert(*r_it.base() == 4);
  std::vector<int>::iterator it(r_it.base());
  assert(*it == 4);
  // erase 或 insert 不支持 reverse_iterator 因此要转换
  // --r_it.base() 修改了 base() 的返回值，(++r_it).base() 写法更好
  v.erase((++r_it).base());
  assert(v == (std::vector<int>{1, 2, 4, 5}));
}
```

## 29 需要一个一个字符输入时考虑使用 istreambuf_iterator

* 把一个文本文件拷贝到一个字符串对象

```cpp
#include <cassert>
#include <fstream>
#include <iterator>
#include <string>

int main() {
  std::string filepath = "data.txt";
  {
    std::ofstream fstream{filepath, std::ios_base::trunc};
    fstream << "hello world" << std::endl;
  }
  std::ifstream fstream(filepath);
  // operator>> 读取时默认忽略空格，保留空格需要清除输入流的 skipws 标志
  fstream.unsetf(std::ios::skipws);
  std::string dst(std::istream_iterator<char>{fstream},
                  std::istream_iterator<char>());
  assert(dst == "hello world\n");
}
```

* istream_iterators 拷贝速度可能不快，它依靠的 `operator>>` 函数进行的是格式化输入，每次调用时必须建立和销毁岗哨（sentry）对象（为每个 `operator>>` 调用进行建立和清除活动的特殊的 iostream 对象），检查可能影响行为的流标志（比如 skipws），进行全面的读取错误检查，如果遇到问题必须检查流的异常掩码决定是否抛出异常。更高效的方法是不用格式化输入，用 istreambuf_iterators，它从 istream s 中读取会调用 `s.rdbuf()->sgetc()` 读 s 的下一个字符，只抓取流缓冲区的下一个字符，不需要 unsetf

```cpp
#include <cassert>
#include <fstream>
#include <iterator>
#include <string>

int main() {
  std::string filepath = "data.txt";
  {
    std::ofstream fstream{filepath, std::ios_base::trunc};
    fstream << "hello world" << std::endl;
  }
  std::ifstream fstream(filepath);
  std::string dst(std::istreambuf_iterator<char>{fstream},
                  std::istreambuf_iterator<char>());
  assert(dst == "hello world\n");
}
```

## 30 确保目标区间足够大

* 使用 transform 插入元素时必须保证目标容器有足够的空间，通常会结合迭代器函数，尾部插入用 back_inserter，它返回的迭代器会调用 push_back，首部插入用 front_insert，它返回的迭代器会调用 push_front，任意位置插入用 inserter

```cpp
#include <algorithm>
#include <cassert>
#include <list>
#include <vector>

int main() {
  std::vector<int> v{1, 2, 3};
  std::vector<int> dst;
  std::transform(v.begin(), v.end(), std::back_inserter(dst),
                 [](int x) { return x + 1; });
  assert(dst == (std::vector<int>{2, 3, 4}));
  dst.reserve(dst.size() +
              v.size());  // 每次插入时仍有移动元素的开销，但避免了扩容开销
  std::transform(v.begin(), v.end(), std::inserter(dst, dst.begin() + 1),
                 [](int x) { return x + 4; });
  assert(dst == (std::vector<int>{2, 5, 6, 7, 3, 4}));
  std::list<int> dst2;
  std::transform(v.begin(), v.end(), std::front_inserter(dst2),
                 [](int x) { return x + 1; });
  assert(dst2 == (std::list<int>{4, 3, 2}));
  dst2.clear();
  std::transform(v.rbegin(), v.rend(), std::front_inserter(dst2),
                 [](int x) { return x + 1; });
  assert(dst2 == (std::list<int>{2, 3, 4}));
  std::transform(v.rbegin(), v.rend(), dst2.begin(),
                 [](int x) { return x + 1; });  // 已有足够空间，可直接覆盖
  assert(dst2 == (std::list<int>{4, 3, 2}));
}
```

## 31 sort 外的排序算法

* 选出部分最满足某个条件的元素，剩余元素不排序以节省开销

```cpp
#include <algorithm>
#include <cassert>
#include <vector>

int main() {
  std::vector<int> v{3, 2, 5, 2, 4, 1};
  std::partition(v.begin(), v.end(), [](int a) { return a == 2; });
  assert(v == (std::vector<int>{2, 2, 5, 3, 4, 1}));
  std::nth_element(v.begin(), v.begin() + 3, v.end(),
                   [](int a, int b) { return a < b; });
  assert(v == (std::vector<int>{2, 1, 2, 3, 4, 5}));
  std::partial_sort(v.begin(), v.begin() + 3, v.end(),
                    [](int a, int b) { return a > b; });
  assert(v == (std::vector<int>{5, 4, 3, 1, 2, 2}));
}
```

* partition、partial_sort、nth_element、sort 是不稳定排序，对相等元素排序不保证维持原有相对顺序。stable_partition、stable_sort 是稳定排序，partial_sort 和 nth_element 没有稳定排序版本
* partition 和 stable_partition 只需要双向迭代器，可以在任何标准序列迭代器上使用
* sort、stable_sort、partial_sort、nth_element 需要随机访问迭代器，只能用于 vector、string、deque 和数组，不能用于关联容器
* list 例外，自带稳定排序版本的 sort 成员函数

## 32 remove-erase 惯用法

* remove 并没有进行删除，而是移动所有不需要删除的元素位于区间首部，并返回最后一个不需要被删除的元素的下一个位置的迭代器，删除需要结合 erase

```cpp
#include <algorithm>
#include <cassert>
#include <vector>

int main() {
  std::vector<int> v{1, 2, 3, 4, 5, 2};
  auto it = std::remove_if(v.begin(), v.end(), [](int i) { return i == 2; });
  assert(v == (std::vector<int>{1, 3, 4, 5, 5, 2}));
  assert(it == v.end() - 2);
  v.erase(it, v.end());
  assert(v == (std::vector<int>{1, 3, 4, 5}));
}
```

## 33 提防在指针的容器上使用类似 remove 的算法

* 对于指针元素类型的容器，remove 时需要删除的指针位置可能被覆盖，导致内存泄漏，安全的做法是先遍历释放再删除或改用智能指针

```cpp
#include <algorithm>
#include <memory>
#include <vector>

struct A {
  bool is_valid;
};

void del_and_nullify_invalid(A* a) {
  if (a && !a->is_valid) {
    delete a;
    a = nullptr;
  }
}

int main() {
  std::vector<A*> v;
  std::for_each(v.begin(), v.end(), del_and_nullify_invalid);
  v.erase(std::remove_if(v.begin(), v.end(),
                         [](const auto& a) { return !a->is_valid; }),
          v.end());
  std::vector<std::shared_ptr<A>> v2;
  v2.erase(std::remove_if(v2.begin(), v2.end(),
                          [](const auto& a) { return !a->is_valid; }),
           v2.end());
}
```

## 34 只适用有序区间的算法

```cpp
#include <algorithm>
#include <cassert>
#include <vector>

int main() {
  std::vector<int> v{1, 2, 2, 3, 3, 4, 5, 7};
  // 二分查找
  assert(std::binary_search(v.begin(), v.end(), 2));
  assert(!std::binary_search(v.begin(), v.end(), 6));
  // lower_bound 返回第一个大于等于目标值的迭代器
  assert(std::lower_bound(v.begin(), v.end(), 2) == v.begin() + 1);
  assert(std::lower_bound(v.begin(), v.end(), 6) == v.end() - 1);
  assert(std::lower_bound(v.begin(), v.end(), 8) == v.end());
  // upper_bound 返回第一个大于目标值的迭代器
  assert(std::upper_bound(v.begin(), v.end(), 2) == v.begin() + 3);
  assert(std::upper_bound(v.begin(), v.end(), 6) == v.end() - 1);
  assert(std::upper_bound(v.begin(), v.end(), 8) == v.end());
  // equal_range 返回一对迭代器，即 lower_bound 和 upper_bound
  auto [l, r] = std::equal_range(v.begin(), v.end(), 2);
  assert(l != r);  // found
  assert(l == v.begin() + 1);
  assert(r == v.begin() + 3);
  // 线性复杂度合并
  std::vector<int> v2{2, 3, 4, 5, 6, 7};
  std::vector<int> dst;
  std::set_union(v.cbegin(), v.cend(), v2.cbegin(), v2.cend(),
                 std::back_inserter(dst));
  assert(dst == (std::vector<int>{1, 2, 2, 3, 3, 4, 5, 6, 7}));
  dst.clear();
  std::set_intersection(v.cbegin(), v.cend(), v2.cbegin(), v2.cend(),
                        std::back_inserter(dst));
  assert(dst == (std::vector<int>{2, 3, 4, 5, 7}));
  dst.clear();
  std::set_difference(v.cbegin(), v.cend(), v2.cbegin(), v2.cend(),
                      std::back_inserter(dst));
  assert(dst == (std::vector<int>{1, 2, 3}));
  dst.clear();
  std::set_symmetric_difference(v.cbegin(), v.cend(), v2.cbegin(), v2.cend(),
                                std::back_inserter(dst));
  assert(dst == (std::vector<int>{1, 2, 3, 6}));
  dst.clear();
  std::merge(v.cbegin(), v.cend(), v2.cbegin(), v2.cend(),
             std::back_inserter(dst));
  assert(dst == (std::vector<int>{1, 2, 2, 2, 3, 3, 3, 4, 4, 5, 5, 6, 7, 7}));
  dst = {3, 5, 4, 2, 1, 7, 6};
  std::inplace_merge(dst.begin(), dst.begin() + 3, dst.end());
  assert(dst == (std::vector<int>{2, 1, 3, 5, 4, 7, 6}));
  assert(!std::includes(v.begin(), v.end(), v2.begin(), v2.end()));
  dst = {1, 2, 2, 3};
  assert(std::includes(v.begin(), v.end(), dst.begin(), dst.end()));
  // unique 和 unique_copy 不要求但一般用于有序区间
  dst.erase(std::unique(dst.begin(), dst.end()), dst.end());
  assert(dst == (std::vector<int>{1, 2, 3}));
  dst.clear();
  std::unique_copy(v.begin(), v.end(), std::back_inserter(dst),
                   [](int a, int b) { return a == 2 && b == 2; });
  assert(dst == (std::vector<int>{1, 2, 3, 3, 4, 5, 7}));
}
```

## 35 通过 mismatch 或 lexicographical 比较实现简单的忽略大小写字符串比较

* 忽略大小写比较字符串

```cpp
#include <algorithm>
#include <cassert>
#include <cctype>
#include <iostream>
#include <string>

int main() {
  std::string s = "AbcDEFCba";
  auto [l, r] = std::mismatch(
      s.begin(), s.end(), s.rbegin(),
      [](char a, char b) { return std::tolower(a) == std::tolower(b); });
  assert(l - s.begin() == 3);
  assert(r - s.rbegin() == 3);
  assert((std::string{s.begin(), l}) == "Abc");
  assert((std::string{s.rbegin(), r}) == "abC");
  assert(std::lexicographical_compare(
      s.begin(), s.begin() + 3, s.rbegin(), s.rbegin() + 3,
      [](char a, char b) { return std::tolower(a) == std::tolower(b); }));
}
```

## 37 用 accumulate 或 for_each 来统计区间

```cpp
#include <algorithm>
#include <cassert>
#include <numeric>
#include <string>
#include <vector>

struct Point {
  Point(int x, int y) : x(x), y(y) {}
  bool operator==(const Point& rhs) const { return x == rhs.x && y == rhs.y; }
  int x;
  int y;
};

int main() {
  std::vector<int> v{2, 3, 4};
  assert(std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>()) == 24);
  std::vector<std::string> v2{"hello", "world"};
  assert(std::accumulate(v2.begin(), v2.end(), 0,
                         [](std::size_t so_far, const std::string& s) {
                           return so_far + s.size();
                         }) == 10);
  std::vector<Point> v3;
  v3.emplace_back(1, -2);
  v3.emplace_back(-2, 1);
  v3.emplace_back(2, -5);
  v3.emplace_back(1, 7);
  assert(std::accumulate(
             v3.begin(), v3.end(), Point{0, 0},
             [](const Point& so_far, const Point& p) {
               return Point{(so_far.x + p.x) / 2, (so_far.y + p.y) / 2};
             }) == (Point{0, 2}));
  Point point_sum{0, 0};
  int point_cnt = 0;
  std::for_each(v3.begin(), v3.end(), [&](const Point& p) {
    ++point_cnt;
    point_sum.x += p.x;
    point_sum.y += p.y;
  });
  point_sum.x /= point_cnt;
  point_sum.y /= point_cnt;
  assert(point_sum == (Point{0, 0}));
}
```

## 41 了解使用 mem_fn

```cpp
#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>
#include <vector>

struct A {
  void test() { assert(val == 0); }
  int val = 0;
};

int main() {
  std::vector<A> v(3);
  std::for_each(v.begin(), v.end(), std::mem_fn(&A::test));
  auto p = std::make_unique<A>();
  std::vector<A*> v2{p.get()};
  std::for_each(v2.begin(), v2.end(), std::mem_fn(&A::test));
}
```

## 44 用成员函数代替同名算法函数

* 部分容器有和 algorithm 库同名的成员函数，此时只用成员函数，一是二者行为不一定一致（如前文相等和等价的区别，如 list 的 remove、unique 真正删除了元素），而是成员函数专为容器设计开销更小，如关联容器的 count、find、lower_bound、upper_bound、equal_range，list 的 remove、remove_if、unique、sort、merge、reverse

```cpp
#include <algorithm>
#include <cassert>
#include <list>
#include <set>

int main() {
  std::set<int> s;
  s.find(42);                         // O(log n)
  std::find(s.begin(), s.end(), 42);  // O(n)
  std::list<int> l{1, 2, 3, 2};
  // list 成员函数原地操作 list 节点指针，algorithm 库函数会拷贝对象
  l.reverse();
  assert(l == (std::list<int>{2, 3, 2, 1}));
  l.sort();  // std::sort 不能用于 std::list
  assert(l == (std::list<int>{1, 2, 2, 3}));
  l.merge(std::list<int>{1, 3, 4, 5, 6});
  assert(l == (std::list<int>{1, 1, 2, 2, 3, 3, 4, 5, 6}));
  l.remove(1);
  assert(l == (std::list<int>{2, 2, 3, 3, 4, 5, 6}));
  l.unique();
  assert(l == (std::list<int>{2, 3, 4, 5, 6}));
  l.remove_if([](int a) { return a > 1; });
  assert(l.empty());
}
```

## STL algorithm

\* 标注为 C++11/14 引入

### Non-modifying sequence operations

| 函数名 | 功能 | 形式 |
|:-:|:-:|:-:|
|*all_of | 检查范围内是否所有元素满足给定条件，返回一个 bool|all_of (begin (v), end (v), f)|
|*any_of | 检查范围内是否存在元素满足给定条件，返回一个 bool|any_of (begin (v), end (v), f)|
|*none_of | 检查范围内是否所有元素不满足给定条件，返回一个 bool|none_of (begin (v), end (v), f)|
|for_each | 遍历范围内元素，传给 f 执行 | for_each (begin (v), end (v), f)|
|find | 查找范围内是否存在 30，若存在则返回第一个对应迭代器，否则返回尾迭代器 | find (begin (v), end (v), 30)|
|find_if | 查找范围内是否存在满足给定条件的元素，若存在则返回第一个对应迭代器，否则返回尾迭代器 | find_if (begin (v), end (v), f)|
|*find_if_not | 查找范围内是否存在不满足给定条件的元素，若存在则返回第一个对应迭代器，否则返回尾迭代器 | find_if_not (begin (v), end (v), f)|
|find_end | 范围 2 为范围 1 的最后的子序列，返回范围 2 首元素在范围 1 中的对应的迭代器，若未找到则返回范围 1 的尾迭代器，参数可以多一个自定义条件 f|find_end (begin (v), end (v), begin (sub), end (sub), f)|
|find_first_of | 范围 2 为范围 1 的第一个子序列，返回范围 2 首元素在范围 1 中的对应的迭代器，若未找到则返回范围 1 的尾迭代器，参数可以多一个自定义条件 f|find_first_of (begin (v), end (v), begin (sub), end (sub), f)|
|adjacent_find | 查找第一对相邻且相同的元素，返回后一个元素的迭代器，若未找到则返回尾迭代器，参数可以多一个自定义条件 f|adjacent_find (begin (v), end (v), f)|
|count | 返回值为范围内 30 出现的次数 | count (begin (v), end (v), 30)|
|count_if | 返回范围内满足给定条件的元素出现的次数 | count_if (begin (v), end (v), f)|
|mismatch | 返回范围内不匹配给定数组的两个不同的值组成的 pair，参数可以多一个自定义条件 f。如果都相同则内存溢出 | mismatch (begin (v), end (v), arr, f)|
|equal | 查找范围内的元素是否与数组完全相同，返回一个 bool，参数可以多一个自定义条件 f。如果元素数量不同则内存溢出 | equal (begin (v), end (v), arr, f)|
|*is_permutation | 查找范围内是否含有相同的元素，顺序和数量可以不同 | is_permutation (begin (v), end (v), arr)|
|search | 范围 2 为范围 1 的第一个子序列，返回范围 2 首元素在范围 1 中的对应的迭代器，若未找到则返回范围 1 的尾迭代器，参数可以多一个自定义条件 f|search (begin (v), end (v), begin (sub), end (sub), f)|
|search_n | 查找范围内出现 2 个连续的 30 的的第一个 30 对应的迭代器，未找到则返回尾迭代器，参数可以多一个自定义条件 f|search_n (begin (v), end (v), 2, 30, f)|

### Modifying sequence operations

| 函数名 | 功能 | 形式 |
|:-:|:-:|:-:|
|copy | 把范围 1 的内容拷贝到设置好 size 的 v 中 | copy (arr, arr+7, begin (v))|
|*copy_n | 把 arr 的 7 个元素拷贝到设置好 size 的 v 中 | copy_n (arr, 7, begin (v))|
|*copy_if | 把满足条件的 v 中元素拷贝到 b 中，返回目标范围的尾迭代器 | copy_if (begin (v), end (v), begin (b), f)|
|copy_backforward | 把范围内元素逆序从后往前拷贝 | copy_backward (begin (v), begin (v)+5, end (v))|
|*move | 把范围 1 的元素移动到 bar 中，或直接 move 整个容器 | move (begin (v), begin (v)+4, begin (bar));foo = std::move (bar)|
|*move_backforward | 把范围 1 的元素逆序从后往前移动 | move_backward (elems,elems+4,elems+5)|
|swap | 交换元素或容器的全部内容 | swap (x, y);swap (foo, bar)|
|swap_ranges | 将范围 1 内的元素和 bar 对应数量的元素交换 | swap_ranges (begin (foo)+1, end (foo)-1, begin (bar))|
|iter_swap | 交换两个迭代器的元素 | iter_swap (arr+1,begin (v)+2)|
|transform | 对 foo 所有元素进行 op_increase 操作，拷贝到 bar|transform (begin (foo), end (foo), begin (bar),  op_increase);transform (begin (foo), end (foo), begin (bar), begin (foo), std::plus<int>())|
|replace | 把范围内的 20 换成 99|replace (begin (v), end (v), 20, 99)|
|replace_if | 把范围内满足条件的元素换成 0|replace_if (begin (v), end (v), f, 0)|
|replace_copy | 把范围中的 20 以 99 拷贝到 v（范围元素不变）|replace_copy (arr, arr+8, begin (v), 20, 99)|
|replace_copy_if | 把范围内满足条件的元素以 0 拷贝到 bar|replace_copy_if (begin (foo), end (foo), begin (bar), f, 0)|
|fill | 把范围内元素换成 5|fill (begin (v),begin (v)+4,5)|
|fill_n | 把 4 个 20 拷贝到对应位置 | fill_n (begin (v)+3,4,20)|
|generate | 将 f 的执行结果拷贝到范围内 | generate (begin (v), end (v), f)|
|generate_n | 将 f 的执行结果拷贝到指定位置开始的 n 个元素中 | generate_n (arr, 9, f)|
|remove | 将范围内的 20 删除 | remove (begin (v), end (v), 20)|
|remove_if | 将范围内满足条件的元素删除 | remove_if (begin (v), end (v), f)|
|remove_copy | 将范围内的 20 删除拷贝到 v（范围元素不变）|remove_copy (arr,arr+8,begin (v),20)|
|remove_copy_if | 将范围内满足条件的元素删除拷贝到 v|remove_copy_if (arr,arr+9,begin (v),f)|
|unique | 去除重复元素，也可以自定义判断条件 | unique (begin (v), end (v));unique (begin (v), end (v),f)|
|unique_copy | 去除范围内的重复元素拷贝到 v，可以自定义判断条件 | unique_copy (arr,arr+9,begin (v));unique_copy (arr,arr+9,begin (v),f)|
|reverse | 反转范围内元素 | reverse (begin (v),end (v))|
|reverse_copy | 将反转的范围内元素拷贝到 v|reverse_copy (arr, arr+9, begin (v))|
|rotate | 交换 [first,middle) 和 [middle,last) 两个范围内元素 | rotate (begin (v),begin (v)+3,end (v))|
|rotate_copy | 将交换的结果拷贝到 v|rotate_copy (arr,arr+3,arr+7,begin (v))|
|random_shuffle | 随机打乱范围内元素位置 | random_shuffle (begin (v),end (v));random_shuffle (begin (v),end (v), f)|
|*shuffle | 用随机数引擎打乱范围内元素位置 | unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();shuffle (begin (v), end (v), std::default_random_engine (seed));|

### Partitions

| 函数名 | 功能 | 形式 |
|:-:|:-:|:-:|
|*is_partitioned | 检测范围是否按条件划分，返回一个 bool|is_partitioned (begin (v),end (v),f)|
|partition | 按条件将原容器划分为两个范围，返回划分边界（划分后第一个不满足条件位置）的迭代器，不保持元素初始相对位置 | partition (begin (v), end (v), f)|
|stable_partition | 按条件划分，返回划分边界的迭代器，保持初始位置 | stable_partition (begin (v), end (v), f)|
|*partition_copy | 范围内满足条件的划分到两个新容器内 | partition_copy (begin (v), end (v), begin (odd), begin (even), IsOdd)|
|*partition_point | 返回第一个不满足条件的迭代器，不存在则返回尾迭代器 | partition_point (begin (v),end (v),IsOdd)|

### Sorting

| 函数名 | 功能 | 形式 |
|:-:|:-:|:-:|
|sort | 排序，默认用 <排序，可以自定义条件 | sort (begin (v), end (v));sort (begin (v), end (v), f)|
|stable_sort | 稳定排序 | 同上 |
|partial_sort | 前 5 个最小的元素从小到大排序，剩下的未排序 | partial_sort (begin (v), begin (v)+5, end (v));partial_sort (begin (v), begin (v)+5, end (v), f)|
|partial_sort_copy | 把部分排序的结果拷贝到 v|partial_sort_copy (arr, arr+9, begin (v), end (v));partial_sort_copy (arr, arr+9, begin (v), end (v), f)|
|*is_sorted | 返回一个 bool 判断是否已排序 | std::is_sorted (begin (v), end (v), f)|
|*is_sorted_until | 返回第一个未排序的迭代器，若不存在则返回尾迭代器 | is_sorted_until (begin (v), end (v), f)|
|nth_element | 将某个元素放到下标为 5 的位置，不大于它的在前，不小于它的在后 | nth_element (begin (v), begin (v)+5, end (v));nth_element (begin (v), begin (v)+5, end (v), f)|

### Binary search

| 函数名 | 功能 | 形式 |
|:-:|:-:|:-:|
|lower_bound | 返回第一个大于等于 20 的元素的迭代器 | lower_bound (begin (v), end (v), 20)|
|upper_bound | 返回第一个大于 20 的元素的迭代器 | upper_bound (begin (v), end (v), 20)|
|equal_range | 返回一个 part，part.first 为 lower_bound，part.second 为 upper_bound，如果 f 为 > 则相反 | equal_range (begin (v), end (v), 20);equal_range (begin (v), end (v), 20, f)|
|binary_search | 二分查找，返回一个 bool|binary_search (begin (v), end (v), 3);binary_search (begin (v), end (v), 3, f)|

### Merge

| 函数名 | 功能 | 形式 |
|:-:|:-:|:-:|
|merge | 将两个有序序列合并到 v 并排序 | merge (first,first+5,second,second+5,begin (v))|
|inplace_merge | 连接在一起的序列 [first,middle) 和 [middle,last) 都已排序，将其归并为单一有序序列 | inplace_merge (begin (v),begin (v)+5,end (v))|
|includes | 返回一个 bool，判断 arr2 是否为 arr1 的子集 | includes (arr1,arr1+10,arr2,arr2+4);includes (arr1,arr1+10,arr2,arr2+4,f)|
|set_union | 将两个集合的并集拷贝到 v，返回并集元素数后一个迭代器 | set_union (first, first+5, second, second+5, begin (v))|
|set_intersection | 交集 | 同上 |
|set_difference | 范围 1 减去范围 2 的差集 | 同上 |
|set_symmetric_difference | 对称差 | 同上 |

### Heap

| 函数名 | 功能 | 形式 |
|:-:|:-:|:-:|
|push_heap | 向堆中加一个元素 | v.push_back (99);push_heap (begin (v),end (v))|
|pop_heap | 删除堆中最大元素 | pop_heap (begin (v),end (v))|
|make_heap | 用指定范围造一个堆 | make_heap (begin (v),end (v))|
|sort_heap | 堆排序 | sort_heap (begin (v),end (v))|
|*is_heap | 判断是否为二叉堆结构，返回一个 bool|is_heap (begin (v),end (v))|
|*is_heap_until | 返回第一个破坏二叉堆结构的迭代器 | is_heap_until (begin (v),end (v))|

### Min/max

| 函数名 | 功能 | 形式 |
|:-:|:-:|:-:|
|min | 返回较小元素 | min ('a','z')|
|max | 返回较大元素 | max (3.3, 2.5)|
|*minmax | 返回一个 pair，pair.first 为最小值，pair.second 为最大值 | auto result = minmax ({1,2,3,4,5})|
|min_element | 返回范围内最小值的迭代器 | min_element (myints,myints+7);min_element (myints,myints+7,f)|
|max_element | 返回范围内最大值的迭代器 | 同上 |
|*minmax_element | 返回一个 pair，pair.first 为最小值的迭代器，pair.second 为最大值的迭代器 | auto result = std::minmax_element (begin (v),end (v))|

### Other

| 函数名 | 功能 | 形式 |
|:-:|:-:|:-:|
|lexicographical_compare | 按字典序比较两个序列 | lexicographical_compare (foo,foo+5,bar,bar+9, compare)|
|next_permutation | 范围按字典序的下一个排列组合，若没有则返回 false|next_permutation (arr,arr+3)|
|prev_permutation | 范围按字典序的上一个排列组合，若没有则返回 false|prev_permutation (arr,arr+3)|

## STL numeric

### accumulate

* 累加指定范围内的元素

```cpp
template <class InputIterator, class T>
T accumulate(InputIterator first, InputIterator last, T init) {
  while (first != last) {
    // or: init = binary_op(init,*first) for the binary_op version
    init = init + *first;
    ++first;
  }
  return init;
}

// accumulate example
#include <cassert>
#include <functional>
#include <numeric>

int f(int x, int y) { return x + 2 * y; }

struct A {
  int operator()(int x, int y) { return x + 3 * y; }
};

int main() {
  int init = 100;
  int v[] = {10, 20, 30};
  assert(std::accumulate(v, v + 3, init) == 160);
  assert(std::accumulate(v, v + 3, init, std::minus<int>()) == 40);
  assert(std::accumulate(v, v + 3, init, f) == 220);
  assert(std::accumulate(v, v + 3, init, A{}) == 280);
}
```

### adjacent_difference

* 计算范围中相邻元素的差

```cpp
template <class InputIterator, class OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
                                   OutputIterator result) {
  if (first != last) {
    typename iterator_traits<InputIterator>::value_type val, prev;
    *result = prev = *first;
    while (++first != last) {
      val = *first;
      *++result = val - prev;  // or: *++result = binary_op(val,prev)
      prev = val;
    }
    ++result;
  }
  return result;
}

// adjacent_difference example
#include <cassert>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
  int val[] = {1, 2, 3, 5, 9, 11, 12};
  std::vector<int> res(7);
  std::adjacent_difference(val, val + 7, res.data());
  assert(res == (std::vector<int>{1, 1, 1, 2, 4, 2, 1}));
  std::adjacent_difference(val, val + 7, res.data(), std::multiplies<int>());
  assert(res == (std::vector<int>{1, 2, 6, 15, 45, 99, 132}));
  std::adjacent_difference(val, val + 7, res.data(),
                           [](int x, int y) { return x + y; });
  assert(res == (std::vector<int>{1, 3, 5, 8, 14, 20, 23}));
}
```

### inner_product

* 计算两个范围的内积

```cpp
template <class InputIterator1, class InputIterator2, class T>
T inner_product(InputIterator1 first1, InputIterator1 last1,
                InputIterator2 first2, T init) {
  while (first1 != last1) {
    init = init + (*first1) * (*first2);
    // or: init = binary_op1 (init, binary_op2(*first1,*first2));
    ++first1;
    ++first2;
  }
  return init;
}

// inner_product example
#include <cassert>
#include <functional>
#include <numeric>

int main() {
  int init = 100;
  int series1[] = {10, 20, 30};
  int series2[] = {1, 2, 3};

  assert(std::inner_product(series1, series1 + 3, series2, init) == 240);
  // 70 = 100 - 10 / 1 - 20 / 2 - 30 / 3
  assert(std::inner_product(series1, series1 + 3, series2, init,
                            std::minus<int>(), std::divides<int>()) == 70);
  // 34 = 100 - (10 + 1) - (20 + 2) - (30 + 3)
  assert(std::inner_product(
             series1, series1 + 3, series2, init,
             [](int x, int y) { return x - y; },
             [](int x, int y) { return x + y; }) == 34);
}
```

### partial_sum

* 计算范围中的元素的部分和

```cpp
template <class InputIterator, class OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last,
                           OutputIterator result) {
  if (first != last) {
    typename iterator_traits<InputIterator>::value_type val = *first;
    *result = val;
    while (++first != last) {
      val = val + *first;  // or: val = binary_op(val,*first)
      *++result = val;
    }
    ++result;
  }
  return result;
}

// partial_sum example
#include <cassert>
#include <functional>
#include <numeric>
#include <vector>

int main() {
  int val[] = {1, 2, 3, 4, 5};
  std::vector<int> res(5);
  std::partial_sum(val, val + 5, res.data());
  assert(res == (std::vector<int>{1, 3, 6, 10, 15}));
  std::partial_sum(val, val + 5, res.data(), std::multiplies<int>());
  assert(res == (std::vector<int>{1, 2, 6, 24, 120}));
  std::partial_sum(val, val + 5, res.data(),
                   [](int x, int y) { return x + y + 1; });
  assert(res == (std::vector<int>{1, 4, 8, 13, 19}));
}
```

### iota

* 用顺序递增的值赋值指定范围

```cpp
template <class ForwardIterator, class T>
void iota(ForwardIterator first, ForwardIterator last, T val) {
  while (first != last) {
    *first = val;
    ++first;
    ++val;
  }
}

// iota example
#include <cassert>
#include <numeric>

int main() {
  int numbers[10];
  int init = 100;
  std::iota(numbers, numbers + 10, init);
  for (auto x : numbers) {
    assert(x == init++);
  }
}
```

## std::string 查找函数

* 以下函数查找成功时返回所在位置的下标，失败返回 std::string::npos

### find

* 查找目标字符串第一次出现的位置

```cpp
// string
size_t find(const string& str, size_t pos = 0) const noexcept;
// c-string
size_t find(const char* s, size_t pos = 0) const;
// buffer
// 查找 s 的前 n 个字符
size_t find(const char* s, size_t pos, size_type n) const;
// character
size_t find(char c, size_t pos = 0) const noexcept;
```

* 例子

```cpp
#include <cassert>
#include <string>

int main() {
  std::string str("There are two needles in this haystack with needles.");
  std::string str2("needle");
  std::size_t pos = str.find(str2);
  assert(str.substr(0, pos) == "There are two ");
  pos = str.find("needles are small", pos + 1, 6);
  assert(str.substr(0, pos) == "There are two needles in this haystack with ");
  str.replace(str.find(str2), str2.length(), "preposition");
  assert(str == "There are two prepositions in this haystack with needles.");
}
```

### rfind

* 从后往前查找目标字符串第一次出现的位置

```cpp
// string
size_t rfind(const string& str, size_t pos = npos) const noexcept;
// c-string
size_t rfind(const char* s, size_t pos = npos) const;
// buffer
size_t rfind(const char* s, size_t pos, size_t n) const;
// character
size_t rfind(char c, size_t pos = npos) const noexcept;
```

* 例子

```cpp
#include <cassert>
#include <string>

int main() {
  std::string str("The sixth sick sheik's sixth sheep's sick.");
  std::string key("sixth");
  std::size_t pos = str.rfind(key);
  if (pos != std::string::npos) {
    str.replace(pos, key.length(), "seventh");
  }
  assert(str == "The sixth sick sheik's seventh sheep's sick.");
}
```

### find_first_of

* 查找目标字符串的任意字符第一次出现的位置

```cpp
// string
size_t find_first_of(const string& str, size_t pos = 0) const noexcept;
// c-string
size_t find_first_of(const char* s, size_t pos = 0) const;
// buffer
size_t find_first_of(const char* s, size_t pos, size_t n) const;
// character
size_t find_first_of(char c, size_t pos = 0) const noexcept;
```

* 例子

```cpp
#include <cassert>
#include <string>

int main() {
  std::string str("Please, replace the vowels in this sentence by asterisks.");
  size_t pos = str.find_first_of("aeiou");
  while (pos != std::string::npos) {
    str[pos] = '*';
    pos = str.find_first_of("aeiou", pos + 1);
  }
  assert(str == "Pl**s*, r*pl*c* th* v*w*ls *n th*s s*nt*nc* by *st*r*sks.");
}
```

### find_last_of

* 从后往前查找目标字符串的任意字符第一次出现的位置

```cpp
// string
size_t find_last_of(const string& str, size_t pos = npos) const noexcept;
// c-string
size_t find_last_of(const char* s, size_t pos = npos) const;
// buffer
size_t find_last_of(const char* s, size_t pos, size_t n) const;
// character
size_t find_last_of(char c, size_t pos = npos) const noexcept;
```

* 例子

```cpp
#include <cassert>
#include <string>
#include <vector>

std::vector<std::string> split_filename(const std::string& str) {
  std::size_t found = str.find_last_of("/\\");
  return std::vector<std::string>{str.substr(0, found), str.substr(found + 1)};
}

int main() {
  std::string str1("/usr/bin/man");
  std::string str2("C:\\Windows\\winhelp.exe");
  assert(split_filename(str1) == (std::vector<std::string>{"/usr/bin", "man"}));
  assert(split_filename(str2) ==
         (std::vector<std::string>{"C:\\Windows", "winhelp.exe"}));
}
```

### find_first_not_of

* 查找目标字符串的任意字符第一次未出现的位置

```cpp
// string
size_t find_first_not_of(const string& str, size_t pos = 0) const noexcept;
// c-string
size_t find_first_not_of(const char* s, size_t pos = 0) const;
// buffer
size_t find_first_not_of(const char* s, size_t pos, size_t n) const;
// character
size_t find_first_not_of(char c, size_t pos = 0) const noexcept;
```

* 例子

```cpp
#include <cassert>
#include <string>

int main() {
  std::string str("look for non-alphabetic characters");
  std::size_t pos = str.find_first_not_of("abcdefghijklmnopqrstuvwxyz ");
  assert(str.substr(0, pos) == "look for non");
}
```

### find_last_not_of

* 从后往前查找目标字符串的任意字符第一次未出现的位置

```cpp
// string
size_t find_last_not_of(const string& str, size_t pos = npos) const noexcept;
// c-string
size_t find_last_not_of(const char* s, size_t pos = npos) const;
// buffer
size_t find_last_not_of(const char* s, size_t pos, size_t n) const;
// character
size_t find_last_not_of(char c, size_t pos = npos) const noexcept;
```

* 例子

```cpp
#include <cassert>
#include <string>

int main() {
  std::string str("hello   \n");
  std::string str2(" \t\f\v\n\r");
  size_t pos = str.find_last_not_of(str2);
  if (pos != std::string::npos) {
    str.erase(pos + 1);
  }
  assert(str == "hello");
}
```
