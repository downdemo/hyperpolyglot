## 链接方式

* 直接在 html 中链接

```html
<style>
p { color: red; }
</style>

<p>test</p>
```

* 或者保存一个单独的 css 文件

```css
/* test.css */
p { color: red; }
```

* 用 link 标签链接

```html
<link rel="stylesheet" href="test.css">

<p>test</p>
```

* 或者用 @import 指令链接

```html
<style>
@import url(test.css);
</style>

<p>test</p>
```

* 也可以不链接，直接写为内联样式

```html
<p style="color: red;">test</p>
```

## 选择器

* 通配选择器

```css
* { color: red; } /* *匹配所有元素 */
```

* 多类选择器，匹配的是包含所有类的元素

```html
<style>
.a.b { color: red; }
</style>

<p class="a b">red</p>
<p class="a">black</p>
```

* 属性选择器

```html
<style>
p[name] { color: red; }
</style>

<p name="a">red</p>
<p name="b">red</p>
```

* 多属性选择器

```html
<style>
p[name][age] { color: red; }
</style>

<p name="a" age="1">red</p>
<p name="b">black</p>
```

* 匹配准确属性值

```html
<style>
p[name='a'] { color: red; }
</style>

<p name="a">red</p>
<p name="b">black</p>
```

* 匹配部分属性值

```html
<style>
p[name~='bar'] { color: red; } /* 包含空格隔开的单词 bar */
p[name*='sub'] { color: blue; } /* 包含子串 sub */
p[name^='start'] { color: yellow; } /* 以字符串 start 开头 */
p[name$='end'] { color: green; } /* 以字符串 end 结尾 */
</style>

<p name="a bar cat">red</p>
<p name="aasubaa">blue</p>
<p name="startaaa">yellow</p>
<p name="aaaend">green</p>
```

* 后代选择器中，祖先元素的所有后代元素（不仅是直接父子元素）都会受影响

```html
<style>
country school { color: red; }
</style>

<country>
  <school>red</school>
  <city>
    <school>red</school>
  </city>
</country>
```

* 只选择直接子元素

```html
<style>
country > school { color: red; }
</style>

<country>
  <school>red</school>
  <city>
    <school>black</school>
  </city>
</country>
```

* 只选择下一个兄弟元素

```html
<style>
h + p { color: red; } /* 跟随在 h 之后的 p */
</style>

<h></h>
<p>red</p>
<p>black</p>
<p>black</p>
<h></h>
<p>red</p>
```

* 选择之后所有的兄弟元素

```html
<style>
h ~ p { color: red; }
</style>

<h></h>
<p>red</p>
<p>red</p>
```

## 伪类

* 选择是唯一子元素的元素

```html
<style>
p:only-child { color: red; }
</style>

<p>black</p>
<div>
  <p>red</p>
</div>
<div>
  <p>black</p>
  <p>black</p>
</div>
```

* 选择是第一个子元素的元素

```html
<style>
p:first-child { color: red; }
</style>

<p>red</p>
<div>
  <p>red</p>
</div>
<div>
  <p>red</p>
  <p>black</p>
</div>
```

* 选择是最后一个子元素的元素

```html
<style>
p:last-child { color: red; }
</style>

<p>black</p>
<div>
  <p>red</p>
</div>
<div>
  <p>black</p>
  <p>red</p>
</div>
```

* 选择是第 n 个子元素的元素

```html
<style>
p:nth-child(2) { color: red; }
</style>

<div>
  <p>black</p>
  <p>red</p>
  <p>black</p>
</div>
```

* 按周期选择是第 n 个子元素的元素

```html
<style>
p:nth-child(2n + 1) { color: red; }
</style>

<div>
  <p>red</p>
  <p>black</p>
  <p>red</p>
  <p>black</p>
  <p>red</p>
  <p>black</p>
  <h></h>
  <p>black</p>
  <p>red</p>
</div>
```

* 选择是唯一类型的元素

```html
<style>
p:only-of-type { color: red; }
</style>

<p>red</p>
<div>
  <p>red</p>
</div>
<div>
  <p>black</p>
  <p>black</p>
</div>
```

* 选择第一个为某类型元素

```html
<style>
p:first-of-type { color: red; }
</style>

<p>red</p>
<div>
  <p>red</p>
</div>
<div>
  <p>red</p>
  <p>black</p>
</div>
```

* 选择最后一个为某类型元素

```html
<style>
p:last-of-type { color: red; }
</style>

<p>red</p>
<div>
  <p>red</p>
</div>
<div>
  <p>black</p>
  <p>red</p>
</div>
```

* 选择第 n 个为某类型元素

```html
<style>
p:nth-of-type(2) { color: red; }
</style>

<div>
  <h></h>
  <p>black</p>
  <p>red</p>
  <p>black</p>
</div>
```

* 按周期选择第 n 个为某类型元素

```html
<style>
p:nth-of-type(2n+1) { color: red; }
</style>

<div>
  <h></h>
  <p>red</p>
  <p>black</p>
  <p>red</p>
  <p>black</p>
</div>
```

## 特异性

* 如果多个样式作用于同一元素，特异性值最高的生效。特异性用一个四元组 `(a, b, c, d)` 表示
  * 内联样式令 a 加 1
  * 每个 id 选择器，为 b 加 1
  * 每个类、伪类、属性选择器选择器，为 c 加 1
  * 每个元素、伪元素选择器，为 d 加 1
* 下面是一些特异性的例子

```html
<style>
#a { color: blue; } /* (0, 1, 0, 0) */
.b { color: green; } /* (0, 0, 1, 0) */
#a.b { color: yellow; } /* (0, 1, 1, 0) */
p { color: red; } /* (0, 0, 0, 1) */
div p { color: purple; } /* 0,0,0,2 */
#d #a { color: pink; } /* (0, 2, 0, 0) */
#d.e #a.b { color: silver; } /* (0, 2, 2, 0) */
*[name='c'] { color: gray; } /* (0, 0, 1, 0) */
*[id='a'] { color: maroon; } /* (0, 0, 1, 0)，这里的 id 是属性选择器，不是真正的 id */
p[name='c'] { color: navy; } /* (0, 0, 1, 1) */
#d.e #a.b[name='c'] { color: orange; } /* (0, 2, 3, 0) */
</style>

<div id="d" class="e">
<p id="a" class="b" name="c" style="color: black;">black</p> <!-- (1, 0, 0, 0) -->
</div>
```

* `!important` 声明拥有超过特异性的最高权重

```html
<style>
#a { color: blue; } /* (0, 1, 0, 0) */
.b { color: green; } /* (0, 0, 1, 0) */
p { color: red !important; }
</style>

<p style="color: black;">red</p>
```

* `!important` 声明也可以写在内联样式中，多个 `!important` 声明特异性最高的生效

```html
<style>
#a { color: blue; } /* (0, 1, 0, 0) */
.b { color: green; } /* (0, 0, 1, 0) */
p { color: red !important; }
</style>

<p style="color: black !important;">black</p>
```

* 完全相同的选择器，后出现的权重高

```html
<style>
p { color: blue !important; }
p { color: red !important; }
</style>

<p>red</p>
```

## 特殊值

* none：表示无
* inherit：表示从父类继承属性
* initial：表示使用默认属性
* unset：如果属性是继承的，则使用父类属性，否则使用默认属性
