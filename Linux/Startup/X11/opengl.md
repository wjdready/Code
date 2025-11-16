

嵌入式SOC芯片中的GPU品牌和对应的底层驱动: https://blog.csdn.net/zyy29182918/article/details/144185457
Mali GPU: https://blog.csdn.net/u011956078/article/details/118158516
Mali GPU调试: https://blog.csdn.net/szembed/article/details/148927220

常见的 OpenGL 库

libGLdispatch.so : OpenGL命令分发库, 负责将OpenGL调用路由到正确的驱动实现
libGL.so : OpenGL核心库，提供主要的OpenGL 1.x-4.x API功能, 传统桌面OpenGL实现的基础
libGLX.so : OpenGL与X Window系统的集成库, 提供GLX扩展，用于X11环境下的OpenGL渲染
libGLU.so : 提供辅助函数(如矩阵操作、曲面细分等), 已逐渐被现代OpenGL替代
libEGL.so : 用于管理OpenGL ES/OpenGL与本地窗口系统的接口, 替代GLX在移动/嵌入式设备上的角色
libGLESv2.so : OpenGL ES 2.0/3.x实现, 移动设备和嵌入式系统的主要图形API
libGLESv1_CM.so : OpenGL ES 1.x Common Profile实现, 固定功能管线，已被可编程着色器取代
libOpenGL.so : 现代OpenGL的另一种实现方式, 通常作为libGL.so的替代或补充


## 测试 opengl 

```sh
# mesa-utils:
# 1. glxinfo 查看 OpenGL 信息, 可用来查看是否有硬件驱动，是软件渲染还是硬件渲染
# 2. glxgears 测试 OpenGL 性能

# glmark2:
# 1. glmark2 测试 OpenGL 性能
```


