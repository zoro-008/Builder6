#ifndef __APIIMPORT_H__
#define __APIIMPORT_H__

#include <windows.h>
#include "../Lib/ImCamDef.h"

typedef BOOL (WINAPI *LPFuncImCamInit)();	
typedef LPFuncImCamInit PFuncImCamInit;	

typedef VOID (WINAPI *LPFuncImCamUninit)();	
typedef LPFuncImCamUninit PFuncImCamUninit;

typedef BOOL (WINAPI *LPFuncImCamGetAPIVersion)(OUT INT *pVersion);
typedef LPFuncImCamGetAPIVersion PFuncImCamGetAPIVersion;

typedef BOOL (WINAPI *LPFuncImCamGetName)(IN INT nCamIndex, OUT INT *pnSize, OUT CHAR *pstrName);
typedef LPFuncImCamGetName	PFuncImCamGetName;
 
typedef BOOL (WINAPI *LPFuncImCamGetModelName)(IN INT nCamIndex, OUT INT *pnSize, OUT CHAR *pstrName);
typedef LPFuncImCamGetModelName	PFuncImCamGetModelName;

typedef BOOL (WINAPI *LPFuncImCamGetFirmwareVersion)(IN INT nCamIndex, OUT CHAR *pstrVersion);
typedef LPFuncImCamGetFirmwareVersion		PFuncImCamGetFirmwareVersion;

//////////////////////////////////////////////////////////////////////////////
//
//			Camera format, mode, framerate functions
//
//	- First, retrieve format, mode, framerate
//	- Second, set format, mode and framerate
//  - format, mode, framerate information can be retrieved
//
//////////////////////////////////////////////////////////////////////////////
// Camera format
typedef BOOL (WINAPI *LPFuncImCamGetFormatList)(IN INT nCamIndex,
												IN OUT INT *pnListSize, IN OUT INT pFormat[]);
typedef LPFuncImCamGetFormatList	PFuncImCamGetFormatList;

typedef BOOL (WINAPI *LPFuncImCamSetVideoFormat)(IN INT nCamIndex, IN INT format);
typedef LPFuncImCamSetVideoFormat	PFuncImCamSetVideoFormat;

typedef BOOL (WINAPI *LPFuncImCamGetVideoFormat)(IN INT nCamIndex, OUT INT *pFormat);
typedef LPFuncImCamGetVideoFormat	PFuncImCamGetVideoFormat;

// Camera Mode
typedef BOOL (WINAPI *LPFuncImCamGetModeList)(IN INT nCamIndex, IN INT Format, 
											  IN OUT INT *pnListSize, IN OUT INT pMode[]);
typedef LPFuncImCamGetModeList	PFuncImCamGetModeList;

typedef BOOL (WINAPI *LPFuncImCamSetVideoMode)(IN INT nCamIndex, IN INT mode);
typedef LPFuncImCamSetVideoMode	PFuncImCamSetVideoMode;

typedef BOOL (WINAPI *LPFuncImCamGetVideoMode)(IN INT nCamIndex, OUT INT *pMode);
typedef LPFuncImCamGetVideoMode	PFuncImCamGetVideoMode;

typedef BOOL (WINAPI *LPFuncImCamGetMaxSizeInfo)(IN INT nCamIndex, IN INT Foramt, IN INT Mode, OUT INT *pnX, OUT INT *pnY);
typedef LPFuncImCamGetMaxSizeInfo PFuncImCamGetMaxSizeInfo;

// Camera Frame Rates
typedef BOOL (WINAPI *LPFuncImCamGetFrameRateList)(IN INT nCamIndex, IN INT Format, IN INT Mode,
												 IN OUT INT *pnListSize, IN OUT INT pFrate[]);
typedef LPFuncImCamGetFrameRateList	PFuncImCamGetFrameRateList;

typedef BOOL (WINAPI *LPFuncImCamSetFrameRate)(IN INT nCamIndex, IN INT Frate);
typedef LPFuncImCamSetFrameRate		PFuncImCamSetFrameRate;

typedef BOOL (WINAPI *LPFuncImCamGetFrameRate)(IN INT nCamIndex, OUT INT *pFrate);
typedef LPFuncImCamGetFrameRate		PFuncImCamGetFrameRate;


//////////////////////////////////////////////////////////////////////////////
//
//			Camera display control functions
//
//////////////////////////////////////////////////////////////////////////////
typedef BOOL (WINAPI *LPFuncImCamOpen)(IN INT nCamIndex, IN BOOL bUseCallback, IN BOOL bUsePreview, 
										IN HWND hPreviewWnd, IN ULONG ulResizeView, 
										IN BOOL bFullView, IN INT grabMode,
										IN BOOL bColorConvert, IN INT nColorPattern);
