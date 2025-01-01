* 在 `localhost:4200` 中可以显示图片，部署到服务器图片加载失败，报错如下

```
Failed to load resource: the server responded with a status of 404 (Not Found)
```

* 出错代码为

```html
<img src="../../../assets/images/test.png">
```

* 解决方案是改为相对 `index.html` 的路径，`assets` 文件夹与其在同一级，因此应写为

```html
<img src="assets/images/test.png">
```

* 但在 css 中，还是使用前面这种写法，因为样式表的相对地址是相对其自身的

```css
.container {
  background: url(../../../assets/images/test.png);
}
```
