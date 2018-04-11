# **uhashlib** – 哈希算法
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