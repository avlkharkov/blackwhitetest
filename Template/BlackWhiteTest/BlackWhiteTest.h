/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

/*
	BlackWhiteTest.h
*/

#pragma once

#ifndef BLACKWHITETEST_H
#define BLACKWHITETEST_H

typedef unsigned char		u_char;
typedef unsigned short		u_short;
typedef unsigned short		u_int16;
typedef unsigned long		u_long;
typedef short int			int16;
#define PF_TABLE_BITS	12
#define PF_TABLE_SZ_16	4096

#define PF_DEEP_COLOR_AWARE 1	// make sure we get 16bpc pixels; 
								// AE_Effect.h checks for this.

#include "AEConfig.h"

#ifdef AE_OS_WIN
	typedef unsigned short PixelType;
	#include <Windows.h>
#endif

#include "entry.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_Macros.h"
#include "Param_Utils.h"
#include "AE_EffectCBSuites.h"
#include "String_Utils.h"
#include "AE_GeneralPlug.h"
#include "AEFX_ChannelDepthTpl.h"
#include "AEGP_SuiteHandler.h"

#include "BlackWhiteTest_Strings.h"

/* Versioning information */

#define	MAJOR_VERSION	1
#define	MINOR_VERSION	0
#define	BUG_VERSION		0
#define	STAGE_VERSION	PF_Stage_DEVELOP
#define	BUILD_VERSION	1


/* Parameter defaults */

#define BLACKWHITETEST_WEIGHT_MIN       -100
#define BLACKWHITETEST_WEIGHT_MAX        100
#define BLACKWHITETEST_R_WEIGHT_DFLT     30
#define BLACKWHITETEST_G_WEIGHT_DFLT     59
#define BLACKWHITETEST_B_WEIGHT_DFLT     11

enum {
	BLACKWHITETEST_INPUT = 0,
	BLACKWHITETEST_R_WEIGHT,
	BLACKWHITETEST_G_WEIGHT,
	BLACKWHITETEST_B_WEIGHT,
	BLACKWHITETEST_NUM_PARAMS
};

enum {
	R_WEIGHT_DISK_ID = 1,
	G_WEIGHT_DISK_ID,
	B_WEIGHT_DISK_ID,
};

typedef struct WeightInfo{
	PF_FpLong	rWeightF;
	PF_FpLong	gWeightF;
	PF_FpLong	bWeightF;
} WeightInfo, *WeightInfoP, **WeightInfoH;


extern "C" {

	DllExport
	PF_Err
	EffectMain(
		PF_Cmd			cmd,
		PF_InData		*in_data,
		PF_OutData		*out_data,
		PF_ParamDef		*params[],
		PF_LayerDef		*output,
		void			*extra);

}

#endif // BLACKWHITETEST_H