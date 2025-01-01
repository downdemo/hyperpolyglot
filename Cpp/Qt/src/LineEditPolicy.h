void setLineEditWithButton(QLineEdit* lineEdit, QPushButton* button) {
  button->setFixedWidth(50);
  auto layout = new QHBoxLayout;
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(button, 0, Qt::AlignRight);
  lineEdit->setLayout(layout);
}
