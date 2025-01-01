void quickSetTableStyleSheet(QTableWidget* table) {
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
  table->setItemDelegate(new NoFocusDelegate());
  table->setFrameShape(QFrame::NoFrame);                      // 隐藏边框线
  table->setEditTriggers(QAbstractItemView::CurrentChanged);  // 单击编辑
  table->horizontalHeader()->setHighlightSections(
      false);  // 取消上侧表头点击时高亮
  table->verticalHeader()->setHighlightSections(false);
  table->setStyleSheet(
      "selection-background-color:#F8F8FF;selection-color:red");  // 设置选中单元格背景色和字体颜色
  table->verticalHeader()->setStyleSheet(
      "QHeaderView::section{background:#FFDEAD;}");
  table->horizontalHeader()->setStyleSheet(
      "QHeaderView::section{background:#FFDEAD;}");

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
}

void setColHeader(QTableWidget* table, const QVector<QString>& v) {
  if (v.size() >= table->columnCount()) {
    for (int i = 0; i < table->columnCount(); ++i) {
      auto item = new QTableWidgetItem(v[i]);
      item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
      table->setHorizontalHeaderItem(i, item);
    }
    table->verticalHeader()->setVisible(false);
  }
}

void setRowHeader(QTableWidget* table, const QVector<QString>& v) {
  if (v.size() >= table->rowCount()) {
    for (int i = 0; i < table->rowCount(); ++i) {
      auto item = new QTableWidgetItem(v[i]);
      item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
      table->setVerticalHeaderItem(i, item);
    }
    table->horizontalHeader()->setVisible(false);
  }
}

void setEmptyCell(QTableWidget* table, int colStart, int colEnd) {
  if (colEnd == -1) colEnd = table->columnCount();
  for (int i = 0; i < table->rowCount(); ++i) {
    for (int j = colStart; j <= colEnd; ++j) {
      auto item = new QTableWidgetItem(u8"");
      table->setItem(i, j, item);
    }
  }
}

template <typename T,
          typename = std::enable_if_t<std::is_convertible_v<T*, QTableWidget*>>>
void setAutoSize(T* table) {
  const int defaultHeight = 30;
  table->verticalHeader()->setDefaultSectionSize(defaultHeight);
  if (table->horizontalHeaderItem(0) != 0) {
    table->horizontalHeader()->setFixedHeight(defaultHeight);
    for (int i = 0; i < table->columnCount(); ++i) {
      if (table->horizontalHeaderItem(i)->text().length() > 9) {
        table->setColumnWidth(i, 200);
      } else if (table->horizontalHeaderItem(i)->text().length() > 5) {
        table->setColumnWidth(i, 150);
      } else {
        table->setColumnWidth(i, 100);
      }
    }
    int count = 0;
    for (int i = 0; i < table->columnCount(); ++i) {
      if (table->horizontalHeaderItem(i)->text().length() > 5) {
        count += table->horizontalHeaderItem(i)->text().length() / 4;
      }
    }
    table->setFixedWidth(100 * table->columnCount() + 20 + 40 * count);
    table->setFixedHeight((table->rowCount() + 1) * defaultHeight);
  } else {
    const int wd = 300 / table->columnCount();
    for (int i = 0; i < table->columnCount(); ++i) {
      table->setColumnWidth(i, wd);
    }
    int maxLength = 0;
    for (int i = 0; i < table->rowCount(); ++i) {
      if (table->verticalHeaderItem(i)->text().length() > maxLength) {
        maxLength = table->verticalHeaderItem(i)->text().length();
      }
    }
    table->setFixedWidth(wd * table->columnCount() + maxLength * 12);
    table->setFixedHeight(table->rowCount() * defaultHeight);
  }
}

template <typename T, typename... Ts>
void setAutoSize(T* table, Ts*... ts) {
  if constexpr (std::is_convertible_v<T*, QTableWidget*>) {
    setAutoSize(table);
  }
  setAutoSize(ts...);
}
