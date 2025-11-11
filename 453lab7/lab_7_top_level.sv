`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/10/2025 01:51:19 PM
// Design Name: 
// Module Name: lab_7_top_level
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module lab_7_top_level(
    input  logic   clk,
    input  logic   reset,
    input  logic [15:0] switches_inputs,
    input  logic [1:0] bin_bcd_select,
    input          vauxp15, // Analog input (positive) - connect to JXAC4:N2 PMOD pin  (XADC4)
    input          vauxn15, // Analog input (negative) - connect to JXAC10:N1 PMOD pin (XADC4)
    input          pwm_comp, r2r_comp,
    output logic   CA, CB, CC, CD, CE, CF, CG, DP,
    output logic   AN1, AN2, AN3, AN4,
    output logic [15:0] led,
    output logic   pwm_out,
    output logic [7:0] R2R_out
);

    // XADC signals
    logic        ready;              // Data ready from XADC
    logic [15:0] data;              // Raw ADC data
    logic        enable;                // XADC enable
   // logic [4:0]  channel_out;           // Current XADC channel
    //logic        eoc_out;               // End of conversion
    logic [6:0]  daddr_in;              // XADC address
    logic        eos_out;               // End of sequence
    logic        busy_out;              // XADC busy signal
    
    logic comparator; // comp mux output
   
    logic raw_adc; // recorder output
    
    // Constants
    localparam CHANNEL_ADDR = 7'h1f;     // XA4/AD15 (for XADC4)

    xadc_wiz_0 XADC_INST (
        .di_in(16'h0000),        // Not used for reading
        .daddr_in(CHANNEL_ADDR), // Channel address
        .den_in(enable),         // Enable signal
        .dwe_in(1'b0),           // Not writing, so set to 0
        .drdy_out(ready),        // Data ready signal (when high, ADC data is valid)
        .do_out(data),           // ADC data output
        .dclk_in(clk),           // Use system clock
        .reset_in(reset),   // Active-high reset
        .vp_in(1'b0),            // Not used, leave disconnected
        .vn_in(1'b0),            // Not used, leave disconnected
        .vauxp15(vauxp15),       // Auxiliary analog input (positive)
        .vauxn15(vauxn15),       // Auxiliary analog input (negative)
        .channel_out(),          // Current channel being converted
        .eoc_out(enable),        // End of conversion
        .alarm_out(),            // Not used
        .eos_out(eos_out),       // End of sequence
        .busy_out(busy_out)      // XADC busy signal
    );
    
    sawtooth_generator SAWTOOTH (
        .clk(clk),
        .reset(reset),
        .enable(enable),
        .pwm_out(pwm_out),
        .R2R_out(R2R_out)
    );
    
    mux21 #(.WIDTH(1)) COMP_SELECT (
        .select(switches_inputs[14]),
        .d0(pwm_comp),
        .d1(r2r_comp),
        .y(comparator)
    );
    
    recorder #(.WIDTH(8)) RECORDER (
        .clk(clk),
        .reset(reset),
        .ready(comparator),
        .duty_cycle(R2R_out),
        .duty_cycle_out(raw_adc)
    );
    
     
    
    
    
    
endmodule
