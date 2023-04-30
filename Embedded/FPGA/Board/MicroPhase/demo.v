always @(posedge clk)
    case (a)
        0: b <= 0;
        1: b <= 1;
        2: b <= 2;
        default: b <= 0;
    endcase
