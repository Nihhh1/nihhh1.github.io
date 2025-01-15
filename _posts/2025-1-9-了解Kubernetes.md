---
layout: post
title: 了解Kubernetes
date: 2025-1-9
author: ni
catalog: true
tags:
  - Kubernetes
  - 云原生
---

## 背景：容器、Docker 和 Kubernetes

### 什么是容器？
容器是轻量级的可执行应用程序组件，它将源代码与任何环境中运行代码所需的所有操作系统 (OS) 库和依赖项结合。

容器利用一种操作系统虚拟化形式，通过隔离进程并控制这些进程可以访问的 CPU、内存和磁盘数量，允许多个应用程序共享一个操作系统实例。由于容器比虚拟机 (VM) 更小、更节省资源且更易于移动，容器的资源效率也更高。它们允许您在更多的机器（虚拟服务器和物理服务器）上运行更多应用程序，并且操作系统实例更少。

由于容器可以在任何地方持续运行，它们对于支持混合多云环境（本地、私有云、公共云以及来自自一个云供应商的多个云服务的组合）的底层架构至关重要。

### 什么是 Docker？
Docker 是用于创建和运行 Linux® 容器的最流行工具。虽然早期的容器形式在几十年前就已推出（采用 FreeBSD Jails 和 AIX Workload Partitions 等技术），但直到 2013 年，Docker 才通过一种新的开发人员友好型和云友好型实现方式将它们真正普及。

Docker 最初是一个开源项目，但如今，它也指 Docker Inc.，即生产 Docker 的公司。Docker 是一个基于开源项目构建的商业容器工具包，并将这些改进回馈给开源社区。

Docker 建立在传统的 Linux 容器技术之上，但能够实现 Linux 内核进程的更细粒度的虚拟化，并添加了一些功能，使开发人员能够轻松地构建、部署、管理和保护容器。

尽管如今存在其他替代容器运行时平台，如开放容器计划 (OCI)、CoreOS 和 Canonical (Ubuntu) LXD，但 Docker 仍是主流选择。此外，Docker 已成为容器的代名词，有时被认为是 Kubernetes 等补充技术的竞争对手。

如今，Docker 和 Kubernetes 是领先的容器化工具，其中 Docker 占据 82% 的市场份额，而 Kubernetes 将在 2024 年占据 11.52% 的市场份额。

### 使用 Kubernetes 进行容器编排
随着容器的激增，如今，一个组织可能拥有数百或数千个容器。需要运维团队来安装和自动化容器部署。

Kubernetes 基于 Google 的内部容器编排平台 Borg，于 2014 年作为开源工具向公众推出，Microsoft、Red Hat®、IBM 和其他主要技术公司均已加入 Kubernetes 社区，成为早期成员。2015 年，Google 将 Kubernetes 捐赠给云原生计算基金会 (CNCF)，该基金会是云原生计算的开源供应商中心。

Kubernetes 于 2016 年 3 月成为 CNCF 的首个托管项目。自那时起，Kubernetes 已成为全球运行基于容器的工作负载的最广泛使用的容器编排工具。在 CNCF 的一份报告中，Kubernetes 是全球第二大开源项目（仅次于 Linux），也是 71% 的财富 100 强企业的主要容器编排工具。

2018 年，Kubernetes 成为 CNCF 的首个毕业项目，成为历史上发展最快的开源项目之一。尽管其他容器编排选择（最著名的是 Docker Swarm 和 Apache Mesos）在早期获得了一些关注，但 Kubernetes 很快就成为了容器领域的首选。

自 2016 年 Kubernetes 加入 CNCF 以来，贡献者已增至 8,012 人，增长率达 996%。截至撰写本文时，贡献者已向 GitHub 上的 Kubernetes 存储库添加了超过 123,000 条提交。

### Kubernetes 做什么？
Kubernetes 在整个应用程序生命周期内安排和自动化与容器相关的任务，包括以下内容：

- **部署**  
  将指定数量的容器部署到指定主机并保持它们处于所需状态运行。

- **发布**  
  推送是对部署的更改。Kubernetes 允许您启动、暂停、恢复或回滚推送。

- **服务发现**  
  Kubernetes 可以使用域名系统 (DNS) 名称或 IP 地址自动将容器公开到互联网或其他容器。

- **存储配置**  
  设置 Kubernetes 以根据需要为您的容器挂载持久本地或云存储。

- **负载均衡**  
  根据 CPU 使用率或自定义指标，Kubernetes 负载均衡可以跨网络分配工作负载以保持性能和稳定性。

- **自动缩放**  
  当流量激增时，Kubernetes 自动扩展可以根据需要启动新的集群来处理外部的工作负载。

- **自愈修复和实现高可用性**  
  当容器发生故障时，Kubernetes 可以自动重启或替换它以防止停机。它还可以删除不符合健康标准的容器。

