## [QComboBox](http://doc.qt.io/qt-5/qcombobox.html)

* 新建

```cpp
QComboBox* comboBox = new QComboBox;
comboBox->addItem("op0");
comboBox->addItem("op1");
comboBox->addItem("op2");
comboBox->addItem("op3");

QCheckBox* checkBox = new QCheckBox("isOK");
```

* 改变下拉项样式

```cpp
auto delegate = new QStyledItemDelegate;
comboBox->setItemDelegate(delegate);  // 必须设置代理才能使改变子项高度生效
comboBox->setStyleSheet(
    "QComboBox{border:0px solid gray;background: "
    "white;font:12px;color:black;height: 30px;}"
    // "QComboBox:editable{background:black;}"
    "QComboBox QAbstractItemView{border: "
    "0px;outline:0px;selection-background-color:#FFDEAD;height:100px;"
    "background: rgb(1,58,80);font:22px;color:white;}"
    // "QComboBox QAbstractItemView::item{height:50px;width:200px;}"
    // "QComboBox QAbstractItemView::item:selected{background-color:
    // rgb(46,98,118);}"
    // "QComboBox::down-arrow{image:url(:/icon_home_trangle.png);}"
    // "QComboBox::drop-down{border:0px;}"
    "QComboBox QAbstractItemView::item {min-height: 30px;}"  // 设置子项最小高度
);
```

* 添加到 QTableWidget

```cpp
table = new QTableWidget(3, 3);
table->horizontalHeader()->setVisible(false);
table->setCellWidget(0, 0, comboBox);
table->setCellWidget(1, 0, checkBox);
```

* 居中添加到 QTableWidget

```cpp
auto w = new QWidget;
auto layout = new QVBoxLayout;
layout->addWidget(comboBox);
layout->setMargin(0);
layout->setAlignment(comboBox, Qt::AlignCenter);
w->setLayout(layout);
table->setCellWidget(0, 0, w);
```

* 获取 QTableWidget 中嵌入的 QComboBox 的内容

```cpp
QString s = qobject_cast<QComboBox*>(table->cellWidget(0, 0))->currentText();
```

* bug：QTableWidget 一旦调用过 setCellWidget，选中但不双击编辑框的单元格，直接输入拼音将导致程序崩溃，错误信息为栈溢出

```
0x00007FF9515DFB69 (Qt5Cored.dll)处(位于 QtGuiApplication1.exe 中)引发的异常: 0xC00000FD: Stack overflow (参数: 0x0000000000000001, 0x000000CE36CB3FE8)。
0x00007FF9515DFB69 (Qt5Cored.dll) (QtGuiApplication1.exe 中)处有未经处理的异常: 0xC00000FD: Stack overflow (参数: 0x0000000000000001, 0x000000CE36CB3FE8)。
```

* 绕过此问题的方法是，将 QTableWidget 设置为单击单元格进入编辑状态

```cpp
// 单击编辑
table->setEditTriggers(QAbstractItemView::CurrentChanged);
```

* QComboBox 常用成员函数

```cpp
int count() const;
int currentIndex() const;
QString currentText() const;
QString itemText(int index) const;
void insertItem(int index, const QString &text,
                const QVariant &userData = QVariant());
void insertSeparator(int index);  // 在 index 位置前插入一条分割线
void removeItem(int index);
void setEditable(bool editable);
void setMaxCount(int max);
```

* QComboBox 信号槽

```cpp
// currentIndexChanged 有 int 和 QString 参数两个重载版本，必须指定版本
connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
        &dlg::slot);
// 旧的语法
connect(comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot()));
// 也可以用 currentTextChanged，它没有重载版本，但也会在改变选项时发送
connect(comboBox, &QComboBox::currentTextChanged, this, &dlg::slot);

void dlg::slot() {
  if (comboBox->currentIndex() == 0) {
    table->setRowHidden(1, true);
    table->setRowHidden(2, true);
  } else if (comboBox->currentIndex() == 1) {
    table->setRowHidden(1, false);
    table->setRowHidden(2, true);
  } else if (comboBox->currentIndex() == 2) {
    table->setRowHidden(1, true);
    table->setRowHidden(2, false);
  } else {
    table->setRowHidden(0, false);
    table->setRowHidden(1, false);
    table->setRowHidden(2, false);
  }
}
```

## [QCheckBox](http://doc.qt.io/qt-5/qcheckbox.html)

* QCheckBox 常用成员函数

```cpp
bool isChecked() const;                    // 返回两态中的一个状态
void setChecked(bool isChecked);           // 设置两态中的一个状态
void setTristate(bool y = true);           // 设置开启三态模式
Qt::CheckState checkState() const;         // 返回三态中的一个状态
void setCheckState(Qt::CheckState state);  // 设置三态中的一个状态

enum Qt::CheckState {
  Qt::Unlocked = 0,
  Qt::Partially = 1,
  Qt::Checked = 2,
};
```

* QCheckBox 信号槽

```cpp
connect(checkBok, &QCheckBox::stateChanged, this, &dlg::slot);
```

## 将 QComboBox 下拉选项设置为 QCheckBox

* 头文件

```cpp
#pragma once

#include <qwidget.h>

#include "qboxlayout.h"
#include "qcheckbox.h"
#include "qcombobox.h"
#include "qlineedit.h"
#include "qlistwidget.h"
#include "ui_MyComboBox.h"

namespace Ui {
class MyComboBox;
}

class MyComboBox : public QWidget {
  Q_OBJECT
 public:
  explicit MyComboBox(QWidget* parent = Q_NULLPTR);
  ~MyComboBox();
  QString currentText();
 private slots:
  void checkBoxSlots(int state);
  void lineEditSlots(const QString& text);

 private:
  Ui::MyComboBox* ui;
  QComboBox* comboBox;
  QListWidget* listWidget;
  QLineEdit* lineEdit;

 private:
  QString lineEditText;
};
```

