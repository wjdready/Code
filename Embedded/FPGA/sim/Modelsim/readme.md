


安装目录有大量的 examples 可以进行参考

> 注意: 文件夹路径中不能包含中文

test.v

```verilog
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

    initial begin
        a = 0;
        b = 1;
    end

    always begin
        a = a + 1;
        b = b + 1;
        #10;
    end

    add #(8) u_add (
        .a(a),
        .b(b),
        .c(c)
    );

endmodule
```

```sh

# 创建一个工作 lib, 可以不事先创建, vlog 时也会创建
vlib work

# 编译, 默认 work 文件夹, 不存在时会自动创建, 可指定 -work mywork
vlog test.v

# 运行仿真程序并指明 top 层, =r 表示只观察 reg 类型的信号
vsim -voptargs=+acc=r top

# 去掉 =r 则可以显示所有信号
vsim -voptargs=+acc top

# 运行地同时执行 tcl 指令, 可在 GUI 界面观察然后粘贴到此
vsim -voptargs=+acc=r work.top -do "add wave sim:/top/a sim:/top/b sim:/top/clk; run 10ms;"

# 运行然后退出 -c 不打开 GUI, 可用来生成波形或验证
vsim -voptargs=+acc=r work.top -do "run 10ms; exit;" -c
```
