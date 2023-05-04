
// module mm(inout [7:0] dat, [7:0] index);

//     parameter opt = 1;

//     reg [7:0] mem[255:0];

//     always begin
//         if(opt) mem[index] = dat;
//         else dat = mem[index];
//     end

// endmodule

module test ();
    
    // reg [7:0] dat;
    
    // mm m(dat, )

    reg [7:0] mem[255:0];

    reg [7:0] x;
    reg clock;
    reg [7:0]i;

    initial begin
        $dumpfile("wave.vcd");
        $dumpvars;
        #5000 $finish;
    end

    initial begin
        clock = 0;
        x = 0;
        for (i = 0; i < 255; i++) begin
            mem[i] = i;
        end

        i = -1;
    end

    always #10 clock = ~clock;

    always @(posedge clock) begin
        i++;
        x = mem[i];
    end
    
endmodule
