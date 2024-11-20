---
layout: post
title: Docker Compose
date: 2024-11-19
author: ni
catalog: true
tags:
  - docker

---

## 前言

​	实习项目使用docker Compose + 云效进行部署，对服务器系统运维了解的第一步，后续将进一步学习k8s，再学习使用Opsdev，掌握多种技术，了解特点灵活使用

## 产生背景

- 使用Docker时步骤
  - 1. 定义`Dockerfile`文件
  - 2. 使用`docker build`、`docker run `等命令操作容器
- 但是当使用`微服务架构`的系统时，会有多个实例需要部署，如果每个都手动启停，那就会出现**效率低下**，**维护难**的问题
- <font color=#FF0000 > 为了提高效率，需要一种工具，能够对多个容器进行编排和部署的工具</font>
- `Docker Compose` 就可以解决上述问题

## 作用

- `Docker Compose` 可以通过YMAL文件来配置服务，然后通过使用`Docker Compose` 命令，创建和启动、编排配置的服务
- `compose` 可以在任何工作环境中使用，生产环境，开发环境，持续集成等场景

## 1 Docker Compose 模版文件

​	

```yaml
version: "3.7"
services:
​
  redis:
    image: redis:alpine
    ports:
      - "6379:6379"
    networks:
      - frontend
    deploy:
      # 指定运行容器的数量
      replicas: 2
      update_config:
        # 一次性更新的容器数量
        parallelism: 2
        # 更新一组容器之间的等待时间
        delay: 10s
      restart_policy:
        condition: on-failure
​
  db:
    image: postgres:9.4
    volumes:
      - db-data:/var/lib/postgresql/data
    networks:
      - backend
    deploy:
      placement:
        constraints: [node.role == manager]
​
  vote:
    image: dockersamples/examplevotingapp_vote:before
    ports:
      - 5000:80
    networks:
      - frontend
    depends_on:
      - redis
    deploy:
      replicas: 2
      update_config:
        # 一次性更新的容器数量
        parallelism: 2
      restart_policy:
        condition: on-failure
​
  result:
    image: dockersamples/examplevotingapp_result:before
    ports:
      - 5001:80
    networks:
      - backend
    depends_on:
      - db
    deploy:
      replicas: 1
      update_config:
        # 一次性更新的容器数量
        parallelism: 2
        # 更新一组容器之间的等待时间
        delay: 10s
      restart_policy:
        condition: on-failure
​
​
networks:
  frontend:
  backend:
​
volumes:
  db-data: 
```

## 1.1 项目级配置

-	version：指定 docker-compose.yml 文件的写法格式

  -	如 3.7 / 3 / ...

-	volumes / 卷标设置与查看

  ```yaml
  使用卷标模式进行挂载
  ​
  volumes:
    db-data:	
  ```

- 挂载方式对比：

  - 绝对路径：直接挂在到本地，直观但需要管理本地路径
  - 卷标：简介，但不知道数据存在什么位置

- networks / 网络定义 （实现网络隔离）

  ```
  networks:
    frontend:
    backend:
  
  docker network create --subnet=172.158.0.0/16 frontend
  ```

## 1.2 服务级配置

- replicas：指定运行容器的数量
- update_config：
  - parpallelism：一次性更新的容器数量
  - delay：更新一组容器之间的等待时间
- mode：replicated 如果服务是replicated（默认），需要指定运行的容器数量
- restart_policy 重启策略

> 配置容器的重新启动，代替 restart

> - condition : 值可以为 none 、on-failure 以及 any(默认)
> - delay : 尝试重启的等待时间，默认为 0
> - max_attempts : 在放弃之前尝试重新启动容器次数（默认：从不放弃）。如果重新启动在配置中没有成功  window，则此尝试不计入配置max_attempts 值。例如，如果 max_attempts 值为  2，并且第一次尝试重新启动失败，则可能会尝试重新启动两次以上。
> - windows : 在决定重新启动是否成功之前的等时间，指定为持续时间（默认值：立即决定）

