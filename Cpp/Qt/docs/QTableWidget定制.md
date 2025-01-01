* MyTableWidget.h
```cpp
#pragma once
#include "ui_MyTableWidget.h"

namespace Ui {
class MyTableWidget;
}

class MyTableWidget : public QTableWidget {
  Q_OBJECT
 public:
  explicit MyTableWidget(QTableWidget* parent = Q_NULLPTR);
  explicit MyTableWidget(int, int, QTableWidget* parent = Q_NULLPTR);
  ~MyTableWidget();
 private slots:
  void popMenu(QPoint pos);
  void addTableRow();
  void deleteTableRow();

 private:
  Ui::MyTableWidget* ui;
  int rowIndex;
  QMenu* rightClickMenu;
  QAction* addRowAction;
  QAction* deleteRowAction;
};
```
* MyTableWidget.cpp
```cpp
#include "MyTableWidget.h"
#include "stdafx.h"

class NoFocusDelegate : public QStyledItemDelegate {
 private:
  void paint(QPainter* painter, const QStyleOptionViewItem& option,
             const QModelIndex& index) const {
    QStyleOptionViewItem itemOption(option);
    if (itemOption.state & QStyle::State_HasFocus) {
      itemOption.state = itemOption.state ^ QStyle::State_HasFocus;
    }
    QStyledItemDelegate::paint(painter, itemOption, index);
  }
};

MyTableWidget::MyTableWidget(QTableWidget* parent)
    : QTableWidget(parent), ui(new Ui::MyTableWidget) {
  ui->setupUi(this);
  setFrameShape(QFrame::NoFrame);  // 隐藏边框线
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, &QWidget::customContextMenuRequested, this,
          &MyTableWidget::popMenu);
}

MyTableWidget::MyTableWidget(int row, int col, QTableWidget* parent)
    : MyTableWidget(parent) {
  setRowCount(row);
  setColumnCount(col);
}

MyTableWidget::~MyTableWidget() { delete ui; }

void MyTableWidget::popMenu(QPoint pos) {
  QModelIndex index = indexAt(pos);
  rowIndex = index.row();  // 获取鼠标所在行
  if (rowIndex != -1)      // 鼠标在表格范围内
  {
    rightClickMenu = new QMenu;
    addRowAction = new QAction(tr(u8"添加行"));
    deleteRowAction = new QAction(tr(u8"删除行"));

    rightClickMenu->addAction(addRowAction);
    rightClickMenu->addAction(deleteRowAction);

    rightClickMenu->move(cursor().pos());
    rightClickMenu->show();
    connect(addRowAction, &QAction::triggered, this,
            &MyTableWidget::addTableRow);
    connect(deleteRowAction, &QAction::triggered, this,
            &MyTableWidget::deleteTableRow);
  }
}

void MyTableWidget::addTableRow() {
  if (rowCount() < 5) {
    insertRow(rowIndex + 1);  // 在鼠标当前行的下一行添加
    for (int i = 0; i < columnCount(); ++i) {
      QTableWidgetItem* item = new QTableWidgetItem(tr(u8""));
      setItem(rowIndex + 1, i, item);
    }
  }
}

void MyTableWidget::deleteTableRow() {
  if (rowCount() > 1) {
    removeRow(rowIndex);
  }
}
```
