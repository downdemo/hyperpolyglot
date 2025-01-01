## OSG 源码命名习惯

* 命名空间：OSG 的域名空间使用小写字母开头，然后可以使用大写字母以避免混淆。例如，osg，osgSim，osgFX 等
* 类：OSG 的类名以大写字母开头，如果类的名称是多个单词的组合，此后每个单词的首字母大写。例如，MatrixTransform，NodeVisitor，Optimizer
* 类方法：OSG 类的方法名使用小写字母开头，如果方法的名称是多个单词的组合，此后每个单词的首字母大写。例如，addDrawable()，getNumChildren()，setAttributeAndModes()
* 类成员变量：类的成员变量命名与方法命名的方式相同
* 模板：OSG 模板的命名用小写字母，多个单词之间使用下划线分隔。例如，ref_ptr<>，graph_array<>，observer_ptr<>
* 静态量：静态变量和函数的名称使用 s_ 开头，此后的命名与类成员变量及函数的命名方法相同。例如，s_applicationUsage，s_ArrayNames()
* 全局量：全局类的实例命名用 g_ 开头。例如，g_NotifyLevel，g_readerWriter_BMP_Proxy

## OSG 组件

* OSG 运行时文件由一系列动态链接库（或共享对象）和可执行文件组成。这些链接库可分为以下五大类
  * OSG 核心库：提供基本的场景图形和渲染功能，以及 3D 图形程序所需的某些特定功能实现
  * NodeKits：扩展了核心 OSG 场景图形节点类的功能，以提供高级节点类型和渲染特效
  * OSG 插件：其中包括了 2D 图像和 3D 模型文件的读写功能库
  * 互操作库：使得 OSG 易于与其它开发环境集成，例如脚本语言 Python 和 Lua
  * 不断扩展中的程序和示例集：提供了实用的功能函数和正确使用 OSG 的例子

```
                  ----------------
                  | example code |
                  ----------------
             __________________________
            |  OSG Introspection API   |
            |         OSG API          |
            |__________________________|
OSG Plug-in > OSG Core  | OSG NodeKits |
            |___________|______________|
```

* OSG 核心库提供了应用程序和 NodeKits 所需的功能模块。而 OSG 核心库和 NodeKits 一同组成了 OSG 的 API。OSG 核心库中的 osgDB 则通过对 OSG 插件的管理，为用户提供了 2D 和 3D 文件 I/O 的接口

## 1. OSG 核心库

* OSG 核心库提供了用于场景图形操作的核心场景图形功能、类和方法；开发 3D 图形程序所需的某些特定功能函数和编程接口；以及 2D 和 3D 文件 I/O 的 OSG 插件入口。OSG 核心库包含了以下四个链接库
  * osg 库：包含了用于构建场景图形的场景图形节点类，用作向量和矩阵运算的类，几何体类，以及用于描述和管理渲染状态的类。osg 库中还包括 3D 图形程序所需的典型功能类，例如命令行参数解析，动画路径管理，以及错误和警告信息类
  * osgUtil 库：包括的类和函数，可以用于场景图形及其内容的操作，场景图形数据统计和优化，以及渲染器的创建。它还包括了几何操作的类，例如 Delaunay 三角面片化（Delaunay triangulation），三角面片条带化（triangle stripification），纹理坐标生成等
  * osgDB 库：包括建立和渲染 3D 数据库的类与函数。其中包括用于 2D 和 3D 文件读写的 OSG 插件类的注册表，以及用于访问这些插件的特定功能类。osgDB 数据库分页机（database pager）可以支持大型数据段的动态读入和卸载
  * osgViewer 库：这个库是 OSG 的 2.0 版本新增的，它包含了场景中视口及可视化内容的管理类。osgViewer 已将 OSG 集成到多种多样的视窗系统中

### 1.1 osg 库

> 命名空间：osg
头文件：<OSG_DIR>/include/osg
Windows 库文件：osg.dll，osg.lib
Linux 和 Mac OS X 库文件：libosg.lib

* osg 库是 OpenSceneGraph 的核心部分。它定义了组成场景图形的核心节点，以及帮助用户进行场景图形管理和程序开发的一些附加类

#### 1.1.1 场景图形类

