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
    logic [15:0] data;              // Raw XADC data
    logic        enable;                // XADC enable
   // logic [4:0]  channel_out;           // Current XADC channel
    //logic        eoc_out;               // End of conversion
    logic [6:0]  daddr_in;              // XADC address
    logic        eos_out;               // End of sequence
    logic        busy_out;              // XADC busy signal
    
    logic comparator; // comp mux output
    
    logic ready_r_out;
   
    logic [7:0] raw_adc; // recorder output
    
    logic [15:0] scaled_xadc_data, scaled_adc_data; // processing output
    
    logic [15:0] signal_select_out;
    
    logic [15:0] bcd_out;
    
    logic [15:0] binbcd_sel_out;
    
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
    
    mux21 #(.WIDTH(1)) COMP_SEL (
        .select(switches_inputs[13]),
        .d0(pwm_comp),
        .d1(r2r_comp),
        .y(comparator)
    );
    
    recorder #(.DC_WIDTH(8)) RECORDER (
        .clk(clk),
        .reset(reset),
        .ready(comparator),
        .duty_cycle(R2R_out),
        .duty_cycle_out(raw_adc),
        .ready_r_out(ready_r_out)
    );
    
    xadc_processing XADC_PROC (
        .clk(clk),
        .reset(reset),
        .ready(ready),
        .data(data),
        .scaled_adc_data(scaled_xadc_data),
        .ready_pulse(ready_pulse)
    );
    
    adc_processing ADC_PROC (
        .clk(clk),
        .reset(reset),
        .ready(ready_r_out),
        .data(raw_adc),
        .scaled_adc_data(scaled_adc_data)
    );
    
    mux41 #(.WIDTH(16)) SIGNAL_SEL (
        .select(switches_inputs[15:14]),
        .d0(data),
        .d1(scaled_xadc_data),
        .d2({8'b0, raw_adc}),
        .d3(scaled_adc_data),
        .y(signal_select_out)
    );
    
    bin_to_bcd BIN2BCD (
        .clk(clk),
        .reset(reset),
        .bin_in(signal_select_out),
        .bcd_out(bcd_out)
    );
    
    mux21 #(.WIDTH(16)) BINBCD_SEL (
        .select(switches_inputs[12]),
        .d0(signal_select_out),
        .d1(bcd_out),
        .y(binbcd_sel_out)
    );
    
    seven_segment_display_subsystem SSD (
        .clk(clk),
        .reset(reset),
        .sec_dig1(binbcd_sel_out[3:0]),
        .sec_dig2(binbcd_sel_out[7:4]),
        .min_dig1(binbcd_sel_out[11:8]),
        .min_dig2(binbcd_sel_out[15:12]),
        .CA(CA), .CB(CB), .CC(CC), .CD(CD),
        .CE(CE), .CF(CF), .CG(CG), .DP(DP),
        .AN1(AN1), .AN2(AN2), .AN3(AN3), .AN4(AN4)
    );
     
    
    
endmodule
