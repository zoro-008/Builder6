//---------------------------------------------------------------------------

#pragma hdrstop

#include "NeptuneImport.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

FPntcInit                       ntcInit          ;
FPntcUninit                     ntcUninit        ;
FPntcGetCameraCount             ntcGetCameraCount;
FPntcGetCameraInfo              ntcGetCameraInfo;
FPntcSetDeviceCheckCallback     ntcSetDeviceCheckCallback;
FPntcOpen                       ntcOpen;
FPntcClose                      ntcClose;
FPntcGetCameraType              ntcGetCameraType;
FPntcSetHeartbeatTime           ntcSetHeartbeatTime;
FPntcSetUnplugCallback          ntcSetUnplugCallback;
FPntcShowControlDialog          ntcShowControlDialog;
FPntcGetPixelFormatList         ntcGetPixelFormatList;
FPntcGetPixelFormat             ntcGetPixelFormat;
FPntcGetPixelFormatString       ntcGetPixelFormatString;
FPntcSetPixelFormat             ntcSetPixelFormat;
FPntcGetBitsPerPixel            ntcGetBitsPerPixel;
FPntcGetFrameRateList           ntcGetFrameRateList;
FPntcGetFrameRate               ntcGetFrameRate;
FPntcGetFrameRateString         ntcGetFrameRateString;
FPntcSetFrameRate               ntcSetFrameRate;
FPntcGetImageSize               ntcGetImageSize;
FPntcGetMaxImageSize            ntcGetMaxImageSize;
FPntcSetImageSize               ntcSetImageSize;
FPntcGetBayerConvert            ntcGetBayerConvert;
FPntcSetBayerConvert            ntcSetBayerConvert;
FPntcGetBayerLayout             ntcGetBayerLayout;
FPntcSetBayerLayout             ntcSetBayerLayout;
FPntcGetBytePerPacket           ntcGetBytePerPacket;
FPntcSetBytePerPacket           ntcSetBytePerPacket;
FPntcGetPacketSize              ntcGetPacketSize;
FPntcSetPacketSize              ntcSetPacketSize;
FPntcGetAcquisitionMode         ntcGetAcquisitionMode;
FPntcSetAcquisitionMode         ntcSetAcquisitionMode;
FPntcOneShot                    ntcOneShot;
FPntcMultiShot                  ntcMultiShot;
FPntcSetBufferCount             ntcSetBufferCount;
FPntcGetBufferSize              ntcGetBufferSize;
FPntcSetUserBuffer              ntcSetUserBuffer;
FPntcSetEffect                  ntcSetEffect;
FPntcGrab                       ntcGrab;
FPntcSetFrameCallback           ntcSetFrameCallback;
FPntcSetFrameDropCallback       ntcSetFrameDropCallback;
FPntcSetDisplay                 ntcSetDisplay;
FPntcSetAcquisition             ntcSetAcquisition;
FPntcGetAcquisition             ntcGetAcquisition;
FPntcSetMulticastAddress        ntcSetMulticastAddress;
FPntcGetReceiveFrameRate        ntcGetReceiveFrameRate;
FPntcGetRGBData                 ntcGetRGBData;
FPntcSaveImage                  ntcSaveImage;
FPntcStartStreamCapture         ntcStartStreamCapture;
FPntcStopStreamCapture          ntcStopStreamCapture;
FPntcGetFeature                 ntcGetFeature;
FPntcSetFeature                 ntcSetFeature;
FPntcSetShutterString           ntcSetShutterString;
FPntcGetAutoAreaControl         ntcGetAutoAreaControl;
FPntcSetAutoAreaControl         ntcSetAutoAreaControl;
FPntcSetAFControl               ntcSetAFControl;
FPntcSaveCameraParameter        ntcSaveCameraParameter;
FPntcLoadCameraParameter        ntcLoadCameraParameter;
FPntcGetTriggerInfo             ntcGetTriggerInfo;
FPntcGetTrigger                 ntcGetTrigger;
FPntcSetTrigger                 ntcSetTrigger;
FPntcSetTriggerMode14Exposure   ntcSetTriggerMode14Exposure;
FPntcGetTriggerDelay            ntcGetTriggerDelay;
FPntcSetTriggerDelay            ntcSetTriggerDelay;
FPntcRunSWTrigger               ntcRunSWTrigger;
FPntcReadTriggerTable           ntcReadTriggerTable ;
FPntcSaveTriggerTable           ntcSaveTriggerTable;
FPntcLoadTriggerTable           ntcLoadTriggerTable;
FPntcGetStrobeInfo              ntcGetStrobeInfo;
FPntcGetStrobe                  ntcGetStrobe;
FPntcSetStrobe                  ntcSetStrobe;
FPntcSetAutoIrisMode            ntcSetAutoIrisMode;
FPntcGetAutoIrisAverageFrame    ntcGetAutoIrisAverageFrame;
FPntcSetAutoIrisAverageFrame    ntcSetAutoIrisAverageFrame;
FPntcGetAutoIrisTargetValue     ntcGetAutoIrisTargetValue;
FPntcSetAutoIrisTargetValue     ntcSetAutoIrisTargetValue;
FPntcSetGPIO                    ntcSetGPIO;
FPntcGetUserSet                 ntcGetUserSet;
FPntcSetUserSet                 ntcSetUserSet;
FPntcSetDefaultUserSet          ntcSetDefaultUserSet;
FPntcSetPowerOnDefaultUserSet   ntcSetPowerOnDefaultUserSet;
FPntcGetKneeLUT                 ntcGetKneeLUT;
FPntcSetKneeLUT                 ntcSetKneeLUT;
FPntcGetUserLUT                 ntcGetUserLUT;
FPntcSetUserLUT                 ntcSetUserLUT;
FPntcGetFrameSave               ntcGetFrameSave;
FPntcSetFrameSave               ntcSetFrameSave;
FPntcSetSIO                     ntcSetSIO;
FPntcWriteSIO                   ntcWriteSIO;
FPntcReadSIO                    ntcReadSIO;
FPntcReadRegister               ntcReadRegister;
FPntcWriteRegister              ntcWriteRegister;
FPntcReadBlock                  ntcReadBlock;
FPntcWriteBlock                 ntcWriteBlock;
FPntcWriteBroadcast             ntcWriteBroadcast;
FPntcGetNodeVisibility          ntcGetNodeVisibility;
FPntcSetNodeVisibility          ntcSetNodeVisibility;
FPntcGetNodeList                ntcGetNodeList;
FPntcGetNodeListChar            ntcGetNodeListChar;
FPntcGetNodeInfo                ntcGetNodeInfo;
FPntcGetNodeInt                 ntcGetNodeInt;
FPntcSetNodeInt                 ntcSetNodeInt;
FPntcGetNodeFloat               ntcGetNodeFloat;
FPntcSetNodeFloat               ntcSetNodeFloat;
FPntcGetNodeEnum                ntcGetNodeEnum;
FPntcGetNodeEnumChar            ntcGetNodeEnumChar;
FPntcSetNodeEnum                ntcSetNodeEnum;
FPntcGetNodeString              ntcGetNodeString;
FPntcSetNodeString              ntcSetNodeString;
FPntcGetNodeBoolean             ntcGetNodeBoolean;
FPntcSetNodeBoolean             ntcSetNodeBoolean;
FPntcSetNodeCommand             ntcSetNodeCommand;
FPntcGetPackageFeature          ntcGetPackageFeature;
FPntcSetPackageFeature          ntcSetPackageFeature;


