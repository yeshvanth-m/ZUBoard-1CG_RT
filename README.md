# ZUBoard-1CG_RT

This repository contains files used in the roadtest of the ZUBoard 1CG Development Kit from Avnet
- Link to the detailed review: https://community.element14.com/products/roadtest/rv/roadtest_reviews/1744/bench-marking_zynq_ultrascale_mpsoc_with_custom_built_aes_core
- Link to the detailed AES implementation: https://github.com/yeshvanth-m/AES-128

Images from the review:
## The board
![ZUB1CG](/Docs/images/Board.jpg)

## Out of box demo
![ZUB1CG](/Docs/images/Out-of-box.png)

## A53 Core Software (1.2GHz) Profiling vs PL Hardware (100MHz)
![ZUB1CG](/Docs/images/A53_Core.png)

## R5 Core Software (500MHz) Profiling vs PL Hardware (100MHz)
![ZUB1CG](/Docs/images/R5_Core.png)

 ```
Folder organization:
/Vivado: Contains all files related to Vivado 2023.1 project, might have to add sources from /HW_Design_Files directory
/Vitis: Contains all files related to Vitis source and project files
/SW_Implementation_Files: Contains all C implementation files of the AES128 SW
/HW_Design_Files: Contains all Verilog implementation files of AES128 Core HW
```