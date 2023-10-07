-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2023.1 (win64) Build 3865809 Sun May  7 15:05:29 MDT 2023
-- Date        : Fri Oct  6 22:39:57 2023
-- Host        : Yeshvanth-Workstation running 64-bit major release  (build 9200)
-- Command     : write_vhdl -force -mode synth_stub -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
--               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ aes128_zynq_interface_aes128_wrapper_0_0_stub.vhdl
-- Design      : aes128_zynq_interface_aes128_wrapper_0_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xczu1cg-sbva484-1-e
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
  Port ( 
    aes128_clk_i : in STD_LOGIC;
    aes128_ctrl_i : in STD_LOGIC_VECTOR ( 2 downto 0 );
    aes128_data_word1_i : in STD_LOGIC_VECTOR ( 31 downto 0 );
    aes128_data_word2_i : in STD_LOGIC_VECTOR ( 31 downto 0 );
    aes128_data_word3_i : in STD_LOGIC_VECTOR ( 31 downto 0 );
    aes128_data_word4_i : in STD_LOGIC_VECTOR ( 31 downto 0 );
    aes128_data_word1_o : out STD_LOGIC_VECTOR ( 31 downto 0 );
    aes128_data_word2_o : out STD_LOGIC_VECTOR ( 31 downto 0 );
    aes128_data_word3_o : out STD_LOGIC_VECTOR ( 31 downto 0 );
    aes128_data_word4_o : out STD_LOGIC_VECTOR ( 31 downto 0 );
    aes128_stat_o : out STD_LOGIC_VECTOR ( 1 downto 0 )
  );

end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix;

architecture stub of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "aes128_clk_i,aes128_ctrl_i[2:0],aes128_data_word1_i[31:0],aes128_data_word2_i[31:0],aes128_data_word3_i[31:0],aes128_data_word4_i[31:0],aes128_data_word1_o[31:0],aes128_data_word2_o[31:0],aes128_data_word3_o[31:0],aes128_data_word4_o[31:0],aes128_stat_o[1:0]";
attribute X_CORE_INFO : string;
attribute X_CORE_INFO of stub : architecture is "aes128_wrapper,Vivado 2023.1";
begin
end;
