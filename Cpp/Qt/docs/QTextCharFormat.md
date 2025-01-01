* 设置文本颜色

```cpp
void quickSetTextEditColor(QTextEdit* textEdit, Qt::GlobalColor color) {
  QTextCharFormat format;
  format.setForeground(color);
  textEdit->mergeCurrentCharFormat(format);
}
```
