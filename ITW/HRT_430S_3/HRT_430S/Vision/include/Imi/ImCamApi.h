#ifndef __IMCAM_H__
#define __IMCAM_H__

#include "ImCamDef.h"


// Visual Studio users should define IMCAMAPI_MSC
#ifdef _MBCS	// default directive for MSVC compiler

#ifdef IMCAMLIB_API_EXPORTS
#define IMCAMLIB_API __declspec(dllexport) 
#else
#define IMCAMLIB_API __declspec(dllimport) 
#endif

#else			
#define IMCAMLIB_API 
#endif	// IMCAMAPI_MSC

#ifdef __cplusplus
extern "C"							   
{
#endif

//////////////////////////////////////////////////////////////////////////////
//
//			Camera initialization functions
//
//////////////////////////////////////////////////////////////////////////////
IMCAMLIB_API BOOL WINAPI ImCamInit();	
IMCAMLIB_API VOID WINAPI ImCamUninit();	
IMCAMLIB_API BOOL WINAPI ImCamGetAPIVersion(OUT INT *pVersion);

//////////////////////////////////////////////////////////////////////////////
//
//			Camera selection and information functions
//
//////////////////////////////////////////////////////////////////////////////
IMCAMLIB_API BOOL WINAPI ImCamGetName(IN INT nCamIndex, OUT INT *pnSize, OUT CHAR *pstrName);
IMCAMLIB_API BOOL WINAPI ImCamGetModelName(IN INT nCamIndex, OUT INT *pnSize, OUT CHAR *pstrName);
IMCAMLIB_API BOOL WINAPI ImCamGetFirmwareVersion(IN INT nCamIndex, OUT CHAR *pstrVersion);

//////////////////////////////////////////////////////////////////////////////
//
//			Camera format, mode, frame rate functions
//
//	- First, retrieve format, mode, frame rate
//	- Second, set format, mode and frame rate
//  - format, mode, frame rate information can be retrieved
//
//////////////////////////////////////////////////////////////////////////////
// Camera format
IMCAMLIB_API BOOL WINAPI ImCamGetFormatList(IN INT nCamIndex, IN OUT INT *pnListSize, OUT INT pFormat[]);
IMCAMLIB_API BOOL WINAPI ImCamSetVideoFormat(IN INT nCamIndex, IN INT Format);
IMCAMLIB_API BOOL WINAPI ImCamGetVideoFormat(IN INT nCamIndex, OUT INT *pFormat);
// Camera Mode
IMCAMLIB_API BOOL WINAPI ImCamGetModeList(IN INT nCamIndex, IN INT Format, 
										  IN OUT INT *pnListSize, OUT INT pMode[]);
IMCAMLIB_API BOOL WINAPI ImCamSetVideoMode(IN INT nCamIndex, IN INT Mode);
IMCAMLIB_API BOOL WINAPI ImCamGetVideoMode(IN INT nCamIndex, OUT INT *pMode);
// Added from version 2.1.5.0
IMCAMLIB_API BOOL WINAPI ImCamGetMaxSizeInfo(IN INT nCamIndex, IN INT Foramt, IN INT Mode, OUT INT *pnX, OUT INT *pnY);
// Camera Frame Rates
IMCAMLIB_API BOOL WINAPI ImCamGetFrameRateList(IN INT nCamIndex, IN INT Format, IN INT Mode,
											  IN OUT INT *pnListSize, OUT INT pRate[]);
IMCAMLIB_API BOOL WINAPI ImCamSetFrameRate(IN INT nCamIndex, IN INT Frate);
IMCAMLIB_API BOOL WINAPI ImCamGetFrameRate(IN INT nCamIndex, OUT INT *pFrate);


//////////////////////////////////////////////////////////////////////////////
//
//			Camera display control functions
//
//////////////////////////////////////////////////////////////////////////////
IMCAMLIB_API BOOL WINAPI ImCamOpen(IN INT nCamIndex, IN BOOL bUseCallback, IN BOOL bUsePreview, 
								  IN HWND hPreviewWnd, IN ULONG ulResizeView, 
								  IN BOOL bFullView, IN INT grabMode, 
								  IN BOOL bColorConvert, IN INT nColorPattern);
IMCAMLIB_API BOOL WINAPI ImCamClose(IN INT nCamIndex);
IMCAMLIB_API BOOL WINAPI ImCamIsOpen(IN INT nCamIndex);
IMCAMLIB_API BOOL WINAPI ImCamStart(IN INT nCamIndex);
IMCAMLIB_API BOOL WINAPI ImCamStop(IN INT nCamIndex);
IMCAMLIB_API BOOL WINAPI ImCamGetRealFrame(IN INT nCamIndex, OUT DOUBLE *pRates);


//////////////////////////////////////////////////////////////////////////////
//
//			Camera feature control functions
//
//	- First, query for supporting feature by the camera
//	- Second, query for the auto status of the feature supported by the camera
//	- Thirs, query for supporting one-push mode of the feature supported by the camera
//	- Fourth, query the range of the feature supported by the camera
//	- feature can be set to auto or one-push if support
//	- feature value can be set by ImCamSetFeatureValue function
//	- feature value can be get by ImCamGetFeatureValue function
//	- feature can be set to factory default state
//
//////////////////////////////////////////////////////////////////////////////
// Removed from version 3.0.0.0
/*
IMCAMLIB_API BOOL WINAPI ImCamIsFeatureSupport(IN INT nCamIndex, IN INT feature);
// return value of ImCamFeatureAuto : 1=auto, 0=man, -1=error
IMCAMLIB_API BOOL WINAPI ImCamIsFeatureAuto(IN INT nCamIndex, IN INT feature);
IMCAMLIB_API BOOL WINAPI ImCamSetFeatureAuto(IN INT nCamIndex, IN INT feature, IN BOOL bAuto);
IMCAMLIB_API BOOL WINAPI ImCamIsOnePushSupport(IN INT nCamIndex, IN INT feature);
IMCAMLIB_API BOOL WINAPI ImCamSetOnePush(IN INT nCamIndex, IN INT feature, IN BOOL bOnePush);
IMCAMLIB_API BOOL WINAPI ImCamGetFeatureRange(IN INT nCamIndex, IN INT feature, OUT INT *pMin, OUT INT *pMax);
*/
// Changed from version 3.0.0.0
IMCAMLIB_API BOOL WINAPI ImCamGetFeatureValue(IN INT nCamIndex, IN INT feature, OUT PIMC_FEATURE_PROPERTY pProperty);
// Changed from version 3.0.0.0
IMCAMLIB_API BOOL WINAPI ImCamSetFeatureValue(IN INT nCamIndex, IN INT feature, IN OUT PIMC_FEATURE_PROPERTY pProperty);

IMCAMLIB_API BOOL WINAPI ImCamSetFeatureDefault(IN INT nCamIndex, IN INT feature);
IMCAMLIB_API BOOL WINAPI ImCamSetAllFeatureDefault(IN INT nCamIndex);
IMCAMLIB_API BOOL WINAPI ImCamGetDataBits(IN INT nCamIndex, OUT INT *pnDataBits);
// Removed from version 3.0.0.0
/*
IMCAMLIB_API BOOL WINAPI ImCamSetAWBRegion(IN INT nCamIndex, IN INT nLeft, IN INT nTop, IN INT nRight, IN INT nBottom);
IMCAMLIB_API BOOL WINAPI ImCamSetDefaultAWBRegion(IN INT nCamIndex);
*/
// 2008-07-02
IMCAMLIB_API BOOL WINAPI ImCamSetShutter(IN INT nCamIndex, IN CHAR *pstrValue);
//


//////////////////////////////////////////////////////////////////////////////
//
//			Image size and capture related functions
//	
//	- image size and buffer can be retrieved after calling ImCamOpen function
//
//////////////////////////////////////////////////////////////////////////////
// width size of the image
IMCAMLIB_API BOOL WINAPI ImCamGetImageSize(IN INT nCamIndex, OUT INT *pnWidth, OUT INT *pnHeight);
// total buffer size for one image
IMCAMLIB_API LONG WINAPI ImCamGetBufSize(IN INT nCamIndex);
// set callback function
IMCAMLIB_API BOOL WINAPI ImCamSetCallback(IN INT nCamIndex, 
										  IN INT (CALLBACK *pCallbackFunc)(VOID *pContext, DOUBLE SampleTime, BYTE *pBuf, LONG lBufferSize), 
										  IN VOID *pCBContext);

IMCAMLIB_API BOOL WINAPI ImCamSaveBMP(IN INT nCamIndex, IN CHAR *pstrFileName, 
									  IN BOOL bResize, IN INT nDestWidth, IN INT nDestHeight);
IMCAMLIB_API BOOL WINAPI ImCamSaveTIF(IN INT nCamIndex, IN CHAR *pstrFileName, 
									  IN BOOL bResize, IN INT nDestWidth, IN INT nDestHeight);
IMCAMLIB_API BOOL WINAPI ImCamSaveJPG(IN INT nCamIndex, IN CHAR *pstrFileName, 
									  IN BOOL bResize, IN INT nDestWidth, IN INT nDestHeight);

// Stream Capture
IMCAMLIB_API BOOL WINAPI ImCamStartAVICapture(IN INT nCamIndex, IN CHAR *pstrFileName, IN BOOL bUseCallback, 
											 IN BOOL bView, IN HWND hPreviewWnd, IN BOOL bConvert, IN INT nColorPattern);
IMCAMLIB_API BOOL WINAPI ImCamStopAVICapture(IN INT nCamIndex);
IMCAMLIB_API BOOL WINAPI ImCamStartWMVCapture(IN INT nCamIndex, IN CHAR *pstrFileName, IN BOOL bUseCallback,
											 IN BOOL bView, IN HWND hPreviewWnd, IN BOOL bConvert, IN INT nColorPattern);
IMCAMLIB_API BOOL WINAPI ImCamStopWMVCapture(IN INT nCamIndex);
	
//////////////////////////////////////////////////////////////////////////////
//
//						ROI functions
//
//		This function is related with camera hardware.
//		Please check the camera for ROI support by the camera 
//
//////////////////////////////////////////////////////////////////////////////
IMCAMLIB_API BOOL WINAPI ImCamIsROISupport(IN INT nCamIndex);
IMCAMLIB_API BOOL WINAPI ImCamEnableROIMode(IN INT nCamIndex);
IMCAMLIB_API BOOL WINAPI ImCamDisableROIMode(IN INT nCamIndex);
IMCAMLIB_API BOOL WINAPI ImCamGetROISize(IN INT nCamIndex, OUT INT *pnWidth, OUT INT *pnHeight);
IMCAMLIB_API BOOL WINAPI ImCamGetMaxArea(IN INT nCamIndex, OUT PIMC_ROI_PROPERTY pProp);
IMCAMLIB_API BOOL WINAPI ImCamGetCurrentArea(IN INT nCamIndex, OUT PIMC_ROI_PROPERTY pProp);
IMCAMLIB_API BOOL WINAPI ImCamSetCurrentArea(IN INT nCamIndex, IN IMC_ROI_PROPERTY Prop);
IMCAMLIB_API BOOL WINAPI ImCamSetBytePerPacket(IN INT nCamIndex, IN IMC_ROI_PROPERTY Prop);


//////////////////////////////////////////////////////////////////////////////
//
//			Image effect related functions
//
//////////////////////////////////////////////////////////////////////////////
IMCAMLIB_API BOOL WINAPI ImCamSetMirror(IN INT nCamIndex, IN BOOL bMirror);
IMCAMLIB_API BOOL WINAPI ImCamGetMirror(IN INT nCamIndex);
IMCAMLIB_API BOOL WINAPI ImCamSetFlip(IN INT nCamIndex, IN BOOL bFlip);
IMCAMLIB_API BOOL WINAPI ImCamGetFlip(IN INT nCamIndex);
IMCAMLIB_API BOOL WINAPI ImCamSetNegative(IN INT nCamIndex, IN BOOL bNegative);
IMCAMLIB_API BOOL WINAPI ImCamGetNegative(IN INT nCamIndex);


//////////////////////////////////////////////////////////////////////////////
//
//			Trigger related functions
//
//		Trigger is depend on the camera.
//		Please check the camera manual to see whether trigger support or not.
//
//////////////////////////////////////////////////////////////////////////////
// Normal, Trigger mode setting : false=normal, true=trigger mode
IMCAMLIB_API BOOL WINAPI ImCamGetTriggerMode(IN INT nCamIndex, OUT PIMC_TRIGGER_PROPERTY pTrigProp);
// For 1394 Camera, param should be 0 if mode is 0
IMCAMLIB_API BOOL WINAPI ImCamSetTriggerMode(IN INT nCamIndex, IN IMC_TRIGGER_PROPERTY TrigProp);
IMCAMLIB_API BOOL WINAPI ImCamGetTriggerInfo(IN INT nCamIndex, OUT PIMC_TRIGGER_INFO pTrigInfo);
IMCAMLIB_API BOOL WINAPI ImCamGetTriggerDelay(IN INT nCamIndex, OUT BOOL *pbOnOff, OUT INT *pnValue, OUT INT *pnMinValue, OUT INT *pnMaxValue);
// Removed from version 3.0.0.0
//IMCAMLIB_API BOOL WINAPI ImCamSetTriggerDelayUnit(IN INT nCamIndex, IN BOOL bUnit);
IMCAMLIB_API BOOL WINAPI ImCamSetTriggerDelay(IN INT nCamIndex, IN BOOL bOnOff, IN INT nValue);
IMCAMLIB_API BOOL WINAPI ImCamGenerateSWTrigger(IN INT nCamIndex);


//////////////////////////////////////////////////////////////////////////////
//
//			One-Shot/Multi-Shot related functions
//
//////////////////////////////////////////////////////////////////////////////
IMCAMLIB_API BOOL WINAPI ImCamGetMultiShot(IN INT nCamIndex, OUT PIMC_MULTISHOT_PROPERTY pMultiShotProp);
IMCAMLIB_API BOOL WINAPI ImCamSetMultiShotMode(IN INT nCamIndex, IN BOOL bState);
IMCAMLIB_API BOOL WINAPI ImCamSetMultiShot(IN INT nCamIndex, IN IMC_MULTISHOT_PROPERTY MultiShotProp);


//////////////////////////////////////////////////////////////////////////////
//
//			Strobe related functions
//
//////////////////////////////////////////////////////////////////////////////
IMCAMLIB_API BOOL WINAPI ImCamGetStrobe(IN INT nCamIndex, OUT PIMC_STROBE_PROPERTY pStrobeProp);
// Compatibility for cameras not supporting IIDC 1.31 specification
// 2009-05-08 : Removed from version 3.0.0.0
//IMCAMLIB_API BOOL WINAPI ImCamSetStrobeUnit(IN INT nCamIndex, BOOL bDelayUnit, BOOL bDurationUnit);
IMCAMLIB_API BOOL WINAPI ImCamSetStrobe(IN INT nCamIndex, IN IMC_STROBE_PROPERTY StrobeProp);


//////////////////////////////////////////////////////////////////////////////
//
//			Memory Channel related functions
//
//////////////////////////////////////////////////////////////////////////////
IMCAMLIB_API BOOL WINAPI ImCamGetMemoryChannel(IN INT nCamIndex, OUT PIMC_MEMORY_CHANNEL_PROPERTY pMemoryProp);
// MemoryProp.bSaveLoad=0 -> LOAD, MemoryProp.bSaveLoad=1 -> SAVE
IMCAMLIB_API BOOL WINAPI ImCamSetMemoryChannel(IN INT nCamIndex, IN OUT PIMC_MEMORY_CHANNEL_PROPERTY pMemoryProp);	
IMCAMLIB_API BOOL WINAPI ImCamSetDefaultMemoryChannel(IN INT nCamIndex, IN INT nChannel);


//////////////////////////////////////////////////////////////////////////////
//
//			PIO Control related functions
//
//////////////////////////////////////////////////////////////////////////////
IMCAMLIB_API BOOL WINAPI ImCamReadPIO(IN INT nCamIndex, IN ULONG *pulValue);
IMCAMLIB_API BOOL WINAPI ImCamWritePIO(IN INT nCamIndex, IN ULONG ulValue);


//////////////////////////////////////////////////////////////////////////////
//
//			SIO Control related functions
//
//////////////////////////////////////////////////////////////////////////////
IMCAMLIB_API BOOL WINAPI ImCamGetSIO(IN INT nCamIndex, OUT PIMC_SIO_PROPERTY pSIOProperty);
IMCAMLIB_API BOOL WINAPI ImCamSetSIO(IN INT nCamIndex, IN IMC_SIO_PROPERTY SIOProperty);
IMCAMLIB_API BOOL WINAPI ImCamReadSIO(IN INT nCamIndex, IN OUT PCHAR pString, IN OUT INT *pnLength);
IMCAMLIB_API BOOL WINAPI ImCamWriteSIO(IN INT nCamIndex, IN PCHAR pString);


//////////////////////////////////////////////////////////////////////////////
//
//			Register read, write functions
//
//////////////////////////////////////////////////////////////////////////////
IMCAMLIB_API BOOL WINAPI ImCamReadQuadlet(IN INT nCamIndex, IN ULONG ulAddress, OUT ULONG *pulValue);
IMCAMLIB_API BOOL WINAPI ImCamReadBlock(IN INT nCamIndex, IN ULONG ulAddress, IN ULONG nQuadCount, OUT ULONG *pRet);
IMCAMLIB_API BOOL WINAPI ImCamWriteQuadlet(IN INT nCamIndex, IN ULONG ulAddress, IN ULONG ulValue);
IMCAMLIB_API BOOL WINAPI ImCamWriteBlock(IN INT nCamIndex, IN ULONG ulAddress, IN ULONG nQuadCount, ULONG *pulData);
IMCAMLIB_API BOOL WINAPI ImCamWriteBroadcast(IN INT nCamIndex, IN ULONG ulAddress, IN ULONG ulValue);


//////////////////////////////////////////////////////////////////////////////
//
//			DirectX Filter Control functions
//
//////////////////////////////////////////////////////////////////////////////
IMCAMLIB_API BOOL WINAPI ImCamShowFilterDlg(IN INT nCamIndex);
IMCAMLIB_API BOOL WINAPI ImCamSetDataBits(IN INT nCamIndex, IN INT nBits);
IMCAMLIB_API BOOL WINAPI ImCamSetBayerConvert(IN INT nCamIndex, IN BOOL bConvert, IN INT nAlgo, IN INT nPattern);
IMCAMLIB_API BOOL WINAPI ImCamGetBayerConvert(IN INT nCamIndex, OUT PBOOL pbConvert, OUT PINT pnAlgo, OUT PINT pnPattern);
// 2009-04-27 : Removed from version 3.0.0.0
/*
IMCAMLIB_API BOOL WINAPI ImCamSetBayerConvertGain(IN INT nCamIndex, IN IMC_RGB_GAIN Gain);
IMCAMLIB_API BOOL WINAPI ImCamGetBayerConvertGain(IN INT nCamIndex, OUT PIMC_RGB_GAIN pGain);
IMCAMLIB_API BOOL WINAPI ImCamSetSWAWBOnePush(IN INT nCamIndex, IN BOOL bPush, IN HWND hWnd);
*/


////////////////////////////////////////////////////////////////////////////////
//
//			Lookup Table functions
//
////////////////////////////////////////////////////////////////////////////////
IMCAMLIB_API BOOL WINAPI ImCamGetKneePoint(IN INT nCamIndex, OUT PIMC_KNEE_POINT_PROPERTY pKneeProp);
IMCAMLIB_API BOOL WINAPI ImCamSetKneePoint(IN INT nCamIndex, IN IMC_KNEE_POINT_PROPERTY KneeProp);
IMCAMLIB_API BOOL WINAPI ImCamGetLUT(IN INT nCamIndex, OUT BOOL *pbOnOff, OUT BOOL *pbPath, OUT INT *pnLUTIndex);
IMCAMLIB_API BOOL WINAPI ImCamSetLUT(IN INT nCamIndex, IN BOOL bOnOff, IN BOOL bPath, IN INT nLUTIndex, IN USHORT *pusData);


//////////////////////////////////////////////////////////////////////////////////
//
//			Frame Save Control Functions
//
//////////////////////////////////////////////////////////////////////////////////
IMCAMLIB_API BOOL WINAPI ImCamGetFrameSave(IN INT nCamIndex, OUT BOOL *pbOnOff, OUT INT *pnFrameRemained);
IMCAMLIB_API BOOL WINAPI ImCamSetFrameSave(IN INT nCamIndex, IN BOOL bOnOff, IN BOOL bTransfer, IN INT nFrame);


#ifdef __cplusplus
}
#endif


#endif	// __IMCAM_H__
