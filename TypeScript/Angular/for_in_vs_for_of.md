* in 获取索引，of 获取值

```ts
let x: string[] = ['a', 'b', 'c'];

for (let p in x) {
  console.log(p);  // 123
}

for (let p of x) {
  console.log(p);  // abc
}
```

* 对于 FormGroup 可以用 for...in 来获取所有 FormControl 的名称

```ts
constructor(fb: FormBuilder) {
  this.myForm = fb.group({
    'name': ['', Validators.required],
    'age': ['', Validators.required],
    'gender': ['', Validators.required],
  });
}

x: string[] = [];
o: Object = {};

ngOnInit() {
  for (let p in this.myForm.controls) {
    this.x.push(p);
  }
  for (let p of this.x) {
    console.log(p);  // name age gender
    this.o[p] = '';
    (this.myForm.get(p) as FormControl).valueChanges.subscribe(
      (data)=> {
        console.log(p, ': ', data);
        this.o[p] = data;
      }
    );
  }
}
```

* [Array.prototype.forEach](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/forEach)

```ts
let x = ['a', 'b', 'c'];
x.forEach((value, index) => {
  console.log(index, value);
});
```
