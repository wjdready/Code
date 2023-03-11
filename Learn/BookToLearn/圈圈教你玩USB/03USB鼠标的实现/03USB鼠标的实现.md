---
mytags: myblog
title: 03USB鼠标的实现
date: 2023-03-09 08:16:29
categories: [Book, 圈圈教你玩USB]
tags: [USB]
---

本文主要介绍
<!-- more -->

# USB 中断的处理过程:

一般来说当 USB 接口芯片（或片内外设）完成一个操作后就会产生一个中断请求信号, 以通知 CPU 来进行相关处理. 导致中断发生的事件有 USB 总线复位, 进入挂起状态, 成功接收到数据和发送完成等，具体的中断类型可查询USB接口芯片的中断相关寄存器来判断。

**例子**:

假设有一个USB接口芯片（或片内外设）的中断类型寄存器如下:

|  状态位  |   7   |    6     |     5     |     4     |     3     |     2     |     1     |     0     |
| :------: | :---: | :------: | :-------: | :-------: | :-------: | :-------: | :-------: | :-------: |
| 上电默认 |   0   |    0     |     0     |     0     |     0     |     0     |     0     |     0     |
| 中断类型 | 挂起  | 总线复位 | 端点2输入 | 端点2输出 | 端点1输入 | 端点1输入 | 端点0输出 | 端点0输出 |

当其中某位的值为1，则表示该中断源发出了中断请求。由此可以在中断函数中对每一种中断源进行处理:

```c
/* USB 中断处理 */
void USB_IRQHandler(void)
{
    // 获取中断类型
    uint8_t type = GET_USB_ISR_TYPE();

    if(type & (1 << 7)) usb_bus_suspend(); // 总线挂起中断处理
    if(type & (1 << 6)) usb_bus_reset();   // 总线复位中断处理
    if(type & (1 << 5)) usb_end2_in();     // 端点2输入中断处理
    if(type & (1 << 4)) usb_end2_out();    // 端点2输出中断处理
    if(type & (1 << 3)) usb_end1_in();     // 端点1输入中断处理
    if(type & (1 << 2)) usb_end1_out();    // 端点1输出中断处理
    if(type & (1 << 1)) usb_end0_in();     // 端点0输入中断处理
    if(type & (1 << 0)) usb_end0_out();    // 端点0输出中断处理

    // 中断标志位清空, 有些硬件会在读取寄存器后自动清空
    USB_ISR_FLAG_CLEAR();
}
```

根据前面介绍的 USB 枚举过程可知, USB 主机在枚举过程会先对设备进行复位, 然后对设备的端点 0 进行控制传输, 以获取设备描述符. 

这时会最先触发复位中断，之后再触发端点 0 的输出中断，表示 USB 芯片接收到了主机发来的数据, 因此可以在端点 0 的中断处理函数中将数据读取出来并做相应的处理:


```c
void usb_end0_out(void)
{
    // 将端点0的数据读取到内存
    USB_READ_ENDP_BUF(ENDP0, buf);

    // 这里是对硬件缓冲区进行清理, 因为一些芯片若缓冲区不清理
    // 则下次接收数据包时 (一般 SETUP 包除外, 因为设备必须接收) 
    // 硬件会直接向主机回复 NAK
    USB_ENDP_BUF_CLEAR(ENDP0);
}
```

## USB 标准请求

USB协议定义了一个8字节的标准设备请求，主要用在设备的枚举过程中。这8字节的数据是在控制传输的建立过程通过默认控制端点0发出的。

在这8字节的数据中，包含了数据过程所需要传输数据传输的方向、长度以及数据类型等信息。正是由于8字节标准请求的原因，USB协议规定，端点0的最大包长度至少为8字节。也就是说，任何一个USB设备都能够（而且必须要）接收8字节的标准请求。

### 1. USB 标准请求的结构


| 偏移 | 域            | 大小 |     取值     | 描述                               |
| :--: | ------------- | :--: | :----------: | ---------------------------------- |
|  0   | bmRequestType |  1   |     位图     | 请求的特性                         |
|  1   | bRequest      |  1   |     数值     | 请求代码                           |
|  2   | wValue        |  2   |     数值     | 该域的意义由具体的请求决定         |
|  4   | wIndex        |  2   | 索引或偏移量 | 该域的意义由具体的请求决定         |
|  6   | wLength       |  2   |    字节数    | 数据过程(如果有)所需要传输的字节数 |

其中 `bmRequestType` 请求特性字节各个位含义如下

| D7 数据传输方向 | D[6:5] 请求的类型 | D[4:0] 请求的接收者 |
| --------------- | ----------------- | ------------------- |
| 0 = 主机到设备  | 0 = 标准          | 0 = 设备            |
| 1 = 设备到主机  | 1 = 类            | 1 = 接口            |
|                 | 2 = 厂商          | 2 = 端点            |
|                 | 3 = 保留          | 3 = 其他            |
|                 |                   | 4 ~ 31 = 保留       |

本节只介绍USB协议定义的标准请求，即 bmRequestType 的 D[6:5] 位为 00 的请求。USB协议定义了11个标准请求（bRequest），其名字和请求代码如下表所示:

| bRequest       | Value |     | bRequest          | Value |
| -------------- | ----- | --- | ----------------- | ----- |
| GET_STATUS     | 0     |     | GET_CONFIGURATION | 8     |
| CLEAR_FEATURE  | 1     |     | SET_CONFIGURATION | 9     |
| SET_FEATURE    | 3     |     | GET_INTERFACE     | 10    |
| SET_ADDRESS    | 5     |     | SET_INTERFACE     | 11    |
| GET_DESCRIPTOR | 6     |     | SYNCH_FRAME       | 12    |
| SET_DESCRIPTOR | 7     |     |                   |       |

不同的请求对于其接收者、wValue和wIndex，其各字段的意义是不一样的。并且有的请求只能发送到设备，而有的请求可以发送到设备、接口和端点。

下表是各个标准请求的结构以及数据过程需要传输的数据。其中第一列有的有多个，主要是最低5位不同，即表示接收者不同。

(方向 类型 接收者)

