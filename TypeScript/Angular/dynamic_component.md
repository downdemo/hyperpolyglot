* `CatComponet` 组件中有一个按钮，现在希望点击按钮时创建一个新组件

```html
<button type="button" (click)="f()">创建</button>
<ng-template #x></ng-template>
```

* 假设要显示的组件是 `DogComponent`，其内容为

```html
{{tips}}
```

* 则 `CatComponet` 实现如下

```ts
import { Component, OnInit, ViewChild, ComponentFactoryResolver, ViewContainerRef, ComponentFactory, ComponentRef } from '@angular/core';
import { DogComponent } from '../dog/dog.component';

@Component({
  selector: 'app-cat',
  templateUrl: './cat.component.html',
  styleUrls: ['./cat.component.css']
})
export class CatComponent implements OnInit {
  @ViewChild('x', { read: ViewContainerRef }) x: ViewContainerRef;
  constructor(private componentFactoryResolver: ComponentFactoryResolver) { }

  ngOnInit(): void {
  }

  f() {
    const container: ViewContainerRef = this.x;
    container.clear();

    const fac: ComponentFactory<DogComponent> = this.componentFactoryResolver.resolveComponentFactory(DogComponent);
    const componentRef: ComponentRef<DogComponent> = container.createComponent(fac);
    componentRef.instance.tips = 'hello world';
  }
}
```

* 创建过程可以抽成一个函数

```ts
f() {
  const container: ViewContainerRef = this.x;
  const componentRef: ComponentRef<DogComponent> = this.createComponent(container, DogComponent);
  componentRef.instance.tips = 'hello world';
}

createComponent<T>(container: ViewContainerRef, component: Type<T>): ComponentRef<T> {
  container.clear();
  const fac: ComponentFactory<T> = this.componentFactoryResolver.resolveComponentFactory<T>(component);
  return container.createComponent(fac);
}
```

* 如果出现如下报错，则要把 `DogComponent` 添加到 `app.module.ts` 的 `entryComponents` 中。因为 `DogComponent` 未在模板中直接引用，而是动态加载，最初不会被编译，所以应在 `entryComponents` 中声明

```
No component factory found for DogComponent. Did you add it to @NgModule.entryComponents?
```
