```cpp
void MyWidget::slots() {
  QMessageBox messageBox;
  messageBox.setWindowTitle(tr(u8"窗口标题"));
  messageBox.setText(tr(u8"内容"));
  messageBox.setStandardButtons(QMessageBox::StandardButton::Ok |
                                QMessageBox::StandardButton::Cancel);
  messageBox.setButtonText(QMessageBox::StandardButton::Ok, tr(u8"是"));
  messageBox.setButtonText(QMessageBox::StandardButton::Cancel, tr(u8"否"));
  messageBox.setDefaultButton(QMessageBox::StandardButton::Cancel);
  messageBox.setIcon(QMessageBox::Question);
  messageBox.exec();
  auto ret = messageBox.standardButton(messageBox.clickedButton());
  if (ret == QMessageBox::Ok) {
    // 执行的操作
  }
}
```
