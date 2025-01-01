* 右键响应信号 [QWidget::customContextMenuRequested](http://doc.qt.io/qt-5/qwidget.html#customContextMenuRequested) 会在右键设置了 [contextMenuPolicy](http://doc.qt.io/qt-5/qwidget.html#contextMenuPolicy-prop) 为 [Qt::CustomContextMenu](http://doc.qt.io/qt-5/qt.html#ContextMenuPolicy-enum) 的控件时发送，发送参数为当前鼠标的位置

```cpp
// 必须先设置菜单策略
table->setContextMenuPolicy(Qt::CustomContextMenu);
connect(table, &QWidget::customContextMenuRequested, this, &dlg::popMenu);
// 旧写法
connect(table, SIGNAL(customContextMenuRequested(QPoint)), this,
        SLOT(popMenu(QPoint)));
```

* 利用 QTableView::indexAt (const [QPoint](http://doc.qt.io/qt-5/qpoint.html) &pos) 通过坐标返回 [QModelIndex](http://doc.qt.io/qt-5/qmodelindex.html) 从而获取行号

```cpp
QModelIndex index = table->indexAt(pos);
int rowIndex = index.row();  // 获取鼠标所在行
```

* [QModelIndex](http://doc.qt.io/qt-5/qmodelindex.html) 的成员函数

| return type | function name |
|:-:|:-:|
|  | **[QModelIndex](http://doc.qt.io/qt-5/qmodelindex.html#QModelIndex)**() |
| int | **[column](http://doc.qt.io/qt-5/qmodelindex.html#column)**() const |
| QVariant | **[data](http://doc.qt.io/qt-5/qmodelindex.html#data)**(int *role* = Qt::DisplayRole) const |
| Qt::ItemFlags | **[flags](http://doc.qt.io/qt-5/qmodelindex.html#flags)**() const |
| quintptr | **[internalId](http://doc.qt.io/qt-5/qmodelindex.html#internalId)**() const |
| void * | **[internalPointer](http://doc.qt.io/qt-5/qmodelindex.html#internalPointer)**() const |
| bool | **[isValid](http://doc.qt.io/qt-5/qmodelindex.html#isValid)**() const |
| const QAbstractItemModel * | **[model](http://doc.qt.io/qt-5/qmodelindex.html#model)**() const |
| QModelIndex | **[parent](http://doc.qt.io/qt-5/qmodelindex.html#parent)**() const |
| int | **[row](http://doc.qt.io/qt-5/qmodelindex.html#row)**() const |
| QModelIndex | **[sibling](http://doc.qt.io/qt-5/qmodelindex.html#sibling)**(int *row*, int *column*) const |
| QModelIndex | **[siblingAtColumn](http://doc.qt.io/qt-5/qmodelindex.html#siblingAtColumn)**(int *column*) const |
| QModelIndex | **[siblingAtRow](http://doc.qt.io/qt-5/qmodelindex.html#siblingAtRow)**(int *row*) const |
| bool | **[operator!=](http://doc.qt.io/qt-5/qmodelindex.html#operator-not-eq)**(const QModelIndex &*other*) const |
| bool | **[operator<](http://doc.qt.io/qt-5/qmodelindex.html#operator-lt)**(const QModelIndex &*other*) const |
| bool | **[operator==](http://doc.qt.io/qt-5/qmodelindex.html#operator-eq-eq)**(const QModelIndex &*other*) const |

* 在响应 [QWidget::customContextMenuRequested](http://doc.qt.io/qt-5/qwidget.html#customContextMenuRequested) 的槽函数中添加 [QMenu](http://doc.qt.io/qt-5/qmenu.html) 和 [QAction](http://doc.qt.io/qt-5/qaction.html) 实现右键菜单，点击菜单选项的槽函数用信号 `QAction::triggered` 连接

```cpp
void dlg::popMenu(QPoint pos) {
  QModelIndex index = table->indexAt(pos);
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
    connect(addRowAction, &QAction::triggered, this, &dlg::addTableRow);
    connect(deleteRowAction, &QAction::triggered, this, &dlg::deleteTableRow);

    // 添加子菜单
    // QMenu* subMenu = rightClickMenu->addMenu(tr(u8"更多选项"));
    // subMenu->addAction(new QAction("op1"));
    // subMenu->addAction(new QAction("op2"));
  }
}

void dlg::addTableRow() {
  if (table->rowCount() < 5) {
    table->insertRow(rowIndex + 1);  // 在鼠标当前行的下一行添加
    for (int i = 0; i < table->columnCount(); ++i) {
      QTableWidgetItem* item = new QTableWidgetItem(tr(u8""));
      table->setItem(rowIndex + 1, i, item);
    }
  }
}

void dlg::deleteTableRow() {
  if (table->rowCount() > 1) {
    table->removeRow(rowIndex);
  }
}
```
