module test ();
    
    reg a, b, c, clock;

    initial begin
        $dumpfile("wave.vcd");
        $dumpvars;
        #500 $finish;
    end

    initial begin
        clock = 0;
        {a, b, c} = 0;
    end

    always #10 clock = ~clock;

    always @(posedge clock) begin
        b = a;
        a = 1;
        c = b;
    end

endmodule
