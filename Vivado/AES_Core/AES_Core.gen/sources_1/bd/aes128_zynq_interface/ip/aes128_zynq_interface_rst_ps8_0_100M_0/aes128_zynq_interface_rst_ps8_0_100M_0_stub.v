// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2023.1 (win64) Build 3865809 Sun May  7 15:05:29 MDT 2023
// Date        : Fri Oct  6 21:28:29 2023
// Host        : Yeshvanth-Workstation running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub
//               e:/SW/Vivado_Projetcs/AES_Core/AES_Core.gen/sources_1/bd/aes128_zynq_interface/ip/aes128_zynq_interface_rst_ps8_0_100M_0/aes128_zynq_interface_rst_ps8_0_100M_0_stub.v
// Design      : aes128_zynq_interface_rst_ps8_0_100M_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xczu1cg-sbva484-1-e
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* x_core_info = "proc_sys_reset,Vivado 2023.1" *)
module aes128_zynq_interface_rst_ps8_0_100M_0(slowest_sync_clk, ext_reset_in, aux_reset_in, 
  mb_debug_sys_rst, dcm_locked, mb_reset, bus_struct_reset, peripheral_reset, 
  interconnect_aresetn, peripheral_aresetn)
/* synthesis syn_black_box black_box_pad_pin="ext_reset_in,aux_reset_in,mb_debug_sys_rst,dcm_locked,mb_reset,bus_struct_reset[0:0],peripheral_reset[0:0],interconnect_aresetn[0:0],peripheral_aresetn[0:0]" */
/* synthesis syn_force_seq_prim="slowest_sync_clk" */;
  input slowest_sync_clk /* synthesis syn_isclock = 1 */;
  input ext_reset_in;
  input aux_reset_in;
  input mb_debug_sys_rst;
  input dcm_locked;
  output mb_reset;
  output [0:0]bus_struct_reset;
  output [0:0]peripheral_reset;
  output [0:0]interconnect_aresetn;
  output [0:0]peripheral_aresetn;
endmodule
