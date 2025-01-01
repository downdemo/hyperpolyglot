[QDesktopWidget](https://doc.qt.io/qt-5/qdesktopwidget.html)

```cpp
#include "qdesktopwidget.h"

QDesktopWidget* desktop = QApplication::desktop();
int n = desktop->screenCount();  // 屏幕数量
int primaryScreenWidth = desktop->screenGeometry(-1).width();
int primaryScreenHeight = desktop->screenGeometry(-1).height();

// 固定窗口显示在主屏幕中间
QWidget w;
auto primaryScreenWidth = desktop->screenGeometry(-1).width();
auto primaryScreenHeight = desktop->screenGeometry(-1).height();
w.setGeometry((primaryScreenWidth - w.width()) / 2,
              (primaryScreenHeight - w.height()) / 2, w.width(), w.height());
```
