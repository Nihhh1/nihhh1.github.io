---

layout: post
title: EasyExcel底层逻辑
date: 2024-11-12
author: ni
catalog: true
tags:
  - EasyExcel
  - 学习
---

## 前言

​	**遇到业务需求，需要通过Excel批量的进行导入数据。根据网上的资料进行对比，EasyExcel框架的性能比POI更强，可以在磁盘上进行操作，消耗的内存更小**

## 实现技术

 - **监听器**

## 准备工作

- **1、创建一个自定义监听器实现ExcelListener接口**
	<p>
    <img src="https://ni-blog.oss-rg-china-mainland.aliyuncs.com/my-blog/Snipaste_2024-11-12_19-24-01.png" alt="注解一" title="注解一" />
  
  
  
- **2、创建自定义转换器可以在把数据传给监听器之前根据字典进行转换**
	<p>
    <img src="https://ni-blog.oss-rg-china-mainland.aliyuncs.com/my-blog/Snipaste_2024-11-12_18-51-33.png" alt="注解一" title="注解一" />
  
  
  
- **3、创建要转换的实体类进行标注ExcelProperty取名，如果要字典转换要指定转换器，可根据传进来的字典类型，查询数据库**
	<p>
    <img src="https://ni-blog.oss-rg-china-mainland.aliyuncs.com/my-blog/Snipaste_2024-11-12_19-35-23.png" alt="注解一" title="注解一" />
  
  

## **使用执行流程**

- **在导入之前，通过exportExcel方法进行生成模版，通过指定的类获取属性，通过转换获得有指定格式的Excel在里面添加数据后可进行导入**
	
  <p>
	  <img src="https://ni-blog.oss-rg-china-mainland.aliyuncs.com/my-blog/Snipaste_2024-11-12_19-25-13.png" alt="注解一" title="注解一" />
  
	
- **在doRead方法里进行读取数据**
	<p>
    <img src="https://ni-blog.oss-rg-china-mainland.aliyuncs.com/my-blog/Snipaste_2024-11-12_19-27-09.png" alt="注解一" title="注解一" />
  
  
- **读取完数据后，会被拦截器拦截，进行额外操作，例如根据字典进行转换**

- **执行监听器的invoke方法**

    <p>
    <img src="https://ni-blog.oss-rg-china-mainland.aliyuncs.com/my-blog/Snipaste_2024-11-12_18-39-51.png" alt="注解一" title="注解一" />

## 源码解析

### 1.  **EasyExcel类**

   EasyExcel 类什么都没有，只是简单继承了 EasyExcelFactory，因此，EasyExcelFactory 才是核心。

```java
public class EasyExcel extends EasyExcelFactory {
    public EasyExcel() {
    }
}
```

### 2. EasyExcelFactory类源码

#### 2.1 EasyExcelFactory读取Excel的源码部分

下列源码中，有非常多个 read 方法，最终使用的都是 ExcelReaderBuilder 类的构造模式来创建读 excel 的对象。read 方法的参数无非就是：file（excel 文件），readListener（专门一行一行读取 excel 文件的工具），head（将 excel 按照头部映射为一个 class），pathName（excel 文件路径名），inputstream（excel 文件的输入流）。关键在于看 ExcelReaderBuilder 究竟要构造怎么样的对象。

