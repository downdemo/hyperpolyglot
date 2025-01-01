```js
let s = '       (     1.23    ,    -3.14      )  ';
let r = /(\s)*\(\s*-?\d+\.?\d*\s*,\s*-?\d+\.?\d*\s*\)(\s)*$/;
console.log(r.test(s));
```
