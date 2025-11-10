module seven_segment_digit_selector (
    input logic        clk,
    input logic        reset,
    output logic [3:0] digit_select,
    output logic [3:0] an_outputs
);

    logic [3:0] d, q;
    logic [16:0] count;

    // 1 kHz clock process (100 MHz / 2^17 = 762.9 Hz)
    always_ff @(posedge clk) begin
        if (reset) begin
            count <= 17'b0;
        end else begin
            count <= count + 1;
        end
    end

    // DFFs process
    always_ff @(posedge clk) begin
        if (reset) begin
            // Reset state values for q
            q <= 4'b1111;
        end else if (count == 17'b0) begin
            // Propagate signals through the DFF
            if (q[0] && q[1]) begin
                q <= 4'b1000;
            end else begin
                q <= d;
            end
        end
    end
