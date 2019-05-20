//---------------------------------------------------------------------------
#include <System.hpp>
#pragma hdrstop

#include "HexaPot.h"
#include "UserIni.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

// Function Pointer Variables
FP_ConnectRS232          PI_ConnectRS232          =NULL;
FP_CloseConnection       PI_CloseConnection       =NULL;
FP_qIDN                  PI_qIDN                  =NULL;
FP_GcsCommandset         PI_GcsCommandset         =NULL;
FP_GcsGetAnswer          PI_GcsGetAnswer          =NULL;
FP_GcsGetAnswerSize      PI_GcsGetAnswerSize      =NULL;
FP_IsMoving              PI_IsMoving              =NULL;
FP_qFRF	                 PI_qFRF                  =NULL;
FP_ConnectTCPIP          PI_ConnectTCPIP          =NULL;
FP_EnumerateTCPIPDevices PI_EnumerateTCPIPDevices =NULL;
FP_FRF                   PI_FRF                   =NULL;
FP_IsControllerReady     PI_IsControllerReady     =NULL;
FP_MOV                   PI_MOV                   =NULL;
FP_MVR                   PI_MVR                   =NULL;
FP_qPOS                  PI_qPOS                  =NULL;
FP_SPI                   PI_SPI                   =NULL;//피봇 포인트 변환 함수 xyz가 0,0,0이어야한다.
FP_qCMN                  PI_qCMN                  =NULL;//리밋체크함수???
FP_qCMX                  PI_qCMX                  =NULL;
FP_GetError              PI_GetError              =NULL;//Dll에러 체크..코드 리턴한다.
FP_qERR                  PI_qERR                  =NULL;//Controller에러 체크.
FP_TranslateError        PI_TranslateError        =NULL;//에러 코드 넣고 메세지로 받아온다.
FP_qMOV                  PI_qMOV                  =NULL;//현재 타겟 포지션.
FP_SVO                   PI_SVO                   =NULL;//서보온/오프



CHexaPot HexaPot ;
HINSTANCE g_hPI_Dll=NULL;
CHexaPot::CHexaPot()
{
    m_iId = 0 ;
    m_iLastErrCode = 0 ;
    m_sLastErrMsg  = "";

    memset(m_daTrgPos , 0 , sizeof(double)*MAX_PI_AXIS);
    memset(m_daResPos , 0 , sizeof(double)*MAX_PI_AXIS);
    memset(m_daEncPos , 0 , sizeof(double)*MAX_PI_AXIS);

    if(!LoadDll()) {
        MessageBoxA(0,"헥사포트 Dll로딩 실패","Error",0);//  ShowMessage();
        return ;
    }

    //m_iId = PI_ConnectTCPIP("169.254.10.6" ,50000);
    m_iId = PI_ConnectTCPIP("169.254.9.199" ,50000);


    if(m_iId < 0) {
        MessageBoxA(0,"169.254.9.199 헥사포트 연결 실패","Error",0);//  ShowMessage();

    }

    Load(true);

    if(GetHomeDone()){
        CheckErr(PI_qPOS(m_iId , "" , m_daEncPos));
        for(int i = 0 ; i < MAX_PI_AXIS ; i++){
            m_daTrgPos[i]=m_daEncPos[i];
            m_daResPos[i]=m_daEncPos[i];
        }
    }

}
CHexaPot::~CHexaPot()
{
    PI_CloseConnection(m_iId);

    FreeDll();
    Load(false);
}

