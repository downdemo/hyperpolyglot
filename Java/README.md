* [Java 文档](https://docs.oracle.com/en/java/)
* [Java SE 文档](https://docs.oracle.com/en/java/javase/)
* [JDK 23 API](https://docs.oracle.com/en/java/javase/23/docs/api/index.html)
* [JDK 下载](https://www.oracle.com/technetwork/java/javase/downloads/index.html)
* [IDEA](http://www.jetbrains.com/idea/download/#section=windows)

## Java 环境搭建

* 下载并安装 [JDK](https://www.oracle.com/technetwork/java/javase/downloads/index.html)
* 添加 jdk 的 bin 路径到环境变量，一般默认的安装路径为 `C:\Program Files\Java\jdk-<version>\bin`
* 注销或重启使环境变量生效，运行 `java -version` 检查是否配置成功
* 新建一个内容如下的 `Hello.java` 文件后运行 `javac Hello.java` 编译出机器码文件

```java
public class Hello {
    public static void main(String[] args) {
        System.out.println("Hello, world!");
    }
}
```

* 再运行即可看到打印结果

```sh
java Hello
```

* 下载并安装 [IDEA](http://www.jetbrains.com/idea/download/#section=windows)
* 打开 IDEA，点击 `Create New Project`，选择 Java，并创建一个项目路径，如 `C:\Users\downdemo\Desktop\test`
* 右键 src 目录，选择 `New - Package`，输入 package name，如 `com.downdemo.demo`
* 右键新创建的 package，选择 `New - Java Class`，输入类名，如 `Hello`，其内容更改如下

```java
package com.downdemo.demo;

public class Hello {
    public static void main(String[] args) {
        System.out.println("Hello, world!");
    }
}
```

* 右键代码空白处，选择 `Run 'Hello.main()'` 即可看到运行结果

## Spring 环境搭建

* `File - Settings - Plugins`，搜索 `Spring Assistant`，安装插件后重新打开 IDEA
* `File - New - Project`，选择左侧的 `Spring Assistant`，选择 `Defualt` 初始化器，点击 `Next` 跳转到 `Project properties`
* `Project type` 选择 `Maven Project`，点击 `Next` 跳转到依赖设置
* 添加 `Web - Spring Web`，点击 `Next`，输入项目名称和路径后点击 `Finish`
* 右键 `src/main/java/com.example.demo` 包，添加 `Hello.java`，内容如下

```java
package com.example.demo;

import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class Hello {
    @RequestMapping("/hello")
    public String index() {
        return "hello world";
    }
}
```

* 打开 `src/main/java/com.example.demo/DemoApplication.java`，右键代码空白处，选择 `Run 'DemoApplication.main()'` 即可在控制台中看到如下结果

```
  .   ____          _            __ _ _
 /\\ / ___'_ __ _ _(_)_ __  __ _ \ \ \ \
( ( )\___ | '_ | '_| | '_ \/ _` | \ \ \ \
 \\/  ___)| |_)| | | | | || (_| |  ) ) ) )
  '  |____| .__|_| |_|_| |_\__, | / / / /
 =========|_|==============|___/=/_/_/_/
 :: Spring Boot ::        (v2.2.1.RELEASE)
```

* 打开浏览器，访问 `http://localhost:8080/hello` 即可看到 `hello world`
* 注意添加的类必须在 `src/main/java/com.example.demo` 或其子包之下，否则浏览器结果为 `Whitelabel Error Page`
* 如果想添加参数，将 `Hello.java` 改为

```java
package com.example.demo;

import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class Hello {
    @RequestMapping("/hello")
    public String index(String name) {
        return "hello " + name;
    }
}
```

* 打开浏览器，访问 `http://localhost:8080/hello?name=downdemo` 即可看到 `hello downdemo`
* 要重新打开此项目，点击 `File - New - Project from Existing Sources`，选择 `pom.xml` 文件并点击 `OK`，一直点击 `Next` 即可
* 如果运行时出现 `Web server failed to start. Port 8080 was already in use.` 执行 `netstat -ano` 找到 `0.0.0.0:8080` 的对应 ID，如 9592，执行 `taskkill /F /pid 9592` 杀死该进程即可
