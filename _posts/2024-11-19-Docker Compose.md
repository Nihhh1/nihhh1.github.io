---
layout: post
title: Docker Compose
date: 2024-11-18
author: ni
catalog: true
tags:
  - docker
  - 
---

## 前言

​	实习项目使用docker Compose + 云效进行部署，对服务器系统运维了解的第一步，后续将进一步学习k8s，再学习使用Opsdev，掌握多种技术，了解特点灵活使用

## 产生背景

- 使用Docker时步骤
  - 1. 定义`Dockerfile`文件
  - 2. 使用`docker build`、`docker run `等命令操作容器
- 但是当使用`微服务架构`的系统时，会有多个实例需要部署，如果每个都手动启停，那就会出现**效率低下**，**维护**难得问题
- <font color=#FF0000 >为了提高效率，需要一种工具，能够对多个容器进行编排和部署的工具</font>
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