bool CHexaPot::LoadDll()
{
    String sDllPath = "PI_GCS2_DLL.dll";

    g_hPI_Dll = LoadLibrary (sDllPath.c_str());

    if(!g_hPI_Dll) return false ;

    // 클래스 적재 함수 받음
    PI_ConnectRS232          = (FP_ConnectRS232         )GetProcAddress(g_hPI_Dll, TEXT("PI_ConnectRS232"          )); if(!PI_ConnectRS232         )return false ;
    PI_CloseConnection       = (FP_CloseConnection      )GetProcAddress(g_hPI_Dll, TEXT("PI_CloseConnection"       )); if(!PI_CloseConnection      )return false ;
    PI_qIDN                  = (FP_qIDN                 )GetProcAddress(g_hPI_Dll, TEXT("PI_qIDN"                  )); if(!PI_qIDN                 )return false ;
    PI_GcsCommandset         = (FP_GcsCommandset        )GetProcAddress(g_hPI_Dll, TEXT("PI_GcsCommandset"         )); if(!PI_GcsCommandset        )return false ;
    PI_GcsGetAnswer          = (FP_GcsGetAnswer         )GetProcAddress(g_hPI_Dll, TEXT("PI_GcsGetAnswer"          )); if(!PI_GcsGetAnswer         )return false ;
    PI_GcsGetAnswerSize      = (FP_GcsGetAnswerSize     )GetProcAddress(g_hPI_Dll, TEXT("PI_GcsGetAnswerSize"      )); if(!PI_GcsGetAnswerSize     )return false ;
    PI_IsMoving              = (FP_IsMoving             )GetProcAddress(g_hPI_Dll, TEXT("PI_IsMoving"              )); if(!PI_IsMoving             )return false ;
    PI_qFRF                  = (FP_qFRF                 )GetProcAddress(g_hPI_Dll, TEXT("PI_qFRF"                  )); if(!PI_qFRF                 )return false ;
    PI_ConnectTCPIP          = (FP_ConnectTCPIP         )GetProcAddress(g_hPI_Dll, TEXT("PI_ConnectTCPIP"          )); if(!PI_ConnectTCPIP         )return false ;
    PI_EnumerateTCPIPDevices = (FP_EnumerateTCPIPDevices)GetProcAddress(g_hPI_Dll, TEXT("PI_EnumerateTCPIPDevices" )); if(!PI_EnumerateTCPIPDevices)return false ;
    PI_FRF                   = (FP_FRF                  )GetProcAddress(g_hPI_Dll, TEXT("PI_FRF"                   )); if(!PI_FRF                  )return false ;
    PI_IsControllerReady     = (FP_IsControllerReady    )GetProcAddress(g_hPI_Dll, TEXT("PI_IsControllerReady"     )); if(!PI_IsControllerReady    )return false ;
    PI_MOV                   = (FP_MOV                  )GetProcAddress(g_hPI_Dll, TEXT("PI_MOV"                   )); if(!PI_MOV                  )return false ;
    PI_MVR                   = (FP_MVR                  )GetProcAddress(g_hPI_Dll, TEXT("PI_MVR"                   )); if(!PI_MVR                  )return false ;
    PI_qPOS                  = (FP_qPOS                 )GetProcAddress(g_hPI_Dll, TEXT("PI_qPOS"                  )); if(!PI_qPOS                 )return false ;
    PI_SPI                   = (FP_SPI                  )GetProcAddress(g_hPI_Dll, TEXT("PI_SPI"                   )); if(!PI_SPI                  )return false ;
    PI_qCMN                  = (FP_qCMN                 )GetProcAddress(g_hPI_Dll, TEXT("PI_qCMN"                  )); if(!PI_qCMN                 )return false ;
    PI_qCMX                  = (FP_qCMX                 )GetProcAddress(g_hPI_Dll, TEXT("PI_qCMX"                  )); if(!PI_qCMX                 )return false ;
    PI_GetError              = (FP_GetError             )GetProcAddress(g_hPI_Dll, TEXT("PI_GetError"              )); if(!PI_GetError             )return false ;
    PI_qERR                  = (FP_qERR                 )GetProcAddress(g_hPI_Dll, TEXT("PI_qERR"                  )); if(!PI_qERR                 )return false ;
    PI_TranslateError        = (FP_TranslateError       )GetProcAddress(g_hPI_Dll, TEXT("PI_TranslateError"        )); if(!PI_TranslateError       )return false ;
    PI_qMOV                  = (FP_qMOV                 )GetProcAddress(g_hPI_Dll, TEXT("PI_qMOV"                  )); if(!PI_qMOV                 )return false ;
    PI_SVO                   = (FP_SVO                  )GetProcAddress(g_hPI_Dll, TEXT("PI_SVO"                   )); if(!PI_SVO                  )return false ;
    return true ;
}

bool CHexaPot::FreeDll()
{
    if(g_hPI_Dll==NULL) return false ;
    bool bRet = FreeLibrary(g_hPI_Dll);
    g_hPI_Dll=NULL;
    return bRet ;

}

bool CHexaPot::CheckErr(bool bRslt)
{
    if(bRslt) return true ;

    m_iLastErrCode = PI_GetError(m_iId);
    if(m_iLastErrCode == 0){
        long lConErrCode ;
        if(FP_qERR(m_iId , &lConErrCode)){
            m_iLastErrCode = lConErrCode ;
            char caErrMsg[128];
            PI_TranslateError(m_iLastErrCode , caErrMsg , 128);
            m_sLastErrMsg = caErrMsg ;
        }
        else {
            //함수 에러.

        }
    }
    else {
        char caErrMsg[128];
        PI_TranslateError(m_iLastErrCode , caErrMsg , 128);
        m_sLastErrMsg = caErrMsg ;
    }

    

    return false ;
}

