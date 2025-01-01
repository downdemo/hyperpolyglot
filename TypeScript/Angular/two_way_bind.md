## 使用模板引用变量

```html
<input type="text" value = "{{i}}" #x (keyup) = "f(x.value)">
```

* .ts

```ts
f(v) {
  this.i = v;
}
```

## 使用 [(ngModel)]

* 先在 app.module.ts 中导入 FormsModule，否则会报错

```ts
Template parse errors:
Can't bind to 'ngModel' since it isn't a known property of 'input'.
```

* app.module.ts

```ts
import { FormsModule } from '@angular/forms';

@NgModule({
  ...
  imports: [
    ...
    FormsModule,
  ],
  ...
})
```

* .html

```html
<input [(ngModel)] = "i">
```

* .ts

```ts
this.i = 1;
```

## 在 form 下使用 [(ngModel)]

* 在 formGroup 或 form 下使用 [(ngModel)] 会报错

```ts
ERROR Error:
  ngModel cannot be used to register form controls with a parent formGroup directive.  Try using
  formGroup's partner directive "formControlName" instead.  Example:


<div [formGroup]="myGroup">
  <input formControlName="firstName">
</div>

In your class:

this.myGroup = new FormGroup({
   firstName: new FormControl()
});

  Or, if you'd like to avoid registering this form control, indicate that it's standalone in ngModelOptions:

  Example:


<div [formGroup]="myGroup">
  <input formControlName="firstName">
  <input [(ngModel)]="showMoreControls" [ngModelOptions]="{standalone: true}">
</div>
```

* 或报错

```ts
ERROR Error: If ngModel is used within a form tag, either the name attribute must be set or the form
      control must be defined as 'standalone' in ngModelOptions.

      Example 1: <input [(ngModel)]="person.firstName" name="first">
      Example 2: <input [(ngModel)]="person.firstName" [ngModelOptions]="{standalone: true}">
```

* 报错中已经给出了解决方法，加上 `[ngModelOptions] ="{standalone: true}"`

```html
<form>
  <input [(ngModel)] = "i" [ngModelOptions] = "{standalone: true}">
</form>
```

* 或者指定 formControlName

```html
<form [formGroup] = "myForm">
  <input [(ngModel)] = "i" formControlName = "x">
</form>

// .ts
this.myForm = new FormGroup({
   x: new FormControl()
});
```

* 但不应该在 formGroup 下使用 [(ngModel)]，formGroup 是响应式表单，其中数据应该通过 FormControl 管理
