## 简介

操作系统的课程设计，仿Win10自带任务管理器GUI风格的任务管理器，基于win32 API与Qt 6.2.2 GPLv3开源版. 主要使用面对对象的开发方法.

这个任务管理器主要分为系统调用和GUI两个部分. 系统调用部分使用的是一个起封装作用的类`win32_common`，这个类的实现使用的是win32 API. 这个封装类的使用体现了OOP中的封装性（~~废话~~），同时起到缓冲区的作用，避免不必要的system call，提高性能.

### 功能

主要有这些功能：

- 显示所有进程的各项信息，包括PID、名称、图标、线程数量、占用内存、优先级、所属用户
- 创建和终止进程，创建进程时可以传递命令行参数
- 显示每个用户创建的进程，可按用户折叠
- 显示CPU的型号、逻辑内核数量，RAM的大小
- 在图表中显示CPU在60s内各个内核的利用率以及平均利用率，显示RAM在60s内的利用率
- 关机和注销

## 目录结构

 - [source/](https://github.com/ekv0/my-boring-os-course-design/tree/master/source) 源代码
 - [media/](https://github.com/ekv0/my-boring-os-course-design/tree/master/media) 一些截图
 - [dia/](https://github.com/ekv0/my-boring-os-course-design/tree/master/dia) 使用[drawio](https://github.com/jgraph/drawio-desktop)绘制的一些图
 - [report/](https://github.com/ekv0/my-boring-os-course-design/tree/master/report) 当时的课设报告
 - [slide/](https://github.com/ekv0/my-boring-os-course-design/tree/master/slide) 当时的展示用幻灯片

## 界面展示

全部截图可见[media/](https://github.com/ekv0/my-boring-os-course-design/tree/master/media)

<img src="./media/tabs/perfTab/cpu.png" alt="cpu" width="400" />

<img src="./media/tabs/perfTab/mem.png" alt="mem" width="400" />

<img src="./media/tabs/detailTab/listView/rightclick dialog.png" alt="rightclick dialog" width="400" />

<img src="./media/tabs/userTab/usertab expanded.png" alt="usertab expanded" width="400" />

<img src="./media/menubar/new proc.png" alt="new proc" width="400" />

<img src="./media/menubar/shutdown dialog.png" alt="shutdown dialog" width="400" />

## 进行过的优化

原来的程序结构是每个GUI组件类都拥有一个前述的系统调用封装类`win32_common`，实际运行起来以后发现非常卡顿，每次刷新需要的时间甚至超过了1s的默认的刷新间隔，用鼠标拖动窗口时窗口要过一会儿才移动到新的位置。

使用orbit进行profiling后，发现运行时间最长的是dll是`ntdll.dll`，它的运行时长比排第二的`win32u.dll`高了十几倍。检查这两个dll的调用栈，发现`ntdll.dll`调用的很多是与系统有关的函数，例如`NtReadVirtualMemory` `EnumProcessModules`等；而`win32u.dll`的调用栈则都是绘图函数。联系这两个dll占用时间的差别，可以推测系统调用是程序的性能瓶颈。

因此试验了改为多线程，以及将每个GUI组件类保留的系统调用封装类的实例改为指针这两种方法。

多线程有一定效果，但不太明显，而且Qt的GUI由于多线程产生了撕裂的现象。而将实例改为指针的效果很好，所以最后修改了程序的结构，将系统调用封装类的实例限制为一个，所有类只保留指针。（当时不懂设计模式，现在看来这种情况非常适合使用单例模式）

优化前的profile截图没有保存，以下是优化后的profile和前两个dll的调用栈。可以看出系统调用仍然占程序运行时长的最大比重：

<img src="./media/performance analyze/14.878s sampling result.png" alt="shutdown dialog" width="800" />

`ntdll.dll`调用栈：

<img src="./media/performance analyze/14.878s sampling result 1 callstuck.png" alt="shutdown dialog" width="800" />

`win32u.dll`调用栈：

<img src="./media/performance analyze/14.878s sampling result 2 callstuck.png" alt="shutdown dialog" width="800" />

## License

GPLv3

