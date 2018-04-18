# **ujson** – JSON编码与解码
`ujson` 模块提供 Python 对象到 JSON（JavaScript Object Notation） 数据格式的转换。

更多内容可参考 [json](https://docs.python.org/3/library/json.html?highlight=json#module-json)  。

`函数`

- ujson.dumps(obj)  
  返回 obj JSON字符串。

- ujson.loads(str)  
  解析 str 字符串并返回对象。如果字符串格式错误将引发 ValueError 异常。

----------