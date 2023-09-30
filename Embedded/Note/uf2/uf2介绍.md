
## 概述

UF2 是微软为 PXT (Programming eXperience Toolkit 编程体验工具包) 开发的一种文件格式，特别适合通过 USB 的 MSC (大容量存储类) 来将程序烧录到单片机。

因为大多数 PC 都支持 USB 主机模式和 MSC 类， 因此可以用来作为一种快速固件升级的方案，而无需依赖任何驱动和上位机，即直接将文件复制到文件系统即可完成升级。

UF2文件由一个个512字节的块组成，它们之间都是相互独立的。

每个块的结构如下:

* 开头和结尾的幻数
* 要烧录的flash地址
* 最多 476 字节的数据

通过 MSC 传输的数据始终以 512 字节的倍数到达单片机，包括与 FAT 文件系统结构相关的数据在内，这意味着 UF2 文件的块始终与 MSC 写入的块对齐。

幻数可以让微控制器将 UF2 文件块与其他数据（如 FAT 文件系统表等）区分开来。当识别出 UF2 文件块时，它可以立即烧录到 flash 或将多次数据放到缓存区再一次性写入。

## UF2 文件结构

UF2 文件中块的结构如下:

每个块都以32字节头开始，后跟数据和最后一个幻数。除数据外，所有字段都是32位无符号小端整数。

| Offset | Size | Value                                           |
| ------ | ---- | ----------------------------------------------- |
0      	 4    	 magicStart0 第一个幻数 0x0A324655 ("UF2\n")     
4      	 4    	 magicStart1 第二个幻数 0x9E5D5157               
8      	 4    	 flags 标志位                                    
12     	 4    	 targetAddr 要写入的 flash 地址                  
16     	 4    	 payloadSize 数据区域中数据为多少字节 (通常 256) 
20     	 4    	 blockNo 当前块号，从 0 开始, 编号是连续的       
24     	 4    	 numBlocks 文件中的数据块总数                    
28     	 4    	 fileSize/familyID 文件大小或主板ID              
32     	 476  	 data[476] 数据区域                              
508    	 4    	 magicEnd 末尾幻数, 0x0AB16F30                   

对应的 C 语言结构体如下:

```c
struct UF2_Block {
    // 32 byte header
    uint32_t magicStart0;
    uint32_t magicStart1;
    uint32_t flags;
    uint32_t targetAddr;
    uint32_t payloadSize;
    uint32_t blockNo;
    uint32_t numBlocks;
    uint32_t fileSize; // or familyID;
    uint8_t data[476];
    uint32_t magicEnd;
} UF2_Block;
```

## flags

目前，定义了五个标志:

0x00000001 - not main flash (非主闪存) - 在写入设备闪存时应跳过此块；它可用于在文件中存储一些说明性字符串，通常是嵌入的源代码或调试信息
0x00001000 - file container (文件容器) - 见下文
0x00002000 - familyID present (当前系列ID) - 设置后，fileSize/familyID 包含标识主板系列的值(通常对应于MCU)
0x00004000 - MD5 checksum present (当前 MD5 校验) - 见下文
0x00008000 - extension tags present (当前扩展标记) - 见下文

## Family ID

此字段是可选的，仅当设置了相应的标志时才应设置该字段。

一般来说 BootLoader 都应该适当设置该字段，并且拒绝在没有该字段的情况下烧录 UF2 文件。从而防止错误烧录其他板子上的 uf2 程序。

可以将几个具有不同 Family ID 的文件简单地串联在一起，并将整个生成的文件复制到设备上，而实际上只有一个文件被写入闪存。同时因为该文件具有多个不同的 Family ID， 因此可以用于多个板子的直接烧录。虽然通过 USB 传输了多个文件组成的 uf2 文件，但每个板子只会关心和使用自己对应 Family ID 的那一部分。

## 如此设计的优点

第二个和末尾的幻数是随机选择的，除了末尾幻数的最后一个字节，它被强制为'\n'（0xA）。与第一个幻数 "UF2\n" 一起，这是我们可以轻松地在文本编辑器中识别 UF2 块。

标头填充为 32 字节，因为十六进制编辑器通常使用 16 或 32 字节作为行长度，数据位置就与行开头对齐。这样就可以方便的地十六进制编辑器中查看 uf2 文件

另外所有字段均使用 32 位整数，以便将来可以支持大闪存尺寸。同时也为了简单起见，uf2 使用小端字节序，因为大多数微控制器都是小端字节序。

文件中的块总数和顺序块编号使引导加载程序可以轻松检测到所有块都已传输。


## 数据大小 payloadSize

数据字节的数量是可配置的，并且取决于微控制器上闪存页的大小（即可以擦除的最小大小）。

* 如果闪存一页的大小超过476字节，引导加载程序应该支持任何 payloadSize，因为无论如何它都需要在内存中缓冲整个页面
* 如果闪存页大小小于476字节，则 payloadSize 应该是页面大小的倍数，因此可以在不缓冲的情况下写入；目标地址也应该是页面大小的倍数。
* 无论如何，payloadSize 和目标地址应始终是 4 字节对齐。

一般来说有效负载大小 256 始终正确，并且可以轻松在闪存地址和 UF2 文件偏移量之间进行转换，因为现代闪存的最小擦除大小一般都会大于 512 字节以上。

## 引导加载程序暴露的文件

引导加载程序可能会公开其 MSC 设备中的虚拟文件。以便刷新工具可以自动检测引导加载程序。所需的是 MSC 设备根目录中需包含如下文件:

* INFO_UF2.TXT- 包含有关引导加载程序构建及其运行的主板的信息，刷写工具可以将该文件的存在用来指示目标板已经连接，并读取出板子的信息。
  典型的INFO_UF2.TXT文件如下所示:

  ```
  UF2 Bootloader v1.1.3 SFA
  Model: Arduino Zero
  Board-ID: SAMD21G18A-Zero-v0
  ```

  该Board-ID字段是机器可读的，由许多用破折号分隔的标记组成。第一个标记是 CPU 类型，第二个是主板类型，第三个是主板版本。还可以添加更多标记。
  引导加载程序应将其信息文件作为静态字符串包含在其代码中的某处。
* INDEX.HTM- 重定向到包含 IDE 或其他信息的页面
* CURRENT.UF2 - 设备整个闪存的内容


## 扩展标签

当设置该0x8000标志时，可以在 payloadSize 数据之后附加额外的信息（即从 32 + payloadSize 开始）。例如用于描述固件的版本信息，固件的 SHA-2 校验和，目标设备页面大小等。

## 在 STM32 上实现

使用 Cubemx 生成的 USB MSC 类驱动可以很快实现出具有 UF2 功能的 BootLoader, 最简单的方法便是在 

usbd_storage_if.c 中添加如下函数

```c
int uf2_write_block(uint32_t block_no, uint8_t *data)
{
    (void)block_no;
    UF2_Block *bl = (void *)data;

    if (!is_uf2_block(bl))
        return -1;

    if (bl->familyID == BOARD_UF2_FAMILY_ID)
    {
        // generic family ID
        if(bl->targetAddr % STMFLASH_SECTOR_SIZE == 0)
        {
            bootflash_erase(bl->targetAddr);
        }

        flash_write(bl->targetAddr, bl->data, bl->payloadSize);
        // printf("block_no(#0x%x) \r\n", block_no);
    }
    else
    {
        // TODO family matches VID/PID
        return -1;
    }

    //------------- Update written blocks -------------//

    return SECTOR_SIZE;
}
```