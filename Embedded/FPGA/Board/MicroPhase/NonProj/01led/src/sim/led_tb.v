`timescale 1 s / 1 ps

module led_tb;

    reg clk, rst_n;
    wire [1:0] led;

    initial begin
        clk   = 0;
        rst_n = 0;
        #10;
        rst_n = 1;
        forever #1 clk = ~clk;
    end

    led led_ins (
        .clk  (clk),
        .rst_n(rst_n),
        .led  (led)
    );

endmodule