### Kubernetes 框架和组件
部署 Kubernetes 时及集群，即 Kubernetes 框架的构建块。集群由节点组成，每个节点代表一个计算主机，可以是物理机或虚拟机。

Kubernetes 框架由两个主要部分组成：控制平面组件和管理单个节点的组件。

节点由 pod 组成。这些 pod 是共享相同计算资源和相同网络的容器组。它们也是 Kubernetes 中的可扩展单元。如果 pod 中的容器获取的流量超过其处理能力，Kubernetes 会将该 pod 复制到集群中的其他节点。

控制平面自动处理集群中各个节点的 pod 调度。

#### 控制平面组件
每个集群都有一个主节点，用于处理集群的控制平面。主节点运行一个调度程序服务，该服务根据开发人员设置的部署要求和可用的计算能力自动确定任何地部容器。

Kubernetes 集群的主要组件是 kube-apiserver、etcd、kube-scheduler、kube-controller-manager 和 cloud-controller-manager：

- **API 服务**：Kubernetes 中的应用程序编程接口 (API) 服务公开 Kubernetes API（用于管理、创建和配置 Kubernetes 集群的接口）并充当所有命令和查询的入口点。etcd 是一个开源分布式键值存储，用于保存和管理分布式系统所需的关键情信息。在 Kubernetes 中，etcd 管理配置数据、状态数据和元数据。调度程序：此组件跟踪资源可用性和分配规则、硬件和软件要求。控制器管理器是一个内置的控制器，它运行一个控制循环来监视集群的共享状态并与 API 服务保持通信以执行一个进程。

### 节点组件
工作节点负责部署、运行和管理容器应用程序：

- **Kubelet**：Kubelet 是一个软件代理，它接收并运行来自主节点的命令，并帮助确保容器在 pod 中运行。Kube

- **kube-proxy**：安装在集群中的每个节点上，kube-proxy 维护主机上的网络规则并监视服务和 pod 的变化。

### 其他 Kubernetes 概念和术语
- **ReplicaSet**：ReplicaSet 为特定工作负载维护一组稳定的副本集。

- **Deployment**：部署控制容器化应用程序的创建和状态，使其保持运行。它指定集群上应运行多少个 Pod 副本。如果 Pod 发生故障，ReplicaSet 将创建一个新的 Pod。

- **Kubectl**：开发人员使用 kubectl 管理集群，kubectl 是一个由直接接入 Kubernetes API 进行信任的命令行界面 (CLI) 工具。

- **DaemonSet**：DaemonSet 负责帮助确保集群中的每个节点上创建一个 pod。

- **附加组件**：Kubernetes 附加组件扩展了功能，包括集群 DNS（由 Kubernetes 提供 DNS 记录的 DNS 服务）、Web UI（用于管理集群的 Kubernetes 界面）等。

- **服务**：Kubernetes 服务是一种抽象，它定义了一组逻辑上的 Pod 及其运行方式。服务公开了集群中的一个或多个 Pod 运行的网络应用程序。它提供了一种抽象方式来平衡 Pod 的负载。

### Kubernetes 生态系统
如今，已有超过 90 种经过认证的 Kubernetes 产品，包括企业级管理平台，可提供工具、升级和附加功能，以加速容器化应用程序的开发和交付。

尽管 Kubernetes 是编排基于容器的应用程序的首选技术，但它还需要依赖其他组件（包括网络、负载均衡、存储、持续集成和持续交付 (CI/CD) 等）才能充分发挥作用。

当然可以在基于云的环境中运行 Kubernetes 集群，但对于企业组织来说，设置和管理可能复杂。这就是托管 Kubernetes 服务的用武之地。

使用托管 Kubernetes 服务，提供简便管理 Kubernetes 控制平面组件。托管服务提供帮助自动化更新、负载均衡、扩展和持续的常规操作。例如，Red Hat® OpenShift™ 是一项 Kubernetes 服务，可部署在任何云环境以及所有主要公共云上，包括 Amazon Web Services (AWS)、Microsoft Azure、Google Cloud 和 IBM Cloud™。许多云提供商提供自己的托管 Kubernetes 服务。

### Kubernetes 监控工具
Kubernetes 监控是指收集和分析与 Kubernetes 集群的运行和应用程序的健康、性能和成本特征相关的数据。

监控 Kubernetes 集群可以帮助识别异常、故障和无法加入集群的节点等问题。当前的 Kubernetes 监控解决方案包括应用程序性能管理 (APM)、可观察性、应用程序资源性能管理 (ARM) 等自动化工具。

### Istio 服务网格
Kubernetes 可以部署和扩展 Pod，但它无法管理或自动化 Pod 之间的路由，也不提供任何工具来监控、保护或调试这些连接。

