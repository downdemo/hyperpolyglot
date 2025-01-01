template <typename T>
void quickSetWidth(int wd, T* label) {
  label->setFixedWidth(wd);
}

template <typename T, typename... Ts>
void quickSetWidth(int wd, T* label, Ts*... ts) {
  quickSetWidth(wd, label);
  quickSetWidth(wd, ts...);
}
