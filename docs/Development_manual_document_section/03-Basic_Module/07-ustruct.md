# **ustruct** – 打包和解包原始数据类型

!!! abstract "简介"
    **ustruct** 模块在 Python 值和以 Python 字节对象表示的 C 结构之间执行转换。

- 支持 size/byte 的前缀: @, <, >, !.
- 支持的格式代码: b, B, h, H, i, I, l, L, q, Q, s, P, f, d (最后2个需要支持浮点数).

## 函数

### **ustruct.calcsize**(fmt)  
  返回需要的字节数`fmt`。

### **ustruct.pack**(fmt, v1, v2, ...)  
  按照字符串格式`fmt` 压缩参数 v1, v2, ... 。 返回值是参数编码后的字节对象。

### **ustruct.pack_into**(fmt, buffer, offset, v1, v2, ...)  
  按照字符串格式`fmt` 压缩参数 v1, v2, ... 到缓冲区`buffer`，开始位置是`offset`。`offset`可以是负数，从缓冲区末尾开始计数。

### **ustruct.unpack**(fmt, data)  
  按照字符串格式`fmt`解压数据`data`。 返回值是解压后参数的元组。

### **ustruct.unpack_from**(fmt, data, offset=0)  
  从 fmt 的 offset 开始解压数据，如果 offset 是负数就是从缓冲区末尾开始计算。 返回值是解压后参数元组。

更多的内容可参考  [struct](https://docs.python.org/3/library/struct.html) 。

----------