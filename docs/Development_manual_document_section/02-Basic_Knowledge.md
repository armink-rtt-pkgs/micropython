# RT-Thread MicroPython 基础知识

## 1. 运行 python 文件

在 MicroPython 上运行 python 文件有以下要求：

- 系统内使用了 rt-thread 的文件系统。
- 开启 `msh` 功能。

符合以上两点，就可以使用 `msh` 命令行中的 `python` 命令加上 `*.py` 文件名来执行一个 python 文件了。

## 2. MicroPython 模块介绍

- 下面是在使用 MicroPython 开发的过程中一些常用的模块，了解这些模块的使用方式，可以让你很好的使用 MicroPython 的功能。

- 这些模块可以通过 env 工具的 menuconfig 功能来开启和关闭，如果你需要使用特定的模块，在 menuconfig 中选中模块名，保存退出后，重新编译运行即可。