| bmRequestType | bRequest          | wValue           | wIndex      | wLength    | 数据过程   |
| ------------- | ----------------- | ---------------- | ----------- | ---------- | ---------- |
| (0 00 00000)B | CLEAR_FEATURE     | 特性选择         | 0           | 0          | 没有       |
| (0 00 00001)B | ..                | ..               | 接口号      | ..         | ..         |
| (0 00 00010)B | ..                | ..               | 端点号      | ..         | ..         |
| (1 00 00000)B | GET_CONFIGURATION | 0                | 0           | 1          | 配置值     |
| (1 00 00000)B | GET_DESCRIPTOR    | 描述符类型和索引 | 0或者语言ID | 描述符长度 | 描述符     |
| (1 00 00001)B | GET_INTERFACE     | 0                | 接口号      | 1          | 备用接口号 |
| (1 00 00000)B | GET_STATUS        | 0                | 0           | 2          | 设备状态   |
| (1 00 00001)B | ..                | ..               | 接口号      | ..         | 接口状态   |
| (1 00 00010)B | ..                | ..               | 端点号      | ..         | 端点状态   |
| (0 00 00000)B | SET_ADDRESS       | 设备地址         | 0           | 0          | 没有       |
| (0 00 00000)B | SET_CONFIGURATION | 配置值           | 0           |            |            |
| (0 00 00000)B | SET_DESCRIPTOR    | 描述符类型和索引 | 0或者语言ID |            |            |
| (0 00 00000)B | SET_FEATURE       | 特性选择         | 0           | 0          | 没有       |
| (0 00 00001)B | ..                | ..               | 接口号      | ..         | ..         |
| (0 00 00010)B | ..                | ..               | 端点号      | ..         | ..         |
| (0 00 00001)B | SET_INTERFACE     | 备用接口号       | 接口号      | 0          | 没有       |
| (1 00 00010)B | SYNCH_FRAME       | 0                | 端点号      | 2          | 帧号       |


常用的几个请求为 GET_DESCRIPTOR, SET_ADDRESS 和 SET_CONFIGURATION 

#### 1) GET_DESCRIPTOR 请求

GET_DESCRIPTOR（获取描述符）请求是在枚举过程中用得最多的一个请求。主机通过发送获取描述符请求读取设备的各种描述符，从而可以获知设备类型、端点情况等众多重要信息。

获取描述符的接收者只能是设备，从 bmRequestType 的第 7 位可以看出，它是请求数据输入的。bRequest的值为Ox06（GET_DESCRIPTOR）。

GET_DESCRIPTOR 请求的结构如下:

| bmRequestType    | bRequest              | wValue           | wIndex      | wLength    | 数据过程 |
| ---------------- | --------------------- | ---------------- | ----------- | ---------- | -------- |
| 10000000B (0x80) | GET_DESCRIPTOR (0x06) | 描述符类型和索引 | 0或者语言ID | 描述符长度 | 描述符   |

* wValue 域的第一字节（低字节）表示的是索引号，用来选择同一种描述符（例如字符串描述符和配置描述符）中具体的某个描述符。

* wValue 域的第二字节，表示描述符的类型编号。各种描述符的类型编号定义如下

| 描述符类型                 | 编号 |
| -------------------------- | ---- |
| 设备描述符 (DEVICE)        | 1    |
| 配置描述符 (CONFIGURATION) | 2    |
| 字符串描述符 (STRING)      | 3    |
| 接口描述符 (INTERFACE)     | 4    |
| 端点描述符 (ENDPOINT)      | 5    |

* wIndex 域只在获取字符串描述符中有用，它表示字符串的语言ID号，获取除字符串描述符的其他描述符时，wIndex的值为0. 

* wLength域为请求设备返回数据的字节数，设备实际返回的字节数可以比该域指定的字节数少。设备在收到获取描述符的请求后，应该按照所请求的描述符类型编号，在数据过程中返回相应的描述符。

对于全速模式和低速模式，获取描述符的标准请求只有三种：获取设备描述符、获取配置描述符和获取字符串描述符。

另外的接口描述符和端点描述符是跟随配置描述符一并返回的，不能单独请求返回（如果单独返回，主机无法确认它们属于哪个配置）。

> 需要注意的是wValue、wIndex、wLength这三个域都是两字节的，在USB协议中规定，使用的是小端结构，即低字节在先，高字节在后。

#### 2) SET_ADDRESS 请求

SET_ADDRESS（设置地址）请求是主机请求设备使用指定地址的请求，指定的地址就包含在8字节数据中的wValue字段中。

每个连接在同一个主控制器上的USB设备都需要具有一个唯一的设备地址，这样主机才能区分每个不同的设备。当设备复位后，都使用默认的地址0.

主机从地址为 0 的设备获取设备描述符，一旦收到第一次设备描述符之后，主机就会发送设置地址的请求，以尽量减少设备使用公共地址 0 的时间。

设置地址请求是没有数据过程的，因而 wLength 的值为 0. wIndex也用不着，值为0.

当设备收到设置地址请求后，就直接进入状态过程，等待主机读取0长度的状态数据包。主机成功读取到状态数据包（用ACK响应设备）后，设备将启用新的地址。这以后的传输中，主机都将使用新的地址与设备进行通信。

SET_ADDRESS 请求的结构如下:

| bmRequestType    | bRequest           | wValue   | wIndex | wLength | 数据过程 |
| ---------------- | ------------------ | -------- | ------ | ------- | -------- |
| 00000000B (0x00) | SET_ADDRESS (0x05) | 设备地址 | 0      | 0       | 没有     |

#### 3) SET_CONFIGURATION 请求

SET_CONFIGURATION（设置配置）请求和设置地址请求很类似。区别在于 wValue域的意义。

在设置地址请求中，wValue的第一字节（低字节）为设备的地址；而在设置配置请求中，wValue的第一字节为配置的值。

当该值与某配置描述符中的配置编号一致时，表示选中该配置。该值通常为1，因为大多数USB设备只有一种配置，配置编号为1；

如果该值为0，则会让设备进人设置地址状态。设备只有在收到非0的配置值后，才能启用它的非0端点。

SET_CONFIGURATION 请求的结构如下:

| bmRequestType    | bRequest                 | wValue | wIndex | wLength | 数据过程 |
| ---------------- | ------------------------ | ------ | ------ | ------- | -------- |
| 00000000B (0x00) | SET_CONFIGURATION (0x09) | 配置值 | 0      | 0       | 没有     |


### 2. 设备描述符的实现

当主机请求设备返回设备描述符时，设备应该在数据过程中返回设备的设备描述符。

已知每个设备都必须有且仅有一个设备描述符，它的结构如下:

| 偏移量 | 域                 | 大小 | 说明                           |
| ------ | ------------------ | ---- | ------------------------------ |
| 0      | bLength            | 1    | 该描述符的长度 (18 字节)       |
| 1      | bDescriptorType    | 1    | 描述符类型 (设备描述符为 0x01) |
| 2      | bcdUSB             | 2    | 本设备所使用的 USB 协议版本    |
| 4      | dDeviceClass       | 1    | 类代码                         |
| 5      | bDeviceSubClass    | 1    | 子类代码                       |
| 6      | bDeviceProtocol    | 1    | 设备所使用的协议               |
| 7      | bMaxPackeSize0     | 1    | 端点0最大包长                  |
| 8      | iVender            | 2    | 厂商 ID                        |
| 10     | idProduct          | 2    | 产品 ID                        |
| 12     | bcdDevice          | 2    | 设备版本号                     |
| 14     | iManufacturer      | 1    | 描述厂商的字符串的索引         |
| 15     | iProduct           | 1    | 描述产品的字符串的索引         |
| 16     | iSerialNumber      | 1    | 产品序列号字符串的索引         |
| 17     | bNumConfigurations | 1    | 可能的配置数                   |

