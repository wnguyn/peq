`default_nettype none



// one band boilerplate for this???? yeah
module peq (
    input wire clk,
    input wire rst,
    input wire sample_valid,
    input wire signed [15:0] sample_in,
    output reg sample_out_valid,
    output reg signed [15:0] sample_out
);


    always @(posedge clk) begin
        if (rst) begin
            sample_out_valid <= 1'b0;
            sample_out <= 16'sd0;
        end else begin
            sample_out_valid <= sample_valid;
            if (sample_valid)
                sample_out <= sample_in;
        end
    end
endmodule

`default_nettype wire
