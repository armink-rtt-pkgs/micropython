# **uos** – 基本的"操作系统"服务

!!! abstract "简介"
    `uos` 模块包含了对文件系统的访问操作，是对应 CPython 模块的一个子集。

更多内容可参考 [uos](http://docs.micropython.org/en/latest/pyboard/library/uos.html) 。

`函数`

- uos.chdir(path)  
  更改当前目录。

- uos.getcwd()  
  获取当前目录。

- uos.ilistdir([dir])  
  这个函数返回一个迭代器，然后产生三元组对应正在列出的目录中的条目。没有参数，它列出了
  当前目录，否则它列出了目录给出的`dir`。

- 3-元组的形式`(name, type, inode)`:
  name 是一个字符串（或字节，如果是一个字节对象），是输入的名称；
  type 是一个整数，指定的条目类型，与普通文件和目录0x4000 0x8000；
  inode 对应文件的inode的整数，可0的文件系统，没有这样的概念。
  uos.listdir([dir])
  没有参数，列出当前目录。否则列出给定目录。

- uos.mkdir(path)  
  创建一个目录。

- uos.remove(path)  
  删除文件。

- uos.rmdir(path)  
  删除目录。

- uos.rename(old_path, new_path)  
  重命名文件。

- uos.stat(path)  
  获取文件或目录的状态。

- uos.sync()  
  同步所有的文件系统。

`example`:
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

----------