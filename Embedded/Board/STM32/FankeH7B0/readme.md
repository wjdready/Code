


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



