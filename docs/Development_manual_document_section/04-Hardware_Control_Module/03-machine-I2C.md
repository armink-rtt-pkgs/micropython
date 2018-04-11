# machine.I2C  
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