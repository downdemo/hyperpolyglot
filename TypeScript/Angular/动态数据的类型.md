* 如果一个对象的每个属性都是元素类型相同的数组，该对象类型定义方式如下

```ts
class Line {
  x1: string;
  x2: string;
  y1: string;
  y2: string;
}

class Lines {
  [prop: string]: Line[];
}

let x: Lines = {
  redLines: [],
  yellowLines: [],
  blueLines: [],
}
```
