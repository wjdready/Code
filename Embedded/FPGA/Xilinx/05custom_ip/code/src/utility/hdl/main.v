module cutsom_ip_demo (
    input  wire       clk,
    input  wire       rst_n,
    output reg  [1:0] led
);

    wire [1:0] led_out;

    led_flash_1 led(
        .clk(clk),
        .rst_n(rst_n),
        .led(led_out)
    );

    assign led_out = led;

endmodule
