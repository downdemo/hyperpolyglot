## Windows

* 在 [Boost 官网](https://www.boost.org/) 下载源码包并解压
* 进入 VS 所在目录如 `C:\Program Files (x86)\Microsoft Visual Studio` 找到 cl.exe 如 `C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.16.27023\bin\Hostx64\x64`，将其所在路径添加到环境变量中，注销或重启以使环境变量生效
* 运行批处理文件 `bootstrap.bat`，将生成 boost 编译工具 `b2.exe` 和 `bjam.exe`，两者功能一样，前者新一点
* 运行 `bjam.exe` 即可开始自动编译 boost，大概半小时编译完成
* 编译产生的 `.lib` 文件将生成在 `boost_1_87_0\stage\lib` 目录下
* 也可以使用命令行指定部分需要的库进行编译

```sh
bjam --with-system --with-thread --with-date_time --with-regex --with-serialization stage
```

* 编译完成后，在 VS 中配置头文件和库文件目录，`配置属性 - C/C++ - 常规 - 附加包含目录` 设置 `..\boost_1_87_0`，`配置属性 - 链接器 - 常规 - 附加库目录` 设置 `..\boost_1_87_0\stage\lib;`
* 编译以下代码，成功运行

```cpp
#include <iostream>

#include "boost/asio.hpp"

int main() {
  boost::asio::io_context io;
  boost::asio::steady_timer t(io, boost::asio::chrono::seconds(3));
  t.wait();
  std::cout << "Hello, world!" << std::endl;
}
```

* 编译时有如下警告

```
1>Please define _WIN32_WINNT or _WIN32_WINDOWS appropriately. For example:
1>- add -D_WIN32_WINNT=0x0601 to the compiler command line; or
1>- add _WIN32_WINNT=0x0601 to your project's Preprocessor Definitions.
1>Assuming _WIN32_WINNT=0x0601 (i.e. Windows 7 target).
```

* 在 `配置属性 - C/C++ - 预处理器 - 预处理器定义` 中添加 `_WIN32_WINNT=0x0A00;` 即可，各 Windows 系统对应的_WIN32_WINNT 值如下

```cpp
#define _WIN32_WINNT_NT4 0x0400           // Windows NT 4.0
#define _WIN32_WINNT_WIN2K 0x0500         // Windows 2000
#define _WIN32_WINNT_WINXP 0x0501         // Windows XP
#define _WIN32_WINNT_WS03 0x0502          // Windows Server 2003
#define _WIN32_WINNT_WIN6 0x0600          // Windows Vista
#define _WIN32_WINNT_VISTA 0x0600         // Windows Vista
#define _WIN32_WINNT_WS08 0x0600          // Windows Server 2008
#define _WIN32_WINNT_LONGHORN 0x0600      // Windows Vista
#define _WIN32_WINNT_WIN7 0x0601          // Windows 7
#define _WIN32_WINNT_WIN8 0x0602          // Windows 8
#define _WIN32_WINNT_WINBLUE 0x0603       // Windows 8.1
#define _WIN32_WINNT_WINTHRESHOLD 0x0A00  // Windows 10
#define _WIN32_WINNT_WIN10 0x0A00         // Windows 10
```

## Linux

```sh
wget https://boostorg.jfrog.io/artifactory/main/release/1.87.0/source/boost_1_87_0.tar.gz
tar zxvf boost_1_87_0.tar.gz
cd boost_1_87_0
./boostrap.sh --prefix=/tmp
./b2 install
```

* `g++ gzip.cpp -I/tmp/boost/include -L/tmp/boost/lib -lboost_iostreams -Wl,-rpath=/tmp/boost/lib`

```cpp
#include <boost/iostreams/close.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

struct Message {
  Message() = default;
  Message(uint64_t num, const std::string& str) : num(num) {
    str.copy(msg, sizeof(msg));
  }

  bool operator==(const Message& rhs) const {
    return num == rhs.num && strcmp(msg, rhs.msg) == 0;
  }

  uint64_t num = 0;
  char msg[512] = {};
} __attribute__((packed));

class GZIPWriter {
 public:
  GZIPWriter(const std::string& path)
      : fstream_(path, std::ios_base::app | std::ios_base::binary) {
    assert(fstream_.is_open());
    gzip_.push(boost::iostreams::gzip_compressor(boost::iostreams::gzip_params(
        boost::iostreams::gzip::best_compression)));
    gzip_.push(fstream_);
  }

  ~GZIPWriter() { boost::iostreams::close(gzip_); }

  void append(const Message& msg) {
    gzip_.write(reinterpret_cast<const char*>(&msg), sizeof(msg));
  }

 private:
  std::ofstream fstream_;
  boost::iostreams::filtering_ostream gzip_;
};

class GZIPReader {
 public:
  GZIPReader(const std::string& path) : fstream_(path) {
    assert(fstream_.is_open());
    gzip_.push(boost::iostreams::gzip_decompressor());
    gzip_.push(fstream_);
  }

  std::vector<Message> read() {
    std::vector<Message> res;
    Message msg = {};
    while (!gzip_.read(reinterpret_cast<char*>(&msg), sizeof(msg)).eof()) {
      if (msg.num == 0) {  // 不完整的 gzip 文件，跳过尾部避免陷入死循环
        break;
      }
      res.emplace_back(std::move(msg));
      msg = {};
    }
    return res;
  }

 private:
  std::ifstream fstream_;
  boost::iostreams::filtering_istream gzip_;
};

std::string run_cmd(const std::string& cmd) {
  std::string res;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"),
                                                pclose);
  if (!pipe) {
    return res;
  }
  char buf[128] = {};
  while (fgets(buf, 128, pipe.get())) {
    res += buf;
  }
  return res;
}

int main() {
  std::string file_path = "/tmp/test.txt";
  std::string gzip_file_path = file_path + ".gz";
  run_cmd("rm -rf  " + gzip_file_path);
  run_cmd("touch " + gzip_file_path);
  std::vector<Message> data;
  data.emplace_back(1, "abc");
  data.emplace_back(2, "cde");
  {
    GZIPWriter writer{gzip_file_path};
    for (const auto& x : data) {
      writer.append(x);
    }
  }
  GZIPReader reader{gzip_file_path};
  std::vector<Message> res = reader.read();
  assert(res == data);
  run_cmd("gzip -d " + gzip_file_path);
  run_cmd("rm -rf  " + file_path);
}
```
