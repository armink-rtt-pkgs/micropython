# **usocket** – 套接字模块
!!! abstract "简介"
    `usocket` 模块提供对BSD套接字接口的访问。 

## 常数

### **socket.AF_INET**  
### **socket.AF_INET6**  

`TCP类型定义`

### **socket.SOCK_STREAM**  
### **socket.SOCK_DGRAM**  

`Socket类型`

### **socket.IPPROTO_UDP**  
### **socket.IPPROTO_TCP**  

`IP协议号`

### **socket.SOL_***  
Socket option levels (an argument to setsockopt()). The exact inventory depends on a board.

### **socket.SO_***  
Socket options (an argument to setsockopt()). The exact inventory depends on a board.

### **socket.IPPROTO_SEC**  
Special protocol value to create SSL-compatible socket.

## 函数

### **socket.socket**(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP)  
创建新的套接字，使用指定的地址、类型和协议号。

### **socket.getaddrinfo**(host, port)  
传递 主机/端口 到一个5个数据的元组。元组列表的结构如下:
(family, type, proto, canonname, sockaddr)

### **socket.close**()  
关闭套接字。一旦关闭后，套接字所有的功能都将失效。远端将接收不到任何数据 (清理队列数据后)。 在回收垃圾时套接字会自动关闭，但还是推荐在必要时用 close() 去关闭，或, or to use a with statement around them。

### **socket.bind**(address)  
将套接字绑定到地址，套接字不能是已经绑定的。

### **socket.listen**([backlog])  
允许服务器接收连接。如果指定了 backlog，它不能小于0 (如果小于0将自动设置为0)；超出后系统将拒绝新的连接。如果没有指定，将使用默认值。

### **socket.accept**()  
接收连接。套接字需要指定地址并监听连接。返回值是 (conn, address)，其中conn是用来接收和发送数据的套接字，address是绑定到另一端的套接字。

### **socket.connect**(address)  
连接到指定地址的远端套接字。

### **socket.send**(bytes)  
发送数据。套接字需要已连接到远程。

### **socket.sendall**(bytes)  
发送数据。套接字已连接到远程。 Unlike send(), this method will try to send all of data, by sending data chunk by chunk consecutively.

### **socket.recv**(bufsize)  
接收数据，返回值是数据字节对象。bufsize是接收数据的最大数量。

### **socket.sendto**(bytes, address)  
发送数据。套接字没有连接到远程，目标套接字由地址参数指定。

### **socket.recvfrom**(bufsize)  
接收数据。返回值是 (bytes, address)，其中 bytes 是字节对象，address 是发送数据的套接字。

### **socket.setsockopt**(level, optname, value)  
设置套接字参数。需要的符号常数定义在套接字模块 (SO_* 等)。value 可以是整数或字节对象。

### **socket.settimeout**(value)  
设置阻塞套接字超时时间。value 参数可以是代表秒的正浮点数或 None。如果设定大于 0 的参数，在后面套接字操作超出指定时间后将引起 timeout 异常。如果参数是 0，套接字将使用非阻塞模式。如果是 None，套接字使用阻塞模式。

### **socket.setblocking**(flag)  
设置阻塞或非阻塞模式: 如果 flag 是 false，设置非阻塞模式。

### **socket.read**([size])  
Read up to size bytes from the socket. Return a bytes object. If size is not given, it reads all data available from the socket until EOF; as such the method will not return until the socket is closed. This function tries to read as much data as requested (no “short reads”). This may be not possible with non-blocking socket though, and then less data will be returned.

### **socket.readinto**(buf[, nbytes])  
Read bytes into the buf. If nbytes is specified then read at most that many bytes. Otherwise, read at most len(buf) bytes. Just as read(), this method follows “no short reads” policy.
Return value: number of bytes read and stored into buf.

### **socket.readline**()  
读取一行，以换行符结束，返回读取的数据行。

### **socket.write**(buf)  
Write the buffer of bytes to the socket. This function will try to write all data to a socket (no “short writes”). This may be not possible with a non-blocking socket though, and returned value will be less than the length of buf.
Return value: number of bytes written.

## 示例

### TCP Server example

```
>>> import usocket 
>>> s = usocket.socket(usocket.AF_INET,usocket.SOCK_STREAM)  # Create STREAM TCP socket
>>> s.bind(('192.168.12.32', 6001))   
>>> s.listen(5)
>>> s.setblocking(True)
>>> sock,addr=s.accept()              
>>> sock.recv(10)                    
b'rt-thread\r'
>>> s.close()
```

### TCP Client example

```
>>> import usocket 
>>> s = usocket.socket(usocket.AF_INET,usocket.SOCK_STREAM)
>>> s.connect(("192.168.10.110",6000))  
>>> s.send("micropython")               
11
>>> s.close()
```

`connect to a web site example`:
```
s = socket.socket()
s.connect(socket.getaddrinfo('www.micropython.org', 80)[0][-1])
```

更多的内容可参考 [usocket](http://docs.micropython.org/en/latest/pyboard/library/usocket.html) 。

----------