* C++11 标准草案（N3242）：http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2011/n3242.pdf
* C++14 标准草案（N3797）：http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3797.pdf
* C++17 标准草案（N4659）：http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4659.pdf
* C++20 标准草案（N4849）：http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/n4849.pdf
* https://cpp.extension.sh/
* https://gcc.godbolt.org/
* https://quick-bench.com/
* https://gcc.gnu.org/onlinedocs/libstdc++/manual/abi.html
* [cppquiz](http://cppquiz.org)
* [语言对照表](https://hyperpolyglot.org/)
* [optimize manual](https://www.agner.org/optimize/)
* [Intel SDM](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)
* [hashmap bench](https://martin.ankerl.com/2022/08/27/hashmap-bench-01/)

## 环境搭建

### IDE

* Windows：[Visual Studio](https://visualstudio.microsoft.com/downloads/)
* Mac/Linux：[CLion](https://www.jetbrains.com/clion/download)

### Windows、Mac 下的 Linux 编程

* 代码补全
  * IDE 远程开发：[CLion](https://www.jetbrains.com/clion/download) + Remote Host Toolchain
  * 编辑器远程开发：[VSCode](https://code.visualstudio.com/Download) + LLVM 插件
* 构建工具：[CMake](https://cmake.org/download/)
* Linux 远程机
  * 通用：[Docker](https://www.docker.com/products/docker-desktop)、[阿里云 ECS](https://ecs-workbench.aliyun.com/)
  * Windows：WSL、[VMware Workstation Pro](https://my.vmware.com/cn/web/vmware/downloads/info/slug/desktop_end_user_computing/vmware_workstation_pro/16_0) + [Ubuntu 18.04.5 LTS (Bionic Beaver) Server install image](https://releases.ubuntu.com/)
  * Mac：[Parallels Desktop Pro](https://www.parallels.cn/products/desktop/pro/) + [Ubuntu 18.04.5 LTS (Bionic Beaver) Server install image](https://releases.ubuntu.com/)
* Shell
  * Windows：[MobaXterm](https://mobaxterm.mobatek.net/download-home-edition.html)
  * Mac：[iTerm2](https://iterm2.com/) + [Oh-My-Zsh](https://github.com/ohmyzsh/ohmyzsh)

## CLion 远程开发环境配置

* 准备一台已安装 CMake、g++、gdb 等 C++ 开发工具链的 Linux 远程机
* 安装 [CLion](https://www.jetbrains.com/clion/download)
* 在本地新建一个文件夹作为本地项目路径，打开 CLion，点击 `New Project`，选择 `C++ Excutable`，`Location` 输入本地项目路径，`Language standard` 选择要使用的 C++ 标准，如 `C++17`，点击 `Create` 创建项目。项目创建后会在本地项目路径中生成 `.idea` 文件夹、`cmake-build-debug` 文件夹、`CMakeLists.txt`、`main.cpp`，若出现 CLion 配置问题，删除 `.idea` 文件夹后再打开项目重设配置即可
* 设置使用远程机的 C++ 工具链，打开 `File - Settings - Build, Execution, Deployment - Toolchains`，在右侧的对话框中点击加号，在弹出的选项中选择 `Remote Host`
  * `Name`：任意名称
  * `Credentials`：点击右侧的选项按钮，弹出 `SSH Configurations`，点击加号，输入远程机的 SSH 相关配置信息，点击 `Test Connection` 检查连接，成功连接后点击 `OK` 保存即可在下拉框中选择该远程机
  * `CMake`：远程机的 CMake 路径，会自动检测，检测成功后会显示 Version，也可以手动输入，如 `\usr\bin\cmake`
  * `Make`：无需设置，不使用
  * `C Compiler`：远程机的 gcc 路径，会自动检测，检测成功后会显示 Version，也可以手动输入，如 `\usr\bin\gcc`
  * `C++ Compiler`：远程机的 g++ 路径，会自动检测，检测成功后会显示 Version，也可以手动输入，如 `\usr\bin\g++`
  * `Debugger`：远程机的 GDB 路径，会自动检测，检测成功后会显示 Version，也可以手动输入，如 `\usr\bin\gdb`
* 打开 `File - Settings - Build, Execution, Deployment - CMake`，默认会有 `Debug` 的配置，点击加号，默认增加 `Release` 配置，`ToolChain` 均使用已设置的远程机 Toolchain 的 `Name` 即可
* 打开 `File - Settings - Build, Execution, Deployment - Deployment`，若完成 Toolchain 配置则此处会默认生成远程机配置，若没有配置手动配置即可
  * 点击加号，再弹出的下拉框中选择 `SFTP`，弹出对话框提示输入 `server name`，输入任意名称即可
  * `SSH configuration` 使用已配置过的 SSH 配置即可，若无配置则点击右侧的按钮，输入远程机的 SSH 相关配置信息，点击 `Test Connection` 检查连接，成功连接后点击 `OK` 保存即可在下拉框中选择该远程机
  * `Root path`：`/`
  * `Web server URL`：无需配置
  * 点击 `Mappings` 页，`Local path` 输入本地项目路径，`Deployment path` 输入要映射到的远程机目录
* 上述配置完成后，在主界面的 Project 中点击本地项目的文件夹，点击任务栏的 `Tools - Deployment - Upload to`，在弹出的列表中点击要上传的远程机，即会将本地文件夹的所有文件传送到映射的远程机目录
* 点击 `Build - Rebuild All in Debug` 即可编译 Debug 版本，编译后点击绿色箭头即可直接运行
* 如果修改了 `CMakeLists.txt` 或删除了 cmake 生成的文件夹，点击主界面下方任务栏中的 `CMake` 页，点击刷新图标 Reload CMake Project 即可
* 点击 `Tools - Resync with Remote Hosts` 将同步远程文件到本地，并且可以修复 IDE 中找不到标准库头文件的问题

## VSCode 编辑器环境配置

* 安装 [VSCode](https://code.visualstudio.com/Download)
* 安装插件 `Remote - SSH`，即可 ssh 远程访问虚拟机中的文件夹
* 安装插件 `C/C++` 和 `C/C++ Clang Command Adapter`，安装后实现智能提示
* 安装 `Clang-Format` 插件，Windows 将直接下载 `clang-format` 到

```
C:\Users\downdemo\.vscode\extensions\ms-vscode.cpptools-0.29.0\LLVM\bin
```

* 如果没有自动下载，则需要到 [LLVM](http://llvm.org/builds/) 手动 [下载](https://prereleases.llvm.org/win-snapshots/LLVM-11.0.0-2663a25f-win64.exe) 安装
* Mac 则需要先安装 `homebrew`

```sh
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/uninstall)"
```

* 如果上述命令安装失败，则可以使用国内镜像

```sh
/bin/zsh -c "$(curl -fsSL https://gitee.com/cunkai/HomebrewCN/raw/master/Homebrew.sh)"
```

* 使用 `homebrew` 安装 `clang-format`

```sh
brew install clang-format
```

* 如果长时间卡在如下状态，按 `ctrl + c` 即可中断更新 homebrew 并开始安装 `clang-format`

```
Updating Homebrew...
```

* 安装完成后将显示 `clang-format` 所在的目录

```
==> Downloading https://homebrew.bintray.com/bottles/clang-format-11.0.1.big_sur
==> Downloading from https://d29vzk4ow07wi7.cloudfront.net/a2a124fe20d721ad49073
######################################################################## 100.0%
==> Pouring clang-format-11.0.1.big_sur.bottle.2.tar.gz
🍺  /usr/local/Cellar/clang-format/11.0.1: 12 files, 7MB
```

* Ubuntu 14.04.6 安装 `clang-format` 会提示找不到软件包，可安装 `clang-format-3.9`

```sh
apt-cache search clang-format # 查找软件包
sudo apt-get install clang-format-3.9
dpkg -L clang-format-3.9 # 查找安装位置
```

* 打开 `Preferences - Settings`，选择 `Extensions - Clang-Format configuration`，设置 `clang-format` 所在的路径以及需要使用的格式化风格
  * Executable（Windows）：`C:\Users\downdemo\.vscode\extensions\ms-vscode.cpptools-0.29.0\LLVM\bin\clang-format.exe`
  * Executable（Mac）：`/usr/local/Cellar/clang-format/11.0.1/bin/clang-format`
  * Executable（Ubuntu 14.04.6）：`/usr/bin/clang-format-3.9`
  * Fallback Style（候补的格式化风格）：`Google`
  * Style（默认的格式化风格）：`Google`
* 选择 `Text Editor - Formatting`，勾选 `Format On Save`，这样每次保存文件将自动格式化代码。手动格式化快捷键 `Alt + Shift + F`
* Windows 下按以上步骤安装 clang-format 实际上安装的是 LLVM，再安装插件 `C/C++ Clang Command Adapter`，将 `.vscode\c_cpp_properties.json` 中 `intelliSenseMode` 由 `msvc-x64` 改为 `clang-x64` 即可实现对 Linux API 的自动补全

```json
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/**"
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE"
            ],
            "windowsSdkVersion": "10.0.18362.0",
            "compilerPath": "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.25.28610/bin/Hostx64/x64/cl.exe",
            "cStandard": "c17",
            "cppStandard": "c++17",
            "intelliSenseMode": "gcc-x64"
        }
    ],
    "version": 4
}
```

## 与虚拟机共享文件夹

### Parallels Desktop 16

* 以已安装虚拟机版本 `ubuntu-18.04.5-live-server-amd64.iso` 为例
* 关闭虚拟机，打开配置，在 `硬件 - CD/DVD1` 中，将 `源` 设置为 `/Applications/Parallels Desktop.app/Contents/Resources/Tools/prl-tools-lin.iso`
* 打开虚拟机，初始状态下 `/media` 内容为空，在其中创建挂载文件夹

```sh
sudo mkdir /media/cdrom
```

* 挂载光盘

```sh
sudo mount -o exec /dev/cdrom /media/cdrom

ll /media/cdrom
total 586K
-r-xr-xr-x 1 root root  963 Nov  7 02:13 install
dr-xr-xr-x 4 root root 6.0K Nov  7 02:13 installer
-r-xr-xr-x 1 root root 573K Nov  7 02:13 install-gui
dr-xr-xr-x 2 root root 2.0K Nov  7 02:13 kmods
dr-xr-xr-x 6 root root 4.0K Nov  7 02:13 tools
-r--r--r-- 1 root root   13 Nov  7 02:13 version
```

* 安装（卸载也是通过该程序）

```sh
sudo /media/cdrom/install
```

* 按照提示安装完成后，`/media` 下将多出一个 `psf` 目录，所有共享目录将出现在该目录下

```sh
ll /media
total 6.0K
dr-xr-xr-x 5 root root 2.0K Nov  7 02:13 cdrom
dr-xr-xr-x 3 root root 4.0K Dec 11 11:28 psf
```

* 在控制中心打开虚拟机配置，指定主机文件夹后，即可在虚拟机的共享目录中看到主机文件夹
  * 仅共享某个指定的文件夹，在 `选项 - 共享` 中设置 `自定义文件夹`
  * `共享文件夹` 选项用于共享特定的 Mac 目录，可以选择 `不共享`、`仅 Home 文件夹`、`所有磁盘`
  * `与 Linux 共享 Mac 用户文件夹`、`与 Linux 共享云文件夹`、`将 Mac 宗卷映射到 Linux` 均无需勾选

### VirtualBox

* 点击虚拟机窗口上的 `Devices（设备） - Insert Guest Addtions CD image...（安装增强功能）` 挂载增强工具的镜像
* 创建挂载文件夹

```sh
mkdir -p /mnt/cdrom
```

* 挂载光盘

```sh
sudo mount /dev/cdrom /mnt/cdrom
```

* 安装增强工具

```sh
./mnt/cdrom/VBoxLinuxAdditions.run
```

* 创建共享文件夹

```sh
sudo mkdir -p /mnt/share
```

* 打开 VirtualBox，点击 `设置 - 共享文件夹`，点击加号，选择一个本机文件夹（如 `/Desktop/shared-virtualbox`）作为共享文件夹，勾选 `固定分配`
* 在虚拟机中挂载共享文件夹

```sh
cd ~
sudo mount -t vboxsf shared-virtualbox /mnt/share
```

* 挂载后，本机文件夹（`/Desktop/shared-virtualbox`）和虚拟机文件夹（`/mnt/share`）即可共享文件

### VMware Workstation 16

* 打开虚拟机设置，在 `选项 - 共享文件夹` 中点击 `总是启用`，并点击 `添加` 设置要共享的文件夹，虚拟机中共享目录为 `/mnt/hgfs`，将主机目录挂载到虚拟机共享目录

```sh
sudo vmhgfs-fuse .host:/ /mnt/hgfs -o allow_other -o nonempty
```

### Docker

* docker 可以直接在启动 Container 时用 `-v` 参数 mount 指定文件夹

```sh
docker run -tid -v C:/docker:/shared <IMAGE_ID>
```

### 其他

* 如果无法共享文件夹，可以使用传输文件的工具来同步，如 [Beyond Compare](http://www.scootersoftware.com/download.php)、[FileZilla](https://filezilla-project.org/download.php)、[XFTP](https://www.netsarang.com/zh/free-for-home-school/)

## 连接远程机

* 先在远程机中启动 ssh 服务。如果远程机是 Windows，参考 [安装适用于 Windows Server 2019 和 Windows 10 的 OpenSSH](https://docs.microsoft.com/en-US/windows-server/administration/openssh/openssh_install_firstuse)

```sh
sudo service ssh start
```

* 如果运行失败，则需要先安装相关包

```sh
sudo apt install -y openssh-server
```

* 查看状态

```sh
sudo service ssh status

# 如果启动成功可以看到
Active: active (running)
```

* 其他指令

```sh
sudo service ssh status # 查看状态
# 如果启动成功可以看到
Active: active (running)

sudo service ssh stop # 关闭
sudo service ssh restart # 重启
```

* 检查是否启动

```sh
sudo ps -e | grep sshd

# 如果启动成功将显示一个数
3289 ?          00:00:00 sshd
```

* 成功启动后，查看 IP 地址

```sh
ifconfig
```

* 如果 ifconfig 不生效，则需要先安装

```sh
sudo apt install -y net-tools
```

* 之后即可在本机用如下命令远程连接

```sh
ssh <用户名>@<IP地址>
```

* 如果要连接的是 VirualBox 中的虚拟机，需要先在 `设置 - 网络` 中将 `连接方式` 设为 `桥接网卡`
* 如果希望每次连接时不需要输入密码，在远程机中创建 `~/.ssh/authorized_keys` 文件，并将本机的 ssh 公钥（id_rsa.pub）内容拷贝到该文件中即可

```sh
vim ~/.ssh/authorized_keys
```

* 生成 ssh 钥匙对的执行命令为

```sh
ssh-keygen
```

* 生成的钥匙对默认保存地址
  * Windows：`C:/User/downdemo/.ssh`
  * Mac：`/User/downdemo/.ssh`
  * Linux：`/root/.ssh`

* 其中 `id_rsa` 为私钥，`id_rsa.pub` 为公钥，一串钥匙对可以同时在多台机器上使用。可以直接用如下命令将本地的公钥添加到远程机的 `~/.ssh/authorized_keys` 文件中

```sh
ssh-copy-id -i <公钥本地路径> <用户名>@<IP地址>
```

* 把钥匙对拷到远程机

```sh
scp .ssh/id_rsa* downdemo@10.211.1.111:~/.ssh
```

* 私钥需要设置足够私有的权限，否则会被视为无效

```sh
sudo chmod id_rsa 600
```
