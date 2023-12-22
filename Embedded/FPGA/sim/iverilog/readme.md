
[gtkwave pdf 文档](https://gtkwave.sourceforge.net/gtkwave.pdf)

test.v

```v
module add #(
    parameter WIDTH = 8
) (
    input  [WIDTH-1:0] a,
    input  [WIDTH-1:0] b,
    output [WIDTH-1:0] c
);

    assign c = a + b;

endmodule

module top;

    parameter WIDTH = 8;

    reg [WIDTH-1:0] a, b;
    wire [WIDTH-1:0] c;

    reg clk;

    initial begin
        $dumpfile("wave.vcd");
        $dumpvars;
        a = 0;
        b = 1;
        clk = 0;
        #50000 $finish;
    end

    always #10 clk = ~clk;

    always @(posedge clk) begin
        a = a + 1;
        b = b + 1;
    end

    add #(8) u_add (
        .a(a),
        .b(b),
        .c(c)
    );

endmodule
```


```sh
iverilog -o test.vvp test.v
vvp test.vvp
gtkwave.exe wave.vcd
```

#### 交互模式

去掉仿真中的 `#50000 $finish;` 则 vpp 将一直执行并将波形输出到 wave.vcd, 这时后按 `Ctrl+C` 将暂停 vpp 的执行, 进入可交互的终端, 然后我们可以单步地执行

```sh
iverilog -o test.vvp test.v

# -s 进入后立即停止
vvp -s test.vvp

# 进入 vvp 交互模式
step

# 另一方面, 在另一个终端加载波形
gtkwave.exe  wave.vcd
```

gtkwave pdf 中提供一个使用 fifo 通信进行交互式仿真，但是没成功， 且只能在 linux 环境中实现

```sh
mkfifo wave.vcd
cver test.v &
shmidcat wave.vcd | gtkwave -v -I test.sav
```

## vpi

[参考文献](https://iverilog.fandom.com/wiki/Using_VPI)

vpi 可以扩展仿真软件, 使其运行 C 自定义的指令

hello.c

```c
#include <vpi_user.h>

static int hello_compiletf(char *user_data)
{
    (void)(user_data);
    return 0;
}

static int hello_calltf(char *user_data)
{
    vpi_printf("Hello, World! %s\n", user_data);
    return 0;
}

void hello_register(void)
{
    s_vpi_systf_data tf_data;

    tf_data.type = vpiSysTask;
    tf_data.tfname = "$hello";
    tf_data.calltf = hello_calltf;
    tf_data.compiletf = hello_compiletf;
    tf_data.sizetf = 0;
    tf_data.user_data = 0;
    vpi_register_systf(&tf_data);
}

void (*vlog_startup_routines[])(void) = {hello_register, 0};
```

hello.v

```v
module main;
    initial $hello;
endmodule
```

```sh
iverilog-vpi hello.c
iverilog -o hello.vvp hello.v
vvp -M. -mhello hello.vvp
```


参考文献
https://linux.die.net/man/1/vvp

