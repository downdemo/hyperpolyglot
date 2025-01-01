[QFileDialog](http://doc.qt.io/qt-5/qfiledialog.html)

```cpp
#include "qfiledialog.h"
void MainWindow::slot() {
  // 保存用getSaveFileName
  QString path = QFileDialog::getOpenFileName(this, tr("Open File"),
                                              "C:/Users/downdemo/Desktop",
                                              tr("Text File(*.txt)"));
  // std::string str = path.toStdString());
  if (!path.isEmpty()) {
    lineEdit->setText(path);
    lineEdit->setFocus();
    lineEdit->selectAll();
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QMessageBox::warning(this, tr("Read File"),
                           tr("Cannot open file:\n%1").arg(path));
      return;
    }
    QTextStream in(&file);
    textEdit->setText(in.readAll());
    file.close();
  } else {
    QMessageBox::warning(this, tr("Path"), tr("You did not select any file."));
  }
}
```
