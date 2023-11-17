`timescale 1ns/1ps

module tb_ram_pp ();

    reg clk, rst_n;

    ram_pp ram_pp_ins(clk, rst_n);

    initial begin
        rst_n = 1;
        clk = 0;
        forever #10 clk = ~clk;
    end

endmodule
