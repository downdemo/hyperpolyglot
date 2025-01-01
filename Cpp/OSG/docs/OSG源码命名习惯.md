* 命名空间：OSG 的域名空间使用小写字母开头，然后可以使用大写字母以避免混淆。例如，osg，osgSim，osgFX 等
* 类：OSG 的类名以大写字母开头，如果类的名称是多个单词的组合，此后每个单词的首字母大写。例如，MatrixTransform，NodeVisitor，Optimizer
* 类方法：OSG 类的方法名使用小写字母开头，如果方法的名称是多个单词的组合，此后每个单词的首字母大写。例如，addDrawable()，getNumChildren()，setAttributeAndModes()
* 类成员变量：类的成员变量命名与方法命名的方式相同
* 模板：OSG 模板的命名用小写字母，多个单词之间使用下划线分隔。例如，ref_ptr<>，graph_array<>，observer_ptr<>
* 静态量：静态变量和函数的名称使用 s_ 开头，此后的命名与类成员变量及函数的命名方法相同。例如，s_applicationUsage，s_ArrayNames()
* 全局量：全局类的实例命名用 g_ 开头。例如，g_NotifyLevel，g_readerWriter_BMP_Proxy
