void quickSetLayoutAlign(QBoxLayout* layout, QWidget* w,
                         Qt::AlignmentFlag alignmentFlag) {
  layout->addWidget(w);
  layout->setMargin(0);
  layout->setAlignment(w, alignmentFlag);
}

template <typename T>
void quickSetLayout(QBoxLayout* layout, T x) {
  if constexpr (std::is_same_v<T, QMenuBar*>) {
    layout->setMenuBar(x);
  } else if constexpr (std::is_convertible_v<T, QWidget*>) {
    layout->addWidget(x);
  } else if constexpr (std::is_convertible_v<T, int>) {
    layout->addSpacing(x);
  } else if constexpr (std::is_convertible_v<T, QLayout*>) {
    layout->addLayout(x);
  }
}

template <typename T, typename... Ts>
void quickSetLayout(QBoxLayout* layout, T x, Ts... ts) {
  quickSetLayout(layout, x);
  quickSetLayout(layout, ts...);
}

template <typename T>
auto addLeftSpacing(T&& w, int spacing = 20) {
  QPointer<QHBoxLayout> layout = new QHBoxLayout;
  layout->addSpacing(spacing);
  quickSetLayout(layout, spacing, std::forward<T>(w));
  return layout;
}