* 场景图形类用于辅助场景图形的构建。OSG 中所有场景图形类都继承自 osg::Node。从概念上讲，根节点，组节点和叶节点是不同的节点类型，在 OSG 中，它们都源自于 osg::Node，但是特定的类会提供不同的场景图形功能。此外，OSG 中的根节点并不是特定的节点类型，它仅仅是一个没有父类的 osg::Node 类
  * Node：Node 类是场景图形中所有节点的基类。它包含了用于场景图形遍历、拣选、程序回调以及状态管理的方法
  * Group：Group 类是所有可分支节点的基类。它是场景图形空间组织结构的关键类
  * Geode：Geode 类（即 Geometry Node）相当于 OSG 中的叶节点。它没有子节点，但是包含了 osg::Drawable 对象，而 osg::Drawable 对象中存放了将要被渲染的几何体
  * LOD：LOD 类根据观察点与图像子节点的距离选择显示子节点。通常使用它来创建场景中物体的多个显示层级
  * MatrixTransform：MatrixTransform 类包含了用于实施子节点几何体空间转换的矩阵，以实现场景对象的旋转、平移、缩放、倾斜、映射等操作
  * Switch：Switch 类用布尔掩板来允许或禁止子节点的运作

#### 1.1.2 几何体类

* Geode 类是 OSG 的叶节点，它包含了渲染用的几何数据。使用以下列出的类可以实现 Geode 中几何数据的存储。Geode 类是场景图形的叶节点，其中保存了 Drawable 类；Geometry 类（仅 Drawable 类型）保存了顶点数组数据和及其顶点数组的渲染指令；数据的组成为向量数组
  * Drawable：Drawable 类是用于存储几何数据信息的基类，Geode 维护了一个 Drawable 的列表。Drawable 是纯虚类，无法直接实例化。用户必须实例化其派生类，如 Geometry，或者 ShapeDrawable（允许用户程序绘制预定义的几何形状，如球体、圆锥体和长方体）
  * Geometry：Geometry 类与 PrimitiveSet 类相关联，实现了对 OpenGL 顶点数组功能的高级封装。Geometry 保存顶点数组的数据，纹理坐标，颜色，以及法线数组
  * PrimitiveSet：PrimitiveSet 类提供了 OpenGL 顶点数组绘图命令的高层次支持。用户可以从相关的 Geometry 类中取得保存的数据，再使用这个类来指定要绘制的几何体数据的类型
  * Vector 类（Vec2，Vec3 等）：OSG 提供了预定义好的二维，三维和四维元素向量，支持 float 或者 double 类型。使用这些向量来指定顶点、颜色、法线和纹理坐标的信息
  * Array 类（Vec2Array，Vec3Array 等）：OSG 定义了一些常用的数组类型，如用于贴图纹理坐标的 Vec2Array。指定顶点数组数据时，程序首先将几何数据保存到这些数组中，然后传递至 Geometry 类对象

#### 1.1.3 状态管理类

* OSG 提供了一种机制，用以保存场景图形所需的 OpenGL 渲染状态。在拣选遍历中，同一状态的几何体将被组合集中到一起以使状态的改变呈最小化。在绘制遍历中，状态管理代码将记录当前状态的历史轨迹，以清除冗余的渲染状态变更。和其它场景图形系统不同，OSG 允许状态与任何场景图形节点相关联，在一次遍历中，状态将呈现出某种继承关系
  * 状态集合（StateSet）：OSG 在 StateSet 类中保存一组定义状态数据（模式和属性）。场景图形中的任何 osg::Node 都可以与一个 StateSet 相关联
  * 模式（Modes）：与 OpenGL 的函数 glEnable() 和 glDisable() 相似，模式用于打开或关闭 OpenGL 固定功能（fixed-function）的渲染管道，例如灯光，混合和雾效。方法 osg::StateSet::setMode() 在 StateSet 中保存一个模式信息
  * 属性（Attributes）：应用程序使用属性来指定状态参数，例如混和函数，材质属性，雾颜色等。方法 osg::StateSet::setAttribute() 在 StateSet 中保存属性信息
  * 纹理模式和属性：纹理模式和属性可应用在 OpenGL 多重纹理的某个指定纹理单元上。应用程序必须在设定纹理模式和属性时提供纹理单元的信息，注意，和 OpenGL 不同，OSG 不存在缺省的纹理单元。StateSet 类的方法 setTextureMode() 和 setTextureAttribute() 用于设定状态参量以及纹理单元信息
  * 继承标志：OSG 提供了一些标志量，用于控制场景图形遍历中的状态值。缺省情况下，子节点中的状态集合将重载父节点的状态集合，但是也可以强制父节点的状态重载子节点的状态，或者指定子节点的状态受到保护而不会被其父节点重载

#### 1.1.4 其他实用类

