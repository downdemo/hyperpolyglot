## 变量类型

* 五种基本数据类型：Undefined、Null、Boolean、Number、String
* 一种复杂数据类型：Object
* Undefined 类型只有值 undefined

```js
let x;
console.log(x === undefined);  // true
console.log(y);  // 错误：未声明的变量只能 typeof
console.log(typeof y);  // undefined
console.log(typeof x);  // undefined
```

* Null 类型只有值 null

```js
let x = null;
console.log(typeof x);  // object（null 可以看作空对象指针）
console.log(null == undefined);  // true（== 会为了比较进行转换，而 undefined 派生自 null）
console.log(null === undefined);  // false
```

* Number

```js
const [mx, mn] = [Number.MAX_VALUE, Number.MIN_VALUE];  // 值上下限
const [inf, inf2] = [Infinity, -Infinity];  // 超出上下限的值会转为正负无穷
const [x, y, z] = [NaN, 1/0, 0/0];  // NaN Infinity NaN
console.log(x === z);  // false：任何 NaN 都不相等
Number.isNaN(x);  // true：用 Number.isNaN 检查 NaN，它对任何不能转为数值的值都返回 true
const [n1, n2] = [Number('42'), Number('x')];
Number.isNaN(n1);  // false
Number.isNaN(n2);  // true
```

* String

```js
const x = 42;
const s1 = x.toString();  // '42'
console.log(x == s1);  // true
console.log(x === s1);  // false
const s2 = x.toString(2);  // '101010'（参数表示进制）

const y = null;
const s3 = y.toString();  // 错误
const s4 = String(y);  // 'null'，因此转 String 最好用 String() 而不用 toString()

let z;  // undefined
const s5 = z.toString();  // 错误
const s6 = String(z);  // 'undefined'
```

* Boolean

```js
const a = '1' < 2;  // true（比较会先进行转换）
const b = Boolean(NaN);  // false
const c = Boolean(null);  // false
const d = Boolean(undefined);  // false
const e = 0;
const f = !!e;  // 转 Boolean 的最佳做法
```

* Object

```js
let o = new Object();
```

* ES6 引入了 Symbol，用于生成唯一的值

```js
let x = Symbol('A');
console.log(x);  // Symbol(A)
console.log(typeof x);  // symbol
let y = Symbol('A');
console.log(x === y);  // false
```

## 引用类型

### [Object](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object)

```js
let Person = new Object();
Person.age = 12;
Person.name = 'A';
for (let x in Person) {
  console.log(x);  // name age：依次显示 Person 的属性名，但顺序不可测
}
console.log('toString' in Person);  // true：检查属性是否在对象中（包括继承的属性）
console.log(Person.hasOwnProperty('name'));  // true：检查属性是否在对象中（不包括继承的属性）
console.log(Object.hasOwnProperty.call(Person, 'name'));  // true：这种写法比直接调用对象方法更清晰
delete Person.age;  // 也可以删除属性
console.log(Person.hasOwnProperty('age'));  // false

// 更常用的定义方式
let Person = {
  name : 'A',  // 注意这里是逗号
  'high school' : 'MIT',  // 属性含特殊字符必须加引号
  age : 12,  // 最后一行可以不要逗号，但加逗号更好（单行定义则不用）
  // 因为如果这行新增一个属性，容易忘记给上一行加上逗号
};

console.log(Person.name);  // 也可以用 Person['name'] 访问
console.log(Person['high school']);  // 对于带引号的属性只能用 [] 访问
```

* 所有对象都是 Object 的派生类，所以都包含以下 Object 的方法：[constructor](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/constructor)、[toString](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/toString)、[toLocaleString](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/toLocaleString)、[valueOf](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/valueOf)、[hasOwnProperty](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/hasOwnProperty)、[isPrototypeOf](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/isPrototypeOf)、[propertyIsEnumerable](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/propertyIsEnumerable)