## 1.3 Docker Compose 架构与原理

- #### 1.3.1 总体架构

  <p>
  <img src="https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/docker-compose/1173617-20231024174108363-1026332590.png" alt="注解一" title="注解一" />
  </p>

​		

- #### 1.3.2 Docker Compose 核心概念

  - Docker Compose将所管理的容器分为三层，这三个概念均为Compose抽象的数据类型，其中 project 会包含service以及container：

    - 项目/工程（project）
    - 服务（service）
    - 容器（contaienr）

  - 项目（project）：代表用户需要完成的一个项目。

    - 通过 Docker Compose 管理的一个项目被抽象称为一个 project
      - 何为项目？Compose的一个配置文件可以解析为一个项目。
      - 即：Compose通过分析指定配置文件，得出配置文件所需完成的所有容器管理与部署操作。
      - 例如：用户在当前目录下执行docker-compose up -d，配置文件为当前目录下的配置文件docker-compose.yml，命令请求类型为up，-d为命令参数，对于配置文件中的内容，compose会将其解析为一个project。
    - project 里包括多个 service
      - 一个project拥有特定的名称，并且包含多个或一个service，同时还带有一个Docker Client。

  - service，代表配置文件中的每一项服务，何为服务？

    - 即以

      容器

      为粒度，用户需要Compose所完成的任务。

      - 比如，前面的配置文件中包含了两个service，第一个为web，第二个为redis。

    - 每个 service 定义了容器运行的镜像（或构建镜像）、网络端口、文件挂载、参数、依赖等

      - 一个service包含的内容，无非是用户对服务的定义。定义一个服务，可以为服务容器指定镜像，设定构建的Dockerfile，可以为其指定link的其他容器，还可以为其指定端口的映射等。

    - 每个 service 可包括同一个镜像的多个容器实例。

  - 容器（container）

    <p>
    <img src="https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/docker-compose/1173617-20231024174314776-343716538.png" alt="注解一" title="注解一" />
    </p>

- #### 1.3.2 docker-compose下启动容器的过程

  ```
  docker-compose up -d 
  ```

  <p>
  <img src="https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/docker-compose/1173617-20231024174341813-125622049.png" alt="注解一" title="注解一" />
  </p>

  - 1. 首先，用户执行 docker-compose up 命令调用命令行中的启动方法
  - 1. 然后，如果当前宿主机已存在与该应用对应的容器，docker-compose  则进行行为逻辑判断。如果用户指定可以重新启动已有服务，docker-compose 就会执行 service  模块的容器重启方法，否则就直接启动已有容器。这两种操作的区别在于前者会停止旧的容器，创建并启动新的容器，并把旧容器移除掉。
  - 1. 最后，contaier 模块会调用 docker-py 客户端来执行向 docker daemon 发起创建容器的 POST 请求。

