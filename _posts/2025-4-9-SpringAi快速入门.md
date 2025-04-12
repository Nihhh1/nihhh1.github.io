---
layout: post
title: Spring Ai快速入门
date: 2025-4-9
author: ni
catalog: true
tags:
  - Spring Ai

---

## 前言

- 随着Ai的爆火，各类Ai衍生产品诞生，如AIot，cursor
- Spring作为一个集成最广泛，功能全面的框架当然要进行跟进

- 在Spring官网中可以查到Spring Ai相关文档 https://docs.spring.io/spring-ai/reference/api/chat/deepseek-chat.html
- 本文使用DeepSeek进行快速入门开发

## Spring Ai的快速入门

### 配置Maven

- 根据查阅Spring文档可以进行配置 https://docs.spring.io/spring-ai/reference/getting-started.html

- 首先修改Maven的setting.xml配置文件![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-09_23-26-54.png)

- 添加依赖

  ```java
  <dependencyManagement>
      <dependencies>
          <dependency>
              <groupId>org.springframework.ai</groupId>
              <artifactId>spring-ai-bom</artifactId>
              <version>1.0.0-SNAPSHOT</version>
              <type>pom</type>
              <scope>import</scope>
          </dependency>
      </dependencies>
  </dependencyManagement>
  ```

- 添加配置

  ```yaml
  spring:
    application:
      name: springAi
    ai:
      openai:
        # 可以在DeepSeek官网开放平台获取，非免费1元可买
        api-key: ******* 
        base-url: https://api.deepseek.com
        chat:
          options:
            model: deepseek-chat
  ```

  

- 编写Service代码

  ![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-09_23-43-20.png)

- 实现类

  ![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-09_23-43-36.png)

- Controller控制层

  ![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-09_23-43-47.png)

  

### 结果

- **1、chat方法直接输出结果**![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-09_23-44-01.png)
- **2、chatFlux方法流式输出**![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-09_23-45-15.png)

## 总结

- 本文完成对Ai的调用
- 当然这只是最基本的功能
- 后续有针对不同用户的聊天
- 根据上下文进行回答
- 根据关键词触发程序方法 ...
- 未完待续 ....