```js
constructor: Function;
toString(): string;
toLocaleString(): string;
valueOf(): Object;  // 返回对象的原始值
hasOwnProperty(v: PropertyKey): boolean;  // 检查对象是否含有某属性
isPrototypeOf(v: Object): boolean;  // 检查对象是否在另一对象的原型链上
propertyIsEnumerable(v: PropertyKey): boolean;  // 检查给定属性能否用 for-in 枚举
```

### [Array](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array)

```js
const x = new Array(10);  // 10 个元素的数组
const y = new Array(0, 1, 2);  // 元素为 0、1、2 的数组
let z = [0, 1, 2];  // 元素为 0、1、2 的数组
console.log(z[3]);  // undefined：越界不会出错，而是返回 undefined
// 可以通过对越界元素赋值来添加元素
z[3] = 3;
console.log(z.length);  // 4
// 修改 Array 的 length 可以起到删除元素的效果
z.length = 0;
console.log(z[0]);  // undefined
// 添加元素后中间存在空位，值为 undefined
z[1] = 1;  // z 为 [, 1]
console.log(z[0]);  // undefined
```

* 定义空元素会有歧义

```js
const x = [0, 1, ];
const y = [0, , 2];
console.log(x.length);  // 2
console.log(y.length);  // 3
```

* Array 也是对象，索引被视为属性

```js
const x = ['h', 'e', 'l', 'l', 'o'];
for (let i in x) {  // for-in 遍历的是属性名
  console.log(i);  // 01234
  console.log(x[i]);  // hello
}

for (let i of x) {  // ES6 引入的 for-of 遍历的是元素
  console.log(i);  // hello
}
```

* 检查是否为数组

```js
const x = [];
console.log(Array.isArray(x));  // true
```

* 转换

```js
const x = ['red', 'blue'];
console.log(x);  // red,blue
console.log(x.join('|'));  // red|blue：返回用参数连接元素的字符串
// Array 派生自 Object，继承了 Object 的属性和方法
console.log(x.constructor === Array);  // true
console.log(x.toString());  // red,blue
console.log(x.valueOf());  // red,blue
```

* 首尾插入删除

```js
let x = [0];
x.push(1, 2);  // 元素为 012
let last = x.pop();  // 元素为 01
let first = x.shift();  // 元素为 1
// 如果想直接获取头尾元素，用索引访问，尾元素索引为 length - 1
x.unshift(3, 4, 5);  // 元素为 3451
```

* 查找索引

```js
let x = ['red', 'blue', 'black', 'blue'];
let firstPos = x.indexOf('blue');  // 1
let lastPos = x.lastIndexOf('blue');  // 3
let rmItem = x.splice(pos, 2);  // 删除并返回 pos 位置开始的 2 项

// splice 还可以接受额外的参数，表示接着在删除位置插入元素
let y = [0, 1, 2, 3, 4];
y.splice(2, 1, 5, 6);  // 元素为 015634
y.splice(0, 0, 7, 8, 9);  // 元素为 789015634
```

* 拷贝

```js
let x = ['red', 'blue', 'black', 'white'];
// 拷贝整个数组
const cpx = [...x];
// 拷贝区间
let y = x.slice(1, 3);  // 拷贝下标范围为 [1, 3) 的元素
x.slice(1, -1);  // 结果同上，-1 代表倒数第一个元素
x.slice(1, -5);  // 结果为空，-5 超出了首元素的位置
let z = x.slice(1);  // 拷贝范围为 [1, x.length)
// 连接数组
let res = x.concat(y, z, 42, 33);  // 参数可以是数组或元素
```

* 排序

```js
let x = [15, 10, 2, 1, 3];
x.sort();  // 元素转为字符串后排序
console.log(x);  // 1,10,15,2,3

// 如果希望按数值排序，则需要定义比较器并传给 sort
// 比较器对期望的情况返回负数
x.sort((a, b) => a - b);  // a < b 时即为升序，返回负数
console.log(x);  // 1,2,3,10,15
```

* 迭代方法

