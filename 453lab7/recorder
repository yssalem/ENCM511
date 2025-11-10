module recorder
    #(
        parameter int DC_WIDTH = 8
    )
{
    input logic clk,
    input logic reset,
    input logic ready,
    input logic [DC_WIDTH - 1 : 0] duty_cycle,
    output logic [DC_WIDTH - 1 : 0] duty_cycle_out
};
    logic ready_r;
    register pulse0 #(WIDTH = 1)(
        .clk(clk),
        .en(1),
        .reset(reset),
        .din(~ready_r & ready),
        .q_out(ready_r)
    );
    register pulse1 #(WIDTH = DC_WIDTH)(
        .clk(clk),
        .en(ready_r),
        .reset(reset),
        .din(duty_cycle),
        .q_out(duty_cycle_out)
    );

endmodule
