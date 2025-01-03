* 当写好一个项目时，如果拷贝项目到其他位置，比如本机的桌面，或是其他电脑上，或多或少可能出现各种编译问题。如果是 QtCreator 的 `.pro` 项目可能可以直接编译，因为其中记录的就是简单位置信息
* 如果是用 VS 做的 Qt 项目，没有 `.pro` 只有 `.sln`，移动位置后 `.sln` 可以打开解决方案，但因为位置的改变会出现许多报错，解决这些错误需要一一改正项目属性
* 首先测试 VS 能否正常运行。之前 VS 和 Qt 以及各种库都放在 D 盘，后来把 D 盘清空并到了 C 盘，在 C 盘中重装了 VS，只选择了默认的核心选项，结果就是一堆解决不完的连环错误，把所有依赖都装上即可直接解决这些问题
* 再检查环境变量配置，以前 D 盘中的路径要改到新的对应路径，注销或重启使其生效
* 原有项目最好放到以前相同的相对层次目录，因为项目属性中既有绝对路径又有相对路径，绝对路径不难改，相对路径改起来就麻烦了，后面再说这点
* 打开 `.sln` 导入项目后，打开 `Qt VS Tools - Qt Project Settings`，最后的 `Version` 选择已配置的设置（在 `Qt VS Tools - Qt Options` 中选择设置路径 `C:\Qt\Qt5.10.1\5.10.1\msvc2017_64` 即可设置），若没显示 `OK` 按钮可以回车保存
* 打开项目属性，依次修改 `调试 - 环境`、`Qt Meta-Object Complier`、`Qt Resource Complier`、`Qt User Interface Complier`、`链接器 - 常规 - 附加库目录`、`链接器 - 输入 - 附加依赖项` 中的原来的 D 盘路径
* 以上选项以及 `C/C++ - 常规 - 附加包含目录` 中还包含大量相对路径，修改较为麻烦。保持相对位置不变，比如以前项目放在 `D:\Project`，Qt 路径为 `D:\Qt`，现在 Qt 路径为 `C:\Qt`，直接把项目再放到 `C:\Project`，相对位置就不用修改
* 以上都改好项目即可运行
