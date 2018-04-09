# RT-Thread MicroPython 工具模块
###  **Tools Module**

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