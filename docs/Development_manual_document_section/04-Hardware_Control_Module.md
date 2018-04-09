# RT-Thread MicroPython 硬件控制模块

### Hardware Module

下面是 MicroPython 硬件相关的模块，运用这些模块，你可以使用 MicroPython 轻松的操纵硬件。

#### **machine** – 与硬件相关的功能
`machine` 模块包含与特定开发板上的硬件相关的特定函数。 在这个模块中的大多数功能允许实现直接和不受限制地访问和控制系统上的硬件块（如CPU，定时器，总线等）。如果使用不当，会导致故障，死机，崩溃，在极端的情况下，硬件会损坏。

更多内容可参考 [machine](http://docs.micropython.org/en/latest/pyboard/library/machine.html) 。

`复位功能`

- machine.reset()
重置设备的方式类似类似按下 rst 按钮。

- machine.reset_cause()
Get the reset cause. See constants for the possible return values.

`中断功能`

- machine.disable_irq()
Disable interrupt requests. Returns the previous IRQ state which should be considered an opaque value. This return value should be passed to the enable_irq function to restore interrupts to their original state, before disable_irq was called.

- machine.enable_irq(state)
Re-enable interrupt requests. The state parameter should be the value that was returned from the most recent call to the disable_irq function.

`有效的相关功能`

- machine.freq()
Returns CPU frequency in hertz.

- machine.idle()
Gates the clock to the CPU, useful to reduce power consumption at any time during short or long periods. Peripherals continue working and execution resumes as soon as any interrupt is triggered (on many ports this includes system timer interrupt occurring at regular intervals on the order of millisecond).

- machine.sleep()
Stops the CPU and disables all peripherals except for WLAN. Execution is resumed from the point where the sleep was requested. For wake up to actually happen, wake sources should be configured first.

- machine.deepsleep()
Stops the CPU and all peripherals (including networking interfaces, if any). Execution is resumed from the main script, just as with a reset. The reset cause can be checked to know that we are coming from machine.DEEPSLEEP. For wake up to actually happen, wake sources should be configured first, like Pin change or RTC timeout.

`example`:

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


`类`

- machine.Pin
更多内容可参考 [machine.Pin](http://docs.micropython.org/en/latest/pyboard/library/machine.Pin.html)  。

`example`:
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

- machine.I2C
更多内容可参考 [machine.I2C](http://docs.micropython.org/en/latest/pyboard/library/machine.I2C.html) 。

`software I2C example ` :
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

`hardware I2C example ` :
```
>>> from machine import Pin, I2C
>>> i2c = I2C(0)           # create I2C peripheral at frequency of 100kHz
>>> i2c.scan()                        # scan for slaves, returning a list of 7-bit addresses
[81]                                  # Decimal representation
```

- machine.SPI
更多内容可参考 [machine.SPI](http://docs.micropython.org/en/latest/pyboard/library/machine.SPI.html) 。

`software SPI example ` :
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

`hardware SPI example ` :
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

- machine.UART

----------