/******************************************************************************
* Copyright (c) 2021 - 2022 Xilinx, Inc.  All rights reserved.
* Copyright (c) 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/

/*****************************************************************************/
/**
*
* @file xsecure_elliptic_ipihandler.c
*
* This file contains the xilsecure elliptic IPI handlers implementation.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who  Date        Changes
* ----- ---- -------- -------------------------------------------------------
* 1.0  kal   03/23/2021 Initial release
* 4.6  gm    07/16/2021 Added support for 64-bit address
*      rb    08/11/2021 Fix compilation warnings
* 4.7  kpt   03/18/2022 Replaced XPlmi_Dmaxfr with XPlmi_MemCpy64
* 5.0  kpt   07/24/2022 Moved XSecure_EllipticExecuteKat in to xsecure_kat_plat_ipihandler.c
*
* </pre>
*
* @note
* @endcond
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "xplmi_config.h"

#ifndef PLM_ECDSA_EXCLUDE
#include "xplmi_dma.h"
#include "xsecure_defs.h"
#include "xsecure_elliptic.h"
#include "xsecure_elliptic_ipihandler.h"
#include "xstatus.h"
#include "xplmi.h"
#include "xplmi_tamper.h"
#include "xsecure_error.h"
#include "xsecure_kat.h"
#include "xsecure_init.h"

/************************** Constant Definitions *****************************/

/************************** Function Prototypes *****************************/
static int XSecure_EllipticGenKey(u32 CurveType, u32 SrcAddrLow,
	u32 SrcAddrHigh, u32 DstAddrLow, u32 DstAddrHigh);
static int XSecure_EllipticGenSign(u32 SrcAddrLow, u32 SrcAddrHigh,
	u32 DstAddrLow, u32 DstAddrHigh);
static int XSecure_EllipticValidatePubKey(u32 CurveType,
	u32 SrcAddrLow, u32 SrcAddrHigh);
static int XSecure_EllipticVerifySignature(u32 SrcAddrLow, u32 SrcAddrHigh);

/*************************** Function Definitions *****************************/

/*****************************************************************************/
/**
 * @brief       This function calls respective IPI handler based on the API_ID
 *
 * @param 	Cmd is pointer to the command structure
 *
 * @return
 *	-	XST_SUCCESS - If the handler execution is successful
 *	-	ErrorCode - If there is a failure
 *
 ******************************************************************************/
int XSecure_EllipticIpiHandler(XPlmi_Cmd *Cmd)
{
	volatile int Status = XST_FAILURE;
	u32 *Pload = Cmd->Payload;

	/*
	 * For VersalNet, To generate random mask, ECC library internally uses TRNG hardware.
	 * if TNRG hardware is not initialized then XSecure_TrngInit API initializes it to HRNG mode
	 */
	Status = XSecure_TrngInit();
	if (Status != XST_SUCCESS) {
		goto END;
	}

	switch (Cmd->CmdId & XSECURE_API_ID_MASK) {
	case XSECURE_API(XSECURE_API_ELLIPTIC_GENERATE_KEY):
		Status = XSecure_EllipticGenKey(Pload[0], Pload[1], Pload[2],
				Pload[3], Pload[4]);
		break;
	case XSECURE_API(XSECURE_API_ELLIPTIC_GENERATE_SIGN):
		Status = XSecure_EllipticGenSign(Pload[0], Pload[1], Pload[2],
				Pload[3]);
		break;
	case XSECURE_API(XSECURE_API_ELLIPTIC_VALIDATE_KEY):
		Status = XSecure_EllipticValidatePubKey(Pload[0], Pload[1],
				Pload[2]);
		break;
	case XSECURE_API(XSECURE_API_ELLIPTIC_VERIFY_SIGN):
		Status = XSecure_EllipticVerifySignature(Pload[0], Pload[1]);
		break;
	default:
		XSecure_Printf(XSECURE_DEBUG_GENERAL, "CMD: INVALID PARAM\r\n");
		Status = XST_INVALID_PARAM;
		break;
	}

END:
	return Status;
}

