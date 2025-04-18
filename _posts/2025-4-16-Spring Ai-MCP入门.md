---
layout: post
title: Spring Ai_MCP
date: 2025-4-16
author: ni
catalog: true
tags:
  - Spring Ai

---

## 前言

- 学习了Spring Ai的运用之后
- 引发思考，类似于Claude这类软件如何获取本地资源
- 接下来学习的MCP协议就可以解决上面的问题

## MCP

- MCP 全称为：**Model Context Protocol**，即：**模型上下文协议**，它是一种社区共建的AI 开放协议，它标准化了应用向 AI 应用提供上下文的方式。
- **对于MCP的定义可以查看Spring AI官网中的架构图**![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/java-mcp-client-architecture.jpg)
- 可以看到一个AI工具内可以集成多个MCP Server来提供服务，每个Server都有明确定义的作用
- 并且MCP把各类标准都进行统一，无论是哪种架构的LLM都可以无感切换
- 可以将MCP视为AI领域的"万能适配器"，就像蓝牙技术能够将手机无线连接到耳机、键盘、智能家居等各类设备那样，MCP构建了一个通用协议层，让不同架构的AI模型可以无缝接入数据库、传感器、API接口等多元化资源，实现即插即用的智能化协作。
- **接下来再看官网中Client Server的架构图**![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/java-mcp-server-architecture.jpg)
- 从图中可以看到，由于规则的统一，无论是什么语言开发的，获得是哪种数据，最后只要都符合MCP相关规则都可以相互通信，这使得各个服务都可以进行**拆分解耦**
- 并且可以看到Client和Server之间有两种不同的通信方式
- 根据服务来源，**MCP主要采用通信机制，本地通信时采用标准输入输出，远程通信则通过SSE进行**。
- 这两种通信方式中的消息，都采用了JSON格式进行消息传输，使MCP通信过程能够标准化，并带来了可扩展性。
- MCP服务器还内置了安全机制，允许服务器自己控制资源，不用把API密钥交给大模型。Server可以直接在本地获取，最后传到大模型内只有数据，其中过程对大模型是不可见的

## 运行过程

- 接下来可以看看MCP整个的运行过程，如下图![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/76635b004d4ed65e4839d521b390c594.png)

	1. 用户发送消息：用户通过客户端应用程序发送一条消息。
  2. 客户端传递消息：客户端将用户的消息传递给 LLM。
  3.  LLM 请求工具或数据：LLM 可能需要额外的工具或数据来处理请求，因此向客户端发出请求。
  4.  客户端向 MCP 服务器请求：客户端根据 LLM 的需求，通过 MCP 协议向 MCP 服务器发送请求。
  5.  MCP 服务器访问数据源：MCP 服务器根据请求访问相应的数据源。
  6.  数据源返回数据：数据源将所需的数据返回给 MCP 服务器。
  7.  MCP 服务器返回工具或数据：MCP 服务器将获取到的工具或数据返回给客户端。
  8.  客户端提供工具或数据给 LLM：客户端将工具或数据传递给 LLM。
  9.  LLM 返回处理结果：LLM 使用提供的工具或数据生成处理结果，并返回给客户端。
  10.  客户端显示最终结果：客户端将 LLM 的处理结果展示给用户。

- 类似于Claude，就是相当于在你的程序中跑了MCP Client和MCP Server，当需要请求本地资源时，直接通过本地的MCP Server来获取

## 结论

- 以上就是MCP的相关内容
- MCP的出现主要为了弥补大模型的短板
- 让大模型不在局限于云端的沟通
- 并对协议进行统一，使得AI技术不局限于数据类型，语言
- 更好的实现解耦拆分