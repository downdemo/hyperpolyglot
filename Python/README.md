## Python 环境搭建

* 安装 [pyenv](https://github.com/pyenv/pyenv)

```sh
brew install pyenv
```

* 或者直接手动安装

```sh
git clone https://github.com/yyuu/pyenv.git ~/.pyenv
```

* 添加环境变量

```sh
echo 'export PYENV_ROOT="$HOME/.pyenv"' >> ~/.zshrc
echo 'export PATH="$PYENV_ROOT/bin:$PATH"' >> ~/.zshrc
echo 'eval "$(pyenv init -)"' >> ~/.zshrc
source ~/.zshrc
```

* 显示可安装的 python 版本列表

```sh
pyenv install --list
```

* 比如安装 3.9.6

```sh
pyenv install 3.9.6
```

* 如果安装很慢，先到 [淘宝镜像](https://npm.taobao.org/mirrors/python/) 下载对应版本的包，保存到 `~/.pyenv/cache`

```sh
mkdir -p ~/.pyenv/cache/ && \
    wget https://npm.taobao.org/mirrors/python/3.9.6/Python-3.9.6.tar.xz -P ~/.pyenv/cache/
```

* 再重新执行安装指令即可。安装后设置为全局 python 版本

```sh
pyenv global 3.9.6
pyenv version
3.9.6 (set by /Users/downdemo/.pyenv/version)
```

* 查看所有 pyenv 命令

```sh
pyenv commands
```

* 更换 PIP 源

```sh
mkdir ~/.pip
cat > ~/.pip/pip.conf
[global]
index-url=https://mirrors.aliyun.com/pypi/simple/
[install]
trusted-host=mirrors.aliyun.com
```

* 安装 [PyCharm](https://www.jetbrains.com/pycharm/download/#section=windows)
* 安装后打开，点击 `Create New Project` 创建项目路径，解释器选项 `interpreter` 设为 python 解释器的路径，比如

```
C:\Users\xxx\AppData\Local\Programs\Python\Python37-32\python.exe
```

* 项目创建完成后，右键项目文件夹，选择 `New - Python File` 创建一个 python 文件，输入如下代码

```py
print('hi')
```

* 点击右上角的绿色箭头（如果箭头为灰色，右键代码空白处，点击 `Run`，箭头将变为绿色）或按下 `Ctrl + Shift + F10` 即可看到运行结果
* 选择 `File - Settings - Project:xxx - Project Interpreter`，点击 `+` 即可安装第三方模块
* 使用 PIP 安装的第三方模块位置查看方式

```py
import sys


print(sys.path)
```

* 结果为

```
C:\Users\xxx\AppData\Local\Programs\Python\Python37-32\Lib\site-packages
```

* 要导入第三方模块，则需要把该路径添加到环境变量 `PYTHONPATH` 中
