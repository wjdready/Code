

# [2023-04-27 11:31:36]

[Z7-Lite-7010 板卡资料和教程](https://pan.baidu.com/s/1s9o3pKul2eTkDvlHvv3tog)，提取码：h49u

[ZYNQ开发视频教程](https://pan.baidu.com/s/13AkvW0DD1VyU1PcfRlYvxQ) 提取码：9fmh

根据 Part1_Z7_Lite系列教程之逻辑篇的 pdf 安装 Vivado 和 Modelsim 即可。

## 配置 Modelsim 仿真

使用 Modelsim 来进行仿真需要输出 Xlib 仿真库，选择 Tools->Compile Simulation Libraries

然后 Tools->Settings->3rd Party Simulators 选择对应的路径即可.

**发现 Vivado 也是支持 TCL 命令的**

# [2023-04-27 11:41:54]

## LED 工程

创建一个 RTL Project, 源文件和约束文件可稍后添加，选择芯片型号如下 (开发板的芯片型号为 zynq-xc7z010-clg400 即使400 pin 的封装)

因此 Family 栏中选择 zynq-7000 系列，Package 选择 clg400, 速度等级 1, 最终筛选的型号就是: xc7z010clg400-1

当然，这里选中型号后，进入工程后也可以重新进行设置型号，在 Project Manager 设置即可.

整个工程文件结构如下:

```
led_flash.cache/
led_flash.hw/
led_flash.ip_user_files/
led_flash.sim/
led_flash.srcs/
led_flash.xpr
```

其中 led_flash.srcs 用于存放源文件, led_flash.xpr 则是 Vivado 工程文件

添加 design sources

lcd_flash.v

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

top_pin.xdc

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

这里需要注意文字的大小写，端口名称是数组的话用 { } 刮起来， 端口名称必须和源代码中的名字一致，且端口名字不能和关键字一样。

设置完成之后运行 

* SYNTHESIS (综合) ，将高级抽象层次的电路描述转化为较低层次的描述。
  即将语言描述的电路逻辑转化为与门、或门、非门、触发器等基本逻辑单元的互连关系。
  综合后生成的门级网表只是表示了门与门之间的虚拟的链接关系，并没有规定每个门的位置以及连线的长度等。

* IMPLEMENTATION (实现)：布局+布线，决定了每个门的位置以及连线的长度等

## 烧录

生成比特流 (bitstrean) 文件并通过 JTAG 烧录到片上, 但是掉电就会消失

下载到 QSPI 中，并设置启动位从 QSPI 中启动:

要下载到 QSPI 中, 可在 Hardware Manager 中选择型号右击，添加 Memory Device.

# [2023-04-27 16:27:13]

自动化流程

vivado 命令和TCL 语言的支持使得我们可以让很多工作自动化起来.

为了环境的干净, 这里不打算直接将 vivado 放到环境变量, 而是用到的时候再临时加入环境变量:

fpga.bat

```
set PATH=%PATH%;C:\ProgramFiles\Xilinx\Vivado\2019.1\bin
```

这时打开 cmd, 输入 fpga 再输入 `vivado -help` 可以看到 vivado 正确执行

要在 msys2 中使用, 可定义 fpga.sh 然后再 `source fpga.sh`

fpga.sh

```
#!/bin/bash
export PATH=$PATH:C:\\ProgramFiles\\Xilinx\\Vivado\\2019.1\\bin
```

vivado 命令常用参数:

```
vivado -mode batch -nojournal -nolog -notrace -quiet
```

# [2023-04-28 09:42:51]

使用 VSCode 进行编辑, 让打开时指定文件和文件夹, 可在 Vivado 中添加 Custom Editor:

```
C:/ProgramFiles/VSCode/Code.exe --goto [file name]:[line number]
```

