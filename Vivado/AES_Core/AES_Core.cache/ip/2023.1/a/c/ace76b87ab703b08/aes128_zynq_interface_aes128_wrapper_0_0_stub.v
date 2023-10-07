// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2023.1 (win64) Build 3865809 Sun May  7 15:05:29 MDT 2023
// Date        : Sat Oct  7 11:26:47 2023
// Host        : Yeshvanth-Workstation running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
//               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ aes128_zynq_interface_aes128_wrapper_0_0_stub.v
// Design      : aes128_zynq_interface_aes128_wrapper_0_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xczu1cg-sbva484-1-e
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* X_CORE_INFO = "aes128_wrapper,Vivado 2023.1" *)
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix(aes128_clk_i, aes128_ctrl_i, 
  aes128_data_word1_i, aes128_data_word2_i, aes128_data_word3_i, aes128_data_word4_i, 
  aes128_data_word1_o, aes128_data_word2_o, aes128_data_word3_o, aes128_data_word4_o, 
  aes128_stat_o)
/* synthesis syn_black_box black_box_pad_pin="aes128_ctrl_i[2:0],aes128_data_word1_i[31:0],aes128_data_word2_i[31:0],aes128_data_word3_i[31:0],aes128_data_word4_i[31:0],aes128_data_word1_o[31:0],aes128_data_word2_o[31:0],aes128_data_word3_o[31:0],aes128_data_word4_o[31:0],aes128_stat_o[1:0]" */
/* synthesis syn_force_seq_prim="aes128_clk_i" */;
  input aes128_clk_i /* synthesis syn_isclock = 1 */;
  input [2:0]aes128_ctrl_i;
  input [31:0]aes128_data_word1_i;
  input [31:0]aes128_data_word2_i;
  input [31:0]aes128_data_word3_i;
  input [31:0]aes128_data_word4_i;
  output [31:0]aes128_data_word1_o;
  output [31:0]aes128_data_word2_o;
  output [31:0]aes128_data_word3_o;
  output [31:0]aes128_data_word4_o;
  output [1:0]aes128_stat_o;
endmodule
