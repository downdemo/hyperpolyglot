```js
function deepCopy(o, c) {
  let res = c || {};
  for (let p in o) {
    if (typeof o[p] === 'object') {
      res[p] = Array.isArray(o[p]) ? [] : {};
      deepCopy(o[p], res[p]);
    } else {
      res[p] = o[p];
    }
  }
  return res;
}
```