typedef LPFuncImCamOpen		PFuncImCamOpen;

typedef BOOL (WINAPI *LPFuncImCamClose)(IN INT nCamIndex);
typedef LPFuncImCamClose	PFuncImCamClose;

typedef BOOL (WINAPI *LPFuncImCamIsOpen)(IN INT nCamIndex);
typedef LPFuncImCamIsOpen	PFuncImCamIsOpen;

typedef BOOL (WINAPI *LPFuncImCamStart)(IN INT nCamIndex);
typedef LPFuncImCamStart	PFuncImCamStart;	

typedef BOOL (WINAPI *LPFuncImCamStop)(IN INT nCamIndex);
typedef LPFuncImCamStop		PFuncImCamStop;

typedef BOOL (WINAPI *LPFuncImCamGetRealFrame)(IN INT nCamIndex, OUT DOUBLE *pRates);
typedef LPFuncImCamGetRealFrame		PFuncImCamGetRealFrame;


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
typedef BOOL (WINAPI *LPFuncImCamIsFeatureSupport)(IN INT nCamIndex, IN INT feature);
typedef LPFuncImCamIsFeatureSupport		PFuncImCamIsFeatureSupport;

// return value of ImCamFeatureAuto : 1=auto, 0=man, -1=error
typedef BOOL (WINAPI *LPFuncImCamIsFeatureAuto)(IN INT nCamIndex, IN INT feature);
typedef LPFuncImCamIsFeatureAuto		PFuncImCamIsFeatureAuto;

typedef BOOL (WINAPI *LPFuncImCamSetFeatureAuto)(IN INT nCamIndex, IN INT feature, IN BOOL bAuto);
typedef LPFuncImCamSetFeatureAuto		PFuncImCamSetFeatureAuto;

typedef BOOL (WINAPI *LPFuncImCamIsOnePushSupport)(IN INT nCamIndex, IN INT feature);
typedef LPFuncImCamIsOnePushSupport		PFuncImCamIsOnePushSupport;

typedef BOOL (WINAPI *LPFuncImCamSetOnePush)(IN INT nCamIndex, IN INT feature, IN BOOL bOnePush);
typedef LPFuncImCamSetOnePush			PFuncImCamSetOnePush;

typedef BOOL (WINAPI *LPFuncImCamGetFeatureRange)(IN INT nCamIndex, IN INT feature, OUT INT *pMin, OUT INT *pMax);
typedef LPFuncImCamGetFeatureRange		PFuncImCamGetFeatureRange;
*/

// return value of ImCamGetFeatureValue : -1=error, else=value
typedef BOOL (WINAPI *LPFuncImCamGetFeatureValue)(IN INT nCamIndex, IN INT feature, OUT PIMC_FEATURE_PROPERTY pProperty);
typedef LPFuncImCamGetFeatureValue		PFuncImCamGetFeatureValue;

typedef BOOL (WINAPI *LPFuncImCamSetFeatureValue)(IN INT nCamIndex, IN INT feature, IN OUT PIMC_FEATURE_PROPERTY pProperty);
typedef LPFuncImCamSetFeatureValue		PFuncImCamSetFeatureValue;

typedef BOOL (WINAPI *LPFuncImCamSetFeatureDefault)(IN INT nCamIndex, IN INT feature);
typedef LPFuncImCamSetFeatureDefault	PFuncImCamSetFeatureDefault;

typedef BOOL (WINAPI *LPFuncImCamSetAllFeatureDefault)(IN INT nCamIndex);
typedef LPFuncImCamSetAllFeatureDefault		PFuncImCamSetAllFeatureDefault;

typedef BOOL (WINAPI *LPFuncImCamGetDataBits)(IN INT nCamIndex, OUT INT *pnDataBits);
typedef LPFuncImCamGetDataBits			PFuncImCamGetDataBits;

// Removed from version 3.0.0.0
/*
typedef BOOL (WINAPI *LPFuncImCamSetAWBRegion)(IN INT nCamIndex, IN INT nLeft, IN INT nTop, IN INT nRight, IN INT nBottom);
typedef LPFuncImCamSetAWBRegion			PFuncImCamSetAWBRegion;

typedef BOOL (WINAPI *LPFuncImCamSetDefaultAWBRegion)(IN INT nCamIndex);
typedef LPFuncImCamSetDefaultAWBRegion	PFuncImCamSetDefaultAWBRegion;
*/


