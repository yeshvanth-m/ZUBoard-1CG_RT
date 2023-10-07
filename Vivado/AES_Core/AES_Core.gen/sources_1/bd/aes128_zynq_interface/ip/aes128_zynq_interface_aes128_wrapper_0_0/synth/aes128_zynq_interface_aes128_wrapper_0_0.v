// (c) Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// (c) Copyright 2022-2023 Advanced Micro Devices, Inc. All rights reserved.
// 
// This file contains confidential and proprietary information
// of AMD and is protected under U.S. and international copyright
// and other intellectual property laws.
// 
// DISCLAIMER
// This disclaimer is not a license and does not grant any
// rights to the materials distributed herewith. Except as
// otherwise provided in a valid license issued to you by
// AMD, and to the maximum extent permitted by applicable
// law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
// WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
// AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
// BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
// INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
// (2) AMD shall not be liable (whether in contract or tort,
// including negligence, or under any other theory of
// liability) for any loss or damage of any kind or nature
// related to, arising under or in connection with these
// materials, including for any direct, or any indirect,
// special, incidental, or consequential loss or damage
// (including loss of data, profits, goodwill, or any type of
// loss or damage suffered as a result of any action brought
// by a third party) even if such damage or loss was
// reasonably foreseeable or AMD had been advised of the
// possibility of the same.
// 
// CRITICAL APPLICATIONS
// AMD products are not designed or intended to be fail-
// safe, or for use in any application requiring fail-safe
// performance, such as life-support or safety devices or
// systems, Class III medical devices, nuclear facilities,
// applications related to the deployment of airbags, or any
// other applications that could lead to death, personal
// injury, or severe property or environmental damage
// (individually and collectively, "Critical
// Applications"). Customer assumes the sole risk and
// liability of any use of AMD products in Critical
// Applications, subject only to applicable laws and
// regulations governing limitations on product liability.
// 
// THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
// PART OF THIS FILE AT ALL TIMES.
// 
// DO NOT MODIFY THIS FILE.


// IP VLNV: xilinx.com:module_ref:aes128_wrapper:1.0
// IP Revision: 1

(* X_CORE_INFO = "aes128_wrapper,Vivado 2023.1" *)
(* CHECK_LICENSE_TYPE = "aes128_zynq_interface_aes128_wrapper_0_0,aes128_wrapper,{}" *)
(* CORE_GENERATION_INFO = "aes128_zynq_interface_aes128_wrapper_0_0,aes128_wrapper,{x_ipProduct=Vivado 2023.1,x_ipVendor=xilinx.com,x_ipLibrary=module_ref,x_ipName=aes128_wrapper,x_ipVersion=1.0,x_ipCoreRevision=1,x_ipLanguage=VERILOG,x_ipSimLanguage=MIXED}" *)
(* IP_DEFINITION_SOURCE = "module_ref" *)
(* DowngradeIPIdentifiedWarnings = "yes" *)
module aes128_zynq_interface_aes128_wrapper_0_0 (
  aes128_clk_i,
  aes128_ctrl_i,
  aes128_data_word1_i,
  aes128_data_word2_i,
  aes128_data_word3_i,
  aes128_data_word4_i,
  aes128_data_word1_o,
  aes128_data_word2_o,
  aes128_data_word3_o,
  aes128_data_word4_o,
  aes128_stat_o
);

input wire aes128_clk_i;
input wire [2 : 0] aes128_ctrl_i;
input wire [31 : 0] aes128_data_word1_i;
input wire [31 : 0] aes128_data_word2_i;
input wire [31 : 0] aes128_data_word3_i;
input wire [31 : 0] aes128_data_word4_i;
output wire [31 : 0] aes128_data_word1_o;
output wire [31 : 0] aes128_data_word2_o;
output wire [31 : 0] aes128_data_word3_o;
output wire [31 : 0] aes128_data_word4_o;
output wire [1 : 0] aes128_stat_o;

  aes128_wrapper inst (
    .aes128_clk_i(aes128_clk_i),
    .aes128_ctrl_i(aes128_ctrl_i),
    .aes128_data_word1_i(aes128_data_word1_i),
    .aes128_data_word2_i(aes128_data_word2_i),
    .aes128_data_word3_i(aes128_data_word3_i),
    .aes128_data_word4_i(aes128_data_word4_i),
    .aes128_data_word1_o(aes128_data_word1_o),
    .aes128_data_word2_o(aes128_data_word2_o),
    .aes128_data_word3_o(aes128_data_word3_o),
    .aes128_data_word4_o(aes128_data_word4_o),
    .aes128_stat_o(aes128_stat_o)
  );
endmodule
