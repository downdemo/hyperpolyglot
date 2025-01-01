[Protocol Buffers：C++ Generated Code](https://developers.google.com/protocol-buffers/docs/reference/cpp-generated)

* 安装 [protobuf](https://github.com/protocolbuffers/protobuf/releases)

```sh
brew install protobuf
```

* 检查版本

```sh
protoc --version
libprotoc 3.14.0
```

* 创建 person.proto 文件，内容如下

```
syntax = "proto3";

package downdemo.test;

option go_package = "downdemo.io/test";

message Person {
    string name = 1;
    int32 age = 2;
}
```

## C++

* 创建 `main.cpp`，内容如下

```cpp
#include <iostream>

#include "person.pb.h"

int main() {
  downdemo::test::Person p;
  p.set_name("a");
  p.set_age(12);
  std::cout << p.name() << p.age() << std::endl;  // a12
  std::string data = p.SerializeAsString();
  downdemo::test::Person p2;
  p2.ParseFromString(data);
  std::cout << p2.name() << p2.age() << std::endl;  // a12
}
```

### 手动编译

* 执行以下命令编译当前目录的所有 proto 文件，将在 `./build` 目录下生成 `person.pb.h` 和 `person.pb.cc`

```sh
mkdir build && protoc --proto_path=. --cpp_out=./build *.proto
```

* 编译

```sh
g++ -std=c++11 ./build/*.pb.cc main.cpp -I./build -lprotobuf -o main
```

* 运行

```sh
./main

a12
a12
```

### CMake 编译

* 安装 CMake

```sh
brew install cmake
```

* 检查版本

```sh
cmake --version

cmake version 3.19.1

CMake suite maintained and supported by Kitware (kitware.com/cmake).
```

* 创建 `CMakeLists.txt` 内容如下，使用 [FindProtobuf](https://cmake.org/cmake/help/latest/module/FindProtobuf.html) 检测 protobuf

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(demo-project LANGUAGES CXX)

find_package(Protobuf REQUIRED)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS person.proto)

add_executable(main main.cpp ${PROTO_HDRS} ${PROTO_SRCS})

target_include_directories(main PRIVATE ${Protobuf_INCLUDE_DIRS})
target_include_directories(main PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(main ${PROTOBUF_LIBRARIES})

set_target_properties(main
  PROPERTIES
    CXX_STANDARD 11
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE 1
)
```

* 编译并运行

```sh
mkdir build && cd build && cmake .. && cmake --build . && ./main

-- The CXX compiler identification is AppleClang 11.0.3.11030032
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /Library/Developer/CommandLineTools/usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Found Protobuf: /usr/local/lib/libprotobuf.dylib (found version "3.14.0") 
-- Configuring done
-- Generating done
-- Build files have been written to: /Users/wjc/Desktop/demo/test-for-cpp/build
[ 25%] Running cpp protocol buffer compiler on person.proto
Scanning dependencies of target main
[ 50%] Building CXX object CMakeFiles/main.dir/main.cpp.o
[ 75%] Building CXX object CMakeFiles/main.dir/person.pb.cc.o
[100%] Linking CXX executable main
[100%] Built target main
a12
a12
```

## Go

* 初始化 go.mod

```sh
go mod init demo-project
```

* 创建 `main.go`，内容如下

```go
package main

import (
	"demo-project/build/downdemo.io/test"
	"fmt"

	"google.golang.org/protobuf/proto"
)

func main() {
	p := &test.Person{
		Name: "a",
		Age:  12,
	}
	fmt.Println(p.GetName(), p.GetAge())
	data, _ := proto.Marshal(p)
	p2 := &test.Person{}
	proto.Unmarshal(data, p2)
	p2.Age = 13
	fmt.Println(p2.GetName(), p2.GetAge())
}
```

* 安装 protoc-gen-go

```sh
go get github.com/golang/protobuf/protoc-gen-go

go: found github.com/golang/protobuf/protoc-gen-go in github.com/golang/protobuf v1.4.3
go: downloading google.golang.org/protobuf v1.23.0
```

* 添加 `$GOPATH/bin` 到环境变量

```sh
export PATH=$GOPATH/bin:$PATH
```

* 执行以下命令编译当前目录的所有 proto 文件，将在 `./build` 目录下生成 `downdemo.io/test/person.pb.go`

```sh
mkdir build && protoc --proto_path=. --go_out=./build *.proto
```

* 安装 protobuf

```sh
go get github.com/golang/protobuf/proto

go: found github.com/golang/protobuf/proto in github.com/golang/protobuf v1.4.3
```

* 运行

```sh
go run main.go

a 12
a 13
```
