* [CMake Documentation](https://cmake.org/cmake/help/latest/)

## 单文件编译

* 创建 `main.cpp`，内容如下

```cpp
#include <iostream>

int main() { std::cout << "Hello" << std::endl; }
```

* 创建 `CMakeLists.txt`，内容如下

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-01 LANGUAGES CXX)

add_executable(hello-world main.cpp)
```

* `cmake` 后指定 `CMakeLists.txt` 所在路径，将在当前目录生成配置，可以直接用 `-B` 参数指定输出目录，通常习惯使用 build 作为输出目录名

```
cmake . -Bbuild

-- The CXX compiler identification is GNU 7.5.0
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /home/devops/share/build
```

* 生成的配置文件如下

```
tree build 

build
├── CMakeCache.txt
├── CMakeFiles
│   ├── 3.10.2
│   │   ├── CMakeCXXCompiler.cmake
│   │   ├── CMakeDetermineCompilerABI_CXX.bin
│   │   ├── CMakeSystem.cmake
│   │   └── CompilerIdCXX
│   │       ├── a.out
│   │       ├── CMakeCXXCompilerId.cpp
│   │       └── tmp
│   ├── cmake.check_cache
│   ├── CMakeDirectoryInformation.cmake
│   ├── CMakeOutput.log
│   ├── CMakeTmp
│   ├── feature_tests.bin
│   ├── feature_tests.cxx
│   ├── hello-world.dir
│   │   ├── build.make
│   │   ├── cmake_clean.cmake
│   │   ├── DependInfo.cmake
│   │   ├── depend.make
│   │   ├── flags.make
│   │   ├── link.txt
│   │   └── progress.make
│   ├── Makefile2
│   ├── Makefile.cmake
│   ├── progress.marks
│   └── TargetDirectories.txt
├── cmake_install.cmake
└── Makefile
```

* 用 `cmake --build` 指定 `Makefile` 所在目录即可编译

```
cd build
cmake --build .

Scanning dependencies of target hello-world
[ 50%] Building CXX object CMakeFiles/hello-world.dir/main.cpp.o
[100%] Linking CXX executable hello-world
[100%] Built target hello-world
```

* 运行

```
./hello-world
Hello
```

* `cmake --build .` 相当于

```
cmake --build . --target all
```

* 可以通过 `--target` 指定如下构建规则，详见 https://cmake.org/runningcmake/
  * all：构建所有目标
  * clean：清除生成文件
  * test：在 CTest 的帮助下运行测试
  * install：执行项目的安装规则
  * depend：为源文件生成依赖
  * dependlocal：仅为当前目录生成依赖
  * rebuild_cache：再次调用 CMake 重新构建 `CMakeCache.txt`
  * edit_cache：允许直接编辑 cache entries

* 可以通过如下指令查看支持 Makefile 支持的选项

```
cmake --build . --target help

The following are some of the valid targets for this Makefile:
... all (the default if no target is provided)
... clean
... depend
... rebuild_cache
... hello-world
... edit_cache
... main.o
... main.i
... main.s
```

## 多文件编译

* 创建 `person.h`，内容如下

```cpp
#pragma once

#include <string>

class Person {
 public:
  explicit Person(const std::string& name);
  std::string Name() const;

 private:
  std::string name_;
};
```

* 创建 `person.cpp`，内容如下

```cpp
#include "person.h"

Person::Person(const std::string& name) : name_(name) {}

std::string Person::Name() const { return name_; }
```

* `main.cpp` 改为如下内容

```cpp
#include <iostream>

#include "person.h"

int main() {
  Person p("hello");
  std::cout << p.Name() << std::endl;
}
```

* 在 `CMakeLists.txt` 的 `add_executable` 参数中添加新增文件，重新生成配置并编译即可

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-01 LANGUAGES CXX)

add_executable(hello-world main.cpp person.h person.cpp)
```

* 对于一串文件名，可以用一个变量代替，一般对于局部变量，名称前会加上下划线

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-01 LANGUAGES CXX)

list(APPEND _sources person.h person.cpp)
add_executable(hello-world main.cpp ${_sources})
```

## 静态库与动态库

* 将新增的 `person.h` 和 `person.cpp` 编为静态库，再链接，`CMakeLists.txt` 改为如下内容

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-01 LANGUAGES CXX)

add_library(person STATIC person.h person.cpp)
add_executable(hello-world main.cpp)
target_link_libraries(hello-world person)
```

