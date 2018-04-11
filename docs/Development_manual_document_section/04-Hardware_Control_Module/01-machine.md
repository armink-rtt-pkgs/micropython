# **machine** – 与硬件相关的功能
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