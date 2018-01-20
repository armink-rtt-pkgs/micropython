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

### pyb - functions related to the board

See [pyb](http://docs.micropython.org/en/latest/pyboard/library/pyb.html).

```
>>> import pyb
>>>
>>> pyb.info()              # show information about the board
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
>>> pyb.enable_irq()        # enable interrupt
>>> pyb.disable_irq()       # disable interrupt, WARNING: this operation is dangerous
>>> time_start = pyb.millis()          # return the number of milliseconds
>>> pyb.elapsed_millis(time_start)     # calculate the elapsed time of milliseconds
2449
>>> time_start = pyb.micros()          # return the number of microseconds
>>> pyb.elapsed_micros(time_start)     # calculate the elapsed time of microseconds
1769000
>>> pyb.delay(1000)         # delay milliseconds
>>> pyb.udelay(1000*1000)   # delay microseconds
>>> pyb.hard_reset()        # hard reset, like push RESET button
```

#### Pins and GPIO

See [pyb.Pin](http://docs.micropython.org/en/latest/pyboard/library/pyb.Pin.html#pyb-pin).

```
>>> from pyb import Pin
>>> 
>>> p_out = Pin(("X1", 33), Pin.OUT_PP)
>>> p_out.value(1)              # set io high
>>> p_out.value(0)              # set io low
>>> 
>>> p_in = Pin(("X2", 32), Pin.IN, Pin.PULL_UP)
>>> p_in.value()                # get value, 0 or 1
```

#### uos– basic “operating system” services

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

#### usocket– socketmodule

See [usocket](http://docs.micropython.org/en/latest/pyboard/library/usocket.html).

##### TCP Server

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

##### TCP Client

```
>>> import usocket 
>>> s = usocket.socket(usocket.AF_INET,usocket.SOCK_STREAM)
>>> s.connect(("192.168.10.110",6000))  
>>> s.send("micropython")               
11
>>> s.close()
```

### Coming soon
