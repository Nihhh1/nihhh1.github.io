---

layout: post
title: EasyExcel底层逻辑
subtitle: EasyExcel底层逻辑
date: 2024-11-12
author: ni
catalog: true
tags:
  - EasyExcel
  - 学习
---

## 前言

​	**遇到业务需求，需要通过Excel批量的进行导入数据。根据网上的资料进行对比，EasyExcel框架的性能比POI更强，消耗的内存更小**

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

    

## **执行流程**

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

    </p>