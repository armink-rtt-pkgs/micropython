# machine.SPI  
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