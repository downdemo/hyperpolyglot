[QLineEdit](http://doc.qt.io/qt-5/qlineedit.html)

```cpp
// 继承自 QWidget，鼠标悬停提示
void setToolTip(const QString &);
// 默认显示的占位字符
void setPlaceholderText(const QString &);
void setReadOnly(bool);
void setMaxLength(int);
// 设置文本位置，如
// Qt::AlignLeft、Qt::AlignRight、Qt::AlignHCenter、Qt::AlignJustify
void setAlignment(Qt::Alignment flag);
// 设置显示方式
void setEchoMode(QLineEdit::EchoMode);
enum EchoMode {
  Normal,
  NoEcho,
  Password,
  PasswordEchoOnEdit
}
// 全选文本
setFocus();   // 获取焦点
selectAll();  // 获取焦点后全选才会生效
// 清空内容
void clear();
// 限制输入格式
void setInputMask(const QString &inputMask);
void setValidator(const QValidator *v);
```

* 文本改变时会发送 QLineEdit::textChanged 信号

```cpp
void dlg::slots(const QString& text) {
  qDebug() << text;  // 改变后的当前文本内容
}
connect(pLineEdit, &QLineEdit::textChanged, this, &dlg::slots);
```

* 用 setLayout 在 QLineEdit 的右边嵌入 QPushButton

```cpp
auto lineEdit = new QLineEdit;
lineEdit->setTextMargins(0, 0, 0, 0);  // 设置内部文本的边距
auto btn = new QPushButton("test");
btn->setFixedWidth(50);
auto layout = new QHBoxLayout;
layout->setContentsMargins(0, 0, 0, 0);  // 必须设置外边距，否则无法显示
layout->addWidget(btn, 0, Qt::AlignRight);  // 右侧对齐
lineEdit->setLayout(layout);
```