##### 1) bLength 

长度为1字节，表示该描述符的长度。设备描述符的长度为18字节，写成十六进制就是0x12。

##### 2) bDescriptorType

描述符的类型，长度为1字节。具体的取值如表3.5.5所列。设备描述符的编号为0x01。

##### 3) bcdUSB

该设备所使用的USB协议的版本，长度为2字节。可以取2.0或者1.1等版本号。

> 注意，它是用BCD码来表示的
> 
> 例如USB2.0协议就是0x0200，而USB1.1协议就是0x0110.前面说过，USB协议中使用的是小端结构，所以实际数据在传输时，是低字节在先的
> 
> 也就是说，USB2.0协议的bcdUSB拆成两个字节就是0x00和0x02.而USB1.1的bcdUSB拆成两个字节就是0x10和0x01。


##### 4)  bDeviceClass

长度为1字节，是设备所使用的类代码。设备的类代码由USB协会规定，具体的类代码可查阅USB相关文档。对于大多数标准的USB设备类，该字段通常设置为0，而在接口描述符中的bInterfaceClass中指定接口所实现的功能。当bDeviceClass为0时，下面的bDeviceSubClass也必须为0.如果bDeviceClass为OxFF，表示是厂商自定义的设备类。

##### 5) bDeviceSubClass

长度为1字节，是设备所使用的子类代码。当类代码不为0和OxFF时，子类代码由USB协议规定。当bDeviceClass为0时，bDeviceSubClass也必须为0。

##### 6)  bDeviceProtocol

长度为1字节，是设备所使用的协议，协议代码由USB协会规定。当该字段为0时，表示设备不使用类所定义的协议。当该字段为OxFF时，表示设备使用厂商自定义的协议。bDeviceProtocol必须要结合设备类和设备子类联合使用才有意义，因此当类代码为0时，bDeviceProtocol应该也要为0。

##### 7) bMaxPackeSizeo

长度为1字节，是端点0的最大包长。它的取值可以为8、16、32、64。

##### 8) idVender

长度为2字节，它是厂商的ID号。该ID号由USB协会分配，不能随意使用。可以跟USB协会申请一个厂商D号，但是这是需要交“保护费”的。在实验中，选取了一个0x8888作为厂商ID号。这个ID号是随便选的，也许它已经被某个公司购买了。

只是拿来学习使用是没关系的，如果要做真正的产品设计，就要注意了，必须要使用公司自己的ID号，以避免侵权。另外，主机通常是靠厂商ID号、产品ID号以及产品序列号来安装和加载驱动的，如果使用了别人的ID号，可能会导致驱动程序安装或加载错误，从而导致设备无法工作。跟bcdUSB一样，要注意小端结构的问题。

##### 9) idProduct

长度为2字节，是产品ID号。与厂商ID号不一样，它是由生产厂商自己根据产品来编号的，比较自由。

USB鼠标，可以给它分配产品ID为0x0001；USB键盘，可以分配产品ID为0x0002，而厂商ID则使用同一个0x8888.通常主机会根据厂商ID、产品ID以及设备的序列号来加载对应的驱动。

调试时很容易犯的一个错误是在修改描述符之后，重新测试，却没有修改上述三个中的任何一个，结果加载的还是旧驱动，从而工作不正常。这时如果没有想到这一点，一个劲地在那查找固件的问题，恐怕是找不出问题的。

如果不想修改这些ID号和序列号，还有另外一个方法就是去设备管理器里，将原先的驱动卸载，然后扫描新硬件或者拔下重新插上，让它重新安装驱动。不过有时卸载了还是会有残留信息在注册表中，所以还是改ID比较保险。

##### 10) bcdDevice

长度为2字节，是设备的版本号。当同一个产品升级后（例如修改了固件增加了某些功能），可以通过修改设备的版本号来区别。
##### 11) iManufacturer

长度为1字节，是描述厂商的字符串的索引值。当该值为0时，表示没有厂商字符串。主机获取设备描述符时，会将索引值放在wValue的第一字节中，用来选择不同的字符串。

##### 12) iProduct

长度为1字节，是描述产品的字符串的索引值。

当该值为0时，表示没有产品字符串。当第一次插上某个USB设备时，会在Windows的右下角弹出一个对话框，显示发现新硬件，并且会显示该设备的名称。

其实这里显示的信息就是从产品字符串里获取来的。如果想让它显示出所需要的信息，应该修改产品字符串。

##### 12) iSerialNumber

长度为1字节，是设备的序列号字符串索引值。

最好给你的每个产品指定一个唯一的序列号，好比每个英特尔的奔四处理器都有一个ID号一样。设备序列号可能被主机联合VID和PID用来区别不同的设备，有时同时连接多个具有相同VID、PID以及设备序列号的设备，可能会导致设备无法正确识别。当该值为0时，表示没有序列号字符串。

##### 13) bNumConfigurations

长度为1字节，表示设备有多少种配置。每种配置都会有一个配置描述符，主机通过发送设置配置来选择某一种配置。大部分的USB设备只有一个配置，即该字段的值为1。

通过上面的描述，就可以构造一个设备描述符了。

下面给出USB鼠标实例的设备描述符。这里使用数组来实现，使用数组的好处是可移植性好，但是可读性稍差点。如果使用结构体，可读性会好一些；但是在不同位数的处理器中移植时，可能存在着字节对齐、大小端问题和结构体填充等问题。


| 偏移量 | 域                 | 大小 | 说明                           |
| ------ | ------------------ | ---- | ------------------------------ |
| 0      | bLength            | 1    | 该描述符的长度 (18 字节)       |
| 1      | bDescriptorType    | 1    | 描述符类型 (设备描述符为 0x01) |
| 2      | bcdUSB             | 2    | 本设备所使用的 USB 协议版本    |
| 4      | dDeviceClass       | 1    | 类代码                         |
| 5      | bDeviceSubClass    | 1    | 子类代码                       |
| 6      | bDeviceProtocol    | 1    | 设备所使用的协议               |
| 7      | bMaxPackeSize0     | 1    | 端点0最大包长                  |
| 8      | iVender            | 2    | 厂商 ID                        |
| 10     | idProduct          | 2    | 产品 ID                        |
| 12     | bcdDevice          | 2    | 设备版本号                     |
| 14     | iManufacturer      | 1    | 描述厂商的字符串的索引         |
| 15     | iProduct           | 1    | 描述产品的字符串的索引         |
| 16     | iSerialNumber      | 1    | 产品序列号字符串的索引         |
| 17     | bNumConfigurations | 1    | 可能的配置数                   |


