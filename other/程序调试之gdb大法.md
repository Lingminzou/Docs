## 一、说说 gdb

gdb 是一款 UNIX 系统下基于命令行的程序调试利器，它功能强大，但其命令行的方式想必是挡住了一大波程序员，默默的继续使用 printf。

但作为 Linux 下的程序员应当是要掌握这一强大的调试利器，至少了解下吧，解 bug 的时候肯定用的上。

而且掌握它之后再也不用羡慕那些使用 IDE 开发自带图形化调试页面的同学啦，酷炫的命令行式的操作是不是会更有吸引力。

好了，安利了一波 gdb，那赶紧学起来吧。（其实我对 gdb 也是刚入门吧，哈哈~~）

## 二、开始 gdb 调试 

1、启动 gdb

	gdb ./exe

直接启动调试 exe。

	gdb ./exe <core dump file>

启动调试 exe 并带上 coredump file

	gdb ./exe PID

启动调试一个运行中的程序，带上其 PID

2、设置 so 的搜索路径（如果有用到的话）

	set sysroot PATH
	set solib-absolute-prefix PATH

这里 sysroot 和 solib-absolute-prefix 是等价的，意为查找绝对路径库的头部，

如 exe 有使用 /usr/lib 下面的库，则设置完 sysroot 后其搜索路径就变成了 /PATH/usr/lib，有点取代根目录的意思。

另外一个设置 so 搜索路径的参数是 set solib-search-path，与上不同的是它可以设置多个路径，用冒号间隔。

它可以用来应对一些使用相对路径的情况，如 dlopen 加载的一些库，设置合理可以取代 sysroot。

	set solib-search-path PATH
	set solib-search-path PATH1:PATH2

3、设置程序运行参数（如果有的话）

	set args param1 param2
	show args

即 main 函数的入口参数，使用空格间隔，show args 即查看设定好的参数。

## 三、gdb 调试进行时

1、控制运行状态

- run 或 r：使用程序运行，通常进入 gdb 后的第一条指令，遇到断点后停住，等待后续控制命令
- continue 或 c：继续执行到下一个断点处，程序要先 run 起来后使用它
- next 或 n：单步执行，遇函数时不进入
- step 或 s：单步执行，遇函数会进入
- until：用于进入循环体后直接运行程序直到循环体结束
- untiil + 行号：直接运行到某行处
- call fun()：直接调用函数 fun
- quit 或 q：退出 gdb

2、查看源码

- list 或 l：显示源码，每次 10 行
- list 行号：显示当前文件行号前后的源码
- list 函数名：显示函数名所在的源码

3、打印或观察变量的值

- print a 或 p a：打印 a 的值
- print fun_test(a)：将 a 作为参数调用 fun_test() 函数
- display a：当程序中断时打印 a 的值，常用于断点和单步调试
- watch a：设置观察的变量或表达式，当其值发生变化时中断程序
- whatis a：查询变量或函数
- info function：查询函数
- info locals：显示当前栈页的所有变量

4、查询运行信息

- bt ：打印当前的调用栈
- info program：查看程序的运行状态
- info threads：查看当前可调试的所有线程
- thread apply all bt：展示所以线程的栈

5、设置断点

- break n 或 b n：在第 n 行处设置断点
- b main.c:n：在 main.c 的第 n 行设置断点
- b func：在 func() 函数入口处设置断点
- info b：查看当前程序的断点情况
- delete 断点号n：删除第 n 个断点
- disable 断点号n：失能第 n 个断点
- enable 断点号n：使能第 n 个断点
- delete breakpoints：清除所有断点

## 四、gdb 远程调试

远程调试通常用于嵌入式程序开发当中，即程序编译在服务器端，而执行在目标板上（通常为 ARM 目标板）。

此时我们需要先在目标板上运行 gdbserver 如下

	gdbserver :7788 ./exe

即监听所有网卡的 7788 端口，并启动调试 exe 程序，或调试一个正在运行的程序

	gdbserver :7788 --attach pid

即为连接到进程号为 pid 的程序进行调试。

目标板上设置好了之后再到服务器上（通常是 Linux 服务器），先启动 gdb

	gdb exe

然后连接到目标板：

	target remote IP:7788

这里的 IP 则为目标板的 IP。

扫码关注我了解更多。

![](http://wx1.sinaimg.cn/large/9e169b75gy1fqcisgsbd7j2076076q3e.jpg)