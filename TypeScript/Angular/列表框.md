* 使用 [select multiple](https://www.w3school.com.cn/tags/att_select_multiple.asp) 实现一个能选中行的列表框
* 要对当前选中的行进行添加删除操作

```html
<select multiple="multiple" size="5" id="ListBox">
  <option *ngFor="let p of arr">{{p}}</option>
</select>
<button type="button" (click)="add()">添加</button>
<button type="button" (click)="remove()">删除</button>
```

* .ts

```ts
import { ElementRef } from '@angular/core';

arr: string[] = [];
constructor(private thisElement: ElementRef) { }

add() {
  this.arr.push('new item');
}

remove() {
  let o = this.thisElement.nativeElement.querySelector('#ListBox') as HTMLSelectElement;
  if (o.selectedIndex !== -1) { // 未选列表框中任何项时为-1
    console.log('要删除的项为', o.value);
    this.arr.splice(o.selectedIndex, 1);
  }
}
```