```js
let x = [0, 1, 2, 3, 4, 5];
// find 返回第一个满足条件的值
let tar = x.find(e => e % 2 === 1);
// findIndex 返回第一个满足条件的索引
let pos = x.findIndex(e => e % 2 === 1);
// forEach 对每个元素执行回调函数
x.forEach(e => console.log(e));
// filter 筛选所有满足条件的元素并返回数组
let y = x.filter(e => e % 2 === 0);
// map 对所有元素执行函数并返回数组
let z = x.map(e => e * 2);
// every 检查是否所有元素都满足条件
console.log(x.every(e => e < 3));  // false
// some 检查是否存在元素都满足条件
console.log(x.some(e => e < 3));  // true
// reduce 从左到右执行函数，并把上次返回值作为第一个参数传给下次执行的函数
// 另外可以指定一个额外的参数作为初始值
let sum = x.reduce((accumulator, curVal) => accumulator + curVal, 10);  // 10 + 0 + 1 + 2 + 3 + 4 + 5
// reduceRight 从右到左
let dif = x.reduceRight((accumulator, curVal) => accumulator - curVal, 15);  // 15 - 5 - 4 - 3 - 2 - 1
```

### [String](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/String)

* ES6 引入了模板字符串

```js
const name = 'downdemo';
// 模板字符串用反引号包含
const x = `hi, ${name}!`;  // hi, downdemo!
// 模板字符串可保留字符串中的空白符
console.log(`line1
line2
line3`);
```

* 索引

```js
'hello'.charAt(0);  // h
'hello'[0];  // h
'hello'.charCodeAt(0);  // 104（ASCII 码）
'hello'.indexOf('l');  // 2
'hello'.lastIndexOf('l');  // 3
```

* 子串

```js
const s = 'hello';
const s2 = s.slice(1, -2);  // ell：[1, length - 1) 的元素
// 另一个方法是 substring
const s3 = s.substring(1, 2);  // e：[1, 2) 的元素
const s4 = s.substring(2, 1);  // e：同上，右边小于左边则大小对调
const s5 = s.substring(1, -2);  // h：负数或 NaN 会被置为 0，即 [1, 0)，大小对调即 [0, 1)
const s6 = s.substring(1, s.length - 1);  // ell
const s7 = s.substring(0, 100);  // hello：超过 length 则忽略超出的部分
```

* 字符处理

```js
let s = '      heLLo  '.trim();  // 删除两侧空白字符
s.toUpperCase();  // HELLO
s.toLowerCase();  // hello
s.replace('L', 'l');  // helLo
s[0] = 'H';  // 不出错，但无效
'1|2|3|4|5'.split('|');  // [1, 2, 3, 4, 5]：按参数分割字符串并返回数组
```

* 结合正则表达式

```js
'hello'.replace(/l/gi, 'l');  // hello（g 为全局，i 为不区分大小写）
'1234 3 456 789'.match(/(\d){3}/g);  // [123,456,789]（返回匹配的结果，g 则返回所有匹配）
'1234 3 456 789'.search(/[^\d]/);  // 4（找到第一个不是数字的位置）
```

### [Map](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Map)

```js
let m = new Map();  // 也可以用元素为键值对的数组作为参数来初始化
// let m = new Map([ ['A', 100], ['B', 90] ]);
m.set('A', 100);  // 添加元素
m.set('B', 90);
m.set('B', 80);  // 已存在 key 则更新值
for (let x of m) {
  console.log(`${x[0]}:${x[1]}`);  // A: 100 B: 80
}
m.forEach((val, key, map) => { console.log(`${key}:${val}`); });

const y = new Array([...m]);  // ['A', 100, 'B', 80]：将 Map 转为 Array
m.get('A');  // 100：根据 key 读取值，如果找不到 key 则返回 undefined
m.has('A');  // true：检查是否存在 key
m.delete('A');  // true：根据 key 删除元素，成功则返回 true
m.has('A');  // false
m.clear();  // 删除所有元素
console.log(m.size);  // 0
```

* 合并 Map

```js
let merged = new Map([...m1, ...m2]);
```

