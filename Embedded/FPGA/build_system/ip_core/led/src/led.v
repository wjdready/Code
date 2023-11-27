`timescale 1 ns / 1 ps

module led (
    input wire clk,
    input wire rst_n,
    output reg [1:0] led
);
    reg [31:0] cnt;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            led <= 2'b1;
            cnt <= 32'd0;
        end else begin
            // 开发板使用晶振为 50MHz
            if (cnt >= 32'd49_999_999) begin
                led <= {led[0], led[1]};
                cnt <= 32'd0;
            end else begin
                cnt <= cnt + 32'd1;
            end
        end
    end
endmodule