//////////////////////////////////////////////////////////////////////////////
//
//			Image size and capture related functions
//	
//	- image size and buffer can be retrived after calling ImCamOpen function
//
//////////////////////////////////////////////////////////////////////////////
// width size of the image
typedef BOOL (WINAPI *LPFuncImCamGetImageSize)(IN INT nCamIndex, OUT INT *pnWidth, OUT INT *pnHeight);
typedef LPFuncImCamGetImageSize		PFuncImCamGetImageSize;

// total buffer size for one image
typedef LONG (WINAPI *LPFuncImCamGetBufSize)(IN INT nCamIndex);
typedef LPFuncImCamGetBufSize		PFuncImCamGetBufSize;

// set callback function
typedef BOOL (WINAPI *LPFuncImCamSetCallback)(IN INT nCamIndex, 
											IN INT (CALLBACK *pCallbackFunc)(VOID *pContext, DOUBLE SampleTime, BYTE *pBuf, LONG lBufferSize),
											IN VOID *pCBContext);
typedef LPFuncImCamSetCallback		PFuncImCamSetCallback;

typedef BOOL (WINAPI *LPFuncImCamSaveBMP)(IN INT nCamIndex, IN CHAR *pstrFileName, 
										  IN BOOL bResize, IN INT nDestWidth, IN INT nDestHeight);
typedef LPFuncImCamSaveBMP			PFuncImCamSaveBMP;

typedef BOOL (WINAPI *LPFuncImCamSaveTIF)(IN INT nCamIndex, IN CHAR *pstrFileName, 
										  IN BOOL bResize, IN INT nDestWidth, IN INT nDestHeight);
typedef LPFuncImCamSaveTIF			PFuncImCamSaveTIF;

typedef BOOL (WINAPI *LPFuncImCamSaveJPG)(IN INT nCamIndex, IN CHAR *pstrFileName, 
										  IN BOOL bResize, IN INT nDestWidth, IN INT nDestHeight);
typedef LPFuncImCamSaveJPG			PFuncImCamSaveJPG;

// Stream Capture
typedef BOOL (WINAPI *LPFuncImCamStartAVICapture)(IN INT nCamIndex, IN CHAR *pstrFileName, IN BOOL bUseCallback, 
												 IN BOOL bView, IN HWND hPreviewWnd, IN BOOL bConvert, IN INT nColorPattern);
typedef LPFuncImCamStartAVICapture	PFuncImCamStartAVICapture;	

typedef BOOL (WINAPI *LPFuncImCamStopAVICapture)(IN INT nCamIndex);
typedef LPFuncImCamStopAVICapture	PFuncImCamStopAVICapture;

typedef BOOL (WINAPI *LPFuncImCamStartWMVCapture)(IN INT nCamIndex, IN CHAR *pstrFileName, IN BOOL bUseCallback,
											 IN BOOL bView, IN HWND hPreviewWnd, IN BOOL bConvert, IN INT nColorPattern);
typedef LPFuncImCamStartWMVCapture	PFuncImCamStartWMVCapture;

typedef BOOL (WINAPI *LPFuncImCamStopWMVCapture)(IN INT nCamIndex);
typedef	LPFuncImCamStopWMVCapture	PFuncImCamStopWMVCapture;
	
//////////////////////////////////////////////////////////////////////////////
//
//						ROI functions
//
//		This function is related with camera hardware.
//		Please check the camera for ROI support by the camera 
//
//////////////////////////////////////////////////////////////////////////////
typedef BOOL (WINAPI *LPFuncImCamIsROISupport)(IN INT nCamIndex);
typedef	LPFuncImCamIsROISupport		PFuncImCamIsROISupport;

typedef BOOL (WINAPI *LPFuncImCamEnableROIMode)(IN INT nCamIndex);
typedef LPFuncImCamEnableROIMode	PFuncImCamEnableROIMode;

typedef BOOL (WINAPI *LPFuncImCamDisableROIMode)(IN INT nCamIndex);
typedef LPFuncImCamDisableROIMode	PFuncImCamDisableROIMode;

typedef BOOL (WINAPI *LPFuncImCamGetROISize)(IN INT nCamIndex, OUT INT *pnWidth, OUT INT *pnHeight);
typedef LPFuncImCamGetROISize		PFuncImCamGetROISize;

typedef BOOL (WINAPI *LPFuncImCamGetMaxArea)(IN INT nCamIndex, OUT PIMC_ROI_PROPERTY pProp);
typedef LPFuncImCamGetMaxArea		PFuncImCamGetMaxArea;

