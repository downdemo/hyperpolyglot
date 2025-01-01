## 准备工作

* 包含头文件

```cpp
#include <ActiveQt/qaxobject.h>
```

* 链接器 - 输入 - 附加依赖项：`Qt5AxBased.lib;Qt5AxContainerd.lib;` 否则报错如下

```cpp
1>Qt5AxContainerd.lib(qaxbase.obj) : error LNK2019: 无法解析的外部符号 "bool __cdecl QVariantToVoidStar(class QVariant const &,void *,class QByteArray const &,unsigned int)" (?QVariantToVoidStar@@YA_NAEBVQVariant@@PEAXAEBVQByteArray@@I@Z)，该符号在函数 "private: int __cdecl QAxBase::internalProperty(enum QMetaObject::Call,int,void * *)" (?internalProperty@QAxBase@@AEAAHW4Call@QMetaObject@@HPEAPEAX@Z) 中被引用
1>Qt5AxContainerd.lib(qaxbase.obj) : error LNK2019: 无法解析的外部符号 "void __cdecl clearVARIANT(struct tagVARIANT *)" (?clearVARIANT@@YAXPEAUtagVARIANT@@@Z)，该符号在函数 "public: class QAxObject * __cdecl QAxBase::querySubObject(char const *,class QList<class QVariant> &)" (?querySubObject@QAxBase@@QEAAPEAVQAxObject@@PEBDAEAV?$QList@VQVariant@@@@@Z) 中被引用
1>Qt5AxContainerd.lib(qaxtypes.obj) : error LNK2001: 无法解析的外部符号 "void __cdecl clearVARIANT(struct tagVARIANT *)" (?clearVARIANT@@YAXPEAUtagVARIANT@@@Z)
1>Qt5AxContainerd.lib(qaxtypes.obj) : error LNK2019: 无法解析的外部符号 "class QColor __cdecl OLEColorToQColor(unsigned int)" (?OLEColorToQColor@@YA?AVQColor@@I@Z)，该符号在函数 "class QVariant __cdecl VARIANTToQVariant_container(struct tagVARIANT const &,class QByteArray const &,unsigned int)" (?VARIANTToQVariant_container@@YA?AVQVariant@@AEBUtagVARIANT@@AEBVQByteArray@@I@Z) 中被引用
1>C:\Project\tabtest\x64\Debug\\tabtest.exe : fatal error LNK1120: 3 个无法解析的外部命令
```

* QAxObject 派生自 QAxBase，QAxBase 通过 ActiveX（OLE）操作 COM 对象（Excel），对象关系如下
* Application-WorkBooks-Sheets
  * UsedRange：`property("Row").toInt()` 为起始行，`property("Column").toInt()` 为起始列
    * Rows：`property("Count").toInt()` 为总行数
    * Columns：`property("Count").toInt()` 为总列数
  * Cells

## 写入到文件

```cpp
void dlg::slot() {
  QString path = QFileDialog::getSaveFileName(
      this, tr(u8"另存为"), tr(u8"C:\\Users\\downdemo\\Desktop\\test"),
      tr(u8"Excel文件(*.xlsx);"));
  if (!path.isEmpty()) {
    QAxObject excel("Excel.Application");
    // 设置为 true 则会在写入同时显示文件
    excel.setProperty("Visible", false);
    QAxObject* workbooks = excel.querySubObject("WorkBooks");
    // 添加新工作簿
    workbooks->dynamicCall("Add");
    // 获取活动工作簿
    QAxObject* workbook = excel.querySubObject("ActiveWorkBook");
    // 获取所有工作表，querySubObject("WorkSheets")也可以
    QAxObject* worksheets = workbook->querySubObject("Sheets");
    // 获取第一张工作表
    QAxObject* sheet1 = worksheets->querySubObject("Item(int)", 1);
    // 删除工作表，若该表不存在则报异常
    // sheet1->dynamicCall("delete");
    // 获取单元格并设置值，单元格从 (1, 1) 而非 (0, 0) 开始
    QAxObject* cell = sheet1->querySubObject("Cells(int,int)", 1, 1);
    cell->setProperty("Value2", "111");
    // 设置第一行第二列单元格
    QAxObject* cell2 = sheet1->querySubObject("Cells(int,int)", 1, 2);
    cell2->setProperty("Value2", "222");
    // 设置工作表名称
    sheet1->setProperty("Name", tr(u8"table1"));
    // 获取工作表数量
    // int sheetCount = worksheets->property("Count").toInt();
    // 添加新工作表到第一张表之后
    QAxObject* last = worksheets->querySubObject("Item(int)", 1);
    QAxObject* sheet2 =
        worksheets->querySubObject("Add(QVariant)", last->asVariant());
    last->dynamicCall("Move(QVariant)", sheet2->asVariant());
    // 设置新工作表名称
    sheet2->setProperty("Name", tr(u8"table2"));
    // 添加 table3
    QAxObject* last2 = worksheets->querySubObject("Item(int)", 2);
    QAxObject* sheet3 =
        worksheets->querySubObject("Add(QVariant)", last2->asVariant());
    last2->dynamicCall("Move(QVariant)", sheet3->asVariant());
    sheet3->setProperty("Name", tr(u8"table3"));
    // 添加 table4
    QAxObject* last3 = worksheets->querySubObject("Item(int)", 3);
    QAxObject* sheet4 =
        worksheets->querySubObject("Add(QVariant)", last3->asVariant());
    last3->dynamicCall("Move(QVariant)", sheet4->asVariant());
    sheet4->setProperty("Name", tr(u8"table4"));
    // 打开 Excel 时默认显示第一张工作表
    sheet1->querySubObject("Activate");
    // 另存为
    workbook->dynamicCall(
        "SaveAs(const QString&, int, const QString&, const QString&, bool, "
        "bool)",
        QDir::toNativeSeparators(path), 56, QString(""), QString(""), false,
        false);
    // 关闭文件
    workbook->dynamicCall("Close(Boolean)", false);
    // 退出
    excel.dynamicCall("Quit(void)");
  }
}
```

