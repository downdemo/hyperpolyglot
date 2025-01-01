## [FormControl](https://angular.io/api/forms/FormControl)

* .html

```html
<input type="text" [formControl]="name">
```

* .ts

```ts
export class AComponent implements OnInit {
  name = new FormControl('');
}
```

* 运行将报错

```
Can't bind to 'formControl' since it isn't a known property of 'input'
```

* 原因是使用 FormControl 还需要导入 ReactiveFormsModule

```ts
// app.module.ts
import { ReactiveFormsModule } from '@angular/forms';

@NgModule({
  ...
  imports: [
    ...
    ReactiveFormsModule,
  ],
  ...
```

* 运行成功后，文本框的值会同步到 FormControl，通过 value 属性即可获取值，通过 setValue 即可修改值

```ts
// .html
<input type="text" [formControl]="name">
<button (click)="f()">click</button>
{{name.value}}

// .ts
export class AComponent implements OnInit {
  name = new FormControl('');
  f() {
    this.name.setValue('123');
  }
}
```

* 通过 valueChanges 可以监听值的变化

```ts
export class AComponent implements OnInit {
  name = new FormControl('');
  ngOnInit() {
    this.name.valueChanges.subscribe((data)=> { console.log(data); });
  }
}
```

## [FormGroup](https://angular.io/api/forms/FormGroup)

* FormGroup 可以管理一组 FormControl
* .html
```html
<form [formGroup]="myForm">
  <label>name:<input type="text" formControlName="name"></label>
  <label>age:<input type="text" formControlName="age"></label>
</form>
```

* .ts

```ts
export class AComponent implements OnInit {
  myForm = new FormGroup(
    {
      name: new FormControl(''),
      age: new FormControl(''),
    }
  )
}
```

* FormGroup 可以嵌套

```ts
// .ts
export class AComponent implements OnInit {
  myForm = new FormGroup({
    name: new FormGroup({
      firstName: new FormControl(''),
      lastName: new FormControl(''),
    }),
    age: new FormControl(''),
  })

  f() {
    this.myForm.patchValue({ // patchValue 可以批量修改值
      name: {
        firstName: 'a',
        // lastName: 'b', // 不指定的值则不会被修改
      },
      age: '12',
    });
  }
}

// .html
<form [formGroup]="myForm">
  <div formGroupName="name">
    <label>fisrtName:<input type="text" formControlName="firstName"></label>
    <label>lastName:<input type="text" formControlName="lastName"></label>
  </div>
  <label>age:<input type="text" formControlName="age"></label>
</form>

<button (click)="f()">click</button>
```

## [FormBuilder](https://angular.io/api/forms/FormBuilder)

* 创建 FormGroup 时，重复 new 同样类型的对象十分繁琐，使用 FormBuilder 可以简化这个操作

```ts
import { FormBuilder, FormGroup } from '@angular/forms';

export class AComponent implements OnInit {
  constructor(private fb: FormBuilder) {}

  myForm = this.fb.group({ // myForm = new FormGroup
    name: this.fb.group({ // name: new FormGroup
      firstName:[''], // firstName: new FormControl('')
      lastName: [''],
    }),
    age: [''], // age: new FormControl('')
  })
}
```

## [Validators](https://angular.io/api/forms/Validators)

* Validators 提供了一系列内置的验证器，可添加到 FormControl 中

```ts
export class AComponent implements OnInit {
  constructor(private fb: FormBuilder) {}
  
  myForm = this.fb.group({
    name: this.fb.group({
      firstName: [''],
      lastName: [''],
    }),
    age: ['', Validators.required], // required 表示此处不能为空
  })
}
```

* 不满足验证器要求时，FormControl 的 status 属性为 INVALID
* 若 FormGroup 中存在 status 属性为 INVALID 的 FormControl，则 FormGroup 的 status 也为 INVALID
* 如果禁用 FormControl 中的 input text，不应该直接在 html 的标签中设置 disabled 属性

```html
<form [formGroup]="myForm">
  <input type="text" formControlName="age" disabled="disabled">
</form>
```

* 而应该在 formControl 中设置

```ts
export class AComponent implements OnInit {
  constructor(private fb: FormBuilder) {}
  
  myForm = this.fb.group({
    age: [{ value: '', disabled: true }, Validators.required],
  })
}
```

* 动态禁用

```ts
export function setFormControlDisabled(form: FormGroup, name: string) {
  form.get(name).reset({ value: form.get(name).value, disabled: true });
}

export function cancelFormControlDisabled(form: FormGroup, name: string) {
  form.get(name).reset({ value: form.get(name).value, disabled: false });
}
```

## [FormArray](https://angular.io/api/forms/FormArray)

* 如果不需要指定 FormControl 的名称，可以使用 FormArray 替代 FormGroup

```ts
export class AComponent implements OnInit {
  constructor(private fb: FormBuilder) {}

  myForm = this.fb.group({
    name: this.fb.array([ // group 改为 array，{} 改为 []
      this.fb.control(''), // 原为 firstName:['']
      this.fb.control(''), 
    ]),
    age: ['', Validators.required],
  })
}
```

* FormArray 中的 formControlName 是匿名的，显示方式改为指定索引

```html
<form [formGroup]="myForm">
  <div formArrayName="name">
    <label>fisrtName:<input type="text" formControlName="0"></label>
    <label>lastName:<input type="text" formControlName="1"></label>
  </div>
  <label>age:<input type="text" formControlName="age"></label>
</form>
```

* 对 FormArray 中的 FormControl 同样也可以使用 setValue 和 patchValue 修改值

```ts
f() {
  this.myForm.patchValue({
    name: ['a', 'b'],
  });
  // 等价的修改方式
  this.myForm.get('name').get('0').setValue('a');
  this.myForm.get('name').get('1').setValue('b');
}
```

* FormArray 常用于管理不定数量的 FormControl，不会手动指定索引，而是用循环遍历显示，用 FormArray.controls 即可获取其中保存的 FormControl 数组，注意这种方式把索引赋值给 formControlName 时要加上中括号

```html
<form [formGroup]="myForm">
  <div formArrayName="name">
    <div *ngFor="let p of name.controls; let i=index">
      <input type="text" [formControlName]="i">
    </div>
  </div>
  <label>age:<input type="text" formControlName="age"></label>
</form>
```

* 但这里会报错，因为不知道 name 有 controls 属性

```
ERROR TypeError: Cannot read property 'controls' of undefined
```

* 为此需要为 name 指定一个 getter

```ts
get name() {
  return this.myForm.get('name') as FormArray;  // get 获取的是 AbstractControl 类型，应断言为 FormArray
  // AbstractControl 是 FormControl、FormGroup、FormArray 的抽象基类
}
```

* 此外可以动态添加或删除 FormArray 中的 FormControl

```ts
addName() {
  (this.myForm.get('name') as FormArray).push(this.fb.control(''));
}

removeName(index: number) {
  (this.myForm.get('name') as FormArray).removeAt(index);
}
```
