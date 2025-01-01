* [QTableWidget](http://doc.qt.io/qt-5/qtablewidget.html)
* [QTableWidgetItem](http://doc.qt.io/qt-5/qtablewidgetitem.html)

## 标题居中

```cpp
label->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
```

## 创建表格

```cpp
int row = 20;
int col = 10;
QTableWidget* table = new QTableWidget(row, col, this);
table->horizontalHeader()->setDisabled(true);  // 设置列表头不能拖动
table->horizontalHeader()->setVisible(false);  // 隐藏列表头

// 设置行表头
for (int i = 0; i < row; ++i) {
  QTableWidgetItem* header =
      new QTableWidgetItem(v[i]);  // 元素为 QString 的容器
  header->setTextAlignment(
      Qt::AlignHCenter |
      Qt::AlignVCenter);  // 居中
                          // 列表头则用 setHorizontalHeaderItem
  table->setVerticalHeaderItem(i, header);
}

// 行表头字体加粗
QFont font = table->horizontalHeader()->font();
font.setBold(true);
table->verticalHeader()->setFont(font);

// 设置表内容
for (int i = 0; i < row; ++i) {
  QString txt = QString("%1").arg(row + 1);
  QTableWidgetItem* item = new QTableWidgetItem;
  item->setText(txt);

  item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  item->setFlags(Qt::ItemFlags(32));
  // item->setBackgroundColor(QColor(0, 60, 10));
  item->setTextColor(QColor(200, 111, 100));
  item->setFont(QFont("Helvetica"));
  table->setItem(i, 0, item);
}
```

## 清空表格

```cpp
table->clearContents();  // 清除所有非表头 item
table->setRowCount(0);
```

## 以控件填充单元格

```cpp
QComboBox* combobox = new QComboBox;
combobox->addItem("test1");
combobox->addItem("test2");
table->setCellWidget(0, 0, combobox);
// 获取 ComboBox 文本
QComboBox* tmp = qobject_cast<QComboBox*>(table->cellWidget(0, 0));
qDebug() << tmp->currentText();
```

## 表格宽高设置

```cpp
// 设置表头高度
table->horizontalHeader()->setFixedHeight(30);

// 默认行高
table->verticalHeader()->setDefaultSectionSize(30);
// 默认列宽
table->horizontalHeader()->setDefaultSectionSize(120);

// 设置某列宽度
table->setColumnWidth(int column, int width);
table->horizontalHeader()->resizeSection(0, 100);

// 最后一列自动扩展宽度
table->horizontalHeader()->setStretchLastSection(true);

// 等宽
table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

// 固定某列宽度
table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);
```

## 隐藏

```cpp
// 隐藏行列
setRowHidden(int row, bool hide);
setColumnHidden(int column, bool hide);

// 检查行列隐藏状态
bool isRowHidden(int row) const;
bool isColumnHidden(int column) const;

// 隐藏表头
table->horizontalHeader()->setVisible(false);  // 隐藏列表头
table->verticalHeader()->setVisible(false);    // 隐藏行表头
```

* 选中

```cpp
// 设置单元格为不可编辑方式
table->setEditTriggers(QAbstractItemView::NoEditTriggers);

// 选中方式
table->setSelectionBehavior(QAbstractItemView::SelectItems);  // 单个选中
table->setSelectionBehavior(QAbstractItemView::SelectRows);   // 整行选中
table->setSelectionMode(
    QAbstractItemView::ExtendedSelection);  // 可以选中多个
                                            // 取消选中单元格时表头高亮
table->horizontalHeader()->setHighlightSections(false);
```

## 点击

```cpp
// 点击横向表头（列表头）
connect(table->horizontalHeader(), &QHeaderView::sectionClicked, this,
        &dlg::clickHeader);
// 点击纵向表头（行表头）
connect(table->verticalHeader(), &QHeaderView::sectionClicked, this,
        &dlg::clickHeader);
// 点击表头的信号的参数为 int
void dlg::clickHeader(int n) { qDebug() << n; }

// 点击单元格
connect(table, &QTableWidget::itemClicked, this, &dlg::slot);
// 点击单元格的信号的参数为 QTableWidgetItem*
void dlg::slot(QTableWidgetItem* item) {
  qDebug() << item->row() << item->column();
}
```

## 字体与颜色

```cpp
// 表头字体加粗
QFont font = table1->horizontalHeader()->font();
font.setBold(true);
table->verticalHeader()->setFont(font);

// 颜色样式
table->setFrameShape(QFrame::NoFrame);  // 整个表格无外围黑线边框
table->setShowGrid(false);              // 不显示表格格子线
// 设置选中单元格背景色
table->setStyleSheet("selection-background-color:lightblue;");
// 设置表头背景色

table->horizontalHeader()->setStyleSheet(
    "QHeaderView::section{background:skyblue;}");
// 取消选中时格子虚线，但无法再使用键盘控制表格
table->setFocusPolicy(Qt::NoFocus);
```

## 取消选中时格子周围的虚线

```cpp
// file “style.h”

#pragma once

#include "qstyleditemdelegate.h"
class NoFocusDelegate : public QStyledItemDelegate {
 private:
  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const {
    QStyleOptionViewItem itemOption(option);
    if (itemOption.state & QStyle::State_HasFocus) {
      itemOption.state = itemOption.state ^ QStyle::State_HasFocus;
    }
    QStyledItemDelegate::paint(painter, itemOption, index);
  }
};

// 包含头文件即可直接使用
#include "style.h"

table->setItemDelegate(new NoFocusDelegate());
```

## 滚动条

```cpp
// 隐藏滚动条
table->verticalScrollBar()->setEnabled(false);
table->horizontalScrollBar()->setEnabled(false);

// 滚动条样式
table->horizontalScrollBar()->setStyleSheet(
    "QScrollBar{background:transparent; height:10px;}"
    "QScrollBar::handle{background:lightgray; border:2px solid transparent; "
    "border-radius:5px;}"
    "QScrollBar::handle:hover{background:gray;}"
    "QScrollBar::sub-line{background:transparent;}"
    "QScrollBar::add-line{background:transparent;}");
table->verticalScrollBar()->setStyleSheet(
    "QScrollBar{background:transparent; width: 10px;}"
    "QScrollBar::handle{background:lightgray; border:2px solid transparent; "
    "border-radius:5px;}"
    "QScrollBar::handle:hover{background:gray;}"
    "QScrollBar::sub-line{background:transparent;}"
    "QScrollBar::add-line{background:transparent;}");
```

## 单元格

```cpp
QTableWidgetItem* item = new QTableWidgetItem("text");        // 单元格文字
item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);  // 居中
item->setText(const QString& text);                           // 设置文字
item->setBackgroundColor(QColor(0, 60, 10));                  // 背景色
item->setTextColor(QColor(200, 111, 100));                    // 字体色
item->setFont(QFont("Helvetica"));                            // 字体
table->setItem(0, 0, item);  // 添加到表格 (0, 0) 位置
                             // 单击单元格即进入编辑状态
table->setEditTriggers(QAbstractItemView::CurrentChanged);
item->setFlags(Qt::ItemFlags(32));  // 单元格设置为不可选
enum ItemFlags {
  NoItemFlags = 0,
  ItemIsSelectable = 1,
  ItemIsEditable = 2,
  ItemIsDragEnabled = 4,
  ItemIsDropEnabled = 8,
  ItemIsUserCheckable = 16,
  ItemIsEnabled = 32,
  ItemIsTristate = 64,
  ItemNeverHasChildren = 128,
  ItemIsUserTristate = 256
};

// 0：浅色不可选中不可编辑
// 32：黑色可选中（不高亮）不可编辑
// 63：黑色双击可编辑，枚举中没有，是初始化时的默认值

// 单元格判空
if (table->item(0, 0)->text() == "") {
  // 已设置item
}
if (!table->item(0, 0)) {
  // 未设置item
}
// 获取内容转为 QString
QString str = table->item(0, 0)->data(Qt::DisplayRole).toString();

// 合并单元格
table->setSpan(0, 0, 3, 2);  // 合并 (0, 0) 在内的右下方向的三行两列
```

## 导出单张表到 Excel

```cpp
// 只有行表头
void dlg::slot() {
  QString path = QFileDialog::getSaveFileName(this, tr(u8"另存为"),
                                              "C:\\Users\\downdemo\\Desktop",
                                              tr(u8"Excel文件(*.xlsx);"));

  if (!path.isEmpty()) {
    int row = table1->rowCount();
    int col = table1->columnCount();
    QList<QString> list;
    for (int i = 0; i < row; ++i) {
      QString rowStr = "";
      for (int j = 0; j < col; ++j) {
        rowStr.append(table1->verticalHeaderItem(i)->text() + "\t");
        if (table1->item(i, j) != nullptr) {
          rowStr.append(table1->item(i, j)->text() + "\t");
        } else {
          rowStr.append("\t");
        }
      }
      list.push_back(rowStr);
    }

    QTextEdit textEdit;
    for (int i = 0; i < list.size(); ++i) {
      textEdit.append(list.at(i));
    }

    QFile file(path);
    if (file.open(QFile::WriteOnly | QIODevice::Text)) {
      QTextStream ts(&file);
      ts.setCodec("GB2312");  // 或 "utf-8"
      ts << textEdit.document()->toPlainText();
      file.close();
    }
  }
}

// 只有列表头
void dlg::slot2() {
  QString path = QFileDialog::getSaveFileName(this, tr(u8"另存为"),
                                              "C:\\Users\\downdemo\\Desktop",
                                              tr(u8"Excel文件(*.xlsx);"));

  if (!path.isEmpty()) {
    int row = table2->rowCount();
    int col = table2->columnCount();

    QList<QString> list;
    QString HeaderRow;
    for (int i = 0; i < col; ++i) {
      HeaderRow.append(table2->horizontalHeaderItem(i)->text() + "\t");
    }
    list.push_back(HeaderRow);

    for (int i = 0; i < row; ++i) {
      QString rowStr = "";
      for (int j = 0; j < col; ++j) {
        if (table2->item(i, j) != nullptr) {
          rowStr.append(table2->item(i, j)->text() + "\t");
        } else {
          rowStr.append("\t");
        }
      }
      list.push_back(rowStr);
    }

    QTextEdit textEdit;
    for (int i = 0; i < list.size(); ++i) {
      textEdit.append(list.at(i));
    }

    QFile file(path);
    if (file.open(QFile::WriteOnly | QIODevice::Text)) {
      QTextStream ts(&file);
      ts.setCodec("GB2312");  // 或 "utf-8"
      ts << textEdit.document()->toPlainText();
      file.close();
    }
  }
}
```
