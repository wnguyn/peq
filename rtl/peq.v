`default_nettype none

// Work in progress: signed 16-bit PCM bypass at one sample per clock.
// TODO: Replace the bypass with a fixed-point parametric EQ datapath.
module peq (
    input wire clk,
    input wire rst,
    input wire sample_valid,
    input wire signed [15:0] sample_in,
    output reg sample_out_valid,
    output reg signed [15:0] sample_out
);
    // Synchronous, active-high reset. Output updates on the next rising edge.
    // There is no backpressure: every asserted sample_valid is accepted.
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
