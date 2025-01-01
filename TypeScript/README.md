* [JavaScript Doc](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects)
* [ES6 简介](http://es6.ruanyifeng.com/#docs/intro)
* [RxJS Operators](https://rxjs.dev/guide/operators)
* [RxJS 可视化](https://rxviz.com/)
* [Web API](https://developer.mozilla.org/en-US/docs/Web/API)
* [DOM](https://developer.mozilla.org/en-US/docs/Web/API/Document_Object_Model)
* [BOM](https://www.runoob.com/jsref/obj-window.html)
* [Node](https://nodejs.org/en/download/)
* [TypeScript 官方文档](https://www.typescriptlang.org/docs/handbook/2/basic-types.html)
* [TypeScript 中文文档](https://www.tslang.cn/docs/handbook/basic-types.html)
* [TypeScript 入门](https://ts.xcatliu.com/)
* [HTML](https://www.w3school.com.cn/tags/index.asp)
* [CSS](https://developer.mozilla.org/en-US/docs/Web/CSS/Reference)
* [CSS 颜色表](https://www.w3school.com.cn/cssref/css_colors.asp)
* [CSS matic](https://cssmatic.com/)
* [CSS-TRICKS](https://css-tricks.com/snippets/)
* [CSS CheatSheet](https://htmlcheatsheet.com/css/)
* [caniuse](https://caniuse.com/)
* [BrowserHacks](http://browserhacks.com/)

## TypeScript 环境搭建

* 安装 [node](https://nodejs.org/en/download/)
* 更换 npm 源为淘宝源

```sh
npm config set registry http://registry.npmmirror.com
```

* 安装 TypeScript 后将 ts 编译为 js

```sh
npm install -g typescript
tsc xxx.ts
```

* 如果安装慢，改用 [cnpm](https://www.npmmirror.com/?spm=a2c6h.14029880.d-5123.1.73593839ppfaf3)

```sh
npm install -g cnpm --registry=http://registry.npmmirror.com
```

* 再用 cnpm 安装

```sh
cnpm install -g typescript
```

* 安装 ts-node

```sh
npm install -g ts-node
```

* 创建 `test.ts`，内容如下

```ts
console.log('hello world');
```

* 执行 `ts-node test.ts` 即可运行

## ESLint

* 在 VSCode 中安装 ESLint 插件
* `settings.json` 设置如下

```json
{
  "eslint.alwaysShowStatus": true,
  "eslint.format.enable": true,
  "eslint.lintTask.enable": true,
  "editor.codeActionsOnSave": {
    "source.fixAll.eslint": true
  },
  "editor.formatOnSave": true,
  "terminal.integrated.shell.windows": "C:\\WINDOWS\\System32\\cmd.exe",
  "explorer.confirmDelete": false,
  "editor.detectIndentation": false,
  "editor.tabSize": 2,
  "workbench.editor.enablePreview": false,
}
```

## RxJS 环境搭建

* 安装 RxJS

```sh
npm i rxjs
```

* `test.ts` 代码如下

```ts
import { interval } from 'rxjs';
import { take } from 'rxjs/operators';

interval(1000).pipe(take(10)).subscribe(x => console.log(x));
```

* 执行 `ts-node test.ts` 即可运行

## TypeScript Node 环境搭建

* 安装 node 的 type definition

```sh
npm i @types/node
```

* 读取一个 JSON 文件的内容，运行 `ts-node test.ts 1.json` 即可读取 `1.json`

```ts
const fs = require('fs');

let args: string[] = process.argv.splice(2);
const s: string = fs.readFileSync(args[0]);
console.log(JSON.parse(s));
```
