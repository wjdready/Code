`timescale 1ns / 1ps

module tb_clk_wiz ();

    reg rst_n, clk;
    wire clk_out1, clk_out2, locked;

    clk_wiz_0 clock_inst (
        .clk_out1(clk_out1),
        .clk_out2(clk_out2),

        .reset (~rst_n),
        .locked(locked),

        .clk_in1(clk)
    );

    initial begin
        clk = 0;
        forever #(10) clk = ~clk;
    end

    initial begin
        rst_n = 0;
        #500;
        rst_n = 1;
    end

endmodule