* 除上述类外，osg 库还包括了一些实用的类和工具。其中一些涉及到 OSG 的内存引用计数策略（reference-counted memory scheme），这种策略可以通过清理不再引用的内存以避免内存泄露
  * Referenced：Referenced 类是所有场景图形节点和 OSG 的许多其它对象的基类。它实现了一个用于跟踪内存使用情况的引用计数（referencecount）。如果某个继承自 Referenced 的对象，其引用计数的数值到达 0，那么系统将自动调用其析构函数并清理为此对象分配的内存
  * ref_ptr<>：模板类 ref_ptr<> 为其模板内容定义了一个智能指针，模板内容必须继承自 Referenced 类（或提供一个与之相同的、能实现引用计数的接口）。当对象的地址分配给 ref_ptr<> 时，对象的引用计数将自动增加。同样，清除或者删去 ref_ptr 时，对象的引用计数将自动减少
  * Object：纯虚类 Object 是 OSG 中一切需要 I/O 支持，拷贝和引用计数的对象的基类。所有的节点类，以及某些 OSG 对象均派生自 Object 类
  * Notify：osg 库提供了一系列控制调试，警告和错误输出的函数。用户可以通过指定一个来自 NotifySeverity 枚举量的数值，设定输出的信息量。OSG 中的大部分代码模块执行时都会显示相关的信息

### 1.2 osgUtil

> 命名空间：osgUtil
头文件：<OSG_DIR>/include/osgUtil
Windows 库文件：osgUtil.dll，osgUtil.lib
Linux 和 Mac OS X 库文件：libosgUtil.lib

* osgUtil 库集合了许多用于场景图形处理和几何体修改的工具。osgUtil 库最知名之处可能就是其中一系列支持更新、拣选和绘制遍历的类。在典型的 OSG 程序中，这些遍历由更高层次的支持类，例如 osgViewer::Viewer 来进行处理，用户不需要直接和它们进行交互

#### 1.2.1 交运算

* 一般来说，3D 程序需要为用户提供一些实现交互和选择的功能，比如图形对象的拾取。通过提供大量用于场景图形交运算的类，osgUtil 库可以高效地支持拾取操作。当用户程序从需要进行图形对象拾取的用户那里接收到事件输入时，可以使用以下的类，获得场景图形中被拾取部分的信息
  * Intersector：Intersector 是一个纯虚类，它定义了相交测试的接口。osgUtil 库从 Intersection 继承了多个类，适用于各种类型的几何体（线段，平面等）。执行相交测试时，应用程序将继承自 Intersector 的某个类实例化，传递给 IntersectionVisitor 的实例，并随后请求该实例返回数据以获取交运算的结果
  * IntersectionVisitor：IntersectionVisitor 类搜索场景图形中与指定几何体相交的节点。而最后相交测试的工作将在 Intersector 的继承类中完成
  * LineSegmentIntersector：LineSegmentIntersector 类继承自 Intersector 类，用于检测指定线段和场景图形之间的相交情况，并向程序提供查询相交测试结果的函数
  * PolytopeIntersector：与 LineSegmentIntersector 类似，该类用于检测由一系列平面构成的多面体的相交情况。当用户点击鼠标，希望拾取到鼠标位置附近的封闭多面体区域时，PolytopeIntersector 类尤其有用
  * PlaneIntersector：与 LineSegmentIntersector 类似，这个类用于检测由一系列平面构成的平面的相交情况

#### 1.2.2 优化

* 场景图形的数据结构在理论上有助于实现优化和数据统计工作。osgUtil 库包含的类可以遍历并修改场景图形，以实现渲染的优化和收集场景统计信息的目的
  * Optimizer：正如其名字所示，Optimizer 类用于优化场景图形。其属性使用一组枚举标志进行控制，每一个标志都表示一种特定的优化方式。例如，FLATTEN_STATIC_TRANSFORMS 使用非动态 Transform 节点来变换几何体，通过清除对 OpenGL 的 model-view 矩阵堆栈的修改，实现场景的渲染优化
  * Statistics 和 StatsVisitor：为能够高效地设计 3D 应用程序，开发者应当对将要渲染的对象有尽量多的了解。StatsVisitor 类返回一个场景图形中节点的总数和类型，而 Statistics 类返回渲染几何体的总数和类型

#### 1.2.3 几何体操作

