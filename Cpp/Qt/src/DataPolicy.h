void getData(const std::string& s, std::vector<double>& v) {
  if (!std::empty(v)) {
    v.clear();
  }
  std::regex r(
      "[^,，]?[[:digit:]]+(\\.[[:digit:]]+)?([[:space:]]*\\*[[:space:]]*[[:"
      "digit:]]+(\\.[[:digit:]]+)?)?[^,，]?");
  std::smatch results;
  auto it = std::cbegin(s);
  auto endIt = std::cend(s);
  while (std::regex_search(it, endIt, results, r)) {
    std::string toDeal = results[0];
    auto pos = toDeal.find("*");
    if (pos != std::string::npos) {
      for (int i = 0; i < std::stoi(toDeal.substr(0, pos)); ++i) {
        auto toDealNum =
            std::stod(toDeal.substr(pos + 1, toDeal.size() - 1 - pos));
        if (!std::empty(v)) {
          v.emplace_back(toDealNum + v.back());
        } else {
          v.emplace_back(toDealNum);
        }
      }
    } else {
      if (!std::empty(v)) {
        v.emplace_back(std::stod(toDeal) + v.back());
      } else {
        v.emplace_back(std::stod(toDeal));
      }
    }
    it = results[0].second;
  }
  v.shrink_to_fit();
}

void getData(const QString& s, std::vector<double>& v) {
  if (!std::empty(v)) {
    v.clear();
  }
  if (s.contains(QRegExp("\\d"))) {
    auto strList = s.split(QRegExp(u8",|，"));
    for (auto& x : strList) {
      if (x.contains('*')) {
        auto list = x.split("*");
        for (int i = 0; i < list[0].toInt(); ++i) {
          if (!std::empty(v)) {
            v.emplace_back(list[1].toDouble() + v.back());
          } else {
            v.emplace_back(list[1].toDouble());
          }
        }
      } else {
        if (x.contains(QRegExp("\\d"))) {
          if (!std::empty(v)) {
            v.emplace_back(x.toDouble() + v.back());
          } else {
            v.emplace_back(x.toDouble());
          }
        }
      }
    }
  }
  v.shrink_to_fit();
}

void normalization(const std::vector<double>& v,
                   std::vector<double>& normVector, double normVal) {
  if (!std::empty(normVector)) {
    normVector.clear();
  }
  normVector.reserve(v.size());
  // const auto[minIt, maxIt]( std::minmax_element(std::begin(v), std::end(v)));
  const auto maxIt = std::max_element(std::begin(v), std::end(v));
  std::transform(std::begin(v), std::end(v), std::back_inserter(normVector),
                 [&](double x) { return x * normVal / *maxIt; });
  normVector.shrink_to_fit();
}

void mergeVector(std::vector<double>& v, const std::vector<double>& v1,
                 const std::vector<double>& v2) {
  v.clear();
  v.assign(std::begin(v1), std::end(v1));
  v.insert(std::end(v), std::begin(v2), std::end(v2));
  v.shrink_to_fit();
}

std::string QStringToStdString(const QString& qstr) {
  auto byteArray = qstr.toLocal8Bit();
  std::string s(byteArray.data());
  return s;
}

QString StdStringToQString(const std::string& str) {
  return QString::fromLocal8Bit(str.c_str());
}

QString QVectorToQString(const QVector<int>& v) {
  QString s;
  if (!v.empty()) {
    for (const auto& x : v) {
      s.append(x).append("; ");
    }
    s.chop(2);
  }
  return s;
}
