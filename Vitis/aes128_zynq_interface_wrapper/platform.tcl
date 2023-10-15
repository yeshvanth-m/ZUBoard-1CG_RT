# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct E:\CODE\Vitis_Workspace\aes128_zynq_interface_wrapper\platform.tcl
# 
# OR launch xsct and run below command.
# source E:\CODE\Vitis_Workspace\aes128_zynq_interface_wrapper\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {aes128_zynq_interface_wrapper}\
-hw {E:\SW\Vivado_Projetcs\AES_Core\aes128_zynq_interface_wrapper.xsa}\
-arch {64-bit} -fsbl-target {psu_cortexa53_0} -out {E:/CODE/Vitis_Workspace}

platform write
domain create -name {standalone_psu_cortexa53_0} -display-name {standalone_psu_cortexa53_0} -os {standalone} -proc {psu_cortexa53_0} -runtime {cpp} -arch {64-bit} -support-app {hello_world}
platform generate -domains 
platform active {aes128_zynq_interface_wrapper}
domain active {zynqmp_fsbl}
domain active {zynqmp_pmufw}
domain active {standalone_psu_cortexa53_0}
platform generate -quick
platform generate
platform create -name {aes128_zynq_interface_wrapper}\
-hw {E:\SW\Vivado_Projetcs\AES_Core\aes128_zynq_interface_wrapper.xsa}\
-arch {64-bit} -fsbl-target {psu_cortexa53_0} -out {E:/CODE/Vitis_Workspace}

platform write
domain create -name {standalone_psu_cortexa53_0} -display-name {standalone_psu_cortexa53_0} -os {standalone} -proc {psu_cortexa53_0} -runtime {cpp} -arch {64-bit} -support-app {hello_world}
platform generate -domains 
platform active {aes128_zynq_interface_wrapper}
domain active {zynqmp_fsbl}
domain active {zynqmp_pmufw}
domain active {standalone_psu_cortexa53_0}
platform generate -quick
platform generate
platform clean
platform generate
