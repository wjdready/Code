


# [2023-04-14 10:22:58]


## LTDC 

[参考](https://blog.csdn.net/Ningjianwen/article/details/92781418)

LCD 基础知识

LCD显示有两个主要时间脉冲 -- 垂直同步脉冲 VSYNC 和水平同步脉冲 HSYNC，分别用于控制LCD的帧显示与行显示

垂直同步脉冲有三部分组织:

1）垂直同步脉冲开始时序 - Vertical back porch（VBP）表示垂直同步脉冲开始到一帧的有效像素数据开始前的一段时序，也表示有效像素数据开始时不显示的行数。
2）垂直同步脉冲帧时序 - Vertical active line（VACTL）表示一帧的有效像素数据开始前到一帧结束的时序，也表示有效像素数据行数。
3）垂直同步脉冲结束时序 - Vertical front porch（VFP）表示一帧的有效像素数据开始结束后到下一帧同步脉冲开始前的时序，也表示有效像素数据结束后不显示的行数。

水平同步脉冲有三部分组织:

1）水平同步脉冲开始时序 - Horizontal back porch（HBP）表示水平同步脉冲开始到一行的有效像素数据开始前的一段时序，也表示有效像素数据开始时不显示的像素个数。
2）水平同步脉冲行时序 - Horizontal active line（HACTL）表示一行的有效像素数据开始前到一行结束的时序，也表示有效像素数据像素个数。
3）水平同步脉冲结束时序 - Horizontal front porch（HFP）表示一行的有效像素数据开始结束后到下一行同步脉冲开始前的时序，也表示有效像素数据结束后不显示的像素个数。

DE 数据时钟
DCLK 像素驱动时钟

https://www.bilibili.com/read/cv15545329


# [2023-04-17 09:49:59]

LTDC 调试总结:

1. 进入 HardFault 很可能是因为 DMD2D 时钟没打开, 需要执行 __HAL_RCC_DMA2D_CLK_ENABLE(); 进行打开。另外如果打开 Cache 但是没配置 MPU, 导致 DMA 和 CPU 同时对内存进行读写也会出现该出现问题，这时可以把 DMA 操作的内存部分设置为 Write Throgh。好像很多 HardFault 都和 DMA 有关...
2. 显示屏都不亮，至少也应该花屏吧。这种情况调试很久才发现是因为自己 LTCDC GPIO配置为低速导致的，服了啊。
3. 用 fanke 那个例程配置完成后还需设置 	LCD_SetLayer(0); 后使用 DMA2D 才不会出错，因为里面进行一些 DMA2D 需要的配置，主要就是帧图像的内存地址。

# [2023-04-17 09:50:43]

LTDC 调试总结:

4. 已经在 RTThread 中运行 LTDC, 但是发现容易花屏, 后来发现是 MPU 配置和分散加载文件有问题，导致其它程序写入显存区域，显存区域存在脏数据。

# [2023-04-17 13:07:01]

想着通过 DMA2D 来加速 LVGL 显示的，但是在 lv_conf.h 中打开了 LV_USE_GPU_STM32_DMA2D 文字无法显示了，目前不知到原因。

另外，在 Keil 中打开优化， 不知为何 i2c 读取就会特别慢，只能放弃优化。后续还需要找原因。

