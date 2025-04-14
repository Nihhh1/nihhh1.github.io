---
layout: post
title: Spring Ai_ToolCalling
date: 2025-4-14
author: ni
catalog: true
tags:
  - Spring Ai

---

## 前言

- 前面完成了调用AI的基本操作
- 当前学习使用SpringAi的ToolCalling使用
- 当用户提出特定问题时，可以执行方法，可以通过方法返回相关内容，也可以执行其他操作

## ToolCalling

- 该方法的使用学习非常简单

- 主要为@Tool注解和@ToolParam注解

- 官方文档给的示例

  ```java
  class DateTimeTools {
  
      @Tool(description = "Set a user alarm for the given time")
      void setAlarm(@ToolParam(description = "Time in ISO-8601 format") String time) {
          LocalDateTime alarmTime = LocalDateTime.parse(time, DateTimeFormatter.ISO_DATE_TIME);
          System.out.println("Alarm set for " + alarmTime);
      }
  
  }
  ```

- 很明显的能知道当用户提及到@Tool注解的描述时就会执行该方法

- 而

- 注释`@ToolParam`允许您提供有关工具参数的关键信息：

  - `description`：参数的描述，模型可以通过它来更好地理解如何使用参数。例如，参数应采用什么格式、允许使用哪些值等等。
  - `required`：参数是必需的还是可选的。默认情况下，所有参数都被视为必需的。

- 如果不添加Tool方法获取当前时间![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-14_16-04-37.png)

- 添加Tool方法

  ```java
  public class MyTool {
      @Tool(description = "当前时间是多少")
      String getCurrentTime(){
          return LocalDateTime.now().toString();
      }
  }
  public ...{
  return client.prompt()
                  .user(message)
                  .tools(new MyTool())
                  .system("你是一个架构师，帮助用户回答各类编程难题")
                  .advisors(memoryAdvisor)
                  .call()
                  .content();
  }
  ```

- 结果：![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-14_16-06-35.png)

- 后续可以在方法里添加多种实用的方法例如定时任务什么的
- 有了ToolCalling的支持，可以提供AI的能动性