```c
uint8_t device_descriptor[0x12] = {
    0x12,           // bLength          设备描述符的长度 (18 字节)
    0x01,           // bDescriptorType  设备描述符的编号 0x01
    0x010, 0x01,    // bcdUSB           USB 协议版本 0x0110 (USB1.1)
    0x00,           // dDeviceClass     设备类我们是在接口描述符中定义, 不在设备描述符定义, 所以为 0
    0x00,           // bDeviceSubClass  dDeviceClass 为 0 时, 该字段也为 0
    0x00,           // bDeviceProtocol  dDeviceClass 为 0 时, 该字段也为 0
    0x10,           // bMaxPackeSize0   端点0最大包长, 由 USB 芯片端点的缓冲区大小决定
    0x88, 0x88,     // iVender          厂商 ID, 这里 0x8888 仅实验用
    0x01, 0x00,     // idProduct        产品 ID, 0x0001
    0x00, 0x01,     // bcdDevice        设备版本号, 0x0100, 把它定位为 V1.0 版本
    0x01,           // iManufacturer    描述厂商的字符串的索引，为了方便管理, 这里就从 1 开始
    0x02,           // iProduct         描述产品的字符串的索引, 刚才用了 1, 这里就取 2
    0x03,           // iSerialNumber    产品序列号字符串的索引, 按顺序下来, 这里取 3 即可
    0x01,           // bNumConfigurations 该设备所具有的配置数, 这里只要一种配置即可
}
```

### 3. 设备描述符的返回

在端点0的输出中断处理函数中，我们已经接收到了标准请求的数据, 这时可先对接收到的建立过程的数据进行判断，如果是获取设备描述符的请求，那么将设备描述符数组的内容写人到端点0输入缓冲区中，并使能端点发送。

当主机在下一次发送IN令牌后，USB 接口芯片将会自动将端点0输入缓冲区中的数据返回给主机，这样就实现了获取设备描述符的请求。

```c
static uint8_t *p_send_data;
static uint16_t send_length;
static uint8_t need_zero_package = 0;

void usb_end0_out(void)
{
    // 将端点0的数据读取到内存
    USB_READ_ENDP_BUF(ENDP0, buf);

    uint8_t  bmRequestType  = buf[0];
    uint8_t  bRequest       = buf[1];
    uint16_t wValue         = buf[2] + (buf[3] << 8);
    uint16_t wIndex         = buf[4] + (buf[5] << 8);
    uint8_t  wLength        = buf[6] + (buf[7] << 8);

    // 输入请求
    if(bmRequestType & (1 << 8)) 
    {
        // D[6:5] 为请求的类型, 其中 00 为标准请求
        if(bmRequestType & 0x60 == 0) 
        {
            switch(bRequest)  
            {
                // 获取配置请求
                case GET_CONFIGURATION:
                break;

                // 获取描述符请求
                case GET_DESCRIPTOR:
                break;

                // 处理更多标准输入请求 ...

                // 未定义的标准输入请求
                default:
                break;
            }
        }

        // 请求长度和实际数据长度之间取最小值
        if(wLength < send_length)
            send_length = wLength;

        // 刚好是整数个数据包, 需发一个空包表示结束传输
        if(send_length % device_descriptor[7] == 0)
            need_zero_package = 1;

        /* 立即向端点 0 写数据, 若数据未发完, 会在中断处理函数 usb_end0_in 中继续发送 */
        usb_end0_write_data();
    }

    USB_ENDP_BUF_CLEAR(ENDP0);
}
```

下面是对上述代码框架获取描述符部分的填充:

```c
// 获取描述符请求
case GET_DESCRIPTOR:
    // 根据 wValue 的高字节, 检查请求的描述符类型
    switch ((wValue >> 8) & 0xff)
    {
        // 获取设备描述符
        case DEVICE_DESCRIPTOR:

        p_send_data = device_descriptor;
        send_length = device_descriptor[0];
        break;

        // 获取配置描述符
        case CONFIGURATION_DESCRIPTOR:
        break;

        // 获取字符串描述符
        case STRING_DESCRIPTOR:
        break;
    }
break;
```

其中向端点 0 写数据的函数定义如下:

```c
void usb_end0_write_data(void)
{
    // 剩余要发的数据大于端点包大小, 则发送一个最大包长的数据
    if(send_length > device_descriptor[7])
    {
        USB_WRITE_ENDP_BUF(ENDP0, p_send_data, device_descriptor[7]);
        send_length -= device_descriptor[7];
        p_send_data += device_descriptor[7];
    }

    else if(send_length != 0)
    {
        USB_WRITE_ENDP_BUF(ENDP0, p_send_data, send_length);
        send_length = 0;
    }

    // 要发的数据和端点包大小刚好整除, 还需发最后一个空包来结束发送
    else if(need_zero_package == 1)
    {
        USB_WRITE_ENDP_BUF(ENDP0, p_send_data, 0);
        need_zero_package = 0;
    }
}

// 端点0输入中断处理
void usb_end0_in()
{
    usb_end0_write_data();
}
```

到此程序已经能够自动返回设备描述符了.

### 4. 设置地址请求的处理

每个USB设备都具有一个唯一的设备地址，这个地址是主机在设置地址请求时分配给设备的。设备在收到设置地址请求后，应该返回一个0长度的状态数据包（因为设置地址请求是没有数据过程的），然后等待主机确认这个数据包（即用ACK应答设备）。设备在正确接收到状态数据包的ACK之后，就开始使用新的设备地址了。

一般 USB 接口芯片会提供一个寄存器用来设置设备地址, 我们这里假设宏 `USB_SET_ADDR(addr)` 可完成该功能:

因此只需在处理标准请求框架中填充设置地址的相关代码, 如下:

```c
// 获取描述符请求
case SET_ADDRESS:
    // wValue 中的低8位是设置的地址值
    USB_SET_ADDR(wValue & 0xff);

    // 设置没有数据过程, 直接进入到状态过程, 因此返回一个 0 长度的数据包
    send_length = 0;
break;
```


在设置设备地址成功后, 主机会再次获取设备描述符, 但这次将会是完整地获取。

获取完设备描述符之后，主机接着又发了一个请求：获取配置描述符的请求，请求长度为9字节。标准配置描述符的长度就是9字节的。

通常主机第一次先获取9字节长度的配置描述符，然后根据配置描述符中的配置集合长度，再次获取配置描述符。

第二次获取配置描述符时，会将配置描述符、接口描述符、类特殊描述符（如果有）、端点描述符等一并读回。

因此接下来就是实现配置描述符及接口描述符、类特殊描述符、端点描述符这个大集合----配置描述符集合。

### 5. 配置描述符集合

每个USB设备至少都要有一个配置描述符，在设备描述符中规定了该设备有多少种配置，每种配置都有一个描述符。在本USB实例中，只有一个配置描述符。

#### 1) 配置描述符的结构

如下是USB协议规定的标准配置描述符的结构。

