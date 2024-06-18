`timescale 1us/1ns

module demo (
    input clk,
    input rst_n,
    output out
);

    reg [1:0] cnt;

    always @(posedge clk or negedge rst_n) begin
        if (~rst_n) begin
            cnt <= 0;
        end else begin
            cnt <= cnt + 1;
        end
    end

    assign out = cnt[1];

endmodule

module top;

    reg clk;
    reg rst_n;

    // initial begin
    //     $dumpfile("wave.vcd");
    //     $dumpvars;
    //     #5000 $finish;
    // end
    
    // 复位信号
    initial begin
        rst_n = 0;
        #10 rst_n = 1;
    end

    // 时钟信号
    initial begin
        clk = 0;
        forever #2.5 clk = ~clk; // 200k
    end

    wire out;

    demo u_demo (
        .clk     (clk),
        .rst_n   (rst_n),
        .out     (out)
    );

endmodule

