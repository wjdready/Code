
module adder ( co, sum, a0, al, ci);

    parameter N = 4;

    output [N - 1: 0] sum;
    output co;

    input [N - 1: 0] a0, al;
    input ci;

    generate
        case (N)
            1: adder_1bit adder1 (co, sum, a0, al, ci);
            2: adder_2bit adder2 (co, sum, a0, al, ei);
            //默认的情况下选用位宽为 N 位的超前进位加法器
            default: adder_cla #(N) adder3 (co, sum, a0, al, ci);
        endcase
    endgenerate

endmodule


module test ();
    
    parameter N = 4;

    wire [N - 1: 0] out;
    wire co;

    reg [N - 1: 0] a0, a1;

    adder #(N) m(co, out, a0, a1, 1'b0);

    initial begin
        $dumpfile("wave.vcd");
        $dumpvars;

        repeat(100) begin
            a0 = $random;
            a1 = $random;
            #10;
        end
    end
endmodule
