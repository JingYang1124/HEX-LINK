# HEX LINK - *A Distributed Somatosensory Interaction Device*  

#### ![version: 1.2 (shields.io)](https://img.shields.io/badge/version-1.2-brightgreen)

#### 介绍
**HEX LINK**: **H**igher-order **EX**cess **LINK**

**这是一套可适用于PC端游戏的体感操作设备。**

![](https://jingyang1124.oss-cn-shanghai.aliyuncs.com/img/DIY_Project/HEX_LINK/Hex_Link_ASM%2B4.JPG)

项目演示视频链接：  

[【开源·自制】真正的体感"只狼"_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1qy4y1M7Zw)

[【自制】"真 · 脚动档"极品飞车--程序员的健身日常_哔哩哔哩_bilibili](https://www.bilibili.com/video/BV1m54y1L7WU)

本仓库文件夹结构： 

```hxml
.
├─firmware # 软件（根据自己使用的工具选择如下一种工程即可）
│  ├─ArduinoIDE_Proj # Arduino IDE版工程 
│  │  ├─Additional_Libraries # 里面的文件夹需要复制到Arduino IDE安装目录下的libraries文件夹
│  │  ├─Hex_Link_Leonardo # 需要下载至接收端的程序
│  │  └─Hex_Link_Nano # 需要下载至发送端的程序
│  └─Vscode_PlatformIO_Proj # VScode PlatformIO版工程 
│      ├─Hex_Link_Leonardo # 需要下载至接收端的程序
│      └─Hex_Link_Nano # 需要下载至发送端的程序
├─hardware # 硬件（PCB工程）
│  ├─Hex_Link_Rec # 接收端PCB工程
│  └─Hex_Link_Trans # 发送端PCB工程
├─model # 接收端外壳3维模型
│  ├─Solidworks_Project # 2018版本Solidworks工程
│  └─STL_TAP # STL文件，可直接用于3D打印.TAP文件是顶层亚克力板的CNC加工文件
├─references # 参考文档
└─tools # 额外的脚本工具
```