* `CMakeLists.txt` 中的 `add_library (person STATIC person.h person.cpp)` 会使得编译时生成静态库文件 `libperson.a`（Windows 生成 `person.lib`，Mac 同样生成 `libperson.a`），`target_link_libraries (hello-world person)` 则会链接静态库，如果把 `add_library` 的第二个参数由 `STATIC` 改为 `SHARED`，则会生成动态库文件 `libperson.so`（Windows 生成 `person.dll`，Mac 生成 `libperson.dylib`），一般动态库文件会静态库文件大很多倍。静态链接的可执行文件可以单独运行，动态链接的可执行文件必须同时存在动态库才能运行
* 也可以省略 add_library 的第二个参数，省略则默认构建静态库，如果指定了全局变量 `BUILD_SHARED_LIBS` 为 `ON` 则默认构建动态库

## 流程控制

* 对于布尔值
  * true 可以是：1、ON、YES、Y、true、非零数
  * false 可以是：0、OFF、NO、N、false、IGNORE、NOTFOUNT、空字符串、以 -NOTFOUND 后缀结尾
* 如下 `CMakeLists.txt` 将构建动态库

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-01 LANGUAGES CXX)

set(USE_LIBRARY ON)
message(STATUS "compile into library? ${USE_LIBRARY}")
set(BUILD_SHARED_LIBS ON)
list(APPEND _sources person.h person.cpp)

if(USE_LIBRARY)
  add_library(person ${_sources})
  add_executable(hello-world main.cpp)
  target_link_libraries(hello-world person)
else(USE_LIBRARY)
  add_executable(hello-world main.cpp ${_sources})
endif(USE_LIBRARY)
```

* else 和 endif 的条件语句也可以省略

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-01 LANGUAGES CXX)

set(USE_LIBRARY ON)
message(STATUS "compile into library? ${USE_LIBRARY}")
set(BUILD_SHARED_LIBS ON)
list(APPEND _sources person.h person.cpp)

if(USE_LIBRARY)
  add_library(person ${_sources})
  add_executable(hello-world main.cpp)
  target_link_libraries(hello-world person)
else()
  add_executable(hello-world main.cpp ${_sources})
endif()
```

* `message` 函数会在生成配置时打印信息，其中 `STATUS` 表示打印等级，打印等级还可以是 `WARNING`、`AUTHOR_WARNING`、`SEND_ERROR`、`FATAL_ERROR`

```
cd build
cmake ..

-- The CXX compiler identification is GNU 7.5.0
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- compile into library? ON
-- Configuring done
-- Generating done
-- Build files have been written to: /home/devops/share/build
```

* 除了 if，还有 foreach 循环，比如对以下 `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-01 LANGUAGES CXX)

list(APPEND _sources "person.h" "person.cpp" "main.cpp")

foreach(_source IN LISTS _sources)
  set_source_files_properties(${_source} PROPERTIES COMPILE_FLAGS -O2)
  message(STATUS "Append ${_source}")
endforeach()

foreach(_source ${_sources})
  get_source_file_property(_flags ${_source} COMPILE_FLAGS)
  message(STATUS "${_source} has extra COMPILE_FLAGS: ${_flags}")
endforeach()

foreach(_s "hello" ", " "world" "!")
  message(STATUS "_s= ${_s}")
endforeach()

foreach(_i RANGE 4)
  message(STATUS "01234= ${_i}")
endforeach()

foreach(_i RANGE 1 9 2)
  message(STATUS "13579= ${_i}")
endforeach()
```

* 将打印

```
cmake ..

-- Append person.h
-- Append person.cpp
-- Append main.cpp
-- person.h has extra COMPILE_FLAGS: -O2
-- person.cpp has extra COMPILE_FLAGS: -O2
-- main.cpp has extra COMPILE_FLAGS: -O2
-- _s= hello
-- _s= , 
-- _s= world
-- _s= !
-- 01234= 0
-- 01234= 1
-- 01234= 2
-- 01234= 3
-- 01234= 4
-- 13579= 1
-- 13579= 3
-- 13579= 5
-- 13579= 7
-- 13579= 9
```

* 使用 `option` 可以设置变量默认值，并允许从外部修改变量

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-01 LANGUAGES CXX)

option(USE_LIBRARY "compile into library" ON)
set(BUILD_SHARED_LIBS ON)
list(APPEND _sources person.h person.cpp)

if(USE_LIBRARY)
  add_library(person ${_sources})
  add_executable(hello-world main.cpp)
  target_link_libraries(hello-world person)
else()
  add_executable(hello-world main.cpp ${_sources})
