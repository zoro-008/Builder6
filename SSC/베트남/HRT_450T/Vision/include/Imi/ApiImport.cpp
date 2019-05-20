//#include "stdafx.h"
//#ifdef NO_IMCAMLIB_FILE

#include "apiimport.h"

PFuncImCamInit						ImCamInit;	
PFuncImCamUninit					ImCamUninit;
PFuncImCamGetAPIVersion				ImCamGetAPIVersion;

PFuncImCamGetName					ImCamGetName;
PFuncImCamGetModelName				ImCamGetModelName;
PFuncImCamGetFirmwareVersion		ImCamGetFirmwareVersion;

PFuncImCamGetFormatList				ImCamGetFormatList;
PFuncImCamSetVideoFormat			ImCamSetVideoFormat;
PFuncImCamGetVideoFormat			ImCamGetVideoFormat;
PFuncImCamGetModeList				ImCamGetModeList;
PFuncImCamSetVideoMode				ImCamSetVideoMode;
PFuncImCamGetVideoMode				ImCamGetVideoMode;
PFuncImCamGetMaxSizeInfo			ImCamGetMaxSizeInfo;
PFuncImCamGetFrameRateList			ImCamGetFrameRateList;
PFuncImCamSetFrameRate				ImCamSetFrameRate;
PFuncImCamGetFrameRate				ImCamGetFrameRate;

PFuncImCamOpen						ImCamOpen;
PFuncImCamClose						ImCamClose;
PFuncImCamIsOpen					ImCamIsOpen;
PFuncImCamStart						ImCamStart;	
PFuncImCamStop						ImCamStop;
PFuncImCamGetRealFrame				ImCamGetRealFrame;

// Removed from version 3.0.0.0
/*
PFuncImCamIsFeatureSupport			ImCamIsFeatureSupport;
PFuncImCamIsFeatureAuto				ImCamIsFeatureAuto;
PFuncImCamSetFeatureAuto			ImCamSetFeatureAuto;
PFuncImCamIsOnePushSupport			ImCamIsOnePushSupport;
PFuncImCamSetOnePush				ImCamSetOnePush;
PFuncImCamGetFeatureRange			ImCamGetFeatureRange;
*/
PFuncImCamGetFeatureValue			ImCamGetFeatureValue;
PFuncImCamSetFeatureValue			ImCamSetFeatureValue;
PFuncImCamSetFeatureDefault			ImCamSetFeatureDefault;
PFuncImCamSetAllFeatureDefault		ImCamSetAllFeatureDefault;
PFuncImCamGetDataBits				ImCamGetDataBits;
// Removed from version 3.0.0.0
/*
PFuncImCamSetAWBRegion				ImCamSetAWBRegion;
PFuncImCamSetDefaultAWBRegion		ImCamSetDefaultAWBRegion;
*/

PFuncImCamGetImageSize				ImCamGetImageSize;
PFuncImCamGetBufSize				ImCamGetBufSize;
PFuncImCamSetCallback				ImCamSetCallback;
PFuncImCamSaveBMP					ImCamSaveBMP;
PFuncImCamSaveTIF					ImCamSaveTIF;
PFuncImCamSaveJPG					ImCamSaveJPG;
PFuncImCamStartAVICapture			ImCamStartAVICapture;	
PFuncImCamStopAVICapture			ImCamStopAVICapture;
PFuncImCamStartWMVCapture			ImCamStartWMVCapture;
PFuncImCamStopWMVCapture			ImCamStopWMVCapture;

PFuncImCamIsROISupport				ImCamIsROISupport;
PFuncImCamEnableROIMode				ImCamEnableROIMode;
PFuncImCamDisableROIMode			ImCamDisableROIMode;
PFuncImCamGetROISize				ImCamGetROISize;
PFuncImCamGetMaxArea				ImCamGetMaxArea;
PFuncImCamGetCurrentArea			ImCamGetCurrentArea;
PFuncImCamSetCurrentArea			ImCamSetCurrentArea;
PFuncImCamSetBytePerPacket			ImCamSetBytePerPacket;

