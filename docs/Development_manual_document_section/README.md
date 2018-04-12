# RT-Thread Micropython 开发手册介绍

----------

!!! abstract "摘要"
     本手册介绍了 RT-Thread MicroPython 的基础知识、常用模块，以及开发新模块的流程。带领读者了解 MicroPython ，并学会使用 MicroPython 进行开发。
     
## 1.1 主要特性

- MicroPython 是 Python 3 编程语言的一种精简而高效的实现，它包含 Python 标准库的一个子集，并被优化为在微控制器和受限环境中运行。

- RT-Thread MicroPython 可以运行在任何搭载了 RT-Thread 操作系统并且有一定资源的嵌入式平台上。

- MicroPython 可以运行在有一定资源的开发板上，给你一个低层次的 Python 操作系统，可以用来控制各种电子系统。

- MicroPython 富有各种高级特性，比如交互式提示、任意精度整数、闭包函数、列表解析、生成器、异常处理等等。

- MicroPython 的目标是尽可能与普通 Python 兼容，使开发者能够轻松地将代码从桌面端转移到微控制器或嵌入式系统。

## 1.2 MicroPython 的优势

- Python 是一款容易上手的脚本语言，同时具有强大的功能，语法优雅简单。使用 MicroPython 编程可以降低嵌入式的开发门槛，让更多的人体验嵌入式的乐趣。

- 通过 MicroPython 实现硬件底层的访问和控制，不需要了解底层寄存器、数据手册、厂家的库函数等，即可轻松控制硬件。

- 外设与常用功能都有相应的模块，降低开发难度，使开发和移植变得容易和快速。