# **urandom** - 随机数生成模块
!!! abstract "简介"
    `urandom` 模块实现了伪随机数生成器。

## 函数 

### **urandom.choice**()  
Return a random element from the non-empty sequence seq. If seq is empty, raises IndexError.

### **urandom.getrandbits**()  
Returns a Python integer with k random bits. This method is supplied with the MersenneTwister generator and some other generators may also provide it as an optional part of the API. When available, getrandbits() enables randrange() to handle arbitrarily large ranges.

### **urandom.randint**()  
Return a random integer N such that a <= N <= b. Alias for randrange(a, b+1).

### **urandom.random**()  
Return the next random floating point number in the range [0.0, 1.0).

### **urandom.randrange**()  
Return a randomly selected element from range(start, stop, step). This is equivalent to choice(range(start, stop, step)), but doesn’t actually build a range object.

### **urandom.seed**()  
Initialize the random number generator.
If a is omitted or None, the current system time is used. If randomness sources are provided by the operating system, they are used instead of the system time (see the os.urandom() function for details on availability).

### **urandom.uniform**()  
Return a random floating point number N such that a <= N <= b for a <= b and b <= N <= a for b < a.
The end-point value b may or may not be included in the range depending on floating-point rounding in the equation a + (b-a) * random().

更多内容可参考 [urandom](https://docs.python.org/3/library/random.html?highlight=random#module-random) 。

----------