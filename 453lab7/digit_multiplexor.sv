module digit_multiplexor (
    input  logic [3:0] sec_dig1,
    input  logic [3:0] sec_dig2,
    input  logic [3:0] min_dig1,
    input  logic [3:0] min_dig2,
    input  logic [3:0] selector,
    input  logic [3:0] decimal_point, 
    output logic [3:0] time_digit,
    output logic       dp_in
);

    always_comb begin
        case (selector)
            4'b0001: time_digit = sec_dig1;   // display seconds digit
            4'b0010: time_digit = sec_dig2;   // display tens of seconds digit
            4'b0100: time_digit = min_dig1;   // display minutes digit
            4'b1000: time_digit = min_dig2;   // display tens of minutes digit
            default: time_digit = 4'b0000;    // default case
        endcase
    end

    always_comb begin
        case (selector)
            4'b0001: dp_in = decimal_point[0];   // DP right of seconds digit
            4'b0010: dp_in = decimal_point[1];   // DP right of tens of seconds digit
            4'b0100: dp_in = decimal_point[2];   // DP right of minutes digit
            4'b1000: dp_in = decimal_point[3];   // DP right of tens of minutes digit
            default: dp_in = 0;    // default case (all DP are OFF)
        endcase
    end

endmodule