typedef BOOL (WINAPI *LPFuncImCamGetCurrentArea)(IN INT nCamIndex, OUT PIMC_ROI_PROPERTY pProp);
typedef LPFuncImCamGetCurrentArea	PFuncImCamGetCurrentArea;

typedef BOOL (WINAPI *LPFuncImCamSetCurrentArea)(IN INT nCamIndex, IN IMC_ROI_PROPERTY pProp);
typedef LPFuncImCamSetCurrentArea	PFuncImCamSetCurrentArea;

typedef BOOL (WINAPI *LPFuncImCamSetBytePerPacket)(IN INT nCamIndex, IN IMC_ROI_PROPERTY pProp);
typedef LPFuncImCamSetBytePerPacket PFuncImCamSetBytePerPacket;

//////////////////////////////////////////////////////////////////////////////
//
//			Image effect related functions
//
//////////////////////////////////////////////////////////////////////////////
typedef BOOL (WINAPI *LPFuncImCamSetMirror)(IN INT nCamIndex, IN BOOL bMirror);
typedef	LPFuncImCamSetMirror		PFuncImCamSetMirror;

typedef BOOL (WINAPI *LPFuncImCamGetMirror)(IN INT nCamIndex);
typedef LPFuncImCamGetMirror		PFuncImCamGetMirror;

typedef BOOL (WINAPI *LPFuncImCamSetFlip)(IN INT nCamIndex, IN BOOL bFlip);
typedef LPFuncImCamSetFlip			PFuncImCamSetFlip;

typedef BOOL (WINAPI *LPFuncImCamGetFlip)(IN INT nCamIndex);
typedef LPFuncImCamGetFlip			PFuncImCamGetFlip;

typedef BOOL (WINAPI *LPFuncImCamSetNegative)(IN INT nCamIndex, IN BOOL bNegative);
typedef LPFuncImCamSetNegative		PFuncImCamSetNegative;

typedef BOOL (WINAPI *LPFuncImCamGetNegative)(IN INT nCamIndex);
typedef LPFuncImCamGetNegative		PFuncImCamGetNegative;


//////////////////////////////////////////////////////////////////////////////
//
//			Trigger related functions
//
//		Trigger is depend on the camera.
//		Please check the camera manual to see whether trigger support or not.
//
//////////////////////////////////////////////////////////////////////////////
// Normal, Trigger mode setting : false=normal, true=trigger mode
typedef BOOL (WINAPI *LPFuncImCamGetTriggerMode)(IN INT nCamIndex, OUT PIMC_TRIGGER_PROPERTY pTrigProp);
typedef LPFuncImCamGetTriggerMode		PFuncImCamGetTriggerMode;

// For 1394 Camera, param should be 0 if mode is 0
typedef BOOL (WINAPI *LPFuncImCamSetTriggerMode)(IN INT nCamIndex, IN IMC_TRIGGER_PROPERTY TrigProp);
typedef LPFuncImCamSetTriggerMode		PFuncImCamSetTriggerMode;

typedef BOOL (WINAPI *LPFuncImCamGetTriggerInfo)(IN INT nCamIndex, OUT PIMC_TRIGGER_INFO pTrigInfo);
typedef LPFuncImCamGetTriggerInfo		PFuncImCamGetTriggerInfo;

typedef BOOL (WINAPI *LPFuncImCamGetTriggerDelay)(IN INT nCamIndex, OUT BOOL *pbOnOff, OUT INT *pnValue, OUT INT *pnMinValue, OUT INT *pnMaxValue);
typedef LPFuncImCamGetTriggerDelay		PFuncImCamGetTriggerDelay;

// Removed from version 3.0.0.0
/*
typedef BOOL (WINAPI *LPFuncImCamSetTriggerDelayUnit)(IN INT nCamIndex, IN BOOL bUnit);
typedef LPFuncImCamSetTriggerDelayUnit	PFuncImCamSetTriggerDelayUnit;
*/

typedef BOOL (WINAPI *LPFuncImCamSetTriggerDelay)(IN INT nCamIndex, IN BOOL bOnOff, IN INT nValue);
typedef LPFuncImCamSetTriggerDelay		PFuncImCamSetTriggerDelay;

typedef BOOL (WINAPI *LPFuncImCamGenerateSWTrigger)(IN INT nCamIndex);
typedef LPFuncImCamGenerateSWTrigger	PFuncImCamGenerateSWTrigger;


