module mux31_onehot
    #(
        parameter WIDTH = 1
    )(
        input logic [2:0] select,
        input logic [WIDTH - 1 : 0] d0,
        input logic [WIDTH - 1 : 0] d1,
        input logic [WIDTH - 1 : 0] d2,
        output logic [WIDTH - 1 : 0] y
    );
    always_comb begin
        case (select) 
        3'b001 : y = d0;
        3'b010 : y = d1;
        3'b100 : y = d2;
        default : y = {WIDTH{1'b1}};
        endcase         
    end

endmodule
