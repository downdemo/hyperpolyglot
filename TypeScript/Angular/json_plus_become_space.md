 * 传输 JSON 给后台时，JSON 中的加号会变成空格，解决方法是使用 [encodeURIComponent](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/encodeURIComponent) 处理要传输的 JSON

```ts
encodeURIComponent(JSON.stringify(testJSON));
```

* 这种方法会把加号变为 `%2B`

```ts
let x = '+++';
console.log(encodeURIComponent(x));  // %2B%2B%2B
```

* 也可以直接替换字符串里的加号，但这只是针对加号，实际上 encodeURIComponent 还会处理一些其他的特殊字符

```ts
x.replace(/\+/g, '%2B');
```

* 后台再转换为原数据

```ts
URLDecoder.decode(str, "UTF-8");  // Java 的方法
```