| 偏移量 | 域                  | 大小 | 说明                           |
| ------ | ------------------- | ---- | ------------------------------ |
| 0      | bLength             | 1    | 该描述符的长度 (9 字节)        |
| 1      | bDescriptorType     | 1    | 描述符类型 (配置描述符为 0x02) |
| 2      | wTotalLength        | 2    | 配置描述符集合总长度           |
| 4      | bNumInterfaces      | 1    | 该配置所支持的接口数           |
| 5      | bConfigurationValue | 1    | 该配置的值                     |
| 6      | iConfiguration      | 1    | 描述该配置的字符串的索引值     |
| 7      | bmAttributes        | 1    | 该设备的属性                   |
| 8      | bMaxPower           | 1    | 设备所需要的电流 (单位 2 mA)   |

* **bLength** 大小为1字节，表示该描述符的长度。标准的USB配置描述符的长度为9字节。
* **bDescriptorType** 大小为1字节，表示描述符的类型。配置描述符的类型编码为0x02。
* **wTotalLength** 大小为2字节，表示整个配置描述符集合的总长度，包括配置描述符、接口描述符、类特殊描述符（如果有）和端点描述符。注意低字节在先。
* **bNumInterfaces** 大小为1字节，表示该配置所支持的接口数量。通常，功能单一的设备只具有一个接口（例如鼠标），而复合设备则具有多个接口（例如音频设备）。
* **bConfigurationValue** 大小为1字节，表示该配置的值。通常一个USB设备可以支持多个配置，bConfigurationValue就是每个配置的标识。设置配置请求时会发送一个配置值，如果某个配置的bConfigurationValue值与它相匹配，就表示该配置被激活，为当前配置。
* **iConfiguration** 大小为1字节，是描述该配置的字符串的索引值。如果该值为0，则表示没有字符串。
* **bmAttributes** 大小为1字节，用来描述设备的一些特性。其中，D7是保留的，必须要设置为1. D6表示供电方式，当D6为1时，表示设备是自供电的；当D6为0时，表示设备是总线供电的。D5表示是否支持远程唤醒，当D5为1时，支持远程唤醒。D4～DO保留，设置为0。
* **bMaxPower** 大小为1字节，表示设备需要从总线获取的最大电流量，单位为2mA。例如、如果需要200mA的最大电流，则该字节的值为100。

#### 2) 接口描述符的结构

如下是 USB 协议规定的标准接口描述符的结构。接口描述符不能单独返回，必须附着在配置描述符后一并返回。

| 偏移量 | 域                 | 大小 | 说明                           |
| ------ | ------------------ | ---- | ------------------------------ |
| 0      | bLength            | 1    | 该描述符的长度 (9 字节)        |
| 1      | bDescriptorType    | 1    | 描述符类型 (接口描述符为 0x04) |
| 2      | blnterfaceNumber   | 1    | 该接口的编号 (从0开始)         |
| 3      | bAlternateSetting  | 1    | 该接口的备用编号               |
| 4      | bNumEndpoints      | 1    | 该接口所使用的端点数           |
| 5      | bInterfaceClass    | 1    | 该接口所使用的类               |
| 6      | bInterfaceSubClass | 1    | 该接口所使用的子类             |
| 7      | bInterfaceProtocol | 1    | 该接口所使用的协议             |
| 8      | iInterface         | 1    | 描述该接口的字符串的索引值     |

* **bLength** 大小为1字节，表示该描述符的长度。标准的USB接口描述符的长度为9字节。
* **bDeseriptorType** 大小为1字节，是描述符的类型。接口描述符的类型编码为0x04。
* **blnterfaceNumber** 大小为1字节，表示该接口的编号。当一个配置具有多个接口时，每个接口的编号都不相同。从0开始依次递增对一个配置的接口进行编号。
* **bAlternateSetting** 大小为1字节，是该接口的备用编号。编号规则与bInter-faceNumber一样，很少会使用该字段，设置为0。
* **bNumEndpoints** 大小为1字节，是该接口所使用的端点数（不包括0端点）。如果该字段为0，则表示没有非0端点，只使用默认的控制端点。
* **bInterfaceClass、bInterfaceSubClass、bInterfaceProtocol** 分别是接口所使用的类、子类以及协议，它们的代码由USB协会定义，跟设备描述符中的意义类似。通常在接口中定义设备的功能，而在设备描述符中将类、子类以及协议字段的值设置为0。
* **iInterface** 大小为1字节，是描述该接口的字符串的索引值。如果该值为0，则表示没有字符串。

#### 3) 端点描述符的结构

如下是USB协议规定的标准端点描述符的结构。端点描述符不能单独返回，必须附着在配置描述符后一并返回。

| 偏移量 | 域               | 大小 | 说明                           |
| ------ | ---------------- | ---- | ------------------------------ |
| 0      | bLength          | 1    | 该描述符的长度 (7 字节)        |
| 1      | bDescriptorType  | 1    | 描述符类型 (端点描述符为 0x05) |
| 2      | bEndpointAddress | 1    | 该端点的地址                   |
| 3      | bmAttributes     | 1    | 该端点的属性                   |
| 4      | wMaxPackeSize    | 2    | 该端点支持的最大包长度         |
| 6      | bInterval        | 1    | 端点的查询时间                 |

* **bLength** 大小为1字节，表示该描述符的长度。标准的USB端点描述符的长度为7字节。
* **bDescriptorType** 大小为1字节，表示描述符的类型。端点描述符的类型编码为0x05。
* **bEndpointAddress** 大小为1字节，表示该端点的地址。最高位D7为该端点的传输方向，1 为输入（有点像Input的第一个字母），0为输出（有点像Output的第一个字母）。D3~D0为端点号。D6~D4保留，设为0。
* **bmAttributes** 大小为1字节，是该端点的属性。最低两位D1~D0表示该端点的传输类型，0为控制传输，1为等时传输，2为批量传输，3为中断传输。如果该端点是非等时传输的端点，那么D7~D2为保留值，设为0.如果该端点是等时传输的，则D3~2表示同步的类型，0为无同步，1为异步，2为适配，3为同步；D5~D4表示用途，0为数据端点，1为反馈端点，2为暗含反馈的数据端点，3是保留值。D7~D6保留。
* **wMaxPackeSize** 大小为2字节，是该端点所支持的最大包长度。注意低字节在先。对于全速模式和低速模式，D10~D0表示端点的最大包长，其他位保留为0.对于高速模式，D12~D11为每个帧附加的传输次数，具体请参看USB2.0协议。
* **bInterval** 大小为1字节，表示该端点查询的时间。对于中断端点，表示查询的帧间隔数。对于等时传输以及高速模式的中断、批量传输，该字段的意义请参看USB2.0协议。

#### 4) HID 描述符的结构

我们知道，USB鼠标是属于 USB HID 类的。通过查看 USB HID 类的官方文档，HID类的设备在配置描述符中还需要一个HID描述符。它是一个类描述符，应该跟在接口描述符后面。HID描述符的结构如下:

