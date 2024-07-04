
千兆网 arp 请求发送测试

top.v

```v
module top (
    input clk,
	input rst_n,
    output rgmii_gtx_clk,
	output [3:0]rgmii_tx_data,
	output rgmii_tx_en,
	output phy_rst_n
);

    assign phy_rst_n = rst_n;

    wire gmii_gtx_clk;
    Gowin_rPLL Gowin_rPLL(
        .clkout(gmii_gtx_clk),  // output clkout 125MHz
        .clkin(clk)             // input clkin
    );

    localparam PREAMBLE_BYTES = 64'h55555555555555D5;

    localparam CRC = 32'hFC63BC0B; 
	localparam [31:0]CRC_result = {CRC[7:0],CRC[15:8],CRC[23:16],CRC[31:24]};

    localparam [480 - 1: 0] arp_data  = 480'hffffffffffff000a3501fec008060001080006040001000a3501fec0c0a8080f000000000000c0a808660000ffffffffffff0023cd76631a08060001;
    localparam [480 + 64 + 32 - 1 : 0] lut = {PREAMBLE_BYTES, arp_data, CRC_result};

    reg [7:0] tx_data;
    reg [11:0] cnt_send;
    reg tx_enable;
    always @(posedge gmii_gtx_clk or negedge rst_n) begin
        if (~rst_n) begin
            cnt_send <= 0;
            tx_enable <= 0;
            tx_data <= 0;
        end else begin

            cnt_send <= cnt_send + 12'd1;

            if (cnt_send == 72) begin
                tx_enable <= 0;
                tx_data <= 8'd0;
            end else if (cnt_send < 72) begin
                tx_enable <= 1;
                tx_data <= lut[ ((72 - cnt_send) * 8 - 1) -: 8];
            end
        end
    end

    gmii_to_rgmii gmii_to_rgmii(
	    .reset_n(rst_n),
		.gmii_tx_clk(gmii_gtx_clk),
		.gmii_txd(tx_data),
		.gmii_txen(tx_enable),
		.gmii_txer(1'b0),
		.rgmii_tx_clk(rgmii_gtx_clk),
		.rgmii_txd(rgmii_tx_data),
		.rgmii_txen(rgmii_tx_en)
	);
    
endmodule
```

gmii 转 rgmii (高云平台的 ODDR 原语实现)

gmii_to_rgmii.v

```v
module gmii_to_rgmii (
    reset_n,

    gmii_tx_clk,
    gmii_txd,
    gmii_txen,
    gmii_txer,

    rgmii_tx_clk,
    rgmii_txd,
    rgmii_txen
);

    input reset_n;

    input gmii_tx_clk;
    input [7:0] gmii_txd;
    input gmii_txen;
    input gmii_txer;

    output rgmii_tx_clk;
    output [3:0] rgmii_txd;
    output rgmii_txen;

    generate
        genvar i;
        for (i = 0; i < 4; i = i + 1) begin
            ODDR U_ODDR_dq1 (
                .Q0 (rgmii_txd[i]),
                .Q1 (),
                .D0 (gmii_txd[i]),
                .D1 (gmii_txd[i+4]),
                .TX (1),
                .CLK(gmii_tx_clk)
            );
        end
    endgenerate

    ODDR U_ODDR_en1 (
        .Q0(rgmii_txen),
        .Q1(),

        .D0(gmii_txen),
        .D1(gmii_txen ^ gmii_txer),
        .TX(1),

        .CLK(gmii_tx_clk)
    );

    ODDR U_ODDR_clk1 (
        .Q0(rgmii_tx_clk),
        .Q1(),

        .D0(1),
        .D1(0),
        .TX(1),

        .CLK(gmii_tx_clk)
    );

endmodule
```

仿真

```v
`timescale 1ns/1ns

module top_tb;

    GSR GSR(.GSRI(1'b1));
 
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
        $dumpvars(0, top_tb);
        #500000;
        $stop;
    end
    
    wire rgmii_gtx_clk;
    wire [3:0] rgmii_tx_data;
    wire rgmii_tx_en;
    wire phy_rst_n;

    top  u_top (
        .clk                     ( clk             ),
        .rst_n                   ( rst_n           ),

        .rgmii_gtx_clk           ( rgmii_gtx_clk   ),
        .rgmii_tx_data           ( rgmii_tx_data   ),
        .rgmii_tx_en             ( rgmii_tx_en     ),
        .phy_rst_n               ( phy_rst_n       )
    );

endmodule
```
