/********************************************************************************
* @file     aes128_app.h                                                        *
* @brief    AES128 application                                                  *
* @author   Yeshvanth M  <yeshvanthmuniraj@gmail.com>                           *
* @date     14-Oct-2023                                                         *
*********************************************************************************
*                                                                               *
* This program is free software: you can redistribute it and/or modify it       *
* under the terms of the GNU General Public License as published by the Free    *
* Software Foundation, either version 3 of the License, or (at your option)     *
* any later version.                                                            *
*                                                                               *
* This program is distributed in the hope that it will be useful, but           *
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY    *
* or FITNESS FOR A PARTICULAR PURPOSE.                                          *
* See the GNU General Public License for more details.                          *
*                                                                               *
* You should have received a copy of the GNU General Public License             *
* along with this program. If not, see <https://www.gnu.org/licenses/>.         *
*                                                                               *
********************************************************************************/
/*
 * aes128_app.c: simple AES128 application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "stdbool.h"
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"
#include "xtime_l.h"
#include "xil_types.h"

#define GPIO_CH_1			1
#define GPIO_CH_2			2

#define STAT_CTRL_OP_DIR_CH1	0x7
#define STAT_CTRL_IP_DIR_CH2	0x0

#define SET_ENCRYPT_BIT			0x4
#define CLR_ENCRYPT_BIT			0x3
#define SET_LOAD_DATA_BIT		0x2
#define CLR_LOAD_DATA_BIT		0x5
#define SET_LOAD_KEY_BIT		0x1
#define CLR_LOAD_KEY_BIT		0x6

#define KEY_READY_BIT			0x1
#define CIPHER_READY_BIT		0x2

#define STAT_CTRL_BITM			0x7

#define QWORD1_IP_DIR_CH1	   	0xFF
#define QWORD1_IP_DIR_CH2		QWORD1_IP_DIR_CH1
#define QWORD2_IP_DIR_CH1		QWORD1_IP_DIR_CH1
#define QWORD2_IP_DIR_CH2		QWORD1_IP_DIR_CH1

#define QWORD1_OP_DIR_CH1	   	0x00
#define QWORD1_OP_DIR_CH2		QWORD1_OP_DIR_CH1
#define QWORD2_OP_DIR_CH1		QWORD1_OP_DIR_CH1
#define QWORD2_OP_DIR_CH2		QWORD1_OP_DIR_CH1

XGpio stat_ctrl, qword1_i, qword2_i, qword1_o, qword2_o;

u8 stat_ctrl_ip, stat_ctrl_op;
u32 aes128_qword1_i, aes128_qword2_i, aes128_qword3_i, aes128_qword4_i,
    aes128_qword1_o, aes128_qword2_o, aes128_qword3_o, aes128_qword4_o;

u8 aes128_plainText[16u] =
{
    0x4C, 0x6D, 0x73, 0x64,
    0x6F, 0x20, 0x75, 0x6F,
    0x72, 0x69, 0x6D, 0x6C,
    0x65, 0x70, 0x20, 0x6F
};

uint8_t aes128_cipherKey_hw[16u] =
{
    0x2B, 0x28, 0xAB, 0x09,
    0x7E, 0xAE, 0xF7, 0xCF,
    0x15, 0xD2, 0x15, 0x4F,
    0x16, 0xA6, 0x88, 0x3C
};

uint8_t aes128_cipherText[16u];

void setup(void)
{
	XGpio_Initialize (&stat_ctrl, XPAR_AXI_GPIO_CTRL_STAT_DEVICE_ID);
	XGpio_Initialize (&qword1_i, XPAR_AXI_GPIO_QWORD1_I_DEVICE_ID);
	XGpio_Initialize (&qword2_i, XPAR_AXI_GPIO_QWORD2_I_DEVICE_ID);
	XGpio_Initialize (&qword1_o, XPAR_AXI_GPIO_QWORD1_O_DEVICE_ID);
	XGpio_Initialize (&qword2_o, XPAR_AXI_GPIO_QWORD2_O_DEVICE_ID);

	XGpio_SetDataDirection (&stat_ctrl, GPIO_CH_1, STAT_CTRL_OP_DIR_CH1);
	XGpio_SetDataDirection (&stat_ctrl, GPIO_CH_2, STAT_CTRL_IP_DIR_CH2);

	XGpio_SetDataDirection (&qword1_i, GPIO_CH_1, QWORD1_IP_DIR_CH1);
	XGpio_SetDataDirection (&qword1_i, GPIO_CH_2, QWORD1_IP_DIR_CH2);
	XGpio_SetDataDirection (&qword2_i, GPIO_CH_1, QWORD2_IP_DIR_CH1);
	XGpio_SetDataDirection (&qword2_i, GPIO_CH_2, QWORD2_IP_DIR_CH2);

	XGpio_SetDataDirection (&qword1_o, GPIO_CH_1, QWORD1_OP_DIR_CH1);
	XGpio_SetDataDirection (&qword1_o, GPIO_CH_2, QWORD1_OP_DIR_CH2);
	XGpio_SetDataDirection (&qword2_o, GPIO_CH_1, QWORD2_OP_DIR_CH1);
	XGpio_SetDataDirection (&qword2_o, GPIO_CH_2, QWORD2_OP_DIR_CH2);
}

void load_data (void)
{
	stat_ctrl_op |= SET_LOAD_DATA_BIT;

	XGpio_DiscreteWrite (&stat_ctrl, GPIO_CH_1, stat_ctrl_op);

	XGpio_DiscreteWrite (&qword1_o, GPIO_CH_1, aes128_qword1_o);
	XGpio_DiscreteWrite (&qword1_o, GPIO_CH_2, aes128_qword2_o);
	XGpio_DiscreteWrite (&qword2_o, GPIO_CH_1, aes128_qword3_o);
	XGpio_DiscreteWrite (&qword2_o, GPIO_CH_2, aes128_qword4_o);

	stat_ctrl_op &= CLR_LOAD_DATA_BIT;

	XGpio_DiscreteWrite (&stat_ctrl, GPIO_CH_1, stat_ctrl_op);
}

void set_plainText (void)
{
	aes128_qword1_o = ((aes128_plainText[0]) << 24u | (aes128_plainText[1] << 16u) | (aes128_plainText[2] << 8u) | aes128_plainText[3]);
	aes128_qword2_o = ((aes128_plainText[4]) << 24u | (aes128_plainText[5] << 16u) | (aes128_plainText[6] << 8u) | aes128_plainText[7]);
	aes128_qword3_o = ((aes128_plainText[8]) << 24u | (aes128_plainText[9] << 16u) | (aes128_plainText[10] << 8u) | aes128_plainText[11]);
	aes128_qword4_o = ((aes128_plainText[12]) << 24u | (aes128_plainText[13] << 16u) | (aes128_plainText[14] << 8u) | aes128_plainText[15]);

	load_data();
}

void set_cipherKey (void)
{
	stat_ctrl_op |= SET_LOAD_KEY_BIT;

	aes128_qword1_o = ((aes128_cipherKey_hw[0]) << 24u | (aes128_cipherKey_hw[1] << 16u) | (aes128_cipherKey_hw[2] << 8u) | aes128_cipherKey_hw[3]);
	aes128_qword2_o = ((aes128_cipherKey_hw[4]) << 24u | (aes128_cipherKey_hw[5] << 16u) | (aes128_cipherKey_hw[6] << 8u) | aes128_cipherKey_hw[7]);
	aes128_qword3_o = ((aes128_cipherKey_hw[8]) << 24u | (aes128_cipherKey_hw[9] << 16u) | (aes128_cipherKey_hw[10] << 8u) | aes128_cipherKey_hw[11]);
	aes128_qword4_o = ((aes128_cipherKey_hw[12]) << 24u | (aes128_cipherKey_hw[13] << 16u) | (aes128_cipherKey_hw[14] << 8u) | aes128_cipherKey_hw[15]);

	XGpio_DiscreteWrite (&stat_ctrl, GPIO_CH_1, stat_ctrl_op);

	XGpio_DiscreteWrite (&qword1_o, GPIO_CH_1, aes128_qword1_o);
	XGpio_DiscreteWrite (&qword1_o, GPIO_CH_2, aes128_qword2_o);
	XGpio_DiscreteWrite (&qword2_o, GPIO_CH_1, aes128_qword3_o);
	XGpio_DiscreteWrite (&qword2_o, GPIO_CH_2, aes128_qword4_o);

	stat_ctrl_op &= CLR_LOAD_KEY_BIT;

	XGpio_DiscreteWrite (&stat_ctrl, GPIO_CH_1, stat_ctrl_op);
}

bool get_keyReadyStatus (void)
{
	return  (XGpio_DiscreteRead (&stat_ctrl, GPIO_CH_2) & (KEY_READY_BIT));
}

bool get_cipherReadyStatus (void)
{
	return  (XGpio_DiscreteRead (&stat_ctrl, GPIO_CH_2) & (CIPHER_READY_BIT));
}

void get_cipherText (void)
{
	aes128_qword1_i = XGpio_DiscreteRead (&qword1_i, GPIO_CH_1);
	aes128_qword2_i = XGpio_DiscreteRead (&qword1_i, GPIO_CH_2);
	aes128_qword3_i = XGpio_DiscreteRead (&qword2_i, GPIO_CH_1);
	aes128_qword4_i = XGpio_DiscreteRead (&qword2_i, GPIO_CH_2);
}

int main()
{
	XTime tStart, tEnd;

    init_platform();

    aes128_gen_keys();

    XTime_GetTime(&tStart);
    aes128_encrypt();
	XTime_GetTime(&tEnd);

	printf("Encryption in SW took %lu clock cycles, time taken: %lu ns.\n", 2*(tEnd - tStart), (2*(tEnd - tStart)*833)/1000);

	XTime_GetTime(&tStart);
	aes128_decrypt();
	XTime_GetTime(&tEnd);

	printf("Decryption in SW took %lu clock cycles, time taken: %lu ns.\n", 2*(tEnd - tStart), (2*(tEnd - tStart)*833)/1000);

    setup ();

    stat_ctrl_op = 0u;
    stat_ctrl_op |= SET_ENCRYPT_BIT;

    set_cipherKey ();
	while (!get_keyReadyStatus());

	set_plainText ();

	XTime_GetTime(&tStart);
	while (!get_cipherReadyStatus());
	XTime_GetTime(&tEnd);

	printf("Encryption in HW took %lu clock cycles, time taken: %lu ns.\n", 2*(tEnd - tStart), (2*(tEnd - tStart)*833)/1000);

	get_cipherText ();

	aes128_qword1_o = aes128_qword1_i;
	aes128_qword2_o = aes128_qword2_i;
	aes128_qword3_o = aes128_qword3_i;
	aes128_qword4_o = aes128_qword4_i;

	stat_ctrl_op &= CLR_ENCRYPT_BIT;

	load_data ();

	XTime_GetTime(&tStart);

	while (!get_cipherReadyStatus());

	XTime_GetTime(&tEnd);

	printf("Decryption in HW took %lu clock cycles, time taken: %lu ns.\n", 2*(tEnd - tStart), (2*(tEnd - tStart)*833)/1000);

	get_cipherText ();

    print("Done\n\r");

    cleanup_platform();
    return 0;
}
