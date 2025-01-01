void quickSetComboBoxStyleSheet(QComboBox* comboBox) {
  auto delegate = new QStyledItemDelegate;
  comboBox->setItemDelegate(delegate);
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
      "QComboBox QAbstractItemView::item {min-height: 30px;}");
}

template <typename T, typename... Ts>
void quickSetComboBoxStyleSheet(T comboBox, Ts... ts) {
  if constexpr (std::is_convertible_v<T, QComboBox*>) {
    quickSetComboBoxStyleSheet(comboBox);
  }
  quickSetComboBoxStyleSheet(ts...);
}
