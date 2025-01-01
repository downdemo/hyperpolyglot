* [Angular 官方文档](https://angular.io/docs)
* [Angular 中文文档](https://angular.cn/docs)
* [Angular 材料库](https://material.angular.io/components/categories)
* [Angular 材料库中文版](https://material.angular.cn/components/categories)

## 安装 Angular

* 安装 [node](https://nodejs.org/en/download/)
* 安装 TypeScript

```sh
npm install -g typescript
```

* 安装 angular cli

```sh
npm i -g @angular/cli
```

* 检查版本

```sh
ng version
```

* 新建一个目录，进到该目录下用 `ng new` 创建项目

```sh
cd Desktop/ng-demo
ng new demo
```

* 创建完成后，进入该项目目录，用 `ng serve` 编译

```sh
cd demo
# -o 选项表示编译完成后自动打开 http://localhost:4200/
ng serve -o
```

* 编译完成后，在浏览器中访问 `http://localhost:4200/` 即可
* 如果一台机器编译多个项目将出现端口号冲突，启动另一个项目时用 `--port` 选项指定其他端口号即可

```sh
# 通过 http://localhost:4396/ 访问
ng serve -o --port 4396
```

* 如果想让局域网中的其他用户访问项目，通过 `--host` 选项指定本机 ip 地址即可

```sh
# 局域网中用户均能通过 http://192.168.0.1:4200 访问
ng serve --host 192.168.0.1
```

## 项目结构

* 实际上，默认显示的页面是 `src/index.html`

```html
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <title>Demo</title>
  <base href="/">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" type="image/x-icon" href="favicon.ico">
</head>
<body>
  <app-root></app-root>
</body>
</html>
```

* 其中的 `app-root` 就是默认组件 `AppComponent` 的选择器，`app.component.ts` 内容如下

```ts
import { Component } from '@angular/core';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  title = 'demo';
}
```

* 在 `app.module.ts` 的 `@NgModule` 装饰器中的 `bootstrap` 就是根组件，初始创建的 `app.module.ts` 如下

```ts
import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';

@NgModule({
  declarations: [
    AppComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
```

* 用 `ng g component cat` 创建一个新组件，将多出一个 `cat` 文件夹，`app.module.ts` 将变为

```ts
import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { CatComponent } from './cat/cat.component';  // 新增

@NgModule({
  declarations: [
    AppComponent,
    CatComponent // 新增
  ],
  imports: [
    BrowserModule,
    AppRoutingModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
```

* 如果要彻底删除新增的 `cat` 组件，在删除 `cat` 文件夹之后，还应该在 `app.module.ts` 中删除对应的导入声明

## 依赖注入

* 除了创建组件，还可以创建类和服务
* 用 `ng g class` 创建类

```ts
ng g class someClass
CREATE src/app/some-class.spec.ts (167 bytes)
CREATE src/app/some-class.ts (27 bytes)
```

* 创建的 `some-class.ts` 中内容如下

```ts
export class SomeClass {
}
```

* 如果创建服务，使用 `ng g service`

```ts
ng g service someService
CREATE src/app/some-service.service.spec.ts (383 bytes)
CREATE src/app/some-service.service.ts (140 bytes)
```

* `some-service.service.ts` 和类的区别是，多了一个 `@Injectable` 装饰器

```ts
import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class SomeServiceService {

  constructor() { }
}
```

* `@Injectable` 的作用是，允许该类注入其他组件，比如用 `ng g component cat` 创建一个组件，`cat.component.ts` 内容如下

```ts
import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-cat',
  templateUrl: './cat.component.html',
  styleUrls: ['./cat.component.css']
})
export class CatComponent implements OnInit {

  constructor() { }

  ngOnInit(): void {
  }

}
```

* 在构造函数中注入该服务，就可以在组件中使用该服务的 public 成员

```ts
import { Component, OnInit } from '@angular/core';
import { SomeServiceService } from '../some-service.service';

@Component({
  selector: 'app-cat',
  templateUrl: './cat.component.html',
  styleUrls: ['./cat.component.css']
})
export class CatComponent implements OnInit {

  constructor(private x: SomeServiceService) { }

  ngOnInit(): void {
  }

}
```

* 如果对类也这样注入

```ts
import { Component, OnInit } from '@angular/core';
import { SomeClass } from '../some-class';

@Component({
  selector: 'app-cat',
  templateUrl: './cat.component.html',
  styleUrls: ['./cat.component.css']
})
export class CatComponent implements OnInit {

  constructor(private x: SomeClass) { }

  ngOnInit(): void {
  }

}
```

* 在运行时，浏览器控制台将报错

```ts
ERROR NullInjectorError: R3InjectorError(AppModule)[SomeClass -> SomeClass -> SomeClass]: 
  NullInjectorError: No provider for SomeClass!
```

* 如果将类也加上 `@Injectable` 装饰器，就能和服务一样用于注入。可见，这就是类和服务的本质区别

```ts
import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class SomeClass {
}
```

## 断点调试

* 下载并安装 [Visual Studio Code](https://code.visualstudio.com/Download)
* 用 VS Code 打开项目文件夹（如 `Desktop/workplace/demo`）
* 在工具栏中选择 `View - Terminal` 弹出控制台，在控制台中执行 `ng serve -o` 启动项目
* 点击左侧的 Extensions 图标（快捷键 `Ctrl + Shift + X`），搜索并安装 `Debugger for Chrome`
* 按 F5 运行项目，环境选择 Chrome，弹出配置文件 `lanunch.json`（完整位置为 `./vscode/lanunch.json`），内容为

```json
{
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "type": "chrome",
            "request": "launch",
            "name": "Launch Chrome against localhost",
            "url": "http://localhost:8080",
            "webRoot": "${workspaceFolder}"
        }
    ]
}
```

* 将其中的 `url` 修改为要调试的地址，保存后再次按 F5 进入 Debug 模式，此时即可在.ts 文件中打断点

```ts
{
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "type": "chrome",
            "request": "launch",
            "name": "Launch Chrome against localhost",
            "url": "http://localhost:4200",
            "webRoot": "${workspaceFolder}"
        }
    ]
}
```

## VS Code 使用

* 快捷键

```
Alt + F12：预览函数定义
Shift + F12：预览使用当前变量、函数的位置
Ctrl + Shift + F：全局查找
Ctrl + Shift + O：显示函数列表
F8：跳转到下一个错误
Shift + F8：跳转到上一个错误
Ctrl + Home/End：跳转到头/尾
Ctrl + Shift + |：跳转到括号匹配处
Ctrl + Shift + K：删除当前行
Alt + Up/down：移动当前行到上/下一行
Shift + Alt + Up/down：在上/下一行复制当前行
Ctrl + Up/down：按行向上/下滚动
Shift + PgUp/PgDn：按页向上/下滚动并选中
Alt + 鼠标左键：多光标
```

* `settings.json`

```ts
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
