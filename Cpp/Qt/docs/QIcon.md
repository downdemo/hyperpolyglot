* 任意准备一张图片，[在线转换为 ico 图标文件](http://www.bitbug.net/)
* 将.ico 文件移动到源码所在目录下（如 `C:\Users\downdemo\source\repos\QtGuiApplication1\QtGuiApplication1`）
* 在 VS 中右键项目，选择 `添加 - 资源`，导入 Icon 文件
* 随后在项目资源列表中可以看到多出了 `.ico` 和 `.rc` 文件，编译即可将 debug 生成的 exe 的图标替换为.ico 文件
* 若要在窗口左上角显示 icon，则需要为窗口控件 setWindowIcon

```cpp
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QWidget w;

  QIcon icon;
  icon.addFile("logo.ico");
  w.setWindowIcon(icon);

  w.show();
  return a.exec();
}
```

* 如果仍然不显示 icon，则是因为路径不正确（项目有自己的目录结构，与初始的空白项目不同），使用绝对路径即可正确显示
* 使用相对路径时，当前路径为 `debug/bin`，自行定到 icon 路径也行

```cpp
icon.addFile("../../src/Project1/Resources/logo.ico");
```