//////////////////////////////////////////////////////////////////////////////
//
//			One-Shot/Multi-Shot related functions
//
//////////////////////////////////////////////////////////////////////////////
typedef BOOL (WINAPI *LPFuncImCamGetMultiShot)(IN INT nCamIndex, OUT PIMC_MULTISHOT_PROPERTY pMultiShotProp);
typedef LPFuncImCamGetMultiShot			PFuncImCamGetMultiShot;

typedef BOOL (WINAPI *LPFuncImCamSetMultiShotMode)(IN INT nCamIndex, IN BOOL bState);
typedef LPFuncImCamSetMultiShotMode		PFuncImCamSetMultiShotMode;

typedef BOOL (WINAPI *LPFuncImCamSetMultiShot)(IN INT nCamIndex, IN IMC_MULTISHOT_PROPERTY MultiShotProp);
typedef LPFuncImCamSetMultiShot			PFuncImCamSetMultiShot;


//////////////////////////////////////////////////////////////////////////////
//
//			Strobe related functions
//
//////////////////////////////////////////////////////////////////////////////
typedef BOOL (WINAPI *LPFuncImCamGetStrobe)(IN INT nCamIndex, OUT PIMC_STROBE_PROPERTY pStrobeProp);
typedef LPFuncImCamGetStrobe			PFuncImCamGetStrobe;

// Removed from version 3.0.0.0
/*
typedef BOOL (WINAPI *LPFuncImCamSetStrobeUnit)(IN INT nCamIndex, IN BOOL bDelayUnit, IN BOOL bDurationUnit);
typedef LPFuncImCamSetStrobeUnit		PFuncImCamSetStrobeUnit;
*/

typedef BOOL (WINAPI *LPFuncImCamSetStrobe)(IN INT nCamIndex, IN IMC_STROBE_PROPERTY StrobeProp);
typedef LPFuncImCamSetStrobe			PFuncImCamSetStrobe;


//////////////////////////////////////////////////////////////////////////////
//
//			Memory Channel related functions
//
//////////////////////////////////////////////////////////////////////////////
typedef BOOL (WINAPI *LPFuncImCamGetMemoryChannel)(IN INT nCamIndex, OUT PIMC_MEMORY_CHANNEL_PROPERTY pMemoryProp);
typedef LPFuncImCamGetMemoryChannel		PFuncImCamGetMemoryChannel;

// MemoryProp.bSaveLoad=0 -> LOAD, MemoryProp.bSaveLoad=1 -> SAVE
typedef BOOL (WINAPI *LPFuncImCamSetMemoryChannel)(IN INT nCamIndex, IN OUT PIMC_MEMORY_CHANNEL_PROPERTY pMemoryProp);	
typedef LPFuncImCamSetMemoryChannel		PFuncImCamSetMemoryChannel;

typedef BOOL (WINAPI *LPFuncImCamSetDefaultMemoryChannel)(IN INT nCamIndex, IN INT nChannel);
typedef LPFuncImCamSetDefaultMemoryChannel	PFuncImCamSetDefaultMemoryChannel;


//////////////////////////////////////////////////////////////////////////////
//
//			PIO Control related functions
//
//////////////////////////////////////////////////////////////////////////////
typedef BOOL (WINAPI *LPFuncImCamReadPIO)(IN INT nCamIndex, IN ULONG *pulValue);
typedef LPFuncImCamReadPIO		PFuncImCamReadPIO;

typedef BOOL (WINAPI *LPFuncImCamWritePIO)(IN INT nCamIndex, IN ULONG ulValue);
typedef LPFuncImCamWritePIO		PFuncImCamWritePIO;


//////////////////////////////////////////////////////////////////////////////
//
//			SIO Control related functions
//
//////////////////////////////////////////////////////////////////////////////
typedef BOOL (WINAPI *LPFuncImCamGetSIO)(IN INT nCamIndex, OUT PIMC_SIO_PROPERTY pSIOProperty);
typedef LPFuncImCamGetSIO		PFuncImCamGetSIO;

typedef BOOL (WINAPI *LPFuncImCamSetSIO)(IN INT nCamIndex, IN IMC_SIO_PROPERTY SIOProperty);
typedef LPFuncImCamSetSIO		PFuncImCamSetSIO;

typedef BOOL (WINAPI *LPFuncImCamReadSIO)(IN INT nCamIndex, IN OUT PCHAR pString, IN OUT INT *pnLength);
typedef LPFuncImCamReadSIO		PFuncImCamReadSIO;

