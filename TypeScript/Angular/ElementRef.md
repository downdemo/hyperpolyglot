* document 是整个项目的所有 html 模板（可能存在同名 id），使用 ElementRef 可以限定在当前的组件中

```ts
import { ElementRef } from '@angular/core';

export class AComponent implements OnInit {
  constructor(private e: ElementRef) {}
  const o = this.e.nativeElement.getElementById('#x');  // document.getElementById('#x');
}
```
