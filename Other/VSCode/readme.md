# VSCode 使用技巧


#### 1. C/C++ 开发中配置全局宏定义

在开发 C/C++ 工程时候可以设置 defines 让负责代码提示和补全的编译器知道工程中有哪些全局宏定义

比如 Keil 中定义的宏定义, VSCode 调用的本地编译器无法感知, 需要主动给出宏定义好让编译器根据全局宏定义进行更合理的分析.

可在工作目录下的 .vscode/c_cpp_properties.json 文件中配置, 也可在 .vscode/settings.json 中配置

c_cpp_properties.json

```json
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${default}",
                "${workspaceFolder}/**"
            ],
            "defines": [
                "USE_HAL_DRIVER",
                "STM32H7B0xx"
            ],
            "cStandard": "c17",
            "cppStandard": "gnu++17",
            "intelliSenseMode": "windows-gcc-x64"
        }
    ],
    "version": 4
}
```

settings.json

```json
{
    "files.associations": {
        "*.ui": "xml",
        "main.h": "c",
        "stm32h7xx_hal_ospi.h": "c",
    },
    "C_Cpp.default.includePath": [
        "${workspaceFolder}/**"
    ],
    "C_Cpp.default.defines": [
        "USE_HAL_DRIVER",
        "STM32H7B0xx"
    ]
}
```


#### 2. 使用 VSCode 开发 Linux 工程

直接使用[项目](https://github.com/amezin/vscode-linux-kernel)
[参考](https://stackoverflow.com/questions/49198816/how-to-use-the-visual-studio-code-to-navigate-linux-kernel-source)



## 插件种草

* drawio 方便绘制流程图
* github repo 方便通过远程查看各种开源项目源码，不需要下载下来