endif()
```

* 生成配置时通过 -D 选项即可修改 `option` 设置的变量

```
cd build
cmake -D USE_LIBRARY=OFF ..
```

* `BUILD_SHARED_LIBS` 依赖于 `USE_LIBRARY`，`USE_LIBRARY` 为 `OFF` 时 `BUILD_SHARED_LIBS` 不会被使用，因此可以通过 `cmake_dependent_option` 指定 `USE_LIBRARY` 为 `ON` 时 `BUILD_SHARED_LIBS` 的默认值

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-01 LANGUAGES CXX)

option(USE_LIBRARY "compile into library" ON)

include(CMakeDependentOption)
cmake_dependent_option(
  BUILD_SHARED_LIBS "Compile sources into a shared library" ON
  "USE_LIBRARY" ON
)

list(APPEND _sources person.h person.cpp)

if(USE_LIBRARY)
  add_library(person ${_sources})
  add_executable(hello-world main.cpp)
  target_link_libraries(hello-world person)
else()
  add_executable(hello-world main.cpp ${_sources})
endif()
```

* `cmake_dependent_option` 指定的变量同样可以被外部修改

```
cmake -D USE_LIBRARY=ON -D BUILD_SHARED_LIBS=OFF ..
```

* 通过 `cmake --help-module 模块名` 可以查看该模块的帮助手册

```
cmake --help-module CMakeDependentOption

CMakeDependentOption
--------------------

Macro to provide an option dependent on other options.

This macro presents an option to the user only if a set of other
conditions are true.  When the option is not presented a default value
is used, but any value set by the user is preserved for when the
option is presented again.  Example invocation:

::

 CMAKE_DEPENDENT_OPTION(USE_FOO "Use Foo" ON
                        "USE_BAR;NOT USE_ZOT" OFF)

If USE_BAR is true and USE_ZOT is false, this provides an option
called USE_FOO that defaults to ON.  Otherwise, it sets USE_FOO to
OFF.  If the status of USE_BAR or USE_ZOT ever changes, any value for
the USE_FOO option is saved so that when the option is re-enabled it
retains its old value.
```

## 全局变量

* 通过 `cmake --help-variable 变量名` 可以查看该全局变量的帮助手册

```
cmake --help-variable BUILD_SHARED_LIBS

BUILD_SHARED_LIBS
-----------------

Global flag to cause ``add_library()`` to create shared libraries if on.

If present and true, this will cause all libraries to be built shared
unless the library was explicitly added as a static library.  This
variable is often added to projects as an ``option()`` so that each user
of a project can decide if they want to build the project using shared or
static libraries.
```

* 通过 `cmake --help-variable-list` 可以列出所有全局变量，通过 `cmake --system-information information.txt` 可以查看全局变量的默认值，常见全局变量及其默认值如下

```
CMAKE_CXX_COMPILER_LOADED "1"
CMAKE_CXX_COMPILER_ID "GNU"
CMAKE_CXX_COMPILER_VERSION "7.5.0"
CMAKE_COMPILER_IS_GNUCXX "1"
CMAKE_COMPILER_IS_GNUCC "1"
CMAKE_BUILD_TYPE ""
```

* 通过全局变量检测编译器版本

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(recipe-01 LANGUAGES C CXX)

message(STATUS "Is the C++ compiler loaded? ${CMAKE_CXX_COMPILER_LOADED}")
if(CMAKE_CXX_COMPILER_LOADED)
  message(STATUS "The C++ compiler ID is: ${CMAKE_CXX_COMPILER_ID}")
  message(STATUS "Is the C++ from GNU? ${CMAKE_COMPILER_IS_GNUCXX}")
  message(STATUS "The C++ compiler version is: ${CMAKE_CXX_COMPILER_VERSION}")
endif()

message(STATUS "Is the C compiler loaded? ${CMAKE_C_COMPILER_LOADED}")
if(CMAKE_C_COMPILER_LOADED)
  message(STATUS "The C compiler ID is: ${CMAKE_C_COMPILER_ID}")
  message(STATUS "Is the C from GNU? ${CMAKE_COMPILER_IS_GNUCC}")
  message(STATUS "The C compiler version is: ${CMAKE_C_COMPILER_VERSION}")
endif()
```

* 生成配置时将打印编译器版本信息

```
cmake ..

