//---------------------------------------------------------------------------
#include <System.hpp>
#pragma hdrstop

#include "Confocal.h"
#include "UserIni.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>


HINSTANCE g_hConfocal;
ERR_CODE g_iErr;
CConfocal Confocal ;

String GetErrMsg(int _iErrCode)
{
    switch (_iErrCode){
        default                                       : return "";
	case ERR_NOERROR                              : return String("ERR_NOERROR                             ").Trim(); 
	case ERR_FUNCTION_NOT_SUPPORTED               : return String("ERR_FUNCTION_NOT_SUPPORTED              ").Trim();
	case ERR_CANNOT_OPEN                          : return String("ERR_CANNOT_OPEN                         ").Trim();
	case ERR_NOT_OPEN                             : return String("ERR_NOT_OPEN                            ").Trim();
	case ERR_APPLYING_PARAMS                      : return String("ERR_APPLYING_PARAMS                     ").Trim();
	case ERR_SEND_CMD_TO_SENSOR                   : return String("ERR_SEND_CMD_TO_SENSOR                  ").Trim();
	case ERR_CLEARUNG_BUFFER                      : return String("ERR_CLEARUNG_BUFFER                     ").Trim();
	case ERR_HW_COMMUNICATION                     : return String("ERR_HW_COMMUNICATION                    ").Trim();
	case ERR_TIMEOUT_READING_FROM_SENSOR          : return String("ERR_TIMEOUT_READING_FROM_SENSOR         ").Trim();
	case ERR_READING_SENSOR_DATA                  : return String("ERR_READING_SENSOR_DATA                 ").Trim();
	case ERR_INTERFACE_NOT_SUPPORTED              : return String("ERR_INTERFACE_NOT_SUPPORTED             ").Trim();
	case ERR_ALREADY_OPEN                         : return String("ERR_ALREADY_OPEN                        ").Trim();
	case ERR_CANNOT_CREATE_INTERFACE              : return String("ERR_CANNOT_CREATE_INTERFACE             ").Trim();
	case ERR_NO_SENSORDATA_AVAILABLE              : return String("ERR_NO_SENSORDATA_AVAILABLE             ").Trim();
	case ERR_UNKNOWN_SENSOR_COMMAND               : return String("ERR_UNKNOWN_SENSOR_COMMAND              ").Trim();
	case ERR_UNKNOWN_SENSOR_ANSWER                : return String("ERR_UNKNOWN_SENSOR_ANSWER               ").Trim();
	case ERR_SENSOR_ANSWER_ERROR                  : return String("ERR_SENSOR_ANSWER_ERROR                 ").Trim();
	case ERR_SENSOR_ANSWER_TOO_SHORT              : return String("ERR_SENSOR_ANSWER_TOO_SHORT             ").Trim();
	case ERR_WRONG_PARAMETER                      : return String("ERR_WRONG_PARAMETER                     ").Trim();
	case ERR_NOMEMORY                             : return String("ERR_NOMEMORY                            ").Trim();
	case ERR_NO_ANSWER_RECEIVED                   : return String("ERR_NO_ANSWER_RECEIVED                  ").Trim();
	case ERR_SENSOR_ANSWER_DOES_NOT_MATCH_COMMAND : return String("ERR_SENSOR_ANSWER_DOES_NOT_MATCH_COMMAND").Trim();
	case ERR_BAUDRATE_TOO_LOW                     : return String("ERR_BAUDRATE_TOO_LOW                    ").Trim();
	case ERR_OVERFLOW                             : return String("ERR_OVERFLOW                            ").Trim();
	case ERR_INSTANCE_NOT_EXIST                   : return String("ERR_INSTANCE_NOT_EXIST                  ").Trim();
	case ERR_NOT_FOUND                            : return String("ERR_NOT_FOUND                           ").Trim();
	case ERR_WARNING                              : return String("ERR_WARNING                             ").Trim();
	case ERR_SENSOR_ANSWER_WARNING                : return String("ERR_SENSOR_ANSWER_WARNING               ").Trim();
    }


}



