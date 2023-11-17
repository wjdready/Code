module connnect (
    input  wire       clk1,
    input  wire       rst_n1,
    input reg  [1:0] led1,

    output  wire       clk2,
    output  wire       rst_n2,
    output wire [1:0] led2
);

    assign clk2 = clk1;
    assign rst_n2 = rst_n1;
    assign led2 = led1;

endmodule
