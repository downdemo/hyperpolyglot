```cpp
Widget::Widget(QWidget* parent) : QMainWindow(parent), ui(new Ui::Widget) {
  ui->setupUi(this);
  layout = new QVBoxLayout(this);
  menuBar = new QMenuBar(this);
  menuBar->setGeometry(QRect(0, 0, 400, 24));
  QMenu* menu = menuBar->addMenu(tr("&File"));
  actionNew = menu->addAction(tr("&New"));
  actionNew->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
  QMenu* menu2 = menuBar->addMenu(tr("&File2"));
  layout->setMenuBar(menuBar);
  setLayout(layout);
}
```
