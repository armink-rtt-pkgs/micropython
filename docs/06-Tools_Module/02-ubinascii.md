# **ubinascii** – 二进制/ ASCII转换
!!! abstract "简介"
    `ubinascii` 模块包含许多在二进制和各种 ascii 编码的二进制表示之间转换的方法。

## 函数

### **ubinascii.hexlify**(data[, sep])  
将二进制数据转换为十六进制表示。

### **Difference to CPython**  
If additional argument, sep is supplied, it is used as a separator between hexadecimal values.

### **ubinascii.unhexlify**(data)  
将十六进制数据转换为二进制表示。返回字节串 (换言之， 反二进制转换)

### **ubinascii.a2b_base64**(data)  
Base64编码的数据转换为二进制表示。返回字节串。

### **ubinascii.b2a_base64**(data)  
编码base64格式的二进制数据。返回的字符串。

更多内容可参考 [ubinascii](http://docs.micropython.org/en/latest/pyboard/library/ubinascii.html)  。

----------