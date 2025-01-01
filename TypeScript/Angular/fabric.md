* [fabric doc](http://fabricjs.com/docs/)

## [事件](https://fabricjs.com/docs/events/)

```ts
canvas.on('mousewheel', e => {} );  // 滚轮事件
text.on('scaling', e => {} );  // 缩放过程中
text.on('scaled', e => {} );  // 缩放完成
text.scale(0.5);  // 缩放为原来的一半
```

* 如果想取消事件，则应把函数单独写出来

```ts
i: number = 0;

f = function(e) {
  ++this.i;
}.bind(this);  // 必须 bind，传递给 on 和 off 时，this 才能指代当前类
// 如果不 bind，this.i 中的 i 为 undefined

this.canvas.on('mouse:dblclick', this.f);
this.canvas.off('mouse:dblclick', this.f);
```

## [fabric.Canvas](https://fabricjs.com/api/classes/canvas/)

```ts
const e = this.thisElem.nativeElement.querySelector('#canvas');
let canvas = new fabric.Canvas(e);
const oldState = canvas.viewportTransform;  // 默认值为 [1, 0, 0, 1, 0, 0]
canvas.relativePan(new fabric.Point(100, 200));  // 右移 100 下移 200
canvas.setZoom(0.5);  // 缩放为原来的一半
canvas.setViewportTransform(oldState);  // 恢复原有视角
```
