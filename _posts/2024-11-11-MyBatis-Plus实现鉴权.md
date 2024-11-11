---

layout: post
title: Mybatis-Plus实现鉴权
subtitle: Mybatis-Plus实现鉴权的底层原理
date: 2024-11-11
author: ni
catalog: true
tags:
  - 鉴权
  - Mybatis-Plus
  - 学习
---



## 前言

​	**在实习中开发对后台进行开发时后台的使用用户有多种，需要针对不同的用户进行`权限管理`，例如：同部门的用户只能看到同部门的人员，部门下的角色只能操作，查看该部门信息**

## 实现技术

- **Mybatis-Plus拦截器**
- **注解**
- **执行器**

## 准备工作

- **1、权限注解根据注解获取被权限管理的所有Bean**
  <p>
    <img src="https://ni-blog.oss-rg-china-mainland.aliyuncs.com/Snipaste_2024-11-11_15-39-57.png" alt="注解一" title="注解一" />
  </p>

-  **2、存储权限内占位符名称，以及替换值**
  <p>
   <img src="https://ni-blog.oss-rg-china-mainland.aliyuncs.com/db20ff071cf1268bdea75101aab4bc5.png" alt="注解一" title="注解一"  />
  </p>
  
- **3、权限列表**
  <p>
   <img src="https://ni-blog.oss-rg-china-mainland.aliyuncs.com/Snipaste_2024-11-11_15-44-21.png" alt="权限枚举" title="权限枚举"  />
  </p>
## 鉴权流程

- **创建一个拦截器并且继承 BaseMultiTableInnerInterceptor 实现InnerInterceptor**
   <p>
   <img src="https://ni-blog.oss-rg-china-mainland.aliyuncs.com/Snipaste_2024-11-11_15-40-43.png" alt="权限枚举" title="权限枚举" />
  </p>
- **在MybatisConfig配置类中会进行添加拦截器操作，通过注解PropertySource获取资源文件，通过资源文件获取要扫描的包路径**
  <p>
   <img src="https://ni-blog.oss-rg-china-mainland.aliyuncs.com/Snipaste_2024-11-11_16-33-06.png" alt="拦截器初始化" title="拦截器初始化"  />
  </p>
- **在拦截器初始化过程中会调用执行器的构造函数，在执行器的构造函数中，会对被标记的Mapper类和方法进行加载**
  <p>
   <img src="https://ni-blog.oss-rg-china-mainland.aliyuncs.com/Snipaste_2024-11-11_15-42-59.png" alt="执行器初始化" title="执行器初始化"  />
  </p>
- **拦截器会在执行sql语句前进行设置where条件，在设置条件时就会调`执行器`的`getSqlSegment`方法**
  <p>
   <img src="https://ni-blog.oss-rg-china-mainland.aliyuncs.com/Snipaste_2024-11-11_17-05-14.png" alt="getSqlSegment" title="getSqlSegment"  />
  </p>
- **查看buildDataFilter方法**
  - **提前准备数据**
	   <p>
	   <img src="https://ni-blog.oss-rg-china-mainland.aliyuncs.com/Snipaste_2024-11-11_17-12-13.png" alt="查看buildDataFilter方法" title="查看buildDataFilter方法" />
	   </p>
  - **进行填充，返回条件**
   
     <p>
     <img src="https://ni-blog.oss-rg-china-mainland.aliyuncs.com/Snipaste_2024-11-11_17-18-20.png" alt="填充，返回条件" title="填充，返回条件"  />
     </p>
     
    - **最后根据开头的joinStr去掉开头多的OR或者AND返回完整sql条件**

## 小结

​	**灵活使用拦截器对方法进行拦截，拦截后通过获取标签对象中的值获取条件语句中需要的值，也可以通过对Service的Bean创建名称，执行指定方法获取需要的值进行填充**

## 个人看法

​	**个人认为还可以做的更加灵活，现在这个枚举类规定不够灵活，可以将其创建在数据中，在登录的时候可以进行查询权限，加入到loginUser的用户信息缓存中**
