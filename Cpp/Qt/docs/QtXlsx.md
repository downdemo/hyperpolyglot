* 下载并解压 [QtXlsx 源码包](https://github.com/VSRonin/QtXlsxWriter)
* 进入 VS 所在目录，如 `C:\Program Files (x86)\Microsoft Visual Studio`，找到 `cl.exe`（所在路径如下），将其所在路径添加到环境变量中，注销或重启以使环境变量生效

```
C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\bin\Hostx64\x64
```

* 进入 VS 所在目录，找到批处理文件 `vcvarsall.bat`。打开 cmd，将 `vcvarsall.bat` 拖到命令框中，并在其后加上 `x64` 表示配置环境为 x64，命令如下，回车即可执行此批处理文件

```
"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
```

* 命令行将提示

```
**********************************************************************
** Visual Studio 2017 Developer Command Prompt v15.9.5
** Copyright (c) 2017 Microsoft Corporation
**********************************************************************
[vcvarsall.bat] Environment initialized for: 'x64'
```

* 进入 `QtXlsxWriter-master` 文件夹

```
cd C:\Users\downdemo\Desktop\QtXlsxWriter-master
qmake
# Info: creating stash file C:\Users\wujiacheng\Desktop\QtXlsxWriter-master\.qmake.stash
# Info: creating cache file C:\Users\wujiacheng\Desktop\QtXlsxWriter-master\.qmake.cache
nmake
nmake install
```

* 将自动把需要的文件拷贝到目标位置
* 打开 VS 创建一个 Qt 项目，在 `配置属性 - 链接器 - 输入 - 附加依赖项` 中添加 `Qt5Xlsxd.lib;`
* 源文件代码如下，运行即可在项目路径下看到生成的 `.xlsx` 文件

```cpp
#include <QtXlsx/QtXlsx>

int main(int argc, char *argv[]) {
  QXlsx::Document xlsx;
  xlsx.write("A1", "Hello Qt!");
  xlsx.saveAs("Test.xlsx");
}
```