* 许多 3D 程序都需要对读入的几何体进行修改，以获得所需的性能和渲染效果。osgUtil 库包含的类支持一些通用的几何形体运算
  * Simplifier：使用 Simplifier 类减少 Geometry 对象中几何体的数目，这有助于低细节层次的自动生成
  * Tessellator：OpenGL 不直接支持凹多边形和复杂多边形。Tessellator 类可根据一组顶点的列表，生成由前述顶点列表所描述的多边形，即一个 osg::PrimitiveSet
  * DelaunayTriangulator：正如其名称所示，这个类实现了 Delaunay 三角网格化运算，根据一组顶点的集合生成一系列的三角形
  * TriStripVisitor：一般来说，由于共享顶点的缘故，连续的条带图元（stripprimitives）的渲染效率要高于独立的图元（individual primitives）TriStripVisitor 类可遍历场景图形并将多边形图元转换成三角形和四边形条带
  * SmoothingVisitor：SmoothingVisitor 类可生成顶点法线，也就是所有共享此顶点的面的法线平均值
  * 纹理贴图生成：osgUtil 库包含了帮助建立反射贴图，中途向量（half-wayvector）贴图，以及高光贴图的代码。此外，使用 TangentSpaceGenerator 类还可以逐个的建立各顶点的向量数组，帮助实现凹凸贴图

### 1.3 osgDB

> 命名空间：osgDB
头文件：<OSG_DIR>/include/osgDB
Windows 库文件：osgDB.dll，osgDB.lib
Linux 和 Mac OS X 库文件：libosgDB.lib
* osgDB 库允许用户程序加载、使用和写入 3D 数据库。它采用插件管理的架构，可以支持大量常见的 2D 图形和 3D 模型文件格式。osgDB 负责维护插件的信息注册表，并负责检查将要被载入的 OSG 插件接口的合法性
* OSG 可以支持自己的文件格式。.osg 文件是对场景图形的一种无格式 ASCII 码文本描述，而 .osga 文件是一组 .osg 文件的有序集合。osgDB 库包含了以上文件格式的支持代码（另外，OSG 还支持一种二进制的 ive 格式）
* 由于大型的 3D 地型数据库通常是多段数据块的组合体，因此，应用程序从文件中读取各部分数据库信息时，需要在不干扰当前渲染的前提下以后台线程的方式进行。osgDB::DatabasePager 提供了这样的功能

### 1.4 osgViewer

> 命名空间：osgViewer
头文件：<OSG_DIR>/include/osgViewer
Windows 库文件：osgViewer.dll，osgViewer.lib
Linux 和 Mac OS X 库文件：libosgViewer.lib
* osgViewer 库定义了一些视口类，因而可以将 OSG 集成到许多视窗设计工具中，包括 AGL/CGL，FLTK，Fox，MFC，Qt，SDL，Win32，WxWindows，以及 X11。这些视口类支持单窗口、单视口的程序，也支持使用多个视口和渲染器面的多线程程序。每个视口类都可以提供对摄像机运动，事件处理，以及 osgDB::DatabasePager 的支持。osgViewer 库包含了以下三个可能用到的视口类
  * SimpleViewer：SimpleViewer 类负责管理单一场景图形中的单一视口。使用 SimpleViewer 时，应用程序必须创建一个窗口并设置当前的图形上下文（graphics context）
  * Viewer：Viewer 类用于管理多个同步摄像机，他们将从多个方向渲染单一的视口。根据底层图形系统的能力，Viewer 可以创建一个或多个自己的窗口以及图形上下文，因此使用单一视口的程序也可以在单显示或者多显示的系统上运行
  * CompositeViewer：CompositeViewer 类支持同一场景的多个视口，也支持不同场景的多个摄像机。如果指定各个视口的渲染顺序，用户就可以将某一次渲染的结果传递给别的视口。CompositeViewer 可以用来创建抬头数字显示（HUD），预渲染纹理（prerender textures），也可以用于在单一视口中显示多个视图

## 2. NodeKits

* NodeKits 扩展了 Nodes，Drawables 和 StateAttributes 的概念，也可以看作是 OSG 内核中 osg 库的一种扩展。NodeKits 的意义远大于对 OSG 类的继承，事实上它还能够提供对 .osg 的封装（一种支持对 .osg 文件进行读写的 OSG 插件）。总之，NodeKit 由两部分组成：NodeKit 本身，以及针对 .osg 的封装插件库。OSG 2.0 版本包含有六种 NodeKits
  * osgFX 库：此类 NodeKit 提供了额外的场景图形节点，以便于特效的渲染，例如异向光照（anisotropic lighting），凹凸贴图，卡通着色等
  * osgParticle 库：此类 NodeKit 提供了基于粒子的渲染特效，如爆炸、火焰、烟雾等
  * osgSim 库：此类 NodeKit 提供了仿真系统中以及渲染 OpenFlight 数据库所需的特殊渲染功能，例如地形高程图，光点节点，DOF 变换节点等
  * osgText 库：此类 NodeKit 提供了向场景中添加文字的得力工具，可以完全支持 TrueType 字体
  * osgTerrain 库：此类 NodeKit 提供了渲染高度场数据的能力
  * osgShadow 库：此类 NodeKit 提供了支持阴影渲染的框架结构

