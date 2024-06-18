
main.c

```c
#include "platform.h"
#include "xgpiops.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xscugic.h"
#include <sleep.h>
#include <stdio.h>

#define MIO_LED 0
#define MIO_KEY 9

XScuGic scugic;
XGpioPs gpiops;

void gpio_intr_handler(void *data)
{
    XGpioPs *xgpiops = (XGpioPs *)data;

    if (XGpioPs_IntrGetStatusPin(xgpiops, MIO_KEY))
    {
        printf("GPIO INTR handler\r\n");
        XGpioPs_IntrClearPin(xgpiops, MIO_KEY);
    }
}

void intr_init(void)
{
    // 初始化通用中断控制器 (GIC)
    XScuGic_Config *cfg = XScuGic_LookupConfig(XPAR_SCUGIC_0_DEVICE_ID);
    XScuGic_CfgInitialize(&scugic, cfg, cfg->CpuBaseAddress);

    // 中断属于异常的一种, CPU 先触发的是异常处理函数, 然后由异常调用中断控制器的回调函数
    Xil_ExceptionInit();
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT, 
        (Xil_ExceptionHandler)XScuGic_InterruptHandler, &scugic);
    Xil_ExceptionEnable();

    // 注册 GPIO 中断回调并使能
    XScuGic_Connect(&scugic, XPAR_XGPIOPS_0_INTR, gpio_intr_handler, &gpiops);
    XScuGic_Enable(&scugic, XPAR_XGPIOPS_0_INTR);
}

void gpio_init(void)
{
    // 初始化 PS 端的 GPIO 控制器
    XGpioPs_Config *cfg = XGpioPs_LookupConfig(XPAR_XGPIOPS_0_DEVICE_ID);
    XGpioPs_CfgInitialize(&gpiops, cfg, cfg->BaseAddr);

    // 配置 LED 引脚为输出, 并使能引脚的输出功能
    XGpioPs_SetDirectionPin(&gpiops, MIO_LED, 1);
    XGpioPs_SetOutputEnablePin(&gpiops, MIO_LED, 1);

    // 配置 KEY 引脚为输入, 并使能引脚的中断源功能
    XGpioPs_SetDirectionPin(&gpiops, MIO_KEY, 0);
    XGpioPs_SetIntrTypePin(&gpiops, MIO_KEY, XGPIOPS_IRQ_TYPE_EDGE_FALLING);
    XGpioPs_IntrEnablePin(&gpiops, MIO_KEY);
}

int main()
{
    init_platform();

    gpio_init();

    intr_init();

    while (1)
    {
        print("LED ON\n\r");
        XGpioPs_WritePin(&gpiops, MIO_LED, 0);
        sleep(1);
        print("LED OFF\n\r");
        XGpioPs_WritePin(&gpiops, MIO_LED, 1);
        sleep(1);
    }

    cleanup_platform();
    return 0;
}
```
