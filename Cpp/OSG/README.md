* [OSG 官网](http://www.openscenegraph.org/)
* [OSG 中文网](http://www.osgchina.org/)

## VS2017 编译 OSG3.7.0

* 编译过程参考 [Compiling with Visual Studio](http://www.openscenegraph.org/index.php/documentation/platform-specifics/windows/37-visual-studio)，需要的安装包可在 [osgChina](http://www.osgchina.org) 或 [OSG 官网](http://www.openscenegraph.org/) 获取。以下是此次编译过程中所用的工具版本及其下载地址：
  * [源码包](http://www.openscenegraph.org/index.php/download-section/stable-releases)：[OpenSceneGraph-3.7.0](https://codeload.github.com/openscenegraph/OpenSceneGraph/zip/master)
  * [第三方库依赖](http://www.openscenegraph.org/index.php/download-section/dependencies)：[VS2017 全部依赖 64 位](https://download.osgvisual.org/3rdParty_VS2017_v141_x64_V11_full.7z)
  * [数据包](http://www.openscenegraph.org/index.php/download-section/data)：[OpenSceneGraph-Data-3.4.0.zip](http://www.openscenegraph.org/downloads/stable_releases/OpenSceneGraph-3.4.0/data/OpenSceneGraph-Data-3.4.0.zip)
  * [CMeke](https://cmake.org/download/)：[cmake-3.13.3-win64-x64.msi](https://github.com/Kitware/CMake/releases/download/v3.13.3/cmake-3.13.3-win64-x64.msi)
  * [VS](https://visualstudio.microsoft.com/zh-hans/downloads/)
* 新建 `E:\OSG3.7.0`，下载上述前三个压缩包并解压到此目录

![所需要的源码包](https://upload-images.jianshu.io/upload_images/5587614-c3cc4f18be94ad67.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* 在 `E:\OSG3.7.0\OpenSceneGraph-master` 新建一个 `build` 文件夹用于存放编译内容

![在 OSG 源码包目录下新建 build 文件夹](https://upload-images.jianshu.io/upload_images/5587614-5c7294f4c0670110.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* 安装 `cmake-3.13.3-win64-x64.msi`，安装完成后重启电脑，否则 Configure 时可能出现 `error in configuration process project files may be invalid` 的错误

![cmake 安装后的程序位置](https://upload-images.jianshu.io/upload_images/5587614-f6e08d2c64cda150.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* 打开 `CMake (cmake-gui)`，将 `E:\OSG3.7.0\OpenSceneGraph-master\CMakeLists.txt` 拖到 `CMake (cmake-gui)` 中（或者手动输入路径），并将构建路径改为新建的 `E:\OSG3.7.0\OpenSceneGraph-master\build`

![在 cmake 中设置如图的路径](https://upload-images.jianshu.io/upload_images/5587614-fbad091e90e8369b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* 点击 `Configure`，为了生成 64 位版本，选择 `Visual Studio 15 2017 Win64` 并点击 `Finish`。注意，若此处选择 `Visual Studio 15 2017` 则将生成 32 位版本

![此时即可点击 Finish](https://upload-images.jianshu.io/upload_images/5587614-72440b12710562ba.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* 等待大概二十秒完成 Configure，将出现一批红色选项

![Configure 完成](https://upload-images.jianshu.io/upload_images/5587614-6f6a08204fa470af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* Configure 完成后修改如下几个选项，可用 Search 功能快速找到对应选项
  * `ACTUAL_3RDPARTY_DIR`：`E:/OSG3.7.0/3rdParty_x64`
  * `BUILD_OSG_EXAMPLES`：打勾
  * `CMAKE_INSTALL_PREFIX`：`E:/OSG3.7.0/OpenSceneGraph-master/build`

![用 Search 快速找到选项](https://upload-images.jianshu.io/upload_images/5587614-b24b21540c0ccae3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* 第二次点击 `Configure`，界面选项仍为红色
* 第三次点击 `Configure`，完成后界面将没有红色选项

![第三次点击 Configure 后](https://upload-images.jianshu.io/upload_images/5587614-dcea85645174139c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* 点击 `Generate`，Generate 完成后将在 `E:\OSG3.7.0\OpenSceneGraph-master\build` 中生成工程文件，此时即可关闭 `cmake-gui`

![生成新的工程文件](https://upload-images.jianshu.io/upload_images/5587614-9cc58b697b18c9f5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* 用 VS2017 打开 `E:\OSG3.7.0\OpenSceneGraph-master\build\OpenSceneGraph.sln`，等待加载完成。CMake 构建的解决方案一般包含三个工程：`ALL_BUILD`、`INSTALL` 和 `ZERO_CHECK`
  * 只要编译 `ALL_BUILD` 就将构建整个工程，相当于 makefile 的功能
  * 编译 `INSTALL` 会将生成的 dll 和 exe 文件安装到 `CMAKE_INSTALL_DIR`
  * `ZERO_CHECK` 会监视 `CMakeLists.txt`，只要后者发生改变就会通知编译器重构工程

![](https://upload-images.jianshu.io/upload_images/5587614-7b6e02ab72911eba.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* 打开 `生成 - 批生成（快捷键 Alt+B+T）`，勾选 `ALL_BUILD` 项目的 `Debug|x64` 和 `Release|x64` 配置，点击 `生成` 开始编译。若 `E:\OSG3.7.0\OpenSceneGraph-master\build\lib` 中有文件生成则说明编译过程顺利进行，大概 2.5 小时后完成编译，提示 `========== 生成：成功 542 个，失败 0 个，最新 0 个，跳过 0 个 ==========`

![批生成选项](https://upload-images.jianshu.io/upload_images/5587614-9d8b955fd23eb8b9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![编译中](https://upload-images.jianshu.io/upload_images/5587614-e5af8add4c8372d8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![ALL_BUILD 编译完成](https://upload-images.jianshu.io/upload_images/5587614-ffe59a4fb920f677.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* `ALL_BUILD` 项目编译完成后，再勾选 `INSTALL` 项目的 `Debug|x64` 和 `Release|x64` 配置，点击 `生成` 开始编译

![批生成选项勾选 INSTALL](https://upload-images.jianshu.io/upload_images/5587614-b012630d77d393e6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* 此步会将所需要的文件复制到 `E:\OSG3.7.0\OpenSceneGraph-master\build` 的 `cmake`、`include`、`share`、`lib` 中，`share` 中是样例程序。此步一分钟即可完成，提示 `========== 生成：成功 2 个，失败 0 个，最新 542 个，跳过 0 个 ==========`，编译完成后，编译信息将保存在`E:\OSG3.7.0\OpenSceneGraph-master\build\install_manifest.txt` 中

![INSTALL 编译完成](https://upload-images.jianshu.io/upload_images/5587614-2e0fa7e1322e62cd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![install_manifest.txt](https://upload-images.jianshu.io/upload_images/5587614-1b38308f84f6d0d3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![INSTALL 操作过的文件夹](https://upload-images.jianshu.io/upload_images/5587614-3008cff3fa30063f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 转移数据

* 编译完成后的文件有大概 12G，而实际只需要用到其中一部分：
  * 新建 `C:\OSG` 文件夹
  * 将 `E:\OSG3.7.0\OpenSceneGraph-master\build` 的 `bin`、`include`、`lib` 复制到 `C:\OSG`
  * 将 `E:\OSG3.7.0\OpenSceneGraph-Data` 拷贝到 `C:\OSG`，并改名为 `data`

![C:\OSG](https://upload-images.jianshu.io/upload_images/5587614-b792fa891c84423c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* 添加如下环境变量，并重启电脑以使环境变量生效
  * `OSG_ROOT`：`C:\OSG;`
  * `OSG_BIN_PATH`：`%OSG_ROOT%\bin;`
  * `OSG_INCLUDE_PATH`：`%OSG_ROOT%\include;`
  * `OSG_LIB_PATH`：`%OSG_ROOT%\lib;`
  * `OSG_FILE_PATH`：`%OSG_ROOT%\data;`
  * `PATH`：`%OSG_BIN_PATH%;`
* 打开 CMD 执行以下命令，以检查是否配置成功
  * `osgversion` 显示版本号
  * `osglogo` 出现三维的 logo
  * `osgviewer cow.osg` 出现三维的牛

![osgviewer cow.osg](http://upload-images.jianshu.io/upload_images/5587614-690255bbbf4dc93b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## VS 环境测试

* 打开 VS 并新建一个空项目，将 Debug 调试器设置为 `x64`

![Debug 设置为 x64](https://upload-images.jianshu.io/upload_images/5587614-2d052a53b45939b9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* 右键项目打开 `属性`，展开 `配置属性`
  * 在 `VC++ 目录 - 包含目录` 或 `C/C++ - 常规 - 附加包含目录` 中添加 `$(OSG_INCLUDE_PATH);`
  * 在 `VC++ 目录 - 库目录` 或 `链接器 - 常规 - 附加库目录` 中添加 `$(OSG_LIB_PATH);`
  * `C/C++ - 预处理器 - 预处理器定义` 设置为 `WIN32;_WIN32;NDEBUG;`
  * `链接器 - 输入 - 附加依赖项` 设置为（名称以 d 结尾代表 debug 版本，以下文件在 C:\OSG\lib 中都能找到。kernel32.lib 开始是 VS2017 默认设置的库，只需要添加 kernel32.lib 之前的 lib 即可）`osgWidgetd.lib;osgVolumed.lib;osgUId.lib;osgTerraind.lib;osgSimd.lib;osgShadowd.lib;osgPresentationd.lib;osgParticled.lib;osgManipulatord.lib;osgFXd.lib;osgAnimationd.lib;OpenThreadsd.lib;osgd.lib;osgDBd.lib;osgUtild.lib;osgGAd.lib;osgViewerd.lib;osgTextd.lib;kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;%(AdditionalDependencies)`

![每个 lib 都有一个 debug 版本（后缀带 d）和 release 版本](https://upload-images.jianshu.io/upload_images/5587614-b98adc606d98e69f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* 添加如下源码运行，运行会先弹出一个空的控制台窗口然后显示之前出现过的那头牛

```cpp
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

int main(int argc, char **argv) {
  osgViewer::Viewer viewer;
  viewer.setSceneData(osgDB::readNodeFile("cow.osg"));
  return viewer.run();
}
```

* 样例源码是学习的主要方式，源码在 `E:\OSG3.7.0\OpenSceneGraph-master\examples` 中，如 `examples\osganimate\osganimate.cpp` 是两架飞机在一个平面上盘旋飞行的源码
* OSG3.7.0 支持三维文字，而使用之前版本时发现无法支持。确定在 `C:\Windows\WinSxS\amd64_microsoft-windows-font-truetype-simhei_31bf3856ad364e35_10.0.14393.0_none_219c21d98ac787bb` 中存在字体文件 `simhei.ttf`，运行以下代码，将显示三维文字。如果只显示蓝色背景不显示文字，可能是显卡驱动的问题

```cpp
#include <osg/Geode>
#include <osg/Group>
#include <osg/Material>
#include <osg/Node>
#include <osg/StateSet>
#include <osgDB/WriteFile>
#include <osgText/Text3D>
#include <osgViewer/Viewer>

osg::ref_ptr<osg::Geode> create3DText() {
  osg::ref_ptr<osg::Geode> geode = new osg::Geode();
  // 创建三维文字
  osg::ref_ptr<osgText::Text3D> text = new osgText::Text3D();
  text->setText(L"http://www.OsgChina.org－－OpengSceneGraph中国官方");
  text->setFont(
      "C:\\Windows\\WinSxS\\amd64_microsoft-windows-font-truetype-simhei_"
      "31bf3856ad364e35_10.0.14393.0_none_219c21d98ac787bb\\simhei.ttf");
  text->setCharacterSize(60.0f);
  text->setPosition(osg::Vec3(0.0f, 0.0f, 0.0f));
  // 设置文字渲染模式
  text->setRenderMode(osgText::Text3D::PER_GLYPH);
  // 设置文字深度
  text->setCharacterDepth(10.0f);
  text->setDrawMode(osgText::Text3D::TEXT | osgText::Text3D::BOUNDINGBOX);
  // 设置文字与坐标轴对齐方式
  text->setAxisAlignment(osgText::Text3D::XZ_PLANE);

  geode->addDrawable(text.get());
  // 设置材质
  osg::ref_ptr<osg::Material> front = new osg::Material;
  front->setColorMode(osg::Material::AMBIENT);
  front->setAlpha(osg::Material::FRONT_AND_BACK, 1);
  front->setAmbient(osg::Material::FRONT_AND_BACK,
                    osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
  front->setDiffuse(osg::Material::FRONT_AND_BACK,
                    osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f));
  geode->getOrCreateStateSet()->setAttributeAndModes(front.get());
  return geode.get();
}

int main() {
  osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();
  osg::ref_ptr<osg::Group> root = new osg::Group();
  root->addChild(create3DText());
  viewer->setSceneData(root.get());
  viewer->realize();
  viewer->run();
}
```

## osgQt 编译

* 注意：这里使用的是 Qt5.10.1 版本，进行下列配置出现问题可能是因为版本不同，如使用 Qt5.11.0 提示 `qt5_use_modules` 命令找不到，原因是 Qt5.11.0 移除了这个宏。另外的问题可能来源于环境变量，务必检查环境变量的配置，特别是 Qt 本身的两个环境变量
  * `QT5_DIR`：`C:\Qt\Qt5.10.1\5.10.1\msvc2017_64;`
  * `PATH`：`%QT5_DIR%\bin;`
* 完成 OSG 的编译与环境变量的配置后，为了能在 Qt 界面中嵌入 osg 模型，还需要编译 `osgQt`，可在 [此处](https://github.com/openscenegraph/osgQt) 下载
* 将下载的 `osgQt-master.zip` 移动到 `E:\OSG3.7.0` 并解压，并同样新建一个 `E:\OSG3.7.0\osgQt-master\build` 文件夹用于存储编译后的文件
* 打开 `CMakeLists.txt`，找到如下两行（130-131 行）

```cmake
FIND_PACKAGE(OpenSceneGraph 3.0.0 REQUIRED osgDB osgGA osgUtil osgText osgViewer osgWidget)
SET(OPENSCENEGRAPH_SOVERSION 145)
```

* 修改对应的版本号为 3.7.0，160

```cmake
FIND_PACKAGE(OpenSceneGraph 3.7.0 REQUIRED osgDB osgGA osgUtil osgText osgViewer osgWidget)
SET(OPENSCENEGRAPH_SOVERSION 160)
```

* 打开 `CMake (cmake-gui)`，将 `E:\OSG3.7.0\osgQt-master\CMakeLists.txt` 拖进去，并把构建目录改为之前新建的 `build`
* 点击 `Configure`，选择 `Visual Studio 15 2017 Win64`，点击 `Finish`，等待几秒后 `Configuring done`
* 修改值
  * `BUILD_OSG_EXAMPLES`：打勾
  * `CMAKE_INSTALL_PREFIX`：`E:\OSG\osgQt-master\build`
  * OSG 和 Qt 的设置项会根据环境变量自动检测路径
    * `Qt5Core_DIR`：`C:/Qt/Qt5.10.1/5.10.1/msvc2017_64/lib/cmake/Qt5Core`
    * `Qt5Gui_DIR`：`C:/Qt/Qt5.10.1/5.10.1/msvc2017_64/lib/cmake/Qt5Gui`
    * `Qt5Network_DIR`：`C:/Qt/Qt5.10.1/5.10.1/msvc2017_64/lib/cmake/Qt5Network`
    * `Qt5OpenGL_DIR`：`C:/Qt/Qt5.10.1/5.10.1/msvc2017_64/lib/cmake/Qt5OpenGL`
    * `Qt5WebKitWidgets_DIR`：`C:/Qt/Qt5.10.1/5.10.1/msvc2017_64/lib/cmake/Qt5WebKitWidgets`
    * `Qt5WebKit_DIR`：`C:/Qt/Qt5.10.1/5.10.1/msvc2017_64/lib/cmake/Qt5WebKit`
    * `Qt5Widgets_DIR`：`C:/Qt/Qt5.10.1/5.10.1/msvc2017_64/lib/cmake/Qt5Widgets`
    * `Qt5_DIR`：`C:/Qt/Qt5.10.1/5.10.1/msvc2017_64/lib/cmake/Qt5`

![配置 Qt 的 msvc2017_64 的路径](https://upload-images.jianshu.io/upload_images/5587614-d226508ca4ce489c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* 再次点击 `Configure`，窗口中的红色全变为白色
* 点击 `Generate`，生成工程。`E:\OSG3.7.0\osgQt-master\build` 中将出现生成的 `osgQt` 工程，此时可关闭 `CMake (cmake-gui)`
* 用 `VS2017` 打开 `osgQt.sln`
* 打开 `生成 - 批生成（快捷键 Alt+B+T）`，勾选 `ALL_BUILD` 项目的 `Debug|x64` 和 `Release|x64` 配置，点击 `生成` 开始编译。若 `E:\OSG3.7.0\osgQt-master\build\lib` 中有文件生成则说明编译过程顺利进行，此步花费一分钟即可完成
* `ALL_BUILD` 项目编译完成后，再勾选 `INSTALL` 项目的 `Debug|x64` 和 `Release|x64` 配置，点击 `生成` 开始编译
* `INSTALL` 编译完成后，在 `E:\OSG3.7.0\osgQt-master\share\OpenSceneGraph\bin` 中将有四个可运行的样例程序
* `osgqfont-qt5.exe` 和 `osgqfont-qt5d.exe` 的运行结果是弹出一个窗口，然后显示出字体界面
* `osgviewerQt5.exe` 和 `osgviewerQt5d.exe` 的运行结果是弹出一个窗口，然后显示两个窗口界面，左边一个大窗口中有四个区域，分别显示四个模型，右边一个小窗口，显示一辆卡车模型

## osgQt 环境配置及测试

* 转移数据：
  * 将 `E:\OSG3.7.0\osgQt-master\build\bin` 的所有内容拷贝到 `C:\OSG\bin`
  * 将 `E:\OSG3.7.0\osgQt-master\build\osgQt` 拷贝到 `C:\OSG\include`
  * 将 `E:\OSG3.7.0\osgQt-master\build\lib\pkgconfig\openscenegraph-osgQt5.pc` 拷贝到 `C:\OSG\lib\pkgconfig`
  * 将 `E:\OSG3.7.0\osgQt-master\build\lib` 的 `osgQt5.exp、osgQt5.lib、osgQt5d.exp、osgQt5d.lib` 拷贝到 `C:\OSG\lib`

![osgQt 的库文件](https://upload-images.jianshu.io/upload_images/5587614-2e4a1fb079948496.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* 打开 VS，新建 `Qt GUI Application`

![新建 Qt 界面程序](https://upload-images.jianshu.io/upload_images/5587614-41c214b6b4d76b59.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* 必须在向导中勾选 `OpenGL`，否则后续会出现 `无法打开包括文件: "QGLWidget": No such file or directory` 的错误，原因是后续使用的 `osgQt/GraphicsWindowQt` 中有 `#include <QGLWidget>`

![勾选 OpenGL](https://upload-images.jianshu.io/upload_images/5587614-12427237d70421ca.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![osgQt/GraphicsWindowQt](https://upload-images.jianshu.io/upload_images/5587614-df530308db072a65.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* 项目创建完成，右键项目打开 `属性 - 配置属性`
  * 在 `VC++ 目录 - 包含目录` 或 `C/C++ - 常规 - 附加包含目录` 中添加 `$(OSG_INCLUDE_PATH);`
  * 在 `VC++ 目录 - 库目录` 或 `链接器 - 常规 - 附加库目录` 中添加 `$(OSG_LIB_PATH);`
  * `链接器 - 输入 - 附加依赖项` 设置为（这里比之前多了 osgQt5d.lib，qtmaind.lib 开始是 Qt5 项目默认设置的库，只需要添加 qtmaind.lib 之前的 lib 即可）
`osgQt5d.lib;osgWidgetd.lib;osgVolumed.lib;osgUId.lib;osgTerraind.lib;osgSimd.lib;osgShadowd.lib;osgPresentationd.lib;osgParticled.lib;osgManipulatord.lib;osgFXd.lib;osgAnimationd.lib;OpenThreadsd.lib;osgd.lib;osgDBd.lib;osgUtild.lib;osgGAd.lib;osgViewerd.lib;osgTextd.lib;qtmaind.lib;Qt5Cored.lib;Qt5Guid.lib;Qt5OpenGLd.lib;opengl32.lib;glu32.lib;Qt5Widgetsd.lib;%(AdditionalDependencies)`

* 以样例代码作为测试。样例代码在最初解压的 `E:\OSG3.7.0\osgQt-master\examples` 中，其中 `osgqfont` 和 `osgviewerQt` 分别对应之前运行的两个程序。以第二个程序为例，打开 `E:\OSG3.7.0\osgQt-master\examples\osgviewerQt\osgviewerQt.cpp`

![osgviewerQt.cpp](https://upload-images.jianshu.io/upload_images/5587614-d98fc82508f2d398.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

* 将其内容复制到 Qt 项目的 `main.cpp` 中

```cpp
#include <QApplication>
#include <QGridLayout>
#include <QTimer>
#include <iostream>
#include <osgDB/ReadFile>
#include <osgGA/MultiTouchTrackballManipulator>
#include <osgQt/GraphicsWindowQt>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>

class ViewerWidget : public QWidget, public osgViewer::CompositeViewer {
 public:
  ViewerWidget(QWidget* parent = 0, Qt::WindowFlags f = 0,
               osgViewer::ViewerBase::ThreadingModel threadingModel =
                   osgViewer::CompositeViewer::SingleThreaded)
      : QWidget(parent, f) {
    setThreadingModel(threadingModel);

    // disable the default setting of viewer.done() by pressing Escape.
    setKeyEventSetsDone(0);

    QWidget* widget1 = addViewWidget(createGraphicsWindow(0, 0, 100, 100),
                                     osgDB::readRefNodeFile("cow.osgt"));
    QWidget* widget2 = addViewWidget(createGraphicsWindow(0, 0, 100, 100),
                                     osgDB::readRefNodeFile("glider.osgt"));
    QWidget* widget3 = addViewWidget(createGraphicsWindow(0, 0, 100, 100),
                                     osgDB::readRefNodeFile("axes.osgt"));
    QWidget* widget4 = addViewWidget(createGraphicsWindow(0, 0, 100, 100),
                                     osgDB::readRefNodeFile("fountain.osgt"));
    QWidget* popupWidget = addViewWidget(
        createGraphicsWindow(900, 100, 320, 240, "Popup window", true),
        osgDB::readRefNodeFile("dumptruck.osgt"));
    popupWidget->show();

    QGridLayout* grid = new QGridLayout;
    grid->addWidget(widget1, 0, 0);
    grid->addWidget(widget2, 0, 1);
    grid->addWidget(widget3, 1, 0);
    grid->addWidget(widget4, 1, 1);
    setLayout(grid);

    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
    _timer.start(10);
  }

  QWidget* addViewWidget(osgQt::GraphicsWindowQt* gw,
                         osg::ref_ptr<osg::Node> scene) {
    osgViewer::View* view = new osgViewer::View;
    addView(view);

    osg::Camera* camera = view->getCamera();
    camera->setGraphicsContext(gw);

    const osg::GraphicsContext::Traits* traits = gw->getTraits();

    camera->setClearColor(osg::Vec4(0.2, 0.2, 0.6, 1.0));
    camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));

    // set the draw and read buffers up for a double buffered window with
    // rendering going to back buffer
    camera->setDrawBuffer(GL_BACK);
    camera->setReadBuffer(GL_BACK);

    camera->setProjectionMatrixAsPerspective(
        30.0f,
        static_cast<double>(traits->width) /
            static_cast<double>(traits->height),
        1.0f, 10000.0f);

    view->setSceneData(scene);
    view->addEventHandler(new osgViewer::StatsHandler);
    view->setCameraManipulator(new osgGA::MultiTouchTrackballManipulator);
    gw->setTouchEventsEnabled(true);
    return gw->getGLWidget();
  }

  osgQt::GraphicsWindowQt* createGraphicsWindow(int x, int y, int w, int h,
                                                const std::string& name = "",
                                                bool windowDecoration = false) {
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits =
        new osg::GraphicsContext::Traits;
    traits->windowName = name;
    traits->windowDecoration = windowDecoration;
    traits->x = x;
    traits->y = y;
    traits->width = w;
    traits->height = h;
    traits->doubleBuffer = true;
    traits->alpha = ds->getMinimumNumAlphaBits();
    traits->stencil = ds->getMinimumNumStencilBits();
    traits->sampleBuffers = ds->getMultiSamples();
    traits->samples = ds->getNumMultiSamples();

    return new osgQt::GraphicsWindowQt(traits.get());
  }

  virtual void paintEvent(QPaintEvent* /*event*/) { frame(); }

 protected:
  QTimer _timer;
};

int main(int argc, char** argv) {
  osg::ArgumentParser arguments(&argc, argv);

#if QT_VERSION >= 0x050000
  // Qt5 is currently crashing and reporting "Cannot make QOpenGLContext current
  // in a different thread" when the viewer is run multi-threaded, this is
  // regression from Qt4
  osgViewer::ViewerBase::ThreadingModel threadingModel =
      osgViewer::ViewerBase::SingleThreaded;
#else
  osgViewer::ViewerBase::ThreadingModel threadingModel =
      osgViewer::ViewerBase::CullDrawThreadPerContext;
#endif

  while (arguments.read("--SingleThreaded"))
    threadingModel = osgViewer::ViewerBase::SingleThreaded;
  while (arguments.read("--CullDrawThreadPerContext"))
    threadingModel = osgViewer::ViewerBase::CullDrawThreadPerContext;
  while (arguments.read("--DrawThreadPerContext"))
    threadingModel = osgViewer::ViewerBase::DrawThreadPerContext;
  while (arguments.read("--CullThreadPerCameraDrawThreadPerContext"))
    threadingModel =
        osgViewer::ViewerBase::CullThreadPerCameraDrawThreadPerContext;

#if QT_VERSION >= 0x040800
  // Required for multithreaded QGLWidget on Linux/X11, see
  // http://blog.qt.io/blog/2011/06/03/threaded-opengl-in-4-8/
  if (threadingModel != osgViewer::ViewerBase::SingleThreaded)
    QApplication::setAttribute(Qt::AA_X11InitThreads);
#endif

  QApplication app(argc, argv);
  ViewerWidget* viewWidget = new ViewerWidget(0, Qt::Widget, threadingModel);
  viewWidget->setGeometry(100, 100, 800, 600);
  viewWidget->show();
  return app.exec();
}
```

* 运行不再弹出黑窗口，而是直接显示之前 `osgviewerQt5d.exe` 的两个窗口

![运行结果](https://upload-images.jianshu.io/upload_images/5587614-38ade5839857d7ad.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
