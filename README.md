## 简介

操作系统的课程设计，仿Win10自带任务管理器GUI风格的任务管理器，基于win32 API与Qt 6.2.2 GPLv3开源版. 

实现了显示所有进程的信息、创建和终止进程、显示每个用户创建的进程、显示CPU/RAM利用率等功能. 主要使用面对对象的开发方法.

这个任务管理器主要分为系统调用和GUI两个部分. 系统调用部分使用的是一个起封装作用的类`win32_common`，这个类的实现使用了Win32 API. 此封装类的使用体现了OOP中的封装性~~（废话）~~，同时起到缓冲区的作用，避免不必要的system call，提高性能.

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

## License

GPLv3

