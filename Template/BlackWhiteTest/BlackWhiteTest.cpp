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

/*	BlackWhiteTest.cpp	

	This is a compiling husk of a project. Fill it in with interesting
	pixel processing code.
	
	Revision History

	Version		Change													Engineer	Date
	=======		======													========	======
	1.0			(seemed like a good idea at the time)					bbb			6/1/2002

	1.0			Okay, I'm leaving the version at 1.0,					bbb			2/15/2006
				for obvious reasons; you're going to 
				copy these files directly! This is the
				first XCode version, though.

	1.0			Let's simplify this barebones sample					zal			11/11/2010

	1.0			Added new entry point									zal			9/18/2017

*/

#include "BlackWhiteTest.h"

static PF_Err 
About (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg,
											"%s v%d.%d\r%s",
											STR(StrID_Name), 
											MAJOR_VERSION, 
											MINOR_VERSION, 
											STR(StrID_Description));
	return PF_Err_NONE;
}

static PF_Err 
GlobalSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	out_data->my_version = PF_VERSION(	MAJOR_VERSION, 
										MINOR_VERSION,
										BUG_VERSION, 
										STAGE_VERSION, 
										BUILD_VERSION);

	out_data->out_flags =  PF_OutFlag_DEEP_COLOR_AWARE;	// just 16bpc, not 32bpc
	
	return PF_Err_NONE;
}

