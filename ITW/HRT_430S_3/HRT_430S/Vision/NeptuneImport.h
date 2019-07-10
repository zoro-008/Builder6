//---------------------------------------------------------------------------

#ifndef NeptuneImportH
#define NeptuneImportH

#include <windows.h>
//#include "NeptuneC_API.h"
#include "NeptuneDef.h"
//---------------------------------------------------------------------------
//Visual Studio에서는 enum이 4바이트로 되는듯.
//Visual C에서 총 sizeof(NEPTUNE_FEATURE) == 36
//BC6에서는 총 총 sizeof(NEPTUNE_FEATURE) == 24        NEPTUNE_FEATURE
struct NEPTUNE_FEATURE_BC6 {
	_int32_t/*ENeptuneBoolean*/			bSupport;
	_int32_t/*ENeptuneBoolean*/			bOnOff;				// on/off state, on/off control(SnowNoiseRemove only)
	_uint8_t				SupportAutoModes;	// bit flag for support(bit0:Off, bit1:Once, bit2:Continuous)
	_int32_t/*ENeptuneAutoMode*/		AutoMode;			// current Auto mode, valid only when "SuporeAutoMode != 0"
	_int32_t				Min;				// minimum value
	_int32_t				Max;				// maximum value
	_int32_t				Inc;				// increment step
	_int32_t				Value;				// current value
	_int32_t/*ENeptuneNodeAccessMode*/	ValueAccessMode;	// access state of the value

	NEPTUNE_FEATURE_BC6()
	{
		bOnOff = NEPTUNE_BOOL_TRUE;
		bSupport = NEPTUNE_BOOL_FALSE;
		SupportAutoModes = 0;
		AutoMode = NEPTUNE_AUTO_OFF;
		Min = 0;		// minimum value
		Max = 0;		// maximum value
		Inc = 0;		// increment step
		Value = 0;		// current value
		ValueAccessMode = NEPTUNE_NODE_ACCESSMODE_NA;
	}
};
typedef NEPTUNE_FEATURE_BC6*			PNEPTUNE_FEATURE_BC6;


//Visual Studio에서는 enum이 4바이트로 되는듯.
//Visual C에서 총 sizeof(NEPTUNE_USERSET) == 12
//BC6에서는 총 총 sizeof(NEPTUNE_USERSET) == 4
//                        BC6        VC
//_uint16_t               2byte      4
//ENeptuneUserSet         1byte      4
//ENeptuneUserSetCommand  1byte      4
typedef struct _tagNEPTUNE_USERSET_BC6 {
	_uint32_t                SupportUserSet;		// bit flag for supported user set, 0 is "Default"
	_uint32_t		 UserSetIndex;		// user set index to save or load
	_uint32_t        	 Command;			// save or load

	_tagNEPTUNE_USERSET_BC6()
	{
		SupportUserSet = 0;
		UserSetIndex = (_uint32_t)NEPTUNE_USERSET_DEFAULT;
		Command = (_uint32_t)NEPTUNE_USERSET_CMD_LOAD;
	}
} NEPTUNE_USERSET_BC6, *PNEPTUNE_USERSET_BC6;
