typedef BOOL (WINAPI *LPFuncImCamWriteSIO)(IN INT nCamIndex, IN PCHAR pString);
typedef LPFuncImCamWriteSIO		PFuncImCamWriteSIO;


//////////////////////////////////////////////////////////////////////////////
//
//			Register read, write functions
//
//////////////////////////////////////////////////////////////////////////////
typedef BOOL (WINAPI *LPFuncImCamReadQuadlet)(IN INT nCamIndex, IN ULONG ulAddress, OUT ULONG *pulValue);
typedef LPFuncImCamReadQuadlet		PFuncImCamReadQuadlet;

typedef BOOL (WINAPI *LPFuncImCamReadBlock)(IN INT nCamIndex, IN ULONG ulAddress, IN ULONG nQuadCount, OUT ULONG *pRet);
typedef LPFuncImCamReadBlock		PFuncImCamReadBlock;

typedef BOOL (WINAPI *LPFuncImCamWriteQuadlet)(IN INT nCamIndex, IN ULONG ulAddress, IN ULONG ulValue);
typedef LPFuncImCamWriteQuadlet		PFuncImCamWriteQuadlet;

typedef BOOL (WINAPI *LPFuncImCamWriteBlock)(IN INT nCamIndex, IN ULONG ulAddress, IN ULONG nQuadCount, IN ULONG *pulData);
typedef LPFuncImCamWriteBlock		PFuncImCamWriteBlock;

typedef BOOL (WINAPI *LPFuncImCamWriteBroadcast)(IN INT nCamIndex, IN ULONG ulAddress, IN ULONG ulValue);
typedef LPFuncImCamWriteBroadcast	PFuncImCamWriteBroadcast;


//////////////////////////////////////////////////////////////////////////////
//
//			DirectX Filter Control functions
//
//////////////////////////////////////////////////////////////////////////////
typedef BOOL (WINAPI *LPFuncImCamShowFilterDlg)(IN INT nCamIndex);
typedef LPFuncImCamShowFilterDlg	PFuncImCamShowFilterDlg;

typedef BOOL (WINAPI *LPFuncImCamSetDataBits)(IN INT nCamIndex, IN INT nBits);
typedef LPFuncImCamSetDataBits		PFuncImCamSetDataBits;

typedef BOOL (WINAPI *LPFuncImCamSetBayerConvert)(IN INT nCamIndex, IN BOOL bConvert, IN INT nAlgo, IN INT nPattern);
typedef LPFuncImCamSetBayerConvert	PFuncImCamSetBayerConvert;

typedef BOOL (WINAPI *LPFuncImCamGetBayerConvert)(IN INT nCamIndex,  PBOOL pbConvert, OUT PINT pnAlgo, OUT PINT pnPattern);
typedef LPFuncImCamGetBayerConvert	PFuncImCamGetBayerConvert;

// Removed from version 3.0.0.0
/*
typedef BOOL (WINAPI *LPFuncImCamSetBayerConvertGain)(IN INT nCamIndex, IN IMC_RGB_GAIN Gain);
typedef LPFuncImCamSetBayerConvertGain	PFuncImCamSetBayerConvertGain;

typedef BOOL (WINAPI *LPFuncImCamGetBayerConvertGain)(IN INT nCamIndex, OUT PIMC_RGB_GAIN pGain);
typedef LPFuncImCamGetBayerConvertGain	PFuncImCamGetBayerConvertGain;

typedef BOOL (WINAPI *LPFuncImCamSetSWAWBOnePush)(IN INT nCamIndex, IN BOOL bPush, IN HWND hWnd);
typedef LPFuncImCamSetSWAWBOnePush	PFuncImCamSetSWAWBOnePush;
*/


////////////////////////////////////////////////////////////////////////////////
//
//			Lookup Table functions
//
////////////////////////////////////////////////////////////////////////////////
typedef BOOL (WINAPI *LPFuncImCamGetKneePoint)(IN INT nCamIndex, OUT PIMC_KNEE_POINT_PROPERTY pKneeProp);
typedef LPFuncImCamGetKneePoint	PFuncImCamGetKneePoint;

typedef BOOL (WINAPI *LPFuncImCamSetKneePoint)(IN INT nCamIndex, IN IMC_KNEE_POINT_PROPERTY KneeProp);
typedef LPFuncImCamSetKneePoint	PFuncImCamSetKneePoint;

typedef BOOL (WINAPI *LPFuncImCamGetLUT)(IN INT nCamIndex, OUT BOOL *pbOnOff, OUT BOOL *pbPath, OUT INT *pnLUTIndex);
typedef LPFuncImCamGetLUT	PFuncImCamGetLUT;