## 3. OSG 插件

* OSG 的核心库提供了针对多种 2D 图形和 3D 模型文件格式的 I/O 支持。osgDB::Registry 可以自动管理插件链接库。只要提供的插件确实可用，Registry 就可以找到并使用它，应用程序只需调用相应的函数来读取和写入数据文件即可
* osg 库允许用户程序采用节点到节点（node-by-node）的方式直接建立场景图形。相反的，OSG 插件允许用户程序仅仅通过编写几行代码就能够从磁盘中调用整个场景图形，或者调用部分的场景图形，然后应用程序可以将其列入整个场景图形系统中去
* OSG 的 2.0 版本支持大量常用的 2D 图形文件格式，包括 bmp、dds、gif、jpeg、pic、png、rgb、tga、tiff。OSG 还支持用于读取电影文件的 QuickTime 插件，并有专门的插件用于读取 FreeType 类型的字体。OSG 广泛支持各种 3D 模型文件格式，其中包括 3D Studio Max（.3ds）、Alias Wavefront（.obj）、Carbon Graphics’ Geo（.geo）、Collada（.dae）、ESRI Shapefile（.shp）、OpenFlight（.flt）、Quake（.md2）和 Terrex TerraPage（.txp）等常见格式
* 除上述标准格式以外 OSG 还定义了自身的文件格式。其中 osg 格式是场景图形的另一种 ASCII 文本描述格式，用户可以使用文本编辑器对其进行编辑和修改；而 ive 格式则是一种二进制格式，经过优化之后它更适合于迅速读取
* 除 2D 图形和 3D 模型文件以外，OSG 插件还支持对压缩文件和文件集的 I/O 操作，OSG 目前支持的压缩文件格式有常见的 tgz 和 zip，以及 OSG 特有的 osga 格式
* 此外，OSG 还包含了一组名为 PseudoLoader 的插件，以提供除简单文件读取之外更多的功能
  * 缩放、旋转和平移：此类 PseudoLoader 读取文件并在已读入场景图形根节点上添加一个 Transform 节点，并指定放缩、旋转和平移属性的值以配置 Transform
  * 图标：图标类 PseudoLoader 允许在已读入 3D 场景之上显示 HUD（抬头显示）样式的图片文件

## 4. 互操作性

* 用户可以在任何支持 C++ 库链接的编程环境中使用 OSG。为了确保 OSG 可以在更多环境中运行，OSG 提供了一个语言无关的、可供运行时访问的接口。osgIntrospection 库允许用户软件使用反射式和自省式的编程范式与 OSG 进行交互。应用程序或其它软件可以使用 osgIntrospection 库和方法迭代 OSG 的类型，枚举量和方法，并且无需了解 OSG 编译和链接时的具体过程，即可调用这些方法
* Smalltalk 和 Objective-C 等语言包括了内建的反射式和自省式支持，但使用 C++ 的软件开发人员通常无法运用这些特性，因为 C++ 并未保留必要的元数据（metadata）。为了弥补 C++ 的这一不足，OSG 提供了一系列自动生成的、从 OSG 源代码创建的封装库，用户程序不需要与这些 OSG 的封装库直接交互，它们将完全由 osgIntrospection 进行管理。作为 osgIntrospection 及其封装的结果，许多语言如 Java，Tcl，Lua 和 Python，都可以与 OSG 进行交互

## 5. 程序和示例

* OSG 发行版包含了五个常用的 OSG 工具程序，它们对于调试和其它基于 OSG 的软件开发均十分有益
  * osgarchive：这个程序用于向 .osga 文件包中添加新的文件。也可以用这个程序实现包的分解和列表
  * osgconv：这个程序用于转换文件格式。尤其有用的是，它可以将任意文件格式转换为经过优化的 ive 格式
  * osgdem：这个程序用于将高程图等高度数据及图像数据转换为分页的地形数据库
  * osgversion：这个程序将当前 OSG 版本以及一些记录了 OSG 源代码改动情况和贡献者信息送入 std::cout
  * osgviewer：这是一个灵活而强大的 OSG 场景及模型浏览器
