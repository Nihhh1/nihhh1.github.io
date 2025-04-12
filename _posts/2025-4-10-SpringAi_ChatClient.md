---
layout: post
title: Spring Ai_ChatClient
date: 2025-4-10
author: ni
catalog: true
tags:
  - Spring Ai

---

## 续

- 前面使用ChatModel接口，自动注入Spring Ai的默认实现类，来与大模型交流
- 但后续如果要实现添加默认角色，关键词调用方法，结合上下文等方法ChatModel实现不了
- 需要使用ChatClient
- 并且ChatClient可以使用**多种**大模型进行回答
- 不在局限于在配置文件进行

## 开始

### 创建ChatClient

- 在官方文档中，实现ChatClient直接使用`ChatClient.builder(ChatModel chatModel)`
- 并且传入的ChatModel，是通过Spring自动注入的![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/Snipaste_2025-04-12_18-46-44.png)
- 也就是说用的还是默认配置
- 当我们查看ChatModel接口
- 可以看到是有`OpenAiChatModel`实现类的 
- 并且有builder方法，需要提供一个`OpenAiApi`类
- 很显然我们可以在里面进行配置来实现动态配置
- 如此可以通过数据库或者Redis来修改配置，或者使用多种大模型![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/Snipaste_2025-04-12_19-01-05.png)

### 使用ChatClient

- 创建完ChatClient后开始使用
- 首先查看官方文档![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/Snipaste_2025-04-12_19-03-32.png)

- 很显然我们可以通过.user方法来传递用户问题
- .call方法来规定回答方式与之前说的.call和.stream相同
- 在.call后的方法很显然是规定返回的内容格式
- 通过文档可以看到，如果通过.content可以直接返回文本内容不需要返回多余信息

#### 系统默认角色

- 我们还可以通过在创建Client的时候设置系统默认值![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/Snipaste_2025-04-12_19-10-18.png)

- 官方文档在切换声音场景下使用了系统默认值并可以自动切换
- 我们也可以直接在system方法里写默认值，或在创建时就添加固定默认值![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/Snipaste_2025-04-12_19-14-42.png)

## 测试

- **chat方法： **我们在chat方法里设置了系统默认值，告诉他，他是一个架构师，让他来自我介绍一下![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/Snipaste_2025-04-12_19-16-24.png)

- **chatFlux方法：** 我们没有设置系统默认值并且以流的形式直接返回内容![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/Snipaste_2025-04-12_19-18-24.png)