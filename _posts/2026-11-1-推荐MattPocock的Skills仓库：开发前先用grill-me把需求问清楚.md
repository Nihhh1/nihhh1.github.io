---
layout: post
title: 推荐 Matt Pocock 的 Skills 仓库：开发前先用 grill-me 把需求问清楚
subtitle: 一个 AI 编程技能合集，以及我最常用的那一个
date: 2026-11-1
author: ni
catalog: true
tags:
  - AI
  - 开发
  - 效率
---

## 前言

- 用 AI 写代码一段时间后发现，真正耗时的往往不是写代码，而是需求没对齐：AI 按自己的理解补全了没说清的规则，写完才发现方向不对，再来回沟通、返工。
- 后来开始用 Matt Pocock 开源的 Skills 仓库，其中的 `grill-me` 改变了我开发前的习惯：先让 AI 把需求问透，再动手写代码。
- 这篇文章推荐这个仓库，重点讲 `grill-me`，再简单介绍仓库里的其他 SKILL。

## 仓库简介

- 地址：[https://github.com/mattpocock/skills](https://github.com/mattpocock/skills)
- 作者 Matt Pocock 是 TypeScript 社区很有名的讲师，这个仓库是他日常用 AI 编程时沉淀下来的一套 SKILL，在社区里很受欢迎。
- SKILL 分为两个目录：
  - `skills/engineering/`：工程开发相关，从需求、拆任务、实现到审查。
  - `skills/productivity/`：通用效率类，比如访谈、交接、学习。
- SKILL 分两种触发方式：
  - **用户调用**：需要手动输入命令，比如 `/grill-me`。
  - **模型调用**：AI 判断场景合适时会自动使用，也可以手动调用。

### 安装

- Claude Code 以插件方式安装，默认自动更新：

```sh
claude plugin install mattpocock-skills@claude-plugins-official
```

- Codex、GitHub Copilot、Gemini CLI 也有对应的安装方式；其他 Agent（Cursor、Windsurf 等）可以用：

```sh
npx skills@latest add mattpocock/skills
```

- 同一个 Agent 只用一种方式安装，否则会出现重复的 SKILL。
- 安装后在每个项目里运行一次 `/setup-matt-pocock-skills`，配置项目的问题跟踪方式和文档目录。

## 我最常用的：grill-me

### 它做什么

- `grill-me` 的意思是"拷问我"：你给出一个计划或需求，AI 会持续追问，直到所有分支都问清楚为止。
- `grill-me` 本身很短，实际逻辑在 `grilling` 里，几个规则设计得很好：
  - **设计树**：把需求看成一棵树，每个决定下面都挂着依赖它的其他决定。
  - **按轮提问**：每一轮只问"前置条件已经确定"的问题，不会在上游还没定的时候就问下游细节。
  - **每个问题都附带推荐答案**：问题措辞设计成"回答是就等于采纳推荐"，大部分问题只需要回一个"是"。
  - **事实自己查，决定交给人**：代码里能查到的事实（现有实现、配置、调用关系）由 AI 自己去查，只有业务决策才问你。
  - **问完才动手**：直到没有未确定的分支，并且你确认双方理解一致，才会进入下一步。

### 我的用法

- 开发新功能或迭代老功能前，先把当前功能文档交给 AI，再用 `grill-me` 结合文档确认需求、补全规则。
- 它追问的往往正是需求文档里没写、但开发时一定会遇到的地方：边界条件、异常情况、和已有功能的交互、旧数据怎么兼容。
- 这些问题以前要么是写到一半才发现、回头再问，要么是 AI 自己"合理假设"了一个答案。现在开发前就一次问完，规则写清楚了，双方的沟通时间少了很多。
- 问完之后，这段对话本身就是一份完整的需求上下文，可以直接交给后面的 SKILL 生成需求文档或拆分任务。

## 其他 SKILL 简介

按开发流程大致分一下类。

### 需求澄清

| SKILL | 作用 |
|---|---|
| grill-me | 拷问你的计划，直到所有分支都确定 |
| grilling | grill 系列背后通用的访谈逻辑，其他 SKILL 会复用 |
| grill-with-docs | 一边访谈，一边维护领域术语表（GLOSSARY.md）和架构决策记录（ADR） |
| domain-modeling | 对照术语表梳理项目的领域模型 |
| to-questionnaire | 自己定不了的决定，整理成问卷发给对应的人 |

### 从需求到任务

| SKILL | 作用 |
|---|---|
| to-spec | 把当前对话整理成需求规格，发布到问题跟踪系统 |
| to-tickets | 把计划或需求拆成一组带依赖关系的小任务 |
| wayfinder | 超大型工作先画一张"决策地图"，一次解决一个决策 |
| triage | 让 issue 按分诊角色在状态机里流转 |

### 实现

| SKILL | 作用 |
|---|---|
| implement | 按需求或任务实现，在约定的边界用 TDD，结束时做代码审查 |
| implement-spec | 在一个分支上用多个子代理并行实现整份需求 |
| tdd | 红-绿-重构的测试驱动开发，每次只做一个纵向切片 |
| prototype | 做一个用完即弃的原型，回答某个设计问题 |
| diagnosing-bugs | 按步骤排查 bug：复现、缩小范围、假设、加日志、修复、补回归测试 |
| codebase-design | 设计"深模块"的通用指导：接口小、边界清晰 |
| improve-codebase-architecture | 扫描代码库找可改进的地方，生成报告后和你讨论 |

### 审查与交付

| SKILL | 作用 |
|---|---|
| code-review | 从"编码规范"和"需求实现"两个维度并行审查改动 |
| pr | 生成 PR 描述：改动概览、前后对比、合并风险评估 |
| wizard | 生成一个交互式脚本，引导人完成只能人来做的步骤（如配置凭据、数据迁移） |

### 会话与协作

| SKILL | 作用 |
|---|---|
| handoff | 把当前对话压缩成一份文档，交给另一个 Agent 继续 |
| retro | 会话结束后复盘，按严重程度提出环境改进建议 |
| research | 查一手资料回答问题，保存成带引用的 Markdown |
| wait-what | 用大白话重新解释一段没看懂的回复 |
| teach | 分多次会话教你一个技能或概念 |
| writing-for-agents | 编写 SKILL、AGENTS.md / CLAUDE.md 等给 AI 看的文档的指导 |
| ask-matt | 不知道该用哪个 SKILL 时，帮你选 |
| setup-matt-pocock-skills | 每个项目运行一次，做初始化配置 |

- 其中 `code-review` 我在原版基础上做了改编：翻译成中文，并在代码异味里加了一条"冗余处理"，用来找出已经被前面流程覆盖、没有实际作用的循环和查询。

## 总结

- 这个仓库最值得学的不只是某个 SKILL，而是它的拆分方式：每个 SKILL 只负责开发流程中的一个环节，可以单独用，也可以串起来用。
- 如果只试一个，推荐 `grill-me`：开发前多花十几分钟把需求问清楚，比写完再返工省时间得多。
- 这些 SKILL 都是普通的 Markdown 文件，觉得哪里不适合自己的项目，可以直接改。
