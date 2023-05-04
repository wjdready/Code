module test;
    reg [3:0] memory[0:7];
    integer n;

    initial begin
        if ($test$plusargs("DISPLAY_VAR"))
            $display("Display Var");
        else
            $display("No Display");
    end
endmodule
