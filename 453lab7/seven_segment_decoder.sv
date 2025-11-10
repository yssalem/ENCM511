
module seven_segment_decoder (
    output logic       CA,
    output logic       CB,
    output logic       CC,
    output logic       CD,
    output logic       CE,
    output logic       CF,
    output logic       CG,
    output logic       DP,
    input  logic       dp_in,
    input  logic [3:0] data
);

    logic [6:0] decoded_bits;

    always_comb begin
        // Decode the input data into 7-segment display pattern
		                            // ABCDEFG         7-segment LED pattern for reference (1 is on)
        case (data)                 // 6543210 
            4'b0000: decoded_bits = 7'b1111110; // 0       A-6
            4'b0001: decoded_bits = 7'b0110000; // 1   F-1     B-5
            4'b0010: decoded_bits = 7'b1101101; // 2       G-0
            4'b0011: decoded_bits = 7'b1111001; // 3   E-2     C-4
            4'b0100: decoded_bits = 7'b0110011; // 4       D-3      DP
            4'b0101: decoded_bits = 7'b1011011; // 5
            4'b0110: decoded_bits = 7'b1011111; // 6
            4'b0111: decoded_bits = 7'b1110000; // 7
            4'b1000: decoded_bits = 7'b1111111; // 8
            4'b1001: decoded_bits = 7'b1111011; // 9
            4'b1010: decoded_bits = 7'b1110111; // A (Not used in stopwatch)
            4'b1011: decoded_bits = 7'b1111111; // B (Not used in stopwatch)
            4'b1100: decoded_bits = 7'b1001110; // C (Not used in stopwatch)
            4'b1101: decoded_bits = 7'b1111110; // D (Not used in stopwatch)
            4'b1110: decoded_bits = 7'b1001111; // E (Not used in stopwatch)
            4'b1111: decoded_bits = 7'b1000111; // F (Not used in stopwatch)
// Students: fill in the remaining rows for this case statement,
// to account for the hexademcial digits A, B, C, D, E, and F

            default: decoded_bits = 7'b0000000; // All LEDs off
        endcase                     // ABCDEFG
    end                             // 6543210

    // Assign the decoded bits to the 7-segment display outputs (active-low on Basys3, i.e. 0 is ON)
    // Invert LED signals that were active-high for convenience
    assign DP = ~dp_in; // Passes through the decimal point signal (from top_level)
    assign CA = ~decoded_bits[6];
    assign CB = ~decoded_bits[5];
    assign CC = ~decoded_bits[4];
    assign CD = ~decoded_bits[3];
    assign CE = ~decoded_bits[2];
    assign CF = ~decoded_bits[1];
    assign CG = ~decoded_bits[0];

endmodule