CConfocal::CConfocal()
{
    g_hConfocal = NULL ;

    CreateSensorInstance   = NULL ; 
    CreateSensorInstByName = NULL ;  
    ReleaseSensorInstance  = NULL ;  
    SetParameterInt        = NULL ;  
    SetParameterDWORD      = NULL ;  
    SetParameterDouble     = NULL ;  
    SetParameterString     = NULL ;  
    SetParameterStringL    = NULL ;  
    SetParameters          = NULL ;  
    GetParameterInt        = NULL ;  
    GetParameterDWORD      = NULL ;  
    GetParameterDouble     = NULL ;  
    GetParameterString     = NULL ;  
    ClearAllParameters     = NULL ;  
    OpenSensor             = NULL ;  
    CloseSensor            = NULL ;  
    SensorCommand          = NULL ;  
    DataAvail              = NULL ;  
    TransferData           = NULL ;  
    Poll                   = NULL ;  
    GetError               = NULL ;  
    GetDLLVersion          = NULL ;  
    EnableLogging          = NULL ;  
    OpenSensorRS232        = NULL ;  
    OpenSensorIF2004       = NULL ;  
    OpenSensorIF2008       = NULL ;  
    OpenSensorTCPIP        = NULL ;  
    OpenSensorDriverX_USB  = NULL ;  
    OpenSensorUSBIO        = NULL ;  
    OpenSensorWinUSB       = NULL ;  
    ExecSCmd               = NULL ;  
    SetIntExecSCmd         = NULL ;  
    SetDoubleExecSCmd      = NULL ;  
    SetStringExecSCmd      = NULL ;  
    ExecSCmdGetInt         = NULL ;  
    ExecSCmdGetDouble      = NULL ;  
    ExecSCmdGetString      = NULL ;

    if(!LoadDll()) {
        MessageBoxA(0,"콘포컬 센서 Dll로딩 실패","Error",0);//  ShowMessage();
        return ;
    }

    //m_iId= CreateSensorInstance (SENSOR_ILD2300);
    m_iId= CreateSensorInstByName("IFC2451");

    //g_iErr = SetParameterInt(m_iId, "IP_EnableLogging", 1);
    //g_iErr = SetParameterInt(iSensor, "IP_EnableLogging", 1)

    //ERR_CODE g_iErr= SetParameterString (instance,"IP_Interface", "RS232");
    //g_iErr= SetParameterString (instance, “IP_Port“, “COM1“);
    //g_iErr= OpenSensor (instance);
    //// Receiving sensor settings and getting sensor measurement range
    //g_iErr= SetParameterString (instance, “S_Command“, “Get_Settings“);
    //g_iErr= SensorCommand (instance);
    //double range;
    //g_iErr= GetParameterDouble (instance, “SA_Range“, &range);
    //// Set the sampling rate
    //g_iErr= SetParameterString (instance, “S_Command“, “Set_Speed“);
    //g_iErr= SetParameterInt (instance, “SP_Speed“, 1); // 1 means 1.0 kHz
    //g_iErr= SensorCommand (instance);




    //g_iErr = OpenSensorTCPIP(m_iId , "169.254.168.150"); //
    g_iErr = OpenSensorTCPIP(m_iId , "169.254.168.150"); //
    //int iErr = OpenSensorIF2004 (m_iId, 0, 0);
    if(g_iErr!=ERR_NOERROR){
        MessageBoxA(0,"공초점센서 오픈 함수 에러",GetErrMsg(g_iErr).c_str(),0);
    }



}
CConfocal::~CConfocal()
{
    g_iErr = CloseSensor(m_iId);
    if(g_iErr!=ERR_NOERROR){
        MessageBoxA(0,"공초점센서를 Close 할 수 없습니다.",GetErrMsg(g_iErr).c_str(),0);
    }
    g_iErr = ReleaseSensorInstance (m_iId);
    if(g_iErr!=ERR_NOERROR){
        MessageBoxA(0,"공초점센서 드라이버를 Release 할 수 없습니다.",GetErrMsg(g_iErr).c_str(),0);
    }
    FreeDll();
}

