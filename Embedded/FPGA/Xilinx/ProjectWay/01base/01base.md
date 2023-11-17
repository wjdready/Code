
## LED 工程

创建一个名为 led 的 RTL Prject, 型号选 xc7z010clg400-1, 然后生成工程文件 .xpr

现在重新整理工程文件, 组织成如下形式

```
led
├── proj
│   └── led.xpr
└── src
    └── utility
        ├── hdl
        │   └── led_flash.v
        └── xdc
            └── test_led_flash_pin.xdc
```

## 源文件

led_flash.v

```v
module led_flash (
    input  wire       clk,    // System differential clock
    input  wire       rst_n,  // system reset
    output reg  [1:0] led     // output led
);

    // ==================================================
    // internal singals
    // ==================================================

    reg [27:0] cnt;  // 0.25s counter
    wire add_cnt;
    wire end_cnt;
    // -------------------- cnt --------------------
    always @(posedge clk or negedge rst_n) begin
        if (rst_n == 1'b0) begin
            cnt <= 'd0;
        end else if (add_cnt) begin
            if (end_cnt) cnt <= 'd0;
            else cnt <= cnt + 1'b1;
        end
    end

    assign add_cnt = 1;
    assign end_cnt = add_cnt && cnt == 10_000_000 - 1;

    // -------------------- led --------------------
    always @(posedge clk or negedge rst_n) begin
        if (rst_n == 1'b0) begin
            led <= 2'b10;  // reset state
        end else if (end_cnt) begin
            led <= {led[0], led[1]};
        end else begin
            led <= led;  // keep the state
        end
    end

endmodule
```


## 约束

Vivado 使用的约束文件格式是 xdc 文件, xdc 文件里主要是定义管脚约束,时钟约束, 以及其他时序约束。

里我们需要对 led.v 程序中的输入输出端口分配到 FPGA 的管脚上, 这就需要将管脚分配的 .xdc 文件添加到工程中

test_led_flash_pin.xdc

```xdc
############## clock define##################
create_clock -period 20.000 [get_ports clk]
set_property PACKAGE_PIN N18 [get_ports clk]

set_property IOSTANDARD LVCMOS33 [get_ports clk]
############## key define##################
set_property PACKAGE_PIN P16 [get_ports rst_n]
set_property IOSTANDARD LVCMOS33 [get_ports rst_n]

############## LED define ##################
set_property PACKAGE_PIN P15 [get_ports {led[0]}]
set_property PACKAGE_PIN U12 [get_ports {led[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports rst_n]
set_property IOSTANDARD LVCMOS33 [get_ports {led[*]}]
set_property IOSTANDARD LVCMOS33 [get_ports clk]
```

约束文件如何写，可以参考 xilinx 的相关文档，下面来介绍一下最基本的 XDC 编写的语法，普通 IO 口只需约束引脚号和电压，管脚约束如下：

```
set_property PACKAGE_PIN 引脚编号 [get_ports 端口名称]
```

电平信号的约束如下：

```
set_property IOSTANDARD 电压 [get_ports 端口名称]
```

这里需要注意文字的大小写，端口名称是数组的话用 { } 括起来， 端口名称必须和源代码中的名字一致，且端口名字不能和关键字一样。

设置完成之后运行 

* SYNTHESIS (综合) ，将高级抽象层次的电路描述转化为较低层次的描述。
  即将语言描述的电路逻辑转化为与门、或门、非门、触发器等基本逻辑单元的互连关系。
  综合后生成的门级网表只是表示了门与门之间的虚拟的链接关系，并没有规定每个门的位置以及连线的长度等。

* IMPLEMENTATION (实现)：布局+布线，决定了每个门的位置以及连线的长度等


## 烧录

生成比特流 (bitstrean) 文件并通过 JTAG 烧录到片上, 但是掉电就会消失

下载到 QSPI 中 (需要 SDK 支持, 通过 FSBL.elf) 来实现