PFuncImCamSetMirror					ImCamSetMirror;
PFuncImCamGetMirror					ImCamGetMirror;
PFuncImCamSetFlip					ImCamSetFlip;
PFuncImCamGetFlip					ImCamGetFlip;
PFuncImCamSetNegative				ImCamSetNegative;
PFuncImCamGetNegative				ImCamGetNegative;

PFuncImCamGetTriggerMode			ImCamGetTriggerMode;
PFuncImCamSetTriggerMode			ImCamSetTriggerMode;
PFuncImCamGetTriggerInfo			ImCamGetTriggerInfo;
PFuncImCamGetTriggerDelay			ImCamGetTriggerDelay;
// Removed from version 3.0.0.0
//PFuncImCamSetTriggerDelayUnit		ImCamSetTriggerDelayUnit;
PFuncImCamSetTriggerDelay			ImCamSetTriggerDelay;
PFuncImCamGenerateSWTrigger			ImCamGenerateSWTrigger;

PFuncImCamGetMultiShot				ImCamGetMultiShot;
PFuncImCamSetMultiShotMode			ImCamSetMultiShotMode;
PFuncImCamSetMultiShot				ImCamSetMultiShot;

PFuncImCamGetStrobe					ImCamGetStrobe;
// Removed from version 3.0.0.0
//PFuncImCamSetStrobeUnit				ImCamSetStrobeUnit;
PFuncImCamSetStrobe					ImCamSetStrobe;

PFuncImCamGetMemoryChannel			ImCamGetMemoryChannel;
PFuncImCamSetMemoryChannel			ImCamSetMemoryChannel;
PFuncImCamSetDefaultMemoryChannel	ImCamSetDefaultMemoryChannel;

PFuncImCamReadPIO					ImCamReadPIO;
PFuncImCamWritePIO					ImCamWritePIO;

PFuncImCamGetSIO					ImCamGetSIO;
PFuncImCamSetSIO					ImCamSetSIO;
PFuncImCamReadSIO					ImCamReadSIO;
PFuncImCamWriteSIO					ImCamWriteSIO;

PFuncImCamReadQuadlet				ImCamReadQuadlet;
PFuncImCamReadBlock					ImCamReadBlock;
PFuncImCamWriteQuadlet				ImCamWriteQuadlet;
PFuncImCamWriteBlock				ImCamWriteBlock;
PFuncImCamWriteBroadcast			ImCamWriteBroadcast;

PFuncImCamShowFilterDlg				ImCamShowFilterDlg;
PFuncImCamSetDataBits				ImCamSetDataBits;
PFuncImCamSetBayerConvert			ImCamSetBayerConvert;
PFuncImCamGetBayerConvert			ImCamGetBayerConvert;
// Removed from version 3.0.0.0
/*
PFuncImCamSetBayerConvertGain		ImCamSetBayerConvertGain;
PFuncImCamGetBayerConvertGain		ImCamGetBayerConvertGain;
PFuncImCamSetSWAWBOnePush			ImCamSetSWAWBOnePush;
*/

PFuncImCamGetKneePoint				ImCamGetKneePoint;
PFuncImCamSetKneePoint				ImCamSetKneePoint;
PFuncImCamGetLUT					ImCamGetLUT;
PFuncImCamSetLUT					ImCamSetLUT;

PFuncImCamGetFrameSave				ImCamGetFrameSave;
PFuncImCamSetFrameSave				ImCamSetFrameSave;	

HMODULE g_hImiDll = NULL;

