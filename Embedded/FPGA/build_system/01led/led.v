module led (
    input wire clk,
    input wire rst_n,
    output reg [1:0] led
);
    // 实例化时钟生成器
    wire clk_out; // 声明一个中间信号以接收时钟输出

    clk_wiz_0 instance_name (
        // Clock out ports
        .clk_out1(clk_out),     // 将时钟输出连接到中间信号
        // Clock in ports
        .clk_in1(clk)           // 将输入时钟连接到 clk_wiz_0
    );

    // LED控制逻辑
    always @(posedge clk_out or negedge rst_n) begin
        if (!rst_n) begin
            led[0] <= 1'b0; // 复位时LED熄灭
        end else begin
            // 这里可以根据需要控制LED的状态
            led[0] <= ~led[0]; // 反转 LED0 的状态
        end
    end

    reg [26:0] cnt;         // 计数器，27位可以计数到2^27-1（约134M）

    // 计数器逻辑
    always @(posedge clk_out or negedge rst_n) begin
        if (!rst_n) begin
            cnt <= 27'b0;   // 复位时计数器清零
            led[1] <= 1'b0;   // 复位时LED熄灭
        end else begin
            if (cnt == 27'd50000) begin // 计数到500000 (100MHz / 100Hz / 2)
                cnt <= 27'b0;           // 计数器清零
                led[1] <= ~led[1];      // 反转 LED0 的状态
            end else begin
                cnt <= cnt + 1;         // 计数器加1
            end
        end
    end

endmodule
