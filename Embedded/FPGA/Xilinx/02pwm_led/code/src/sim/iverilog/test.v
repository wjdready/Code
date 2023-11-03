`timescale 1ns/1ps
`include "led_flash.v"

module test ();
    reg clk, rst_n;
    wire [1:0] out;

    led_flash led (
        clk,
        rst_n,
        out
    );

    always #1 clk = ~clk;

    initial begin
        clk   = 0;
        rst_n = 0;
        #10;
        rst_n = 1;
        $dumpfile("wave.vcd");
        $dumpvars;
        #5000 $finish;
    end
endmodule