bool CHexaPot::MoveTo(char* axis, double *dVal)
{
    //PI_MOV("1 2 3", pos[3])
    BOOL bIsMoving[MAX_PI_AXIS] ;//= TRUE;
    if(!CheckErr(PI_IsMoving(m_iId, axis, bIsMoving))){
        return false ;
    }
    if(bIsMoving[0])return false;

    return CheckErr(PI_MOV(m_iId, axis, dVal)) ;
}




void CHexaPot::Update()
{
    CheckErr(PI_qPOS(m_iId , "" , m_daEncPos));

}



bool CHexaPot::DoHome()
{
    BOOL bReferenced;
    BOOL bFlag[MAX_PI_AXIS] ;

    //홈잡으면 알아서 서보온 됌.

    if(!CheckErr(PI_IsControllerReady(m_iId, bFlag))){
        return false;
    }
    if(!CheckErr(PI_qFRF(m_iId, "x", &bReferenced))){
        return false;
    }

    m_daResPos[paX]= m_daTrgPos[paX]= 0.0;
    m_daResPos[paY]= m_daTrgPos[paY]= 0.0;
    m_daResPos[paZ]= m_daTrgPos[paZ]= 0.0;
    m_daResPos[paU]= m_daTrgPos[paU]= 0.0;
    m_daResPos[paV]= m_daTrgPos[paV]= 0.0;
    m_daResPos[paW]= m_daTrgPos[paW]= 0.0;

    return CheckErr(PI_FRF(m_iId, "x")) ;
}


void CHexaPot::MoveAbsRes(ePI_Axis _eAxis , double _dTarget) //쓰기편하게 한축씩 예약하고
{
    m_daResPos[_eAxis] = _dTarget ;

}
void CHexaPot::MoveIncRes(ePI_Axis _eAxis , double _dTarget) //쓰기편하게 한축씩 예약하고
{
    m_daResPos[_eAxis] = m_daResPos[_eAxis]+_dTarget ;

}

bool CHexaPot::Move() //위에서 예약한 곳으로 동시에 이동.
{
    if(!GetStop()) return false ;

    double daCrntTrgPos[MAX_PI_AXIS] ;
    if(!CheckErr(PI_qMOV(m_iId , "" ,daCrntTrgPos))){
        return false ;
    }





            bool bNeedToMove = false ;
    for(int i = 0 ; i < MAX_PI_AXIS ; i++){
        if(daCrntTrgPos[i] != m_daResPos[i]) {
            bNeedToMove = true ;
            break ;
        }
    }

    if(!bNeedToMove) return true ;

    MoveTo("x y z u v w", m_daResPos);
    for(int i = 0 ; i < MAX_PI_AXIS ; i++){
        m_daTrgPos[i] = m_daResPos[i];
    }

    return GetStop();
}

bool CHexaPot::GetStop()
{
    BOOL bIsMoving[MAX_PI_AXIS] ;//= TRUE;
    if(!CheckErr(PI_IsMoving(m_iId, "x", bIsMoving))){
        return false ;
    }
    if(bIsMoving[paX])return false;

    return true ;
}

bool CHexaPot::GetStopInpos()
{
    BOOL bIsMoving[MAX_PI_AXIS] ;//= TRUE;
    if(!CheckErr(PI_IsMoving(m_iId, "x", bIsMoving))){
        return false ;
    }
    if(bIsMoving[paX])return false;

    //double daCrntTrgPos[MAX_PI_AXIS] ;
    //if(!CheckErr(PI_qMOV(m_iId , "" ,daCrntTrgPos))){
    //    return false ;
    //}

    for(int i = 0 ; i < MAX_PI_AXIS ; i++){
        if(m_daEncPos[i] > m_daTrgPos[i]+0.01 ) return false ;
        if(m_daEncPos[i] < m_daTrgPos[i]-0.01 ) return false ;

    }
    return true ;

}

bool CHexaPot::SetPivotPnt(double _dX , double _dY , double _dZ)
{
    double dPnt[3];

    if(!CheckErr(PI_qPOS(m_iId , "" , m_daEncPos))){
        return false ;
    }

    //uvw 0점에서만 피봇 포인트 바꿀수 있음.
    if(!GetStop()) return false ;
    if(m_daEncPos[paU]!=0.0) return false ;
    if(m_daEncPos[paV]!=0.0) return false ;
    if(m_daEncPos[paW]!=0.0) return false ;

    dPnt[paX] = _dX ;
    dPnt[paY] = _dY ;
    dPnt[paZ] = _dZ ;

    return CheckErr(PI_SPI(m_iId , "x y z" , dPnt));
}

double CHexaPot::GetEncPos(ePI_Axis _eAxis)
{
    return m_daEncPos[_eAxis] ;
}

