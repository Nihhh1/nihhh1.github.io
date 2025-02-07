---

layout: post
title: 状态机spring statemachine
date: 2025-2-7
author: ni
catalog: true
tags:
  - 学习
---

## 1 什么是状态机

状态机，也就是 State Machine ，不是指一台实际机器，而是指一个数学模型。说白了，一般就是指一张状态转换图。例如，根据自动门的运行规则，我们可以抽象出下面这么一个图。

自动门有两个状态，open 和 closed ，closed 状态下，如果读取开门信号，那么状态就会切换为 open 。open 状态下如果读取关门信号，状态就会切换为 closed 。

状态机的全称是有限状态自动机，自动两个字也是包含重要含义的。给定一个状态机，同时给定它的当前状态以及输入，那么输出状态时可以明确的运算出来的。例如对于自动门，给定初始状态 closed ，给定输入“开门”，那么下一个状态时可以运算出来的。

#### 1.2 四大概念

下面来给出状态机的四大概念。

- 第一个是 State ，状态。一个状态机至少要包含两个状态。例如上面自动门的例子，有 open 和 closed 两个状态。

- 第二个是 Event ，事件。事件就是执行某个操作的触发条件或者口令。对于自动门，“按下开门按钮”就是一个事件。

- 第三个是 Action ，动作。事件发生以后要执行动作。例如事件是“按开门按钮”，动作是“开门”。编程的时候，一个 Action一般就对应一个函数。

- 第四个是 Transition ，变换。也就是从一个状态变化为另一个状态。例如“开门过程”就是一个变换。

## 2 状态机图

做需求时，需要了解以下六种元素：起始、终止、现态、次态（目标状态）、动作、条件，我们就可以完成一个状态机图了：

以订单为例：以从待支付状态转换为待发货状态为例

