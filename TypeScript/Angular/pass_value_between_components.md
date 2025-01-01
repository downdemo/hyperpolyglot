## 父组件向子组件传值

* child.component.ts
```ts
export class childComponent implements OnInit {
  @Input() i: number;
}
```
* child.component.html
```html
<p>{{i}}</p>
```
* parent.component.ts
```ts
export class parentComponent implements OnInit {
  x: number = 1;
}
```

* parent.component.html
```html
<app-child [i]="x"></app-child>
```

* 上述子组件的 i 与父组件的 x 绑定，改变父组件 x 值即可同时改变子组件的 i 值

```ts
// parent.component.html
<button (click) = "f()">加1</button>
<app-child [i] = "x"></app-child>

// parent.component.ts
export class parentComponent implements OnInit {
  x: number = 1;
  f() { ++this.x; }
}
```

## 子组件使用 setter 监听父组件数据变化

* 现在希望在父组件数据变化时，子组件的类可以做一些额外的事
* child.component.ts

```ts
export class childComponent implements OnInit {
  private _i: number;
  @Input set i(i: number) {
    this._i = i;
    console.log('监听到数据变化');
    // 其他处理过程
  }
  get i(): number { return this._i; }
}
```

* child.component.html

```html
<p>{{i}}</p>
```

* parent.component.ts

```ts
export class parentComponent implements OnInit {
  x: number = 1;
  f() { ++this.x; }
}
```

* parent.component.html

```html
<button (click)="f()">加1</button>
<app-child [i]="x"></app-child>
```

## 子组件通过 ngOnChanges() 监听父组件数据变化

* 此方法只能用于监控 @Input 的数据
* child.component.ts
```ts
export class childComponent implements OnInit {
  @Input() i: number;
  
  ngOnChanges(changes: {[key: string]: SimpleChange})
  { // {[key: string]: SimpleChange} 表示用 string 类型值 key 为索引，得到 SimpleChange 类型
    for(let x in changes) { // x 为 changes 中的属性
      let p = changes[x];  // p 类型即为 SimpleChange，管理着 this.i 的变化情况
      let to = JSON.stringify(p.currentValue);
      if(p.isFirstChange()) {
        console.log(`属性${x}的值初始化为${to}`);
      } else {
        let from = JSON.stringify(p.previousValue);
        console.log(`属性${x}的值由${from}变为${to}`);  // 属性 i 的值由 ... 变为 ...
      }
    }
  }
}
```

* child.component.html

```html
<p>{{i}}</p>
```

* parent.component.ts

```ts
export class parentComponent implements OnInit {
  x: number = 1;
  f() { ++this.x; }
}
```

* parent.component.html

```html
<button (click)="f()">加1</button>
<app-child [i]="x"></app-child>
```

## 子组件向父组件传值

* child.component.ts

```ts
import { EventEmitter } from '@angular/core';  // 注意 EventEmitter 是从 @angular/core 导入
// VS Code 自动 import 的是
// import { EventEmitter } from 'events';
// 这样运行会出现 @Output not initialized 的错误

export class childComponent implements OnInit {
  i: number = 1;
  @Output() s = new EventEmitter();
  f() { this.s.emit(this.i); }
}
```

* child.component.html

```html
<button (click)="f()">向父组件传值</button>
```

* parent.component.ts

```ts
export class parentComponent implements OnInit {
  j: number = 0;
  g(x) { this.j = x; }
}
```

* parent.component.html 中绑定子组件中的事件和父组件中的响应函数

```html
<p>{{j}}</p>
<app-child (s)="g($event)"></app-child>
```

## 父组件通过模板引用变量获取子组件数据

* child.component.ts
```ts
export class childComponent implements OnInit {
  i: number = 1;
  f() {
    console.log(this.i);
  }
}
```

* parent.component.html

```html
<button (click)="x.f()">调用子组件函数</button>
<app-child #x></app-child>
```

## 父组件通过 @ViewChild 获取子组件数据

* 上一方法只能在 html 中调用子组件的函数，父组件的类（.ts）中无法操控子组件数据。现在要在.ts 中操控子组件数据
* child.component.ts

```ts
export class childComponent implements OnInit {
  i: number = 1;
}
```

* parent.component.html

```html
<button (click)="f()">显示子组件成员i</button>
<app-child></app-child>
```

* parent.component.ts

```ts
import { Component, OnInit, ViewChild } from '@angular/core';
import { childComponent } from 'src/app/child/child.component';

export class ParentComponent implements OnInit {
  @ViewChild(childComponent) private x : childComponent;

  f() {
    console.log(this.x.i);
  }
}
```

* 如果有重复使用的构件，可以用模板引用变量来区分

```html
<app-child #x></app-child>
<app-child #y></app-child>
```

* 在.ts 中通过模板引用变量区分不同的子组件

```ts
export class ParentComponent implements OnInit {
  @ViewChild('x') private x : childComponent;
  @ViewChild('y') private y : childComponent;
  f() {
    console.log(this.x.i);
    console.log(this.y.i);
  }
}
```

## 通过 service 传值

* 这种方式将 service 视为中介，因此不要求组件有父子关系，任意组件之间均可传值
* 首先创建一个 service

```ts
ng generate service data
```

* 在 data.service.ts 中设置数据，该数据可被所有使用此 service 的组件共享

```ts
export class DataService {
  i = 1;
}
```

* 添加到 app.module.ts

```ts
import { DataService } from '../data.service';

NgModule({
  ...
  providers: [
    ...
    DataService,
  ],
  ...
```

* a.component.ts

```ts
import { DataService } from '../data.service';

export class AComponent implements OnInit {
  constructor(private data: DataService) { }
}

f() {
  ++this.data.i;
}
```

* a.html

```html
<p>{{data.i}}</p>
<button (click)="f()">加1</button>
```

* b.component.ts

```ts
import { DataService } from '../data.service';

export class BComponent implements OnInit {
  constructor(private data: DataService) { }
}

f() {
  --this.data.i;
}
```

* b.html

```html
<p>{{data.i}}</p>
<button (click)="f()">减1</button>
```

## 通过 service 的 EventEmitter

* service 只提供共享的 EventEmitter，数据借此直接在组件间传输而无需存在于 service 中
* data.service.ts

```ts
import { EventEmitter } from '@angular/core';

export class DataService {
  e = new EventEmitter();
}
```

* a.component.ts

```ts
import { DataService } from '../data.service';

export class AComponent implements OnInit {
  constructor(private data: DataService) { }
}

i = 1;
f() {
  this.data.e.emit(this.i);
}
```

* a.html

```html
<button (click)="f()">发送数据</button>
```

* b.component.ts

```ts
import { DataService } from '../data.service';

export class BComponent implements OnInit {
  constructor(private data: DataService) { }
  ngOnInit() {
    this.data.e.subscribe(
      (data) => { console.log(data); }
    ); 
  }
}
```
