
# [2023-03-09 16:19:59] 开坑

刚读完圈圈的 USB 教程，现在想来尝试 TinyUSB 开源库的使用

首先 clone 下 [源码](https://github.com/hathach/tinyusb) 吧。
了解工程框架：
### 根目录情况：
```
docs            文档
examples        示例
hw              包含各个芯片厂商的驱动和 BSP
lib             第三方库如 Fatfs, FreeRTOS
src             usb 核心源码
test            单元测试
tools           提供一些工具
```

#### docs 

文档是一个 wiki 页面的原文档，使用 .rst 编写。如果看文档的话直接转到已经生成的 [网页](https://docs.tinyusb.org/en/latest/index.html#) 效果更好，不过文档似乎不是很多。
从文档目录或 wiki 页面来看，文档就三部分：
* info 信息
  * User 列出 TinyUSB 目前正被哪些其他项目使用，发现 Espressif IDF 用的是这个，还有 `Raspberry Pi Pico SDK` 和 `MicroPython` 这些都是用过的项目嘛。以前没注意。
  * Changelog 更改日志
  * Contributors 贡献者
* contributing 参与共享
  * Code of Conduct 行为守则，介绍开源项目的一些准则
  * Structure 结构，列出项目结构 （然而并不详细）
  * Porting 移植，给出了移植的一些建议  （重点看这部分）
* reference 参考
  * Supported Devices 支持设备，列出当前直接支持的设备 （支持的设备基本可以直接使用，不支持的话需要自己移植吧。..)
  * Getting Started 入门，介绍将 TinyUSB 添加到自己的项目  （重点看这部分）
  * Concurrency 并发性，对中断和 USB 上下文的一些介绍和建议。..

OK 重点就看入门介绍和移植部分，不过先运行一个示例再说。..

#### examples

基本给出的是应用层的代码，基本模式都是包含头文件，初始化，然后在循环中执行任务，和 LVGL 很类似呢，其实很多库都是这样的结构，初始化然后需要在循环中一直执行某个函数来处理功能。..

```c
#include "bsp/board.h"
#include "tusb.h"

static void cdc_task(void);

int main(void)
{
    board_init();
    tud_init(BOARD_TUD_RHPORT);

    while (1)
    {
        tud_task();
        cdc_task();
    }

    return 0;
}
```

然后我发现 examples 里面才有构建脚本，外面的 src 代码是没有构建脚本的。视角转到 examples, 把它看作主文件夹，src、lib 不过是附加的一样。
其他源代码功能都是很清楚了，不需要单独列出。..

# [2023-03-09 18:58:25]

尝试运行它所支持的 STM32, 刚好有一块 ART-Pi 开发板，在 [入门](https://docs.tinyusb.org/en/latest/reference/getting_started.html) 教程中给出了详细的说明。
### 运行示例

先确保其他模块被加载进来:

# 2023-09-21 08:57:47 (新的版本该方法已经废弃, 使用 get-deps)

```
git submodule update --init lib
```

支持的芯片和板子其实在 ` hw/bsp` 目录中可以看到, 例如我的 Art-Pi 是 H750 的, 查看支持哪些 STM32H7 的

```
ls hw/bsp/stm32h7/boards
daisyseed  stm32h723nucleo  stm32h743eval  stm32h743nucleo  stm32h745disco  waveshare_openh743i
```
看起来好像没有 H750 的, 但实际上 daisyseed 就是 H750 的官方开发板，因此就用它作为例子啦:

先来个 cdc:

```
cd examples/device/cdc_msc
make BOARD=daisyseed
```

编译输出到了 _build 文件夹, 那么怎么烧写呢?

# [2023-09-21 08:58:07]

通过 `get-deps` 获取构建所需依赖, 例如

```sh
make BOARD=daisyseed  get-deps
```
