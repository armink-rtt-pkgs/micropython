# MicroPython 模块

!!! abstract "摘要"
    - MicroPython 提供丰富的模块，每个模块提供特定的功能。了解开发的过程中一些常用的模块的使用方式，可以让你很好的使用 MicroPython 的功能。

    - 这些模块可以通过 env 工具的 menuconfig 功能来开启和关闭，如果你需要使用特定的模块，在 menuconfig 中选中模块名，保存退出后，重新编译运行即可。

## 1. MicroPython 模块的类别

MicroPython 的模块（函数和类库）有以下几类 :

- 内置模块：标准 Python 功能的子集，用户不能扩展。
- 扩展模块：实现了 Python 功能的一个子集，并提供用户扩展（通过Python代码）。
- 扩展模块：实现 micropython 的 Python 标准库。
- 硬件驱动模块: 特定端口或者硬件驱动的模块，因此不可移植。

## 2. Python 标准库和微型库

标准的 Python 库被 “微型化”后，就是 micropython 标准库。它们仅仅提供了该模块的核心功能。一些模块没有直接使用标准的 Python 的名字，而是冠以"u"，例如``ujson``代替``json``。也就是说 micropython 标准库（=微型库），只实现了一部分模块功能。通过他们的名字不同，用户有选择的去写一个 Python 级模块扩展功能，也是为实现更好的兼容性。

在嵌入式平台上，可添加 Python 级别封装库从而实现命名兼容 CPython，微模块即可调用他们的 u-name，也可以调用 non-u-name。根据 non-u-name 包路径的文件可重写。

例如，``import json``的话，首先搜索一个``json.py``文件或``json``目录进行加载。如果没有找到，它回退到加载内置``ujson``模块。

## 3. RT-Thread MicroPython 模块列表

### 1、系统模块
- [rtthread][1] – RT-Thread 系统相关函数
- [utime][2]  – 时间相关函数
- [sys][3]  – 系统特有功能函数
- [math][4]  – 数学函数
- [uio][5]  – 输入/输出流
- [ucollections][6]  – 收集和容器类型
- [ustruct][7]  – 打包和解包原始数据类型
- [array][8]  – 数字数据数组
- [gc][9]  – 控制垃圾回收

### 2、硬件模块
- [machine][10] – 与硬件相关的功能
- [machine.Pin][11]
- [machine.I2C][12]
- [machine.SPI][13]
- [machine.UART][14]

### 3、系统模块
- [uos][15] – 基本的 “操作系统” 服务
- [select][16] – 等待流事件
- [uctypes][17] – 以结构化的方式访问二进制数据
- [uerrno][18] – 系统错误码模块
- [_thread][19] – 多线程支持

### 4、工具模块
- [cmath][20] – 复数的数学函数
- [ubinascii][21] – 二进制/ ASCII转换
- [uhashlib][22] – 哈希算法
- [uheapq][23] – 堆排序算法
- [ujson][24] – JSON编码与解码
- [ure][25] – 正则表达式
- [uzlib][26] – zlib 解压缩
- [urandom][27] – 随机数生成模块

### 5、网络模块
- [usocket][28] – 套接字模块

[1]: 03-Basic_Module/01-rtthread.md
[2]: 03-Basic_Module/02-utime.md
[3]: 03-Basic_Module/03-sys.md
[4]: 03-Basic_Module/04-math.md
[5]: 03-Basic_Module/05-uio.md
[6]: 03-Basic_Module/06-ucollections.md
[7]: 03-Basic_Module/07-ustruct.md
[8]: 03-Basic_Module/08-array.md
[9]: 03-Basic_Module/09-gc.md
[10]: 04-Hardware_Control_Module/01-machine.md
[11]: 04-Hardware_Control_Module/02-machine-Pin.md
[12]: 04-Hardware_Control_Module/03-machine-I2C.md
[13]: 04-Hardware_Control_Module/04-machine-SPI.md
[14]: 04-Hardware_Control_Module/05-machine-UART.md
[15]: 05-System_Module/01-uos.md
[16]: 05-System_Module/02-uselect.md
[17]: 05-System_Module/03-uctypes.md
[18]: 05-System_Module/04-uerrno.md
[19]: 05-System_Module/05-_thread.md
[20]: 06-Tools_Module/01-cmath.md
[21]: 06-Tools_Module/02-ubinascii.md
[22]: 06-Tools_Module/03-uhashlib.md
[23]: 06-Tools_Module/04-uheapq.md
[24]: 06-Tools_Module/05-ujson.md
[25]: 06-Tools_Module/06-ure.md
[26]: 06-Tools_Module/07-uzlib.md
[27]: 06-Tools_Module/08-urandom.md
[28]: 07-Network_Module/01-usocket.md