//typedef bool (__stdcall *LPFuncImCamInit)();                                                                                                                                                                                                             
//
typedef ENeptuneError (__stdcall *FPntcInit          )(                    );                                                                                                             extern FPntcInit           ntcInit          ;                    
typedef ENeptuneError (__stdcall *FPntcUninit        )(                    );                                                                                                             extern FPntcUninit         ntcUninit        ;                    
typedef ENeptuneError (__stdcall *FPntcGetCameraCount)(_uint32_t* pnNumbers);                                                                                                             extern FPntcGetCameraCount ntcGetCameraCount;                    
typedef ENeptuneError (__stdcall *FPntcGetCameraInfo         )(PNEPTUNE_CAM_INFO pInfo, _uint32_t nLength);                                                                               extern FPntcGetCameraInfo ntcGetCameraInfo;                      
typedef ENeptuneError (__stdcall *FPntcSetDeviceCheckCallback)(NeptuneCDevCheckCallback DeviceCallback, void* pContext);                                                                  extern FPntcSetDeviceCheckCallback ntcSetDeviceCheckCallback;    
typedef ENeptuneError (__stdcall *FPntcOpen                  )(_char_t* pstrDevID, NeptuneCamHandle* phCamHandle, ENeptuneDevAccess eAccessFlag=NEPTUNE_DEV_ACCESS_EXCLUSIVE);            extern FPntcOpen ntcOpen;                                        
typedef ENeptuneError (__stdcall *FPntcClose)(NeptuneCamHandle hCamHandle);                                                                                                               extern FPntcClose ntcClose;                                      
typedef ENeptuneError (__stdcall *FPntcGetCameraType)(NeptuneCamHandle hCamHandle, ENeptuneDevType* peType);                                                                              extern FPntcGetCameraType ntcGetCameraType;                      
typedef ENeptuneError (__stdcall *FPntcSetHeartbeatTime)(NeptuneCamHandle hCamHandle, _ulong32_t nTime);                                                                                  extern FPntcSetHeartbeatTime ntcSetHeartbeatTime;                
typedef ENeptuneError (__stdcall *FPntcSetUnplugCallback)(NeptuneCamHandle hCamHandle, NeptuneCUnplugCallback UnplugCallback, void* pContext);                                            extern FPntcSetUnplugCallback ntcSetUnplugCallback;              
typedef ENeptuneError (__stdcall *FPntcShowControlDialog)(NeptuneCamHandle hCamHandle);                                                                                                   extern FPntcShowControlDialog ntcShowControlDialog;              
typedef ENeptuneError (__stdcall *FPntcGetPixelFormatList)(NeptuneCamHandle hCamHandle, ENeptunePixelFormat* peList, _uint32_t* pnSize);                                                  extern FPntcGetPixelFormatList ntcGetPixelFormatList;            
typedef ENeptuneError (__stdcall *FPntcGetPixelFormat)(NeptuneCamHandle hCamHandle, ENeptunePixelFormat* peFormat);                                                                       extern FPntcGetPixelFormat ntcGetPixelFormat;                    
typedef ENeptuneError (__stdcall *FPntcGetPixelFormatString)(NeptuneCamHandle hCamHandle, const ENeptunePixelFormat eFormat, _char_t* pStr, _uint32_t nSize);                             extern FPntcGetPixelFormatString ntcGetPixelFormatString;        
typedef ENeptuneError (__stdcall *FPntcSetPixelFormat)(NeptuneCamHandle hCamHandle, const ENeptunePixelFormat eFormat);                                                                   extern FPntcSetPixelFormat ntcSetPixelFormat;                    
typedef ENeptuneError (__stdcall *FPntcGetBitsPerPixel)(NeptuneCamHandle hCamHandle, _uint32_t* pnBits);                                                                                  extern FPntcGetBitsPerPixel ntcGetBitsPerPixel;                  
typedef ENeptuneError (__stdcall *FPntcGetFrameRateList)(NeptuneCamHandle hCamHandle, ENeptuneFrameRate* peList, _uint32_t* pnSize);                                                      extern FPntcGetFrameRateList ntcGetFrameRateList;                
typedef ENeptuneError (__stdcall *FPntcGetFrameRate)(NeptuneCamHandle hCamHandle, ENeptuneFrameRate* peRate, _double_t* pfValue);                                                         extern FPntcGetFrameRate ntcGetFrameRate;                        
typedef ENeptuneError (__stdcall *FPntcGetFrameRateString)(NeptuneCamHandle hCamHandle, const ENeptuneFrameRate eRate, _char_t* pStr, _uint32_t nSize);                                   extern FPntcGetFrameRateString ntcGetFrameRateString;            
typedef ENeptuneError (__stdcall *FPntcSetFrameRate)(NeptuneCamHandle hCamHandle, ENeptuneFrameRate eRate, _double_t fValue);                                                             extern FPntcSetFrameRate ntcSetFrameRate;                        
typedef ENeptuneError (__stdcall *FPntcGetImageSize)(NeptuneCamHandle hCamHandle, PNEPTUNE_IMAGE_SIZE pImageSize);                                                                        extern FPntcGetImageSize ntcGetImageSize;                        
typedef ENeptuneError (__stdcall *FPntcGetMaxImageSize)(NeptuneCamHandle hCamHandle, PNEPTUNE_IMAGE_SIZE pImageSize);                                                                     extern FPntcGetMaxImageSize ntcGetMaxImageSize;                  
typedef ENeptuneError (__stdcall *FPntcSetImageSize)(NeptuneCamHandle hCamHandle, NEPTUNE_IMAGE_SIZE ImageSize);                                                                          extern FPntcSetImageSize ntcSetImageSize;                        
typedef ENeptuneError (__stdcall *FPntcGetBayerConvert)(NeptuneCamHandle hCamHandle, ENeptuneBayerMethod* peMethod);                                                                      extern FPntcGetBayerConvert ntcGetBayerConvert;                  
typedef ENeptuneError (__stdcall *FPntcSetBayerConvert)(NeptuneCamHandle hCamHandle, ENeptuneBayerMethod eMethod);                                                                        extern FPntcSetBayerConvert ntcSetBayerConvert;                  
typedef ENeptuneError (__stdcall *FPntcGetBayerLayout)(NeptuneCamHandle hCamHandle, ENeptuneBayerLayout* peLayout);                                                                       extern FPntcGetBayerLayout ntcGetBayerLayout;                    
typedef ENeptuneError (__stdcall *FPntcSetBayerLayout)(NeptuneCamHandle hCamHandle, ENeptuneBayerLayout ePattern);                                                                        extern FPntcSetBayerLayout ntcSetBayerLayout;                    
typedef ENeptuneError (__stdcall *FPntcGetBytePerPacket)(NeptuneCamHandle hCamHandle, _uint32_t* pnBpp, _uint32_t* pnMin=NULL, _uint32_t* pnMax=NULL);                                    extern FPntcGetBytePerPacket ntcGetBytePerPacket;                
typedef ENeptuneError (__stdcall *FPntcSetBytePerPacket)(NeptuneCamHandle hCamHandle, _uint32_t nBpp);                                                                                    extern FPntcSetBytePerPacket ntcSetBytePerPacket;                
typedef ENeptuneError (__stdcall *FPntcGetPacketSize)(NeptuneCamHandle hCamHandle, _uint32_t* pnPacketSize, _uint32_t* pnMin=NULL, _uint32_t* pnMax=NULL);                                extern FPntcGetPacketSize ntcGetPacketSize;                      
typedef ENeptuneError (__stdcall *FPntcSetPacketSize)(NeptuneCamHandle hCamHandle, _uint32_t nPacketSize);                                                                                extern FPntcSetPacketSize ntcSetPacketSize;                      
typedef ENeptuneError (__stdcall *FPntcGetAcquisitionMode)(NeptuneCamHandle hCamHandle, ENeptuneAcquisitionMode* peMode, _uint32_t* pnFrames);                                            extern FPntcGetAcquisitionMode ntcGetAcquisitionMode;            
typedef ENeptuneError (__stdcall *FPntcSetAcquisitionMode)(NeptuneCamHandle hCamHandle, ENeptuneAcquisitionMode eMode, _uint32_t nFrames);                                              extern FPntcSetAcquisitionMode ntcSetAcquisitionMode;
typedef ENeptuneError (__stdcall *FPntcOneShot)(NeptuneCamHandle hCamHandle);                                                                                                             extern FPntcOneShot ntcOneShot;                                  
typedef ENeptuneError (__stdcall *FPntcMultiShot)(NeptuneCamHandle hCamHandle);                                                                                                           extern FPntcMultiShot ntcMultiShot;                              
typedef ENeptuneError (__stdcall *FPntcSetBufferCount)(NeptuneCamHandle hCamHandle, _uint32_t nCount);                                                                                    extern FPntcSetBufferCount ntcSetBufferCount;                    
typedef ENeptuneError (__stdcall *FPntcGetBufferSize)(NeptuneCamHandle hCamHandle, _uint32_t* pnSize);                                                                                    extern FPntcGetBufferSize ntcGetBufferSize;
typedef ENeptuneError (__stdcall *FPntcSetUserBuffer)(NeptuneCamHandle hCamHandle, _void_t* pBuffer, _uint32_t nSize, _uint32_t nCount);                                                  extern FPntcSetUserBuffer ntcSetUserBuffer;                      
typedef ENeptuneError (__stdcall *FPntcSetEffect)(NeptuneCamHandle hCamHandle);                                                                                                           extern FPntcSetEffect ntcSetEffect;                              
typedef ENeptuneError (__stdcall *FPntcGrab)(NeptuneCamHandle hCamHandle, PNEPTUNE_IMAGE pImage, ENeptuneGrabFormat eGrabFormat, _uint32_t nTimeOut);                                     extern FPntcGrab ntcGrab;
typedef ENeptuneError (__stdcall *FPntcSetFrameCallback)(NeptuneCamHandle hCamHandle, NeptuneCFrameCallback callback, void* pContext=NULL);                                               extern FPntcSetFrameCallback ntcSetFrameCallback;                
typedef ENeptuneError (__stdcall *FPntcSetFrameDropCallback)(NeptuneCamHandle hCamHandle, NeptuneCFrameDropCallback callback, void* pContext=NULL);                                       extern FPntcSetFrameDropCallback ntcSetFrameDropCallback;        
typedef ENeptuneError (__stdcall *FPntcSetDisplay)(NeptuneCamHandle hCamHandle, HWND hWnd);                                                                                               extern FPntcSetDisplay ntcSetDisplay;                            
typedef ENeptuneError (__stdcall *FPntcSetAcquisition)(NeptuneCamHandle hCamHandle, ENeptuneBoolean eState);                                                                              extern FPntcSetAcquisition ntcSetAcquisition;                    
typedef ENeptuneError (__stdcall *FPntcGetAcquisition)(NeptuneCamHandle hCamHandle, ENeptuneBoolean* peState);                                                                            extern FPntcGetAcquisition ntcGetAcquisition;                    
typedef ENeptuneError (__stdcall *FPntcSetMulticastAddress)(NeptuneCamHandle hCamHandle, _char_t* pstrAddress);                                                                           extern FPntcSetMulticastAddress ntcSetMulticastAddress;
typedef ENeptuneError (__stdcall *FPntcGetReceiveFrameRate)(NeptuneCamHandle hCamHandle, _float32_t* pfRate);                                                                             extern FPntcGetReceiveFrameRate ntcGetReceiveFrameRate;          
typedef ENeptuneError (__stdcall *FPntcGetRGBData)(NeptuneCamHandle hCamHandle, _uchar_t* pBuffer, _uint32_t nSize);                                                                      extern FPntcGetRGBData ntcGetRGBData;                            
typedef ENeptuneError (__stdcall *FPntcSaveImage)(NeptuneCamHandle hCamHandle, _char_t* strFileName, _uint32_t nQuality);                                                                 extern FPntcSaveImage ntcSaveImage;
typedef ENeptuneError (__stdcall *FPntcStartStreamCapture)(NeptuneCamHandle hCamHandle, _char_t* strFileName, ENeptuneBoolean eCompress, _uint32_t nBitrate);                             extern FPntcStartStreamCapture ntcStartStreamCapture;
typedef ENeptuneError (__stdcall *FPntcStopStreamCapture)(NeptuneCamHandle hCamHandle);                                                                                                   extern FPntcStopStreamCapture ntcStopStreamCapture;
typedef ENeptuneError (__stdcall *FPntcGetFeature)(NeptuneCamHandle hCamHandle, ENeptuneFeature eFeature, PNEPTUNE_FEATURE_BC6 pInfo);                                                        extern FPntcGetFeature ntcGetFeature;
typedef ENeptuneError (__stdcall *FPntcSetFeature)(NeptuneCamHandle hCamHandle, ENeptuneFeature eFeature, NEPTUNE_FEATURE_BC6 Info);                                                          extern FPntcSetFeature ntcSetFeature;
typedef ENeptuneError (__stdcall *FPntcSetShutterString)(NeptuneCamHandle hCamHandle, _char_t* pstrShutter);                                                                              extern FPntcSetShutterString ntcSetShutterString;                
typedef ENeptuneError (__stdcall *FPntcGetAutoAreaControl)(NeptuneCamHandle hCamHandle, ENeptuneAutoAreaSelect eSelect, PNEPTUNE_AUTOAREA pArea);                                         extern FPntcGetAutoAreaControl ntcGetAutoAreaControl;            
typedef ENeptuneError (__stdcall *FPntcSetAutoAreaControl)(NeptuneCamHandle hCamHandle, ENeptuneAutoAreaSelect eSelect, NEPTUNE_AUTOAREA Area);                                           extern FPntcSetAutoAreaControl ntcSetAutoAreaControl;            
typedef ENeptuneError (__stdcall *FPntcSetAFControl)(NeptuneCamHandle hCamHandle, ENeptuneAFMode eControlMode);                                                                           extern FPntcSetAFControl ntcSetAFControl;                        
typedef ENeptuneError (__stdcall *FPntcSaveCameraParameter)(NeptuneCamHandle hCamHandle, _char_t* pstrFileName);                                                                          extern FPntcSaveCameraParameter ntcSaveCameraParameter;          
typedef ENeptuneError (__stdcall *FPntcLoadCameraParameter)(NeptuneCamHandle hCamHandle, _char_t* pstrFileName);                                                                          extern FPntcLoadCameraParameter ntcLoadCameraParameter;          
typedef ENeptuneError (__stdcall *FPntcGetTriggerInfo)(NeptuneCamHandle hCamHandle, PNEPTUNE_TRIGGER_INFO pTriggerInfo);                                                                  extern FPntcGetTriggerInfo ntcGetTriggerInfo;                    
typedef ENeptuneError (__stdcall *FPntcGetTrigger)(NeptuneCamHandle hCamHandle, PNEPTUNE_TRIGGER pTrigger);                                                                               extern FPntcGetTrigger ntcGetTrigger;                            
typedef ENeptuneError (__stdcall *FPntcSetTrigger)(NeptuneCamHandle hCamHandle, NEPTUNE_TRIGGER Trigger);                                                                                 extern FPntcSetTrigger ntcSetTrigger;                            
typedef ENeptuneError (__stdcall *FPntcSetTriggerMode14Exposure)(NeptuneCamHandle hCamHandle, _uint32_t nExposure, _uint32_t nInterval);                                                  extern FPntcSetTriggerMode14Exposure ntcSetTriggerMode14Exposure;
typedef ENeptuneError (__stdcall *FPntcGetTriggerDelay)(NeptuneCamHandle hCamHandle, _uint32_t* pnDelay, _uint32_t* pnMin=NULL, _uint32_t* pnMax=NULL);                                   extern FPntcGetTriggerDelay ntcGetTriggerDelay;                  
typedef ENeptuneError (__stdcall *FPntcSetTriggerDelay)(NeptuneCamHandle hCamHandle, _uint32_t nDelay);                                                                                   extern FPntcSetTriggerDelay ntcSetTriggerDelay;                  
typedef ENeptuneError (__stdcall *FPntcRunSWTrigger)(NeptuneCamHandle hCamHandle);                                                                                                        extern FPntcRunSWTrigger ntcRunSWTrigger;                        
typedef ENeptuneError (__stdcall *FPntcReadTriggerTable)(NeptuneCamHandle hCamHandle, PNEPTUNE_TRIGGER_TABLE pTriggerTable);                                                              extern FPntcReadTriggerTable ntcReadTriggerTable ;               
typedef ENeptuneError (__stdcall *FPntcSaveTriggerTable)(NeptuneCamHandle hCamHandle, NEPTUNE_TRIGGER_TABLE TriggerTable);                                                                extern FPntcSaveTriggerTable ntcSaveTriggerTable;                
typedef ENeptuneError (__stdcall *FPntcLoadTriggerTable)(NeptuneCamHandle hCamHandle, _uint32_t nIndex);                                                                                  extern FPntcLoadTriggerTable ntcLoadTriggerTable;                
typedef ENeptuneError (__stdcall *FPntcGetStrobeInfo)(NeptuneCamHandle hCamHandle, PNEPTUNE_STROBE_INFO pStrobeInfo);                                                                     extern FPntcGetStrobeInfo ntcGetStrobeInfo;                      
typedef ENeptuneError (__stdcall *FPntcGetStrobe)(NeptuneCamHandle hCamHandle, PNEPTUNE_STROBE pStrobe);                                                                                  extern FPntcGetStrobe ntcGetStrobe;                              
typedef ENeptuneError (__stdcall *FPntcSetStrobe)(NeptuneCamHandle hCamHandle, NEPTUNE_STROBE Strobe);                                                                                    extern FPntcSetStrobe ntcSetStrobe;                              
typedef ENeptuneError (__stdcall *FPntcSetAutoIrisMode)(NeptuneCamHandle hCamHandle, ENeptuneAutoIrisMode eMode);                                                                         extern FPntcSetAutoIrisMode ntcSetAutoIrisMode;                  
typedef ENeptuneError (__stdcall *FPntcGetAutoIrisAverageFrame)(NeptuneCamHandle hCamHandle, _uint32_t* pnValue, _uint32_t* pnMin=NULL, _uint32_t* pnMax=NULL);                           extern FPntcGetAutoIrisAverageFrame ntcGetAutoIrisAverageFrame;  
typedef ENeptuneError (__stdcall *FPntcSetAutoIrisAverageFrame)(NeptuneCamHandle hCamHandle, _uint32_t nValue);                                                                           extern FPntcSetAutoIrisAverageFrame ntcSetAutoIrisAverageFrame;  
typedef ENeptuneError (__stdcall *FPntcGetAutoIrisTargetValue)(NeptuneCamHandle hCamHandle, _uint32_t* pnValue, _uint32_t* pnMin=NULL, _uint32_t* pnMax=NULL);                            extern FPntcGetAutoIrisTargetValue ntcGetAutoIrisTargetValue;    
typedef ENeptuneError (__stdcall *FPntcSetAutoIrisTargetValue)(NeptuneCamHandle hCamHandle, _uint32_t nValue);                                                                            extern FPntcSetAutoIrisTargetValue ntcSetAutoIrisTargetValue;    
typedef ENeptuneError (__stdcall *FPntcSetGPIO)(NeptuneCamHandle hCamHandle, NEPTUNE_GPIO Gpio);                                                                                          extern FPntcSetGPIO ntcSetGPIO;                                  
typedef ENeptuneError (__stdcall *FPntcGetUserSet)(NeptuneCamHandle hCamHandle, PNEPTUNE_USERSET_BC6 pUserSet);                                                                               extern FPntcGetUserSet ntcGetUserSet;
typedef ENeptuneError (__stdcall *FPntcSetUserSet)(NeptuneCamHandle hCamHandle, NEPTUNE_USERSET_BC6 UserSet);                                                                                 extern FPntcSetUserSet ntcSetUserSet;
typedef ENeptuneError (__stdcall *FPntcSetDefaultUserSet)(NeptuneCamHandle hCamHandle, ENeptuneUserSet eUserSet);                                                                         extern FPntcSetDefaultUserSet ntcSetDefaultUserSet;              
typedef ENeptuneError (__stdcall *FPntcSetPowerOnDefaultUserSet)(NeptuneCamHandle hCamHandle, ENeptuneUserSet eUserSet);                                                                  extern FPntcSetPowerOnDefaultUserSet ntcSetPowerOnDefaultUserSet;
typedef ENeptuneError (__stdcall *FPntcGetKneeLUT)(NeptuneCamHandle hCamHandle, PNEPTUNE_KNEE_LUT pLUTPoints);                                                                            extern FPntcGetKneeLUT ntcGetKneeLUT;                            
typedef ENeptuneError (__stdcall *FPntcSetKneeLUT)(NeptuneCamHandle hCamHandle, NEPTUNE_KNEE_LUT LUTPoints);                                                                              extern FPntcSetKneeLUT ntcSetKneeLUT;                            
typedef ENeptuneError (__stdcall *FPntcGetUserLUT)(NeptuneCamHandle hCamHandle, PNEPTUNE_USER_LUT pLUTData);                                                                              extern FPntcGetUserLUT ntcGetUserLUT;                            
typedef ENeptuneError (__stdcall *FPntcSetUserLUT)(NeptuneCamHandle hCamHandle, NEPTUNE_USER_LUT LUTData);                                                                                extern FPntcSetUserLUT PntcSetUserLUT;                           
typedef ENeptuneError (__stdcall *FPntcGetFrameSave)(NeptuneCamHandle hCamHandle, ENeptuneBoolean* peOnOff, _uint32_t* pnFrameRemained);                                                  extern FPntcGetFrameSave ntcGetFrameSave;                        
typedef ENeptuneError (__stdcall *FPntcSetFrameSave)(NeptuneCamHandle hCamHandle, ENeptuneBoolean eOnOff, ENeptuneBoolean eTransfer, _uint32_t nFrames);                                  extern FPntcSetFrameSave ntcSetFrameSave;                        
typedef ENeptuneError (__stdcall *FPntcSetSIO)(NeptuneCamHandle hCamHandle,NEPTUNE_SIO_PROPERTY Property);                                                                                extern FPntcSetSIO ntcSetSIO;                                    
typedef ENeptuneError (__stdcall *FPntcWriteSIO)(NeptuneCamHandle hCamHandle, NEPTUNE_SIO Data);                                                                                          extern FPntcWriteSIO ntcWriteSIO;                                
typedef ENeptuneError (__stdcall *FPntcReadSIO)(NeptuneCamHandle hCamHandle, PNEPTUNE_SIO pData);                                                                                         extern FPntcReadSIO ntcReadSIO;                                  
typedef ENeptuneError (__stdcall *FPntcReadRegister)(NeptuneCamHandle hCamHandle, _ulong32_t ulAddr, _ulong32_t* pulVal);                                                                 extern FPntcReadRegister ntcReadRegister;                        
typedef ENeptuneError (__stdcall *FPntcWriteRegister)(NeptuneCamHandle hCamHandle, _ulong32_t ulAddr, _ulong32_t ulVal);                                                                  extern FPntcWriteRegister ntcWriteRegister;                      
typedef ENeptuneError (__stdcall *FPntcReadBlock)(NeptuneCamHandle hCamHandle, _ulong32_t ulAddr, _uint8_t* pBuf, _ulong32_t* pnSize);                                                    extern FPntcReadBlock ntcReadBlock;                              
typedef ENeptuneError (__stdcall *FPntcWriteBlock)(NeptuneCamHandle hCamHandle, _ulong32_t ulAddr, _uint8_t* pBuf, _ulong32_t nSize);                                                     extern FPntcWriteBlock ntcWriteBlock;                            
typedef ENeptuneError (__stdcall *FPntcWriteBroadcast)(NeptuneCamHandle hCamHandle, _ulong32_t ulAddr, _ulong32_t ulVal);                                                                 extern FPntcWriteBroadcast ntcWriteBroadcast;                    
typedef ENeptuneError (__stdcall *FPntcGetNodeVisibility)(NeptuneCamHandle hCamHandle, ENeptuneNodeVisibility* peVisibility);                                                             extern FPntcGetNodeVisibility ntcGetNodeVisibility;              
typedef ENeptuneError (__stdcall *FPntcSetNodeVisibility)(NeptuneCamHandle hCamHandle, ENeptuneNodeVisibility eVisibility);                                                               extern FPntcSetNodeVisibility ntcSetNodeVisibility;              
typedef ENeptuneError (__stdcall *FPntcGetNodeList)(NeptuneCamHandle hCamHandle, const _char_t* pstrCategory, PNEPTUNE_XML_NODE_LIST pList);                                              extern FPntcGetNodeList ntcGetNodeList;                          
typedef ENeptuneError (__stdcall *FPntcGetNodeListChar)(NeptuneCamHandle hCamHandle, const _char_t* pstrCategory, _char_t* pstrList, _uint32_t nSize, _uint32_t* pnCount);                extern FPntcGetNodeListChar ntcGetNodeListChar;                  
typedef ENeptuneError (__stdcall *FPntcGetNodeInfo)(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, PNEPTUNE_XML_NODE_INFO pInfo);                                                  extern FPntcGetNodeInfo ntcGetNodeInfo;                          
typedef ENeptuneError (__stdcall *FPntcGetNodeInt)(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, PNEPTUNE_XML_INT_VALUE_INFO pValueInfo);                                         extern FPntcGetNodeInt ntcGetNodeInt;                            
typedef ENeptuneError (__stdcall *FPntcSetNodeInt)(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, _int64_t nValue);                                                                extern FPntcSetNodeInt ntcSetNodeInt;                            
typedef ENeptuneError (__stdcall *FPntcGetNodeFloat)(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, PNEPTUNE_XML_FLOAT_VALUE_INFO pValueInfo);                                     extern FPntcGetNodeFloat ntcGetNodeFloat;                        
typedef ENeptuneError (__stdcall *FPntcSetNodeFloat)(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, _double_t dValue);                                                             extern FPntcSetNodeFloat ntcSetNodeFloat;                        
typedef ENeptuneError (__stdcall *FPntcGetNodeEnum)(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, PNEPTUNE_XML_ENUM_LIST pList);                                                  extern FPntcGetNodeEnum ntcGetNodeEnum;                          
typedef ENeptuneError (__stdcall *FPntcGetNodeEnumChar)(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, PNEPTUNE_XML_ENUM_LIST pList);                                              extern FPntcGetNodeEnumChar ntcGetNodeEnumChar;                  
typedef ENeptuneError (__stdcall *FPntcSetNodeEnum)(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, const _char_t* pstrEnum);                                                       extern FPntcSetNodeEnum ntcSetNodeEnum;                          
typedef ENeptuneError (__stdcall *FPntcGetNodeString)(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, _char_t* pstrString, _uint32_t* pnSize, ENeptuneNodeAccessMode* peAccessMode);extern FPntcGetNodeString ntcGetNodeString;                      
typedef ENeptuneError (__stdcall *FPntcSetNodeString)(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, const _char_t* pstrString);                                                   extern FPntcSetNodeString ntcSetNodeString;                      
typedef ENeptuneError (__stdcall *FPntcGetNodeBoolean)(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, const _char_t* pstrString);                                                  extern FPntcGetNodeBoolean ntcGetNodeBoolean;                    
typedef ENeptuneError (__stdcall *FPntcSetNodeBoolean)(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, ENeptuneBoolean eState);                                                     extern FPntcSetNodeBoolean ntcSetNodeBoolean;                    
typedef ENeptuneError (__stdcall *FPntcSetNodeCommand)(NeptuneCamHandle hCamHandle, const _char_t* pstrNode);                                                                             extern FPntcSetNodeCommand ntcSetNodeCommand;                    
typedef ENeptuneError (__stdcall *FPntcGetPackageFeature)(NeptuneCamHandle hCamHandle, PNEPTUNE_PACKAGE_FEATURE pPackageFeature);                                                         extern FPntcGetPackageFeature ntcGetPackageFeature;              
typedef ENeptuneError (__stdcall *FPntcSetPackageFeature)(NeptuneCamHandle hCamHandle, NEPTUNE_PACKAGE_FEATURE PackageFeature);                                                           extern FPntcGetPackageFeature ntcGetPackageFeature;              
                                                                                                                                                                                                                                                           
                                                                                                                                                                                                                                                           