## 2. Docker Compose 安装卸载与运维使用

	#### 2.1 安装

 - 安装 `docker compose v2` | 官网

   You can download Docker Compose binaries from the [release page](https://github.com/docker/compose/releases) on this repository.
    Rename the relevant binary for your OS to docker-compose and copy it to $HOME/.docker/cli-plugins
    Or copy it into one of these folders to install it system-wide:

#### 2.2 容器管理

##### 2.2.1 构建镜像

​	docker-compose build

- 构建或者重新构建服务的镜像，但不会创建和启动容器

- 当你改变本地代码之后，先执行 docker-compose build 构建新的镜像，然后执行 docker-compose up -d 取代运行中的容器

  ```
  docker-compose build
  
  docker-compose -f docker-compose.yml build
  ```

- 构建指定的镜像

  - 如，仅web目标将需要构建映像

  ```yaml
  version: '3.2'
  
  services:
    database:
      image: mariadb
      restart: always
      volumes:
        - ./.data/sql:/var/lib/mysql
  
    web:
      build:
        dockerfile: Dockerfile-alpine
        context: ./web
      ports:
        - 8099:80
      depends_on:
        - database
  ```

##### 2.2.2 构建镜像，并部署、启动容器(up)

- docker-compose up

用于部署一个 Compose 应用。 默认情况下该命令会读取名为 docker-compose.yml 或 docker-compose.yaml 的文件。

当然用户也可以使用 -f 指定其他文件名。通常情况下，会使用 -d 参数令应用在后台启动。

```shell
docker-compose up

docker-compose -f docker-compose.yml up -d

docker-compose -f docker-compose-non-dev.yml up -d
```

##### 2.2.3 从【镜像仓库】中【拉取镜像】(pull)

- docker-compose pull

`docker compose pull` 是一个Docker Compose命令，用于从**镜像仓库**中拉取所需的Docker镜像。

它的作用是根据`docker-compose.yml`文件中定义的服务和镜像名称，从配置的镜像仓库中下载最新版本的镜像。如果本地没有需要的镜像或者需要更新已有的镜像版本时，使用docker compose pull命令可以获取最新的镜像并存储到本地。

如果要使用`docker compose pull`命令，需要在命令行中切换到包含`docker-compose.yml`文件的目录，然后执行该命令即可。代码示例如下：

```shell
cd /path/to/docker-compose
docker compose pull
```

> demo

```shell
docker compose -f docker-compose-non-dev.yml pull
```

##### 2.2.4 启动容器

- 启动所有的容器

```shell
docker-compose start
```

- 启动指定的容器，如果不指定则停止所有的容器

```shell
docker-compose start {containerName}
```

- 重启所有的容器 | docker-compose restart
  - 重启已停止的 Compose 应用。 如果用户在停止该应用后对其进行了变更，那么变更的内容不会反映在重启后的应用中
  - 这时需要重新部署应用使变更生效。

```shell
docker-compose restart
```

- 重启指定的容器

```shell
docker-compose restart web
```

##### 2.2.5 停止容器

> 停止 Compose 应用相关的所有容器，但**不会删除**它们。 被停止的应用可以很容易地通过 docker-compose restart 命令重新启动。

- 停止所有容器

```shell
docker-compose stop
```

- 停止指定容器，若不指定则停止所有的容器

```shell
docker-compose stop container_name
```

##### 2.3 删除镜像

- 删除镜像 | docker-compose rm

> 用于删除已停止的 Compose 应用；
>  它会删除容器和网络，但是不会删除卷和镜像。

```shell
docker-compose rm
```

- 删除指定的已停止容器，若不指定则删除所有已停止容器

```shell
docker-compose rm {containerName/containerId}
```

##### 2.3.1 停止、删除所有容器，移除自定义网络(down)

- 停止并删除运行中的 Compose 应用。 它会删除容器和网络，但是不会删除卷和镜像

```shell
docker-compose down
```

> demo

```shell
# 停止并移除容器 docker-compose 的所有资源 | 停止并移除docker-compose.yml中定义的所有容器、网络和数据卷
# cd /root/projects/superset
# docker-compose -f docker-compose-non-dev.yml down
```

##### 2.4 查看运行的容器

```shell
docker-compose ls
```

- docker-compose ps

> 用于列出 Compose 应用中的各个容器。 输出内容包括当前状态、容器运行的命令以及网络端口。

```shell
docker-compose ps
```

##### 2.5 查看指定容器的日志

- 查看具体容器的日志
  - -f 参数表示：**实时日志输出**

```shell
docker-compose logs -f {containerName/containerId}
```

##### 2.6 查看指定容器端口所绑定的宿主机端口

> 即 查看某个容器端口所映射的公共端口

```shell
docker-compose port [options] {containerName} {containerPort}
	--protocol=proto 指定端口协议，tcp（默认值）或者 udp
	--index=index 如果同一服务存在多个容器，指定命令对象容器的序号（默认为 1）
# 下面结果表示：将web服务的5000端口映射到了宿主机的5001端口
[root@centos01 ~]# docker-compose port web 5000
0.0.0.0:5001
```
