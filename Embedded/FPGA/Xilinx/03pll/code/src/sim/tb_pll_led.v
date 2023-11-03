`timescale 1ns / 1ps

module tb_pll_led ();

    reg clk, rst_n;
    wire [1:0] out;

    led_flash led(clk, rst_n, out);

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
