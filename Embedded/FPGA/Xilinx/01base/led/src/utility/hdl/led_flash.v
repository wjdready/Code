module led_flash (
    input  wire       clk,    // System differential clock
    input  wire       rst_n,  // system reset
    output reg  [1:0] led     // output led
);

    // ==================================================
    // internal singals
    // ==================================================

    reg [27:0] cnt;  // 0.25s counter
    wire add_cnt;
    wire end_cnt;
    // -------------------- cnt --------------------
    always @(posedge clk or negedge rst_n) begin
        if (rst_n == 1'b0) begin
            cnt <= 'd0;
        end else if (add_cnt) begin
            if (end_cnt) cnt <= 'd0;
            else cnt <= cnt + 1'b1;
        end
    end

    assign add_cnt = 1;
    assign end_cnt = add_cnt && cnt == 10_000_000 - 1;

    // -------------------- led --------------------
    always @(posedge clk or negedge rst_n) begin
        if (rst_n == 1'b0) begin
            led <= 2'b10;  // reset state
        end else if (end_cnt) begin
            led <= {led[0], led[1]};
        end else begin
            led <= led;  // keep the state
        end
    end

endmodule