-- The C compiler identification is GNU 7.5.0
-- The CXX compiler identification is GNU 7.5.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Is the C++ compiler loaded? 1
-- The C++ compiler ID is: GNU
-- Is the C++ from GNU? 1
-- The C++ compiler version is: 7.5.0
-- Is the C compiler loaded? 1
-- The C compiler ID is: GNU
-- Is the C from GNU? 1
-- The C compiler version is: 7.5.0
-- Configuring done
-- Generating done
-- Build files have been written to: /home/devops/share/build
```

* `CMAKE_BUILD_TYPE` 变量表示构建类型，其值可以为
  * Debug：无优化，带 debug symbol
  * Release：优化，无 debug symbol
  * RelWithDebInfo：不太激进的优化，带 debug symbol
  * MinSizeRel：不增加代码大小的优化
* 下面将其值设为 `Release`，并设为 CACHE 变量，以允许之后通过 cache 修改

```cmake
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C flags, Debug configuration: ${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "C flags, Release configuration: ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "C flags, Release configuration with Debug info: ${CMAKE_C_FLAGS_RELWITHDEBINFO}")
message(STATUS "C flags, minimal Release configuration: ${CMAKE_C_FLAGS_MINSIZEREL}")
message(STATUS "C++ flags, Debug configuration: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "C++ flags, Release configuration: ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "C++ flags, Release configuration with Debug info: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
message(STATUS "C++ flags, minimal Release configuration: ${CMAKE_CXX_FLAGS_MINSIZEREL}")
```

* 打印信息如下

```
-- Build type: Release
-- C flags, Debug configuration: -g
-- C flags, Release configuration: -O3 -DNDEBUG
-- C flags, Release configuration with Debug info: -O2 -g -DNDEBUG
-- C flags, minimal Release configuration: -Os -DNDEBUG
-- C++ flags, Debug configuration: -g
-- C++ flags, Release configuration: -O3 -DNDEBUG
-- C++ flags, Release configuration with Debug info: -O2 -g -DNDEBUG
-- C++ flags, minimal Release configuration: -Os -DNDEBUG
```

* 为 Release 和 Debug 分别创建构建目录，使用 `cmake -D CMAKE_BUILD_TYPE=Release/Debug ..`，即可共用一套配置编译 Release 和 Debug 版本

## 设置编译器选项

* 如下全局变量指定了构建类型的默认选项，修改 `CMAKE_CXX_FLAGS` 即可修改编译器选项，如 `cmake -D CMAKE_CXX_FLAGS="-fno-exceptions -fno-rtti"..`，该选项的功能是禁用异常和 RTTI

```
CMAKE_CXX_FLAGS == ""
CMAKE_CXX_FLAGS_DEBUG == "-g"
CMAKE_CXX_FLAGS_MINSIZEREL == "-Os -DNDEBUG"
CMAKE_CXX_FLAGS_RELEASE == "-O3 -DNDEBUG"
CMAKE_CXX_FLAGS_RELWITHDEBINFO == "-O2 -g -DNDEBUG"
CMAKE_CXX_FLAGS ""
CMAKE_CXX_FLAGS_DEBUG "-g"
CMAKE_CXX_FLAGS_DEBUG_INIT "-g"
CMAKE_CXX_FLAGS_INIT ""
CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG"
CMAKE_CXX_FLAGS_MINSIZEREL_INIT "-Os -DNDEBUG"
CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG"
CMAKE_CXX_FLAGS_RELEASE_INIT "-O3 -DNDEBUG"
CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG"
CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O2 -g -DNDEBUG"
```

* 另一种方式是在 `CMakeLists.txt` 中更细粒度地为特定目标指定编译器选项

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-01 LANGUAGES CXX)

message("C++ compiler flags: ${CMAKE_CXX_FLAGS}")
list(APPEND flags "-fPIC" "-Wall")
if(NOT WIN32)
  list(APPEND flags "-Wextra" "-Wpedantic")
endif()
add_library(person STATIC person.h person.cpp)
target_compile_options(person PRIVATE ${flags})
add_executable(hello-world main.cpp)
target_compile_options(hello-world PRIVATE "-fPIC")
target_link_libraries(hello-world person)
```

* `target_compile_options` 中可以指定编译器选项的可见性
  * PRIVATE：选项仅被指定目标使用
  * INTERFACE：选项仅被指定目标和依赖目标使用
  * PUBLIC：选项被指定目标和库使用

* 如果默认添加全局的编译和链接选项，则使用 `add_compile_options` 和 `add_link_options`，GCC 优化选项见 https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html

```cmake
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_AR "gcc-ar" CACHE STRING "GCC AR" FORCE)
  set(CMAKE_AR "gcc-nm" CACHE STRING "GCC NM" FORCE)
  set(CMAKE_RANLIB "gcc-ranlib" CACHE STRING "GCC random library" FORCE)
  add_compile_options(
    -m64
    -funsigned-char
    -faligned-new
    -ffunction-sections # remove unreferenced functions
    -fdata-sections     # remove unreferenced data
    -fno-builtin        # GNU compiler built-in code is not optimal
    -fpic
    -fvisibility-inlines-hidden
    -fno-rtti
    -fconstexpr-depth=1024
    -fconstexpr-cache-depth=8
    -pthread
    -Wall
    -Wextra
    -Wpointer-arith
    -Wcast-qual
    -Wno-unused-parameter
    -Wno-deprecated-declarations
  )

  add_link_options(
    -fuse-ld=gold # use gold linker
    -static
    -pthread
    -static-libstdc++ # g++ force link libstdc++ statically
    -static-libgcc    # g++ force link libgcc statically
    -Wl,--gc-sections # remove unreferenced symbols when export
  )

  if(DEFINED CPU_ARCH)
    add_compile_options(-march=${CPU_ARCH})
  endif()

  set(CMAKE_CXX_FLAGS_DEBUG
    "-D_DEBUG -Og -ggdb3 -fstack-check -fstack-protector-all -fno-inline -Wl,-export-dynamic"
    CACHE STRING "Debug C++ compiler flags." FORCE
  )
  set(CMAKE_CXX_FLAGS_RELEASE
    "-DNDEBUG -s -O3 -ffast-math -fno-finite-math-only -funroll-loops -flto=auto -fuse-linker-plugin"
    CACHE STRING "Release C++ compiler flags." FORCE
  )
  string(REPLACE "." "" COMPILER_VERSION ${CMAKE_CXX_COMPILER_VERSION})
  set(COMPILER_VERSION "gcc${COMPILER_VERSION}"
      CACHE STRING "Compiler version." FORCE
  )

  link_libraries(
    m # libm for <cmath>
    pthread
    stdc++
  )

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  add_compile_options(
    /Gd
    /MP
    /W3
    /EHa
    /Oy-
    /FC
    /Zc:inline
    /Zc:wchar_t
    /Zc:forScope
    /Zc:__cplusplus
    /diagnostics:column
    /fp:precise
    /constexpr:depth10240
    # Warning
    /wd26451
    /wd4305
    # Macro
    /DWin32
    /DUNICODE
    /D_UNICODE
    /D_CRT_SECURE_NO_WARNINGS
    /DWINSOCK_DEPRECATED_NO_WARNINGS
    /DSILENCE_ALL_CXX17_DEPRECATION_WARNINGS
  )

  add_link_options(
    /NOLOGO
    /LTCG
    /OPT:REF
  )

  string(REGEX REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS_DEBUG
    "/D_DEBUG /MDd /Zi /Od /Ob0 /GS"
    CACHE STRING "Debug C++ compiler flags." FORCE
  )
  set(CMAKE_CXX_FLAGS_RELEASE
    "/DNDEBUG /MD /O2 /Ob2 /Oi /GT /GL /Gm- /GS-"
    CACHE STRING "Release C++ compiler flags." FORCE
  )
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/INCREMENTAL:NO /LTCG"
    CACHE STRING "Release C++ linker flags." FORCE
  )
  set(COMPILER_VERSION "vc${MSVC_TOOLSET_VERSION}"
      CACHE STRING "Compiler version." FORCE
  )

  link_libraries(
    wsock32
    ws2_32
  )

else()
  message(FATAL_ERROR "Compiler not supported")
endif()

if(ENABLE_SIMD)
  add_definitions(-DAS_ENABLE_SIMD)
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(
      -mavx
      -mavx2
      -mavx512f
      -mavx512pf
      -mavx512er
      -mavx512cd
    )
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_options(/arch:AVX512)
  endif()
endif()
```

## 设置 C++ 标准

* `main.cpp` 内容如下，使用 C++17 的折叠表达式

```cpp
#include <iostream>

int main() {
  auto f = [](auto &&...ts) { ((std::cout << ts), ...); };
  f('h', 'e', 'l', 'l', 'o');
}
```

* `CMakeLists.txt` 如下

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-01 LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_executable(hello-world main.cpp)
set_target_properties(hello-world
  PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE 1
)
```

* 也可以全局设置语言标准

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hello-world main.cpp)
```

* 一般默认设置标准并打印编译器信息即可

```cmake
cmake_policy(SET CMP0074 NEW)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  CHECK_CXX_COMPILER_FLAG("/std:c++17" COMPILER_SUPPORTS_CXX17)
endif()
set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ version selection" FORCE)
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "Support C++ standard" FORCE) # ensure support standard
set(CMAKE_CXX_EXTENSIONS OFF CACHE BOOL "Remove C++ extensions" FORCE)
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "CXX Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "CXX Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "CXX Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CXX Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CXX Compiler Arch ID: ${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}")
```

## 检测操作系统

* 通过 `CMKAE_SYSTEM_NAME` 获取操作系统名称，此外还可以通过 `CMAKE_SYSTEM_VERSION` 获取操作系统版本

```
cmake --help-variable CMAKE_SYSTEM_NAME

CMAKE_SYSTEM_NAME
-----------------

The name of the operating system for which CMake is to build.
See the ``CMAKE_SYSTEM_VERSION`` variable for the OS version.

System Name for Host Builds
^^^^^^^^^^^^^^^^^^^^^^^^^^^

``CMAKE_SYSTEM_NAME`` is by default set to the same value as the
``CMAKE_HOST_SYSTEM_NAME`` variable so that the build
targets the host system.

System Name for Cross Compiling
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``CMAKE_SYSTEM_NAME`` may be set explicitly when first configuring a new build
tree in order to enable :ref:`cross compiling <Cross Compiling Toolchain>`.
In this case the ``CMAKE_SYSTEM_VERSION`` variable must also be
set explicitly.
```

* 打印操作系统名称

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-01 LANGUAGES CXX)

if(CMKAE_SYSTEM_NAME STREQUAL "Linux")
  message(STATUS "Configuring Linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  message(STATUS "Configuring Mac OS")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  message(STATUS "Configuring Windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "AIX")
  message(STATUS "Configuring IBM AIX")
else()
  message(STATUS "Configuring ${CMAKE_SYSTEM_NAME}")
endif()
```

## 平台相关源码

* `main.cpp` 内容如下

```cpp
#include <iostream>
#include <string>

std::string SystemName() {
#ifdef IS_LINUX
  return "Linux";
#elif IS_Windows
  return "Windows";
#elif IS_MACOS
  return "Mac OS";
#else
  return "Unknown";
#endif
}

int main() { std::cout << SystemName() << std::endl; }
```

* 在 `CMakeLists.txt` 中为不同操作系统在预处理阶段定义宏

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-01 LANGUAGES CXX)

add_executable(hello-world main.cpp)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_compile_definitions(hello-world PUBLIC "IS_LINUX")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  target_compile_definitions(hello-world PUBLIC "IS_Windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  target_compile_definitions(hello-world PUBLIC "IS_MACOS")
endif()
```

## 编译器相关源码

* `main.cpp` 内容如下

```cpp
#include <iostream>
#include <string>

std::string CompilerName() {
#ifdef IS_MSVC_CXX_COMPILER
  return "MSVC";
#elif IS_GNU_CXX_COMPILER
  return "GNU";
#elif IS_APPLE_CLANG_CXX_COMPILER
  return "AppleClang";
#else
  return "Unknown";
#endif
}

int main() {
  std::cout << CompilerName() << std::endl;
#ifdef COMPILER_NAME
  std::cout << COMPILER_NAME << std::endl;
#endif
}
```

* 通过 `CMAKE_CXX_COMPILER_ID` 获取编译器名称

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-01 LANGUAGES CXX)

