---

layout: post
title: SimpleRateLimiter
date: 2024-11-13
author: ni
catalog: true
tags:
  - 接口限流
  - Redis
---

## 前言

**在项目中为了防止用户连续多次点击给系统带来负担，以及避免数据出错。需要对接口进行限流操作**

## 实现技术

- **AOP**
- **Redis**

## 实现

#### 1. 对需要进行限流的接口进行标注

```java
@Target(ElementType.METHOD)
@Retention(RetentionPolicy.RUNTIME)
@Documented
public @interface RateLimiter {
    /**
     * 限流key,支持使用Spring el表达式来动态获取方法上的参数值
     * 格式类似于  #code.id #{#code}
     */
    String key() default "";

    /**
     * 限流时间,单位秒
     */
    int time() default 60;

    /**
     * 限流次数
     */
    int count() default 100;

    /**
     * 限流类型
     */
    LimitType limitType() default LimitType.DEFAULT;

    /**
     * 提示消息 支持国际化 格式为 {code}
     */
    String message() default "{rate.limiter.message}";
}
```

#### 2. 当被注解标记的方法被执行前，先执行Aop的方法

```java
@Before("@annotation(rateLimiter)")
    public void doBefore(JoinPoint point, RateLimiter rateLimiter) {
        int time = rateLimiter.time();
        int count = rateLimiter.count();
        try {
            String combineKey = getCombineKey(rateLimiter, point);
            RateType rateType = RateType.OVERALL;
            if (rateLimiter.limitType() == LimitType.CLUSTER) {
                rateType = RateType.PER_CLIENT;
            }
            long number = RedisUtils.rateLimiter(combineKey, rateType, count, time);
            if (number == -1) {
                String message = rateLimiter.message();
                if (StringUtils.startsWith(message, "{") && StringUtils.endsWith(message, "}")) {
                    message = MessageUtils.message(StringUtils.substring(message, 1, message.length() - 1));
                }
                throw new ServiceException(message);
            }
            log.info("限制令牌 => {}, 剩余令牌 => {}, 缓存key => '{}'", count, number, combineKey);
        } catch (Exception e) {
            if (e instanceof ServiceException) {
                throw e;
            } else {
                throw new RuntimeException("服务器限流异常，请稍候再试", e);
            }
        }
    }

    private String getCombineKey(RateLimiter rateLimiter, JoinPoint point) {
        String key = rateLimiter.key();
        if (StringUtils.isNotBlank(key)) {
            MethodSignature signature = (MethodSignature) point.getSignature();
            Method targetMethod = signature.getMethod();
            Object[] args = point.getArgs();
            //noinspection DataFlowIssue
            MethodBasedEvaluationContext context =
                new MethodBasedEvaluationContext(null, targetMethod, args, pnd);
            context.setBeanResolver(new BeanFactoryResolver(SpringUtils.getBeanFactory()));
            Expression expression;
            if (StringUtils.startsWith(key, parserContext.getExpressionPrefix())
                && StringUtils.endsWith(key, parserContext.getExpressionSuffix())) {
                expression = parser.parseExpression(key, parserContext);
            } else {
                expression = parser.parseExpression(key);
            }
            key = expression.getValue(context, String.class);
        }
        StringBuilder stringBuffer = new StringBuilder(GlobalConstants.RATE_LIMIT_KEY);
        stringBuffer.append(ServletUtils.getRequest().getRequestURI()).append(":");
        if (rateLimiter.limitType() == LimitType.IP) {
            // 获取请求ip
            stringBuffer.append(ServletUtils.getClientIP()).append(":");
        } else if (rateLimiter.limitType() == LimitType.CLUSTER) {
            // 获取客户端实例id
            stringBuffer.append(RedisUtils.getClient().getId()).append(":");
        }
        return stringBuffer.append(key).toString();
    }
```

getCombineKey()方法根据方法名称和限流类型进行拼接Redis的key，然后进行尝试获取锁，并对令牌扣一

```java
public static long rateLimiter(String key, RateType rateType, int rate, int rateInterval) {
        RRateLimiter rateLimiter = CLIENT.getRateLimiter(key);
        rateLimiter.trySetRate(rateType, rate, rateInterval, RateIntervalUnit.SECONDS);
        if (rateLimiter.tryAcquire()) {
            return rateLimiter.availablePermits();
        } else {
            return -1L;
        }
    }
```

## 拓展

#### 1. 市面上常见的限流算法

 - **1.1 计数器**

   利用一个计数器 Count .当请求来时就加一操作，当一个请求处理完成之后 就减一操作。Count > 某个值时，触发限流策略。

 - **1.2 漏斗算法**
   
   <p>
       <img src="https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/75e1b3fc63ddc4db6e29655bf86b5164.png" alt="注解一" title="注解一" />
   </p>
   
    利用漏斗模型来控制，请求处理的流量；通常实现方式是通过一个**队列**来实现；当请求过多时，队列就会开始积压请求，如果队列满了，就会开拒绝请求。很多系统都有这样的设计，例如：TCP的请求，和滑动窗口。



- **1.3 令牌桶算法**

  关于`令牌桶`算法，主要是有一个中间人(**通常是一个任务…**)。在一个桶内按照一定的速率放入一些 token，然后，处理程序要处理请求时，需要拿到 token，才能处理；如果拿不到，则不处理。并且桶的大小是会有限制的，超出之后就被丢弃或者拒绝**这其实一个主动流控的方式。**

#### 2. Guava的RateLimter工具

  RateLimiter使用的是一种叫令牌桶的流控算法，RateLimiter会按照一定的频率往桶里扔令牌，线程拿到令牌才能执行。**它其实就是令牌桶流控算法的一种实现**

#### 2.1 使用

- **导入maven依赖**

```java
<dependency>
    <groupId>com.google.guava</groupId>
    <artifactId>guava</artifactId>
    <version>19.0</version>
</dependency>
```

- **配置RateLimiter**

```java
  @Configuration
public class RateLimiterConfig {

    @Value("${rate.limiter:30}")
    private Integer rateLimiter;

    @Bean
    public RateLimiter rateLimiter(){
        return RateLimiter.create(rateLimiter);
    }
    
}
```

- **使用代码**

```java
/**
 * @author liuzihao
 * @create 2022-01-15-12:30
 * 限流
 */
@RestController
@Slf4j
public class RateLimiterController {

    @Autowired
    RateLimiter rateLimiter;

    /**
     * 非阻塞限流接口,
     * @return
     */
    @GetMapping("/tryAcquire")
    public String tryAcquire() {

        log.info("非阻塞限流接口");

        /**
         * 尝试获取令牌，如未获取到则直接失败(也可以设置try的时间)
         */
        if (rateLimiter.tryAcquire()) {
            log.info("通过.../ 数率为{}", rateLimiter.getRate());
            return "success";
        }
        return "fail";

    }

    /**
     * 阻塞接口
     */

    @GetMapping("/acquire")
    public String acquire() {
        rateLimiter.acquire();
        log.info("通过流接口....... -流量：{}", rateLimiter.getRate());

        return "success";
    }
}
```

  
