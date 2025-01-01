* 详见 [Getting Started with Angular Material](https://material.angular.io/guide/getting-started)
* 执行命令

```sh
ng add @angular/material
```

* 比如要使用 `mat-select`，在 `app.module.ts` 中加入下列语句

```ts
import { MatSelectModule } from '@angular/material';

@NgModule({
  declarations: ...
  imports: ...
    MatSelectModule, // 在import属性中添加该声明
```

* 之后即可在 html 中使用 `mat-select` 标签
