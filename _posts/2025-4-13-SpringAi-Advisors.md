---
layout: post
title: Spring Ai_Advisors
date: 2025-4-10
author: ni
catalog: true
tags:
  - Spring Ai

---

## 回顾

- 学习完ChatClient，可以使用ChatClient跟大模型进行互动
- 并可以添加一些简单功能，如system()
- 但并没有学习核心功能，例如根据历史进行回复
- 就会出现以下情况
- <img src="https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-13_14-20-10.png" style="zoom:50%;" />
- <img src="https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-13_14-20-45.png" style="zoom:50%;" />

- 接下来要为Ai添加上下文功能

## Advisor

- 下面在官方文档中介绍advisor简单的使用方法

  ```java
  var chatClient = ChatClient.builder(chatModel)
      .defaultAdvisors(
          new MessageChatMemoryAdvisor(chatMemory), // chat-memory advisor
          new QuestionAnswerAdvisor(vectorStore)    // RAG advisor
      )
      .build();
  
  String response = this.chatClient.prompt()
      // Set advisor parameters at runtime
      .advisors(advisor -> advisor.param("chat_memory_conversation_id", "678")
              .param("chat_memory_response_size", 100))
      .user(userText)
      .call()
  	.content();
  ```

- 其中`MessageChatMemoryAdvisor(chatMemory)`就是用来为请求添加上下文

- 而`QuestionAnswerAdvisor(vectorStore)`后面备注的RAG的意思就是把数据库中的数据喂给Ai，让Ai可以在这些数据中找答案

- 例如需要使用关于一个公司的内部知识

- **下面是Advisor的核心组件**![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/advisors-api-classes.jpg)

- 根据这个图可以看出，Call和Stream的Advisor分别服务于调用client需要回复结果还是流式回复

- 他们都有一个Chain，说明可以加载多个Advisor，并根据order进行排序

- 往上AroundAdvisor中存在方法around方法，通过名称可以判断该方法是在请求前后进行加强

- 类似于AOP的操作

- **下面是Advisor的工作流程**![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/advisors-flow.jpg)

- 很明显这个Advisor的执行流程就类似于AOP进行了加强

- 如此通过这种方式可以进行添加多种功能![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-13_14-49-58.png)

- 查看一下MessageChatMemoryAdvisor的源码![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-13_14-54-46.png)

- 首先就是在请求前，对请求进行增强操作

- 后续再通过AdvisorChain调用下一个Advisor或者请求模型![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-13_14-53-55.png)

- 通过这种机制SpringAI还提供了封装好的打印日志的Advisor

  ```java
  ChatResponse response = ChatClient.create(chatModel).prompt()
          .advisors(new SimpleLoggerAdvisor())
          .user("Tell me a joke?")
          .call()
          .chatResponse();
  ```

- **以上就是对Advisor的介绍**

## ChatMemory

- 在学习完Advisor后知道，可以把历史记录通过Advisor添加到请求中

- 但我们不能直接把请求用文本的方式直接加入

- 需要把消息封装为ChatMemory

- 在官网中SpringAi提供封装好的`InMemoryChatMemory`, `CassandraChatMemory`, `Neo4jChatMemory`, and `JdbcChatMemory`

- 这里我们使用InMemoryChatMemory进行查看，其余三个分别是使用Cassandra 分布式数据库，Neo4j和Jdbc并且Jdbc目前仅支持`postgresql`和`mariadb`

- 下面是`InMemoryChatMemory`的源码![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-13_15-16-14.png)

- 该类直接把消息保存到了内存中，适用于测试

- 后续可以自己实现把数据存储到redis或者Elasticsearch等库中

  ```java
  public class MyChatMemory implements ChatMemory {
      @Override
      public void add(String conversationId, List<Message> messages) {
  
      }
  
      @Override
      public List<Message> get(String conversationId, int lastN) {
          return null;
      }
  
      @Override
      public void clear(String conversationId) {
  
      }
  }
  ```

- 在`MessageChatMemoryAdvisor`的构建方法中有需要传入一个Memory和一个ConversationId和一个窗口大小（上下文大小）可以知道，在Memory中保存着用户的上下文信息，而ConversationId则是会话Id，可以自定义

- 注意Memory是存在内存中的所以是一个不变的值可以定义为final，代码如下![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-13_18-14-56.png)

## 测试

- **question1:**<img src="https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-13_14-20-10.png" style="zoom:50%;" />

- **question2:**![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-13_18-14-31.png)

- 测试结果为上下文已经可以连贯
- 后续可以自己实现接口编写