* 源文件

```cpp
#include "MyComboBox.h"

MyComboBox::MyComboBox(QWidget* parent)
    : QWidget(parent), ui(new Ui::MyComboBox) {
  ui->setupUi(this);
  listWidget = new QListWidget;
  QVector<QString> v{tr(u8"选项1"), tr(u8"选项2"), tr(u8"选项3")};
  for (int i = 0; i < v.size(); ++i) {
    QListWidgetItem* item = new QListWidgetItem(listWidget);
    item->setData(Qt::UserRole, i);
    listWidget->addItem(item);
    listWidget->addItem(item);
    QCheckBox* checkBox = new QCheckBox;
    checkBox->setText(v[i]);
    listWidget->setItemWidget(item, checkBox);
    connect(checkBox, &QCheckBox::stateChanged, this,
            &MyComboBox::checkBoxSlots);
  }

  lineEdit = new QLineEdit;
  lineEdit->setReadOnly(true);

  comboBox = new QComboBox;
  comboBox->setModel(listWidget->model());
  comboBox->setView(listWidget);  // setView 之前必须先 setModel
  comboBox->setLineEdit(lineEdit);
  connect(lineEdit, &QLineEdit::textChanged, this, &MyComboBox::lineEditSlots);

  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(comboBox);
  setLayout(layout);
}

MyComboBox::~MyComboBox() { delete ui; }

QString MyComboBox::currentText() { return lineEditText; }

void MyComboBox::checkBoxSlots(int state) {
  QString strToShow;
  for (int i = 0; i < listWidget->count(); ++i) {
    QListWidgetItem* item = listWidget->item(i);
    QCheckBox* checkBox =
        qobject_cast<QCheckBox*>(listWidget->itemWidget(item));
    if (checkBox->isChecked()) {
      strToShow.append(tr(u8"、")).append(checkBox->text());
    }
  }
  lineEditText.clear();
  if (!strToShow.isEmpty()) {
    strToShow.remove(0, 1);  // 移除开头的符号
    lineEditText = strToShow;
  }
  lineEdit->setText(lineEditText);
}

void MyComboBox::lineEditSlots(const QString& text) {
  lineEdit->setText(lineEditText);
}
```

## 添加自定义 QComboBox 到 QTableWidget

* 将自定义的 ComboBox 用 setCellWidget 添加到 QTableWidget 时，ComboBox 行高只有单元格高度的一半，将内部的 layout 用如下方式设置边距为 0 之后可以改善填充到约三分之二，但仍然不能填满单元格

```cpp
layout->setContentsMargins(0, 0, 0, 0);
```

* 解决方法是不使用布局，直接设置与单元格相等的宽高

```cpp
comboBox = new QComboBox(this);
comboBox->setGeometry(0, 0, 200, 30);
```

* 头文件

```cpp
#pragma once

#include <qwidget.h>

#include "qcheckbox.h"
#include "qcombobox.h"
#include "qlineedit.h"
#include "qlistwidget.h"
#include "ui_MyComboBox.h"

namespace Ui {
class MyComboBox;
}

class MyComboBox : public QWidget {
  Q_OBJECT
 public:
  explicit MyComboBox(const QVector<QString>& option, int width, int height,
                      QWidget* parent = Q_NULLPTR);
  ~MyComboBox();
 private slots:
  void checkBoxSlots(int state);
  void lineEditSlots(const QString& text);

 private:
  Ui::MyComboBox* ui;
  QListWidget* listWidget;
  QLineEdit* lineEdit;
  QComboBox* comboBox;

 private:
  QString lineEditText;
};
```

* 源文件

```cpp
#include "MyComboBox.h"

MyComboBox::MyComboBox(const QVector<QString>& option, int width, int height,
                       QWidget* parent)
    : QWidget(parent), ui(new Ui::MyComboBox) {
  ui->setupUi(this);
  listWidget = new QListWidget;
  for (int i = 0; i < option.size(); ++i) {
    QListWidgetItem* item = new QListWidgetItem(listWidget);
    item->setData(Qt::UserRole, i);
    listWidget->addItem(item);
    listWidget->addItem(item);
    QCheckBox* checkBox = new QCheckBox;
    checkBox->setText(option[i]);
    listWidget->setItemWidget(item, checkBox);
    connect(checkBox, &QCheckBox::stateChanged, this,
            &MyComboBox::checkBoxSlots);
  }
  lineEdit = new QLineEdit;
  lineEdit->setReadOnly(true);
  comboBox = new QComboBox(this);
  comboBox->setGeometry(0, 0, width, height);
  comboBox->setModel(listWidget->model());
  comboBox->setView(listWidget);  // setView 之前必须先 setModel
  comboBox->setLineEdit(lineEdit);
  connect(lineEdit, &QLineEdit::textChanged, this, &MyComboBox::lineEditSlots);
}

MyComboBox::~MyComboBox() { delete ui; }

void MyComboBox::checkBoxSlots(int state) {
  QString strToShow;
  for (int i = 0; i < listWidget->count(); ++i) {
    QListWidgetItem* item = listWidget->item(i);
    QCheckBox* checkBox =
        qobject_cast<QCheckBox*>(listWidget->itemWidget(item));
    if (checkBox->isChecked()) {
      strToShow.append(tr(u8"、")).append(checkBox->text());
    }
  }
  lineEditText.clear();
  if (!strToShow.isEmpty()) {
    strToShow.remove(0, 1);
    lineEditText = strToShow;
  }
  lineEdit->setText(lineEditText);
}

void MyComboBox::lineEditSlots(const QString& text) {
  lineEdit->setText(lineEditText);
}
```
