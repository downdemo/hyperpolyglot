## 通过 Designer 设计样式

* 在 Designer 中添加一个 QPushButton，右键按钮，点击 `改变样式表`
* 在弹出的窗口中输入 `color:`，再点击 `添加颜色`，选择红色，即可添加如下样式

```
color: rgb(255, 0, 0)
```

* 点击确定后，按钮中的字即变为红色
* 也可以在右侧的属性编辑器中看到 styleSheet 的选项

## 通过代码设计样式

* 任何 Qt 控件都可以直接调用 `QObject::setStyleSheet` 函数来设定样式

```cpp
myPushButton->setStyleSheet("color : blue");
```

* 也可以通过选择器对指定对象设定样式

```
QLineEdit#usernameEdit { background-color: blue }
```

## 样式表的继承关系

* 添加三个 QPushButton 和一个 QCheckBox
* 右键主窗口，选择 `改变样式表`，输入如下样式

```
border: 2px solid gray;
border-radius: 10px;
padding: 0 8px;
background: yellow;
```

* 所有的子控件都会被影响
* 为此，可以指定某个控件类型，将样式改为

```
QPushButton {
  border: 2px solid gray;
  border-radius: 10px;
  padding: 0 8px;
  background: yellow;
}
```

* 现在只有 QPushButton 应用了样式
* 还可以指定特定的对象，将样式改为

```
QPushButton#pushButton_3 {
  border: 2px solid gray;
  border-radius: 10px;
  padding: 0 8px;
  background: yellow;
}
```

* 现在只有名为 pushButton_3 的 QPushButton 受影响
* 即如果为特定对象指定了样式，就会覆盖掉继承的样式，现在把样式改为

```
QPushButton {
  color: red;
  border: 0px;
  padding: 0 8px;
  background: white;
}
QPushButton#pushButton_2 {
  border: 1px solid red;
  border-radius: 10px;
}
QPushButton#pushButton_3 {
  border: 2px solid gray;
  border-radius: 10px;
  padding: 0 8px;
  background: yellow;
}
```

* 第二个按钮仍然继承了 QPushButton 的 color 和 padding
* 还可以为通用选择器指定样式，在上述基础上添加如下样式

```cpp
*{
background:
  qradialgradient(cx : 0.3, cy
                  : -0.4, fx : 0.3, fy
                  : -0.4, radius : 1.35, stop : 0 #fff, stop : 1 #888);
color:
  rgb(255, 255, 255);
border:
  1px solid #ffffff;
}
```

## 代码示例

```cpp
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setWindowState(Qt::WindowMaximized);  // 最大化
  this->setStyleSheet(
      "QMainWindow{"
      "background:white;}"

      "QToolButton{"
      "background-color:rgba(100,225,100,30);"  // 背景色
      "border-style:outset;"                    // 边框样式
      "border-width:4px;"                       // 边框宽度像素
      "border-radius:10px;"                     // 边框圆角半径像素
      "border-color:rgba(255,255,255,30);"      // 边框颜色
      "font:bold 15px;"                         // 字体，字体大小
      "color:rgba(0,0,0,100);"                  // 字体颜色
      "padding:6px;"                            // 填衬
      "}"

      "QToolButton:hover{"
      "background-color:rgba(100,255,100,100);"
      "border:0px;"
      "border-color:rgba(255,255,255,200);"
      "color:rgba(0,0,0,200);"
      "}"

      "QToolButton:pressed{"
      "background-color:rgba(100,255,100,200);"
      "border:0px;"
      "border-color:rgba(255,255,255,30);"
      "border-style:inset;"
      "color:rgba(0,0,0,100);"
      "}");
}
```
