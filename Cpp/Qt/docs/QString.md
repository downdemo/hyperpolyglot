## 中文编码

* 中文显示

```cpp
// 方案 1：C++11 的 u8 前缀表示 UTF-8 编码
QMenu* menu = menuBar->addMenu(tr(u8"测试"));

// 方案 2：不要求支持 C++11
#include "qstring.h"
QMenu* menu = menuBar->addMenu(QString::fromLocal8Bit("测试"));
```

* 中文 std::string 转 QString 打印

```cpp
std::string s = "测试";
qDebug() << QString(QString::fromLocal8Bit(s.c_str()));
```

* 中文 QString 转 std::string

```cpp
QString qstr = tr(u8"测试");
// QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
QByteArray cdata = qstr.toLocal8Bit();
std::string str = std::string(cdata);
// 或者直接
std::string str = qstr.toLocal8Bit();
```

* 无中文时 std::string 与 QString 的互转

```cpp
std::string str = qstr.toStdString();
QString qstr = QString::fromStdString(str);
```

* QVariant 转 QString

```cpp
QVariant v;
QString = v.toString();
```

* QString 转 QVariant

```cpp
QString s;
QVariant v(s);
```

## 正则表达式

* 字符串表示两点间距，目的是解析为到起点距离。如字符串 `100, 2*200, 300` 需要解析为元素为 `100、300、500、800` 的容器

```cpp
void getData(const QString& s, std::vector<double>& v) {
  v.clear();
  if (s.contains(QRegExp("\\d"))) {
    auto strList = s.split(QRegExp(u8",|，"));
    for (auto& x : strList) {
      if (x.contains('*')) {
        auto list = x.split("*");
        for (int i = 0; i < list[0].toInt(); ++i) {
          if (!v.empty()) {
            v.emplace_back(list[1].toDouble() + v.back());
          } else {
            v.emplace_back(list[1].toDouble());
          }
        }
      } else {
        if (x.contains(QRegExp("\\d"))) {
          if (!v.empty()) {
            v.emplace_back(x.toDouble() + v.back());
          } else {
            v.emplace_back(x.toDouble());
          }
        }
      }
    }
  }
}
```

* 标准库实现

```cpp
void getData(const std::string& s, std::vector<double>& v) {
  v.clear();
  std::regex r(
      "[^,，]?[[:digit:]]+(\\.[[:digit:]]+)?([[:space:]]*\\*[[:space:]]*[[:"
      "digit:]]+(\\.[[:digit:]]+)?)?[^,，]?");
  std::smatch results;
  auto it = s.cbegin();
  auto endIt = s.cend();
  while (std::regex_search(it, endIt, results, r)) {
    std::string toDeal = results[0];
    auto pos = toDeal.find("*");
    if (pos != std::string::npos) {
      for (int i = 0; i < std::stoi(toDeal.substr(0, pos)); ++i) {
        auto toDealNum =
            std::stod(toDeal.substr(pos + 1, toDeal.size() - 1 - pos));
        if (!v.empty()) {
          v.emplace_back(toDealNum + v.back());
        } else {
          v.emplace_back(toDealNum);
        }
      }
    } else {
      if (!v.empty()) {
        v.emplace_back(std::stod(toDeal) + v.back());
      } else {
        v.emplace_back(std::stod(toDeal));
      }
    }
    it = results[0].second;
  }
}
```
