[location 对象属性](https://www.runoob.com/jsref/obj-location.html)
([API 文档](https://developer.mozilla.org/en-US/docs/Web/API/Location))

* 对于

```html
http://localhost:4200
```

* 想通过在 url 中添加查询部分来赋值
```html
http://localhost:4200?userId=123
```

* 要获取 url 的查询部分，可以使用 location.search

```ts
console.log(location.search);  // ?userId=123
```

* 此外 location 还可以获取 url 的各个部分

```ts
// 以 http://localhost:4200 为例
location.href // http://localhost:4200
location.protocol // http:
location.host // localhost:4200
location.hostname // localhost
location.port // 4200

// 以 http://localhost:4200/home 为例
location.pathname // /home

// 以 http://localhost:4200#home 为例
location.hash // #home
```