typedef BOOL (WINAPI *LPFuncImCamSetLUT)(IN INT nCamIndex, IN BOOL bOnOff, IN BOOL bPath, IN INT nLUTIndex, IN USHORT *pusData);
typedef LPFuncImCamSetLUT	PFuncImCamSetLUT;


//////////////////////////////////////////////////////////////////////////////////
//
//			Frame Save Control Functions
//
//////////////////////////////////////////////////////////////////////////////////
typedef BOOL (WINAPI *LPFuncImCamGetFrameSave)(IN INT nCamIndex, OUT BOOL *pbOnOff, OUT INT *pnFrameRemained);
typedef LPFuncImCamGetFrameSave	PFuncImCamGetFrameSave;

typedef BOOL (WINAPI *LPFuncImCamSetFrameSave)(IN INT nCamIndex, IN BOOL bOnOff, IN BOOL bTransfer, IN INT nFrame);
typedef LPFuncImCamSetFrameSave	PFuncImCamSetFrameSave;


/////////////////////////////////////////////////////
BOOL LoadImCamApi(char* szDllPathname);
VOID UnloadImCamApi();


///// extern
extern PFuncImCamInit						ImCamInit;	
extern PFuncImCamUninit						ImCamUninit;
extern PFuncImCamGetAPIVersion				ImCamGetAPIVersion;

extern PFuncImCamGetName					ImCamGetName;
extern PFuncImCamGetModelName				ImCamGetModelName;
extern PFuncImCamGetFirmwareVersion			ImCamGetFirmwareVersion;

extern PFuncImCamGetFormatList				ImCamGetFormatList;
extern PFuncImCamSetVideoFormat				ImCamSetVideoFormat;
extern PFuncImCamGetVideoFormat				ImCamGetVideoFormat;
extern PFuncImCamGetModeList				ImCamGetModeList;
extern PFuncImCamSetVideoMode				ImCamSetVideoMode;
extern PFuncImCamGetVideoMode				ImCamGetVideoMode;
extern PFuncImCamGetMaxSizeInfo				ImCamGetMaxSizeInfo;
extern PFuncImCamGetFrameRateList			ImCamGetFrameRateList;
extern PFuncImCamSetFrameRate				ImCamSetFrameRate;
extern PFuncImCamGetFrameRate				ImCamGetFrameRate;

extern PFuncImCamOpen						ImCamOpen;
extern PFuncImCamClose						ImCamClose;
extern PFuncImCamIsOpen						ImCamIsOpen;
extern PFuncImCamStart						ImCamStart;	
extern PFuncImCamStop						ImCamStop;
extern PFuncImCamGetRealFrame				ImCamGetRealFrame;

// Removed from version 3.0.0.0
/*
extern PFuncImCamIsFeatureSupport			ImCamIsFeatureSupport;
extern PFuncImCamIsFeatureAuto				ImCamIsFeatureAuto;
extern PFuncImCamSetFeatureAuto				ImCamSetFeatureAuto;
extern PFuncImCamIsOnePushSupport			ImCamIsOnePushSupport;
extern PFuncImCamSetOnePush					ImCamSetOnePush;
extern PFuncImCamGetFeatureRange			ImCamGetFeatureRange;
*/
extern PFuncImCamGetFeatureValue			ImCamGetFeatureValue;
extern PFuncImCamSetFeatureValue			ImCamSetFeatureValue;
extern PFuncImCamSetFeatureDefault			ImCamSetFeatureDefault;
extern PFuncImCamSetAllFeatureDefault		ImCamSetAllFeatureDefault;
extern PFuncImCamGetDataBits				ImCamGetDataBits;
// Removed from version 3.0.0.0
/*
extern PFuncImCamSetAWBRegion				ImCamSetAWBRegion;
extern PFuncImCamSetDefaultAWBRegion		ImCamSetDefaultAWBRegion;
*/

extern PFuncImCamGetImageSize				ImCamGetImageSize;
extern PFuncImCamGetBufSize					ImCamGetBufSize;
extern PFuncImCamSetCallback				ImCamSetCallback;
extern PFuncImCamSaveBMP					ImCamSaveBMP;
extern PFuncImCamSaveTIF					ImCamSaveTIF;
extern PFuncImCamSaveJPG					ImCamSaveJPG;
extern PFuncImCamStartAVICapture			ImCamStartAVICapture;	
extern PFuncImCamStopAVICapture				ImCamStopAVICapture;
extern PFuncImCamStartWMVCapture			ImCamStartWMVCapture;
extern PFuncImCamStopWMVCapture				ImCamStopWMVCapture;

