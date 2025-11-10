module adc_processing #(
    parameter int SCALING_FACTOR = 79993,
    parameter int SHIFT_FACTOR   = 19
) (
    input  logic        clk,
    input  logic        reset,
    input  logic        ready,
    input  logic [15:0] data,
    output logic [15:0] scaled_adc_data,
    output logic        ready_pulse
);
    logic [15:0] ave_data;
    logic        ready_r;
    logic [15:0] scaled_adc_data_pipe;

    // Calculate the bit width for the scaling factor
    localparam int SCALING_FACTOR_WIDTH = $clog2(SCALING_FACTOR) + 1;
    
    // Calculate the intermediate width dynamically based on ave_data (16-bits) and scaling factor
    localparam int INTERMEDIATE_WIDTH = 16 + SCALING_FACTOR_WIDTH;

    // Define the intermediate register with the required bit width, to avoid 32-bit overflow on intermediate calculation 
    logic [INTERMEDIATE_WIDTH-1:0] scaled_adc_data_temp; 

    // Pulser 
    always_ff @(posedge clk)
        if (reset)
            ready_r <= 0;
        else
            ready_r <= ready;
       
    assign ready_pulse = ~ready_r & ready; // generate 1-clk pulse when ready goes high

    averager #(
        .power(8), // 2**(power) samples, default is 2**8 = 256 samples (4^4 = 256 samples, adds 4 bits of ADC resolution)
        .N(16)     // # of bits to take the average of
    ) AVERAGER (
        .reset(reset),
        .clk(clk),
        .EN(ready_pulse),
        .Din(data),
        .Q(ave_data)
    );
    
    always_ff @(posedge clk) begin
        if (reset) begin
            scaled_adc_data <= 0;
            scaled_adc_data_temp <= 0;
            scaled_adc_data_pipe <= 0;
        end
        else if (ready_pulse) begin
            scaled_adc_data_temp <= ave_data * SCALING_FACTOR;            // Use a wider temp register to avoid 32-bit overflow error
            scaled_adc_data_pipe <= scaled_adc_data_temp >> SHIFT_FACTOR; // Shift right by 19 after the multiplication
            scaled_adc_data      <= scaled_adc_data_pipe; // Additional register faciliates pipelining, if needed, for
        end                                               // higher clock frequencies. Costs are an additional register
    end                                                   // and an additional 1 clk cycle latency.    
endmodule
