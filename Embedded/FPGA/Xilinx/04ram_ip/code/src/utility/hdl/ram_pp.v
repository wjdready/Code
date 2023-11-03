module ram_pp (
    input  wire clk,
    input  wire rst_n
);

    reg [7:0] din_a, din_b;
    reg [7:0] rd_addr_a, wr_addr_a, rd_addr_b, wr_addr_b;

    wire[7:0] dout_a, dout_b;
    wire wr_rama_en, wr_ramb_en;

    ram_wr256x8_rd256x8 ram_a (
        .clka (clk ), // input wire clka
        .wea (wr_rama_en ), // input wire [0 : 0] wea
        .addra(wr_addr_a ), // input wire [7 : 0] addra
        .dina (din_a ), // input wire [7 : 0] dina

        .clkb (clk ), // input wire clkb
        .addrb(rd_addr_a ), // input wire [7 : 0] addrb
        .doutb(dout_a ) // output wire [7 : 0] doutb
    );

    ram_wr256x8_rd256x8 ram_b (
        .clka (clk ), // input wire clka
        .wea (wr_ramb_en ), // input wire [0 : 0] wea
        .addra(wr_addr_b ), // input wire [7 : 0] addra
        .dina (din_b ), // input wire [7 : 0] dina

        .clkb (clk ), // input wire clkb
        .addrb(rd_addr_b ), // input wire [7 : 0] addrb
        .doutb(dout_b ) // output wire [7 : 0] doutb
    );

    always @(posedge clk or negedge rst_n) begin
        if(~rst_n) begin
            rd_addr_a <= 0;
            wr_addr_a <= 0;
        end
        
        else begin 
            rd_addr_a <= rd_addr_a + 1;
            din_a <= 254;
        end
    end

    always @(posedge clk or negedge rst_n) begin
        if(~rst_n) begin
            rd_addr_b <= 0;
            wr_addr_b <= 0;
        end
        
        else begin 
            wr_addr_b <= wr_addr_b + 1;
            din_b <= 254;
        end
    end

    assign wr_rama_en = 1;
    assign wr_ramb_en = 1;

endmodule
