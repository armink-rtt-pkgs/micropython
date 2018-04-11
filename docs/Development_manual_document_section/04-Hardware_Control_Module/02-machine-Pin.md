# machine.Pin  
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