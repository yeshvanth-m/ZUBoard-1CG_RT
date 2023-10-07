`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/01/2023 11:29:29 PM
// Design Name: 
// Module Name: aes128_wrapper
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


module aes128_wrapper(aes128_clk_i, aes128_ctrl_i,
                      aes128_data_word1_i, aes128_data_word2_i, aes128_data_word3_i, aes128_data_word4_i,
                      aes128_data_word1_o, aes128_data_word2_o, aes128_data_word3_o, aes128_data_word4_o,
                      aes128_stat_o);
   
input aes128_clk_i;
input [2:0] aes128_ctrl_i;
input [31:0] aes128_data_word1_i, aes128_data_word2_i, aes128_data_word3_i, aes128_data_word4_i;
output [31:0] aes128_data_word1_o, aes128_data_word2_o, aes128_data_word3_o, aes128_data_word4_o;
output [1:0] aes128_stat_o; 

wire [127:0] aes128_data_i, aes128_cipher_text_o;
wire aes128_reset_key_i, aes128_load_plain_text_i, aes128_enc_or_dec_i, aes128_key_ready_o, aes128_cipher_ready_o;

assign aes128_reset_key_i = aes128_ctrl_i[0];
assign aes128_load_plain_text_i = aes128_ctrl_i[1];
assign aes128_enc_or_dec_i = aes128_ctrl_i[2];

assign aes128_data_i = {aes128_data_word1_i, aes128_data_word2_i, aes128_data_word3_i, aes128_data_word4_i};
assign {aes128_data_word1_o, aes128_data_word2_o, aes128_data_word3_o, aes128_data_word4_o} = aes128_cipher_text_o;

assign aes128_stat_o[0] = aes128_key_ready_o;
assign aes128_stat_o[1] = aes128_cipher_ready_o;

aes128
    aes128_core (.clk_i(aes128_clk_i), .reset_key_i(aes128_reset_key_i), .load_data_i(aes128_load_plain_text_i), 
                 .enc_or_dec_i(aes128_enc_or_dec_i), .plain_text_i(aes128_data_i), .cipher_key_i(aes128_data_i), 
                 .cipher_text_o(aes128_cipher_text_o), .key_ready_o(aes128_key_ready_o), .cipher_ready_o(aes128_cipher_ready_o));
    
endmodule