/*
// 114 EA
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcInit();
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcUninit();
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetCameraCount(_uint32_t* pnNumbers);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetCameraInfo(PNEPTUNE_CAM_INFO pInfo, _uint32_t nLength);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetDeviceCheckCallback(NeptuneCDevCheckCallback DeviceCallback, void* pContext);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcOpen(_char_t* pstrDevID, NeptuneCamHandle* phCamHandle, ENeptuneDevAccess eAccessFlag=NEPTUNE_DEV_ACCESS_EXCLUSIVE);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcClose(NeptuneCamHandle hCamHandle);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetCameraType(NeptuneCamHandle hCamHandle, ENeptuneDevType* peType);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetHeartbeatTime(NeptuneCamHandle hCamHandle, _ulong32_t nTime);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetUnplugCallback(NeptuneCamHandle hCamHandle, NeptuneCUnplugCallback UnplugCallback, void* pContext);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcShowControlDialog(NeptuneCamHandle hCamHandle);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetPixelFormatList(NeptuneCamHandle hCamHandle, ENeptunePixelFormat* peList, _uint32_t* pnSize);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetPixelFormat(NeptuneCamHandle hCamHandle, ENeptunePixelFormat* peFormat);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetPixelFormatString(NeptuneCamHandle hCamHandle, const ENeptunePixelFormat eFormat, _char_t* pStr, _uint32_t nSize);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetPixelFormat(NeptuneCamHandle hCamHandle, const ENeptunePixelFormat eFormat);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetBitsPerPixel(NeptuneCamHandle hCamHandle, _uint32_t* pnBits);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetFrameRateList(NeptuneCamHandle hCamHandle, ENeptuneFrameRate* peList, _uint32_t* pnSize);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetFrameRate(NeptuneCamHandle hCamHandle, ENeptuneFrameRate* peRate, _double_t* pfValue);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetFrameRateString(NeptuneCamHandle hCamHandle, const ENeptuneFrameRate eRate, _char_t* pStr, _uint32_t nSize);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetFrameRate(NeptuneCamHandle hCamHandle, ENeptuneFrameRate eRate, _double_t fValue);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetImageSize(NeptuneCamHandle hCamHandle, PNEPTUNE_IMAGE_SIZE pImageSize);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetMaxImageSize(NeptuneCamHandle hCamHandle, PNEPTUNE_IMAGE_SIZE pImageSize);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetImageSize(NeptuneCamHandle hCamHandle, NEPTUNE_IMAGE_SIZE ImageSize);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetBayerConvert(NeptuneCamHandle hCamHandle, ENeptuneBayerMethod* peMethod);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetBayerConvert(NeptuneCamHandle hCamHandle, ENeptuneBayerMethod eMethod);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetBayerLayout(NeptuneCamHandle hCamHandle, ENeptuneBayerLayout* peLayout);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetBayerLayout(NeptuneCamHandle hCamHandle, ENeptuneBayerLayout ePattern);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetBytePerPacket(NeptuneCamHandle hCamHandle, _uint32_t* pnBpp, _uint32_t* pnMin=NULL, _uint32_t* pnMax=NULL);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetBytePerPacket(NeptuneCamHandle hCamHandle, _uint32_t nBpp);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetPacketSize(NeptuneCamHandle hCamHandle, _uint32_t* pnPacketSize, _uint32_t* pnMin=NULL, _uint32_t* pnMax=NULL);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetPacketSize(NeptuneCamHandle hCamHandle, _uint32_t nPacketSize);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetAcquisitionMode(NeptuneCamHandle hCamHandle, ENeptuneAcquisitionMode* peMode, _uint32_t* pnFrames);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetAcquisitionMode(NeptuneCamHandle hCamHandle, ENeptuneAcquisitionMode eMode, _uint32_t nFrames=2);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcOneShot(NeptuneCamHandle hCamHandle);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcMultiShot(NeptuneCamHandle hCamHandle);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetBufferCount(NeptuneCamHandle hCamHandle, _uint32_t nCount);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetBufferSize(NeptuneCamHandle hCamHandle, _uint32_t* pnSize);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetUserBuffer(NeptuneCamHandle hCamHandle, _void_t* pBuffer, _uint32_t nSize, _uint32_t nCount);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetEffect(NeptuneCamHandle hCamHandle, _int32_t nEffect);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGrab(NeptuneCamHandle hCamHandle, PNEPTUNE_IMAGE pImage, ENeptuneGrabFormat eGrabFormat, _uint32_t nTimeOut=1000);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetFrameCallback(NeptuneCamHandle hCamHandle, NeptuneCFrameCallback callback, void* pContext=NULL);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetFrameDropCallback(NeptuneCamHandle hCamHandle, NeptuneCFrameDropCallback callback, void* pContext=NULL);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetDisplay(NeptuneCamHandle hCamHandle, HWND hWnd);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetAcquisition(NeptuneCamHandle hCamHandle, ENeptuneBoolean eState);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetAcquisition(NeptuneCamHandle hCamHandle, ENeptuneBoolean* peState);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetMulticastAddress(NeptuneCamHandle hCamHandle, _char_t* pstrAddress);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetReceiveFrameRate(NeptuneCamHandle hCamHandle, _float32_t* pfRate);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetRGBData(NeptuneCamHandle hCamHandle, _uchar_t* pBuffer, _uint32_t nSize);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSaveImage(NeptuneCamHandle hCamHandle, _char_t* strFileName, _uint32_t nQuality=80);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcStartStreamCapture(NeptuneCamHandle hCamHandle, _char_t* strFileName, ENeptuneBoolean eCompress, _uint32_t nBitrate=1000);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcStopStreamCapture(NeptuneCamHandle hCamHandle);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetFeature(NeptuneCamHandle hCamHandle, ENeptuneFeature eFeature, PNEPTUNE_FEATURE pInfo);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetFeature(NeptuneCamHandle hCamHandle, ENeptuneFeature eFeature, NEPTUNE_FEATURE Info);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetShutterString(NeptuneCamHandle hCamHandle, _char_t* pstrShutter);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetAutoAreaControl(NeptuneCamHandle hCamHandle, ENeptuneAutoAreaSelect eSelect, PNEPTUNE_AUTOAREA pArea);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetAutoAreaControl(NeptuneCamHandle hCamHandle, ENeptuneAutoAreaSelect eSelect, NEPTUNE_AUTOAREA Area);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetAFControl(NeptuneCamHandle hCamHandle, ENeptuneAFMode eControlMode);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSaveCameraParameter(NeptuneCamHandle hCamHandle, _char_t* pstrFileName);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcLoadCameraParameter(NeptuneCamHandle hCamHandle, _char_t* pstrFileName);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetTriggerInfo(NeptuneCamHandle hCamHandle, PNEPTUNE_TRIGGER_INFO pTriggerInfo);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetTrigger(NeptuneCamHandle hCamHandle, PNEPTUNE_TRIGGER pTrigger);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetTrigger(NeptuneCamHandle hCamHandle, NEPTUNE_TRIGGER Trigger);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetTriggerMode14Exposure(NeptuneCamHandle hCamHandle, _uint32_t nExposure, _uint32_t nInterval);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetTriggerDelay(NeptuneCamHandle hCamHandle, _uint32_t* pnDelay, _uint32_t* pnMin=NULL, _uint32_t* pnMax=NULL);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetTriggerDelay(NeptuneCamHandle hCamHandle, _uint32_t nDelay);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcRunSWTrigger(NeptuneCamHandle hCamHandle);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcReadTriggerTable(NeptuneCamHandle hCamHandle, PNEPTUNE_TRIGGER_TABLE pTriggerTable);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSaveTriggerTable(NeptuneCamHandle hCamHandle, NEPTUNE_TRIGGER_TABLE TriggerTable);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcLoadTriggerTable(NeptuneCamHandle hCamHandle, _uint32_t nIndex);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetStrobeInfo(NeptuneCamHandle hCamHandle, PNEPTUNE_STROBE_INFO pStrobeInfo);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetStrobe(NeptuneCamHandle hCamHandle, PNEPTUNE_STROBE pStrobe);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetStrobe(NeptuneCamHandle hCamHandle, NEPTUNE_STROBE Strobe);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetAutoIrisMode(NeptuneCamHandle hCamHandle, ENeptuneAutoIrisMode eMode);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetAutoIrisAverageFrame(NeptuneCamHandle hCamHandle, _uint32_t* pnValue, _uint32_t* pnMin=NULL, _uint32_t* pnMax=NULL);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetAutoIrisAverageFrame(NeptuneCamHandle hCamHandle, _uint32_t nValue);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetAutoIrisTargetValue(NeptuneCamHandle hCamHandle, _uint32_t* pnValue, _uint32_t* pnMin=NULL, _uint32_t* pnMax=NULL);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetAutoIrisTargetValue(NeptuneCamHandle hCamHandle, _uint32_t nValue);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetGPIO(NeptuneCamHandle hCamHandle, NEPTUNE_GPIO Gpio);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetUserSet(NeptuneCamHandle hCamHandle, PNEPTUNE_USERSET pUserSet);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetUserSet(NeptuneCamHandle hCamHandle, NEPTUNE_USERSET UserSet);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetDefaultUserSet(NeptuneCamHandle hCamHandle, ENeptuneUserSet eUserSet);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetPowerOnDefaultUserSet(NeptuneCamHandle hCamHandle, ENeptuneUserSet eUserSet);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetKneeLUT(NeptuneCamHandle hCamHandle, PNEPTUNE_KNEE_LUT pLUTPoints);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetKneeLUT(NeptuneCamHandle hCamHandle, NEPTUNE_KNEE_LUT LUTPoints);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetUserLUT(NeptuneCamHandle hCamHandle, PNEPTUNE_USER_LUT pLUTData);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetUserLUT(NeptuneCamHandle hCamHandle, NEPTUNE_USER_LUT LUTData);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetFrameSave(NeptuneCamHandle hCamHandle, ENeptuneBoolean* peOnOff, _uint32_t* pnFrameRemained);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetFrameSave(NeptuneCamHandle hCamHandle, ENeptuneBoolean eOnOff, ENeptuneBoolean eTransfer, _uint32_t nFrames);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetSIO(NeptuneCamHandle hCamHandle,NEPTUNE_SIO_PROPERTY Property);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcWriteSIO(NeptuneCamHandle hCamHandle, NEPTUNE_SIO Data);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcReadSIO(NeptuneCamHandle hCamHandle, PNEPTUNE_SIO pData);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcReadRegister(NeptuneCamHandle hCamHandle, _ulong32_t ulAddr, _ulong32_t* pulVal);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcWriteRegister(NeptuneCamHandle hCamHandle, _ulong32_t ulAddr, _ulong32_t ulVal);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcReadBlock(NeptuneCamHandle hCamHandle, _ulong32_t ulAddr, _uint8_t* pBuf, _ulong32_t* pnSize);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcWriteBlock(NeptuneCamHandle hCamHandle, _ulong32_t ulAddr, _uint8_t* pBuf, _ulong32_t nSize);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcWriteBroadcast(NeptuneCamHandle hCamHandle, _ulong32_t ulAddr, _ulong32_t ulVal);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetNodeVisibility(NeptuneCamHandle hCamHandle, ENeptuneNodeVisibility* peVisibility);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetNodeVisibility(NeptuneCamHandle hCamHandle, ENeptuneNodeVisibility eVisibility);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetNodeList(NeptuneCamHandle hCamHandle, const _char_t* pstrCategory, PNEPTUNE_XML_NODE_LIST pList);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetNodeListChar(NeptuneCamHandle hCamHandle, const _char_t* pstrCategory, _char_t* pstrList, _uint32_t nSize, _uint32_t* pnCount);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetNodeInfo(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, PNEPTUNE_XML_NODE_INFO pInfo);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetNodeInt(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, PNEPTUNE_XML_INT_VALUE_INFO pValueInfo);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetNodeInt(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, _int64_t nValue);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetNodeFloat(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, PNEPTUNE_XML_FLOAT_VALUE_INFO pValueInfo);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetNodeFloat(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, _double_t dValue);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetNodeEnum(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, PNEPTUNE_XML_ENUM_LIST pList);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetNodeEnumChar(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, _char_t* pstrList, _uint32_t nSize, _uint32_t* pnCount, _uint32_t* pnIndex);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetNodeEnum(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, const _char_t* pstrEnum);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetNodeString(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, _char_t* pstrString, _uint32_t* pnSize, ENeptuneNodeAccessMode* peAccessMode);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetNodeString(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, const _char_t* pstrString);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetNodeBoolean(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, ENeptuneBoolean* peState);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetNodeBoolean(NeptuneCamHandle hCamHandle, const _char_t* pstrNode, ENeptuneBoolean eState);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetNodeCommand(NeptuneCamHandle hCamHandle, const _char_t* pstrNode);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcGetPackageFeature(NeptuneCamHandle hCamHandle, PNEPTUNE_PACKAGE_FEATURE pPackageFeature);
NEPTUNE_C_API ENeptuneError API_CALLTYPE ntcSetPackageFeature(NeptuneCamHandle hCamHandle, NEPTUNE_PACKAGE_FEATURE PackageFeature);
*/
//dllimport ENeptuneError API_CALLTYPE ntcInit();

bool LoadNeptuneApi(char* szDllPathname);
void UnloadNeptuneApi();

#endif