static PF_Err 
ParamsSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err		err		= PF_Err_NONE;
	PF_ParamDef	def;	

	AEFX_CLR_STRUCT(def);

	PF_ADD_FLOAT_SLIDERX(STR(StrID_R_Weight_Param_Name),
		BLACKWHITETEST_WEIGHT_MIN,
		BLACKWHITETEST_WEIGHT_MAX,
		BLACKWHITETEST_WEIGHT_MIN,
		BLACKWHITETEST_WEIGHT_MAX,
		BLACKWHITETEST_R_WEIGHT_DFLT,
		PF_Precision_INTEGER,
		0,
		0,
		R_WEIGHT_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_FLOAT_SLIDERX(STR(StrID_G_Weight_Param_Name),
		BLACKWHITETEST_WEIGHT_MIN,
		BLACKWHITETEST_WEIGHT_MAX,
		BLACKWHITETEST_WEIGHT_MIN,
		BLACKWHITETEST_WEIGHT_MAX,
		BLACKWHITETEST_G_WEIGHT_DFLT,
		PF_Precision_INTEGER,
		0,
		0,
		G_WEIGHT_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_FLOAT_SLIDERX(STR(StrID_B_Weight_Param_Name),
		BLACKWHITETEST_WEIGHT_MIN,
		BLACKWHITETEST_WEIGHT_MAX,
		BLACKWHITETEST_WEIGHT_MIN,
		BLACKWHITETEST_WEIGHT_MAX,
		BLACKWHITETEST_B_WEIGHT_DFLT,
		PF_Precision_INTEGER,
		0,
		0,
		B_WEIGHT_DISK_ID);

	out_data->num_params = BLACKWHITETEST_NUM_PARAMS;

	return err;
}

static PF_Err
ConvertToGreyFunc16 (
	void		*refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err		err = PF_Err_NONE;

	WeightInfo	*wiP	= reinterpret_cast<WeightInfo*>(refcon);
	PF_FpLong	brightness = 0;
					
	if (wiP){
		brightness = (inP->red * wiP->rWeightF + inP->green * wiP->gWeightF + inP->blue * wiP->bWeightF) / 100.0;
		if (brightness < 0)
		{
			brightness = 0;
		}

		outP->alpha		=	inP->alpha;
		outP->red		=	MIN(A_u_short(brightness), PF_MAX_CHAN16);
		outP->green		=	MIN(A_u_short(brightness), PF_MAX_CHAN16);
		outP->blue		=	MIN(A_u_short(brightness), PF_MAX_CHAN16);
	}

	return err;
}

static PF_Err
ConvertToGreyFunc8 (
	void		*refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err		err = PF_Err_NONE;

	WeightInfo	*wiP	= reinterpret_cast<WeightInfo*>(refcon);
	PF_FpLong	brightness = 0;
					
	if (wiP){
		brightness = (inP->red * wiP->rWeightF + inP->green * wiP->gWeightF + inP->blue * wiP->bWeightF) / 100.0;
		if (brightness < 0)
		{
			brightness = 0;
		}

		outP->alpha		=	inP->alpha;
		outP->red		=	MIN((A_u_char)brightness, PF_MAX_CHAN8);
		outP->green		=	MIN((A_u_char)brightness, PF_MAX_CHAN8);
		outP->blue		=	MIN((A_u_char)brightness, PF_MAX_CHAN8);
	}

	return err;
}

static PF_Err 
Render (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err				err		= PF_Err_NONE;
	AEGP_SuiteHandler	suites(in_data->pica_basicP);

	/*	Put interesting code here. */
	WeightInfo			wiP;
	AEFX_CLR_STRUCT(wiP);
	A_long				linesL	= 0;

	linesL 		= output->extent_hint.bottom - output->extent_hint.top;
	wiP.rWeightF = params[BLACKWHITETEST_R_WEIGHT]->u.fs_d.value;
	wiP.gWeightF = params[BLACKWHITETEST_G_WEIGHT]->u.fs_d.value;
	wiP.bWeightF = params[BLACKWHITETEST_B_WEIGHT]->u.fs_d.value;

	if (PF_WORLD_IS_DEEP(output)){
		ERR(suites.Iterate16Suite1()->iterate(	in_data,
												0,								// progress base
												linesL,							// progress final
												&params[BLACKWHITETEST_INPUT]->u.ld,	// src 
												NULL,							// area - null for all pixels
												(void*)&wiP,					// refcon - your custom data pointer
												ConvertToGreyFunc16,				// pixel function pointer
												output));
	} else {
		ERR(suites.Iterate8Suite1()->iterate(	in_data,
												0,								// progress base
												linesL,							// progress final
												&params[BLACKWHITETEST_INPUT]->u.ld,	// src 
												NULL,							// area - null for all pixels
												(void*)&wiP,					// refcon - your custom data pointer
												ConvertToGreyFunc8,				// pixel function pointer
												output));	
	}

	return err;
}


extern "C" DllExport
PF_Err PluginDataEntryFunction(
	PF_PluginDataPtr inPtr,
	PF_PluginDataCB inPluginDataCallBackPtr,
	SPBasicSuite* inSPBasicSuitePtr,
	const char* inHostName,
	const char* inHostVersion)
{
	PF_Err result = PF_Err_INVALID_CALLBACK;

	result = PF_REGISTER_EFFECT(
		inPtr,
		inPluginDataCallBackPtr,
		"BlackWhiteTest", // Name
		"ADBE BlackWhiteTest", // Match Name
		"Sample Plug-ins", // Category
		AE_RESERVED_INFO); // Reserved Info

	return result;
}


PF_Err
EffectMain(
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra)
{
	PF_Err		err = PF_Err_NONE;
	
	try {
		switch (cmd) {
			case PF_Cmd_ABOUT:

				err = About(in_data,
							out_data,
							params,
							output);
				break;
				
			case PF_Cmd_GLOBAL_SETUP:

				err = GlobalSetup(	in_data,
									out_data,
									params,
									output);
				break;
				
			case PF_Cmd_PARAMS_SETUP:

				err = ParamsSetup(	in_data,
									out_data,
									params,
									output);
				break;
				
			case PF_Cmd_RENDER:

				err = Render(	in_data,
								out_data,
								params,
								output);
				break;
		}
	}
	catch(PF_Err &thrown_err){
		err = thrown_err;
	}
	return err;
}

