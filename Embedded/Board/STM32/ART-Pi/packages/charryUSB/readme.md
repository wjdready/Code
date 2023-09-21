---
mytags: myblog
title: RTThread 上使用 CharryUSB packages
date: 2023-09-21 16:19:02
categories: [RTThread, packages]
tags: [RTThread, USB]
---

本文主要介绍如何在 RTThread 上使用 CharryUSB
<!-- more -->

#### 介绍

CharryUSB 试用了一下，感觉还不错，本身实现了多种 USB IP，支持多个 MCU. 许多 MCU 厂商对于 USB 的实现都是直接买现成的 IP，例如 stm32 是的 dwc2, 因此各个厂商的 USB 驱动可以使用同一套 IP 的驱动，这就是为什么 RTThread menuconfig 选项中有选择 IP 选项。

#### 配置

那么开始配置 menuconfig:

```
RT-Thread online packages  --->
    system packages  ---> 
        CherryUSB: tiny and portable USB host/device stack for embedded system with USB IP  ---> 
             Enable usb device mode  --->
```

然后 Select usb device ip 记得选择 DWC2, 其他根据需要进行选择. 例如这里选择了 `Enable usb msc device  --->` 并且使用 ram 模拟磁盘 `Use msc ram template`

设置完成后 `rtconfig.h`` 新增如下内容:

```c
#define PKG_USING_CHERRYUSB
#define PKG_CHERRYUSB_DEVICE
#define PKG_CHERRYUSB_DEVICE_FS
#define PKG_CHERRYUSB_DEVICE_DWC2
#define PKG_CHERRYUSB_DEVICE_DWC2_STM32
#define PKG_CHERRYUSB_DEVICE_DWC2_PORT_FS
#define PKG_CHERRYUSB_DEVICE_MSC
#define PKG_CHERRYUSB_DEVICE_MSC_TEMPLATE
#define PKG_USING_CHERRYUSB_LATEST_VERSION
```

> 注意: 不需要在  RT-Thread Components --> Device Drivers  ---> 中再启动 usb 驱动了, 因为 CharryUSB 直接集成 DWC2 驱动, RT-Thread 默认的 USB 驱动关闭就好了

设置完成后执行 `pkgs --update` 拉取 CharryUSB 包。

#### 使用

拉取完成后似乎直接就能编译, 那么怎么使用呢？程序执行入口在哪? 启动流程是什么样的？

CharryUSB 唯一需要提供的是一个 `usb_dc_low_level_init` 函数, 这个函数是用于将外设时钟使能以及和 IO 绑定的操作，实际上就等价于 cubemx 自动生成代码中 xxx_msp 函数, 因此如果使用 cubemx 自动生成则可以直接拿来使用。比如 

cubemx 生成代码如下:

```c
void HAL_PCD_MspInit(PCD_HandleTypeDef* hpcd)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(hpcd->Instance==USB_OTG_FS)
  {
  /* USER CODE BEGIN USB_OTG_FS_MspInit 0 */

  /* USER CODE END USB_OTG_FS_MspInit 0 */
  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
  /** Enable USB Voltage detector
  */
    HAL_PWREx_EnableUSBVoltageDetector();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USB_OTG_FS GPIO Configuration
    PA12     ------> USB_OTG_FS_DP
    PA11     ------> USB_OTG_FS_DM
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
    /* USB_OTG_FS interrupt Init */
    HAL_NVIC_SetPriority(OTG_FS_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
  /* USER CODE BEGIN USB_OTG_FS_MspInit 1 */

  /* USER CODE END USB_OTG_FS_MspInit 1 */
  }
}
```

可以发现该函数 hpcd 其实没大多用, 去掉即可, 于是有:

```c
void usb_dc_low_level_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    /* USER CODE BEGIN USB_OTG_FS_MspInit 0 */

    /* USER CODE END USB_OTG_FS_MspInit 0 */
    /** Initializes the peripherals clock
     */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        Error_Handler();
    }
    /** Enable USB Voltage detector
     */
    HAL_PWREx_EnableUSBVoltageDetector();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USB_OTG_FS GPIO Configuration
    PA12     ------> USB_OTG_FS_DP
    PA11     ------> USB_OTG_FS_DM
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
    /* USB_OTG_FS interrupt Init */
    HAL_NVIC_SetPriority(OTG_FS_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
    /* USER CODE BEGIN USB_OTG_FS_MspInit 1 */

    /* USER CODE END USB_OTG_FS_MspInit 1 */
}
```


执行流程如下，首先注册设备描述符, 添加接口, 然后执行 `usbd_initialize` 初始化 usb

例如 msc 设备:

```c
void msc_ram_init(void)
{
    usbd_desc_register(msc_ram_descriptor);
    usbd_add_interface(usbd_msc_init_intf(&intf0, MSC_OUT_EP, MSC_IN_EP));

    usbd_initialize();
}
```

而 msc_ram_init 我们可以放到 shell 中手动执行，也可以设置开机后自动执行.

usbd_initialize 会调用 usb_dc_init, 而 usb_dc_init 是移植到不同平台需要实现的部分, 对于 dwc2 平台, CharryUSB 已经有实现:


```c
// core/usbd_core.c
int usbd_initialize(void)
{
    return usb_dc_init();
}

// port/dwc2/usb_dc_dwc2.c
__WEAK void usb_dc_low_level_init(void)
{
}

__WEAK void usb_dc_low_level_deinit(void)
{
}

int usb_dc_init(void)
{
  /*!< dc init */
  usb_dc_low_level_pre_init();

  memset(&usb_dc_cfg, 0, sizeof(usb_dc_cfg));
  /*!< Clear USB Event Interrupt */
  NRF_USBD->EVENTS_USBEVENT = 0;
  NRF_USBD->EVENTCAUSE |= NRF_USBD->EVENTCAUSE;

  /*!< Reset interrupt */
  NRF_USBD->INTENCLR = NRF_USBD->INTEN;
  NRF_USBD->INTENSET = USBD_INTEN_USBRESET_Msk | USBD_INTEN_USBEVENT_Msk | USBD_INTEN_EPDATA_Msk |
                       USBD_INTEN_EP0SETUP_Msk | USBD_INTEN_EP0DATADONE_Msk | USBD_INTEN_ENDEPIN0_Msk | USBD_INTEN_ENDEPOUT0_Msk | USBD_INTEN_STARTED_Msk;

  usb_dc_low_level_post_init();
  return 0;
}
```

因此实际需要实现的函数就仅剩下 usb_dc_low_level_pre_init 了. 在 port/dwc2/usb_dc_dwc2.c 中 usb_dc_low_level_init 默认为 weak 这也就是为何即使一开始没实现该函数也能编译成功。

至于 msc_ram_init 函数, 因为勾选了 Use msc ram template

在 `third_party/rt-thread-4.1.1/msh_cmd.c` 命令也给我们导出来了

```c
#if defined(PKG_CHERRYUSB_DEVICE_MSC_TEMPLATE)
void msc_ram_init(void);
MSH_CMD_EXPORT(msc_ram_init, start msc_ram_init);
#endif
```

下载后通过命令执行 `msc_ram_init` 即可.

