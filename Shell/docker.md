* 换源

```js
{
  "registry-mirrors": ["https://docker.mirrors.ustc.edu.cn"]
}
```

* 下载镜像

```
docker pull downdemo/ubuntu:20.04
```

* 挂载目录运行

```
docker run -tid --privileged --user=root --pid host --net host --name ub20 -v C:/docker:/shared <IMAGE_ID>
```

* 进入容器后切换到后台运行

```
ctrl+p => ctrl + q // 后台运行而非 exit
docker attach 容器ID // 重新进入后台运行的程序
```

* 将容器打包成镜像

```
# docker commit -m 注释信息 -a 用户ID 容器ID 镜像名称:标签
docker commit -m "for cpp and go" -a "downdemo" 9a07c06bbe59 ubuntu:cpp_go
```

* [DockerHub](https://cloud.docker.com/repository/list) 中创建一个名为 `linux` 的仓库，上传镜像到仓库

```
docker tag <REPOSITORY:TAG> downdemo/ubuntu:20.04
docker push downdemo/ubuntu:20.04
```

* 镜像保存为文件

```
docker save -o 文件名称 要保存的镜像名称
```

* 从文件读取镜像

```
docker load --input 文件名称
```

* 回收所有 docker 空间

```
docker system prune
```