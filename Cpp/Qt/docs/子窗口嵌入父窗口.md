```cpp
// 给子窗口的构造函数加上
setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
hide();

// 默认最大化
setWindowState(Qt::WindowMaximized);

void MainWindow::showDlg() {
  dlg* d;
  d = new dlg(this);
  d->move(100, 0);
  d->show();
}

void MainWindow::on_toolButton_clicked() { showDlg(); }
```

* 去掉窗口右下角角标的方法：打开 Designer，在对象查看器中选择 QStatusBar，取消勾选其属性中的 SizeGripEnable
