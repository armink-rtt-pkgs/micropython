# RT-Thread MicroPython 开发指南：初级篇


## 1. 本文简介

- 介绍如何在 MicroPython 上运行一个完整的 python 文件。
- 介绍 MicroPython 上常用模块的使用方式，带你熟悉开发 MicroPython 的基本方法。
- 介绍如何使用 machine 模块对硬件进行控制，让你熟悉使用 MicroPython 操作硬件。

---

## 2. 在 MicroPython 中运行 python 文件

在 MicroPython 上运行整个 python 文件有以下要求：

- 系统内使用了 rt-thread 的文件系统
- 开启了 msh 

符合以上两点，我们就可以使用 msh 命令行中的 python 命令加上文件名来执行一个 python 文件了。


## 3. MicroPython 常用模块介绍

下面是在使用 MicroPython 开发的过程中一些常用的模块，了解这些模块的使用方式，可以让你很好的使用 MicroPython 的功能。

这些模块可以通过 env 工具的 menuconfig 功能来开启和关闭，如果你需要使用特定的模块，在menuconfig 中选中模块名，保存退出后，重新编译运行即可。

### 3.1 Basis Module

下面是 micropython 的基本模块，运用这些模块，你可以使用 MicroPython 的基本功能。

#### rtthread
#### time
#### sys
#### math
#### uio
#### ucollections
#### ustruct
#### array
#### gc

### 3.2 Hardware Module

下面是 MicroPython 硬件相关的模块，运用这些模块，你可以使用 MicroPython 轻松的操纵硬件。

#### machine
##### 1、machine.Pin
##### 2、machine.I2C
##### 3、machine.SPI
##### 4、machine.UART

### 3.3 System Module

下面是 MicroPython 系统相关的模块，运用这些模块，你可以使用系统相关的功能。

#### uos
#### uselect
#### uctypes
#### uerrno

### 3.4 Tools Module

下面是 MicroPython 工具类模块，运用这些模块，你就可以方便的使用这些工具。

#### cmath
#### ubinascii
#### uhashlib
#### uheapq
#### ujson
#### ure
#### uzlib
#### urandom

### 3.5 Network Module

下面是 MicroPython 网络类模块，通过这个基本模块，你可以使用网络连接的基本功能。

#### usocket