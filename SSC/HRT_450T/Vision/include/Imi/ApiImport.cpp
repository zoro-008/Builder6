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

HMODULE g_hDll = NULL;

BOOL LoadImCamApi(char* szDllPathname)
{
	g_hDll = LoadLibrary(szDllPathname);
	if(g_hDll == NULL) return FALSE;

	// initialize functions
	ImCamInit = (PFuncImCamInit)GetProcAddress(g_hDll, "ImCamInit");	
	ImCamUninit = (PFuncImCamUninit)GetProcAddress(g_hDll, "ImCamUninit");	
	ImCamGetAPIVersion = (PFuncImCamGetAPIVersion)GetProcAddress(g_hDll, "ImCamGetAPIVersion");

	// camera information functions
	ImCamGetName = (PFuncImCamGetName)GetProcAddress(g_hDll, "ImCamGetName");	
	ImCamGetModelName = (PFuncImCamGetModelName)GetProcAddress(g_hDll, "ImCamGetModelName");	
	ImCamGetFirmwareVersion = (PFuncImCamGetFirmwareVersion)GetProcAddress(g_hDll, "ImCamGetFirmwareVersion");

	// format functions
	ImCamGetFormatList = (PFuncImCamGetFormatList)GetProcAddress(g_hDll, "ImCamGetFormatList");	
	ImCamSetVideoFormat = (PFuncImCamSetVideoFormat)GetProcAddress(g_hDll, "ImCamSetVideoFormat");	
	ImCamGetVideoFormat = (PFuncImCamGetVideoFormat)GetProcAddress(g_hDll, "ImCamGetVideoFormat");	

	// mode functions
	ImCamGetModeList = (PFuncImCamGetModeList)GetProcAddress(g_hDll, "ImCamGetModeList");	
	ImCamSetVideoMode = (PFuncImCamSetVideoMode)GetProcAddress(g_hDll, "ImCamSetVideoMode");	
	ImCamGetVideoMode = (PFuncImCamGetVideoMode)GetProcAddress(g_hDll, "ImCamGetVideoMode");	
	ImCamGetMaxSizeInfo = (PFuncImCamGetMaxSizeInfo)GetProcAddress(g_hDll, "ImCamGetMaxSizeInfo");

	// framerate functions
	ImCamGetFrameRateList = (PFuncImCamGetFrameRateList)GetProcAddress(g_hDll, "ImCamGetFrameRateList");	
	ImCamSetFrameRate = (PFuncImCamSetFrameRate)GetProcAddress(g_hDll, "ImCamSetFrameRate");	
	ImCamGetFrameRate = (PFuncImCamGetFrameRate)GetProcAddress(g_hDll, "ImCamGetFrameRate");	

	// display functions
	ImCamOpen = (PFuncImCamOpen)GetProcAddress(g_hDll, "ImCamOpen");	
	ImCamClose = (PFuncImCamClose)GetProcAddress(g_hDll, "ImCamClose");	
	ImCamIsOpen = (PFuncImCamIsOpen)GetProcAddress(g_hDll, "ImCamIsOpen");	
	ImCamStart = (PFuncImCamStart)GetProcAddress(g_hDll, "ImCamStart");	
	ImCamStop = (PFuncImCamStop)GetProcAddress(g_hDll, "ImCamStop");
	ImCamGetRealFrame = (PFuncImCamGetRealFrame)GetProcAddress(g_hDll, "ImCamGetRealFrame");

	// feature functions
	// Removed from version 3.0.0.0
	/*
	ImCamIsFeatureSupport = (PFuncImCamIsFeatureSupport)GetProcAddress(g_hDll, "ImCamIsFeatureSupport");	
	ImCamIsFeatureAuto = (PFuncImCamIsFeatureAuto)GetProcAddress(g_hDll, "ImCamIsFeatureAuto");	
	ImCamSetFeatureAuto = (PFuncImCamSetFeatureAuto)GetProcAddress(g_hDll, "ImCamSetFeatureAuto");	
	ImCamIsOnePushSupport = (PFuncImCamIsOnePushSupport)GetProcAddress(g_hDll, "ImCamIsOnePushSupport");	
	ImCamSetOnePush = (PFuncImCamSetOnePush)GetProcAddress(g_hDll, "ImCamSetOnePush");	
	ImCamGetFeatureRange = (PFuncImCamGetFeatureRange)GetProcAddress(g_hDll, "ImCamGetFeatureRange");	
	*/
	ImCamGetFeatureValue = (PFuncImCamGetFeatureValue)GetProcAddress(g_hDll, "ImCamGetFeatureValue");	
	ImCamSetFeatureValue = (PFuncImCamSetFeatureValue)GetProcAddress(g_hDll, "ImCamSetFeatureValue");	
	ImCamSetFeatureDefault = (PFuncImCamSetFeatureDefault)GetProcAddress(g_hDll, "ImCamSetFeatureDefault");	
	ImCamSetAllFeatureDefault = (PFuncImCamSetAllFeatureDefault)GetProcAddress(g_hDll, "ImCamSetAllFeatureDefault");	
	ImCamGetDataBits = (PFuncImCamGetDataBits)GetProcAddress(g_hDll, "ImCamGetDataBits");
	// Removed from version 3.0.0.0
	/*
	ImCamSetAWBRegion = (PFuncImCamSetAWBRegion)GetProcAddress(g_hDll, "ImCamSetAWBRegion");	
	ImCamSetDefaultAWBRegion = (PFuncImCamSetDefaultAWBRegion)GetProcAddress(g_hDll, "ImCamSetDefaultAWBRegion");	
	*/
	
	// size and capture functions
	ImCamGetImageSize = (PFuncImCamGetImageSize)GetProcAddress(g_hDll, "ImCamGetImageSize");
	ImCamGetBufSize = (PFuncImCamGetBufSize)GetProcAddress(g_hDll, "ImCamGetBufSize");
	ImCamSetCallback = (PFuncImCamSetCallback)GetProcAddress(g_hDll, "ImCamSetCallback");
	ImCamSaveBMP = (PFuncImCamSaveBMP)GetProcAddress(g_hDll, "ImCamSaveBMP");
	ImCamSaveTIF = (PFuncImCamSaveTIF)GetProcAddress(g_hDll, "ImCamSaveTIF");
	ImCamSaveJPG = (PFuncImCamSaveJPG)GetProcAddress(g_hDll, "ImCamSaveJPG");
	ImCamStartAVICapture = (PFuncImCamStartAVICapture)GetProcAddress(g_hDll, "ImCamStartAVICapture");
	ImCamStopAVICapture = (PFuncImCamStopAVICapture)GetProcAddress(g_hDll, "ImCamStopAVICapture");
	ImCamStartWMVCapture = (PFuncImCamStartWMVCapture)GetProcAddress(g_hDll, "ImCamStartWMVCapture");
	ImCamStopWMVCapture = (PFuncImCamStopWMVCapture)GetProcAddress(g_hDll, "ImCamStopWMVCapture");

	// ROI functions
	ImCamIsROISupport = (PFuncImCamIsROISupport)GetProcAddress(g_hDll, "ImCamIsROISupport");
	ImCamEnableROIMode = (PFuncImCamEnableROIMode)GetProcAddress(g_hDll, "ImCamEnableROIMode");
	ImCamDisableROIMode = (PFuncImCamDisableROIMode)GetProcAddress(g_hDll, "ImCamDisableROIMode");
	ImCamGetROISize = (PFuncImCamGetROISize)GetProcAddress(g_hDll, "ImCamGetROISize");
	ImCamGetMaxArea = (PFuncImCamGetMaxArea)GetProcAddress(g_hDll, "ImCamGetMaxArea");
	ImCamGetCurrentArea = (PFuncImCamGetCurrentArea)GetProcAddress(g_hDll, "ImCamGetCurrentArea");
	ImCamSetCurrentArea = (PFuncImCamSetCurrentArea)GetProcAddress(g_hDll, "ImCamSetCurrentArea");
	ImCamSetBytePerPacket = (PFuncImCamSetBytePerPacket)GetProcAddress(g_hDll, "ImCamSetBytePerPacket");

	// image effect functions
	ImCamSetMirror = (PFuncImCamSetMirror)GetProcAddress(g_hDll, "ImCamSetMirror");
	ImCamGetMirror = (PFuncImCamGetMirror)GetProcAddress(g_hDll, "ImCamGetMirror");
	ImCamSetFlip = (PFuncImCamSetFlip)GetProcAddress(g_hDll, "ImCamSetFlip");
	ImCamGetFlip = (PFuncImCamGetFlip)GetProcAddress(g_hDll, "ImCamGetFlip");
	ImCamSetNegative = (PFuncImCamSetNegative)GetProcAddress(g_hDll, "ImCamSetNegative");
	ImCamGetNegative = (PFuncImCamGetNegative)GetProcAddress(g_hDll, "ImCamGetNegative");

	// trigger functions
	ImCamGetTriggerMode = (PFuncImCamGetTriggerMode)GetProcAddress(g_hDll, "ImCamGetTriggerMode");
	ImCamSetTriggerMode = (PFuncImCamSetTriggerMode)GetProcAddress(g_hDll, "ImCamSetTriggerMode");
	ImCamGetTriggerInfo = (PFuncImCamGetTriggerInfo)GetProcAddress(g_hDll, "ImCamGetTriggerInfo");
	ImCamGetTriggerDelay = (PFuncImCamGetTriggerDelay)GetProcAddress(g_hDll, "ImCamGetTriggerDelay");
	// Removed from version 3.0.0.0
	//ImCamSetTriggerDelayUnit = (PFuncImCamSetTriggerDelayUnit)GetProcAddress(g_hDll, "ImCamSetTriggerDelayUnit");
	ImCamSetTriggerDelay = (PFuncImCamSetTriggerDelay)GetProcAddress(g_hDll, "ImCamSetTriggerDelay");
	ImCamGenerateSWTrigger = (PFuncImCamGenerateSWTrigger)GetProcAddress(g_hDll, "ImCamGenerateSWTrigger");

	// one-shot/multi-shot functions
	ImCamGetMultiShot = (PFuncImCamGetMultiShot)GetProcAddress(g_hDll, "ImCamGetMultiShot");
	ImCamSetMultiShotMode = (PFuncImCamSetMultiShotMode)GetProcAddress(g_hDll, "ImCamSetMultiShotMode");
	ImCamSetMultiShot = (PFuncImCamSetMultiShot)GetProcAddress(g_hDll, "ImCamSetMultiShot");

	// strobe functions
	ImCamGetStrobe = (PFuncImCamGetStrobe)GetProcAddress(g_hDll, "ImCamGetStrobe");
	// Removed from version 3.0.0.0
	//ImCamSetStrobeUnit = (PFuncImCamSetStrobeUnit)GetProcAddress(g_hDll, "ImCamSetStrobeUnit");
	ImCamSetStrobe = (PFuncImCamSetStrobe)GetProcAddress(g_hDll, "ImCamSetStrobe");

	// memory channel functions
	ImCamGetMemoryChannel = (PFuncImCamGetMemoryChannel)GetProcAddress(g_hDll, "ImCamGetMemoryChannel");
	ImCamSetMemoryChannel = (PFuncImCamSetMemoryChannel)GetProcAddress(g_hDll, "ImCamSetMemoryChannel");
	ImCamSetDefaultMemoryChannel = (PFuncImCamSetDefaultMemoryChannel)GetProcAddress(g_hDll, "ImCamSetDefaultMemoryChannel");

	// PIO functions
	ImCamReadPIO = (PFuncImCamReadPIO)GetProcAddress(g_hDll, "ImCamReadPIO");
	ImCamWritePIO = (PFuncImCamWritePIO)GetProcAddress(g_hDll, "ImCamWritePIO");

	// SIO functions
	ImCamGetSIO = (PFuncImCamGetSIO)GetProcAddress(g_hDll, "ImCamGetSIO");
	ImCamSetSIO = (PFuncImCamSetSIO)GetProcAddress(g_hDll, "ImCamSetSIO");
	ImCamReadSIO = (PFuncImCamReadSIO)GetProcAddress(g_hDll, "ImCamReadSIO");
	ImCamWriteSIO = (PFuncImCamWriteSIO)GetProcAddress(g_hDll, "ImCamWriteSIO");

	// register functions
	ImCamReadQuadlet = (PFuncImCamReadQuadlet)GetProcAddress(g_hDll, "ImCamReadQuadlet");
	ImCamReadBlock = (PFuncImCamReadBlock)GetProcAddress(g_hDll, "ImCamReadBlock");
	ImCamWriteQuadlet = (PFuncImCamWriteQuadlet)GetProcAddress(g_hDll, "ImCamWriteQuadlet");
	ImCamWriteBlock = (PFuncImCamWriteBlock)GetProcAddress(g_hDll, "ImCamWriteBlock");
	ImCamWriteBroadcast = (PFuncImCamWriteBroadcast)GetProcAddress(g_hDll, "ImCamWriteBroadcast");

	// DirectX Filter control functions
	ImCamShowFilterDlg = (PFuncImCamShowFilterDlg)GetProcAddress(g_hDll, "ImCamShowFilterDlg");
	ImCamSetDataBits = (PFuncImCamSetDataBits)GetProcAddress(g_hDll, "ImCamSetDataBits");
	ImCamSetBayerConvert = (PFuncImCamSetBayerConvert)GetProcAddress(g_hDll, "ImCamSetBayerConvert");
	ImCamGetBayerConvert = (PFuncImCamGetBayerConvert)GetProcAddress(g_hDll, "ImCamGetBayerConvert");
	// Removed from version 3.0.0.0
	/*
	ImCamSetBayerConvertGain = (PFuncImCamSetBayerConvertGain)GetProcAddress(g_hDll, "ImCamSetBayerConvertGain");
	ImCamGetBayerConvertGain = (PFuncImCamGetBayerConvertGain)GetProcAddress(g_hDll, "ImCamGetBayerConvertGain");
	ImCamSetSWAWBOnePush = (PFuncImCamSetSWAWBOnePush)GetProcAddress(g_hDll, "ImCamSetSWAWBOnePush");
	*/

	ImCamGetKneePoint = (PFuncImCamGetKneePoint)GetProcAddress(g_hDll, "ImCamGetKneePoint");
	ImCamSetKneePoint = (PFuncImCamSetKneePoint)GetProcAddress(g_hDll, "ImCamSetKneePoint");
	ImCamGetLUT = (PFuncImCamGetLUT)GetProcAddress(g_hDll, "ImCamGetLUT");
	ImCamSetLUT = (PFuncImCamSetLUT)GetProcAddress(g_hDll, "ImCamSetLUT");

	ImCamGetFrameSave = (PFuncImCamGetFrameSave)GetProcAddress(g_hDll, "ImCamGetFrameSave");
	ImCamSetFrameSave = (PFuncImCamSetFrameSave)GetProcAddress(g_hDll, "ImCamSetFrameSave");

	return TRUE;
}


VOID UnloadImCamApi()
{
	if(g_hDll == NULL) return;

	FreeLibrary(g_hDll);
	g_hDll = NULL;
}

//#endif	// #ifdef NO_IMCAMLIB_FILE