随着集群中容器数量的增加，它们之间可能的连接路径数量呈指数级增长。例如，2 个容器有 2 个潜在连接，而 10 个 Pod 有 90 个，这会带来潜在的配置和管理难题。

Istio 是一个可配置的开源服务网格，它通过连接、监控和保护 Kubernetes 集群中的容器来提供解决方案。其他重要功能包括改进的调试功能和仪表板，DevOps 团队和管理员可以使用该仪表板来监控特性。

### Knative 和无服务器计算
Knative（发音为“kay-native”）是一个开源平台，它提供了通往无服务器计算的便捷途径，无服务器计算是一种可以自动扩展应用程序并处理事件的模型，使开发人员无需配置或管理服务器或端点基础设施即可构建和运行应用程序。

无服务器计算不会容器化一个特定状态的代码实例，而是根据需要实时动态代码，根据需要请求对其进行扩展或缩减，然后在不使用时删除代码。无服务器计算可防止浪费计算能力和成本，因为您只需为实际使用的资源付费。

### Kubernetes 用例
企业组织使用 Kubernetes 来支持以下用例，这些用例在构建现代 IT 基础设施时都起着至关重要的作用。

<div style="display: flex; align-items: center; margin-bottom: 20px;">
  <svg width="60" height="60" viewBox="0 0 100 100" style="margin-right: 20px;">
    <rect x="20" y="20" width="15" height="15" fill="none" stroke="#000" stroke-width="2"/>
    <rect x="42.5" y="20" width="15" height="15" fill="none" stroke="#000" stroke-width="2"/>
    <rect x="65" y="20" width="15" height="15" fill="none" stroke="#000" stroke-width="2"/>
    <rect x="20" y="42.5" width="15" height="15" fill="none" stroke="#000" stroke-width="2"/>
    <rect x="42.5" y="42.5" width="15" height="15" fill="none" stroke="#000" stroke-width="2"/>
    <rect x="65" y="42.5" width="15" height="15" fill="none" stroke="#000" stroke-width="2"/>
    <rect x="20" y="65" width="15" height="15" fill="none" stroke="#000" stroke-width="2"/>
    <rect x="42.5" y="65" width="15" height="15" fill="none" stroke="#000" stroke-width="2"/>
    <rect x="65" y="65" width="15" height="15" fill="none" stroke="#000" stroke-width="2"/>
    <line x1="35" y1="27.5" x2="42.5" y2="27.5" stroke="#000" stroke-width="2"/>
    <line x1="57.5" y1="27.5" x2="65" y2="27.5" stroke="#000" stroke-width="2"/>
    <line x1="27.5" y1="35" x2="27.5" y2="42.5" stroke="#000" stroke-width="2"/>
    <line x1="50" y1="35" x2="50" y2="42.5" stroke="#000" stroke-width="2"/>
    <line x1="72.5" y1="35" x2="72.5" y2="42.5" stroke="#000" stroke-width="2"/>
  </svg>
  <div>
    <h4>微服务架构和云原生开发</h4>
    <p>云原生是一种用于构建、部署和管理基于云的应用程序的软件开发方法。云原生的主要优势在于，它允许 DevOps 和其他团队编写一次代码，然后部署在任何云服务提供商的任何云基础设施上。</p>
    <p>这种现代开发过程依赖于微服务，这种方法中，单个应用程序由许多松散耦合且可独立部署的较小组件或服务组成，这些组件或服务部署在由 Kubernetes 管理的容器中。</p>
    <p>有助于确保每个微服务都具有有效运行所需的资源，同时最大限度地减少与手动管理多个容器相关的运营开销。</p>
  </div>
</div>

<div style="display: flex; align-items: center; margin-bottom: 20px;">
  <svg width="60" height="60" viewBox="0 0 100 100" style="margin-right: 20px;">
    <path d="M25,45 Q30,35 40,35 Q45,35 50,40 Q55,35 60,35 Q70,35 75,45 Q80,55 70,60 Q60,65 50,60 Q40,65 30,60 Q20,55 25,45" fill="none" stroke="#000" stroke-width="2"/>
    <rect x="40" y="55" width="20" height="15" fill="none" stroke="#000" stroke-width="2"/>
    <line x1="45" y1="58" x2="55" y2="58" stroke="#000" stroke-width="2"/>
    <line x1="45" y1="62" x2="55" y2="62" stroke="#000" stroke-width="2"/>
  </svg>
  <div>
    <h4>混合多云环境</h4>
    <p>混合云结合并统一了公有云、私有云和本地数据中心基础设施，从而创建单一、灵活、成本最优的 IT 基础设施。</p>
    <p>如今，混合云已与来自多家云供应商的多云、公有云服务合并，从而创建了混合多云环境。</p>
    <p>混合多云方法可以提高灵活性，减少组织对单一供应商的依赖，从而防止供应商锁定。由于 Kubernetes 为云原生开发奠定了基础，因此它是混合多云的关键。</p>
  </div>
