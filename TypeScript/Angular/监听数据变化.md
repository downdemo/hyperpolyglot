* 利用 ngDoCheck 检查数据

```ts
i: number = 1;

ngDoCheck() {
  if(this.i !== 1) {
    console.log('数据发生变化');
    // 进行某些操作
  }
}
```

* 这可以用于组件间的互动，比起 emit 更简单。比如在一个共享的 service 中设置一个值作为标记，组件一可修改标记值，组件二检测到值的改变则进行某种操作

```ts
// x.service.ts
i = false;

// a.component.ts
f() {
  this.service.i = true;
}

// b.component.ts
ngDoCheck() { // 注意：i 是 true 再赋值为 true 也会触发 ngDoCheck
  if(this.service.i) {
    // 要进行的操作
    this.service.i = false;
  }
}
```

* 但如果有多个组件，只用一个标记是不行的

```ts
// x.service.ts
i = false;

// a.component.ts
f() {
  this.service.i = true;
}

// b.component.ts
ngDoCheck() {
  if(this.service.i) {
    // 要进行的操作
    this.service.i = false;
  }
}

// c.component.ts
ngDoCheck() {
  if(this.service.i) { // a 中改为 true，但 b 中改为 false，结果未执行此处操作
    // 要进行的操作
    this.service.i = false;
  }
}
```

* 一个简单的解决方法是对每个组件使用不同的标记，以确保每个标记只被两个组件来回改动

```ts
// x.service.ts
i = false;
j = false;

// a.component.ts
f() {
  this.service.i = true;
  this.service.j = true;
}

// b.component.ts
ngDoCheck() {
  if(this.service.i) {
    // 要进行的操作
    this.service.i = false;
  }
}

// c.component.ts
ngDoCheck() {
  if(this.service.i) {
    // 要进行的操作
    this.service.j = false;
  }
}
```

* 一种错误的想法是只在某个组件中改回标记值

```ts
// x.service.ts
i = false;

// a.component.ts
f() {
  this.service.i = true;
}

// b.component.ts
ngDoCheck() {
  if(this.service.i) {
    // 要进行的操作
  }
}

// c.component.ts
ngDoCheck() {
  if(this.service.i) { // b中不改为false以确保执行此处
    // 要进行的操作
    this.service.i = false;
  }
}
```

* 这种做法的错误之处在于，其他值（而不仅限于要检测的标记值）改变时也会触发 ngDoCheck，而此时标记值为 true，于是导致一旦其他值改变就执行目标操作
* 另一种错误的想法是改用 number 作标记

```ts
// x.service.ts
i = 0;

// a.component.ts
f() {
  ++this.service.i;
}

// b.component.ts
ngDoCheck() {
  if(this.service.i === 1) {
    // 要进行的操作
    ++this.service.i;
  }
}

// c.component.ts
ngDoCheck() {
  if(this.service.i === 2) {
    // 要进行的操作
    this.service.i = 0;
  }
}
```

* 这种错误在于，c 的 ngDoCheck 可能先执行，于是没有执行 c 中的操作，随后轮到 b，此时即使增加 i 也不会再执行 c 的 ngDoCheck。这种顺序是不可预估的