![](https://nihhh1-blog.oss-cn-beijing.aliyuncs.com/my-blog/%E7%8A%B6%E6%80%81%E6%9C%BA/Snipaste_2025-02-07_10-54-59.png)

- ①现态：是指当前所处的状态。待支付

- ②条件：又称为“事件”，当一个条件被满足，将会触发一个动作，或者执行一次状态的迁移。支付事件

- ③动作：条件满足后执行的动作。动作执行完毕后，可以迁移到新的状态，也可以仍旧保持原状态。动作不是必需的，当条件满足后，也可以不执行任何动作，直接迁移到新状态。状态转换为待发货

- ④次态：条件满足后要迁往的新状态。“次态”是相对于“现态”而言的，“次态”一旦被激活，就转变成新的“现态”了。待发货 注意事项

1、避免把某个“程序动作”当作是一种“状态”来处理。那么如何区分“动作”和“状态”？“动作”是不稳定的，即使没有条件的触发，“动作”一旦执行完毕就结束了；而“状态”是相对稳定的，如果没有外部条件的触发，一个状态会一直持续下去。

2、状态划分时漏掉一些状态，导致跳转逻辑不完整。所以在设计状态机时，我们需要反复的查看设计的状态图或者状态表，最终达到一种牢不可破的设计方案。

## 3 spring statemachine

#### 3.1  状态机spring statemachine 概述

Spring Statemachine是应用程序开发人员在Spring应用程序中使用状态机概念的框架

Spring Statemachine旨在提供以下功能：

- 易于使用的扁平单级状态机，用于简单的使用案例。

- 分层状态机结构，以简化复杂的状态配置。

- 状态机区域提供更复杂的状态配置。

- 使用触发器，转换，警卫和操作。

- 键入安全配置适配器。

- 生成器模式，用于在Spring Application上下文之外使用的简单实例化通常用例的食谱

- 基于Zookeeper的分布式状态机

- 状态机事件监听器。

- UML Eclipse Papyrus建模。

- 将计算机配置存储在永久存储中。

- Spring IOC集成将bean与状态机关联起来。

状态机功能强大，因为行为始终保证一致，使调试相对容易。这是因为操作规则是在机器启动时写成的。这个想法是你的应用程序可能存在于有限数量的状态中，某些预定义的触发器可以将你的应用程序从一个状态转移到另一个状态。此类触发器可以基于事件或计时器。

在应用程序之外定义高级逻辑然后依靠状态机来管理状态要容易得多。您可以通过发送事件，侦听更改或仅请求当前状态来与状态机进行交互。

#### 3.2 快速开始

以订单状态扭转的例子为例：

表结构设计如下：

```sql
CREATE TABLE `tb_order` (
      `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT COMMENT '主键ID',
      `order_code` varchar(128) COLLATE utf8mb4_bin DEFAULT NULL COMMENT '订单编码',
      `status` smallint(3) DEFAULT NULL COMMENT '订单状态',
      `name` varchar(64) COLLATE utf8mb4_bin DEFAULT NULL COMMENT '订单名称',
      `price` decimal(12,2) DEFAULT NULL COMMENT '价格',
      `delete_flag` tinyint(2) NOT NULL DEFAULT '0' COMMENT '删除标记，0未删除  1已删除',
      `create_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP COMMENT '创建时间',
      `update_time` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT '更新时间',
      `create_user_code` varchar(32) COLLATE utf8mb4_bin DEFAULT NULL COMMENT '创建人',
      `update_user_code` varchar(32) COLLATE utf8mb4_bin DEFAULT NULL COMMENT '更新人',
      `version` int(11) NOT NULL DEFAULT '0' COMMENT '版本号',
      `remark` varchar(64) COLLATE utf8mb4_bin DEFAULT NULL COMMENT '备注',
      PRIMARY KEY (`id`)
    ) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin COMMENT='订单表';
```

```sql
    insert  into `tb_order`(`id`,`order_code`,`status`,`name`,`price`,`delete_flag`,`create_time`,`update_time`,`create_user_code`,`update_user_code`,`version`,`remark`) values 
    (2,'A111',1,'A','22.00',0,'2022-10-15 16:14:11','2022-10-02 21:29:14','zhangsan','zhangsan',0,NULL),
    (3,'A111',1,'订单A','22.00',0,'2022-10-02 21:53:13','2022-10-02 21:29:14','zhangsan','zhangsan',0,NULL),
    (4,'A111',1,'订单A','22.00',0,'2022-10-02 21:53:13','2022-10-02 21:29:14','zhangsan','zhangsan',0,NULL),
    (5,'A111',1,'订单A','22.00',0,'2022-10-03 09:08:30','2022-10-02 21:29:14','zhangsan','zhangsan',0,NULL);
```

**1）引入依赖**


        <dependency>
            <groupId>org.springframework.statemachine</groupId>
            <artifactId>spring-statemachine-redis</artifactId>
            <version>1.2.9.RELEASE</version>
        </dependency>
        
        <dependency>
            <groupId>org.springframework.statemachine</groupId>
            <artifactId>spring-statemachine-starter</artifactId>
            <version>2.0.1.RELEASE</version>

**2）定义状态机状态和事件**

状态枚举：

    public enum OrderStatus {
            // 待支付，待发货，待收货，已完成
            WAIT_PAYMENT(1, "待支付"),
            WAIT_DELIVER(2, "待发货"),
            WAIT_RECEIVE(3, "待收货"),
            FINISH(4, "已完成");
            private Integer key;
            private String desc;
            OrderStatus(Integer key, String desc) {
                this.key = key;
                this.desc = desc;
            }
            public Integer getKey() {
                return key;
            }
            public String getDesc() {
                return desc;
            }
            public static OrderStatus getByKey(Integer key) {
                for (OrderStatus e : values()) {
                    if (e.getKey().equals(key)) {
                        return e;
                    }
                }
                throw new RuntimeException("enum not exists.");
            }
        }

事件：

```java
public enum OrderStatusChangeEvent {

        PAYED, DELIVERY, RECEIVED;
}
```

**3）定义状态机规则和配置状态机**

```java
@Configuration
@EnableStateMachine(name = "orderStateMachine")
public class OrderStateMachineConfig extends StateMachineConfigurerAdapter<OrderStatus, OrderStatusChangeEvent> {

       /* 配置状态
        *
        * @param states
        * @throws Exception
        */
    public void configure(StateMachineStateConfigurer<OrderStatus, OrderStatusChangeEvent> states) throws Exception {
        states
            .withStates()
            .initial(OrderStatus.WAIT_PAYMENT)
            .states(EnumSet.allOf(OrderStatus.class));
    }

       /* 配置状态转换事件关系
        *
        * @param transitions
        * @throws Exception
        */
    public void configure(StateMachineTransitionConfigurer<OrderStatus, OrderStatusChangeEvent> transitions) throws Exception {
        transitions

            .withExternal().source(OrderStatus.WAIT_PAYMENT).target(OrderStatus.WAIT_DELIVER).event(OrderStatusChangeEvent.PAYED)
            .and()

            .withExternal().source(OrderStatus.WAIT_DELIVER).target(OrderStatus.WAIT_RECEIVE).event(OrderStatusChangeEvent.DELIVERY)
            .and()

            .withExternal().source(OrderStatus.WAIT_RECEIVE).target(OrderStatus.FINISH).event(OrderStatusChangeEvent.RECEIVED);
    }
}
 
```

配置持久化：


```java
@Configuration
@Slf4j
public class Persist<E, S> {

       /* 持久化到内存map中
        *
        * @return
        */
        @Bean(name = "stateMachineMemPersister")
        public static StateMachinePersister getPersister() {
        return new DefaultStateMachinePersister(new StateMachinePersist() {
            @Override
            public void write(StateMachineContext context, Object contextObj) throws Exception {
                log.info("持久化状态机,context:{},contextObj:{}", JSON.toJSONString(context), JSON.toJSONString(contextObj));
                map.put(contextObj, context);
            }
            @Override
            public StateMachineContext read(Object contextObj) throws Exception {
                log.info("获取状态机,contextObj:{}", JSON.toJSONString(contextObj));
                StateMachineContext stateMachineContext = (StateMachineContext) map.get(contextObj);
                log.info("获取状态机结果,stateMachineContext:{}", JSON.toJSONString(stateMachineContext));
                return stateMachineContext;
            }
            private Map map = new HashMap();
        });
    }

    @Resource
    private RedisConnectionFactory redisConnectionFactory;

       /* 持久化到redis中，在分布式系统中使用
        *
        * @return
        */
        @Bean(name = "stateMachineRedisPersister")
        public RedisStateMachinePersister<E, S> getRedisPersister() {
        RedisStateMachineContextRepository<E, S> repository = new RedisStateMachineContextRepository<>(redisConnectionFactory);
        RepositoryStateMachinePersist p = new RepositoryStateMachinePersist<>(repository);
        return new RedisStateMachinePersister<>(p);
    }
}
```

**4）业务系统**

controller：


```java
@RestController
@RequestMapping("/order")
public class OrderController {
    @Resource
    private OrderService orderService;
        /*
         * 根据id查询订单
         *
         * @return
         */
    @RequestMapping("/getById")
    public Order getById(@RequestParam("id") Long id) {

        Order order = orderService.getById(id);
        return order;
    }

       /* 创建订单
        *
        * @return
        */
    @RequestMapping("/create")
    public String create(@RequestBody Order order) {

        orderService.create(order);
        return "sucess";
    }

       /* 对订单进行支付
        *
        * @param id
        * @return
        */
    @RequestMapping("/pay")
    public String pay(@RequestParam("id") Long id) {

        orderService.pay(id);
        return "success";
    }
    
        /* 对订单进行发货
         *
         * @param id
         * @return
         */
    @RequestMapping("/deliver")
    public String deliver(@RequestParam("id") Long id) {

        orderService.deliver(id);
        return "success";
    }

        /* 对订单进行确认收货
         *
         * @param id
         * @return
         */
    @RequestMapping("/receive")
    public String receive(@RequestParam("id") Long id) {

        orderService.receive(id);
        return "success";
    }
}
```


servie：


```java
@Service("orderService")
@Slf4j
public class OrderServiceImpl extends ServiceImpl<OrderMapper, Order> implements OrderService {
    @Resource
    private StateMachine<OrderStatus, OrderStatusChangeEvent> orderStateMachine;
    @Resource
    private StateMachinePersister<OrderStatus, OrderStatusChangeEvent, String> stateMachineMemPersister;
    @Resource
    private OrderMapper orderMapper;

    /* 创建订单
         *
         * @param order
         * @return
         */
    public Order create(Order order) {
        order.setStatus(OrderStatus.WAIT_PAYMENT.getKey());
        orderMapper.insert(order);
        return order;
    }

    /* 对订单进行支付
         *
         * @param id
         * @return
         */
    public Order pay(Long id) {
        Order order = orderMapper.selectById(id);
        log.info("线程名称：{},尝试支付，订单号：{}" ,Thread.currentThread().getName() , id);
        if (!sendEvent(OrderStatusChangeEvent.PAYED, order)) {
            log.error("线程名称：{},支付失败, 状态异常，订单信息：{}", Thread.currentThread().getName(), order);
            throw new RuntimeException("支付失败, 订单状态异常");
        }
        return order;
    }

    /* 对订单进行发货
         *
         * @param id
         * @return
         */
    public Order deliver(Long id) {
        Order order = orderMapper.selectById(id);
        log.info("线程名称：{},尝试发货，订单号：{}" ,Thread.currentThread().getName() , id);
        if (!sendEvent(OrderStatusChangeEvent.DELIVERY, order)) {
            log.error("线程名称：{},发货失败, 状态异常，订单信息：{}", Thread.currentThread().getName(), order);
            throw new RuntimeException("发货失败, 订单状态异常");
        }
        return order;
    }

    /* 对订单进行确认收货
         *
         * @param id
         * @return
         */
    public Order receive(Long id) {
        Order order = orderMapper.selectById(id);
        log.info("线程名称：{},尝试收货，订单号：{}" ,Thread.currentThread().getName() , id);
        if (!sendEvent(OrderStatusChangeEvent.RECEIVED, order)) {
            log.error("线程名称：{},收货失败, 状态异常，订单信息：{}", Thread.currentThread().getName(), order);
            throw new RuntimeException("收货失败, 订单状态异常");
        }
        return order;
    }

    /* 发送订单状态转换事件
         * synchronized修饰保证这个方法是线程安全的
         *
         * @param changeEvent
         * @param order
         * @return
         */
    private synchronized boolean sendEvent(OrderStatusChangeEvent changeEvent, Order order) {
        boolean result = false;
        try {

            orderStateMachine.start();

            stateMachineMemPersister.restore(orderStateMachine, String.valueOf(order.getId()));
            Message message = MessageBuilder.withPayload(changeEvent).setHeader("order", order).build();
            result = orderStateMachine.sendEvent(message);

            stateMachineMemPersister.persist(orderStateMachine, String.valueOf(order.getId()));
        } catch (Exception e) {
            log.error("订单操作失败:{}", e);
        } finally {
            orderStateMachine.stop();
        }
        return result;
    }
}
```


监听状态的变化：


```java
@Component("orderStateListener")
@WithStateMachine(name = "orderStateMachine")
@Slf4j
public class OrderStateListenerImpl {
    @Resource
    private OrderMapper orderMapper;

    @OnTransition(source = "WAIT_PAYMENT", target = "WAIT_DELIVER")
    public void payTransition(Message<OrderStatusChangeEvent> message) {
        Order order = (Order) message.getHeaders().get("order");
        log.info("支付，状态机反馈信息：{}",  message.getHeaders().toString());

        order.setStatus(OrderStatus.WAIT_DELIVER.getKey());
        orderMapper.updateById(order);

    }
    @OnTransition(source = "WAIT_DELIVER", target = "WAIT_RECEIVE")
    public void deliverTransition(Message<OrderStatusChangeEvent> message) {
        Order order = (Order) message.getHeaders().get("order");
        log.info("发货，状态机反馈信息：{}",  message.getHeaders().toString());

        order.setStatus(OrderStatus.WAIT_RECEIVE.getKey());
        orderMapper.updateById(order);

    }
    @OnTransition(source = "WAIT_RECEIVE", target = "FINISH")
    public void receiveTransition(Message<OrderStatusChangeEvent> message) {
        Order order = (Order) message.getHeaders().get("order");
        log.info("确认收货，状态机反馈信息：{}",  message.getHeaders().toString());

        order.setStatus(OrderStatus.FINISH.getKey());
        orderMapper.updateById(order);

    }
}
```

