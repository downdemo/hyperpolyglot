* 利用 [localStorage](https://developer.mozilla.org/en-US/docs/Web/API/Window/localStorage) 保存全局变量

```ts
// a.component.ts
export class AComponent {
  constructor() { localStorage.setItem('userId', '123'); }
}

// b.component.ts
export class BComponent {
  userId: string = localStorage.getItem('userId');
}
```

* 删除操作

```ts
localStorage.removeItem('userId');  // 移除某个元素
localStorage.clear();  // 清空
```
