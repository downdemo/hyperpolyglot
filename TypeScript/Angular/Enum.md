```ts
enum color { 'red' , 'blue', 'white' };
let x: string = color[1];
let y: number = color['blue'];
let z: color = color['blue'];  // 实际 typeof z 还是 number
console.log(x);  // blue
console.log(y);  // 1
console.log(z);  // 1
```

* enum 原理

```ts
enum color { 'red' , 'blue', 'white' };

// 编译为 JS 后的代码
var color;
(function(color) {
	color[color['red'] = 0] = 'red';
	color[color['blue'] = 1] = 'blue';
	color[color['white'] = 2] = 'white';
}) (color || color = {}))

// 即
var color = {};
color['red'] = 0;
color['blue'] = 1;
color['white'] = 2;
color[0] = 'red';
color[1] = 'blue';
color[2] = 'white';
```

* 数组转 enum

```ts
let X = ['red' , 'blue', 'white'];

var A;
(function (A) {
  X.forEach((value, index) => {
    A[A[value] = index] = value;
  })
})(A || (A = {}));

// 相当于
enum A { 'red' , 'blue', 'white' };
```

* 封装为函数

```ts
interface EnumType {
  [index: string]: number | string;
}

function makeEnum(arr: string[]): EnumType {
  const res: EnumType = {};
  arr.forEach((value, index) => {
    res[index] = value;
    res[value] = index;
  });
  Object.freeze(res);
  return res;
}
```
