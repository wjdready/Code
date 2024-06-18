

# [2024-05-24 23:10:53]

[官网 wiki](https://wiki.sipeed.com/hardware/zh/tang/tang-primer-25k/primer-25k.html) 

[原理图](https://dl.sipeed.com/shareURL/TANG/Primer_25K/02_Schematic) 


| pin | function  |
| --- | --------- |
| E2  | 50MHz     |
| E8  | LED_READY |
| D7  | LED_DONE  |
| H11 | Key_S1    |
| H10 | Key_S2    |
| B3  | UART_RX   |
| C3  | UART_TX   |

> 注意: E2 E8 D7 作为特殊引脚, 需要进行进行双用途配置: Project->Configuration->Dual-Purpose Pin

