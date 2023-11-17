
testbench 文件本身也是一个模块，但是可以不需要输入和输出，全走内部信号, 比如:

```v
`timescale 1ns/1ns

module test();

    reg a;

    initial begin
        a = 0;
        #100
        a = 1;
    end

    always begin
        #10;
        if ($time >= 150)
            $finish;
    end

endmodule
```

> a 的值将维持低电平 100ns, 然后维持高电平一直到仿真结束，这里 always 语句将时刻检测仿真时间，若大于 150 则结束仿真
> 因此最终结果就是 a 低电平维持 100ns，然后高电平 50ns

## 调用模块

将 testbench 中的内部信号输出到调用的模块即可对模块进行仿真，调用模块的方法是 

```
module_name custom_name(
    .参数1(本地变量1),
    .参数2(本地变量2),
);
```

例如下面是一个取非模块的调用:

```v
module not_module (
    input wire in, 
    output wire out
);

    assign out = !in;

endmodule


module test();

    reg a;
    wire b;

    not_module u_not_module(
        .in(a),
        .out(b)
    );

    initial begin
        a = 0;
        #100
        a = 1;
    end

    always begin
        #10;
        if ($time >= 150)
            $finish;
    end

endmodule
```

> 在一个源文件中写多个模块是可以的，Modelsim 中的 Library 中将会显示文件中的多个模块, 仿真时对顶层的 testbench 模块 test 进行仿真即可.

每次编译过后都要重新打开仿真?

## 自动仿真

Makefile

```makefile
export PATH := $(PATH):/c/ProgramFiles/Modelsim/win64/vsim

test:
	vlog -work work test.v not_module.v
	vsim -voptargs=+acc work.test -do "add wave sim:/test/a sim:/test/b; run 10ms;"
```

添加波形可先在图形界面添加，下方会有对应的 tcl 脚本，从而能够直接复制粘贴

