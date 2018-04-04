# MicroPython port for RT-Thread

MicroPython - a lean and efficient Python implementation for microcontrollers and constrained systems https://micropython.org

The IDE project is on https://github.com/armink/MpyOnRtt . This is only a software package for RT-Thread.

Let's get started by RT-Thread……

## Run MicroPython by Finsh/MSH

- input `pyhton` command will goto the MicroPython REPL(Read-Evaluate-Print-Loop)

```
msh />python

MicroPython v1.9.3-6-g1742ab26-dirty on 2017-11-04; RT-Thread Board with stm32f4
>>> 
>>> print('Hello World')
Hello World
```

- Now you can iuput and run the python code on the terminal
- If you want exit the python REPL. Please press `CTRL + D` .

## Supported modules

### RT-Thread

```
>>> import rtthread
>>> 
>>> rtthread.is_preempt_thread()       # determine if code is running in a preemptible thread
True
>>> rtthread.current_tid()             # current thread id
268464956
>>> rtthread.stacks_analyze()          # show thread information
thread     pri  status      sp     stack size max used left tick  error
---------- ---  ------- ---------- ----------  ------  ---------- ---
elog_async  31  suspend 0x000000a8 0x00000400    26%   0x00000003 000
tshell      20  ready   0x00000260 0x00001000    39%   0x00000003 000
tidle       31  ready   0x00000070 0x00000100    51%   0x0000000f 000
SysMonitor  30  suspend 0x000000a4 0x00000200    32%   0x00000005 000
timer        4  suspend 0x00000080 0x00000200    25%   0x00000009 000
>>> 
```

### Delay and timing