```java
	public static ExcelReaderBuilder read() {
        return new ExcelReaderBuilder();
    }
    public static ExcelReaderBuilder read(File file) {
        return read((File)file, (Class)null, (ReadListener)null);
    }
    public static ExcelReaderBuilder read(File file, ReadListener readListener) {
        return read((File)file, (Class)null, readListener);
    }
    public static ExcelReaderBuilder read(File file, Class head, ReadListener readListener) {
        ExcelReaderBuilder excelReaderBuilder = new ExcelReaderBuilder();
        excelReaderBuilder.file(file);
        if (head != null) {
            excelReaderBuilder.head(head);
        }
        if (readListener != null) {
            excelReaderBuilder.registerReadListener(readListener);
        }
        return excelReaderBuilder;
    }
    public static ExcelReaderBuilder read(String pathName) {
        return read((String)pathName, (Class)null, (ReadListener)null);
    }
    public static ExcelReaderBuilder read(String pathName, ReadListener readListener) {
        return read((String)pathName, (Class)null, readListener);
    }
    public static ExcelReaderBuilder read(String pathName, Class head, ReadListener readListener) {
        ExcelReaderBuilder excelReaderBuilder = new ExcelReaderBuilder();
        excelReaderBuilder.file(pathName);
        if (head != null) {
            excelReaderBuilder.head(head);
        }
        if (readListener != null) {
            excelReaderBuilder.registerReadListener(readListener);
        }
        return excelReaderBuilder;
    }
    public static ExcelReaderBuilder read(InputStream inputStream) {
        return read((InputStream)inputStream, (Class)null, (ReadListener)null);
    }
    public static ExcelReaderBuilder read(InputStream inputStream, ReadListener readListener) {
        return read((InputStream)inputStream, (Class)null, readListener);
    }
    public static ExcelReaderBuilder read(InputStream inputStream, Class head, ReadListener readListener) {
        ExcelReaderBuilder excelReaderBuilder = new ExcelReaderBuilder();
        excelReaderBuilder.file(inputStream);
        if (head != null) {
            excelReaderBuilder.head(head);
        }
        if (readListener != null) {
            excelReaderBuilder.registerReadListener(readListener);
        }
        return excelReaderBuilder;
    }

```

#### 2.2ExcelReaderBuilder 构造ReadWorkbook

ReadWorkbook 类记录的是读 excel 这个动作的所有属性，比如读取的是哪种 excel 文件（xls，还是 xlsx），哪个文件，是否忽略空行，等等。

```java
public class ReadWorkbook extends ReadBasicParameter {
    private ExcelTypeEnum excelType; //读取哪类excel，这是枚举，包含 .xls 和 .xlsx
    private InputStream inputStream; // 文件的输入流
    private File file; // 文件
    private Boolean mandatoryUseInputStream; // 是否强制使用文件输入流方式
    private Boolean autoCloseStream; // 是否自动关闭输入流
    private Object customObject; // 不清楚 
    private ReadCache readCache; // 缓存，当需要把 excel 的全部/部分读取到内存时，用缓存存储
    private Boolean ignoreEmptyRow; // 是否忽略空行
    private ReadCacheSelector readCacheSelector; // 用于选择什么时候用内存去存储临时数据，什么时候用磁盘存储临时数据
    private String password; // 打开excel文件的密码
    private String xlsxSAXParserFactoryName; 
    private Boolean useDefaultListener; // 是否使用默认的监听器
    private Set<CellExtraTypeEnum> extraReadSet; // 额外读取单元格的哪些信息
}

```

```java
public class ReadBasicParameter extends BasicParameter {
    private Integer headRowNumber; // 表头为多少行
    private List<ReadListener> customReadListenerList = new ArrayList();
}

```

```java
public class BasicParameter {
    private List<List<String>> head; // 读取文件头对应的列表，会根据列表匹配数据
    private Class clazz; // excel 数据映射的类
    private List<Converter> customConverterList; 
    private Boolean autoTrim; // 自动去除首尾空格
    private Boolean use1904windowing; // 	excel中时间是存储1900年起的一个双精度浮点数，但是有时候默认开始日期是1904，所以设置这个值改成默认1904年开始
    private Locale locale;
}

```

最终会调用 ReadListener 的几个方法，ReadListener 是接口，我们可以实现接口，并实现接口的几个方法，这样，就可以达到在该几个方法中处理 excel 数据的目的。

#### 2.3 ReadListener 接口

```java
public interface ReadListener<T> extends Listener {
    void onException(Exception var1, AnalysisContext var2) throws Exception;
    void invokeHead(Map<Integer, CellData> var1, AnalysisContext var2);
    void invoke(T var1, AnalysisContext var2);
    void extra(CellExtra var1, AnalysisContext var2);
    void doAfterAllAnalysed(AnalysisContext var1);
    boolean hasNext(AnalysisContext var1);
}

```

1. invokeHead 方法：读取头部，那前几行是头部呢？ReadWorkbook 中 headRowNumber 属性决定，
   headRowNumber 等于几，那么 invokeHead 方法就执行几次，每次只读取一行表头，存储在参数 Map<Integer, CellData> var1 中，一般情况下我们自定义的 ReadListener 的 invokeHead 方法都是 invokeHeadMap(Map<Integer, String> headMap, AnalysisContext context)，Map 接口存储的就是一行表头，key 表示第几列，从 0 开始，value 表示表头的内容。