double CHexaPot::GetTrgPos(ePI_Axis _eAxis)
{
    return m_daTrgPos[_eAxis] ;
}

double CHexaPot::MoveSaveTrgRes(ePI_Axis _eAxis)
{
    m_daResPos[_eAxis] = m_daLastTrgPos[_eAxis];
}

double CHexaPot::GetLmtMinPos(ePI_Axis _eAxis)
{
    double daLmtMinPos[MAX_PI_AXIS];

    if(!CheckErr(PI_qCMN(m_iId , "" , daLmtMinPos))){
        return 0.0 ;
    }

    return daLmtMinPos[_eAxis];

}

double CHexaPot::GetLmtMaxPos(ePI_Axis _eAxis)
{
    double daLmtMaxPos[MAX_PI_AXIS];

    if(!CheckErr(PI_qCMN(m_iId , "" , daLmtMaxPos))){
        return 0.0 ;
    }

    return daLmtMaxPos[_eAxis];
}

bool CHexaPot::GetHomeDone()
{
    //에러 나면 홈잡는것으로 리턴.
    BOOL bReferenced[MAX_PI_AXIS];
    bool bRet ;
    if(!CheckErr(PI_qFRF(m_iId, NULL, bReferenced))){
        return false;
    }

    bRet = true ;
    for(int i = 0 ; i < MAX_PI_AXIS ; i++) {
        if(!bReferenced[i]){
            bRet = false ;
        }
    }
    return bRet ;


    /*
    BOOL bReferenced;
    BOOL bFlag[MAX_PI_AXIS] ;

    //홈잡으면 알아서 서보온 됌.

    if(!CheckErr(PI_IsControllerReady(m_iId, bFlag))){
        return false;
    }
    if(!CheckErr(PI_qFRF(m_iId, "x", &bReferenced))){
        return false;
    }

    m_daResPos[paX]= m_daTrgPos[paX]= 0.0;
    m_daResPos[paY]= m_daTrgPos[paY]= 0.0;
    m_daResPos[paZ]= m_daTrgPos[paZ]= 0.0;
    m_daResPos[paU]= m_daTrgPos[paU]= 0.0;
    m_daResPos[paV]= m_daTrgPos[paV]= 0.0;
    m_daResPos[paW]= m_daTrgPos[paW]= 0.0;

    return CheckErr(PI_FRF(m_iId, "x")) ;
    */

}

bool CHexaPot::SetServo(bool _bOn)
{
    BOOL baServoOn[MAX_PI_AXIS] ;//= TRUE;
    baServoOn[paX] = _bOn ;
    baServoOn[paY] = _bOn ;
    baServoOn[paZ] = _bOn ;
    baServoOn[paU] = _bOn ;
    baServoOn[paV] = _bOn ;
    baServoOn[paW] = _bOn ;

    if(!CheckErr(PI_SVO(m_iId, "x y z u v w", baServoOn))){
        return false ;
    }
    return true ;
}


String CHexaPot::GetLastErrMsg()
{
    return m_sLastErrMsg ;
}


void CHexaPot::Load(bool _bLoad)
{
    //Make Dir.
    TUserINI    UserINI ;
    AnsiString  sPath ,sItem;
    sPath = EXE_FOLDER + "SeqData\\HexaPotLastPos.INI";
    if ( _bLoad ) {
        //UserINI.Load(sPath, "Member" , "m_daTrgPosX" , m_daTrgPos[paX]);
        //UserINI.Load(sPath, "Member" , "m_daTrgPosY" , m_daTrgPos[paY]);
        //UserINI.Load(sPath, "Member" , "m_daTrgPosZ" , m_daTrgPos[paZ]);
        //UserINI.Load(sPath, "Member" , "m_daTrgPosU" , m_daTrgPos[paU]);
        //UserINI.Load(sPath, "Member" , "m_daTrgPosV" , m_daTrgPos[paV]);
        //UserINI.Load(sPath, "Member" , "m_daTrgPosW" , m_daTrgPos[paW]);
    }
    else {
        UserINI.ClearFile(sPath) ;
        //UserINI.Save(sPath, "Member" , "m_daTrgPosX" , m_daTrgPos[paX]);
        //UserINI.Save(sPath, "Member" , "m_daTrgPosY" , m_daTrgPos[paY]);
        //UserINI.Save(sPath, "Member" , "m_daTrgPosZ" , m_daTrgPos[paZ]);
        //UserINI.Save(sPath, "Member" , "m_daTrgPosU" , m_daTrgPos[paU]);
        //UserINI.Save(sPath, "Member" , "m_daTrgPosV" , m_daTrgPos[paV]);
        //UserINI.Save(sPath, "Member" , "m_daTrgPosW" , m_daTrgPos[paW]);
    }
}
