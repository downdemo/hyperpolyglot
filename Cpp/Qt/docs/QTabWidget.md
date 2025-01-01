[QTabWidget](http://doc.qt.io/qt-5/qtabwidget.html)

## 文字横向显示

* 需要设置 QProxyStyle 风格样式，新建一个样式头文件

```cpp
// CustomTabStyle.h
#pragma once

#include <QPainter>
#include <QProxyStyle>
#include <QStyleOption>

class CustomTabStyle : public QProxyStyle {
 public:
  /* sizeFromContents 用于设置 Tab 标签大小
   * 1. 获取原标签大小
   * 2· 宽高切换
   * 3· 强制宽高
   * 4. return
   * */
  QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                         const QSize &size, const QWidget *widget) const {
    QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
    if (type == QStyle::CT_TabBarTab) {
      s.transpose();
      s.rwidth() = 100;  // 设置每个 tabBar 中 item 的大小
      s.rheight() = 50;
    }
    return s;
  }

  /* drawControl 画控件
   * 1. 过滤 CE_TabBarTabLabel
   * 2. 选择状态下的背景绘制
   *      a. 获取Rect
   *      b. 保存旧的画笔
   *      c. 设置新画笔
   *      d. 画 Rect
   *      e. 恢复旧的画笔
   * 3. 设定 Text 对齐及选中和非选中状态下的 Text 颜色
   * 4. 过滤 CE_TabBarTab，对其进行绘制
   * */
  void drawControl(ControlElement element, const QStyleOption *option,
                   QPainter *painter, const QWidget *widget) const {
    if (element == CE_TabBarTabLabel) {
      if (const QStyleOptionTab *tab =
              qstyleoption_cast<const QStyleOptionTab *>(option)) {
        QRect allRect = tab->rect;
        if (tab->state & QStyle::State_Selected) {
          painter->save();
          // tab 边框线颜色
          painter->setPen(0xFFDEAD);
          // tab 背景色
          painter->setBrush(QBrush(0x6A5ACD));
          // 调整边距
          painter->drawRect(allRect.adjusted(0, 0, -1, -1));
          painter->restore();
          // 选中 tab 的文字颜色
          painter->setPen(0xFAFFF0);
        } else {
          // 未选中 tab 的文字颜色
          painter->setPen(0x6A5ACD);
          // 未选中 tab 的背景色
          painter->setBrush(QBrush(0xF8F8FF));
          painter->drawRect(allRect.adjusted(0, 0, -1, -1));
        }
        QTextOption textOption;
        textOption.setAlignment(Qt::AlignCenter);
        QFont font;
        font.setBold(true);
        font.setFamily(tr(u8"KaiTi"));
        font.setPointSize(12);
        font.setLetterSpacing(QFont::AbsoluteSpacing, 1);
        painter->setFont(font);
        painter->drawText(allRect, tab->text, textOption);
        return;
      }
    }
    if (element == CE_TabBarTab) {
      QProxyStyle::drawControl(element, option, painter, widget);
    }
  }
};
```

* 再直接设置即可

```cpp
tabWidget->setTabPosition(QTabWidget::West);
tabWidget->tabBar()->setStyle(new CustomTabStyle);
```

## 其他常用操作

```cpp
tab->insertTab(0, w, u8"页面1");  // 插入 tab 到索引为 0 的位置
tab->removeTab(2);                // 移除索引为 2 的 tab
tab->setCurrentIndex(2);          // 跳转到索引为 2 的 tab
```
