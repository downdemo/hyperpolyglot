* [contenteditable](https://developer.mozilla.org/en-US/docs/Web/HTML/Global_attributes/contenteditable)

```html
<div id="x" contenteditable="true" style="height: 200px; width: 400px; border: 1px solid black; overflow:auto;">
  <span style="color: red">red</span>
  <span style="color: blue">blue</span>
</div>
```

* 动态添加文本

```ts
let obj = document.getElementById('x');
for(let i = 0; i < 100; ++i) {
  obj.scrollTop = obj.scrollHeight;  // 自动滚动到最下方
  let tmp = document.createElement('span');
  tmp.innerText = i.toString();
  tmp.style.color = 'red';
  obj.appendChild(tmp);
  obj.appendChild(document.createElement('br'));  // 换行
}
```
