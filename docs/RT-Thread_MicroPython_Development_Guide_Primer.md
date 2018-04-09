# RT-Thread MicroPython 开发指南：初级篇

## 1. 简介

- 本文将介绍如何在 MicroPython 上运行一个完整的 python 文件。
- 介绍 MicroPython 上常用模块的使用方式，带你熟悉开发 MicroPython 的基本方法。
- 介绍如何使用 machine 模块对硬件进行控制，让你熟悉使用 MicroPython 操作硬件。

---

## 2. 运行 python 文件

在 MicroPython 上运行 python 文件有以下要求：

- 系统内使用了 rt-thread 的文件系统。
- 开启 `msh` 功能。

符合以上两点，就可以使用 `msh` 命令行中的 `python` 命令加上 `*.py` 文件名来执行一个 python 文件了。

## 3. MicroPython 常用模块介绍

- 下面是在使用 MicroPython 开发的过程中一些常用的模块，了解这些模块的使用方式，可以让你很好的使用 MicroPython 的功能。

- 这些模块可以通过 env 工具的 menuconfig 功能来开启和关闭，如果你需要使用特定的模块，在 menuconfig 中选中模块名，保存退出后，重新编译运行即可。

### 3.1 **Basis Module**

下面是 micropython 的基本模块，运用这些模块，你可以使用 MicroPython 的基本功能。

#### **rtthread** – 系统相关函数
`rtthread` 模块提供了与 rt-thread 操作系统相关的功能，如查看栈使用情况等。

- rtthread.current_tid()
返回当前线程的 id 。

- rtthread.is_preempt_thread()
返回是否是可抢占线程。

- rtthread.stacks_analyze()
返回当前系统线程和栈使用信息。

`example`:
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

----------

#### **utime** – 时间相关函数
`utime` 模块提供获取当前时间和日期、测量时间间隔和延迟的功能。