/*****************************************************************************/
/**
 * @brief       This function handler calls XSecure_EllipticGenerateKey
 * 		server API
 *
 * @param	CurveType	- Is a type of elliptic curve
 * 		SrcAddrLow	- Lower 32 bit address of the
 * 				static private key
 * 		SrcAddrHigh	- Higher 32 bit address of the
 * 				static private key
 * 		DstAddrLow	- Lower 32 bit address of the public key
 * 				to be stored
 * 		DstAddrHigh	- Higher 32 bit address of the public key
 * 				to be stored
 *
 * @return
 *	-	XST_SUCCESS - If the elliptic key generation is successful
 *	-	ErrorCode - If there is a failure
 *
 ******************************************************************************/
static int XSecure_EllipticGenKey(u32 CurveType, u32 SrcAddrLow,
	u32 SrcAddrHigh, u32 DstAddrLow, u32 DstAddrHigh)
{
	volatile int Status = XST_FAILURE;
	volatile int StatusTmp = XST_FAILURE;
	u64 SrcAddr = ((u64)SrcAddrHigh << 32U) | (u64)SrcAddrLow;
	u64 DstAddr = ((u64)DstAddrHigh << 32U) | (u64)DstAddrLow;
	u32 OffSet = 0U;

	if (CurveType == (u32)XSECURE_ECC_NIST_P384) {
		OffSet = XSECURE_ECC_P384_SIZE_IN_BYTES;
	}
	else if (CurveType == (u32)XSECURE_ECC_NIST_P521) {
		OffSet = XSECURE_ECC_P521_SIZE_IN_BYTES;
	}
	else {
		Status = XST_INVALID_PARAM;
		goto END;
	}

	XSecure_EllipticKeyAddr KeyAddr = {DstAddr, (DstAddr + (u64)OffSet)};
	Status = XSecure_EllipticGenerateKey_64Bit(
			(XSecure_EllipticCrvTyp)CurveType,
			SrcAddr, (XSecure_EllipticKeyAddr *) &KeyAddr);
	if (Status != XST_SUCCESS) {
		goto END;
	}

	if (XPlmi_IsCryptoKatEn() == TRUE) {
		XPlmi_ClearKatMask(XPLMI_SECURE_ECC_PWCT_KAT_MASK);
		XPLMI_HALT_BOOT_SLD_TEMPORAL_CHECK(XSECURE_KAT_MAJOR_ERROR, Status, StatusTmp, XSecure_EllipticPwct, CurveType,
			SrcAddr, (XSecure_EllipticKeyAddr *)&KeyAddr);
		if ((Status == XST_SUCCESS) && (Status == XST_SUCCESS)) {
			XPlmi_SetKatMask(XPLMI_SECURE_ECC_PWCT_KAT_MASK);
		}
	}

END:
	return Status;
}

/*****************************************************************************/
/**
 * @brief       This function handler calls XSecure_EllipticGenerateSignature
 * 		server API
 *
 * @param
 * 		SrcAddrLow	- Lower 32 bit address of the
 * 				XSecure_EllipticSignGenParams structure
 * 		SrcAddrHigh	- Higher 32 bit address of the
 * 				XSecure_EllipticSignGenParams structure
 * 		DstAddrLow	- Lower 32 bit address of the signature
 * 				to be stored
 * 		DstAddrHigh	- Higher 32 bit address of the signature
 * 				to be stored
 *
 * @return
 *	-	XST_SUCCESS - If the elliptic sign generation is successful
 *	-	ErrorCode - If there is a failure
 *
 ******************************************************************************/
static int XSecure_EllipticGenSign(u32 SrcAddrLow, u32 SrcAddrHigh,
	u32 DstAddrLow, u32 DstAddrHigh)
{
	volatile int Status = XST_FAILURE;
	u64 SrcAddr = ((u64)SrcAddrHigh << 32U) | (u64)SrcAddrLow;
	u64 DstAddr = ((u64)DstAddrHigh << 32U) | (u64)DstAddrLow;
	XSecure_EllipticSignGenParams EcdsaParams;

	if (XPlmi_IsKatRan(XPLMI_SECURE_ECC_SIGN_GEN_SHA3_384_KAT_MASK) != TRUE) {
		Status = XSECURE_ERR_KAT_NOT_EXECUTED;
		goto END;
	}

	Status = XPlmi_MemCpy64((UINTPTR)&EcdsaParams, SrcAddr, sizeof(EcdsaParams));
	if (Status != XST_SUCCESS) {
		goto END;
	}

	XSecure_EllipticHashData HashInfo = {EcdsaParams.HashAddr,
			EcdsaParams.Size};
	XSecure_EllipticSignAddr SignAddr = {DstAddr,
			(DstAddr + (u64)EcdsaParams.Size)};

	Status = XST_FAILURE;
	Status = XSecure_EllipticGenerateSignature_64Bit(
			(XSecure_EllipticCrvTyp)EcdsaParams.CurveType,
			(XSecure_EllipticHashData *) &HashInfo,
			EcdsaParams.PrivKeyAddr,
			EcdsaParams.EPrivKeyAddr,
			(XSecure_EllipticSignAddr *) &SignAddr);

END:
	return Status;
}

