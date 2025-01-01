* mainwindow.h

```cpp
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QInputDialog>
#include <QLineEdit>
#include <QListView>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QStringListModel>
#include <QVBoxLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = 0);
  ~MainWindow();
 private slots:
  void insertData();
  void deleteData();
  void showData();

 private:
  Ui::MainWindow* ui;

  QStringListModel* model;
  QListView* listView;
};

#endif  // MAINWINDOW_H
```

* mainwindow.cpp

```cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  QStringList data;
  data << "data1"
       << "data2"
       << "data3"
       << "data4";
  model = new QStringListModel(this);
  model->setStringList(data);

  listView = new QListView(this);
  listView->setModel(model);

  QHBoxLayout *btnLayout = new QHBoxLayout;
  QPushButton *insertBtn = new QPushButton(tr("insert"), this);
  connect(insertBtn, SIGNAL(clicked()), this, SLOT(insertData()));
  QPushButton *delBtn = new QPushButton(tr("Delete"), this);
  connect(delBtn, SIGNAL(clicked()), this, SLOT(deleteData()));
  QPushButton *showBtn = new QPushButton(tr("Show"), this);
  connect(showBtn, SIGNAL(clicked()), this, SLOT(showData()));
  btnLayout->addWidget(insertBtn);
  btnLayout->addWidget(delBtn);
  btnLayout->addWidget(showBtn);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(listView);
  mainLayout->addLayout(btnLayout);

  QWidget *centralWidget = new QWidget();
  centralWidget->setLayout(mainLayout);
  setCentralWidget(centralWidget);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::insertData() {
  bool isOK;
  QString text = QInputDialog::getText(
      this, "Insert", "input new data:", QLineEdit::Normal, "new data", &isOK);
  if (isOK) {
    int row = listView->currentIndex().row();
    model->insertRows(row, 1);
    QModelIndex index = model->index(row);
    model->setData(index, text);
    listView->setCurrentIndex(index);
    listView->edit(index);
  }
}

void MainWindow::deleteData() {
  if (model->rowCount() > 1) {
    model->removeRows(listView->currentIndex().row(), 1);
  }
}

void MainWindow::showData() {
  QStringList data = model->stringList();
  QString str;
  foreach (QString s, data) {
    str += s + "\n";
  }
  QMessageBox::information(this, "Data", str);
}
```
