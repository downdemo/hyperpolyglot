* [Haskell 标准库](https://hackage.haskell.org/package/base)
* [Learn You a Haskell for Great Good!](http://learnyouahaskell.com/chapters)
* [Real World Haskell](https://github.com/huangzworks/real-world-haskell-cn)
* [H-99: Ninety-Nine Haskell Problems](https://wiki.haskell.org/H-99:_Ninety-Nine_Haskell_Problems)

## Haskell 环境搭建

### 方式一：使用 Haskell 编译器 GHC

* 安装 [GHC](https://www.haskell.org/ghc/download.html)
* 运行 `ghc --version` 查看版本号检查是否安装成功
* 运行 `ghci`，即可使用 GHCi 解释器

```
GHCi, version 8.8.4: https://www.haskell.org/ghc/  :? for help
Prelude> 1+2
3
```

* 创建 `Main.hs` 内容如下，执行 `runghc Main.hs` 或 `runhaskell Main.hs` 运行

```hs
main :: IO ()
main = print "hello world"
```

### 方式二：使用包管理工具 Stack

* 安装 [Stack](https://docs.haskellstack.org/en/stable/README/)，Windows 直接下载 [安装包](https://get.haskellstack.org/stable/windows-x86_64-installer.exe)，Mac 执行

```
brew install haskell-stack
```

* 安装完成后查看版本

```
stack --version
Version 2.5.1, Git revision d6ab861544918185236cf826cb2028abb266d6d5 x86_64 hpack-0.33.0
```

* 下载 GHC

```
stack setup
```

* 如果很慢，替换 [清华源](https://mirror.tuna.tsinghua.edu.cn/help/stackage/)，在 `C:/sr/config.yaml` （Mac 为 `~/.stack/config.yaml`）中添加如下内容

```
setup-info-locations: ["http://mirrors.tuna.tsinghua.edu.cn/stackage/stack-setup.yaml"]
urls:
  latest-snapshot: http://mirrors.tuna.tsinghua.edu.cn/stackage/snapshots.json
```

* 替换源后再下载 GHC

```
stack setup

Warning: http://mirrors.tuna.tsinghua.edu.cn/stackage/stack-setup.yaml: Unrecognized field in GHCDownloadInfo: version
Preparing to install GHC to an isolated location.
This will not interfere with any system-level installation.
Downloaded ghc-8.8.4.
Downloaded 7z.dll.
Downloaded 7z.exe.
Decompressing ghc-8.8.4.tar.xz...

7-Zip 9.20  Copyright (c) 1999-2010 Igor Pavlov  2010-11-18

Processing archive: C:\Users\downdemo\AppData\Local\Programs\stack\x86_64-windows\ghc-8.8.4.tar.xz
Extracting  ghc-8.8.4.tar

Everything is Ok

Size:       2555320320
Compressed: 204705252
Extracting ghc-8.8.4.tar...
Extracted total of 10093 files from ghc-8.8.4.tar
GHC installed to C:\Users\downdemo\AppData\Local\Programs\stack\x86_64-windows\ghc-8.8.4\
Downloaded msys2-20200903.
Already downloaded.

Processing archive: C:\Users\downdemo\AppData\Local\Programs\stack\x86_64-windows\msys2-20200903.tar.xz

Extracting  msys2-20200903.tar

Everything is Ok

Size:       377667072
Compressed: 92352020
Extracting msys2-20200903.tar...
Extracted total of 17382 files from msys2-20200903.tar
Copying skeleton files.
These files are for the users to personalise their msys2 experience.

They will never be overwritten nor automatically updated.

'./.bashrc' -> '/home/downdemo/.bashrc'
'./.bash_logout' -> '/home/downdemo/.bash_logout'
'./.bash_profile' -> '/home/downdemo/.bash_profile'
'./.inputrc' -> '/home/downdemo/.inputrc'
'./.profile' -> '/home/downdemo/.profile'
stack will use a sandboxed GHC it installed
For more information on paths, see 'stack path' and 'stack exec env'
To use this GHC and packages outside of a project, consider using:
stack ghc, stack ghci, stack runghc, or stack exec
```

* 如果出现报错 `ConnectionFailure user error (Network.Socket.gai_strerror not supported: 11004)`，说明 `raw.githubusercontent.com` 无法访问，在 `C:\Windows\System32\drivers\etc\hosts` （Mac 为 `/etc/hosts`）中添加如下内容

```
199.232.4.133 raw.githubusercontent.com
```

* 安装完成后查看 GHC 版本，用 GHCi 解释器执行一条语句

```
stack ghc -- --version
The Glorious Glasgow Haskell Compilation System, version 8.8.4

stack ghci
Prelude> 1+2
3
```

* 新建项目

```
stack new test-for-haskell

Downloading template "new-template" to create project "test-for-haskell" in test-for-haskell\ ...

The following parameters were needed by the template but not provided: author-name
You can provide them in C:\sr\config.yaml, like this:
templates:
  params:
    author-name: value
Or you can pass each one as parameters like this:
stack new test-for-haskell new-template -p "author-name:value"


The following parameters were needed by the template but not provided: author-email, author-name, category, copyright, github-username
You can provide them in C:\sr\config.yaml, like this:
templates:
  params:
    author-email: value
    author-name: value
    category: value
    copyright: value
    github-username: value
Or you can pass each one as parameters like this:
stack new test-for-haskell new-template -p "author-email:value" -p "author-name:value" -p "category:value" -p "copyright:value" -p "github-username:value"

Looking for .cabal or package.yaml files to use to init the project.
Using cabal packages:
- test-for-haskell\

Selecting the best among 18 snapshots...

* Matches https://raw.githubusercontent.com/commercialhaskell/stackage-snapshots/master/lts/16/31.yaml

Selected resolver: https://raw.githubusercontent.com/commercialhaskell/stackage-snapshots/master/lts/16/31.yaml
Initialising configuration using resolver: https://raw.githubusercontent.com/commercialhaskell/stackage-snapshots/master/lts/16/31.yaml
Total number of user packages considered: 1
Writing configuration to file: test-for-haskell\stack.yaml
All done.
C:\sr\templates\new-template.hsfiles:    3.72 KiB downloaded...
```

* 编译

```
cd test-for-haskell
stack build

[1 of 2] Compiling Main             ( C:\\sr\setup-exe-src\setup-Z6RU0evB.hs, C:\\sr\setup-exe-src\setup-Z6RU0evB.o )
[2 of 2] Compiling StackSetupShim   ( C:\\sr\setup-exe-src\setup-shim-Z6RU0evB.hs, C:\\sr\setup-exe-src\setup-shim-Z6RU0evB.o )
Linking C:\\sr\\setup-exe-cache\\x86_64-windows\\tmp-Cabal-simple_Z6RU0evB_3.0.1.0_ghc-8.8.4.exe ...
Building all executables for `test-for-haskell' once. After a successful build of all of them, only specified executables will be rebuilt.
test-for-haskell> configure (lib + exe)
Configuring test-for-haskell-0.1.0.0...
test-for-haskell> build (lib + exe)
Preprocessing library for test-for-haskell-0.1.0.0..
Building library for test-for-haskell-0.1.0.0..
[1 of 2] Compiling Lib
[2 of 2] Compiling Paths_test_for_haskell
Preprocessing executable 'test-for-haskell-exe' for test-for-haskell-0.1.0.0..
Building executable 'test-for-haskell-exe' for test-for-haskell-0.1.0.0..
[1 of 2] Compiling Main
[2 of 2] Compiling Paths_test_for_haskell
Linking .stack-work\dist\29cc6475\build\test-for-haskell-exe\test-for-haskell-exe.exe ...
test-for-haskell> copy/register
Installing library in C:\Users\downdemo\Desktop\test-for-haskell\.stack-work\install\6f224cbe\lib\x86_64-windows-ghc-8.8.4\test-for-haskell-0.1.0.0-20O5XYB3fp2JaTVnB3A1Ht
Installing executable test-for-haskell-exe in C:\Users\downdemo\Desktop\test-for-haskell\.stack-work\install\6f224cbe\bin
Registering library for test-for-haskell-0.1.0.0..
```

* 运行

```
cd .stack-work\install\6f224cbe\bin\
stack exec test-for-haskell-exe.exe
```

* 用 [VSCode](https://code.visualstudio.com/Download) 打开文件夹，安装 [Haskell 插件](https://github.com/haskell/vscode-haskell)，安装过程中会自动安装 [haskell-language-server](https://github.com/haskell/haskell-language-server)，并保存在 `C:\Users\downdemo\AppData\Roaming\Code\User\globalStorage\haskell.haskell`

```
haskell-language-server-0.8.0-win32-8.8.4.exe
haskell-language-server-0.8.0-win32-8.8.4.exe.download.zip
haskell-language-server-wrapper-0.8.0-win32.exe
haskell-language-server-wrapper-0.8.0-win32.exe.download.zip
```

* 注意 Windows 要求 GHC 版本高于 8.8.2，如果版本过低可以用以下命令手动安装更高版本的 GHC，安装后会保存到 `C:\Users\downdemo\AppData\Local\Programs\stack\x86_64-windows`（Mac 为 `/Users/downdemo/.stack/programs/x86_64-osx`）

```
stack --resolver ghc-8.10.3 setup
```

* 或者从 [GHC 官网](https://www.haskell.org/ghc/download.html) 下载，下载后将压缩包移动到 `C:\Users\downdemo\AppData\Local\Programs\stack\x86_64-windows` 并解压
* 查看路径会发现当前依然使用旧版本 GHC

```
stack path
```

* 切换到指定版本的 GHC

```
stack config set resolver ghc-8.10.3
```

* 上述命令会在 `C:\sr\global-project\stack.yaml` 中添加如下内容

```
resolver:
  compiler: ghc-8.10.3
```

* 删除之前安装的 Haskell 插件及其缓存，再用 stack 创建一个新项目，在新创建项目中的 `stack.yaml` 中指定使用新的 GHC 版本

```
stack new demo
cd demo
stack config set resolver ghc-8.10.3
stack setup
stack build
```

* 用 VSCode 打开项目文件夹后再重装 `Haskell` 插件即可
