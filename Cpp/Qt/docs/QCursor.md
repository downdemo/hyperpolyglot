* 获取鼠标在显示器中的坐标

```cpp
QPoint p = QCursor::pos();
int posX = QCursor::pos().x();
int poxY = QCUrsor::pos().y();
```

* 如果超出主屏幕宽度，则将窗口移动到副屏幕

```cpp
if (QCursor::pos().x() > QApplication::desktop()->screenGeometry(-1).width()) {
  w.move(desktop->screenGeometry(-1).width(), 0);
}
```
