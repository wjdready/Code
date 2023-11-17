`timescale 1 ns / 1 ps

module top (
    input wire clk,
    input wire rst_n,
    output wire [1:0] led
);

    led_0 led_ins (
        .clk  (clk),
        .rst_n(rst_n),
        .led  (led)
    );

endmodule
