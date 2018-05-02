# **uctypes** – 以结构化的方式访问二进制数据

!!! abstract "简介"
    `uctypes` 模块是 MicroPython 的外函数库，它提供 C 兼容的数据类型。

## 常量
### **uctypes.LITTLE_ENDIAN**
Layout type for a little-endian packed structure. (Packed means that every field occupies exactly as many bytes as defined in the descriptor, i.e. the alignment is 1).

### **uctypes.BIG_ENDIAN**  
Layout type for a big-endian packed structure.

### **uctypes.NATIVE**  
Layout type for a native structure - with data endianness and alignment conforming to the ABI of the system on which MicroPython runs.

## 函数
### **uctypes.sizeof**(struct)  
Return size of data structure in bytes. Argument can be either structure class or specific instantiated structure object (or its aggregate field).

### **uctypes.addressof**(obj)  
Return address of an object. Argument should be bytes, bytearray or other object supporting buffer protocol (and address of this buffer is what actually returned).

### **uctypes.bytes_at**(addr, size)  
Capture memory at the given address and size as bytes object. As bytes object is immutable, memory is actually duplicated and copied into bytes object, so if memory contents change later, created object retains original value.

### **uctypes.bytearray_at**(addr, size)  
Capture memory at the given address and size as bytearray object. Unlike bytes_at() function above, memory is captured by reference, so it can be both written too, and you will access current value at the given memory address.

更多内容可参考 [ctypes](https://docs.python.org/3/library/ctypes.html?highlight=ctypes#module-ctypes) 。

----------