void * g_hDll = 0;

bool LoadNeptuneApi(char* szDllPathname)
{
	g_hDll = LoadLibrary(szDllPathname);
	if(g_hDll == NULL) return false;

	// initialize functions
        ntcInit                     = (FPntcInit                     )GetProcAddress(g_hDll, "ntcInit"                    ); if( ntcInit                     == NULL) { MessageBoxA(0,"ntcInit                     ","Function Error",0); return false ;}
        ntcUninit                   = (FPntcUninit                   )GetProcAddress(g_hDll, "ntcUninit"                  ); if( ntcUninit                   == NULL) { MessageBoxA(0,"ntcUninit                   ","Function Error",0); return false ;}
        ntcGetCameraCount           = (FPntcGetCameraCount           )GetProcAddress(g_hDll, "ntcGetCameraCount"          ); if( ntcGetCameraCount           == NULL) { MessageBoxA(0,"ntcGetCameraCount           ","Function Error",0); return false ;}
        ntcGetCameraInfo            = (FPntcGetCameraInfo            )GetProcAddress(g_hDll, "ntcGetCameraInfo"           ); if( ntcGetCameraInfo            == NULL) { MessageBoxA(0,"ntcGetCameraInfo            ","Function Error",0); return false ;}
        ntcSetDeviceCheckCallback   = (FPntcSetDeviceCheckCallback   )GetProcAddress(g_hDll, "ntcSetDeviceCheckCallback"  ); if( ntcSetDeviceCheckCallback   == NULL) { MessageBoxA(0,"ntcSetDeviceCheckCallback   ","Function Error",0); return false ;}
        ntcOpen                     = (FPntcOpen                     )GetProcAddress(g_hDll, "ntcOpen"                    ); if( ntcOpen                     == NULL) { MessageBoxA(0,"ntcOpen                     ","Function Error",0); return false ;}
        ntcClose                    = (FPntcClose                    )GetProcAddress(g_hDll, "ntcClose"                   ); if( ntcClose                    == NULL) { MessageBoxA(0,"ntcClose                    ","Function Error",0); return false ;}
        ntcGetCameraType            = (FPntcGetCameraType            )GetProcAddress(g_hDll, "ntcGetCameraType"           ); if( ntcGetCameraType            == NULL) { MessageBoxA(0,"ntcGetCameraType            ","Function Error",0); return false ;}
        ntcSetHeartbeatTime         = (FPntcSetHeartbeatTime         )GetProcAddress(g_hDll, "ntcSetHeartbeatTime"        ); if( ntcSetHeartbeatTime         == NULL) { MessageBoxA(0,"ntcSetHeartbeatTime         ","Function Error",0); return false ;}
        ntcSetUnplugCallback        = (FPntcSetUnplugCallback        )GetProcAddress(g_hDll, "ntcSetUnplugCallback"       ); if( ntcSetUnplugCallback        == NULL) { MessageBoxA(0,"ntcSetUnplugCallback        ","Function Error",0); return false ;}
        ntcShowControlDialog        = (FPntcShowControlDialog        )GetProcAddress(g_hDll, "ntcShowControlDialog"       ); if( ntcShowControlDialog        == NULL) { MessageBoxA(0,"ntcShowControlDialog        ","Function Error",0); return false ;}
        ntcGetPixelFormatList       = (FPntcGetPixelFormatList       )GetProcAddress(g_hDll, "ntcGetPixelFormatList"      ); if( ntcGetPixelFormatList       == NULL) { MessageBoxA(0,"ntcGetPixelFormatList       ","Function Error",0); return false ;}
        ntcGetPixelFormat           = (FPntcGetPixelFormat           )GetProcAddress(g_hDll, "ntcGetPixelFormat"          ); if( ntcGetPixelFormat           == NULL) { MessageBoxA(0,"ntcGetPixelFormat           ","Function Error",0); return false ;}
        ntcGetPixelFormatString     = (FPntcGetPixelFormatString     )GetProcAddress(g_hDll, "ntcGetPixelFormatString"    ); if( ntcGetPixelFormatString     == NULL) { MessageBoxA(0,"ntcGetPixelFormatString     ","Function Error",0); return false ;}
        ntcSetPixelFormat           = (FPntcSetPixelFormat           )GetProcAddress(g_hDll, "ntcSetPixelFormat"          ); if( ntcSetPixelFormat           == NULL) { MessageBoxA(0,"ntcSetPixelFormat           ","Function Error",0); return false ;}
        ntcGetBitsPerPixel          = (FPntcGetBitsPerPixel          )GetProcAddress(g_hDll, "ntcGetBitsPerPixel"         ); if( ntcGetBitsPerPixel          == NULL) { MessageBoxA(0,"ntcGetBitsPerPixel          ","Function Error",0); return false ;}
        ntcGetFrameRateList         = (FPntcGetFrameRateList         )GetProcAddress(g_hDll, "ntcGetFrameRateList"        ); if( ntcGetFrameRateList         == NULL) { MessageBoxA(0,"ntcGetFrameRateList         ","Function Error",0); return false ;}
        ntcGetFrameRate             = (FPntcGetFrameRate             )GetProcAddress(g_hDll, "ntcGetFrameRate"            ); if( ntcGetFrameRate             == NULL) { MessageBoxA(0,"ntcGetFrameRate             ","Function Error",0); return false ;}
        ntcGetFrameRateString       = (FPntcGetFrameRateString       )GetProcAddress(g_hDll, "ntcGetFrameRateString"      ); if( ntcGetFrameRateString       == NULL) { MessageBoxA(0,"ntcGetFrameRateString       ","Function Error",0); return false ;}
        ntcSetFrameRate             = (FPntcSetFrameRate             )GetProcAddress(g_hDll, "ntcSetFrameRate"            ); if( ntcSetFrameRate             == NULL) { MessageBoxA(0,"ntcSetFrameRate             ","Function Error",0); return false ;}
        ntcGetImageSize             = (FPntcGetImageSize             )GetProcAddress(g_hDll, "ntcGetImageSize"            ); if( ntcGetImageSize             == NULL) { MessageBoxA(0,"ntcGetImageSize             ","Function Error",0); return false ;}
        ntcGetMaxImageSize          = (FPntcGetMaxImageSize          )GetProcAddress(g_hDll, "ntcGetMaxImageSize"         ); if( ntcGetMaxImageSize          == NULL) { MessageBoxA(0,"ntcGetMaxImageSize          ","Function Error",0); return false ;}
        ntcSetImageSize             = (FPntcSetImageSize             )GetProcAddress(g_hDll, "ntcSetImageSize"            ); if( ntcSetImageSize             == NULL) { MessageBoxA(0,"ntcSetImageSize             ","Function Error",0); return false ;}
        ntcGetBayerConvert          = (FPntcGetBayerConvert          )GetProcAddress(g_hDll, "ntcGetBayerConvert"         ); if( ntcGetBayerConvert          == NULL) { MessageBoxA(0,"ntcGetBayerConvert          ","Function Error",0); return false ;}
        ntcSetBayerConvert          = (FPntcSetBayerConvert          )GetProcAddress(g_hDll, "ntcSetBayerConvert"         ); if( ntcSetBayerConvert          == NULL) { MessageBoxA(0,"ntcSetBayerConvert          ","Function Error",0); return false ;}
        ntcGetBayerLayout           = (FPntcGetBayerLayout           )GetProcAddress(g_hDll, "ntcGetBayerLayout"          ); if( ntcGetBayerLayout           == NULL) { MessageBoxA(0,"ntcGetBayerLayout           ","Function Error",0); return false ;}
        ntcSetBayerLayout           = (FPntcSetBayerLayout           )GetProcAddress(g_hDll, "ntcSetBayerLayout"          ); if( ntcSetBayerLayout           == NULL) { MessageBoxA(0,"ntcSetBayerLayout           ","Function Error",0); return false ;}
        ntcGetBytePerPacket         = (FPntcGetBytePerPacket         )GetProcAddress(g_hDll, "ntcGetBytePerPacket"        ); if( ntcGetBytePerPacket         == NULL) { MessageBoxA(0,"ntcGetBytePerPacket         ","Function Error",0); return false ;}
        ntcSetBytePerPacket         = (FPntcSetBytePerPacket         )GetProcAddress(g_hDll, "ntcSetBytePerPacket"        ); if( ntcSetBytePerPacket         == NULL) { MessageBoxA(0,"ntcSetBytePerPacket         ","Function Error",0); return false ;}
        ntcGetPacketSize            = (FPntcGetPacketSize            )GetProcAddress(g_hDll, "ntcGetPacketSize"           ); if( ntcGetPacketSize            == NULL) { MessageBoxA(0,"ntcGetPacketSize            ","Function Error",0); return false ;}
        ntcSetPacketSize            = (FPntcSetPacketSize            )GetProcAddress(g_hDll, "ntcSetPacketSize"           ); if( ntcSetPacketSize            == NULL) { MessageBoxA(0,"ntcSetPacketSize            ","Function Error",0); return false ;}
        ntcGetAcquisitionMode       = (FPntcGetAcquisitionMode       )GetProcAddress(g_hDll, "ntcGetAcquisitionMode"      ); if( ntcGetAcquisitionMode       == NULL) { MessageBoxA(0,"ntcGetAcquisitionMode       ","Function Error",0); return false ;}
        ntcSetAcquisitionMode       = (FPntcSetAcquisitionMode       )GetProcAddress(g_hDll, "ntcSetAcquisitionMode"      ); if( ntcSetAcquisitionMode       == NULL) { MessageBoxA(0,"ntcSetAcquisitionMode       ","Function Error",0); return false ;}
        ntcOneShot                  = (FPntcOneShot                  )GetProcAddress(g_hDll, "ntcOneShot"                 ); if( ntcOneShot                  == NULL) { MessageBoxA(0,"ntcOneShot                  ","Function Error",0); return false ;}
        ntcMultiShot                = (FPntcMultiShot                )GetProcAddress(g_hDll, "ntcMultiShot"               ); if( ntcMultiShot                == NULL) { MessageBoxA(0,"ntcMultiShot                ","Function Error",0); return false ;}
        ntcSetBufferCount           = (FPntcSetBufferCount           )GetProcAddress(g_hDll, "ntcSetBufferCount"          ); if( ntcSetBufferCount           == NULL) { MessageBoxA(0,"ntcSetBufferCount           ","Function Error",0); return false ;}
        ntcGetBufferSize            = (FPntcGetBufferSize            )GetProcAddress(g_hDll, "ntcGetBufferSize"           ); if( ntcGetBufferSize            == NULL) { MessageBoxA(0,"ntcGetBufferSize            ","Function Error",0); return false ;}
        ntcSetUserBuffer            = (FPntcSetUserBuffer            )GetProcAddress(g_hDll, "ntcSetUserBuffer"           ); if( ntcSetUserBuffer            == NULL) { MessageBoxA(0,"ntcSetUserBuffer            ","Function Error",0); return false ;}
        ntcSetEffect                = (FPntcSetEffect                )GetProcAddress(g_hDll, "ntcSetEffect"               ); if( ntcSetEffect                == NULL) { MessageBoxA(0,"ntcSetEffect                ","Function Error",0); return false ;}
        ntcGrab                     = (FPntcGrab                     )GetProcAddress(g_hDll, "ntcGrab"                    ); if( ntcGrab                     == NULL) { MessageBoxA(0,"ntcGrab                     ","Function Error",0); return false ;}
        ntcSetFrameCallback         = (FPntcSetFrameCallback         )GetProcAddress(g_hDll, "ntcSetFrameCallback"        ); if( ntcSetFrameCallback         == NULL) { MessageBoxA(0,"ntcSetFrameCallback         ","Function Error",0); return false ;}
        ntcSetFrameDropCallback     = (FPntcSetFrameDropCallback     )GetProcAddress(g_hDll, "ntcSetFrameDropCallback"    ); if( ntcSetFrameDropCallback     == NULL) { MessageBoxA(0,"ntcSetFrameDropCallback     ","Function Error",0); return false ;}
        ntcSetDisplay               = (FPntcSetDisplay               )GetProcAddress(g_hDll, "ntcSetDisplay"              ); if( ntcSetDisplay               == NULL) { MessageBoxA(0,"ntcSetDisplay               ","Function Error",0); return false ;}
        ntcSetAcquisition           = (FPntcSetAcquisition           )GetProcAddress(g_hDll, "ntcSetAcquisition"          ); if( ntcSetAcquisition           == NULL) { MessageBoxA(0,"ntcSetAcquisition           ","Function Error",0); return false ;}
        ntcGetAcquisition           = (FPntcGetAcquisition           )GetProcAddress(g_hDll, "ntcGetAcquisition"          ); if( ntcGetAcquisition           == NULL) { MessageBoxA(0,"ntcGetAcquisition           ","Function Error",0); return false ;}
        ntcSetMulticastAddress      = (FPntcSetMulticastAddress      )GetProcAddress(g_hDll, "ntcSetMulticastAddress"     ); if( ntcSetMulticastAddress      == NULL) { MessageBoxA(0,"ntcSetMulticastAddress      ","Function Error",0); return false ;}
        ntcGetReceiveFrameRate      = (FPntcGetReceiveFrameRate      )GetProcAddress(g_hDll, "ntcGetReceiveFrameRate"     ); if( ntcGetReceiveFrameRate      == NULL) { MessageBoxA(0,"ntcGetReceiveFrameRate      ","Function Error",0); return false ;}
        ntcGetRGBData               = (FPntcGetRGBData               )GetProcAddress(g_hDll, "ntcGetRGBData"              ); if( ntcGetRGBData               == NULL) { MessageBoxA(0,"ntcGetRGBData               ","Function Error",0); return false ;}
        ntcSaveImage                = (FPntcSaveImage                )GetProcAddress(g_hDll, "ntcSaveImage"               ); if( ntcSaveImage                == NULL) { MessageBoxA(0,"ntcSaveImage                ","Function Error",0); return false ;}
        ntcStartStreamCapture       = (FPntcStartStreamCapture       )GetProcAddress(g_hDll, "ntcStartStreamCapture"      ); if( ntcStartStreamCapture       == NULL) { MessageBoxA(0,"ntcStartStreamCapture       ","Function Error",0); return false ;}
        ntcStopStreamCapture        = (FPntcStopStreamCapture        )GetProcAddress(g_hDll, "ntcStopStreamCapture"       ); if( ntcStopStreamCapture        == NULL) { MessageBoxA(0,"ntcStopStreamCapture        ","Function Error",0); return false ;}
        ntcGetFeature               = (FPntcGetFeature               )GetProcAddress(g_hDll, "ntcGetFeature"              ); if( ntcGetFeature               == NULL) { MessageBoxA(0,"ntcGetFeature               ","Function Error",0); return false ;}
        ntcSetFeature               = (FPntcSetFeature               )GetProcAddress(g_hDll, "ntcSetFeature"              ); if( ntcSetFeature               == NULL) { MessageBoxA(0,"ntcSetFeature               ","Function Error",0); return false ;}
        ntcSetShutterString         = (FPntcSetShutterString         )GetProcAddress(g_hDll, "ntcSetShutterString"        ); if( ntcSetShutterString         == NULL) { MessageBoxA(0,"ntcSetShutterString         ","Function Error",0); return false ;}
        ntcGetAutoAreaControl       = (FPntcGetAutoAreaControl       )GetProcAddress(g_hDll, "ntcGetAutoAreaControl"      ); if( ntcGetAutoAreaControl       == NULL) { MessageBoxA(0,"ntcGetAutoAreaControl       ","Function Error",0); return false ;}
        ntcSetAutoAreaControl       = (FPntcSetAutoAreaControl       )GetProcAddress(g_hDll, "ntcSetAutoAreaControl"      ); if( ntcSetAutoAreaControl       == NULL) { MessageBoxA(0,"ntcSetAutoAreaControl       ","Function Error",0); return false ;}
        ntcSetAFControl             = (FPntcSetAFControl             )GetProcAddress(g_hDll, "ntcSetAFControl"            ); if( ntcSetAFControl             == NULL) { MessageBoxA(0,"ntcSetAFControl             ","Function Error",0); return false ;}
        ntcSaveCameraParameter      = (FPntcSaveCameraParameter      )GetProcAddress(g_hDll, "ntcSaveCameraParameter"     ); if( ntcSaveCameraParameter      == NULL) { MessageBoxA(0,"ntcSaveCameraParameter      ","Function Error",0); return false ;}
        ntcLoadCameraParameter      = (FPntcLoadCameraParameter      )GetProcAddress(g_hDll, "ntcLoadCameraParameter"     ); if( ntcLoadCameraParameter      == NULL) { MessageBoxA(0,"ntcLoadCameraParameter      ","Function Error",0); return false ;}
        ntcGetTriggerInfo           = (FPntcGetTriggerInfo           )GetProcAddress(g_hDll, "ntcGetTriggerInfo"          ); if( ntcGetTriggerInfo           == NULL) { MessageBoxA(0,"ntcGetTriggerInfo           ","Function Error",0); return false ;}
        ntcGetTrigger               = (FPntcGetTrigger               )GetProcAddress(g_hDll, "ntcGetTrigger"              ); if( ntcGetTrigger               == NULL) { MessageBoxA(0,"ntcGetTrigger               ","Function Error",0); return false ;}
        ntcSetTrigger               = (FPntcSetTrigger               )GetProcAddress(g_hDll, "ntcSetTrigger"              ); if( ntcSetTrigger               == NULL) { MessageBoxA(0,"ntcSetTrigger               ","Function Error",0); return false ;}
        ntcSetTriggerMode14Exposure = (FPntcSetTriggerMode14Exposure )GetProcAddress(g_hDll, "ntcSetTriggerMode14Exposure"); if( ntcSetTriggerMode14Exposure == NULL) { MessageBoxA(0,"ntcSetTriggerMode14Exposure ","Function Error",0); return false ;}
        ntcGetTriggerDelay          = (FPntcGetTriggerDelay          )GetProcAddress(g_hDll, "ntcGetTriggerDelay"         ); if( ntcGetTriggerDelay          == NULL) { MessageBoxA(0,"ntcGetTriggerDelay          ","Function Error",0); return false ;}
        ntcSetTriggerDelay          = (FPntcSetTriggerDelay          )GetProcAddress(g_hDll, "ntcSetTriggerDelay"         ); if( ntcSetTriggerDelay          == NULL) { MessageBoxA(0,"ntcSetTriggerDelay          ","Function Error",0); return false ;}
        ntcRunSWTrigger             = (FPntcRunSWTrigger             )GetProcAddress(g_hDll, "ntcRunSWTrigger"            ); if( ntcRunSWTrigger             == NULL) { MessageBoxA(0,"ntcRunSWTrigger             ","Function Error",0); return false ;}
        ntcReadTriggerTable         = (FPntcReadTriggerTable         )GetProcAddress(g_hDll, "ntcReadTriggerTable"        ); if( ntcReadTriggerTable         == NULL) { MessageBoxA(0,"ntcReadTriggerTable         ","Function Error",0); return false ;}
        ntcSaveTriggerTable         = (FPntcSaveTriggerTable         )GetProcAddress(g_hDll, "ntcSaveTriggerTable"        ); if( ntcSaveTriggerTable         == NULL) { MessageBoxA(0,"ntcSaveTriggerTable         ","Function Error",0); return false ;}
        ntcLoadTriggerTable         = (FPntcLoadTriggerTable         )GetProcAddress(g_hDll, "ntcLoadTriggerTable"        ); if( ntcLoadTriggerTable         == NULL) { MessageBoxA(0,"ntcLoadTriggerTable         ","Function Error",0); return false ;}
        ntcGetStrobeInfo            = (FPntcGetStrobeInfo            )GetProcAddress(g_hDll, "ntcGetStrobeInfo"           ); if( ntcGetStrobeInfo            == NULL) { MessageBoxA(0,"ntcGetStrobeInfo            ","Function Error",0); return false ;}
        ntcGetStrobe                = (FPntcGetStrobe                )GetProcAddress(g_hDll, "ntcGetStrobe"               ); if( ntcGetStrobe                == NULL) { MessageBoxA(0,"ntcGetStrobe                ","Function Error",0); return false ;}
        ntcSetStrobe                = (FPntcSetStrobe                )GetProcAddress(g_hDll, "ntcSetStrobe"               ); if( ntcSetStrobe                == NULL) { MessageBoxA(0,"ntcSetStrobe                ","Function Error",0); return false ;}
        ntcSetAutoIrisMode          = (FPntcSetAutoIrisMode          )GetProcAddress(g_hDll, "ntcSetAutoIrisMode"         ); if( ntcSetAutoIrisMode          == NULL) { MessageBoxA(0,"ntcSetAutoIrisMode          ","Function Error",0); return false ;}
        ntcGetAutoIrisAverageFrame  = (FPntcGetAutoIrisAverageFrame  )GetProcAddress(g_hDll, "ntcGetAutoIrisAverageFrame" ); if( ntcGetAutoIrisAverageFrame  == NULL) { MessageBoxA(0,"ntcGetAutoIrisAverageFrame  ","Function Error",0); return false ;}
        ntcSetAutoIrisAverageFrame  = (FPntcSetAutoIrisAverageFrame  )GetProcAddress(g_hDll, "ntcSetAutoIrisAverageFrame" ); if( ntcSetAutoIrisAverageFrame  == NULL) { MessageBoxA(0,"ntcSetAutoIrisAverageFrame  ","Function Error",0); return false ;}
        ntcGetAutoIrisTargetValue   = (FPntcGetAutoIrisTargetValue   )GetProcAddress(g_hDll, "ntcGetAutoIrisTargetValue"  ); if( ntcGetAutoIrisTargetValue   == NULL) { MessageBoxA(0,"ntcGetAutoIrisTargetValue   ","Function Error",0); return false ;}
        ntcSetAutoIrisTargetValue   = (FPntcSetAutoIrisTargetValue   )GetProcAddress(g_hDll, "ntcSetAutoIrisTargetValue"  ); if( ntcSetAutoIrisTargetValue   == NULL) { MessageBoxA(0,"ntcSetAutoIrisTargetValue   ","Function Error",0); return false ;}
        ntcSetGPIO                  = (FPntcSetGPIO                  )GetProcAddress(g_hDll, "ntcSetGPIO"                 ); if( ntcSetGPIO                  == NULL) { MessageBoxA(0,"ntcSetGPIO                  ","Function Error",0); return false ;}
        ntcGetUserSet               = (FPntcGetUserSet               )GetProcAddress(g_hDll, "ntcGetUserSet"              ); if( ntcGetUserSet               == NULL) { MessageBoxA(0,"ntcGetUserSet               ","Function Error",0); return false ;}
        ntcSetUserSet               = (FPntcSetUserSet               )GetProcAddress(g_hDll, "ntcSetUserSet"              ); if( ntcSetUserSet               == NULL) { MessageBoxA(0,"ntcSetUserSet               ","Function Error",0); return false ;}
        ntcSetDefaultUserSet        = (FPntcSetDefaultUserSet        )GetProcAddress(g_hDll, "ntcSetDefaultUserSet"       ); if( ntcSetDefaultUserSet        == NULL) { MessageBoxA(0,"ntcSetDefaultUserSet        ","Function Error",0); return false ;}
        ntcSetPowerOnDefaultUserSet = (FPntcSetPowerOnDefaultUserSet )GetProcAddress(g_hDll, "ntcSetPowerOnDefaultUserSet"); if( ntcSetPowerOnDefaultUserSet == NULL) { MessageBoxA(0,"ntcSetPowerOnDefaultUserSet ","Function Error",0); return false ;}
        ntcGetKneeLUT               = (FPntcGetKneeLUT               )GetProcAddress(g_hDll, "ntcGetKneeLUT"              ); if( ntcGetKneeLUT               == NULL) { MessageBoxA(0,"ntcGetKneeLUT               ","Function Error",0); return false ;}
        ntcSetKneeLUT               = (FPntcSetKneeLUT               )GetProcAddress(g_hDll, "ntcSetKneeLUT"              ); if( ntcSetKneeLUT               == NULL) { MessageBoxA(0,"ntcSetKneeLUT               ","Function Error",0); return false ;}
        ntcGetUserLUT               = (FPntcGetUserLUT               )GetProcAddress(g_hDll, "ntcGetUserLUT"              ); if( ntcGetUserLUT               == NULL) { MessageBoxA(0,"ntcGetUserLUT               ","Function Error",0); return false ;}
        ntcSetUserLUT               = (FPntcSetUserLUT               )GetProcAddress(g_hDll, "ntcSetUserLUT"              ); if( ntcSetUserLUT               == NULL) { MessageBoxA(0,"ntcSetUserLUT               ","Function Error",0); return false ;}
        ntcGetFrameSave             = (FPntcGetFrameSave             )GetProcAddress(g_hDll, "ntcGetFrameSave"            ); if( ntcGetFrameSave             == NULL) { MessageBoxA(0,"ntcGetFrameSave             ","Function Error",0); return false ;}
        ntcSetFrameSave             = (FPntcSetFrameSave             )GetProcAddress(g_hDll, "ntcSetFrameSave"            ); if( ntcSetFrameSave             == NULL) { MessageBoxA(0,"ntcSetFrameSave             ","Function Error",0); return false ;}
        ntcSetSIO                   = (FPntcSetSIO                   )GetProcAddress(g_hDll, "ntcSetSIO"                  ); if( ntcSetSIO                   == NULL) { MessageBoxA(0,"ntcSetSIO                   ","Function Error",0); return false ;}
        ntcWriteSIO                 = (FPntcWriteSIO                 )GetProcAddress(g_hDll, "ntcWriteSIO"                ); if( ntcWriteSIO                 == NULL) { MessageBoxA(0,"ntcWriteSIO                 ","Function Error",0); return false ;}
        ntcReadSIO                  = (FPntcReadSIO                  )GetProcAddress(g_hDll, "ntcReadSIO"                 ); if( ntcReadSIO                  == NULL) { MessageBoxA(0,"ntcReadSIO                  ","Function Error",0); return false ;}
        ntcReadRegister             = (FPntcReadRegister             )GetProcAddress(g_hDll, "ntcReadRegister"            ); if( ntcReadRegister             == NULL) { MessageBoxA(0,"ntcReadRegister             ","Function Error",0); return false ;}
        ntcWriteRegister            = (FPntcWriteRegister            )GetProcAddress(g_hDll, "ntcWriteRegister"           ); if( ntcWriteRegister            == NULL) { MessageBoxA(0,"ntcWriteRegister            ","Function Error",0); return false ;}
        ntcReadBlock                = (FPntcReadBlock                )GetProcAddress(g_hDll, "ntcReadBlock"               ); if( ntcReadBlock                == NULL) { MessageBoxA(0,"ntcReadBlock                ","Function Error",0); return false ;}
        ntcWriteBlock               = (FPntcWriteBlock               )GetProcAddress(g_hDll, "ntcWriteBlock"              ); if( ntcWriteBlock               == NULL) { MessageBoxA(0,"ntcWriteBlock               ","Function Error",0); return false ;}
        ntcWriteBroadcast           = (FPntcWriteBroadcast           )GetProcAddress(g_hDll, "ntcWriteBroadcast"          ); if( ntcWriteBroadcast           == NULL) { MessageBoxA(0,"ntcWriteBroadcast           ","Function Error",0); return false ;}
        ntcGetNodeVisibility        = (FPntcGetNodeVisibility        )GetProcAddress(g_hDll, "ntcGetNodeVisibility"       ); if( ntcGetNodeVisibility        == NULL) { MessageBoxA(0,"ntcGetNodeVisibility        ","Function Error",0); return false ;}
        ntcSetNodeVisibility        = (FPntcSetNodeVisibility        )GetProcAddress(g_hDll, "ntcSetNodeVisibility"       ); if( ntcSetNodeVisibility        == NULL) { MessageBoxA(0,"ntcSetNodeVisibility        ","Function Error",0); return false ;}
        ntcGetNodeList              = (FPntcGetNodeList              )GetProcAddress(g_hDll, "ntcGetNodeList"             ); if( ntcGetNodeList              == NULL) { MessageBoxA(0,"ntcGetNodeList              ","Function Error",0); return false ;}
        ntcGetNodeListChar          = (FPntcGetNodeListChar          )GetProcAddress(g_hDll, "ntcGetNodeListChar"         ); if( ntcGetNodeListChar          == NULL) { MessageBoxA(0,"ntcGetNodeListChar          ","Function Error",0); return false ;}
        ntcGetNodeInfo              = (FPntcGetNodeInfo              )GetProcAddress(g_hDll, "ntcGetNodeInfo"             ); if( ntcGetNodeInfo              == NULL) { MessageBoxA(0,"ntcGetNodeInfo              ","Function Error",0); return false ;}
        ntcGetNodeInt               = (FPntcGetNodeInt               )GetProcAddress(g_hDll, "ntcGetNodeInt"              ); if( ntcGetNodeInt               == NULL) { MessageBoxA(0,"ntcGetNodeInt               ","Function Error",0); return false ;}
        ntcSetNodeInt               = (FPntcSetNodeInt               )GetProcAddress(g_hDll, "ntcSetNodeInt"              ); if( ntcSetNodeInt               == NULL) { MessageBoxA(0,"ntcSetNodeInt               ","Function Error",0); return false ;}
        ntcGetNodeFloat             = (FPntcGetNodeFloat             )GetProcAddress(g_hDll, "ntcGetNodeFloat"            ); if( ntcGetNodeFloat             == NULL) { MessageBoxA(0,"ntcGetNodeFloat             ","Function Error",0); return false ;}
        ntcSetNodeFloat             = (FPntcSetNodeFloat             )GetProcAddress(g_hDll, "ntcSetNodeFloat"            ); if( ntcSetNodeFloat             == NULL) { MessageBoxA(0,"ntcSetNodeFloat             ","Function Error",0); return false ;}
        ntcGetNodeEnum              = (FPntcGetNodeEnum              )GetProcAddress(g_hDll, "ntcGetNodeEnum"             ); if( ntcGetNodeEnum              == NULL) { MessageBoxA(0,"ntcGetNodeEnum              ","Function Error",0); return false ;}
        ntcGetNodeEnumChar          = (FPntcGetNodeEnumChar          )GetProcAddress(g_hDll, "ntcGetNodeEnumChar"         ); if( ntcGetNodeEnumChar          == NULL) { MessageBoxA(0,"ntcGetNodeEnumChar          ","Function Error",0); return false ;}
        ntcSetNodeEnum              = (FPntcSetNodeEnum              )GetProcAddress(g_hDll, "ntcSetNodeEnum"             ); if( ntcSetNodeEnum              == NULL) { MessageBoxA(0,"ntcSetNodeEnum              ","Function Error",0); return false ;}
        ntcGetNodeString            = (FPntcGetNodeString            )GetProcAddress(g_hDll, "ntcGetNodeString"           ); if( ntcGetNodeString            == NULL) { MessageBoxA(0,"ntcGetNodeString            ","Function Error",0); return false ;}
        ntcSetNodeString            = (FPntcSetNodeString            )GetProcAddress(g_hDll, "ntcSetNodeString"           ); if( ntcSetNodeString            == NULL) { MessageBoxA(0,"ntcSetNodeString            ","Function Error",0); return false ;}
        ntcGetNodeBoolean           = (FPntcGetNodeBoolean           )GetProcAddress(g_hDll, "ntcGetNodeBoolean"          ); if( ntcGetNodeBoolean           == NULL) { MessageBoxA(0,"ntcGetNodeBoolean           ","Function Error",0); return false ;}
        ntcSetNodeBoolean           = (FPntcSetNodeBoolean           )GetProcAddress(g_hDll, "ntcSetNodeBoolean"          ); if( ntcSetNodeBoolean           == NULL) { MessageBoxA(0,"ntcSetNodeBoolean           ","Function Error",0); return false ;}
        ntcSetNodeCommand           = (FPntcSetNodeCommand           )GetProcAddress(g_hDll, "ntcSetNodeCommand"          ); if( ntcSetNodeCommand           == NULL) { MessageBoxA(0,"ntcSetNodeCommand           ","Function Error",0); return false ;}
        //ntcGetPackageFeature        = (FPntcGetPackageFeature        )GetProcAddress(g_hDll, "ntcGetPackageFeature"       ); if( ntcGetPackageFeature        == NULL) { MessageBoxA(0,"ntcGetPackageFeature        ","Function Error",0); return false ;}
        //ntcSetPackageFeature        = (FPntcSetPackageFeature        )GetProcAddress(g_hDll, "ntcSetPackageFeature"       ); if( ntcSetPackageFeature        == NULL) { MessageBoxA(0,"ntcSetPackageFeature        ","Function Error",0); return false ;}


	return true;
}


void UnloadNeptuneApi()
{
	if(g_hDll == NULL) return ;

	FreeLibrary(g_hDll);
	g_hDll = NULL;
}