add_executable(hello-world main.cpp)
target_compile_definitions(hello-world PUBLIC "COMPILER_NAME=\"${CMAKE_CXX_COMPILER_ID}\"")

if(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
  target_compile_definitions(hello-world PUBLIC "IS_MSVC_CXX_COMPILER")
elseif(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  target_compile_definitions(hello-world PUBLIC "IS_GNU_CXX_COMPILER")
elseif(CMAKE_CXX_COMPILER_ID MATCHES AppleClang)
  target_compile_definitions(hello-world PUBLIC "IS_APPLE_CLANG_CXX_COMPILER")
endif()
```

## 体系结构相关源码

* `main.cpp` 内容如下

```cpp
#include <iostream>
#include <string>

std::string Architecture() {
#ifdef IS_64_BIT_ARCH
  return "64 bit";
#elif IS_32_BIT_ARCH
  return "32 bit";
#else
  return "Unknown";
#endif
}

int main() {
  std::cout << Architecture() << std::endl;
#ifdef ARCHITECTURE
  std::cout << ARCHITECTURE << std::endl;
#endif
}
```

* 通过 `CMAKE_HOST_SYSTEM_PROCESSOR` 或 `CMAKE_SYSTEM_PROCESSOR`，区别是后者在交叉编译时为目标架构的 CPU 名称）获取处理器类型。通过检测 `CMAKE_SIZEOF_VOID_P` 的大小来检测是 32 位还是 64 位

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-01 LANGUAGES CXX)

add_executable(hello-world main.cpp)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  target_compile_definitions(hello-world PUBLIC "IS_64_BIT_ARCH")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
  target_compile_definitions(hello-world PUBLIC "IS_32_BIT_ARCH")
endif()

if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES AMD64)
  message(STATUS "Detecting AMD64 architecture")
elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES x86_64)
  message(STATUS "Detecting x86_64 architecture")
elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES i386)
  message(STATUS "Detecting i386 architecture")
elseif(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES i686)
  message(STATUS "Detecting i686 architecture")
else()
  message(STATUS "Detecting Unknown architecture")
endif()

target_compile_definitions(hello-world PUBLIC "ARCHITECTURE=\"${CMAKE_HOST_SYSTEM_PROCESSOR}\"")
```

## 处理器指令集相关源码

* `main.cpp` 内容如下

```cpp
#include <iostream>

#include "config.h"

int main() {
  std::cout << "Number of logical cores: " << NUMBER_OF_LOGICAL_CORES
            << std::endl;
  std::cout << "Number of physical cores: " << NUMBER_OF_PHYSICAL_CORES
            << std::endl;

  std::cout << "Total virtual memory in megabytes: " << TOTAL_VIRTUAL_MEMORY
            << std::endl;
  std::cout << "Available virtual memory in megabytes: "
            << AVAILABLE_VIRTUAL_MEMORY << std::endl;
  std::cout << "Total physical memory in megabytes: " << TOTAL_PHYSICAL_MEMORY
            << std::endl;
  std::cout << "Available physical memory in megabytes: "
            << AVAILABLE_PHYSICAL_MEMORY << std::endl;

  std::cout << "Processor is 64Bit: " << IS_64BIT << std::endl;
  std::cout << "Processor has floating point unit: " << HAS_FPU << std::endl;
  std::cout << "Processor supports MMX instructions: " << HAS_MMX << std::endl;
  std::cout << "Processor supports Ext. MMX instructions: " << HAS_MMX_PLUS
            << std::endl;
  std::cout << "Processor supports SSE instructions: " << HAS_SSE << std::endl;
  std::cout << "Processor supports SSE2 instructions: " << HAS_SSE2
            << std::endl;
  std::cout << "Processor supports SSE FP instructions: " << HAS_SSE_FP
            << std::endl;
  std::cout << "Processor supports SSE MMX instructions: " << HAS_SSE_MMX
            << std::endl;
  std::cout << "Processor supports 3DNow instructions: " << HAS_AMD_3DNOW
            << std::endl;
  std::cout << "Processor supports 3DNow+ instructions: " << HAS_AMD_3DNOW_PLUS
            << std::endl;
  std::cout << "IA64 processor emulating x86 : " << HAS_IA64 << std::endl;

  std::cout << "OS name: " << OS_NAME << std::endl;
  std::cout << "OS sub-type: " << OS_RELEASE << std::endl;
  std::cout << "OS build ID: " << OS_VERSION << std::endl;
  std::cout << "OS platform: " << OS_PLATFORM << std::endl;
}
```

* `config.h.in` 内容如下

```
#pragma once
  
#define NUMBER_OF_LOGICAL_CORES @_NUMBER_OF_LOGICAL_CORES@
#define NUMBER_OF_PHYSICAL_CORES @_NUMBER_OF_PHYSICAL_CORES@
#define TOTAL_VIRTUAL_MEMORY @_TOTAL_VIRTUAL_MEMORY@
#define AVAILABLE_VIRTUAL_MEMORY @_AVAILABLE_VIRTUAL_MEMORY@
#define TOTAL_PHYSICAL_MEMORY @_TOTAL_PHYSICAL_MEMORY@
#define AVAILABLE_PHYSICAL_MEMORY @_AVAILABLE_PHYSICAL_MEMORY@
#define IS_64BIT @_IS_64BIT@
#define HAS_FPU @_HAS_FPU@
#define HAS_MMX @_HAS_MMX@
#define HAS_MMX_PLUS @_HAS_MMX_PLUS@
#define HAS_SSE @_HAS_SSE@
#define HAS_SSE2 @_HAS_SSE2@
#define HAS_SSE_FP @_HAS_SSE_FP@
#define HAS_SSE_MMX @_HAS_SSE_MMX@
#define HAS_AMD_3DNOW @_HAS_AMD_3DNOW@
#define HAS_AMD_3DNOW_PLUS @_HAS_AMD_3DNOW_PLUS@
#define HAS_IA64 @_HAS_IA64@
#define OS_NAME "@_OS_NAME@"
#define OS_RELEASE "@_OS_RELEASE@"
#define OS_VERSION "@_OS_VERSION@"
#define OS_PLATFORM "@_OS_PLATFORM@"
```

* 通过 `config.h.in` 在 `build` 目录下生成 `config.h`，通过 `PROJECT_BINARY_DIR` 获取 `build` 目录的绝对路径，通过 [cmake_host_system_information](https://cmake.org/cmake/help/latest/command/cmake_host_system_information.html) 查询系统信息并保存到指定变量中，通过 [configure_file](https://cmake.org/cmake/help/latest/command/configure_file.html) 替代原文件中的 `@XXX@` 为 `CMakeLists.txt` 中设置的 `XXX` 变量的值来生成新文件

```cmake
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(recipe-01 LANGUAGES CXX)

add_executable(hello-world "")
target_sources(hello-world PRIVATE main.cpp)
target_include_directories(hello-world PRIVATE ${PROJECT_BINARY_DIR})

foreach(key
  IN ITEMS
    NUMBER_OF_LOGICAL_CORES
    NUMBER_OF_PHYSICAL_CORES
    TOTAL_VIRTUAL_MEMORY
    AVAILABLE_VIRTUAL_MEMORY
    TOTAL_PHYSICAL_MEMORY
    AVAILABLE_PHYSICAL_MEMORY
    IS_64BIT
    HAS_FPU
    HAS_MMX
    HAS_MMX_PLUS
    HAS_SSE
    HAS_SSE2
    HAS_SSE_FP
    HAS_SSE_MMX
    HAS_AMD_3DNOW
    HAS_AMD_3DNOW_PLUS
    HAS_IA64
    OS_NAME
    OS_RELEASE
    OS_VERSION
    OS_PLATFORM
  )
  cmake_host_system_information(RESULT _${key} QUERY ${key})
endforeach()

configure_file(config.h.in config.h @ONLY)
```

* Linux 下生成的 `config.h` 内容如下

```cpp
#pragma once

#define NUMBER_OF_LOGICAL_CORES 2
#define NUMBER_OF_PHYSICAL_CORES 2
#define TOTAL_VIRTUAL_MEMORY 2047
#define AVAILABLE_VIRTUAL_MEMORY 2047
#define TOTAL_PHYSICAL_MEMORY 1970
#define AVAILABLE_PHYSICAL_MEMORY 1578
#define IS_64BIT 1
#define HAS_FPU 1
#define HAS_MMX 1
#define HAS_MMX_PLUS 0
#define HAS_SSE 1
#define HAS_SSE2 1
#define HAS_SSE_FP 0
#define HAS_SSE_MMX 0
#define HAS_AMD_3DNOW 0
#define HAS_AMD_3DNOW_PLUS 0
#define HAS_IA64 0
#define OS_NAME "Linux"
#define OS_RELEASE "4.15.0-124-generic"
#define OS_VERSION "#127-Ubuntu SMP Fri Nov 6 10:54:43 UTC 2020"
#define OS_PLATFORM "x86_64"
```

* Windows 下生成的 `config.h` 内容如下

```cpp
#pragma once

#define NUMBER_OF_LOGICAL_CORES 8
#define NUMBER_OF_PHYSICAL_CORES 4
#define TOTAL_VIRTUAL_MEMORY 15404
#define AVAILABLE_VIRTUAL_MEMORY 2235
#define TOTAL_PHYSICAL_MEMORY 8113
#define AVAILABLE_PHYSICAL_MEMORY 1836
#define IS_64BIT 1
#define HAS_FPU 1
#define HAS_MMX 1
#define HAS_MMX_PLUS 0
#define HAS_SSE 1
#define HAS_SSE2 1
#define HAS_SSE_FP 0
#define HAS_SSE_MMX 0
#define HAS_AMD_3DNOW 0
#define HAS_AMD_3DNOW_PLUS 0
#define HAS_IA64 0
#define OS_NAME "Windows"
#define OS_RELEASE " Professional"
#define OS_VERSION " (Build 18363)"
#define OS_PLATFORM "AMD64"
```

* Mac OS 下生成的 `config.h` 内容如下

```cpp
#pragma once

#define NUMBER_OF_LOGICAL_CORES 8
#define NUMBER_OF_PHYSICAL_CORES 4
#define TOTAL_VIRTUAL_MEMORY 5120
#define AVAILABLE_VIRTUAL_MEMORY 1408
#define TOTAL_PHYSICAL_MEMORY 8192
#define AVAILABLE_PHYSICAL_MEMORY 1801
#define IS_64BIT 1
#define HAS_FPU 0
#define HAS_MMX 0
#define HAS_MMX_PLUS 0
#define HAS_SSE 0
#define HAS_SSE2 0
#define HAS_SSE_FP 0
#define HAS_SSE_MMX 0
#define HAS_AMD_3DNOW 0
#define HAS_AMD_3DNOW_PLUS 0
#define HAS_IA64 0
#define OS_NAME "Mac OS X"
#define OS_RELEASE "10.15.7"
#define OS_VERSION "19H2"
#define OS_PLATFORM "x86_64"
```
