# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: E:\CODE\Vitis_Workspace\AES128_Benchmark_RT_system\_ide\scripts\debugger_aes128_benchmark_rt-default.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source E:\CODE\Vitis_Workspace\AES128_Benchmark_RT_system\_ide\scripts\debugger_aes128_benchmark_rt-default.tcl
# 
connect -url tcp:127.0.0.1:3121
source E:/SW/Xilinx/Vitis/2023.1/scripts/vitis/util/zynqmp_utils.tcl
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -nocase -filter {name =~"RPU*"}
enable_split_mode
targets -set -filter {jtag_cable_name =~ "Avnet ZUBoard1CG 1234-oj1A" && level==0 && jtag_device_ctx=="jsn-ZUBoard1CG-1234-oj1A-04688093-0"}
fpga -file E:/CODE/Vitis_Workspace/AES128_Benchmark_RT/_ide/bitstream/aes128_zynq_interface_wrapper.bit
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw E:/CODE/Vitis_Workspace/aes128_zynq_interface_wrapper_1/export/aes128_zynq_interface_wrapper_1/hw/aes128_zynq_interface_wrapper.xsa -mem-ranges [list {0x80000000 0xbfffffff} {0x400000000 0x5ffffffff} {0x1000000000 0x7fffffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
set mode [expr [mrd -value 0xFF5E0200] & 0xf]
targets -set -nocase -filter {name =~ "*R5*#0"}
rst -processor
dow E:/CODE/Vitis_Workspace/aes128_zynq_interface_wrapper_1/export/aes128_zynq_interface_wrapper_1/sw/aes128_zynq_interface_wrapper_1/boot/fsbl.elf
set bp_25_15_fsbl_bp [bpadd -addr &XFsbl_Exit]
con -block -timeout 60
bpremove $bp_25_15_fsbl_bp
targets -set -nocase -filter {name =~ "*R5*#0"}
rst -processor
dow E:/CODE/Vitis_Workspace/AES128_Benchmark_RT/Debug/AES128_Benchmark_RT.elf
configparams force-mem-access 0
targets -set -nocase -filter {name =~ "*R5*#0"}
con
