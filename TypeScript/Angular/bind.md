```ts
function bind(f, context) {
  return function() {
    return f.apply(context, arguments);
  }
}

export class AComponent {
  this.i = 1;
  this.canvas.on('mouse:dblclick', bind(this.f, this));
  f(e) {
    console.log(this.i);  // 如果不 bind，this 指的是 window，this.i 将是 undefined
  }
}
```