bool CConfocal::LoadDll()
{
    String sDllPath = "MEDAQLib.dll";

    g_hConfocal = LoadLibrary (sDllPath.c_str());

    if(!g_hConfocal) return false ;

    // 클래스 적재 함수 받음
    CreateSensorInstance      = (CREATESENSORINSTANCE  )GetProcAddress(g_hConfocal, TEXT("CreateSensorInstance"  )); if(!CreateSensorInstance   )return false ;
    CreateSensorInstByName    = (CREATESENSORINSTBYNAME)GetProcAddress(g_hConfocal, TEXT("CreateSensorInstByName")); if(!CreateSensorInstByName )return false ;
    ReleaseSensorInstance     = (RELEASESENSORINSTANCE )GetProcAddress(g_hConfocal, TEXT("ReleaseSensorInstance" )); if(!ReleaseSensorInstance  )return false ;
    SetParameterInt           = (SETPPARAMETERINT      )GetProcAddress(g_hConfocal, TEXT("SetParameterInt"       )); if(!SetParameterInt        )return false ;
    SetParameterDWORD         = (SETPPARAMETERDWORD    )GetProcAddress(g_hConfocal, TEXT("SetParameterDWORD"     )); if(!SetParameterDWORD      )return false ;
    SetParameterDouble        = (SETPPARAMETERDOUBLE   )GetProcAddress(g_hConfocal, TEXT("SetParameterDouble"    )); if(!SetParameterDouble     )return false ;
    SetParameterString        = (SETPPARAMETERSTRING   )GetProcAddress(g_hConfocal, TEXT("SetParameterString"    )); if(!SetParameterString     )return false ;
    SetParameterStringL       = (SETPPARAMETERSTRINGL  )GetProcAddress(g_hConfocal, TEXT("SetParameterStringL"   )); if(!SetParameterStringL    )return false ;
    SetParameters             = (SETPPARAMETERS        )GetProcAddress(g_hConfocal, TEXT("SetParameters"         )); if(!SetParameters          )return false ;
    GetParameterInt           = (GETPPARAMETERINT      )GetProcAddress(g_hConfocal, TEXT("GetParameterInt"       )); if(!GetParameterInt        )return false ;
    GetParameterDWORD         = (GETPPARAMETERDWORD    )GetProcAddress(g_hConfocal, TEXT("GetParameterDWORD"     )); if(!GetParameterDWORD      )return false ;
    GetParameterDouble        = (GETPPARAMETERDOUBLE   )GetProcAddress(g_hConfocal, TEXT("GetParameterDouble"    )); if(!GetParameterDouble     )return false ;
    GetParameterString        = (GETPPARAMETERSTRING   )GetProcAddress(g_hConfocal, TEXT("GetParameterString"    )); if(!GetParameterString     )return false ;
    ClearAllParameters        = (CLEARALLPARAMETERS    )GetProcAddress(g_hConfocal, TEXT("ClearAllParameters"    )); if(!ClearAllParameters     )return false ;
    OpenSensor                = (OPENSENSOR            )GetProcAddress(g_hConfocal, TEXT("OpenSensor"            )); if(!OpenSensor             )return false ;
    CloseSensor               = (CLOSESENSOR           )GetProcAddress(g_hConfocal, TEXT("CloseSensor"           )); if(!CloseSensor            )return false ;
    SensorCommand             = (SENSORCOMMAND         )GetProcAddress(g_hConfocal, TEXT("SensorCommand"         )); if(!SensorCommand          )return false ;
    DataAvail                 = (DATAAVAIL             )GetProcAddress(g_hConfocal, TEXT("DataAvail"             )); if(!DataAvail              )return false ;
    TransferData              = (TRANSFERDATA          )GetProcAddress(g_hConfocal, TEXT("TransferData"          )); if(!TransferData           )return false ;
    Poll                      = (POLL                  )GetProcAddress(g_hConfocal, TEXT("Poll"                  )); if(!Poll                   )return false ;
    GetError                  = (GETERROR              )GetProcAddress(g_hConfocal, TEXT("GetError"              )); if(!GetError               )return false ;
    GetDLLVersion             = (GETDLLVERSION         )GetProcAddress(g_hConfocal, TEXT("GetDLLVersion"         )); if(!GetDLLVersion          )return false ;
    EnableLogging             = (ENABLELOGGING         )GetProcAddress(g_hConfocal, TEXT("EnableLogging"         )); if(!EnableLogging          )return false ;
    OpenSensorRS232           = (OPENSENSORRS232       )GetProcAddress(g_hConfocal, TEXT("OpenSensorRS232"       )); if(!OpenSensorRS232        )return false ;
    OpenSensorIF2004          = (OPENSENSORIF2004      )GetProcAddress(g_hConfocal, TEXT("OpenSensorIF2004"      )); if(!OpenSensorIF2004       )return false ;
    OpenSensorIF2008          = (OPENSENSORIF2008      )GetProcAddress(g_hConfocal, TEXT("OpenSensorIF2008"      )); if(!OpenSensorIF2008       )return false ;
    OpenSensorTCPIP           = (OPENSENSORTCPIP       )GetProcAddress(g_hConfocal, TEXT("OpenSensorTCPIP"       )); if(!OpenSensorTCPIP        )return false ;
    OpenSensorDriverX_USB     = (OPENSENSORDRIVERX_USB )GetProcAddress(g_hConfocal, TEXT("OpenSensorDriverX_USB" )); if(!OpenSensorDriverX_USB  )return false ;
    OpenSensorUSBIO           = (OPENSENSORUSBIO       )GetProcAddress(g_hConfocal, TEXT("OpenSensorUSBIO"       )); if(!OpenSensorUSBIO        )return false ;
    OpenSensorWinUSB          = (OPENSENSORWINUSB      )GetProcAddress(g_hConfocal, TEXT("OpenSensorWinUSB"      )); if(!OpenSensorWinUSB       )return false ;
    ExecSCmd                  = (EXECSCMD              )GetProcAddress(g_hConfocal, TEXT("ExecSCmd"              )); if(!ExecSCmd               )return false ;
    SetIntExecSCmd            = (SETINTEXECSCMD        )GetProcAddress(g_hConfocal, TEXT("SetIntExecSCmd"        )); if(!SetIntExecSCmd         )return false ;
    SetDoubleExecSCmd         = (SETDOUBLEEXECSCMD     )GetProcAddress(g_hConfocal, TEXT("SetDoubleExecSCmd"     )); if(!SetDoubleExecSCmd      )return false ;
    SetStringExecSCmd         = (SETSTRINGEXECSCMD     )GetProcAddress(g_hConfocal, TEXT("SetStringExecSCmd"     )); if(!SetStringExecSCmd      )return false ;
    ExecSCmdGetInt            = (EXECSCMDGETINT        )GetProcAddress(g_hConfocal, TEXT("ExecSCmdGetInt"        )); if(!ExecSCmdGetInt         )return false ;
    ExecSCmdGetDouble         = (EXECSCMDGETDOUBLE     )GetProcAddress(g_hConfocal, TEXT("ExecSCmdGetDouble"     )); if(!ExecSCmdGetDouble      )return false ;
    ExecSCmdGetString         = (EXECSCMDGETSTRING     )GetProcAddress(g_hConfocal, TEXT("ExecSCmdGetString"     )); if(!ExecSCmdGetString      )return false ;

    return true ;
}

