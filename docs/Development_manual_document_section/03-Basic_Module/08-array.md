# **array** – 数字数据数组

!!! tip "简介"
    ``array``  模块定义了一个对象类型，它可以简洁地表示基本值的数组：字符、整数、浮点数。

- 支持代码格式: b, B, h, H, i, I, l, L, q, Q, f, d (后2个支持浮点数)。

## 类

- class array.array(typecode[, iterable])  
  指定类型创建数组元素。用可选项［］做为数组的初始值，可选项［］未指定的，则创建空数组。

- append(val)  
  将新元素添加到数组的结尾，并将其扩展。

- extend(iterable)  
  使用迭代方式将新元素添加到数组的结尾，并将其扩展。
  
更多内容可参考  [array](https://docs.python.org/3/library/array.html) 。

----------