2. invoke 方法：当 invokeHead 方法执行完后，就执行 invoke 方法，invoke 方法表示读取 excel 内容数据，没执行一次，就读取一行数据，参数 var1 就表示一行数据。
3. extra 方法：excel 的单元格除了内容外，可能还有注释，超链接等等，此方法就用于读取单元的额外信息。
4. doAfterAllAnalysed 方法：当读取完 excel 的数据后，就执行一次此方法，用于做结束工作。
5. hasNext 方法：用于判断后续是否还有数据，返回true继续扫描，false停止，可以对扫描进行控制

#### 2.4EasyExcelFactory 中写 excel 的源码部分

excel 的数据映射为 java 的类，列就对应成员变量，所以写 excel 就是将 List<映射对象> 写入一个文件中。
 看看如下 EasyExcelFactory 中写 excel 的关键代码，也就是需要一个文件和映射类，表明将什么样类型的数据写入哪个文件中。

```java
public static ExcelWriterBuilder write(File file, Class head)
public static ExcelWriterBuilder write(String pathName, Class head)
public static ExcelWriterBuilder write(OutputStream outputStream, Class head)

```

ExcelWriterBuilder 构造类又能设置哪些写属性呢？属性值是用 WriteWorkbook 类存储的。

```java
public class WriteWorkbook extends WriteBasicParameter {
    private ExcelTypeEnum excelType; // 写入哪种类型文件，xls 或者 xlsx
    private File file; // excel 文件
    private OutputStream outputStream; // excel 输出流
    private InputStream templateInputStream; 
    private File templateFile;
    private Boolean autoCloseStream; // 自动关闭
    private Boolean mandatoryUseInputStream; // 是否强制使用输出流方式
    private String password; // excel 文件密码
    private Boolean inMemory; // excel 文件放在内存，还是放在磁盘
    private Boolean writeExcelOnException; 
}

```

```java
public class WriteBasicParameter extends BasicParameter {
    private Integer relativeHeadRowIndex; // 头部最后一行的索引值，从 0 开始
    private Boolean needHead;  // 是否需要头部
    private List<WriteHandler> customWriteHandlerList = new ArrayList();
    private Boolean useDefaultStyle;  // 是否使用默认样式
    private Boolean automaticMergeHead;
    private Collection<Integer> excludeColumnIndexes;
    private Collection<String> excludeColumnFiledNames;
    private Collection<Integer> includeColumnIndexes;
    private Collection<String> includeColumnFiledNames;
}

```

使用例子：

```java
		try {
            File excelFile = File.createTempFile("hehe", ".xlsx");
            List<ExcelReadEntity> list = new ArrayList<>();
            ExcelReadEntity entity = new ExcelReadEntity();
            entity.setId(32535L);
            entity.setName("张三");
            list.add(entity);
            EasyExcel.write(excelFile, ExcelReadEntity.class)
                    .excelType(ExcelTypeEnum.XLSX)
                    .autoCloseStream(true)
                    .inMemory(true)
                    .password("hello")
                    .sheet()
                    .sheetName("第一页")
                    .doWrite(list);
            return excelFile;
        } catch (Exception e) {
            e.printStackTrace();
        }

```



## 注意事项

#### 1. 如果 excel 的数据量不是很大，且只是简单地把数据读取出来，可以直接使用默认的同步监听器来读取即可，默认监听器也是 invoke 方法一次次执行，将 excel 数据添加到一个 list 中，当读取完毕后，将 list 返回。注意，当 excel 非常大时，list 会占用非常大的内存，容易导致溢出，因此，建议后端在收到 excel 文件时，虽然无法判断 excel 包含多少条数据，但是可以判断一下文件的大小，比如不能超过 5M，超过则不处理。

#### 2. 读取的Excel数据为String，建议接收的类的属性都设置为String来接收

#### 3. 在使用时例如在执行doRead()之前必须通过read()方法获取ExcelReaderBuilder 在通过sheet()方法获取ExcelReaderSheetBuilder并可以在之中定义属性值



