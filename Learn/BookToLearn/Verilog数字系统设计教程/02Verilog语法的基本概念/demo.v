module muxtwo(out, a, b, sl);
    input a, b, sl;
    output out;
    reg out;

    always @ (sl or a or b) begin
        if (! sl) out = a;
        else out = b;
    end

endmodule

module test ();

    reg a, b, sl;
    reg clock;
    
    wire out;

    muxtwo m(out, a, b, sl);
    
    initial begin
        $dumpfile("wave.vcd");
        $dumpvars;

        a = 0;
        b = 0;
        sl = 0;
        clock = 0;

        // 50000 单位时间后停止仿真
        #50000 $finish;
    end

    // 产生一个不断重复地周期为 100 个时钟信号的 clock
    // 也可以简写成 always #50 clock = ~clock;
    always begin
        #50;
        clock = ~clock;
    end

    always @ (posedge clock) begin
        #1 a = $random % 2;
        #3 b = $random % 2;
    end

    always #10000 sl = !sl;

endmodule
