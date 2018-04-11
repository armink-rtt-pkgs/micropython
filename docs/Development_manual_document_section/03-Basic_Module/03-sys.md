# **sys** – 系统特有功能函数

!!! tip "简介"
    `sys` 模块提供系统相关的功能。

## 函数

- sys.exit(retval=0)  
  终止当前程序给定的退出代码。 函数会抛出 SystemExit 异常。

- sys.print_exception(exc, file=sys.stdout)  
  打印异常与追踪到一个类似文件的对象 file (或者缺省 sys.stdout ).

## 常数

- sys.argv  
  当前程序启动时参数的可变列表。

- sys.byteorder  
  系统字节顺序 (“little” or “big”).

- sys.implementation  
  使用当前Python实现的。

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

## 示例 

```
>>> import sys
>>> sys.version
'3.4.0'
>>> sys.version_info
(3, 4, 0)
>>> sys.path
['', '/libs/mpy/']
>>> sys.__name__
'sys'
>>> sys.platform
'rt-thread'
```

更多内容可参考 [sys](http://docs.micropython.org/en/latest/pyboard/library/sys.html) 。

----------