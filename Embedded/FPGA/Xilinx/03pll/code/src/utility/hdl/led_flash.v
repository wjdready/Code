module led_flash (
    input  wire clk,
    input  wire rst_n,
    output wire [1:0] led
);

    wire clk_out2, clk_out1;

    clk_wiz_0 clock_inst (
        .clk_out1(clk_out1),
        .clk_out2(clk_out2),

        .reset(~rst_n),
        .locked(locked),

        .clk_in1(clk)
    );

    parameter CLK_HZ = 50_000_000;
    parameter CNT_1S = 1 * CLK_HZ - 1;
    parameter PER = CLK_HZ / 100;

    reg [32:0] count, duty;
    reg pwm;

    always @(posedge clk_out2 or negedge rst_n) begin
        if (~rst_n) begin 
            count <= 0;
            pwm <= 0;
        end

        else begin
            if(count == CNT_1S) begin
                count <= 0;
                pwm <= ~pwm;
            end
            else 
                count <= count + 1;
            // pwm <= count < duty ? 1 : 0;
        end
    end

    // always @(posedge clk or negedge rst_n) duty <= ~rst_n ? 0 : duty + 1;

    assign led = {pwm, ~pwm};

endmodule
