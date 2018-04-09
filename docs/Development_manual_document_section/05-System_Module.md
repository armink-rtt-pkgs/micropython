# RT-Thread MicroPython 系统模块

### System Module

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