BOOL LoadImCamApi(char* szDllPathname)
{
	g_hImiDll = LoadLibrary(szDllPathname);
	if(g_hImiDll == NULL) return FALSE;

	// initialize functions
	ImCamInit = (PFuncImCamInit)GetProcAddress(g_hImiDll, "ImCamInit");	
	ImCamUninit = (PFuncImCamUninit)GetProcAddress(g_hImiDll, "ImCamUninit");	
	ImCamGetAPIVersion = (PFuncImCamGetAPIVersion)GetProcAddress(g_hImiDll, "ImCamGetAPIVersion");

	// camera information functions
	ImCamGetName = (PFuncImCamGetName)GetProcAddress(g_hImiDll, "ImCamGetName");	
	ImCamGetModelName = (PFuncImCamGetModelName)GetProcAddress(g_hImiDll, "ImCamGetModelName");	
	ImCamGetFirmwareVersion = (PFuncImCamGetFirmwareVersion)GetProcAddress(g_hImiDll, "ImCamGetFirmwareVersion");

	// format functions
	ImCamGetFormatList = (PFuncImCamGetFormatList)GetProcAddress(g_hImiDll, "ImCamGetFormatList");	
	ImCamSetVideoFormat = (PFuncImCamSetVideoFormat)GetProcAddress(g_hImiDll, "ImCamSetVideoFormat");	
	ImCamGetVideoFormat = (PFuncImCamGetVideoFormat)GetProcAddress(g_hImiDll, "ImCamGetVideoFormat");	

	// mode functions
	ImCamGetModeList = (PFuncImCamGetModeList)GetProcAddress(g_hImiDll, "ImCamGetModeList");	
	ImCamSetVideoMode = (PFuncImCamSetVideoMode)GetProcAddress(g_hImiDll, "ImCamSetVideoMode");	
	ImCamGetVideoMode = (PFuncImCamGetVideoMode)GetProcAddress(g_hImiDll, "ImCamGetVideoMode");	
	ImCamGetMaxSizeInfo = (PFuncImCamGetMaxSizeInfo)GetProcAddress(g_hImiDll, "ImCamGetMaxSizeInfo");

	// framerate functions
	ImCamGetFrameRateList = (PFuncImCamGetFrameRateList)GetProcAddress(g_hImiDll, "ImCamGetFrameRateList");	
	ImCamSetFrameRate = (PFuncImCamSetFrameRate)GetProcAddress(g_hImiDll, "ImCamSetFrameRate");	
	ImCamGetFrameRate = (PFuncImCamGetFrameRate)GetProcAddress(g_hImiDll, "ImCamGetFrameRate");	

	// display functions
	ImCamOpen = (PFuncImCamOpen)GetProcAddress(g_hImiDll, "ImCamOpen");	
	ImCamClose = (PFuncImCamClose)GetProcAddress(g_hImiDll, "ImCamClose");	
	ImCamIsOpen = (PFuncImCamIsOpen)GetProcAddress(g_hImiDll, "ImCamIsOpen");	
	ImCamStart = (PFuncImCamStart)GetProcAddress(g_hImiDll, "ImCamStart");	
	ImCamStop = (PFuncImCamStop)GetProcAddress(g_hImiDll, "ImCamStop");
	ImCamGetRealFrame = (PFuncImCamGetRealFrame)GetProcAddress(g_hImiDll, "ImCamGetRealFrame");

	// feature functions
	// Removed from version 3.0.0.0
	/*
	ImCamIsFeatureSupport = (PFuncImCamIsFeatureSupport)GetProcAddress(g_hImiDll, "ImCamIsFeatureSupport");	
	ImCamIsFeatureAuto = (PFuncImCamIsFeatureAuto)GetProcAddress(g_hImiDll, "ImCamIsFeatureAuto");	
	ImCamSetFeatureAuto = (PFuncImCamSetFeatureAuto)GetProcAddress(g_hImiDll, "ImCamSetFeatureAuto");	
	ImCamIsOnePushSupport = (PFuncImCamIsOnePushSupport)GetProcAddress(g_hImiDll, "ImCamIsOnePushSupport");	
	ImCamSetOnePush = (PFuncImCamSetOnePush)GetProcAddress(g_hImiDll, "ImCamSetOnePush");	
	ImCamGetFeatureRange = (PFuncImCamGetFeatureRange)GetProcAddress(g_hImiDll, "ImCamGetFeatureRange");	
	*/
	ImCamGetFeatureValue = (PFuncImCamGetFeatureValue)GetProcAddress(g_hImiDll, "ImCamGetFeatureValue");	
	ImCamSetFeatureValue = (PFuncImCamSetFeatureValue)GetProcAddress(g_hImiDll, "ImCamSetFeatureValue");	
	ImCamSetFeatureDefault = (PFuncImCamSetFeatureDefault)GetProcAddress(g_hImiDll, "ImCamSetFeatureDefault");	
	ImCamSetAllFeatureDefault = (PFuncImCamSetAllFeatureDefault)GetProcAddress(g_hImiDll, "ImCamSetAllFeatureDefault");	
	ImCamGetDataBits = (PFuncImCamGetDataBits)GetProcAddress(g_hImiDll, "ImCamGetDataBits");
	// Removed from version 3.0.0.0
	/*
	ImCamSetAWBRegion = (PFuncImCamSetAWBRegion)GetProcAddress(g_hImiDll, "ImCamSetAWBRegion");	
	ImCamSetDefaultAWBRegion = (PFuncImCamSetDefaultAWBRegion)GetProcAddress(g_hImiDll, "ImCamSetDefaultAWBRegion");	
	*/
	
	// size and capture functions
	ImCamGetImageSize = (PFuncImCamGetImageSize)GetProcAddress(g_hImiDll, "ImCamGetImageSize");
	ImCamGetBufSize = (PFuncImCamGetBufSize)GetProcAddress(g_hImiDll, "ImCamGetBufSize");
	ImCamSetCallback = (PFuncImCamSetCallback)GetProcAddress(g_hImiDll, "ImCamSetCallback");
	ImCamSaveBMP = (PFuncImCamSaveBMP)GetProcAddress(g_hImiDll, "ImCamSaveBMP");
	ImCamSaveTIF = (PFuncImCamSaveTIF)GetProcAddress(g_hImiDll, "ImCamSaveTIF");
	ImCamSaveJPG = (PFuncImCamSaveJPG)GetProcAddress(g_hImiDll, "ImCamSaveJPG");
	ImCamStartAVICapture = (PFuncImCamStartAVICapture)GetProcAddress(g_hImiDll, "ImCamStartAVICapture");
	ImCamStopAVICapture = (PFuncImCamStopAVICapture)GetProcAddress(g_hImiDll, "ImCamStopAVICapture");
	ImCamStartWMVCapture = (PFuncImCamStartWMVCapture)GetProcAddress(g_hImiDll, "ImCamStartWMVCapture");
	ImCamStopWMVCapture = (PFuncImCamStopWMVCapture)GetProcAddress(g_hImiDll, "ImCamStopWMVCapture");

	// ROI functions
	ImCamIsROISupport = (PFuncImCamIsROISupport)GetProcAddress(g_hImiDll, "ImCamIsROISupport");
	ImCamEnableROIMode = (PFuncImCamEnableROIMode)GetProcAddress(g_hImiDll, "ImCamEnableROIMode");
	ImCamDisableROIMode = (PFuncImCamDisableROIMode)GetProcAddress(g_hImiDll, "ImCamDisableROIMode");
	ImCamGetROISize = (PFuncImCamGetROISize)GetProcAddress(g_hImiDll, "ImCamGetROISize");
	ImCamGetMaxArea = (PFuncImCamGetMaxArea)GetProcAddress(g_hImiDll, "ImCamGetMaxArea");
	ImCamGetCurrentArea = (PFuncImCamGetCurrentArea)GetProcAddress(g_hImiDll, "ImCamGetCurrentArea");
	ImCamSetCurrentArea = (PFuncImCamSetCurrentArea)GetProcAddress(g_hImiDll, "ImCamSetCurrentArea");
	ImCamSetBytePerPacket = (PFuncImCamSetBytePerPacket)GetProcAddress(g_hImiDll, "ImCamSetBytePerPacket");

	// image effect functions
	ImCamSetMirror = (PFuncImCamSetMirror)GetProcAddress(g_hImiDll, "ImCamSetMirror");
	ImCamGetMirror = (PFuncImCamGetMirror)GetProcAddress(g_hImiDll, "ImCamGetMirror");
	ImCamSetFlip = (PFuncImCamSetFlip)GetProcAddress(g_hImiDll, "ImCamSetFlip");
	ImCamGetFlip = (PFuncImCamGetFlip)GetProcAddress(g_hImiDll, "ImCamGetFlip");
	ImCamSetNegative = (PFuncImCamSetNegative)GetProcAddress(g_hImiDll, "ImCamSetNegative");
	ImCamGetNegative = (PFuncImCamGetNegative)GetProcAddress(g_hImiDll, "ImCamGetNegative");

	// trigger functions
	ImCamGetTriggerMode = (PFuncImCamGetTriggerMode)GetProcAddress(g_hImiDll, "ImCamGetTriggerMode");
	ImCamSetTriggerMode = (PFuncImCamSetTriggerMode)GetProcAddress(g_hImiDll, "ImCamSetTriggerMode");
	ImCamGetTriggerInfo = (PFuncImCamGetTriggerInfo)GetProcAddress(g_hImiDll, "ImCamGetTriggerInfo");
	ImCamGetTriggerDelay = (PFuncImCamGetTriggerDelay)GetProcAddress(g_hImiDll, "ImCamGetTriggerDelay");
	// Removed from version 3.0.0.0
	//ImCamSetTriggerDelayUnit = (PFuncImCamSetTriggerDelayUnit)GetProcAddress(g_hImiDll, "ImCamSetTriggerDelayUnit");
	ImCamSetTriggerDelay = (PFuncImCamSetTriggerDelay)GetProcAddress(g_hImiDll, "ImCamSetTriggerDelay");
	ImCamGenerateSWTrigger = (PFuncImCamGenerateSWTrigger)GetProcAddress(g_hImiDll, "ImCamGenerateSWTrigger");

	// one-shot/multi-shot functions
	ImCamGetMultiShot = (PFuncImCamGetMultiShot)GetProcAddress(g_hImiDll, "ImCamGetMultiShot");
	ImCamSetMultiShotMode = (PFuncImCamSetMultiShotMode)GetProcAddress(g_hImiDll, "ImCamSetMultiShotMode");
	ImCamSetMultiShot = (PFuncImCamSetMultiShot)GetProcAddress(g_hImiDll, "ImCamSetMultiShot");

	// strobe functions
	ImCamGetStrobe = (PFuncImCamGetStrobe)GetProcAddress(g_hImiDll, "ImCamGetStrobe");
	// Removed from version 3.0.0.0
	//ImCamSetStrobeUnit = (PFuncImCamSetStrobeUnit)GetProcAddress(g_hImiDll, "ImCamSetStrobeUnit");
	ImCamSetStrobe = (PFuncImCamSetStrobe)GetProcAddress(g_hImiDll, "ImCamSetStrobe");

	// memory channel functions
	ImCamGetMemoryChannel = (PFuncImCamGetMemoryChannel)GetProcAddress(g_hImiDll, "ImCamGetMemoryChannel");
	ImCamSetMemoryChannel = (PFuncImCamSetMemoryChannel)GetProcAddress(g_hImiDll, "ImCamSetMemoryChannel");
	ImCamSetDefaultMemoryChannel = (PFuncImCamSetDefaultMemoryChannel)GetProcAddress(g_hImiDll, "ImCamSetDefaultMemoryChannel");

	// PIO functions
	ImCamReadPIO = (PFuncImCamReadPIO)GetProcAddress(g_hImiDll, "ImCamReadPIO");
	ImCamWritePIO = (PFuncImCamWritePIO)GetProcAddress(g_hImiDll, "ImCamWritePIO");

	// SIO functions
	ImCamGetSIO = (PFuncImCamGetSIO)GetProcAddress(g_hImiDll, "ImCamGetSIO");
	ImCamSetSIO = (PFuncImCamSetSIO)GetProcAddress(g_hImiDll, "ImCamSetSIO");
	ImCamReadSIO = (PFuncImCamReadSIO)GetProcAddress(g_hImiDll, "ImCamReadSIO");
	ImCamWriteSIO = (PFuncImCamWriteSIO)GetProcAddress(g_hImiDll, "ImCamWriteSIO");

	// register functions
	ImCamReadQuadlet = (PFuncImCamReadQuadlet)GetProcAddress(g_hImiDll, "ImCamReadQuadlet");
	ImCamReadBlock = (PFuncImCamReadBlock)GetProcAddress(g_hImiDll, "ImCamReadBlock");
	ImCamWriteQuadlet = (PFuncImCamWriteQuadlet)GetProcAddress(g_hImiDll, "ImCamWriteQuadlet");
	ImCamWriteBlock = (PFuncImCamWriteBlock)GetProcAddress(g_hImiDll, "ImCamWriteBlock");
	ImCamWriteBroadcast = (PFuncImCamWriteBroadcast)GetProcAddress(g_hImiDll, "ImCamWriteBroadcast");

	// DirectX Filter control functions
	ImCamShowFilterDlg = (PFuncImCamShowFilterDlg)GetProcAddress(g_hImiDll, "ImCamShowFilterDlg");
	ImCamSetDataBits = (PFuncImCamSetDataBits)GetProcAddress(g_hImiDll, "ImCamSetDataBits");
	ImCamSetBayerConvert = (PFuncImCamSetBayerConvert)GetProcAddress(g_hImiDll, "ImCamSetBayerConvert");
	ImCamGetBayerConvert = (PFuncImCamGetBayerConvert)GetProcAddress(g_hImiDll, "ImCamGetBayerConvert");
	// Removed from version 3.0.0.0
	/*
	ImCamSetBayerConvertGain = (PFuncImCamSetBayerConvertGain)GetProcAddress(g_hImiDll, "ImCamSetBayerConvertGain");
	ImCamGetBayerConvertGain = (PFuncImCamGetBayerConvertGain)GetProcAddress(g_hImiDll, "ImCamGetBayerConvertGain");
	ImCamSetSWAWBOnePush = (PFuncImCamSetSWAWBOnePush)GetProcAddress(g_hImiDll, "ImCamSetSWAWBOnePush");
	*/

	ImCamGetKneePoint = (PFuncImCamGetKneePoint)GetProcAddress(g_hImiDll, "ImCamGetKneePoint");
	ImCamSetKneePoint = (PFuncImCamSetKneePoint)GetProcAddress(g_hImiDll, "ImCamSetKneePoint");
	ImCamGetLUT = (PFuncImCamGetLUT)GetProcAddress(g_hImiDll, "ImCamGetLUT");
	ImCamSetLUT = (PFuncImCamSetLUT)GetProcAddress(g_hImiDll, "ImCamSetLUT");

	ImCamGetFrameSave = (PFuncImCamGetFrameSave)GetProcAddress(g_hImiDll, "ImCamGetFrameSave");
	ImCamSetFrameSave = (PFuncImCamSetFrameSave)GetProcAddress(g_hImiDll, "ImCamSetFrameSave");

	return TRUE;
}


VOID UnloadImCamApi()
{
	if(g_hImiDll == NULL) return;

	FreeLibrary(g_hImiDll);
	g_hImiDll = NULL;
}

//#endif	// #ifdef NO_IMCAMLIB_FILE
