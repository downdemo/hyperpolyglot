## 操作

* 移动光标

```sh
# 下翻一行
ctrl+e
# 上翻一行
ctrl+y
# 下翻半页
ctrl+d
# 上翻半页
ctrl+u
# 下翻一页
ctrl+f
# 上翻一页
ctrl+b
# 将当前行显示为屏幕居中行
zz
# 将当前行显示为第一行
zt
# 将当前行显示为最后一行
zb
# 移动到行首
0
# 移动到行尾
$
# 移动到文件头
gg
# 移动到文件尾
G
# 移动到单词头
b
# 移动到单词尾
e
# 移动到下个单词头
w
# 右缩进
>>
# 左缩进
<<
# 多行缩进
v+>>
```

* 插入

```sh
# 插入到光标前
i
# 插入到光标后
a
# 插入到行首
I
# 插入到行尾
A
# 下一行插入空行
o
# 上一行插入空行
O
```

* 删除

```sh
# 剪切当前字符
x
# 剪切当前行
dd
# 剪切当前位置到移动到目标位置之间的内容
d+光标移动指令
# 删除 [行首, 当前位置) 的所有字符
d0
# 删除 [当前位置, 行尾（不含行尾的换行）] 的所有字符
d$
# 撤销
u
# 反撤销
ctrl+r
# 移动 n 次
数字+光标移动指令
# 剪切到移动 n 次后位置之间的内容
d+数字+光标移动指令
```

* 拷贝

```sh
# 小写 p 向后粘贴，如果粘贴的是行则在下一行粘贴，如果不是行则在光标所在位置后粘贴
p
# 大写 P 向前粘贴
P
# 复制当前行
yy
# 复制，与光标指令结合方式与 d 同理
y
# 输入一个新字符替换当前字符
r
# 输入 n 个新字符替换当前字符开始的 n 个字符
数字+r
# 替换模式，直到按 Esc 退出
R
# 剪切并进入插入模式（和 d 再 i 相比光标位置会有一些不同）
c
```

* 查询

```sh
# 查看光标当前位置
crtl+g
# 跳转到 n行（或者 # 行号）
行号+G
# 跳到匹配括号的另一半
%
# 从当前位置开始往下搜索字符串，n 跳转到下一个，N 上一个，搜索特殊符号（.*[]^%/?~$）要加 \ 转义
/+string
# 与 / 搜索方向相反
?+string
# 替换当前行的第一个 old 为 new
:s/old/new
# 整行替换
:s/old/new/g
# 全局替换
:%s/old/new/g
# 每次替换时询问（y/n/a/q/l/^E/^Y）？分别代表 yes/no/all/quit/替换并进入插入模式/向下滚动/向上滚动
:s/old/new/gc
# 替换 3 到 5 行
3,5:ws/old/new/g
```

* 其他

```sh
# 文件另存为 1.txt
:w 1.txt
# 将块内容另存为 1.txt
v 选中块后再 :w 1.txt
# 读取 1.txt 文件内容粘贴到光标下一行
r 1.txt
# 垂直并排打开 1.txt 2.txt
vim -o 1.txt 2.txt
# 水平并排打开 1.txt 2.txt
vim -O 1.txt 2.txt
# 打开多文件时切换光标所在窗口
ctrl+w+方向键
# 关闭所有文件
:qa！
# 查看设置参数
:set all
# 显示行号
:set number
# 关闭行号
:set nonumber
# tab大小
:set tabstop=4
# 缩进大小
:set shiftwidth=4
# 改变字体大小
ctrl+shift+'+'
ctrl+'-'
# 改变窗口大小
crtl+w+'+'
ctrl+w+'-'
```

## 配置

* 全局配置文件为 `/etc/vim/vimrc`，一般不直接修改，而是为每个用户配置文件为 `~/.vimrc`，如果没有则可以手动创建一个，比如可以将上述默认操作添加到配置文件

```sh
set number
set tabstop=4
set shiftwidth=4
set expandtab
```

* 为了使用插件，下载 [vim-plug](https://github.com/junegunn/vim-plug)，将 `plug.vim` 移动到 `~/.vim/autoload` 下
* 以安装 [YouCompleteMe](https://github.com/ycm-core/YouCompleteMe) 为例，用 vim 打开 `~/.vimrc`，添加如下内容，`:source ~/.vimrc` 或重启 vim

```sh
call plug#begin('~/.vim/plugged')

Plug 'ycm-core/YouCompleteMe'

call plug#end()
```

* 执行 `:PlugInstall` 即可安装所有指定的插件，插件的安装目录为 `./.vim/plugged`
* 安装 YCM 出现报错

```sh
x Post-update hook for YouCompleteMe ... Exit status: 1
```

* 进入插件目录

```sh
cd ~/.vim/plugged/YouCompleteMe
```

* 运行其中的 `run_test.py`

```sh
python3 run_test.py
```

* 报错缺少依赖，先安装 pip

```sh
sudo apt install python3-pip
```

* 依次用 pip 安装依赖即可

```sh
pip3 install flake8
pip3 install pytest

sudo apt search hamcrest
sudo apt install python3-hamcrest
```

* 最后设置 clang 补全

```sh
python3 install.py --clang-completer
```

* 用 vim 创建一个源文件即可发现支持自动补全

```sh
vim test.cpp
```

* 但是补全效果不理想，为此还需要做额外配置

```sh
sudo cp ~/.vim/plugged/YouCompleteMe/third_party/ycmd/.ycm_extra_conf.py ~/.ycm_extra_conf.py
vim ~/.ycm_extra_conf.py
```

* 在

```sh
'-isystem',
'cpp/BoostParts',
```

* 下面继续添加包含头文件的目录

```sh
'-isystem',
'/usr/include',
'-isystem',
'/usr/lib',
'-isystem',
'/usr/local/include',
'-isystem',
'/usr/include/c++/9',
'-isystem',
'usr/include/x86_64-linux-gnu',
'-isystem',
'usr/include/x86_64-linux-gnu/c++',
'-isystem',
'usr/include/x86_64-linux-gnu/c++/9',
```

* 在 `.vimrc` 中添加以下内容，默认使用 `~/.ycm_extra_conf.py`

```sh
let g:ycm_global_ycm_extra_conf = '~/.ycm_extra_conf.py'
let g:ycm_confirm_extra_conf=0 "每次直接加载.ycm_extra_conf.py，而不需要询问
```