/*****************************************************************************/
/**
 * @brief       This function handler calls XSecure_EllipticValidateKey
 * 		server API
 *
 * @param	CurveType	- Is a type of elliptic curve
 * 		SrcAddrLow	- Lower 32 bit address of the public key
 * 		SrcAddrHigh	- Higher 32 bit address of the public key
 *
 * @return
 *	-	XST_SUCCESS - If the elliptic key validation is successful
 *	-	ErrorCode - If there is a failure
 *
 ******************************************************************************/
static int XSecure_EllipticValidatePubKey(u32 CurveType, u32 SrcAddrLow,
	u32 SrcAddrHigh)
{
	volatile int Status = XST_FAILURE;
	u64 SrcAddr = ((u64)SrcAddrHigh << 32U) | (u64)SrcAddrLow;
	u32 Size;

	if (CurveType == (u32)XSECURE_ECC_NIST_P384) {
		Size = XSECURE_ECC_P384_SIZE_IN_BYTES;
	}
	else if (CurveType == (u32)XSECURE_ECC_NIST_P521) {
		Size = XSECURE_ECC_P521_SIZE_IN_BYTES;
	}
	else {
		Status = XST_INVALID_PARAM;
		goto END;
	}

	XSecure_EllipticKeyAddr KeyAddr = {SrcAddr, (SrcAddr + (u64)Size)};
	Status = XSecure_EllipticValidateKey_64Bit(
			(XSecure_EllipticCrvTyp)CurveType,
			(XSecure_EllipticKeyAddr *) &KeyAddr);
END:
	return Status;
}

/*****************************************************************************/
/**
 * @brief       This function handler calls XSecure_EllipticVerifySign
 * 		server API
 *
 * @param
 * 		SrcAddrLow	- Lower 32 bit address of the
 * 				XSecure_EllipticSignVerifyParams structure
 * 		SrcAddrHigh	- Higher 32 bit address of the
 * 				XSecure_EllipticSignVerifyParams structure
 *
 * @return
 *	-	XST_SUCCESS - If the elliptic sign verify is successful
 *	-	ErrorCode - If there is a failure
 *
 ******************************************************************************/
static int XSecure_EllipticVerifySignature(u32 SrcAddrLow, u32 SrcAddrHigh)
{
	volatile int Status = XST_FAILURE;
	u64 Addr = ((u64)SrcAddrHigh << 32U) | (u64)SrcAddrLow;
	XSecure_EllipticSignVerifyParams EcdsaParams;

	if (XPlmi_IsKatRan(XPLMI_SECURE_ECC_SIGN_VERIFY_SHA3_384_KAT_MASK) != TRUE) {
		Status = XSECURE_ERR_KAT_NOT_EXECUTED;
		goto END;
	}

	Status = XPlmi_MemCpy64((UINTPTR)&EcdsaParams, Addr, sizeof(EcdsaParams));
	if (Status != XST_SUCCESS) {
		goto END;
	}

	XSecure_EllipticKeyAddr KeyAddr = {EcdsaParams.PubKeyAddr,
			(EcdsaParams.PubKeyAddr + (u64)EcdsaParams.Size)};
	XSecure_EllipticHashData HashInfo = {EcdsaParams.HashAddr,
			EcdsaParams.Size};
	XSecure_EllipticSignAddr SignAddr = {EcdsaParams.SignAddr,
			(EcdsaParams.SignAddr + (u64)EcdsaParams.Size)};

	Status = XST_FAILURE;
	Status = XSecure_EllipticVerifySign_64Bit(
			(XSecure_EllipticCrvTyp)EcdsaParams.CurveType,
			(XSecure_EllipticHashData *) &HashInfo,
			(XSecure_EllipticKeyAddr *) &KeyAddr,
			(XSecure_EllipticSignAddr *) &SignAddr);

END:
	return Status;
}

#endif