| 偏移量 | 域                | 大小 | 说明                            |
| ------ | ----------------- | ---- | ------------------------------- |
| 0      | bLength           | 1    | 该描述符的长度                  |
| 1      | bDescriptorType   | 1    | 描述符类型 ( HID 描述符为 0x21) |
| 2      | bcdHID            | 2    | HID 协议的版本                  |
| 4      | bCountyCode       | 1    | 国家代码                        |
| 5      | bNumDescriptors   | 1    | 下级描述符的数量                |
| 6      | bDescriptorType   | 1    | 下级描述符的类型                |
| 7      | bDescriptorLength | 2    | 下级描述符的长度                |
| 9      | bDescriptorType   | 1    | 下级描述符的类型 (可选)         |
| 10     | bDescriptorLength | 2    | 下级描述符的长度  (可选)        |
| 12     | ...               | 1    | ... 可选                        |

* **bLength** 大小为1字节，是该描述符的总长度。它的大小与该描述符中下级描述符的个数有关。例如，只有一个下级描述符时，总长度为 1+1+2+1+1+1+2 = 9 字节。
* **bDescriptorType** 大小为1字节，是该描述符的编号。HID描述符的编号为0x21。
* **bedHID** 大小为2字节，是该设备所使用的HID协议的版本号。这里使用的HID协议是USB HID1.1协议，因此这里为0x0110。
* **bCountyCode** 大小为1字节，是设备所适用的国家。通常我们的键盘是美式键盘，代码为33，即0x21。
* **bNumDescriptors**  大小为1字节，是下级描述符的数量。该值至少为1，即至少要有一个报告描述符。下级描述符可以是报告描述符或物理描述符。
* **bDescriptorType** 大小为l字节，是下级描述符的类型。报告描述符的编号为0x22，物理描述符编号为0x23。
* **bDescriptorLength** 大小为2字节，是下级描述符的长度。当有多个下级描述符时，bDescritpor Type和bDescriptorLength交替重复下去。

### 6. 配置描述符集合的实现以及返回


通过前面的分析知道了配置描述符集合的结构，接下来就要用代码来实现一个配置描述符集合，并在主机的获取配置描述符请求中返回。

```c

// 报告描述符暂时空着
uint8_t report_descriptor[] = {
    0x00
};

uint8_t configuration_descriptor[9 + 9 + 9 + 7] = {
    /*************** 配置描述符 *********************/
    0x09,         // bLength              配置描述符的长度为 9
    0x02,         // bDescriptorType      配置描述符编号为 0x02
    
    // wTotalLength 配置描述符集合的总长度, 包括配置描述符本身, 接口描述符、类描述符、端点描述符等
    sizeof(configuration_descriptor) & 0xff,
    (sizeof(configuration_descriptor) >> 8) & 0xff,
    0x01,         // bNumInterfaces       该配置包含的接口数, 这里只有一个接口
    0x01,         // bConfigurationValue  该配置的值, 作为配置的标识
    0x00,         // iConfiguration       该配置的字符串索引, 这里没有, 故为0
    0x80,         // bmAttributes         该设备的属性, 这里0x80为: 总线供电, 禁止唤醒功能
    0x32,         // bMaxPower            设备所需要的电流 (单位 2mA), 这里为 0x32 (50) 即 100mA

    /*************** 接口描述符 *********************/
    0x09,         // bLength              接口描述符长度为 9 字节
    0x04,         // bDescriptorType      接口描述符的编号为 0x04
    0x00,         // blnterfaceNumber     该接口描述符的编号，第一个接口编号为 0
    0x00,         // bAlternateSetting    该接口的备用编号为 0
    0x01,         // bNumEndpoints        该接口非 0 端点的数目. 由于USB鼠标只需一个中断输入端点, 故为1
    0x03,         // bInterfaceClass      该接口所使用的类, USB鼠标是HID类, 编码为 0x03
    0x01,         // bInterfaceSubClass   该接口所使用的子类, 0x01 支持BIOS引导启动的子类
    0x02,         // bInterfaceProtocol   该接口所使用的协议, 若子类为 0x01, 则协议可选键盘(0x01)或鼠标(0x02)
    0x00,         // iInterface           该接口的字符串的索引，这里没有, 故为0
    
    /*************** HID 描述符 *********************/
    0x09,         // bLength              该 HID 描述符下只有一个下级描述符, 故长度为 9 字节
    0x21,         // bDescriptorType      HID 描述符的编号为 0x21
    0x10, 0x01,   // bcdHID               该 HID 使用的协议版本, 这里为 0x0110 (HID1.1)
    0x21,         // bCountyCode          该 HID 适用的国家代码, 这里为 0x21 (美国)

    0x01,         // bNumDescriptors      该 HID 下级描述符的数量, 这里只有一个报告描述符, 故为 1
    0x22,         // bDescriptorType      该 HID 下级描述符的类型, 这里为 0x22 (报告描述符)
    // bDescriptorLength 该 HID 下级描述符的长度
    sizeof(report_descriptor) & 0xff,
    (sizeof(report_descriptor) >> 8) & 0xff,
    
    /*************** 端点描述符 *********************/
    0x07,         // bLength              端点描述符长度为 7 字节
    0x05,         // bDescriptorType      端点描述符编号为 0x05
    0x81,         // bEndpointAddress     该端点的地址, 这里为 0x81 (D7 为 1 表示输入, D[3:0] 为 1 表示地址)
    0x03,         // bmAttributes         该端点的属性, 这里为 0x03 (D[1:0] 为 11B 表示中断传输, 其他位保留为 0)
    0x01, 0x00    // wMaxPackeSize        该端点的最大包长, 这里为 0x0010 (16 字节)
    0x0A          // bInterval            该端点查询的时间, 这里设为 0x0A (10个帧时间, 即 10ms)
};
```


然后在获取描述符请求中增加对配置描述符返回的代码即可, 修改端点0输出中断处理函数中获取配置描述符的代码如下:

```c
// 获取配置描述符
case CONFIGURATION_DESCRIPTOR:
    p_send_data = configuration_descriptor;
    send_length = configuration_descriptor[3] * 256 + configuration_descriptor[2];
break;
```

到此完成配置描述符集合的返回. 不过报告描述符还没有实现, 但在次之前先来看看字符串描述符.

主机可能会在获取 9 字节的配置描述符后, 先获取设备描述符中的字符串描述符索引，而我们还也没有实现该部分。
### 7. 字符串及语言ID请求的实现

在USB协议中，字符串描述符是可选的。当某个描述符中的字符串索引值为非0时，就表示它具有那个字符串描述符，注意索引值不能重复。

上文在设备描述符中，申请了3个非0的索引值，分别是厂商字符串、产品字符串以及产品序列号；其索引值分别为1,2,3.

USB主机使用获取字符串描述符和索引值来获取对应的字符串。当索引值为0时，表示获取语言ID。语言ID是一个描述该设备支持的语言种类的数组，每个ID号占2字节。

字符串描述符的结构如下:

