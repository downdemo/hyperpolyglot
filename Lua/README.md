* [Lua 文档](http://www.lua.org/docs.html)
* [Lua 在线运行](http://www.lua.org/demo.html)

## Lua 环境搭建

```sh
lua_version=5.4.7
curl -R -O https://www.lua.org/ftp/lua-${lua_version}.tar.gz
tar zxvf lua-${lua_version}.tar.gz
cd lua-${lua_version}
make all test
make install
lua -v
```

* 创建如下 `hello.lua` 文件

```lua
print("hello world")
```

* 运行

```sh
lua hello.lua
hello world
```

* 直接在代码中指定解释器

```lua
#!/usr/local/bin/lua

print("hello world")
```

* 运行

```sh
chmod a+x hello.lua
./hello.lua
hello world
```

* 通过 `-e` 选项在命令行中输入代码

```sh
lua -e "print(\"hello world\")"
hello world
```

* 获取命令行参数

```lua
-- 命令为 lua -e "sin=math.sin" script a b
arg[-3] -- "lua"
arg[-2] -- "-e"
arg[-1] -- "sin=math.sin"
arg[0] -- "script"
arg[1] -- "a"
arg[2] -- "b"
```