bool CConfocal::FreeDll()
{
    if(g_hConfocal==NULL) return false ;
    bool bRet = FreeLibrary(g_hConfocal);
    g_hConfocal=NULL;
    return bRet ;

}

double CConfocal::GetHeight()
{
    double dData ;
    g_iErr = Poll (m_iId , NULL, &dData, 1);

    if(g_iErr!=ERR_NOERROR){
        //MessageBoxA(0,"공초점센서 측정에러",GetErrMsg(g_iErr).c_str(),0);
        return CONFOCAL_MIN ; //범위 벋어나면 20이 반환됌.
    }
    if(dData > CONFOCAL_MAX) {
        dData = CONFOCAL_MAX ;
    }
    if(dData < CONFOCAL_MIN) {
        dData = CONFOCAL_MIN ;
    }

    //센서에서 멀면 값이 작아지게 반전.. 나중에 물어보고 센서에서 설정해보자.
    double dRet = CONFOCAL_MAX - dData ;
    if(dRet < 0.00001) dRet = 0.0 ;

    return dRet ;
}

//잘안됌 나중에 문의 하자.
void CConfocal::Rezero(bool _bOn)
{
    double dData ;
    int iOn = _bOn ? 1 : 0 ;
    g_iErr = SetParameterInt(m_iId , "SP_Master", iOn);
    if(g_iErr!=ERR_NOERROR){
        MessageBoxA(0,"공초점센서 리제로 온오프 에러",GetErrMsg(g_iErr).c_str(),0);
    }

    g_iErr = SetParameterDouble(m_iId , "SP_MasterValue", 0.0);
    if(g_iErr!=ERR_NOERROR){
        MessageBoxA(0,"공초점센서 리제로 마스터벨류 에러",GetErrMsg(g_iErr).c_str(),0);
    }
}


