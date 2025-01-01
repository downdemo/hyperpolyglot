## 通过 DOM 获取

```html
<input type="file" id="x">
```

* [HTMLInputElement](https://developer.mozilla.org/en-US/docs/Web/API/HTMLInputElement)

```ts
fileElem = document.getElementById("x")  as HTMLInputElement;  // 获取 input 元素
// 因为 HTMLElement 没有 files 属性，所以要断言为 HTMLInputElement
fileArray: FileList = fileElem.files;  // 文件数组
fileObject1: File = fileArray[0];  // 文件对象
console.log(fileArray.length);  // 文件个数
```

* [类型断言](https://ts.xcatliu.com/basics/type-assertion) 不是类型转换，用于手动指定一个值的类型，语法为

```ts
// <类型> 值
fileElem = <HTMLInputElement>document.getElementById("x");
// 或 值 as 类型
fileElem = document.getElementById("x")  as HTMLInputElement;
```

## 通过事件获取

* .html

```html
<input type="file" (change)="f($event)">
```

* .ts

```ts
fileArray: FileList;

f(e) {
  this.fileArray = e.target.files;
}
```
