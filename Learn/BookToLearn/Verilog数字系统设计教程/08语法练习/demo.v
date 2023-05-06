

module test();

    reg A, B, F, G;

    initial begin
        A = 1'b1;
        B = 1'b0;
        #100;
        $display("%b %b", F, G);

    end

    always @(A or B) begin
        case (A)
            1'b0: F = B;
            1'b1: G = B;
        endcase
    end

endmodule
