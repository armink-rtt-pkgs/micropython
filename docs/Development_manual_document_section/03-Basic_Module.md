# RT-Thread MicroPython 基础模块

### **Basis Module**

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