| 偏移 | 域              | 大小 | 描述                         |
| :--: | --------------- | :--: | ---------------------------- |
|  0   | bLength         |  1   | 该描述符的长度               |
|  1   | bDescriptorType |  1   | 描述符的类型 (字符串为 0x03) |
|  2   | bString         |  N   | Unicode 编码的字符串         |


语言 ID 描述符的结构如下:

|   偏移    | 域              | 大小 | 描述                         |
| :-------: | --------------- | :--: | ---------------------------- |
|     0     | bLength         |  1   | 该描述符的长度               |
|     1     | bDescriptorType |  1   | 描述符的类型 (字符串为 0x03) |
|     2     | wLANGID[0]      |  2   | 语言 ID 号 0                 |
|    ...    | ...             | ...  | ...                          |
| 2 * n + 2 | wLANGID[n]      |  2   | 语言 ID 号  n                |

语言ID，这里只使用美式英语一种，即0x0409.不同国家地区的语言ID号，可以查看USB_LANGIDs.PDF文档，可直接上http://www.usb.org下载。

字符串描述符中的 bString 字段是使用 Unicode. 

下面是语言 ID、厂商字符串、产品字符串以及产品序列号的定义:

```c
uint8_t language_id[4] = {
     /*************** 语言 ID 描述符 *********************/
    0x04,         // bLength              该语言 ID 描述符长度为 4 字节
    0x03,         // bDescriptorType      语言 ID 描述符和字符串描述符编号为 0x03
    0x09, 0x04    // wLANGID              该语言 ID 号, 0x0409 (美式英语)
};

#define manufacturer_string "测试厂商字符串描述符"
#define product_string "测试产品字符串描述符"
#define serial_number_string "测试V1.0"

uint8_t manufacturer_string_descriptor[2 + sizeof(manufacturer_string) - 1] = {
     /*************** 制造商字符串描述符 *********************/
    2 + sizeof(manufacturer_string) - 1,   // bLength          该字符串描述符的长度
    0x03,                                  // bDescriptorType  字符串描述符编号为 0x03
    // ... 这里实际的字符串先空着不写, 等需要上传时再拷贝进来, 其他部分同理
};

uint8_t product_string_descriptor[2 + sizeof(product_string) - 1] = {
     /*************** 产品字符串描述符 *********************/
    2 + sizeof(product_string) - 1,       
    0x03,
};

uint8_t serial_number_string_descriptor[2 + sizeof(serial_number_string) - 1] = {
     /*************** 产品序列号字符串描述符 *********************/
    2 + sizeof(serial_number_string) - 1,
    0x03,
};
```

在获取描述符请求中增加对字符串描述符的返回:

```c
// 获取字符串描述符
case STRING_DESCRIPTOR:

    // 根据 wValue 的低字节, 检查字符串描述符类型
    switch (wValue & 0xff)
    {
        // 获取语言 ID
        case 0:
        p_send_data = language_id;
        send_length = language_id[0];
        break;

        // 厂商字符串索引值设为1, 所以这里为厂商字符串
        case 1:
        p_send_data = manufacturer_string_descriptor;
        send_length = manufacturer_string_descriptor[0];
        memcpy(manufacturer_string_descriptor + 2, manufacturer_string, send_length);
        break;

        // 产品字符串索引值设为2, 所以这里为产品字符串
        case 2:
        p_send_data = product_string_descriptor;
        send_length = product_string_descriptor[0];
        memcpy(product_string_descriptor + 2, product_string, send_length);
        break;

        // 产品序列号字符串索引值设为3, 所以这里为产品序列号字符串
        case 3:
        p_send_data = serial_number_string_descriptor;
        send_length = serial_number_string_descriptor[0];
        memcpy(serial_number_string_descriptor + 2, serial_number_string, send_length);
        break;

        // 未知索引值
        default:
        send_length = 0;
        break;
    }
break;
```

主机获取字符串描述符后就会去获取之前实现的配置描述符集合, 然后会发出一个标准的设置配置请求，因此接下来是对设置配置请求的实现。

### 8. 设置配置请求的实现

设置配置请求的实现比较简单，它是一个输出请求，只要根据所请求的配置值，使能相应的端点即可。

由于我们的鼠标只有一个配置，所以连配置值都可以忽略，直接使能端点，然后返回一个0长度的状态数据包即可。

> 注意，只有收到非0的配置值之后才可以使能非0端点，否则要禁用非0端点。

实现标准请求处理中的设置配置请求:

```c
case SET_CONFIGURATION:
    // 使能端点 1
    USB_ENABLE_ENDP(ENDP1);
    // 发送空包
    send_length = 0;
break;
```

### 9. 报告描述符的实现

USB HID设备是通过报告（report）来传送数据的，报告有输入报告和输出报告。

输入报告是USB设备发送给主机的，例如：USB鼠标将鼠标移动和鼠标点击等信息返回给计算机，键盘将按键数据返回给计算机等。

输出报告是主机发送给USB设备的，例如：键盘上的数字键盘锁定灯和大写字母锁定灯的控制等。

报告里面包含的是所要传送的数据，数量为整数字节，被划分成一个个域。通常，输入报告是通过中断输入端点返回的，而输出报告有点区别，当没有中断输出端点时，可以通过控制输出端点0发送，当有中断输出端点时，通过中断输出端点发出。

当然，不管设备是否具有中断输出端点（中断输入端点是必须要的），主机都可以通过获取报告和设置报告的请求从端点0来获取或者发送报告。

而报告描述符（report descriptor），是用来描述一个报告的结构以及该报告里面的数据是用来干什么用的。通过报告描述符，USB主机就可以分析出报告里面的数据所表示的意义。报告描述符与普通描述符一样，都是通过控制输入端点0来返回，主机使用获取报告描述符请求来获取报告描述符，注意这个请求是**发送到接口的，而不是到设备**。

一个报告描述符可以描述多个报告，不同的报告通过报告ID来识别。报告ID放在报告的最前面，即第一个字节。当报告描述符中没有规定报告ID时，报告中就没有ID字段，开始就是数据。

报告描述符与前面所遇到的描述符结构不一样，它并没有描述符长度和描述符类型等信息，而是由一个个条目（item）组成的。通常，在写报告描述符时，一个条目占据一行。这样看起来清晰一些。
HID协议中规定了两种条目：短条日和长条目。长条日很少使用，这里不介绍，只介绍短条目的结构。

**短条目**由1字节的前缀后面跟上可选的数据字节组成。可选的数据字节可以为0字节、1字节、2字节或者4字节。实际所使用的条目，大部分是只有1字节可选数据的，少数会使用0字节或2字节数据。条目的前缀结构如下:

| D[7:4]                         | D[3:2]                | D[1:0]                       |
| ------------------------------ | --------------------- | ---------------------------- |
| bTag                           | bType                 | bSize                        |
| 表示该条目的功能               | 表示条目的类型        | 用来表示后面所跟数据的字节数 |
| 具体请参看HID协议以及HID用途表 | 0 为主条目            |                              |
|                                | 1 为全局（global）条目 |                              |
|                                | 2 为局部（local）条目  |                              |
|                                | 3 是保留值             |                              |