extern PFuncImCamIsROISupport				ImCamIsROISupport;
extern PFuncImCamEnableROIMode				ImCamEnableROIMode;
extern PFuncImCamDisableROIMode				ImCamDisableROIMode;
extern PFuncImCamGetROISize					ImCamGetROISize;
extern PFuncImCamGetMaxArea					ImCamGetMaxArea;
extern PFuncImCamGetCurrentArea				ImCamGetCurrentArea;
extern PFuncImCamSetCurrentArea				ImCamSetCurrentArea;
extern PFuncImCamSetBytePerPacket			ImCamSetBytePerPacket;

extern PFuncImCamSetMirror					ImCamSetMirror;
extern PFuncImCamGetMirror					ImCamGetMirror;
extern PFuncImCamSetFlip					ImCamSetFlip;
extern PFuncImCamGetFlip					ImCamGetFlip;
extern PFuncImCamSetNegative				ImCamSetNegative;
extern PFuncImCamGetNegative				ImCamGetNegative;

extern PFuncImCamGetTriggerMode				ImCamGetTriggerMode;
extern PFuncImCamSetTriggerMode				ImCamSetTriggerMode;
extern PFuncImCamGetTriggerInfo				ImCamGetTriggerInfo;
extern PFuncImCamGetTriggerDelay			ImCamGetTriggerDelay;
// Removed from version 3.0.0.0
//extern PFuncImCamSetTriggerDelayUnit		ImCamSetTriggerDelayUnit;
extern PFuncImCamSetTriggerDelay			ImCamSetTriggerDelay;
extern PFuncImCamGenerateSWTrigger			ImCamGenerateSWTrigger;

extern PFuncImCamGetMultiShot				ImCamGetMultiShot;
extern PFuncImCamSetMultiShotMode			ImCamSetMultiShotMode;
extern PFuncImCamSetMultiShot				ImCamSetMultiShot;

extern PFuncImCamGetStrobe					ImCamGetStrobe;
// Removed from version 3.0.0.0
//extern PFuncImCamSetStrobeUnit				ImCamSetStrobeUnit;
extern PFuncImCamSetStrobe					ImCamSetStrobe;

extern PFuncImCamGetMemoryChannel			ImCamGetMemoryChannel;
extern PFuncImCamSetMemoryChannel			ImCamSetMemoryChannel;
extern PFuncImCamSetDefaultMemoryChannel	ImCamSetDefaultMemoryChannel;

extern PFuncImCamReadPIO					ImCamReadPIO;
extern PFuncImCamWritePIO					ImCamWritePIO;

extern PFuncImCamGetSIO						ImCamGetSIO;
extern PFuncImCamSetSIO						ImCamSetSIO;
extern PFuncImCamReadSIO					ImCamReadSIO;
extern PFuncImCamWriteSIO					ImCamWriteSIO;

extern PFuncImCamReadQuadlet				ImCamReadQuadlet;
extern PFuncImCamReadBlock					ImCamReadBlock;
extern PFuncImCamWriteQuadlet				ImCamWriteQuadlet;
extern PFuncImCamWriteBlock					ImCamWriteBlock;
extern PFuncImCamWriteBroadcast				ImCamWriteBroadcast;

extern PFuncImCamShowFilterDlg				ImCamShowFilterDlg;
extern PFuncImCamSetDataBits				ImCamSetDataBits;
extern PFuncImCamSetBayerConvert			ImCamSetBayerConvert;
extern PFuncImCamGetBayerConvert			ImCamGetBayerConvert;
// Removed from version 3.0.0.0
/*
extern PFuncImCamSetBayerConvertGain		ImCamSetBayerConvertGain;
extern PFuncImCamGetBayerConvertGain		ImCamGetBayerConvertGain;
extern PFuncImCamSetSWAWBOnePush			ImCamSetSWAWBOnePush;
*/

extern PFuncImCamGetKneePoint				ImCamGetKneePoint;
extern PFuncImCamSetKneePoint				ImCamSetKneePoint;
extern PFuncImCamGetLUT						ImCamGetLUT;
extern PFuncImCamSetLUT						ImCamSetLUT;

extern PFuncImCamGetFrameSave				ImCamGetFrameSave;
extern PFuncImCamSetFrameSave				ImCamSetFrameSave;


#endif	// __APIIMPORT_H__