## 从文件读取

```cpp
void dlg::slot2() {
  QString path = QFileDialog::getOpenFileName(this, tr(u8"打开"),
                                              "C:\\Users\\downdemo\\Desktop\\",
                                              tr(u8"Excel文件(*.xlsx);"));
  if (!path.isEmpty()) {
    QAxObject excel("Excel.Application");
    // 设置为 true 则会在写入同时显示文件
    excel.setProperty("Visible", false);
    // 更改标题栏
    excel.setProperty("Caption", "My Excel");
    // 获取标题
    QVariant titleValue = excel.property("Caption");
    QString titleName = titleValue.toString();
    qDebug() << titleName;  // 打印 "My Excel"
    QAxObject* workbooks = excel.querySubObject("WorkBooks");
    // 打开已有工作簿
    workbooks->dynamicCall("Open(const QString&)", path);
    // 获取活动工作簿
    QAxObject* workbook = excel.querySubObject("ActiveWorkBook");
    // 获取所有工作表，querySubObject("WorkSheets")也可以
    QAxObject* worksheets = workbook->querySubObject("Sheets");
    // 获取第一张工作表
    QAxObject* sheet1 = worksheets->querySubObject("Item(int)", 1);
    // 删除工作表，若该表不存在则报异常
    // sheet1->dynamicCall("delete");
    // 获取sheet1名称
    QString sheetName = sheet1->property("Name").toString();
    // 获取范围以及行列数
    QAxObject* usedrange = sheet1->querySubObject("UsedRange");
    QAxObject* rows = usedrange->querySubObject("Rows");
    QAxObject* columns = usedrange->querySubObject("Columns");
    int rowStart = usedrange->property("Row").toInt();
    int colStart = usedrange->property("Column").toInt();
    int rowCount = rows->property("Count").toInt();
    int colCount = columns->property("Count").toInt();
    // 打印sheet1的表格内容
    for (int i = rowStart; i < rowStart + rowCount; ++i) {
      for (int j = colStart; j < colStart + colCount; ++j) {
        QAxObject* cell = sheet1->querySubObject("Cells(int,int)", i, j);
        qDebug() << i << j << cell->property("Value2").toString();
      }
    }
    // 另一种读取范围批量打印的方法
    QString Range = "A1:";
    Range.append(QChar('A' + colCount - 1)).append(QString::number(rowCount));
    QAxObject* data = sheet1->querySubObject("Range(const QString&)", Range);
    QVariant dataValue = data->property("Value2");
    QVariantList dataList = dataValue.toList();
    for (int i = 0; i < rowCount; ++i) {
      QVariantList outList = dataList[i].toList();
      for (int j = 0; j < colCount; ++j) {
        qDebug() << i + 1 << j + 1 << outList[j].toString();
      }
    }
    // 保存工作簿
    workbook->dynamicCall("Save");
    // 关闭文件
    workbook->dynamicCall("Close(Boolean)", false);
    // 退出
    excel.dynamicCall("Quit(void)");
  }
}
```

## 其他操作