#### 1) 主条目 

总共有5个，分别为Input（输入）、Output（输出）、Feature（特性）、Collection（集合开始）和EndCollection（集合结束）。主条目用来定义或者分组报告的数据域。

例如，可以使用输人主条目将输人报告划分为不同的数据域，以及指定该域的属性。对于Input、Output、Feature三个主条目，后面跟的第一字节数据每个位的数据表示一种属性，例如：位0表示该数据域是变量还是常量，位1表示是数组还是单一变量，位2表示是相对值还是绝对值等。

#### 2) 全局条目

全局条目主要用来选择用途页，定义数据域的长度、数量、报告ID等。全局条目在出现后对接下来的所有主条目都有效，除非遇到另外一个全局条目来改变它。

常用的全局条目有：UsagePage（用途页）、LogicalMinimum（逻辑最小值）、Logical Maximum（逻辑最大值）、PhysicalMinimum（物理最小值）、PhysicalMaximum（物理最大值）、ReportSize（数据域大小）、ReportCount（数据域数量）和ReportID（报告ID）。

其中，ReportSize用来描述某个数据域有多少个位：ReportCount用来描述这样的数据域有多少个；LogicalMinimum和LogicalMaximum用来描述数据域的取值范围。

#### 3) 局部条目

局部条目用来定义控制的特性，例如，该数据域的用途、用途最小值、用途最大值等。局部条目只在局部有效，遇到一个主条目后，它的效用就结束了。

常用的局部条目有：Usage（用途）、UsageMinimum（用途最小值）和UsageeMaximum（用途最大值）。

各种条目的功能值在此就不一一列举了，大家可以对照协议来设计报告描述符，也可以使用USB官方网站提供的HID描述符工具来生成：还可以使用现成的报告描述符进行修改，例如，在HID协议以及用途表文档中，就有很多现成的例子。
下面给出USB鼠标报告描述符的实际代码，并在代码中增加注释来说明这个报告描述符的意义。

```c
// 报告描述符
uint8_t report_descriptor[] = {
    /* 前缀: bTag[7:4] bType[3:2] bSize[1:0] */
    0x05, 0x01,             // 全局条目, bTag = 0x0 (UsagePage), 0x01 (Generic Desktop)
    0x09, 0x02,             // 局部条目, Usage (Mouse)
    0xa1, 0x01,             // 主条目  , Collection (Application), 说明应用集合开始
        0x09, 0x01,         // 局部条目, Usage (Pointer) 说明用途为指针集合
        0xa1, 0x00,         // 主条目  , Collection (Physical) 说明物理集合开始
            0x05, 0x09,     // 局部条目, UsagePage (Button) 说明用途页为按键
            0x19, 0x01,     // 局部条目, UsageMinium (Button 1 鼠标左键)
            0x29, 0x03,     // 局部条目, UsageMaxium (Button 3 鼠标中键)
            0x15, 0x00,     // 全局条目, LogicMinium (0)
            0x25, 0x01,     // 全局条目, LogicMaxium (1)
            0x95, 0x03,     // 全局条目, ReportCount (3)
            0x75, 0x01,     // 全局条目, ReportSize (1)

            0x81, 0x02,     // 主条目  , Input (Data, Var, Abs)
            0x95, 0x01,     // 全局条目, ReportCount (1)
            0x75, 0x05,     // 全局条目, ReportSize (5)
            0x81, 0x03,     // 主条目  , Input (cnst, Var, Abs)
            0x05, 0x01,     // 全局条目, UsagePage (Generic Desktop)
            0x09, 0x30,     // 局部条目, Usage(X)
            0x09, 0x31,     // 局部条目, Usage(Y)
            0x09, 0x38,     // 局部条目, Usage(Wheel)
            0x15, 0x81,     // 全局条目, LogicMinium (-127)
            0x25, 0x7f,     // 全局条目, LogicMaxium (127)
            0x75, 0x08,     // 全局条目, ReportSize (8)
            0x95, 0x03,     // 全局条目  , ReportCount (3)
            0x81, 0x06,     // 主条目, Input (Data, Var, Rel)
        0xc0,               // CollectionEnd
    0xc0                    // CollectionEnd
};
```

通过上面的报告描述符的定义可知，返回的输入报告具有4字节。

* 第一字节的低3位用来表示按键是否按下，高5位为常数0，无用；
* 第二字节表示X轴的改变量；
* 第三字节表示Y轴的改变量；
* 第四字节表示滚轮的改变量。

在中断输入端点1中应该要按照上面的格式返回实际的鼠标数据。

在描述符获取请求中添加报告描述符的请求:


```c
case REPORT_DESCRIPTOR:
    p_send_data = report_descriptor;
    send_length = sizeof(report_descriptor);
break;
```

至此, HID 设备才算枚举成功. 之后主机应该就会一直在查询端点1，以读取鼠标返回的报告，下面就对 HID 类的相关请求进行实现。

### 11. HID 类请求的实现 

在 HID 协议文档中, 定义了一些类请求, 例如 SET_IDLE, GET_IDLE, GET_REPORT, SET_REPORT 等，下面是对这些请求的实现. 

bmRequestType 中的 D[6:5] 为 01B 时即为类请求.

```c
// 类请求
else if(bmRequestType & 0x60 == 0x20) 
{
    switch(bRequest)
    {
        // 设置空闲请求
        case SET_IDLE:
        break;

        // 获取报告描述符请求
        case GET_REPORT:
        break;

        // 处理更多 HID 类请求 ...

        // 未定义的 HID 类请求
        default:
        break;
    }
}
```

#### 1) SET_IDLE

SET_IDLE 的编码为 0x0A, 这个请求告诉设备，在没有新的事件发生时，不要从中断端点返回数据。对于我们的USB鼠标来说，收到这个请求可以什么都不用干，直接返回一个0长度的状态数据包即可。修改端点0输出中断函数中的类输出请求部分代码如下：

```c
// 设置空闲请求
case SET_IDLE:
    // 发送空包
    send_length = 0;
break;
```

### 12.  报告的返回

通过前面报告描述符的定义知道要返回4字节的报告数据。

```c
// 端点1输入中断处理
void usb_end1_in(void)
{
    // 接收过配置请求
    if(config_value == 0)
        return ;

    char buf[4];
    // 获取鼠标当前状态
    // buf[0] 的 D0 就是左键, D1 就是右键, D2 就是中键
    // buf[1] 为X轴, buf[2] 为Y轴, buf[3] 为滚轮
    get_mouse_status(buf);
    // 发送鼠标状态
    USB_WRITE_ENDP_BUF(ENDP0, buf, 4);
}
```

至此, 这个 USB 实例就算完成了. 但还有很多请求没有实现, 如 GET_REPORT 等, 这可在后续自行完善.

