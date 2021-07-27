# HEX LINK firmware

#### 介绍
可以根据自己的工具选择其中一个类型的工程下载：

**ArduinoIDE_Proj**中的工程需要使用Arduino IDE打开。

> 优点：软件操作简便
>
> 缺点：文件结构没有Vscode工程清晰、无代码颜色提示、函数无法跳转等等
>
> ***注意：******Additional_Libraries文件夹下的所有文件夹需要拷贝到Arduino IDE安装目录下的Libraries文件夹中***



**Vscode_PlatformIO_Proj**中的工程需要使用安装了PlatformIO插件的Vscode打开。

> 优点：文件结构清晰、代码提示、代码补全、函数跳转提示等功能应有尽有。
>
> 缺点：需要安装Vscode以及PlatformIO插件
>
> ***安装Vscode以及PlatformIO可以参考：***
>
> [Arduino--VS Code开发Arduino PlatformIO IDE_似水流年-CSDN博客](https://liefyuan.blog.csdn.net/article/details/114917179?utm_medium=distribute.pc_relevant_t0.none-task-blog-2~default~OPENSEARCH~default-1.control&dist_request_id=1331647.20492.16184600638122219&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2~default~OPENSEARCH~default-1.control)



**Bootloaders**中的两个hex是需要分别下载接收端与发送端的芯片中。

> Leonardo_atmega32u4.hex需要下载至接收端的atmega32U4芯片中。熔丝位配置：低位值FF-高位值D8-拓展位值CB
>
> Nano_atmega328.hex需要下载至发送端的atmega328P芯片中。          熔丝位配置：低位值FF-高位值DA-拓展位值FD