```cpp
cell->setProperty("RowHeight", 20);    // 设置行高
cell->setProperty("ColumnWidth", 30);  // 设置列宽
// 默认水平靠左，垂直居中
// 水平居中，靠左 -4131，靠右 -4152
cell->setProperty("HorizontalAlignment", -4108);
// 垂直居中，靠上 -4160，靠下 -4107
cell->setProperty("VerticalAlignment", -4108);
cell->setProperty("WrapText", true);   // 超过列宽自动换行
cell->dynamicCall("ClearContents()");  // 清空单元格内容
// 单元格背景色
QAxObject* interior = cell->querySubObject("Interior");
// 单元格背景色（红色）
interior->setProperty("Color", QColor(255, 0, 0));
// 边框色
QAxObject* border = cell->querySubObject("Borders");
// 边框色（绿色）
border->setProperty("Color", QColor(0, 255, 0));
// 字体
QAxObject* font = cell->querySubObject("Font");
// 字体色（蓝色）
font->setProperty("Color", QColor(0, 0, 255));
// 字体大小
font->setProperty("Size", 12);
font->setProperty("Name", QStringLiteral("仿宋"));
// 加粗
font->setProperty("Bold", true);
// 斜体
font->setProperty("Italic", true);
// 下划线
font->setProperty("Underline", true);
// 合并单元格范围 A3:G7
// 合并实际只保留左上角单元格数据，范围内其他单元格数据为空
QString merge;
merge.append(QChar('A'));
merge.append(QString::number(3));
merge.append(":");
merge.append(QChar('A' + 6));
merge.append(QString::number(7));
QAxObject* mergeRange = sheet1->querySubObject("Range(const QString&)", merge);
// 靠左 -4131，靠右 -4152
mergeRange->setProperty("HorizontalAlignment", -4108);
// 靠上 -4160，靠下 -4107
mergeRange->setProperty("VerticalAlignment", -4108);
// 合并单元格
mergeRange->setProperty("MergeCells", true);
// 拆分单元格
// mergeRange->setProperty("MergeCells", false);
```

## 从 QTableWidget 导出

```cpp
// 只有行表头的表格
for (int i = 0; i < table1->rowCount(); ++i) {
  QAxObject* cell = sheet1->querySubObject("Cells(int, int)", i + 1, 1);
  cell->setProperty("Value2", table1->verticalHeaderItem(i)->text());
}
for (int i = 0; i < table1->rowCount(); ++i) {
  for (int j = 0; j < table1->columnCount(); ++j) {
    QAxObject* cell = sheet1->querySubObject("Cells(int, int)", i + 1, j + 2);
    if (table1->item(i, j) != nullptr) {
      cell->setProperty("Value2", table1->item(i, j)->text());
    }
  }
}
// 只有列表头的表格
for (int j = 0; j < table2->columnCount(); ++j) {
  QAxObject* cell = sheet2->querySubObject("Cells(int, int)", 1, j + 1);
  cell->setProperty("Value2", table2->horizontalHeaderItem(j)->text());
}
for (int i = 0; i < table2->rowCount(); ++i) {
  for (int j = 0; j < table2->columnCount(); ++j) {
    QAxObject* cell = sheet2->querySubObject("Cells(int, int)", i + 2, j + 1);
    if (table2->item(i, j) != nullptr) {
      cell->setProperty("Value2", table2->item(i, j)->text());
    }
  }
}
```

* 通过 setCellWidget 嵌入了 QComboBox 的表格

```cpp
// 假设只有列表头
for (int j = 0; j < table->columnCount(); ++j) {
  QAxObject* cell = sheet2->querySubObject("Cells(int, int)", 1, j + 1);
  cell->setProperty("Value2", table->horizontalHeaderItem(j)->text());
}
for (int i = 0; i < table->rowCount(); ++i) {
  for (int j = 0; j < table->columnCount(); ++j) {
    QAxObject* cell = sheet2->querySubObject("Cells(int, int)", i + 2, j + 1);
    if (j == 0 || j == 1 || j == 2) {  // 前三列都是 QComboBox
      cell->setProperty(
          "Value2",
          qobject_cast<QComboBox*>(table->cellWidget(i, j))->currentText());
    } else if (table->item(i, j) != nullptr) {  // 其他列和之前的处理方式相同
      cell->setProperty("Value2", table->item(i, j)->text());
    }
  }
}
```

## 错误情况

* 如果报错如下则需要手动初始化创建 COM 对象

```
CoCreateInstance failure(尚未调用 CoInitialize)
QAxBase::setControl : requested control Excel.Application could not be instantiated
QAxBase::dynamicCallHelper : Object is not initialized, or initialization failed
```

* 方法如下

```cpp
#include "qt_windows.h"

// 在操作前加上
HRESULT r = OleInitialize(0);
CoInitialize(0);
if (r != S_OK && r != S_FALSE) {
  qWarning("Qt: Could not initialize OLE (error %x)\n", (unsigned int)r);
}

// 结束加上释放代码
OleUninitialize();
```

* 报错如下则是因为没有安装 Excel（而不是 WPS），缺少 Excel 组件服务

```
CoCreateInstance failure (没有注册类)
QAxBase::setControl: requested control Excel.Application could not be instantiated
QAxBase::dynamicCallHelper: Object is not initialized, or initialization failed
0x00007FF7E57B4EBE 处(位于 tabtest.exe 中)引发的异常: 0xC0000005: 读取位置 0x0000000000000018 时发生访问冲突。
```

* 如果存在 Excel 组件，WIN+R 运行 `dcomcnfg` 打开组件服务，查看 `组件服务 - 计算机 - 我的电脑 - DCOM 配置`，其中可以看到 `Mircosoft Excel Application`
