

# PL 端 demo

### 按键控制 LED 灯

```v
`timescale 1ns / 1ps

module demo (
    input wire clk,
    input wire rst_n,
    input wire key,
    output wire led
);
    
    // 用于对输入的信号进行打拍，消除亚稳态
    reg [1:0] key_dd;

    always @(posedge clk or negedge rst_n) begin
        // 通过位拼接实现
        key_dd <= rst_n == 1'b0 ? 2'b11 : {key_dd[0], key};
    end

    assign led = key_dd[1];

endmodule
```

约束文件, 通过 vivado 配置

```xdc
set_property PACKAGE_PIN P15 [get_ports led]
set_property PACKAGE_PIN N18 [get_ports clk]
set_property PACKAGE_PIN T12 [get_ports key]
set_property PACKAGE_PIN T16 [get_ports rst_n]

set_property IOSTANDARD LVCMOS33 [get_ports clk]
set_property IOSTANDARD LVCMOS33 [get_ports key]
set_property IOSTANDARD LVCMOS33 [get_ports led]
set_property OFFCHIP_TERM NONE [get_ports led]
set_property IOSTANDARD LVCMOS33 [get_ports rst_n]
```

我的示例

```v
`timescale 1ns / 1ps

module demo (
    input  wire clk,
    input  wire key,
    output wire led
);

    reg [27:0] cnt;

    always @(posedge clk) begin
        if (key == 0)
            cnt <= 0;
        else
            cnt <= cnt + 1;
    end

    assign led = cnt[26];

endmodule
```

对每个时钟上升沿进行判断, 当按键按下时，复位计数器 cnt, 这时输出总是为 0， 否则每个时钟周期加 1，取计数器的最高位进行输出。

因为计数器最高位会在前面所有位变 1 后完成一次翻转，2^26 个时钟周期后翻转一次，clk 直接接 50MHz 的晶振，因此翻转周期为 2^26  * (1 / 50M) = 1.34s
