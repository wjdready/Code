

以下是一个百兆网的 arp 循环发送

arp_data 在 wireshark 抓包提取得到的实际 arp 包, crc 通过软件计算得到

rmii_arp.v

```v
module rmii_arp (
    input clk_50M,
    input rst_n,

    output reg [1:0]  phy_rmii_tx,
    output reg phy_rmii_txen
);

    localparam PREAMBLE_BYTES = 64'h55555555555555D5;

    localparam CRC = 32'hFC63BC0B; 
    localparam [31:0]CRC_result = {CRC[7:0],CRC[15:8],CRC[23:16],CRC[31:24]};

    localparam [480 - 1: 0] arp_data  = 480'hffffffffffff000a3501fec008060001080006040001000a3501fec0c0a8080f000000000000c0a808660000ffffffffffff0023cd76631a08060001;
    localparam [480 + 64 + 32 - 1 : 0] lut = {PREAMBLE_BYTES, arp_data, CRC_result};

    // -------------------------------------------------------------------------
    //                  50MHz 分频生成 12.5MHz 时钟 
    // -------------------------------------------------------------------------
    reg clk_12M5;
    reg [4:0] cnt_clk;
    localparam CLK_CNT = 50_000_000 / 12_500_000 / 2 - 1;
    always @(posedge clk_50M or negedge rst_n) begin
        if (~rst_n) begin
            cnt_clk <= 'd0;
            clk_12M5 = 0;
        end else if (cnt_clk == CLK_CNT) begin
            cnt_clk <= 4'd0;
            clk_12M5 = ~clk_12M5;
        end
        else begin
            cnt_clk <= cnt_clk + 4'd1;
        end
    end

    // -------------------------------------------------------------------------
    //                    填充数据
    // -------------------------------------------------------------------------
    reg [7:0] tx_data;
    reg [15:0] cnt_send;
    reg tx_enable;
    always @(posedge clk_12M5 or negedge rst_n) begin
        if (~rst_n) begin
            cnt_send <= 0;
            tx_enable <= 0;
            tx_data <= 0;
        end else begin

            cnt_send <= cnt_send + 16'd1;

            if (cnt_send == 72) begin
                tx_enable <= 0;
                tx_data <= 8'd0;
            end else if (cnt_send < 72) begin
                tx_enable <= 1;
                tx_data <= lut[ ((72 - cnt_send) * 8 - 1) -: 8];
            end
        end
    end

    // -------------------------------------------------------------------------
    //                    发送接口
    // -------------------------------------------------------------------------
    reg [1:0] step;
    always @(posedge clk_50M or negedge rst_n) begin
        if (~rst_n) begin
            step <= 'd0;
            phy_rmii_tx <= 0;
            phy_rmii_txen <= 0;
        end else if (tx_enable) begin
            phy_rmii_txen <= 1;
            case (step)
                2'd0: phy_rmii_tx <= tx_data[1:0];
                2'd1: phy_rmii_tx <= tx_data[3:2];
                2'd2: phy_rmii_tx <= tx_data[5:4];
                2'd3: phy_rmii_tx <= tx_data[7:6];
            endcase
            step <= step + 2'd1;
        end else begin
            step <= 'd0;
            phy_rmii_tx <= 2'd0;
            phy_rmii_txen <= 0;
        end
    end

endmodule
```

仿真文件 rmii_arp_tb.v

```v
`timescale 1ns / 1ns

module rmii_arp_tb;

    // -------------------------------------------------------------------------
    //                    clk and reset signal 
    // -------------------------------------------------------------------------
    reg clk;
    reg rst_n;
    initial clk = 0;
    always #10 clk = ~clk;

    initial begin
        rst_n = 0;
        #200;
        rst_n = 1;
    end

    initial begin
        $dumpvars(0, rmii_arp_tb);
        #500000;
        $finish;
    end

    wire [1:0] phy_rmii_tx;
    wire phy_rmii_txen;
    rmii_arp u_rmii_arp (
        .clk_50M       (clk),
        .rst_n         (rst_n),

        .phy_rmii_tx  (phy_rmii_tx),
        .phy_rmii_txen(phy_rmii_txen)
    );

endmodule
```


