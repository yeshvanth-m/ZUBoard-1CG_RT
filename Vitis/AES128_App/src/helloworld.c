/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
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
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"
#include "stdbool.h"

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

uint8_t aes128_cipherKey[16u] =
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

void set_plainText (void)
{
	aes128_qword1_o = ((aes128_plainText[0]) << 24u | (aes128_plainText[1] << 16u) | (aes128_plainText[2] << 8u) | aes128_plainText[3]);
	aes128_qword2_o = ((aes128_plainText[4]) << 24u | (aes128_plainText[5] << 16u) | (aes128_plainText[6] << 8u) | aes128_plainText[7]);
	aes128_qword3_o = ((aes128_plainText[8]) << 24u | (aes128_plainText[9] << 16u) | (aes128_plainText[10] << 8u) | aes128_plainText[11]);
	aes128_qword4_o = ((aes128_plainText[12]) << 24u | (aes128_plainText[13] << 16u) | (aes128_plainText[14] << 8u) | aes128_plainText[15]);

	load_data();
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

void set_cipherKey (void)
{
	stat_ctrl_op |= SET_LOAD_KEY_BIT;

	aes128_qword1_o = ((aes128_cipherKey[0]) << 24u | (aes128_cipherKey[1] << 16u) | (aes128_cipherKey[2] << 8u) | aes128_cipherKey[3]);
	aes128_qword2_o = ((aes128_cipherKey[4]) << 24u | (aes128_cipherKey[5] << 16u) | (aes128_cipherKey[6] << 8u) | aes128_cipherKey[7]);
	aes128_qword3_o = ((aes128_cipherKey[8]) << 24u | (aes128_cipherKey[9] << 16u) | (aes128_cipherKey[10] << 8u) | aes128_cipherKey[11]);
	aes128_qword4_o = ((aes128_cipherKey[12]) << 24u | (aes128_cipherKey[13] << 16u) | (aes128_cipherKey[14] << 8u) | aes128_cipherKey[15]);

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
    init_platform();

    setup ();

    stat_ctrl_op = 0u;
    stat_ctrl_op |= SET_ENCRYPT_BIT;

    set_cipherKey ();
	while (!get_keyReadyStatus());

	set_plainText ();
	while (!get_cipherReadyStatus());

	get_cipherText ();

	aes128_qword1_o = aes128_qword1_i;
	aes128_qword2_o = aes128_qword2_i;
	aes128_qword3_o = aes128_qword3_i;
	aes128_qword4_o = aes128_qword4_i;

	stat_ctrl_op &= CLR_ENCRYPT_BIT;

	load_data ();

	while (!get_cipherReadyStatus());
	get_cipherText ();

    print("Hello World\n\r");

    cleanup_platform();
    return 0;
}
