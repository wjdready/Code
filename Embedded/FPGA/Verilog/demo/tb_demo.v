`timescale 1ns/1ps

module tb_demo();
    reg a;
    reg b;
    wire c;

    demo DUT(
        .a(a),
        .b(b),
        .c(c)
    );


    initial begin
        
        // 导出波形文件
        $dumpfile("wave.vcd"); 
        $dumpvars; 

        a = 0;
        b = 0;
        #100;
        a = 1;
        b = 0;
        #100;
        a = 0;
        b = 1;
        #100;
        a = 1;
        b = 1;
        #100;
        
    end

endmodule
