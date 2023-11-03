module led_flash (
    input  wire clk,
    input  wire rst_n,
    output wire [1:0] led
);

    parameter CLK_HZ = 50_000_000;
    parameter CNT_1S = 1 * CLK_HZ - 1;
    parameter PER = CLK_HZ / 100;
    
    reg [32:0] count, duty;
    reg pwm;

    always @(posedge clk or negedge rst_n) begin
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