Use the [`time`](http://docs.micropython.org/en/latest/pyboard/library/utime.html#module-utime) module:

```
>>> import time
>>> 
>>> time.sleep(1)           # sleep for 1 second
>>> time.sleep_ms(500)      # sleep for 500 milliseconds
>>> time.sleep_us(10)       # sleep for 10 microseconds
>>> start = time.ticks_ms() # get value of millisecond counter
>>> delta = time.ticks_diff(time.ticks_ms(), start) # compute time difference
```

### machine - functions related to the hardware

See [machine](http://docs.micropython.org/en/latest/pyboard/library/machine.html).

```
>>> import machine
>>>
>>> machine.info()              # show information about the board
---------------------------------------------
RT-Thread
---------------------------------------------
total memory: 131048
used memory : 4920
maximum allocated memory: 5836
thread     pri  status      sp     stack size max used left tick  error
---------- ---  ------- ---------- ----------  ------  ---------- ---
elog_async  31  suspend 0x000000a8 0x00000400    26%   0x00000003 000
tshell      20  ready   0x0000019c 0x00001000    39%   0x00000006 000
tidle       31  ready   0x0000006c 0x00000100    50%   0x0000000b 000
SysMonitor  30  suspend 0x000000a8 0x00000200    32%   0x00000005 000
timer        4  suspend 0x0000007c 0x00000200    24%   0x00000009 000
---------------------------------------------
qstr:
  n_pool=0
  n_qstr=0
  n_str_data_bytes=0
  n_total_bytes=0
---------------------------------------------
GC:
  16064 total
  464 : 15600
  1=14 2=6 m=3
>>> machine.enable_irq()        # enable interrupt
>>> machine.disable_irq()       # disable interrupt, WARNING: this operation is dangerous
>>> machine.reset()        # hard reset, like push RESET button
```

#### Pins and GPIO

See [machine.Pin](http://docs.micropython.org/en/latest/pyboard/library/machine.Pin.html).

```
>>> from machine import Pin
>>> 
>>> p_out = Pin(("X1", 33), Pin.OUT_PP)
>>> p_out.value(1)              # set io high
>>> p_out.value(0)              # set io low
>>> 
>>> p_in = Pin(("X2", 32), Pin.IN, Pin.PULL_UP)
>>> p_in.value()                # get value, 0 or 1
```

#### I2C

See [machine.I2C](http://docs.micropython.org/en/latest/pyboard/library/machine.I2C.html).

`software I2C` :
```
>>> from machine import Pin, I2C
>>> clk = Pin(("clk", 43), Pin.OUT_OD)   # Select the 43 pin device as the clock
>>> sda = Pin(("sda", 44), Pin.OUT_OD)   # Select the 44 pin device as the data line
>>> i2c = I2C(-1, clk, sda, freq=100000) # create I2C peripheral at frequency of 100kHz
>>> i2c.scan()                        # scan for slaves, returning a list of 7-bit addresses
[81]                                  # Decimal representation
>>> i2c.writeto(0x51, b'123')         # write 3 bytes to slave with 7-bit address 42
3 
>>> i2c.readfrom(0x51, 4)             # read 4 bytes from slave with 7-bit address 42
b'X\x08\x105'
>>> i2c.readfrom_mem(0x51, 0x02, 1)   # read 1 bytes from memory of slave 0x51(7-bit),
b'\x12'                               # starting at memory-address 8 in the slave
>>> i2c.writeto_mem(0x51, 2, b'\x10') # write 1 byte to memory of slave 42,
                                      # starting at address 2 in the slave
```

`hardware I2C` :
```
>>> from machine import Pin, I2C
>>> i2c = I2C(0)           # create I2C peripheral at frequency of 100kHz
>>> i2c.scan()                        # scan for slaves, returning a list of 7-bit addresses
[81]                                  # Decimal representation
```

#### SPI

See [machine.SPI](http://docs.micropython.org/en/latest/pyboard/library/machine.SPI.html).

`software SPI` :
```
>>> from machine import Pin, SPI
>>> clk = Pin(("clk", 43), Pin.OUT_PP)
>>> mosi = Pin(("mosi", 44), Pin.OUT_PP)
>>> miso = Pin(("miso", 45), Pin.IN)
>>> spi = SPI(-1,500000,polarity = 0,phase = 0,bits = 8,firstbit = 0,sck = clk,mosi = mosi,miso = miso)
>>> print(spi)
SoftSPI(baudrate=500000, polarity=0, phase=0, sck=clk, mosi=mosi, miso=miso)
>>> spi.write("hello rt-thread!")
>>> spi.read(10)
b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
```

`hardware SPI` :
```
>>> from machine import SPI
>>> spi = SPI(50)
>>> print(spi)
SPI(device port : spi50)
>>> spi.write(b'\x9f')
>>> spi.read(5)
b'\xff\xff\xff\xff\xff'
>>> buf = bytearray(1)
>>> spi.write_readinto(b"\x9f",buf)
>>> buf
bytearray(b'\xef')
>>> spi.init(100000,0,0,8,1)     # Resetting SPI parameter
```

### **_thread** - multithreading support

```
>>> ###  press CTRL + E to enter paste mode

paste mode; Ctrl-C to cancel, Ctrl-D to finish
=== import _thread
=== import time
=== 
=== def testThread():
===     while True:
===         print("Hello from thread")
===         time.sleep(2)
=== 
=== _thread.start_new_thread(testThread, ())
=== while True:
===     pass

```

### **uos** – basic “operating system” services

See [uos](http://docs.micropython.org/en/latest/pyboard/library/uos.html).

```
>>> import uos
>>> uos.                        # Tab 
__name__        uname           chdir           getcwd
listdir         mkdir           remove          rmdir
stat            unlink          mount           umount
>>> uos.mkdir("rtthread")
>>> uos.getcwd()
'/'
>>> uos.chdir("rtthread")
>>> uos.getcwd()
'/rtthread'
>>> uos.listdir()
['web_root', 'rtthread', '11']
>>> uos.rmdir("11")
>>> uos.listdir()
['web_root', 'rtthread']
>>> 
```

### **usocket** – socketmodule

See [usocket](http://docs.micropython.org/en/latest/pyboard/library/usocket.html).

#### TCP Server

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
>>> 
```

#### TCP Client

```
>>> import usocket 
>>> s = usocket.socket(usocket.AF_INET,usocket.SOCK_STREAM)
>>> s.connect(("192.168.10.110",6000))  
>>> s.send("micropython")               
11
>>> s.close()
```

### Coming soon 
