module averager_simple
    #(parameter int power = 8) // must be at least 8: 2**power samples, default is 2**8 = 256 samples 
    (
        input logic clk,
        reset,
        EN,
        input logic [7:0] Din,   // input to averager 
        output logic [15:0] Q     // N-bit moving average
    );
    
    // Internal signals
    logic [7:0] REG_ARRAY [2**power:1];
    logic [power+7:0] sum;
    logic [7:0] Q_temp, lower8bits;
    
    // Output assignment
    assign Q_temp     = sum[power+7:power];
    assign lower8bits = sum[power-1:power-8];    
    assign Q = {Q_temp,lower8bits};
   
    always_ff @(posedge clk) begin
        if (reset) begin
            sum <= 0;
            for (int j = 1; j <= 2**power; j++) begin
                REG_ARRAY[j] <= 0;
            end
        end
        else if (EN) begin
            sum <= sum + Din - REG_ARRAY[2**power];
            for (int j = 2**power; j > 1; j--) begin
                REG_ARRAY[j] <= REG_ARRAY[j-1];
            end
            REG_ARRAY[1] <= Din;
        end
    end
endmodule