</div>

<div style="display: flex; align-items: center; margin-bottom: 20px;">
  <svg width="60" height="60" viewBox="0 0 100 100" style="margin-right: 20px;">
    <rect x="25" y="25" width="50" height="40" fill="none" stroke="#000" stroke-width="2"/>
    <rect x="30" y="30" width="40" height="8" fill="none" stroke="#000" stroke-width="2"/>
    <line x1="30" y1="45" x2="70" y2="45" stroke="#000" stroke-width="2"/>
    <line x1="30" y1="52" x2="70" y2="52" stroke="#000" stroke-width="2"/>
  </svg>
  <div>
    <h4>大规模应用</h4>
    <p>Kubernetes 支持通过自动扩展来大规模部署云应用。此过程允许应用程序扩展或缩减规模，自动适应需求变化，速度快、效率高且停机时间短。</p>
    <p>部署的弹性可扩展性意味着可以根据用户流量的变化（例如零售网站上的限时抢购）添加或删除资源。</p>
  </div>
</div>

<div style="display: flex; align-items: center; margin-bottom: 20px;">
  <svg width="60" height="60" viewBox="0 0 100 100" style="margin-right: 20px;">
    <rect x="20" y="35" width="25" height="30" fill="none" stroke="#000" stroke-width="2" rx="3"/>
    <path d="M50,50 L60,50" stroke="#000" stroke-width="2"/>
    <path d="M60,50 L55,45 M60,50 L55,55" stroke="#000" stroke-width="2"/>
    <circle cx="70" cy="40" r="4" fill="none" stroke="#000" stroke-width="2"/>
    <circle cx="70" cy="50" r="4" fill="none" stroke="#000" stroke-width="2"/>
    <circle cx="70" cy="60" r="4" fill="none" stroke="#000" stroke-width="2"/>
    <line x1="70" y1="40" x2="70" y2="60" stroke="#000" stroke-width="1" stroke-dasharray="2,2"/>
  </svg>
  <div>
    <h4>应用程序现代化</h4>
    <p>Kubernetes 提供了支持应用程序现代化所需的现代云平台，将单体式遗留应用程序迁移和转换为基于微服务架构的云应用程序。</p>
  </div>
</div>

<div style="display: flex; align-items: center; margin-bottom: 20px;">
  <svg width="60" height="60" viewBox="0 0 100 100" style="margin-right: 20px;">
    <g transform="translate(50,50)">
      <path d="M-20,-10 A25,25 0 0,1 20,-10" fill="none" stroke="#000" stroke-width="2" stroke-linecap="round"/>
      <path d="M20,10 A25,25 0 0,1 -20,10" fill="none" stroke="#000" stroke-width="2" stroke-linecap="round"/>
      <path d="M-20,-10 L-25,-15 L-15,-15 L-20,-10" fill="#000"/>
      <path d="M20,10 L25,15 L15,15 L20,10" fill="#000"/>
    </g>
    <circle cx="50" cy="50" r="2" fill="#000"/>
  </svg>
  <div>
    <h4>DevOps 实践</h4>
    <p>自动化是 DevOps 的核心，它通过整合和自动化软件开发和 IT 运维团队的工作来提高质量和交付的速度。通过自动化应用程序的配置和部署，帮助 DevOps 团队快速构建和更新应用程序。</p>
  </div>
</div>

<div style="display: flex; align-items: center; margin-bottom: 20px;">
  <svg width="60" height="60" viewBox="0 0 100 100" style="margin-right: 20px;">
    <g transform="translate(50,50)">
      <circle cx="0" cy="-20" r="4" fill="#fff" stroke="#000" stroke-width="2"/>
      <circle cx="-20" cy="0" r="4" fill="#fff" stroke="#000" stroke-width="2"/>
      <circle cx="20" cy="0" r="4" fill="#fff" stroke="#000" stroke-width="2"/>
      <circle cx="0" cy="20" r="4" fill="#fff" stroke="#000" stroke-width="2"/>
      <path d="M0,-20 L-20,0 M0,-20 L20,0 M-20,0 L0,20 M20,0 L0,20" 
            stroke="#000" stroke-width="1.5" 
            stroke-dasharray="3,2"/>
      <circle cx="0" cy="-20" r="2" fill="#000"/>
      <circle cx="20" cy="0" r="2" fill="#000"/>
    </g>
  </svg>
  <div>
    <h4>人工智能 (AI) 和机器学习 (ML)</h4>
    <p>支持 AI 的 ML 模型和大型语言模型 (LLM) 包含一些组件，如果单独管理这些组件会非常复杂。因此通过跨云平台进行集成，部署可以更好地提供训练、测试和部署这些模型的能力和灵活性。</p>
  </div>
</div>
