[TOC]

#  MicroPython port for RT-Thread

MicroPython 是一个有效的python实现，适合运行在mcu嵌入式系统之上。

## 1.体验MicroPython

###  1.1通过finsh/msh运行micropython

在msh命令行内输入python即可进入MicroPython的交互命令行REPL。

```
msh />python

MicroPython v1.9.3-6-g1742ab26-dirty on 2017-11-04; RT-Thread Board with stm32f4
>>> 
>>> print('Hello World')
Hello World

```

现在你就可以在终端上输入和运行python的代码。
如果你想退出python的REPL,可以使用 `CTRL + D` 快捷键。

### 1.2在REPL下粘贴程序

`micropython`有一个特别的粘贴模式。

先在命令行提示符状态下，按下Ctrl-E组合键，就会出现提示：

paste mode; Ctrl-C to cancel, Ctrl-D to finish

然后在粘贴代码，完成后按下Ctlr-D推出粘贴模式。

### 1.3运行已有的python脚本
```
msh /sdcard>python rt-thread.py

hello world!

```

### 1.4导入外部模块并运行
#### 1.4.1 rtthread module

Use the rtthread module:

```
>>> import rtthread
>>> 
>>> rtthread.is_preempt_thread()       # determine if code is running in a preemptible thread
True
>>> rtthread.current_tid()             # current thread id
268464956
>>> rtthread.stacks_analyze()          # show thread information
thread     pri  status      sp     stack size max used left tick  error
---------- ---  ------- ---------- ----------  ------  ---------- ---
elog_async  31  suspend 0x000000a8 0x00000400    26%   0x00000003 000
tshell      20  ready   0x00000260 0x00001000    39%   0x00000003 000
tidle       31  ready   0x00000070 0x00000100    51%   0x0000000f 000
SysMonitor  30  suspend 0x000000a4 0x00000200    32%   0x00000005 000
timer        4  suspend 0x00000080 0x00000200    25%   0x00000009 000
>>> 
```
#### 1.4.2 time module
Use the [`time`](http://docs.micropython.org/en/latest/pyboard/library/utime.html#module-utime) module:
```
>>> import time
>>> 
>>> time.sleep(1)           # sleep for 1 second
>>> time.sleep_ms(500)      # sleep for 500 milliseconds
>>> time.sleep_us(10)       # sleep for 10 microseconds
>>> start = time.ticks_ms() # get value of millisecond counter
>>> delta = time.ticks_diff(time.ticks_ms(), start) # compute time difference
```
#### 1.4.3 pyb module
Use the [pyb](http://docs.micropython.org/en/latest/pyboard/library/pyb.html) module:
```
>>> import pyb
>>>
>>> pyb.info()              # show information about the board
---------------------------------------------
RT-Thread
---------------------------------------------
total memory: 131048
used memory : 4920
maximum allocated memory: 5836
thread     pri  status      sp     stack size max used left tick  error
---------- ---  ------- ---------- ----------  ------  ---------- ---
elog_async  31  suspend 0x000000a8 0x00000400    26%   0x00000003 000
tshell      20  ready   0x0000019c 0x00001000    39%   0x00000006 000
tidle       31  ready   0x0000006c 0x00000100    50%   0x0000000b 000
SysMonitor  30  suspend 0x000000a8 0x00000200    32%   0x00000005 000
timer        4  suspend 0x0000007c 0x00000200    24%   0x00000009 000
---------------------------------------------
qstr:
  n_pool=0
  n_qstr=0
  n_str_data_bytes=0
  n_total_bytes=0
---------------------------------------------
GC:
  16064 total
  464 : 15600
  1=14 2=6 m=3
>>> pyb.enable_irq()        # enable interrupt
>>> pyb.disable_irq()       # disable interrupt, WARNING: this operation is dangerous
>>> time_start = pyb.millis()          # return the number of milliseconds
>>> pyb.elapsed_millis(time_start)     # calculate the elapsed time of milliseconds
2449
>>> time_start = pyb.micros()          # return the number of microseconds
>>> pyb.elapsed_micros(time_start)     # calculate the elapsed time of microseconds
1769000
>>> pyb.delay(1000)         # delay milliseconds
>>> pyb.udelay(1000*1000)   # delay microseconds
>>> pyb.hard_reset()        # hard reset, like push RESET button
```



## 2.MicroPython 库

- MicroPython实现了python函数库每个模块的一部分。
- 为了便于扩展，MicroPython版本的标准python模块都有一个“u”的前缀。
- 因为资源或者一些其他限制，一个特定的MicroPython版本或者移植对比标准python模块可能会缺少一些特性或者功能。
- 也可以使用upip来下载一些软件包。

| Builtin functions and exceptions | 简介                                       | 当前版本是否支持 |
| -------------------------------- | ---------------------------------------- | -------- |
| array                            | arrays of numeric data                   | yes      |
| cmath                            | mathematical functions for complex numbers | yes      |
| gc                               | control the garbage collector            | yes      |
| math                             | mathematical functions                   | yes      |
| sys                              | system specific functions                | yes      |
| time                             | time related functions                   | yes      |
| ubinascii                        | binary/ASCII conversions                 | yes      |
| ucollections                     | collection and container types           | yes      |
| uerrno                           | system error codes                       | no       |
| uhashlib                         | hashing algorithms                       | yes      |
| uheapq                           | heap queue algorithm                     | yes      |
| uio                              | input/output streams                     | yes      |
| ujson                            | JSON encoding and decoding               | yes      |
| uos                              | basic “operating system” services        | no       |
| ure                              | simple regular expressions               | yes      |
| uselect                          | wait for events on a set of streams      | no       |
| usocket                          | socket module                            | no       |
| ustruct                          | pack and unpack primitive data types     | yes      |
| uzlib                            | zlib decompression                       | yes      |
| _thread                          | multithreading support                   | no       |

| micropython-specific libraries | 简介                                       | 当前版本是否支持 |
| ------------------------------ | ---------------------------------------- | -------- |
| btree                          | simple BTree database                    | no       |
| framebuf                       | Frame buffer manipulation                | no       |
| machine                        | functions related to the hardware        | yes      |
| micropython                    | access and control MicroPython internals | yes      |
| network                        | network configuration                    | no       |
| uctypes                        | access binary data in a structured way   | yes      |

| Libraries specific to the pyboard | 简介                             | 当前版本是否支持 |
| --------------------------------- | ------------------------------ | -------- |
| pyb                               | functions related to the board | yes      |
| Time related functions            |                                | yes      |
| Reset related functions           |                                | yes      |
| Interrupt related functions       |                                | yes      |
| Power related functions           |                                | yes      |
| Miscellaneous functions           |                                | yes      |
| Classes                           |                                | no       |


| rt-thread libraries for micropython | 简介                    | 当前版本是否支持 |
| ----------------------------------- | --------------------- | -------- |
| rtthread                            | rt-thread system call | yes      |

## 3.资源占用情况

使用gcc工具链编译的情况下，开启micropython，bin文件增大300KB左右。

目前默认给micropython分配的堆大小为8K，可以在menuconfig中对micropython的堆大小进行配置。

## 4.测试脚本

### 4.1 闪灯

- i.MX RT1050: 第 52 号 pin 为 LED D18，与 phy 复位引脚公用

```python
import time
from machine import Pin

LED = Pin(("LED1", 52), Pin.OUT_PP)
while True:
    LED.value(1)
    time.sleep_ms(500)
    LED.value(0)
    time.sleep_ms(500)
```

### 4.2 按键灯

- i.MX RT1050: 第 125 号 pin 为 SW8

```python
import time
from machine import Pin

led = Pin(("LED1", 52), Pin.OUT_PP)
key = Pin(("KEY", 125), Pin.IN, Pin.PULL_UP)
while True:
    if key.value():
        led.value(0)
    else:
        led.value(1)
```

### 4.3 socket

```python
import usocket
socket = usocket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
addr = usocket.getaddrinfo('www.micropython.org', 80)[0][-1]
s.connect(addr)
s.bind(("127.0.0.1", 8000))
```