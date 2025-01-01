* [Go 入门文档](http://www.topgoer.com/)
* [Go 标准库中文文档](http://ioqx.com/)
* [pkg.go.dev](https://pkg.go.dev/)

## Go 环境搭建

* 下载 [Go](https://golang.google.cn/dl/) 并[安装](https://go.dev/doc/install)

```sh
sudo rm -rf /usr/local/go
wget -c https://golang.google.cn/dl/go1.23.4.linux-amd64.tar.gz -O - | sudo tar xz -C /usr/local
export PATH=$PATH:/usr/local/go/bin
```

* 查看版本

```sh
go version
```

* 设置 [Go 模块代理](https://goproxy.cn/)

```sh
go env -w GO111MODULE=on
go env -w GOPROXY=https://goproxy.cn,direct
```

* 查看

```sh
go env
```

* 设置环境变量

```sh
export GO111MODULE=on
export GOPROXY=https://goproxy.cn
```

* 对于 Windows，在 PowerShell 中执行

```sh
$env:GO111MODULE = "on"
$env:GOPROXY = "https://goproxy.cn"
```

* 对于 GOPATH，建议 Mac 设为 `$HOME/go`，Windows 设为 `C:\Go`
* 在桌面创建文件夹 `demo`，执行 `go mod init demo-project` 初始化名为 `demo-project` 的 module
```sh
go mod init demo-project
go: creating new go.mod: module demo-project

cat go.mod
module demo-project

go 1.23.4
```

* 创建文件 `demo/main.go`，内容如下

```go
package main

import (
	"fmt"
	"runtime"
	"time"
)

const yield_times int = 10000000

func yield() {
	for i := 0; i < yield_times; i++ {
		runtime.Gosched()
	}
}

func main() {
	runtime.GOMAXPROCS(1)
	t1 := time.Now()
	go yield()
	yield()
	t2 := time.Now()
	diff := float64(t2.Sub(t1).Nanoseconds()) / (2 * float64(yield_times))
	fmt.Println("Goroutine context switching costs", diff, "ns.")
}
```

* 直接运行，可见 go 的协程切换开销约为 74 纳秒

```sh
go run main.go

goroutine context switching costs 73.72583535 ns
```

* 格式化

```sh
go fmt main.go
```

* 编译成二进制文件并运行

```sh
go build main.go
./main
```
