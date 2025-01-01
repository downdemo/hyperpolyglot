## [QSizePolicy](http://doc.qt.io/qt-5/qsizepolicy.html)

* 通过重写返回 [QSize](http://doc.qt.io/qt-5/qsize.html) 的虚函数设置大小

```cpp
// 如果控件在 layout 中则返回合适大小，否则返回无效值
virtual QSize sizeHint() const;
// 最小合适大小（没有最大合适大小）
virtual QSize minimumSizeHint() const;
```

* setSizePolicy 设置控件的伸展策略

```cpp
void QWidget::setSizePolicy(QSizePolicy::Policy horizontal,
                            QSizePolicy::Policy vertical);
setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
```

| Constant | Value | Description |
|:-:|:-:|:-:|
| QSizePolicy::Fixed | 0 | 只允许 [QWidget::sizeHint](http://doc.qt.io/qt-5/qwidget.html#sizeHint-prop)，控件不会伸缩 (如 pushbutton 的垂直方向) |
| QSizePolicy::Minimum | GrowFlag | sizeHint() 是可允许的最小尺寸， 可以变大 (如 pushbutton 的水平方向) |
| QSizePolicy::Maximum | ShrinkFlag | sizeHint() 最大尺寸，如果没有其他需要空间的控件 (如 separator line)，则此控件可任意伸缩 |
| QSizePolicy::Preferred | GrowFlag \| ShrinkFlag | sizeHint() 是最佳选择，但控件仍能伸缩，这是 QWidget 的默认策略 |
| QSizePolicy::Expanding | GrowFlag \| ShrinkFlag \| ExpandFlag | sizeHint() 未被忽略，但控件尽可能扩张 |
| QSizePolicy::MinimumExpanding | GrowFlag \| ExpandFlag | sizeHint() 是最小尺寸，但控件尽可能扩张 (如 horizontal slider 的水平方向) |
| QSizePolicy::Ignored | ShrinkFlag \| GrowFlag \| IgnoreFlag | sizeHint() 被忽略，控件尽可能扩张 |