### [Set](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Set)

```js
let s = new Set();  // 也可以用数组作为参数初始化
// let s = new Set(['A', 'B', 'C']);
s.add('A');  // 添加元素
s.add('B');
s.add('C');
for (let x of s) {
  console.log(x);  // ABC
}
s.forEach((val, idx, set) => { console.log(`${idx}:${val}`); });  // 0: A 1: B 2 : C

const y = [...s];  // ['A', 'B', 'C']：将 Set 转为 Array
s.delete('A');  // true
s.has('A');  // false
s.clear();
console.log(s.size);  // 0
```

* 其他操作

```js
const union = new Set([...s1, ...s2]);  // 并
const intersect = new Set([...s1].filter(x => s2.has(x)));  // 交
const difference = new Set([...s1].filter(x => !s2.has(x)));  // 差
```

### [JSON](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/JSON)

* 序列化与反序列化
```js
const Person = {
  name : 'A',
  age : 11,
  country : 'China',
}

const s = JSON.stringify(Person);  // 返回 JSON 字符串
const obj = JSON.parse(s);  // 字符串转为对象
console.log(obj.name);  // 'A'
```

* 第二个参数为数组则筛选要保留的属性（全保留用 null）

```js
const Person = {
  name : 'A',
  age : 11,
  country : 'China',
}

const s = JSON.stringify(Person, ['name', 'country'], '  ');  // 第三个参数是缩进，用于美化输出
console.log(s);
```

* 第二个参数也可以是函数，用于处理对象的每个键值对

```js
const Person = {
  name : 'A',
  age : 11,
  country : 'China',
}

function f(key, val) {
  if (typeof val === 'string') return val.toUpperCase();
  return val;
}
const s = JSON.stringify(Person, f);

function g(key, val) {
  if (key === 'age') return val + 10;
  return val;
}
const obj = JSON.parse(s, g);
```

* 也可以在对象中声明一个 toJSON 函数来决定行为

```js
const Person = {
  name : 'A',
  age : 11,
  country : 'China',
  toJSON: function() {
    return {
      'New Prop1' : this.name,
      'New Prop2' : this.age
    };
  }
}

console.log(JSON.stringify(Person));
```

### [Function](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Function)

* 传入的参数数量可以与函数的参数列表不同

```js
function f()  // 虽然参数数量为 0，但可以传进任意数量的参数 {
  console.log(arguments[0]);  // 可以用 arguments 访问参数
  console.log(arguments.length);  // 用 length 获取传进的参数数量
  console.log(f.length);  // 函数参数列表的参数数量
}

f(1, 2, 3);  // 130
f();  // undefined00
```

* ES6 引入了 rest 参数来获取额外参数

```js
function f(x, y, ...args) {
  console.log(args.length);  // rest 参数和 arguments 一样是数组
  console.log(args[0]);  // 获取第一个额外参数
  console.log(arguments[2]);  // 也能获取第一个额外参数，但不直观
}
```

* 不存在函数重载，后定义的函数会覆盖前者

```js
function f() { console.log(1); }
function f(x) { console.log(2); }
f();  // 2

// 覆盖的原因是，上述函数声明相当于
let f = function() { console.log(1); }
f = function(x) { console.log(2); }
```

* 作用域

```js
function f(x, y) {
  sum = x + y;  // sum 未声明，会被添加到全局作用域
  return sum;
}

let res = f(1, 2);
console.log(sum);  // 3：// 如果声明了 sum 则只能在函数内部访问，从而可以使这里报错
```

