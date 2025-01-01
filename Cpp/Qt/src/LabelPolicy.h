template <typename T>
void quickSetLabelStyle(T* label) {
  label->setFixedSize(QSize(160, 25));
  QFont font;
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPointSize(11);
  label->setFont(font);
  label->setStyleSheet("border:2px solid #242424; background: yellow;");
  if constexpr (std::is_convertible_v<T*, QLabel*>) {
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  }
}

template <typename T, typename... Ts>
void quickSetLabelStyle(T* label, Ts*... ts) {
  quickSetLabelStyle(label);
  quickSetLabelStyle(ts...);
}

template <typename T>
void quickSetLabelTextBold(T* label) {
  QFont font;
  font.setFamily("KaiTi");
  font.setBold(true);
  font.setPointSize(11);
  label->setFont(font);
  label->setAlignment(Qt::AlignLeft);
}

template <typename T, typename... Ts>
void quickSetLabelTextBold(T* label, Ts*... ts) {
  quickSetLabelTextBold(label);
  quickSetLabelTextBold(ts...);
}
