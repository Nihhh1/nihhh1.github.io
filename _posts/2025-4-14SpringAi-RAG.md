---
layout: post
title: Spring Ai_RGA
date: 2025-4-14
author: ni
catalog: true
tags:
  - Spring Ai

---

## 前言

检索增强生成（RAG）是一种结合[信息检索](https://so.csdn.net/so/search?q=信息检索&spm=1001.2101.3001.7020)和生成模型的技术，用于将相关数据嵌入到Prompts中，以提高AI模型的响应准确性。

## RAG

- 使用RAG可以使用`QuestionAnswerAdvisor(vectorStore)`方法
- 需要传入一个vectorStore参数
- SpringAi提供了一个`SimpleVectorStore`让我们观察一下VectorStore如何加载数据![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-14_11-32-47.png)
- 如图可以看到，我们需要加载一个Document类

## Document

- Spring AI提供了：
  1. DocumentReader：用来读取TXT、PDF等文件内容
  2. DocumentTransformer：用来解析文件内容
  3. DocumentWriter：用来写入文件内容到向量数据库

### DocumentReader

- 实现类有：
  JsonReader：读取JSON格式的文件
  TextReader：读取txt文件
  PagePdfDocumentReader：使用Apache PdfBox读取PDF文件
  TikaDocumentReader：使用Apache Tika来读取PDF, DOC/DOCX, PPT/PPTX, and HTML等文件

- 比如使用TextReader来读取meituan.txt文件内容：

  ```java
  @Component
  public class DocumentService {
  
      @Value("classpath:deepseekDocument.txt") // This is the text document to load
      private Resource resource;
  
      public List<Document> loadText() {
          TextReader textReader = new TextReader(resource);
          textReader.getCustomMetadata().put("filename", "meituan-qa.txt");
          List<Document> documents = textReader.get();
  
          return documents;
      }
  }
  ```

  ```java
  @Autowired
      DocumentService documentService;
      @GetMapping("/load/document")
      public List<Document> document() {
          return documentService.loadText();
      }
  ```

- 结果：<img src="https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-14_12-07-37.png" style="zoom:50%;" />

- 使用DocumentReader可以把所有文本加载出来，但如果需要切分则需要使用`DocumentTransformer`



### DocumentTransformer

- Spring AI默认提供了一个`TokenTextSplitter`，我们可以基于Document来进行切分：

  ```java
   public List<Document> loadText() {
          TextReader textReader = new TextReader(resource);
          textReader.getCustomMetadata().put("filename", "deepseekDocument.txt");
          List<Document> documents = textReader.get();
  
          TokenTextSplitter tokenTextSplitter = new TokenTextSplitter();
          List<Document> list = tokenTextSplitter.apply(documents);
          return list;
      }
  ```

- 结果：<img src="https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-14_12-10-47.png" style="zoom:80%;" />

- 但是我们发现他并没有划分规则，查看`TokenTextSplitter`源码可以发现他的规则
  1. 先将文本encode为tokens
  2. 按指定的chunkSize（默认为800）对tokens进行切分，得到一个chunk
  3. 将chunk进行decode，得到原始文本
  4. 获取原始文本中最后一个’.‘、’?‘、’!‘、’\n’的位置，该位置表示一段话的结束。
  5. 如果结束位置超过了minChunkSizeChars，那么则进行切分得到一段话的chunk，否则不切分
  6. 将切分后的chunk记录到一个List中
  7. 然后跳转到第二步，处理剩余的tokens

- 我们可以自己实现一个简单的划分规则

  ```java
  public class MyTextSplitter extends TextSplitter{
      @Override
      protected List<String> splitText(String text) {
          return List.of(split(text));
      }
  
      private String[] split(String text) {
          return text.split("\\s*\\R\\s*\\R\\s*");
      }
  }
  ```

- 在Service中替换成自己的Splitter

- 结果：![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-14_12-26-39.png)



### DocumentWriter

得到按指定逻辑切分后的Document之后，就需要把它们做向量化并存入向量数据库了。DocumentWriter有一个子接口VectorStore，就表示向量数据库，而VectorStore有一个默认实现类SimpleVectorStore，可以先尝试使用它来进行Document的向量化和存储。

## VectorStore

通过创建SimpleVectorStore可以将前面配置好的Document载入

- 创建SimpleVectorStore代码

  ```java
  @PostConstruct
      private void init(){
          OpenAiApi build = OpenAiApi.builder()
                  .apiKey("sk-***************")
                  .baseUrl("https://dashscope.aliyuncs.com/compatible-mode")
                  .build();
  
          OpenAiChatModel chatModelBuild = OpenAiChatModel.builder()
                  .openAiApi(build)
                  .defaultOptions(OpenAiChatOptions.builder().
                          model("qwen-plus").build())
                  .build();
  
          client = ChatClient.builder(chatModelBuild)
                  .build();
          OpenAiEmbeddingOptions options = OpenAiEmbeddingOptions.builder().
                  model("text-embedding-v3")
                  .build();
          OpenAiEmbeddingModel embeddingModel = new OpenAiEmbeddingModel(build, MetadataMode.EMBED, options);
          vectorStore = SimpleVectorStore.builder(embeddingModel).build();
      }
  ```

  **注意：**这里需要使用支持embedding的，之前使用DeepSeek并没有，报错会报404，可以注册一个阿里的提供免费token

- **使用：**

  ```java
  public String chat(String message) {
          ChatClient client = deepSeekChatClient.getClient();
          SimpleVectorStore vectorStore = deepSeekChatClient.getVectorStore();
          vectorStore.doAdd(documentService.loadText());
  
          MessageChatMemoryAdvisor memoryAdvisor = new MessageChatMemoryAdvisor(inMemoryChatMemory, "user123", 10);
          return client.prompt()
                  .user(message)
                  .system("你是一个架构师，帮助用户回答各类编程难题")
                  .advisors(memoryAdvisor, QuestionAnswerAdvisor.builder(vectorStore).build())
                  .call()
                  .content();
      }
  ```

- 导入的文本![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-14_15-40-00.png)

- 结果：![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-14_15-41-11.png)![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/Spring%20Ai/Snipaste_2025-04-14_15-40-59.png)

- 这样就可以成功查询

- SimpleVectorStore默认使用内存进行保存

- 后续可使用RedisVectorStore进行操作