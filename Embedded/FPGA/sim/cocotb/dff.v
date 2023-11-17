`timescale 1us / 1ns

module dff (
    input  clk,
    input  d,
    output logic q
);

    always @(posedge clk) begin
        q <= d;
    end

endmodule
