# 6.1810 (Fall 2024) 课程材料

本分支（`materials`）存放 MIT 6.1810 2024 秋季课程的配套材料，与仓库中各 lab 分支
（`util`/`syscall`/`pgtbl`/`traps`/`cow`/`net`/`lock`/`fs`/`mmap`，对应 upstream
`xv6-labs-2024`）配套使用。

## 目录结构

| 路径 | 内容 |
|---|---|
| `schedule.html` | 课程表（含每讲阅读要求、lab 发布时间），本地入口建议从这里开始 |
| `xv6/book-riscv-rev4.pdf` | xv6 教材（rev4） |
| `quiz/` | 2024 期中/期末真题及答案（`q24-1*` 期中，`q24-2*` 期末）；往年题见 https://pdos.csail.mit.edu/6.1810/2024/quiz.html |
| `lec/` | 各讲文字笔记（`l-*.txt`）、FAQ、示例代码（`l-c/`、`l-overview/`、`l-coordination.c`、`mmap.c`）；slides 未收录 |
| `labs/` | 9 个 lab 的说明页 |
| `homework/` | 作业题页面 |
| `scripts/fetch-materials.sh` | 按需拉取第三方阅读材料到 `readings/`（已 gitignore，版权属原作者，不随仓库分发） |

## 使用

```sh
git switch materials        # 查看材料
sh scripts/fetch-materials.sh   # 可选：下载 readings（论文、手册，约 8MB）
```

浏览器打开 `index.html` 或 `schedule.html` 即可离线浏览；未收录的文件（往年考题、
slides、readings）链接仍指向原课程网站。

## 署名与许可

- 课程材料（讲义、考题、lab 说明、作业、课程网站页面）来自
  [MIT 6.1810 Fall 2024](https://pdos.csail.mit.edu/6.1810/2024/)，
  版权所有 MIT，按课程网站标注以 [CC BY 3.0 US](https://creativecommons.org/licenses/by/3.0/us/) 分发。
- xv6 教材 `book-riscv-rev4.pdf`：Copyright 2006-2024 Frans Kaashoek, Robert Morris, Russ Cox。
- xv6 代码（其他分支）：MIT License，见各分支 `LICENSE`。
