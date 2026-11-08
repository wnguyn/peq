
module i2s_pcm (
    input wire bclk,
    input wire lrclk,
    input wire sdata,
    input wire rst, // random reset wire lol

    output reg signed [15:0] x0, // x[n]
    output reg signed [15:0] x1, // x[n-1]
    output reg signed [15:0] x2, // x[n-2]

    output reg signed [15:0] y1,
    output reg signed [15:0] y2,
    output reg sample_valid
);

    reg lrclk_d1, lrclk_d2;
    reg [4:0] bit_count;
    reg [15:0] shift_reg;


    // get x[n], x[n-1], x[n-2]
    always @(posedge bclk) begin
        // debug thing?
        if (rst) begin
            lrclk_d1 <= 0;
            lrclk_d2 <= 0;
            bit_count <= 16;
            shift_reg <= 0;
            x0 <= 0;
            x1 <= 0;
            x2 <= 0;
            sample_valid <= 0;
        end else begin
            lrclk_d1 <= lrclk;
            lrclk_d2 <= lrclk_d1;
            sample_valid <= 0;

            if (lrclk_d1 != lrclk_d2) begin
                bit_count <= 1;
                shift_reg <= {15'b0, sdata};
            end

            else if (bit_count < 16) begin
                shift_reg <= {shift_reg[14:0], sdata};
                bit_count <= bit_count + 1'b1;

                if (bit_count == 15 && !lrclk_d1) begin
                    x2 <= x1;
                    x1 <= x0;
                    x0 <= {shift_reg[14:0], sdata};
                    sample_valid <= 1;
                end
            end
        end
    end
    // if sample_valid we take the PCM samples through 8 times 
    always @(posedge bclk) begin
        if (sample_valid) begin
            sample_valid <= 0; //
                        
        end
        


    end

endmodule



module i2s_out {
    
}
