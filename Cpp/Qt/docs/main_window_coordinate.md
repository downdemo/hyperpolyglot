* var.h

```cpp
#pragma once

extern int px;
extern int py;
```

* var.cpp

```cpp
#include "var.h"

int px;
int py;
```

* main.cpp

```cpp
#include <QtWidgets/QApplication>

#include "QtGuiApplication.h"
#include "var.h"

int main(int argc, char** argv) {
  QApplication app(argc, argv);
  QtGuiApplication w;
  w.show();
  // 在显示之后获取，否则坐标为(0, 0)
  px = w.x();
  py = w.y();
  return app.exec();
}
```

* 此时坐标已获取，在其他窗口中包含 var.h 直接使用即可
* QtGuiApplication.h（即主窗口）

```cpp
#include "var.h"

class QtGuiApplication : public QMainWindow {
 private slots:
  void showDlg();  // 添加槽函数
}
```

* QtGuiApplication.cpp

```cpp
void QtGuiApplication2::showDlg() {
  // ViewerWidget是自定义的类
  ViewerWidget* viewWidget =
      new ViewerWidget(0, Qt::Widget, osgViewer::ViewerBase::SingleThreaded);
  // 设置相对父窗口的位置
  viewWidget->setGeometry(px + 100, py + 100, 800, 600);
  viewWidget->show();
}
```
