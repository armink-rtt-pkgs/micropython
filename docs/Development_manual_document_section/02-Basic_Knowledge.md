# RT-Thread MicroPython 基础知识

## 1. 运行 python 文件

在 MicroPython 上运行 python 文件有以下要求：

- 系统内使用了 rt-thread 的文件系统。
- 开启 `msh` 功能。

符合以上两点，就可以使用 `msh` 命令行中的 `python` 命令加上 `*.py` 文件名来执行一个 python 文件了。