更多内容可参考 [`time`](http://docs.micropython.org/en/latest/pyboard/library/utime.html#module-utime)  。

`函数`

- utime.sleep(seconds)
休眠指定的时间（秒），Seconds 可以是浮点数。注意有些版本的 MicroPython不支持浮点数，为了兼容可以使用 sleep_ms() 和 ``sleep_us()``函数。

- utime.sleep_ms(ms)
延时指定毫秒，参数不能小于0。

- utime.sleep_us(us)
延时指定微秒，参数不能小于0。

- utime.ticks_ms()
返回不断递增的毫秒计数器，在某些值后会重新计数(未指定)。计数值本身无特定意义，只适合用在``ticks_diff()``。

注：执行标准数学运算（+，-）或关系运算符（<，>，>，> =）直接在这些值上会导致无效结果。执行数学运算然后传递结果作为论据来`ticks_diff()` 或 ` ticks_add() ` 也将导致后一个函数的无效结果。

- utime.ticks_us()
和上面 ticks_ms() 类似，只是返回微秒。

- utime.ticks_cpu()
与 ticks_ms() 和 ticks_us() 类似，具有更高精度 (使用 CPU 时钟)。

- 可用性：并非每个端口都实现此功能。

`example`:

```
>>> import time
>>> 
>>> time.sleep(1)           # sleep for 1 second
>>> time.sleep_ms(500)      # sleep for 500 milliseconds
>>> time.sleep_us(10)       # sleep for 10 microseconds
>>> start = time.ticks_ms() # get value of millisecond counter
>>> delta = time.ticks_diff(time.ticks_ms(), start) # compute time difference
```

----------

#### **sys** – 系统特有功能函数
`sys` 模块提供系统相关的功能。

更多内容可参考 [sys](http://docs.micropython.org/en/latest/pyboard/library/sys.html) 。

`函数`

- sys.exit(retval=0)
终止当前程序给定的退出代码。 函数会抛出 SystemExit 异常。

- sys.print_exception(exc, file=sys.stdout)
打印异常与追踪到一个类似文件的对象 file (或者缺省 sys.stdout ).

`常数`

- sys.argv
当前程序启动时参数的可变列表。

- sys.byteorder
系统字节顺序 (“little” or “big”).

- sys.implementation
使用当前Python实现的。对于micropython，它具有以下属性:

- sys.modules
加载模块字典。在一部分环境中它可能不包含内置模块。

- sys.path
搜索导入模块的可变目录列表。

- sys.platform
返回当前操作系统信息。

- sys.stderr
标准错误流。

- sys.stdin
标准输入流。

- sys.stdout
标准输出流。

- sys.version
符合的Python语言版本，如字符串。

- sys.version_info
Python 语言版本，实现符合，作为一个元组的值。

`example`:

```
>>> import uos
>>> uos.                        # Tab 
__name__        uname           chdir           getcwd
listdir         mkdir           remove          rmdir
stat            unlink          mount           umount
>>> uos.mkdir("rtthread")
>>> uos.getcwd()
'/'
>>> uos.chdir("rtthread")
>>> uos.getcwd()
'/rtthread'
>>> uos.listdir()
['web_root', 'rtthread', '11']
>>> uos.rmdir("11")
>>> uos.listdir()
['web_root', 'rtthread']
>>> 
```

----------

#### **math** – 数学函数
`math` 模块提供了对 C 标准定义的数学函数的访问。

**注意** : 需要带有硬件FPU，精度是32位，这个模块需要浮点功能支持。

更多内容可参考  [math](https://docs.python.org/3.5/library/math.html?highlight=math#module-math) 。

`函数`

- math.acos(x)
返回 ``x`` 的反余弦。

- math.acosh(x)
返回 ``x`` 的逆双曲余弦。

- math.asin(x)
返回 ``x`` 的反正弦。

- math.asinh(x)
返回``x`` 的逆双曲正弦。

- math.atan(x)
返回 ``x`` 的逆切线。

- math.atan2(y, x)
Return the principal value of the inverse tangent of y/x.

- math.atanh(x)
Return the inverse hyperbolic tangent of x.

- math.ceil(x)
Return an integer, being x rounded towards positive infinity.

- math.copysign(x, y)
Return x with the sign of y.

- math.cos(x)
Return the cosine of x.

- math.cosh(x)
Return the hyperbolic cosine of x.

- math.degrees(x)
Return radians x converted to degrees.

- math.erf(x)
Return the error function of x.

- math.erfc(x)
Return the complementary error function of x.

- math.exp(x)
Return the exponential of x.

- math.expm1(x)
Return exp(x) - 1.

- math.fabs(x)
Return the absolute value of x.

- math.floor(x)
Return an integer, being x rounded towards negative infinity.

- math.fmod(x, y)
Return the remainder of x/y.

- math.frexp(x)
Decomposes a floating-point number into its mantissa and exponent. The returned value is the tuple (m, e) such that x == m * 2**e exactly. If x == 0 then the function returns (0.0, 0), otherwise the relation 0.5 <= abs(m) < 1 holds.

- math.gamma(x)
Return the gamma function of x.

- math.isfinite(x)
Return True if x is finite.

- math.isinf(x)
Return True if x is infinite.

- math.isnan(x)
Return True if x is not-a-number

- math.ldexp(x, exp)
Return x * (2**exp).

- math.lgamma(x)
Return the natural logarithm of the gamma function of x.

- math.log(x)
Return the natural logarithm of x.

- math.log10(x)
Return the base-10 logarithm of x.

- math.log2(x)
Return the base-2 logarithm of x.

- math.modf(x)
Return a tuple of two floats, being the fractional and integral parts of x. Both return values have the same sign as x.

- math.pow(x, y)
Returns x to the power of y.

- math.radians(x)
Return degrees x converted to radians.

- math.sin(x)
Return the sine of x.

- math.sinh(x)
Return the hyperbolic sine of x.

- math.sqrt(x)
Return the square root of x.

- math.tan(x)
Return the tangent of x.

- math.tanh(x)
Return the hyperbolic tangent of x.

- math.trunc(x)
返回一个整数, x 接近于0。

`常数`

- math.e
自然对数的底

- math.pi
圆周率

----------

#### **uio** – 输入/输出流
`uio` 模块包含流类型 (类似文件) 对象和帮助函数，
更多内容可参考  [uio](http://docs.micropython.org/en/latest/pyboard/library/uio.html) 。

`函数`

- uio.open(name, mode='r', **kwargs)
打开一个文件，关联到内建函数``open()``。所有端口 (用于访问文件系统) 需要支持模式参数，但支持其他参数不同的端口。

`类`

- class uio.FileIO(...)
这个文件类型用二进制方式打开文件，等于使用``open(name, “rb”)``。 不应直接使用这个实例。

- class uio.TextIOWrapper(...)
这个类型以文本方式打开文件，等同于使用``open(name, “rt”)``不应直接使用这个实例。

- class uio.StringIO([string])

- class uio.BytesIO([string])
内存文件对象。StringIO 用于文本模式 I/O (用 “t” 打开文件)，BytesIO 用于二进制方式 (用 “b” 方式)。文件对象的初始内容可以用字符串参数指定（stringio用普通字符串，bytesio用byets对象）。所有的文件方法，如 read(), write(), seek(), flush(), close() 都可以用在这些对象上，包括下面方法:

- getvalue()
获取缓存区内容。

----------

#### **ucollections** – 收集和容器类型
`ucollections` 模块实现了专门的容器数据类型，它提供了 Python 的通用内置容器的替代方案，包括了字典、列表、集合和元组。

更多的内容可参考 [ucollections](http://docs.micropython.org/en/latest/pyboard/library/ucollections.html) 。

`类`

- ucollections.namedtuple(name, fields)
这是工厂函数创建一个新的 namedtuple 型与一个特定的字段名称和集合。namedtuple 是元组允许子类要访问它的字段不仅是数字索引，而且还具有属性使用符号字段名访问语法。 字段是字符串序列指定字段名称。为了兼容的实现也可以用空间分隔的字符串命名的字段（但效率较低） 使用示例:

```python
from ucollections import namedtuple

MyTuple = namedtuple("MyTuple", ("id", "name"))
t1 = MyTuple(1, "foo")
t2 = MyTuple(2, "bar")
print(t1.name)
assert t2.name == t2[1]
ucollections.OrderedDict(...)

```

dict 类型的子类，记住并保留键的追加顺序。keys/items返回的顺序被加入:

```python
from ucollections import OrderedDict

# To make benefit of ordered keys, OrderedDict should be initialized
# from sequence of (key, value) pairs.
d = OrderedDict([("z", 1), ("a", 2)])
# More items can be added as usual
d["w"] = 5
d["b"] = 3
for k, v in d.items():
    print(k, v)
    
```
输出:

z 1
a 2
w 5
b 3

----------

#### **ustruct** – 打包和解包原始数据类型

`ustruct` 模块在 Python 值和以 Python 字节对象表示的 C 结构之间执行转换。

更多的内容可参考  [struct](https://docs.python.org/3/library/struct.html) 。

支持 size/byte 的前缀: @, <, >, !.
支持的格式代码: b, B, h, H, i, I, l, L, q, Q, s, P, f, d (最后2个需要浮点库支持).

`函数`

- ustruct.calcsize(fmt)
返回需要的字节数`fmt`。

- ustruct.pack(fmt, v1, v2, ...)
按照字符串格式`fmt` 压缩参数 v1, v2, ... 。 返回值是参数编码后的字节对象。

- ustruct.pack_into(fmt, buffer, offset, v1, v2, ...)
按照字符串格式`fmt` 压缩参数 v1, v2, ... 到缓冲区`buffer`，开始位置是`offset`。`offset`可以是负数，从缓冲区末尾开始计数。

- ustruct.unpack(fmt, data)
按照字符串格式`fmt`解压数据`data`。 返回值是解压后参数的元组。

- ustruct.unpack_from(fmt, data, offset=0)
从 fmt 的 offset 开始解压数据，如果 offset 是负数就是从缓冲区末尾开始计算。 返回值是解压后参数元组。

----------

#### **array** – 数字数据数组
``array``  模块定义了一个对象类型，它可以简洁地表示基本值的数组：字符、整数、浮点数。
更多内容可参考  [array](https://docs.python.org/3/library/array.html) 。

支持代码格式: b, B, h, H, i, I, l, L, q, Q, f, d (后2个支持浮点数)。

`类`

- class array.array(typecode[, iterable])
指定类型创建数组元素。用可选项［］做为数组的初始值，可选项［］未指定的，则创建空数组。

- append(val)
将新元素添加到数组的结尾，并将其扩展。

- extend(iterable)
使用迭代方式将新元素添加到数组的结尾，并将其扩展。

----------

#### **gc** – 控制垃圾回收
`gc` 模块提供了垃圾收集器的控制接口。
更多内容可参考  [gc](https://docs.python.org/3.5/library/gc.html#module-gc) 。

`函数`

- gc.enable()
启动自动垃圾回收。

- gc.disable()
禁用自动垃圾回收。 堆内存仍然可以分配，垃圾回收仍然可以手动启动使用 gc.collect().

- gc.collect()
运行垃圾回收。

- gc.mem_alloc()
返回分配的堆RAM的字节数。

- gc.mem_free()
返回可用堆内存的字节数。

----------

### 3.2 Hardware Module

下面是 MicroPython 硬件相关的模块，运用这些模块，你可以使用 MicroPython 轻松的操纵硬件。

#### **machine** – 与硬件相关的功能
`machine` 模块包含与特定开发板上的硬件相关的特定函数。 在这个模块中的大多数功能允许实现直接和不受限制地访问和控制系统上的硬件块（如CPU，定时器，总线等）。如果使用不当，会导致故障，死机，崩溃，在极端的情况下，硬件会损坏。

更多内容可参考 [machine](http://docs.micropython.org/en/latest/pyboard/library/machine.html) 。

`复位功能`

- machine.reset()
重置设备的方式类似类似按下 rst 按钮。

- machine.reset_cause()
Get the reset cause. See constants for the possible return values.

`中断功能`

- machine.disable_irq()
Disable interrupt requests. Returns the previous IRQ state which should be considered an opaque value. This return value should be passed to the enable_irq function to restore interrupts to their original state, before disable_irq was called.

- machine.enable_irq(state)
Re-enable interrupt requests. The state parameter should be the value that was returned from the most recent call to the disable_irq function.

`有效的相关功能`

- machine.freq()
Returns CPU frequency in hertz.

- machine.idle()
Gates the clock to the CPU, useful to reduce power consumption at any time during short or long periods. Peripherals continue working and execution resumes as soon as any interrupt is triggered (on many ports this includes system timer interrupt occurring at regular intervals on the order of millisecond).

- machine.sleep()
Stops the CPU and disables all peripherals except for WLAN. Execution is resumed from the point where the sleep was requested. For wake up to actually happen, wake sources should be configured first.

- machine.deepsleep()
Stops the CPU and all peripherals (including networking interfaces, if any). Execution is resumed from the main script, just as with a reset. The reset cause can be checked to know that we are coming from machine.DEEPSLEEP. For wake up to actually happen, wake sources should be configured first, like Pin change or RTC timeout.

`example`:

```
>>> import machine
>>>
>>> machine.info()              # show information about the board
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
>>> machine.enable_irq()        # enable interrupt
>>> machine.disable_irq()       # disable interrupt, WARNING: this operation is dangerous
>>> machine.reset()        # hard reset, like push RESET button
```




`类`

- machine.Pin
更多内容可参考 [machine.Pin](http://docs.micropython.org/en/latest/pyboard/library/machine.Pin.html)  。

`example`:
```
>>> from machine import Pin
>>> 
>>> p_out = Pin(("X1", 33), Pin.OUT_PP)
>>> p_out.value(1)              # set io high
>>> p_out.value(0)              # set io low
>>> 
>>> p_in = Pin(("X2", 32), Pin.IN, Pin.PULL_UP)
>>> p_in.value()                # get value, 0 or 1
```

- machine.I2C
更多内容可参考 [machine.I2C](http://docs.micropython.org/en/latest/pyboard/library/machine.I2C.html) 。

`software I2C example ` :
```
>>> from machine import Pin, I2C
>>> clk = Pin(("clk", 43), Pin.OUT_OD)   # Select the 43 pin device as the clock
>>> sda = Pin(("sda", 44), Pin.OUT_OD)   # Select the 44 pin device as the data line
>>> i2c = I2C(-1, clk, sda, freq=100000) # create I2C peripheral at frequency of 100kHz
>>> i2c.scan()                        # scan for slaves, returning a list of 7-bit addresses
[81]                                  # Decimal representation
>>> i2c.writeto(0x51, b'123')         # write 3 bytes to slave with 7-bit address 42
3 
>>> i2c.readfrom(0x51, 4)             # read 4 bytes from slave with 7-bit address 42
b'X\x08\x105'
>>> i2c.readfrom_mem(0x51, 0x02, 1)   # read 1 bytes from memory of slave 0x51(7-bit),
b'\x12'                               # starting at memory-address 8 in the slave
>>> i2c.writeto_mem(0x51, 2, b'\x10') # write 1 byte to memory of slave 42,
                                      # starting at address 2 in the slave
```

`hardware I2C example ` :
```
>>> from machine import Pin, I2C
>>> i2c = I2C(0)           # create I2C peripheral at frequency of 100kHz
>>> i2c.scan()                        # scan for slaves, returning a list of 7-bit addresses
[81]                                  # Decimal representation
```

- machine.SPI
更多内容可参考 [machine.SPI](http://docs.micropython.org/en/latest/pyboard/library/machine.SPI.html) 。

`software SPI example ` :
```
>>> from machine import Pin, SPI
>>> clk = Pin(("clk", 43), Pin.OUT_PP)
>>> mosi = Pin(("mosi", 44), Pin.OUT_PP)
>>> miso = Pin(("miso", 45), Pin.IN)
>>> spi = SPI(-1,500000,polarity = 0,phase = 0,bits = 8,firstbit = 0,sck = clk,mosi = mosi,miso = miso)
>>> print(spi)
SoftSPI(baudrate=500000, polarity=0, phase=0, sck=clk, mosi=mosi, miso=miso)
>>> spi.write("hello rt-thread!")
>>> spi.read(10)
b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
```

`hardware SPI example ` :
```
>>> from machine import SPI
>>> spi = SPI(50)
>>> print(spi)
SPI(device port : spi50)
>>> spi.write(b'\x9f')
>>> spi.read(5)
b'\xff\xff\xff\xff\xff'
>>> buf = bytearray(1)
>>> spi.write_readinto(b"\x9f",buf)
>>> buf
bytearray(b'\xef')
>>> spi.init(100000,0,0,8,1)     # Resetting SPI parameter
```

- machine.UART

----------

### 3.3 System Module

下面是 MicroPython 系统相关的模块，运用这些模块，你可以使用系统相关的功能。

#### **uos** – 基本的操作系统服务
`uos` 模块包含了对文件系统的访问操作，是对应 CPython 模块的一个子集。

更多内容可参考 [uos](http://docs.micropython.org/en/latest/pyboard/library/uos.html) 。

`函数`

- uos.chdir(path)
更改当前目录。

- uos.getcwd()
获取当前目录。

- uos.ilistdir([dir])
这个函数返回一个迭代器，然后产生三元组对应正在列出的目录中的条目。没有参数，它列出了
当前目录，否则它列出了目录给出的`dir`。

- 3-元组的形式`(name, type, inode)`:
name 是一个字符串（或字节，如果是一个字节对象），是输入的名称；
type 是一个整数，指定的条目类型，与普通文件和目录0x4000 0x8000；
inode 对应文件的inode的整数，可0的文件系统，没有这样的概念。
uos.listdir([dir])
没有参数，列出当前目录。否则列出给定目录。

- uos.mkdir(path)
创建一个目录。

- uos.remove(path)
删除文件。

- uos.rmdir(path)
删除目录。

- uos.rename(old_path, new_path)
重命名文件。

- uos.stat(path)
获取文件或目录的状态。

- uos.sync()
同步所有的文件系统。

`example`:
```
>>> import uos
>>> uos.                        # Tab 
__name__        uname           chdir           getcwd
listdir         mkdir           remove          rmdir
stat            unlink          mount           umount
>>> uos.mkdir("rtthread")
>>> uos.getcwd()
'/'
>>> uos.chdir("rtthread")
>>> uos.getcwd()
'/rtthread'
>>> uos.listdir()
['web_root', 'rtthread', '11']
>>> uos.rmdir("11")
>>> uos.listdir()
['web_root', 'rtthread']
>>> 
```

----------

#### **uselect** – 等待流事件
`uselect` 模块提供了在流上等待事件的功能（选择可操作的流），轮询是在多个对象上等待读/写活动的有效方法。

更多内容可参考 [select](https://docs.python.org/3.5/library/select.html#module-select) 。

`函数`

- select.poll()
创建轮询类的实例。

- select.select(rlist, wlist, xlist[, timeout])
等待激活一组对象。

提供的兼容性和效率不高,推荐使用 `Poll`。

`eventmask` 

- select.POLLIN - 读取可用数据

- select.POLLOUT - 写入更多数据

- select.POLLERR - 发生错误

- select.POLLHUP - 流结束/连接终止检测
eventmask 默认 select.POLLIN | select.POLLOUT.

`类 Poll`

- poll.register(obj[, eventmask])
登记轮询对象 obj 。 

- poll.unregister(obj)
注销轮询对象 obj。

- poll.modify(obj, eventmask)
修改对象 ``obj`` 的 eventmask。

- poll.poll([timeout])
等待至少一个已注册的对象准备就绪。返回列表(obj, event, ...) 元组, event 元素指定了一个流发生的事件，是上面所描述的 `select.POLL*`常量组合。 在元组中可能有其他元素，取决于平台和版本，所以不要假定它的大小是2。如果超时，则返回空列表。超时为毫秒。

----------

#### **uctypes** – 以结构化的方式访问二进制数据
`uctypes` 模块是 MicroPython 的外函数库，它提供 C 兼容的数据类型。

更多内容可参考 [ctypes](https://docs.python.org/3/library/ctypes.html?highlight=ctypes#module-ctypes) 。

- uctypes.LITTLE_ENDIAN
Layout type for a little-endian packed structure. (Packed means that every field occupies exactly as many bytes as defined in the descriptor, i.e. the alignment is 1).

- uctypes.BIG_ENDIAN
Layout type for a big-endian packed structure.

- uctypes.NATIVE
Layout type for a native structure - with data endianness and alignment conforming to the ABI of the system on which MicroPython runs.

- uctypes.sizeof(struct)
Return size of data structure in bytes. Argument can be either structure class or specific instantiated structure object (or its aggregate field).

- uctypes.addressof(obj)
Return address of an object. Argument should be bytes, bytearray or other object supporting buffer protocol (and address of this buffer is what actually returned).

- uctypes.bytes_at(addr, size)
Capture memory at the given address and size as bytes object. As bytes object is immutable, memory is actually duplicated and copied into bytes object, so if memory contents change later, created object retains original value.

- uctypes.bytearray_at(addr, size)
Capture memory at the given address and size as bytearray object. Unlike bytes_at() function above, memory is captured by reference, so it can be both written too, and you will access current value at the given memory address.

----------

#### **uerrno** – 系统错误码模块

`uerrno` 模块提供了标准的 errno 系统符号，每个符号的值是对应的整数值。

更多内容可参考 [errno](https://docs.python.org/3/library/errno.html?highlight=errno#module-errno) 。

`Usage example` :

```
try:
    uos.mkdir("my_dir")
except OSError as exc:
    if exc.args[0] == uerrno.EEXIST:
        print("Directory already exists")
uerrno.errorcode
Dictionary mapping numeric error codes to strings with symbolic error code (see above):

>>> print(uerrno.errorcode[uerrno.EEXIST])
EEXIST
```

----------

#### **_thread** – 多线程支持
`_thread` 模块提供了用于处理多线程的基本方法——多个控制线程共享它们的全局数据空间。为了实现同步，提供了简单的锁（也称为互斥锁或二进制信号量）。

更多内容可参考 [_thread](https://docs.python.org/3/library/_thread.html?highlight=_thread#module-_thread)  。

`example`:
```
>>> ###  press CTRL + E to enter paste mode

paste mode; Ctrl-C to cancel, Ctrl-D to finish
=== import _thread
=== import time
=== 
=== def testThread():
===     while True:
===         print("Hello from thread")
===         time.sleep(2)
=== 
=== _thread.start_new_thread(testThread, ())
=== while True:
===     pass

```

----------

### 3.4 **Tools Module**

下面是 MicroPython 工具类模块，运用这些模块，你就可以方便的使用这些工具。

#### **cmath** – 复数的数学函数
`cmath` 模块提供了对复数的数学函数的访问。这个模块中的函数接受整数、浮点数或复数作为参数。他们还将接受任何有复杂（）或浮点（）方法的Python对象：这些方法分别用于将对象转换成复数或浮点数，然后将该函数应用到转换的结果中。

更多内容可参考 [cmath](https://docs.python.org/3/library/cmath.html?highlight=cmath#module-cmath)  。

`函数`

- cmath.cos(z)
返回``z``的余弦。

- cmath.exp(z)
返回``z``的指数。

- cmath.log(z)
返回``z``的对数。

- cmath.log10(z)
返回``z``的常用对数。

- cmath.phase(z)
返回``z``的相位, 范围是(-pi, +pi]，以弧度表示。

- cmath.polar(z)
返回``z``的极坐标.

- cmath.rect(r, phi)
返回`模量`r``和相位``phi``的复数。

- cmath.sin(z)
返回``z``的正弦。

- cmath.sqrt(z)
返回``z``的平方根。

`常数`

- cmath.e
自然对数的指数。

- cmath.pi
圆周率。

----------

#### **ubinascii** – 二进制/ ASCII转换
`ubinascii` 模块包含许多在二进制和各种 ascii 编码的二进制表示之间转换的方法。

更多内容可参考 [binascii](https://docs.python.org/3/library/binascii.html?highlight=binascii#module-binascii)  。


`函数`

- ubinascii.hexlify(data[, sep])
将二进制数据转换为十六进制表示。

- Difference to CPython
If additional argument, sep is supplied, it is used as a separator between hexadecimal values.

- ubinascii.unhexlify(data)
将十六进制数据转换为二进制表示。返回字节串 (换言之， 反二进制转换)

- ubinascii.a2b_base64(data)
Base64编码的数据转换为二进制表示。返回字节串。

- ubinascii.b2a_base64(data)
编码base64格式的二进制数据。返回的字符串。

----------

#### **uhashlib** – 哈希算法
`uhashlib` 模块实现了二进制数据哈希算法。

更多内容可参考 [hashlib](https://docs.python.org/3/library/hashlib.html?highlight=hashlib#module-hashlib)  。

其中的算法提供的功能有:

SHA256 - The current generation, modern hashing algorithm (of SHA2 series). It is suitable for cryptographically-secure purposes. Included in the MicroPython core and any board is recommended to provide this, unless it has particular code size constraints.

SHA1 - A previous generation algorithm. Not recommended for new usages, but SHA1 is a part of number of Internet standards and existing applications, so boards targetting network connectivity and interoperatiability will try to provide this.

MD5 - A legacy algorithm, not considered cryptographically secure. Only selected boards, targetting interoperatibility with legacy applications, will offer this.

`函数`

- class uhashlib.sha256([data])
创建一个SHA256哈希对象并提供 data 赋值。

- class uhashlib.sha1([data])
创建一个SHA1哈希对象并提供 data 赋值。

- class uhashlib.md5([data])
创建一个MD5哈希对象并提供 data 赋值。

- hash.update(data)
将更多二进制数据放入哈希表中。

- hash.digest()
返回字节对象哈希的所有数据。调用此方法后，将无法将更多数据送入哈希。

- hash.hexdigest()
此方法没有实现， 使用 ubinascii.hexlify(hash.digest()) 达到类似效果。

----------

#### **uheapq** – 堆排序算法

`uheapq` 模块提供了堆排序相关算法，堆队列是一个列表，它的元素以特定的方式存储。

更多内容可参考 [heapq](https://docs.python.org/3/library/heapq.html?highlight=heapq#module-heapq)  。

`函数`

- uheapq.heappush(heap, item)
把 item 推到 heap。

- uheapq.heappop(heap)
从 heap 弹出第一个元素并返回。 如果是堆时空的会抛出 IndexError。

- uheapq.heapify(x)
将列表 x 转换成堆。

----------

#### **ujson** – JSON编码与解码
`ujson` 模块提供 Python 对象到 JSON（JavaScript Object Notation） 数据格式的转换。

更多内容可参考 [json](https://docs.python.org/3/library/json.html?highlight=json#module-json)  。

`函数`

- ujson.dumps(obj)
返回 obj JSON字符串。

- ujson.loads(str)
解析 str 字符串并返回对象。如果字符串格式错误将引发 ValueError 异常。

----------

#### **ure** – 正则表达式
`ure` 模块用于测试字符串的某个模式，执行正则表达式操作。

更多内容可参考 [re](https://docs.python.org/3/library/re.html?highlight=re#module-re)  。

- 支持操作符:
``'.'``

- 匹配任意字符。
``'[]'``

- 匹配字符集合，支持单个字符和一个范围。
``'^'``
``'$'``
``'?'``
``'*'``
``'+'``
``'??'``
``'*?'``
``'+?'``

重复计数 ({m,n}), 不支持高级的断言、命名组等。

`函数`

- ure.compile(regex)
编译正则表达式，返回 regex 对象。

- ure.match(regex, string)
用 string 匹配 regex，匹配总是从字符串的开始匹配。

- ure.search(regex, string)
在 string 中搜索 regex。不同于匹配，它搜索第一个匹配位置的正则表达式字符串 (结果可能会是0)。

- ure.DEBUG
标志值，显示表达式的调试信息。

**正则表达式对象**:

编译正则表达式，使用 `ure.compile()` 创建实例。

- regex.match(string)
- regex.search(string)
- regex.split(string, max_split=-1)

**匹配对象** :

匹配对象是 match() 和 search() 方法的返回值。

- match.group([index])
只支持数字组。

----------

#### **uzlib** – zlib 解压缩

`uzlib` 模块实现了使用 DEFLATE 算法解压缩二进制数据 (常用的 zlib 库和 gzip 文档)。目前不支持压缩。

更多内容可参考 [zlib](https://docs.python.org/3/library/zlib.html?highlight=zlib#module-zlib) 。

`函数`

- uzlib.decompress(data)
返回解压后的 bytes 数据。

----------

#### **urandom** - 随机数生成模块
`urandom` 模块实现了伪随机数生成器。

更多内容可参考 [random](https://docs.python.org/3/library/random.html?highlight=random#module-random) 。

`函数` 

- urandom.choice()
Return a random element from the non-empty sequence seq. If seq is empty, raises IndexError.

- urandom.getrandbits()
Returns a Python integer with k random bits. This method is supplied with the MersenneTwister generator and some other generators may also provide it as an optional part of the API. When available, getrandbits() enables randrange() to handle arbitrarily large ranges.

- urandom.randint()
Return a random integer N such that a <= N <= b. Alias for randrange(a, b+1).

- urandom.random()
Return the next random floating point number in the range [0.0, 1.0).

- urandom.randrange()
Return a randomly selected element from range(start, stop, step). This is equivalent to choice(range(start, stop, step)), but doesn’t actually build a range object.


- urandom.seed()
Initialize the random number generator.
If a is omitted or None, the current system time is used. If randomness sources are provided by the operating system, they are used instead of the system time (see the os.urandom() function for details on availability).

- urandom.uniform()
Return a random floating point number N such that a <= N <= b for a <= b and b <= N <= a for b < a.
The end-point value b may or may not be included in the range depending on floating-point rounding in the equation a + (b-a) * random().


----------

### 3.5 Network Module

下面是 MicroPython 网络类模块，通过这个基本模块，你可以使用网络连接的基本功能。

#### **usocket** – 套接字模块
`usocket` 模块提供对BSD套接字接口的访问。

更多的内容可参考 [socket](https://docs.python.org/3/library/socket.html) 。

`常数`

- socket.AF_INET
- socket.AF_INET6

`TCP类型定义`

- socket.SOCK_STREAM
- socket.SOCK_DGRAM

`Socket类型`

- socket.IPPROTO_UDP
- socket.IPPROTO_TCP

`IP协议号`

- socket.SOL_*
Socket option levels (an argument to setsockopt()). The exact inventory depends on a board.

- socket.SO_*
Socket options (an argument to setsockopt()). The exact inventory depends on a board.

- socket.IPPROTO_SEC
Special protocol value to create SSL-compatible socket.

`函数`

- socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP)
创建新的套接字，使用指定的地址、类型和协议号。

- socket.getaddrinfo(host, port)
传递 主机/端口 到一个5个数据的元组。元组列表的结构如下:
(family, type, proto, canonname, sockaddr)

- socket.close()
关闭套接字。一旦关闭后，套接字所有的功能都将失效。远端将接收不到任何数据 (清理队列数据后)。 在回收垃圾时套接字会自动关闭，但还是推荐在必要时用 close() 去关闭，或, or to use a with statement around them。

- socket.bind(address)
将套接字绑定到地址，套接字不能是已经绑定的。

- socket.listen([backlog])
允许服务器接收连接。如果指定了 backlog，它不能小于0 (如果小于0将自动设置为0)；超出后系统将拒绝新的连接。如果没有指定，将使用默认值。

- socket.accept()
接收连接。套接字需要指定地址并监听连接。返回值是 (conn, address)，其中conn是用来接收和发送数据的套接字，address是绑定到另一端的套接字。

- socket.connect(address)
连接到指定地址的远端套接字。

- socket.send(bytes)
发送数据。套接字需要已连接到远程。

- socket.sendall(bytes)
发送数据。套接字已连接到远程。 Unlike send(), this method will try to send all of data, by sending data chunk by chunk consecutively.
- socket.recv(bufsize)
接收数据，返回值是数据字节对象。bufsize是接收数据的最大数量。

- socket.sendto(bytes, address)
发送数据。套接字没有连接到远程，目标套接字由地址参数指定。

- socket.recvfrom(bufsize)
接收数据。返回值是 (bytes, address)，其中 bytes 是字节对象，address 是发送数据的套接字。

- socket.setsockopt(level, optname, value)
设置套接字参数。需要的符号常数定义在套接字模块 (SO_* 等)。value 可以是整数或字节对象。

- socket.settimeout(value)
设置阻塞套接字超时时间。value 参数可以是代表秒的正浮点数或 None。如果设定大于 0 的参数，在后面套接字操作超出指定时间后将引起 timeout 异常。如果参数是 0，套接字将使用非阻塞模式。如果是 None，套接字使用阻塞模式。

- socket.setblocking(flag)
设置阻塞或非阻塞模式: 如果 flag 是 false，设置非阻塞模式。

- socket.read([size])
Read up to size bytes from the socket. Return a bytes object. If size is not given, it reads all data available from the socket until EOF; as such the method will not return until the socket is closed. This function tries to read as much data as requested (no “short reads”). This may be not possible with non-blocking socket though, and then less data will be returned.

- socket.readinto(buf[, nbytes])
Read bytes into the buf. If nbytes is specified then read at most that many bytes. Otherwise, read at most len(buf) bytes. Just as read(), this method follows “no short reads” policy.
Return value: number of bytes read and stored into buf.

- socket.readline()
读取一行，以换行符结束，返回读取的数据行。

- socket.write(buf)
Write the buffer of bytes to the socket. This function will try to write all data to a socket (no “short writes”). This may be not possible with a non-blocking socket though, and returned value will be less than the length of buf.
Return value: number of bytes written.

`TCP Server example`:

```
>>> import usocket 
>>> s = usocket.socket(usocket.AF_INET,usocket.SOCK_STREAM)  # Create STREAM TCP socket
>>> s.bind(('192.168.12.32', 6001))   
>>> s.listen(5)
>>> s.setblocking(True)
>>> sock,addr=s.accept()              
>>> sock.recv(10)                    
b'rt-thread\r'
>>> s.close()
>>> 
```

`TCP Client example`:

```
>>> import usocket 
>>> s = usocket.socket(usocket.AF_INET,usocket.SOCK_STREAM)
>>> s.connect(("192.168.10.110",6000))  
>>> s.send("micropython")               
11
>>> s.close()
```

`connect to a web site example`:
```
s = socket.socket()
s.connect(socket.getaddrinfo('www.micropython.org', 80)[0][-1])
```