* [块作用域](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Control_flow_and_error_handling#Block_Statement)

```js
var x = 1; { var x = 2; }  // ES6 之前没有块作用域，这里 x 不会被销毁
console.log(x);  // 2

// 更隐蔽的情况
for (var i = 0; i < 10; ++i) {}
console.log(i);  // 10

// 用 ES6 引入的 let 即可避免此问题
for (let j = 0; j < 10; ++j) {}  // let 声明的是块级作用域变量
console.log(j);  // 错误：j 未定义
```

* this

```js
function f() {
  console.log(this.i);
}

let o = new Object();
o.i = 1;
o.f = f;
o.f();  // 1
```

* [call](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Function/call) 和 [apply](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Function/apply)

```js
function f(x, y) {
  return x + y;
}

function g(x, y) {
  return f.call(this, x, y);
}

function h(x, y) {
  return f.apply(this, [x, y]);  // apply 和 call 的区别是，apply 传入的是数组
   // return f.apply(this, arguments);  // 也可以这样写
}

console.log(g(1, 2));  // 3
console.log(h(1, 2));  // 3
```

* ES6 引入了箭头函数

```js
// 参数 => 函数体
let f = x => x * x;
// 等价于
let f = function(x) {
  return x * x;
}
```

* 不要使用函数声明的写法

```js
f();  // 1：函数 f 是全局的，可以在声明之前调用

function f() {
  console.log(1);
}
```

* 而应该使用函数表达式写法

```js
f();  // 在函数之前调用将报错

let f = ()=> { console.log(1); }
```

## 类

* 最原始的类创建方法是创建一个 Object 对象，再添加属性和方法
```js
let Person = new Object();
person.name = 'A';
person.age = 12;
person.f = function() { console.log('hi'); };
```

* 更好的写法是字面量

```js
let Person = {
  name : 'A',
  age: 12,
  f: function() { console.log('hi'); },
};
```

* 为了多次创建具有同样属性的对象，则需要使用工厂模式

```js
function createPerson(name, age) {
  let o = new Object();
  o.name = name;
  o.age = age;
  o.f = function() { console.log('hi'); }
  return o;
}

let p1 = createPerson('Any', 12);
let p2 = createPerson('Boom', 11);
```

* 进一步想知道对象类型，使用构造函数写法

```js
function Person(name, age) {
  this.name = name;
  this.age = age;
  this.f = function() { console.log('hi'); }
}

let p1 = new Person('Any', 6);
let p2 = new Person('Boom', 7);
console.log(p1 instanceof Person);  // true
console.log(p1.f === p2.f);  // false
```

* 构造函数的问题是，每次创建对象，都要在内部重新创建一个函数（函数也是对象），不同对象中的函数并不是同一对象，为此可以把函数定义转移到外部

```js
function Person(name, age) {
  this.name = name;
  this.age = age;
  this.f = f;
}

function f() { console.log('hi'); }
```

* 但这样为了一个类在全局作用域定义函数显然是不合理的，另一种让对象共享属性的方法是使用原型模式

```js
function Person() {}
Person.prototype.name = 'A';
Person.prototype.age = 12;
Person.prototype.f = function() { console.log('hi'); };

let p1 = new Person();
let p2 = new Person();
console.log(p1.constructor === Person);  // true
console.log(p1.f === p2.f);  // true
p2.name = 'B';  // 不想使用默认值则需要自行指定
console.log(p1.name);  // 'A'：实例中未找到该属性则使用原型中的属性
console.log(p2.name);  // 'B'：实例中找到该属性，原型中的同名属性被屏蔽
delete p2.name;  // 删除实例中的 name 属性
console.log(p2.name);  // 'A'
console.log(p2.hasOwnProperty('name'));  // false：删除后实例中不存在该属性
```

* 更简洁的原型语法

```js
function Person() {}
Person.prototype = {
  name : 'A',
  age : 12,
  f : function() { console.log('hi'); },
}

let p1 = new Person();
console.log(p1.constructor === Person);  // false
console.log(p1.constructor === Object);  // true
```

* 创建一个函数时会同时创建它的 prototype 对象，并自动获取 [prototype.constructor](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/constructor) 值

```js
function Person() {}
console.log(Person.prototype.constructor);  // Person
let p1 = new Person;
console.log(p1.constructor);  // Person
```

* 上述简洁的原型写法相当于重写了 prototype，于是丢失了类型，为此只需在重写时加上一个 constructor 属性

```js
function Person() {}
Person.prototype = {
  constructor : Person,
  name : 'A',
  age : 12,
  f : function() { console.log('hi'); },
}
```

* 这样会导致 prototype.constructor 的 [[Enumerable]] 被设为 true，如果不希望这样则可以用 [Object.defineProperty
](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/defineProperty) 设置

```js
function Person() {}
Person.prototype = {
  name : 'A',
  age : 12,
  f : function() { console.log('hi'); },
}

Object.defineProperty(Person.prototype, 'constructor', {
  enumerable : false,
  value : Person,
});

for (x in Person.prototype) {  // 不会枚举 Person.prototype.constructor
  console.log(x);
}
```

* 此外原型模式还有一个问题，虽然函数对象被所有实例共享（希望的），但数据属性也会被共享

```js
function Person() {}
Person.prototype = {
  friends : ['A', 'B'],
}

let p1 = new Person();
let p2 = new Person();
p1.friends.push('C');  // 只想改动 p1.friends
console.log(p2.friends);  // ABC：同时影响了 p2.friends
```

* 因此最终方案是结合使用构造函数和原型模式，这样可以只共享函数

```js
function Person(name, age) {
  this.name = name;
  this.age = age;
}

Person.prototype = {
  constructor : Person,
  f : function() { console.log('hi'); },
}
```

## 继承

### 原型链继承

```js
function A() {
  this.i = 1;
}

A.prototype.f = function() {
  return this.i;
}

function B() {
  this.j = 2;
}

// 将派生类的 prototype 对象设为基类实例
B.prototype = new A();
// 再为派生类添加额外的函数（可以重写基类中的函数）
B.prototype.g = function() {
  return this.j;
}
// 注意不能使用字面量的写法
// B.prototype= { ... };  // 这样会重写 B.prototype，B.constructor 为 Object

let x = new B();
console.log(x.f());  // 1：搜索实例 => 搜索 B.prototype => 搜索 A.prototype
console.log(x.constructor);  // A
// 检查实例与原型的关系
console.log(x instanceof Object);  // true
console.log(x instanceof A);  // true
console.log(x instanceof B);  // true
console.log(Object.prototype.isPrototypeOf(x));  // true
console.log(A.prototype.isPrototypeOf(x));  // true
console.log(B.prototype.isPrototypeOf(x));  // true
```

* 原型链继承的问题在于，创建派生类对象时无法给基类的构造函数传参，此外更关键的是，基类的引用类型数据会被派生类共享

```js
function A() {
  this.i = [0, 1, 2];
}

function B() {}
B.prototype = new A();

let x = new B();
x.i.push(3);

let y = new B();
console.log(y.i);  // 0123
```

### 借用构造函数（伪造对象、经典继承）

* 即在派生类构造函数中调用基类构造函数

```js
function A() {
  this.i = [0, 1, 2];
}

function B() {
  A.call(this);  // 继承了 A
}

let x = new B();
x.i.push(3);

let y = new B();
console.log(y.i);  // 012
```

* 使用借用构造函数从派生类向基类传参

```js
function A(name) {
  this.name = name;
}

function B() {
  A.call(this, 'Ace');
  this.age = 11;
}

let x = new B();
console.log(x.name);  // Ace
console.log(x.age);  // 11
```

* 借用构造函数的问题在于，只能在构造函数中定义方法，此外派生类也无法得到基类原型中定义的方法

### 组合继承（伪经典继承）

* 结合两者的优点，用原型链继承方法，用借用构造函数继承属性

```js
function A(name) {
  this.name = name;
  this.i = [0, 1, 2];
}

A.prototype.f = function() { console.log(this.name); }

function B(name, age) {
  A.call(this, name);  // 继承属性
  this.age = age;
}

B.prototype = new A();  // 继承方法
B.prototype.g = function() { console.log(this.age); }

let x = new B('Ann', 12);
x.i.push(3);
x.f();  // Ann
x.g();  // 12

let y =  new B('Boom', 11);
console.log(y.i); 012
y.f();  // Boom
y.g();  // 11
```

### 原型式继承

* 借助原型基于已有对象创建新对象，而又不用创建新类型

```js
// 首先实现一个函数
function object(o) {
  function F() {}
  F.prototype = o;
  return new F();
}

let x = {
  name : 'A',
  i : [0, 1, 2],
};

let y = object(x);
y.name = 'B';
y.i.push(3);

let z = object(x);
z.name = 'C';
z.i.push(4);

console.log(x.i);  // 01234：xyz 共享 i
```

* ES5 引入了 [Object.create()](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/create) 实现上面的函数，它还可以接受一个额外的参数

```js
let x = {
  name : 'A',
  i : [0, 1, 2],
};

let y = Object.create(x, {
  name : {
    value : 'B',
  },
});

console.log(y.name);  // B
```

* 原型式继承适用于只想创建一个类似的对象，而不想费力创建一个构造函数的情况。但注意，和原型模式一样，这种方法会共享引用类型值

### 寄生式继承

* 类似于工厂模式，创建一个封装继承过程的函数，在函数中为对象添加方法，最后返回对象

```js
function object(o) {
  function F() {}
  F.prototype = o;
  return new F();
}

function createNew(o) {
  let c = object(o);
  c.f = function() { console.log('hi'); }
  return c;
}

let x = {
  name : 'A',
  i : [0, 1, 2],
};

let y = createNew(x);
y.f();  // hi
```

### 寄生组合式继承

* 组合继承的问题是，任何情况下都需要调用两次构造函数

```js
function A(name) {
  this.name = name;
  this.i = [0, 1, 2];
}

A.prototype.f = function() { console.log(this.name); }

function B(name, age) {
  A.call(this, name);  // 第二次调用 A()
  this.age = age;
}

B.prototype = new A();  // 第一次调用 A()
B.prototype.constructor = B;
B.prototype.g = function() { console.log(this.age); }

let x = new B('Ann', 12);
x.i.push(3);
x.f();  // Ann
x.g();  // 12
```

* 寄生式组合继承，即通过借用构造函数继承属性，通过原型链继承方法

```js
/*
function object(o) {
  function F() {}
  F.prototype = o;
  return new F();
}

function inherit(child, parent) {
  let c = object(parent.prototype);
  c.constructor = child;
  child.prototype = c;
}
*/

// 即
function inherit(child, parent) {
  function F() {}
  F.prototype = parent.prototype;
  child.prototype = new F;
  child.prototype.constructor = child;
}

function A(name) {
  this.name = name;
  this.i = [0, 1, 2];
}

A.prototype.f = function() { console.log(this.name); }

function B(name, age) {
  A.call(this, name);
  this.age = age;
}

inherit(B, A);
B.prototype.g = function() { console.log(this.age); }
```

* 在 ES6 之前，寄生组合式继承是最理想的继承模式

### class

* ES6 引入了 class 关键字使得定义类更加方便

```js
class A {
  constructor(name) {  // 如果不显式定义则会默认添加一个空 constructor
    this.name = name;
  }
  
  f() { console.log(this.name); }  // 不要加 function 关键字
  static sayHi() { console.log('hi'); }  // 允许定义 static 函数
}
// 相当于 A.prototype = {...};

let x = new A('AAA');  // 不同于函数，class 必须用 new 创建，否则会出错
x.f();

// 使用 extends 关键字即可实现继承
class B extends A {
  constructor(name, age) {
    super(name);  // 用 super() 调用基类构造函数
    this.age = age;
  }
  
  g() { console.log(this.age); }
}

let y = new B('BBB', 12);
y.f();
y.g();
B.sayHi();  // 静态方法只能通过类调用，可以被继承

console.log(x.constructor === A);  // true
console.log(y.constructor === B);  // true
console.log(A.prototype.constructor === A);  // true
console.log(B.prototype.constructor === B);  // true
```

* class 允许 getter 和 setter，但由于其造成的副作用难以测试，一般不建议使用

```js
class A {
  constructor(name) {
    this.name = name;
  }
  
  get name() { return 'Cat'; }
  set name(val) { console.log(`set: ${val}`); } 
}

let x = new A('Tom');  // set: Tom（触发 setter）
x.name = 'Jerry';  // set: Jerry（触发 setter）
console.log(x.name);  // Cat（触发 getter）
```

* class 的属性名也可以是表达式

```js
let functionName = 'f';

class A {
  [functionName]() { console.log('hi'); }
}

let x = new A();
x.f();
```

## [DOM 操作](https://www.w3school.com.cn/js/js_htmldom_document.asp)

* [Document.getElementById](https://developer.mozilla.org/en-US/docs/Web/API/Document/getElementById)（id 唯一，因此可以定位唯一节点）

```html
<p id="x"></p>

<script>
document.getElementById('x').innerHTML = 'hi';
document.querySelector('#x').innerHTML = 'hi';
</script>
```

* [Document.getElementsByTagName](https://developer.mozilla.org/en-US/docs/Web/API/Document/getElementsByTagName)（返回一组节点）

```html
<p>1</p>
<p>2</p>

<script>
document.getElementsByTagName('p')[1].innerHTML = 'hi';
document.querySelectorAll('p')[1].innerHTML = 'hi';
</script>
```

* [Document.getElementsByClassName](https://developer.mozilla.org/en-US/docs/Web/API/Document/getElementsByClassName)（返回一组节点）

```html
<p class="x">1</p>
<p class="x">2</p>

<script>
document.getElementsByClassName('x')[1].innerHTML = 'hi';
document.querySelectorAll('.x')[1].innerHTML = 'hi';
</script>
```

* [ParentNode.children](https://developer.mozilla.org/en-US/docs/Web/API/ParentNode/children)

```html
<div id="x">
  <p class="y">1</p>
  <p class="z">2</p>
</div>

<script>
document.getElementById('x').children[1].innerHTML = 'hi';
document.querySelector('#x p.z').innerHTML = 'hi';
</script>
```

* [ParentNode.firstElementChild](https://developer.mozilla.org/en-US/docs/Web/API/ParentNode/firstElementChild)、[ParentNode.lastElementChild](https://developer.mozilla.org/en-US/docs/Web/API/ParentNode/lastElementChild)

```html
<div id="x">
  <p name="y">1</p>
  <p name="z">2</p>
</div>

<script>
document.getElementById('x').firstElementChild.innerHTML = 'hello';
document.getElementById('x').lastElementChild.innerHTML = 'world';
document.querySelector('#x p[name=z]').innerHTML = 'world';
</script>
```

* [Node.appendChild](https://developer.mozilla.org/en-US/docs/Web/API/Node/appendChild)

```html
<div id="x"></div>

<script>
let o = document.createElement('p');
o.id = 'y';
o.innerHTML = 'hi';
o.style.color = 'red';
document.getElementById('x').appendChild(o);
</script>
```

* [Node.insertBefore](https://developer.mozilla.org/en-US/docs/Web/API/Node/insertBefore)

```html
<div id="x">
  <p>1</p>
  <p>2</p>
</div>

<script>
let o = document.createElement('p');
o.innerHTML = 'hi';
let x = document.getElementById('x');
x.insertBefore(o, x.children[1]);
</script>
```

* [Node.removeChild](https://developer.mozilla.org/en-US/docs/Web/API/Node/removeChild)

```html
<div id="x">
  <p id="y">1</p>
  <p>2</p>
</div>

<script>
document.getElementById('x').removeChild(document.getElementById('y'));
</script>
```

* [Node.replaceChild](https://developer.mozilla.org/en-US/docs/Web/API/Node/replaceChild)

```html
<div id="x">
  <p id="y">1</p>
  <p>2</p>
</div>

<script>
let o = document.createElement('p');
o.innerHTML = 'hi';
document.getElementById('x').replaceChild(o, document.getElementById('y'));
</script>
```
