* [QDockWidget](https://doc.qt.io/qt-5/qdockwidget.html) 只能用于 [QMainWindow](https://doc.qt.io/qt-5/qmainwindow.html)

```cpp
auto dock = new QDockWidget(tr(u8"示意图"));
dock->setFeatures(QDockWidget::DockWidgetMovable |
                  QDockWidget::DockWidgetFloatable);
dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
addDockWidget(Qt::RightDockWidgetArea, dock);
resizeDocks({dock}, {600}, Qt::Horizontal);
```

* 常用成员函数

```cpp
setFeatures(QDockWidget::DockWidgetFeatures);
enum DockWidgetFeature {
  QDockWidget::DockWidgetClosable,
  QDockWidget::DockWidgetMovable,
  QDockWidget::DockWidgetFloatable,
  QDockWidget::DockWidgetVerticalTitleBar,
  QDockWidget::AllDockWidgetFeatures,
  QDockWidget::NoDockWidgetFeatures
};

setAllowedAreas(Qt::DockWidgetAreas);

enum Qt::DockWidgetArea {
  Qt::LeftDockWidgetArea,
  Qt::RightDockWidgetArea,
  Qt::TopDockWidgetArea,
  Qt::BottomDockWidgetArea,
  Qt::AllDockWidgetAreas,
  Qt::NoDockWidgetArea
};

setWidget(QWidget*);
```

* QMainWindow 相关函数

```cpp
addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockwidget);
addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockwidget,
              Qt::Orientation orientation);
enum Qt::Orientation { Qt::Horizontal, Qt::Vertical };

void removeDockWidget(QDockWidget *dockwidget);
resizeDocks(const QList<QDockWidget *> &docks, const QList<int> &sizes,
            Qt::Orientation orientation);

// 把 first 的某个方向的部分空出来给 second，以实现并排布局
void splitDockWidget(QDockWidget *first, QDockWidget *second,
                     Qt::Orientation orientation);

// 将两个 dock 合并到一个 tab 下
void tabifyDockWidget(QDockWidget *first, QDockWidget *second);

setDockNestingEnabled(bool);
```
