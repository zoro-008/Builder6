//---------------------------------------------------------------------------
#pragma hdrstop
#include "Axt_FS20.h"

//System header
#include <mem.h>
#include <math.h>

//Ajin Lib
#include "AxtLIB.h"
#include "AxtCAMCFS20.h"

//User Header
#include "Timer.h"
#include "UserIni.h"
#include "SLogUnit.h"
#include "FormMsgOk.h"

#pragma package(smart_init)

#pragma comment(lib, "AxtLib.lib")            // Ajinextek Library

//AXT Trigger
#pragma comment(lib, "AxtTriggerLib.lib")     // Ajinextek Trigger Library
#include "AXTKeLIB.h"             // Base Board의 1차 Low Level 함수를 사용하기 위해
#include "AxtKeCamcFs20.h"        // FS20의 1차 Low Level 함수를 사용하기 위해
#include "AxtTrigger.h"           // Tirgger Function 사용을 위한 헤더파일

#define     TRIG_DNEG	0       // Trigger Doun Edge
#define     TRIG_UPEG	1       // Trigger Up Edge

SAxtParaUi AxtParaUi ;

bool bInitOk ;

void MakeAxtUI()
{
    static bool bFirst = true ;
    if(!bFirst) return ;
    bFirst = false ;

    AxtParaUi.pnBase = new TPanel(Application);

    AxtParaUi.edPhysicalNo   = new TEdit    (AxtParaUi.pnBase); AxtParaUi.edPhysicalNo   -> Parent = AxtParaUi.pnBase ;                                                               SetAxtArrange(AxtParaUi.edPhysicalNo   , 1 , 0 );
    AxtParaUi.lbPhysicalNo   = new TLabel(AxtParaUi.pnBase);    AxtParaUi.lbPhysicalNo   -> Parent = AxtParaUi.pnBase ; AxtParaUi.lbPhysicalNo   -> Caption = "Physical No        " ; SetAxtArrange(AxtParaUi.lbPhysicalNo   , 0 , 0 );

    AxtParaUi.cbUseSCurve    = new TCheckBox(AxtParaUi.pnBase); AxtParaUi.cbUseSCurve    -> Parent = AxtParaUi.pnBase ; AxtParaUi.cbUseSCurve    -> Caption = "Use S Curve        " ; SetAxtArrange(AxtParaUi.cbUseSCurve    , 0 , 1 );

    AxtParaUi.cbEncReverse   = new TCheckBox(AxtParaUi.pnBase); AxtParaUi.cbEncReverse   -> Parent = AxtParaUi.pnBase ; AxtParaUi.cbEncReverse   -> Caption = "Enc Reverse        " ; SetAxtArrange(AxtParaUi.cbEncReverse   , 0 , 2 );

    AxtParaUi.cbEncInput     = new TComboBox(AxtParaUi.pnBase); AxtParaUi.cbEncInput     -> Parent = AxtParaUi.pnBase ;                                                               SetAxtArrange(AxtParaUi.cbEncInput     , 1 , 3 );
    AxtParaUi.lbEncInput     = new TLabel(AxtParaUi.pnBase);    AxtParaUi.lbEncInput     -> Parent = AxtParaUi.pnBase ; AxtParaUi.lbEncInput     -> Caption = "Enc Input          " ; SetAxtArrange(AxtParaUi.lbEncInput     , 0 , 3 );

    AxtParaUi.cbPulseOutput  = new TComboBox(AxtParaUi.pnBase); AxtParaUi.cbPulseOutput  -> Parent = AxtParaUi.pnBase ;                                                               SetAxtArrange(AxtParaUi.cbPulseOutput  , 1 , 4 );
    AxtParaUi.lbPulseOutput  = new TLabel(AxtParaUi.pnBase);    AxtParaUi.lbPulseOutput  -> Parent = AxtParaUi.pnBase ; AxtParaUi.lbPulseOutput  -> Caption = "Pulse Output Method" ; SetAxtArrange(AxtParaUi.lbPulseOutput  , 0 , 4 );

    AxtParaUi.edSCurveAcPer  = new TEdit    (AxtParaUi.pnBase); AxtParaUi.edSCurveAcPer  -> Parent = AxtParaUi.pnBase ;                                                               SetAxtArrange(AxtParaUi.edSCurveAcPer  , 1 , 5 );
    AxtParaUi.lbSCurveAcPer  = new TLabel(AxtParaUi.pnBase);    AxtParaUi.lbSCurveAcPer  -> Parent = AxtParaUi.pnBase ; AxtParaUi.lbSCurveAcPer  -> Caption = "S Curve Acc %      " ; SetAxtArrange(AxtParaUi.lbSCurveAcPer  , 0 , 5 );

    AxtParaUi.edSCurveDcPer  = new TEdit    (AxtParaUi.pnBase); AxtParaUi.edSCurveDcPer  -> Parent = AxtParaUi.pnBase ;                                                               SetAxtArrange(AxtParaUi.edSCurveDcPer  , 1 , 6 );
    AxtParaUi.lbSCurveDcPer  = new TLabel(AxtParaUi.pnBase);    AxtParaUi.lbSCurveDcPer  -> Parent = AxtParaUi.pnBase ; AxtParaUi.lbSCurveDcPer  -> Caption = "S Curve Dcc %      " ; SetAxtArrange(AxtParaUi.lbSCurveDcPer  , 0 , 6 );

    AxtParaUi.cbHomeActLevel = new TCheckBox(AxtParaUi.pnBase); AxtParaUi.cbHomeActLevel -> Parent = AxtParaUi.pnBase ; AxtParaUi.cbHomeActLevel -> Caption = "Home Active High   " ; SetAxtArrange(AxtParaUi.cbHomeActLevel , 0 , 7 );

    AxtParaUi.cbPLimActLevel = new TCheckBox(AxtParaUi.pnBase); AxtParaUi.cbPLimActLevel -> Parent = AxtParaUi.pnBase ; AxtParaUi.cbPLimActLevel -> Caption = "+ Limit Active High" ; SetAxtArrange(AxtParaUi.cbPLimActLevel , 0 , 8 );

    AxtParaUi.cbNLimActLevel = new TCheckBox(AxtParaUi.pnBase); AxtParaUi.cbNLimActLevel -> Parent = AxtParaUi.pnBase ; AxtParaUi.cbNLimActLevel -> Caption = "- Limit Active High" ; SetAxtArrange(AxtParaUi.cbNLimActLevel , 0 , 9 );

    AxtParaUi.cbAlarmActLevel= new TCheckBox(AxtParaUi.pnBase); AxtParaUi.cbAlarmActLevel-> Parent = AxtParaUi.pnBase ; AxtParaUi.cbAlarmActLevel-> Caption = "Alarm Active High  " ; SetAxtArrange(AxtParaUi.cbAlarmActLevel, 0 , 10);

    AxtParaUi.cbInposActLevel= new TCheckBox(AxtParaUi.pnBase); AxtParaUi.cbInposActLevel-> Parent = AxtParaUi.pnBase ; AxtParaUi.cbInposActLevel-> Caption = "Inpos Active High  " ; SetAxtArrange(AxtParaUi.cbInposActLevel, 0 , 11);


//    AxtParaUi.edSCurveDcPer -> Left = 1 ;
//    AxtParaUi.edSCurveDcPer -> Top  = 1 ;
/*
    AxtParaUi.cbEncInput    -> AddItem("Up/Down"         ,NULL);
    AxtParaUi.cbEncInput    -> AddItem("1 multiplication",NULL);
    AxtParaUi.cbEncInput    -> AddItem("2 multiplication",NULL);
    AxtParaUi.cbEncInput    -> AddItem("4 multiplication",NULL); //보통 4체배.

    AxtParaUi.cbPulseOutput -> AddItem("OneHighLowHigh"  ,NULL); // 0x0, 1펄스 방식, PULSE(Active High), 정방향(DIR=Low)  / 역방향(DIR=High)
    AxtParaUi.cbPulseOutput -> AddItem("OneHighHighLow"  ,NULL); // 0x1, 1펄스 방식, PULSE(Active High), 정방향(DIR=High) / 역방향(DIR=Low)
    AxtParaUi.cbPulseOutput -> AddItem("OneLowLowHigh "  ,NULL); // 0x2, 1펄스 방식, PULSE(Active Low),  정방향(DIR=Low)  / 역방향(DIR=High)
    AxtParaUi.cbPulseOutput -> AddItem("OneLowHighLow "  ,NULL); // 0x3, 1펄스 방식, PULSE(Active Low),  정방향(DIR=High) / 역방향(DIR=Low)
    AxtParaUi.cbPulseOutput -> AddItem("TwoCcwCwHigh  "  ,NULL); // 0x4, 2펄스 방식, PULSE(CCW:역방향),  DIR(CW:정방향),  Active High
    AxtParaUi.cbPulseOutput -> AddItem("TwoCcwCwLow   "  ,NULL); // 0x5, 2펄스 방식, PULSE(CCW:역방향),  DIR(CW:정방향),  Active Low
    AxtParaUi.cbPulseOutput -> AddItem("TwoCwCcwHigh  "  ,NULL); // 0x6, 2펄스 방식, PULSE(CW:정방향),   DIR(CCW:역방향), Active High
    AxtParaUi.cbPulseOutput -> AddItem("TwoCwCcwLow   "  ,NULL); // 0x7, 2펄스 방식, PULSE(CW:정방향),   DIR(CCW:역방향), Active Low
*/
}

void SetAxtUIParent (TPanel * _pnBase)
{
//  AxtParaUi.pnBase -> Parent = _pnBase ;
//  AxtParaUi.pnBase -> ParentWindow = _pnse ;
//  Parent = dynamic_cast<TWinControl *>(Owner);
//    AxtParaUi.pnBase = new TPanel(_pnBase);
//    AxtParaUi.pnBase -> Parent = _pnBase ;

    AxtParaUi.pnBase -> Parent = _pnBase  ;

    AxtParaUi.pnBase -> Align  = alClient ;

//    AxtParaUi.pnBase -> ParentFont = true ;

    AxtParaUi.cbEncInput    -> AddItem("Up/Down"         ,NULL);
    AxtParaUi.cbEncInput    -> AddItem("1 multiplication",NULL);
    AxtParaUi.cbEncInput    -> AddItem("2 multiplication",NULL);
    AxtParaUi.cbEncInput    -> AddItem("4 multiplication",NULL); //보통 4체배.

    AxtParaUi.cbPulseOutput -> AddItem("OneHighLowHigh"  ,NULL); // 0x0, 1펄스 방식, PULSE(Active High), 정방향(DIR=Low)  / 역방향(DIR=High)
    AxtParaUi.cbPulseOutput -> AddItem("OneHighHighLow"  ,NULL); // 0x1, 1펄스 방식, PULSE(Active High), 정방향(DIR=High) / 역방향(DIR=Low)
    AxtParaUi.cbPulseOutput -> AddItem("OneLowLowHigh "  ,NULL); // 0x2, 1펄스 방식, PULSE(Active Low),  정방향(DIR=Low)  / 역방향(DIR=High)
    AxtParaUi.cbPulseOutput -> AddItem("OneLowHighLow "  ,NULL); // 0x3, 1펄스 방식, PULSE(Active Low),  정방향(DIR=High) / 역방향(DIR=Low)
    AxtParaUi.cbPulseOutput -> AddItem("TwoCcwCwHigh  "  ,NULL); // 0x4, 2펄스 방식, PULSE(CCW:역방향),  DIR(CW:정방향),  Active High
    AxtParaUi.cbPulseOutput -> AddItem("TwoCcwCwLow   "  ,NULL); // 0x5, 2펄스 방식, PULSE(CCW:역방향),  DIR(CW:정방향),  Active Low
    AxtParaUi.cbPulseOutput -> AddItem("TwoCwCcwHigh  "  ,NULL); // 0x6, 2펄스 방식, PULSE(CW:정방향),   DIR(CCW:역방향), Active High
    AxtParaUi.cbPulseOutput -> AddItem("TwoCwCcwLow   "  ,NULL); // 0x7, 2펄스 방식, PULSE(CW:정방향),   DIR(CCW:역방향), Active Low

}

const int iControlWidth   = 130;//165 ;
const int iControlHeight  = 17 ;//25  ;
const int iStartRowOffset = 10 ;//15  ;
const int iStartColOffset = 15 ;//20  ;

void SetAxtArrange (TControl * _tcControl , int _iCol , int _iRow)
{
    _tcControl -> Left = _iCol * iControlWidth  * 1.1 + _iCol + iStartColOffset ; _tcControl -> Width = iControlWidth  ;
    _tcControl -> Top  = _iRow * iControlHeight * 1.1 + _iRow + iStartRowOffset ; _tcControl -> Height= iControlHeight ;
}

void DeleteAxtUI()
{
    if(AxtParaUi.pnBase) {
        delete AxtParaUi.pnBase ; //컴포넌트는 소유주나 Parent삭제시에 모두 제거 됨.
        AxtParaUi.pnBase = NULL ;
    }
}


int GetAxtFS20AxisCnt()
{
    return CFS20get_total_numof_axis();
}

bool InitAxtFS20Board()
{
    bInitOk = true ;

    if(!AxtIsInitialized()){				// 통합 라이브러리가 사용 가능하지 (초기화가 되었는지) 확인
        if(!AxtInitialize(NULL, NULL)){		        // 통합 라이브러리 초기화
            ShowMessageT("AJIN AXT Lib Loading Error");
            bInitOk = false ;
            return false;
        }
    }

    if(!AxtIsInitializedBus(BUSTYPE_PCI)){		// PCI BUS가 초기화 되었는지 확인
        if(!AxtOpenDeviceAuto(BUSTYPE_PCI)){		// 새로운 베이스보드를 자동으로 통합라이브러리에 추가
            ShowMessageT("AJIN Axt Baseboard Loading Error");
            bInitOk = false ;
            return false;
        }
    }

    if(!CFS20IsInitialized()){				// CAMC-FS모듈을 사용가능 한지 확인.
        if(!InitializeCAMCFS20(TRUE)){			// CAMC-FS모듈을 초기화(열려있는 모든베이스보드에서 FS모듈을 검색하여 초기화)
            ShowMessageT("AJIN Axt CAMC-FS Module Loading Error");
            bInitOk = false ;
            return false;
        }
    }

    return true ;
}

void CloseAxtFS20Board()
{
    AxtCloseDeviceAll();
    AxtClose();
}

//---CAxtFS20 Unit-------------------------------------------------------------
CAxtFS20::CAxtFS20(int _iAxisNo)
{
    InitLib();
    MakeAxtUI();

    memset(&AxtPara , 0 , sizeof(SAxtPara));

    m_iAxisNo = _iAxisNo ;

    Load();
}

CAxtFS20::~CAxtFS20()
{
//    DeleteAxtUI();
    CloseAxtFS20Board();
}


// 통합 라이브러리 및 모듈 초기화
bool CAxtFS20::InitLib()
{
    static bool bFirst = true ;
    if(!bFirst) return true ;
    bFirst = false ;

    InitAxtFS20Board();
    return true ;
}

// 모터 센서 구성은 여러가지 경우가 있으나 작업효율상 일반적인 경우 몇개만 추려 구현 하겠다.
// 1. - h + : 가장 이상적인 형태 home센서로 홈잡는다. - h 센서 사이에 도그가 들어와서 - h 사이에 있지만 둘다 들어 오지 않는 경우가 잇을수 있다.
//                                                    모터가 기구적으로 항상 - + 사이에 있도록 설계해야함.
//
// 2. -   + :                    -   센서로 홈잡는다.  모터가 기구적으로 항상 - + 사이에 있도록 설계해야함.
// 3. h   + :                    home센서로 홈잡는다.  모터가 기구적으로 항상 h + 사이에 있도록 설계해야함.
// 4. h     :                    home센서로 홈잡는다.  않좋은 상태 예외처리 불가능.
/*
CFS20start_signal_search1(INT16 axis, double velocity, double acceleration, UINT8 detect_signal);
UINT8 detect_signal 인자 설명.
// detect_signal : 검색 신호 설정(typedef : DETECT_DESTINATION_SIGNAL)
// PElmNegativeEdge    = 0x0,        // +Elm(End limit) 하강 edge
// NElmNegativeEdge    = 0x1,        // -Elm(End limit) 하강 edge
// PSlmNegativeEdge    = 0x2,        // +Slm(Slowdown limit) 하강 edge
// NSlmNegativeEdge    = 0x3,        // -Slm(Slowdown limit) 하강 edge
// In0DownEdge         = 0x4,        // IN0(ORG) 하강 edge
// In1DownEdge         = 0x5,        // IN1(Z상) 하강 edge
// In2DownEdge         = 0x6,        // IN2(범용) 하강 edge
// In3DownEdge         = 0x7,        // IN3(범용) 하강 edge
// PElmPositiveEdge    = 0x8,        // +Elm(End limit) 상승 edge
   NElmPositiveEdge    = 0x9,        // -Elm(End limit) 상승 edge
// PSlmPositiveEdge    = 0xa,        // +Slm(Slowdown limit) 상승 edge
// NSlmPositiveEdge    = 0xb,        // -Slm(Slowdown limit) 상승 edge
// In0UpEdge           = 0xc,        // IN0(ORG) 상승 edge
// In1UpEdge           = 0xd,        // IN1(Z상) 상승 edge
// In2UpEdge           = 0xe,        // IN2(범용) 상승 edge
// In3UpEdge           = 0xf         // IN3(범용) 상승 edge
*/

bool CAxtFS20::GetStop()
{
    return CFS20motion_done(AxtPara.iPhysicalNo) ;
}

void CAxtFS20::ApplyAxtPara ()
{
    const int iStart_Stop_speed = 10 ; //모션프로파일 생성시 처음 치고 나가는 속도. 이보다 느리면 구동이 안됨.
    CFS20set_startstop_speed   (AxtPara.iPhysicalNo, iStart_Stop_speed     ); // 초기속도(Start/Stop speed) 설정
    CFS20set_end_limit_enable  (AxtPara.iPhysicalNo, false                 ); // 급정지 리미트 기능 사용유무를 설정 윗단에서 해결 함.
    CFS20set_alarm_enable      (AxtPara.iPhysicalNo, false                 );

    CFS20set_enc_reverse       (AxtPara.iPhysicalNo, AxtPara.bEncReverse   ); // 엔코더 방향 전환.
    CFS20set_enc_input_method  (AxtPara.iPhysicalNo, AxtPara.iEncInput     ); // 엔코더 입력 방식 설정
    CFS20set_pulse_out_method  (AxtPara.iPhysicalNo, AxtPara.iPulseOutput  ); // 펄스 출력 방식 설정
    CFS20set_s_rate            (AxtPara.iPhysicalNo, AxtPara.dSCurveAcPer ,
                                                     AxtPara.dSCurveDcPer  ); // S Curve 세팅.

    CFS20set_pend_limit_level  (AxtPara.iPhysicalNo, AxtPara.bPLimActLevel ); // 정방향 리미트(+End limit)의 액티브레벨 설정
    CFS20set_nend_limit_level  (AxtPara.iPhysicalNo, AxtPara.bNLimActLevel ); // 역방향 리미트(-End limit)의 액티브레벨 설정
    CFS20set_alarm_level       (AxtPara.iPhysicalNo, AxtPara.bAlarmActLevel); // 알람(Alarm) 신호 액티브레벨 설정
    CFS20set_inposition_level  (AxtPara.iPhysicalNo, AxtPara.bInposActLevel); // 인포지션(Inposition) 신호 액티브레벨 설정

    CFS20set_max_speed         (AxtPara.iPhysicalNo, 4000 * 1000 ); // 최대 구동 속도를 설정  4000mm/sec * Unit/Pulse

    //요놈들은 안쓰거나 윗단에서 해결 함.
//  CFS20set_moveunit_perpulse (AxtPara.iPhysicalNo, UNIT_PER_PULSE        ); // Unit당 이동단위를 설정. 이동량 및 속도에 영향을 준다 안씀.

//  CFS20set_end_limit_enable  (AxtPara.iPhysicalNo, AxtPara.bEndLimEnable ); // 급정지 리미트 기능 사용유무를 설정
//  CFS20set_alarm_enable      (AxtPara.iPhysicalNo, AxtPara.bAlarmEnable  ); // 서보 알람 입력신호 기능의 사용유무를 설정
//  CFS20set_inposition_enable (AxtPara.iPhysicalNo, AxtPara.bInposEnable  ); // 인포지션(Inposition) 신호 사용유무 설정
}

void CAxtFS20::SetServo(bool _bOn)
{
    //Stop the Motor.
    Stop();

    //Servo On/Off.
    if (_bOn) CFS20set_output_bit  (AxtPara.iPhysicalNo , 0);
    else      CFS20reset_output_bit(AxtPara.iPhysicalNo , 0);

}

bool CAxtFS20::GetServo()
{
    return CFS20get_servo_enable(AxtPara.iPhysicalNo) ;
}

void CAxtFS20::SetReset(bool _bOn)
{
    if (_bOn) CFS20set_output_bit  (AxtPara.iPhysicalNo , 1);
    else      CFS20reset_output_bit(AxtPara.iPhysicalNo , 1);
}

double CAxtFS20::GetCmdPos()
{
    return CFS20get_command_position(AxtPara.iPhysicalNo);
}

double CAxtFS20::GetEncPos()
{
    return CFS20get_actual_position (AxtPara.iPhysicalNo); //Servo , Easy Servo - Actual Position.
}

void CAxtFS20::SetPos (double _dPos )
{
    CFS20set_command_position(AxtPara.iPhysicalNo , _dPos); //Commnad Position.
    CFS20set_actual_position (AxtPara.iPhysicalNo , _dPos); //Actual Position.
}

bool CAxtFS20::GetHomeSnsr()
{
    //Home Sensor
    bool bRet ;

    bRet = ((CFS20get_input(AxtPara.iPhysicalNo) >> 0 ) & 0x01) ;
    bRet = AxtPara.bHomeActLevel ?  bRet : !bRet ;

    return bRet ;
}

bool CAxtFS20::GetNLimSnsr()
{
    //Get Mechanical Signal.
    bool bRet ;

    bRet = (CFS20get_mechanical_signal(AxtPara.iPhysicalNo) >> 1) & 0x01;

    return bRet ;
}

bool CAxtFS20::GetPLimSnsr()
{
    //Get Mechanical Signal.
    bool bRet ;

    bRet = (CFS20get_mechanical_signal(AxtPara.iPhysicalNo) >> 0) & 0x01;

    return bRet ;
}

bool CAxtFS20::GetZphaseSnsr()
{
    //Get ZPhase Signal.
    bool bRet ;

    bRet = ((CFS20get_input(AxtPara.iPhysicalNo) >> 1 ) & 0x01) ;
    bRet = !bRet ;//ra.bZphaActLevel ?  bRet : !bRet ;

    //if(bRet)MsgOk("ZZZZZ","GeeGeeGeeGee Baby Baby");

    return bRet ;
}

bool CAxtFS20::GetPackInPos()
{
    //Get Mechanical Signal.
    bool bRet ;

    bRet = (CFS20get_mechanical_signal(AxtPara.iPhysicalNo) >> 5) & 0x01;

    return bRet ;
}

bool CAxtFS20::GetAlarm()
{
    bool bRet ;

    bRet = (CFS20get_mechanical_signal(AxtPara.iPhysicalNo) >> 4) & 0x01;

    return bRet ;
}

bool CAxtFS20::GetHomeDone()
{
    bool  bRet ;

    bRet = CFS20get_home_done(AxtPara.iPhysicalNo) ;

    return bRet ;
}

bool CAxtFS20::Stop()
{
    //Stop.
    CFS20abort_home_search(AxtPara.iPhysicalNo , 0);
    
    return CFS20set_stop(AxtPara.iPhysicalNo) ;
}

bool CAxtFS20::EmgStop()
{
    //Stop Home Proc.
    CFS20abort_home_search(AxtPara.iPhysicalNo , 0);

    //Stop.
    return CFS20set_e_stop(AxtPara.iPhysicalNo) ;
}

bool CAxtFS20::JogP(double _dVel , double _dAcc)
{
    return CFS20v_move_ex(AxtPara.iPhysicalNo , _dVel , _dAcc);
}

bool CAxtFS20::JogN(double _dVel , double _dAcc)
{
    return CFS20v_move_ex(AxtPara.iPhysicalNo , -1 * _dVel , _dAcc);
}

bool CAxtFS20::GoAbs(double _dPos , double _dVel , double _dAcc)
{
    //Go Motor.
    if (GetStop()) {
        if(AxtPara.bUseSCurve) CFS20start_s_move_ex(AxtPara.iPhysicalNo , _dPos , _dVel , _dAcc );
        else                   CFS20start_move_ex  (AxtPara.iPhysicalNo , _dPos , _dVel , _dAcc );
    }

    //Check InPos.
    bool bRet = GetStop();
    return bRet ;
}

bool CAxtFS20::GoOvr(double _dPos)
{
    //Go Motor.
    CFS20position_override(AxtPara.iPhysicalNo , _dPos ); //감속 프로파일은 전에 무브 동작것을 자동적으로 따라감.

    //Check InPos.
    bool bRet = GetStop();
    return bRet ;
}

bool CAxtFS20::FindEdgeStop (double _dVel ,double _dAcc ,EN_FINDEDGE_TYPE _ftFindEdgeType)
{
    EN_FINDEDGE_TYPE ftFindEdgeType ;

    if(_ftFindEdgeType == ftOrgUpEdge || _ftFindEdgeType == ftOrgDnEdge) {
        if(AxtPara.bHomeActLevel) {
            ftFindEdgeType = _ftFindEdgeType ;
        }
        else {
            ftFindEdgeType = (_ftFindEdgeType == ftOrgUpEdge) ? ftOrgDnEdge : ftOrgUpEdge ;
        }
    }
    else {
        ftFindEdgeType = _ftFindEdgeType ;
    }
    return CFS20start_signal_search1_ex(AxtPara.iPhysicalNo, _dVel , _dAcc , ftFindEdgeType);
}

bool CAxtFS20::FindEdgeEmgStop(double _dVel ,EN_FINDEDGE_TYPE _ftFindEdgeType)
{
    EN_FINDEDGE_TYPE ftFindEdgeType ;

    if(_ftFindEdgeType == ftOrgUpEdge || _ftFindEdgeType == ftOrgDnEdge) {
        if(AxtPara.bHomeActLevel) {
            ftFindEdgeType = _ftFindEdgeType ;
        }
        else {
            ftFindEdgeType = (_ftFindEdgeType == ftOrgUpEdge) ? ftOrgDnEdge : ftOrgUpEdge ;
        }
    }
    else {
        ftFindEdgeType = _ftFindEdgeType ;
    }

    return CFS20start_signal_search2(AxtPara.iPhysicalNo, _dVel , ftFindEdgeType);
}

void CAxtFS20::SetAxtPara (SAxtPara &_AxtPara ){memcpy(& AxtPara , &_AxtPara , sizeof(SAxtPara ));}
void CAxtFS20::GetAxtPara (SAxtPara &_AxtPara ){memcpy(&_AxtPara , & AxtPara , sizeof(SAxtPara ));}


void CAxtFS20::Save()
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sAxisNo ;
    AnsiString sPath   ;

    //Set Dir.
    sAxisNo = (AnsiString)m_iAxisNo ;
    sPath   = EXE_FOLDER + "Util\\Motor_" + sAxisNo + "_Para.INI" ;

//    UserINI.ClearFile(sPath) ;

    //Save Device.
    UserINI.Save(sPath, "AxtPara"  , "iPhysicalNo   " , AxtPara.iPhysicalNo   );
    UserINI.Save(sPath, "AxtPara"  , "bUseSCurve    " , AxtPara.bUseSCurve    );
    UserINI.Save(sPath, "AxtPara"  , "bEncReverse   " , AxtPara.bEncReverse   );
    UserINI.Save(sPath, "AxtPara"  , "iEncInput     " , AxtPara.iEncInput     );
    UserINI.Save(sPath, "AxtPara"  , "iPulseOutput  " , AxtPara.iPulseOutput  );
    UserINI.Save(sPath, "AxtPara"  , "dSCurveAcPer  " , AxtPara.dSCurveAcPer  );
    UserINI.Save(sPath, "AxtPara"  , "dSCurveDcPer  " , AxtPara.dSCurveDcPer  );
    UserINI.Save(sPath, "AxtPara"  , "bHomeActLevel " , AxtPara.bHomeActLevel );
    UserINI.Save(sPath, "AxtPara"  , "bPLimActLevel " , AxtPara.bPLimActLevel );
    UserINI.Save(sPath, "AxtPara"  , "bNLimActLevel " , AxtPara.bNLimActLevel );
    UserINI.Save(sPath, "AxtPara"  , "bAlarmActLevel" , AxtPara.bAlarmActLevel);
    UserINI.Save(sPath, "AxtPara"  , "bInposActLevel" , AxtPara.bInposActLevel);

    ApplyAxtPara ();

}
void CAxtFS20::Load()
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sAxisNo ;
    AnsiString sPath   ;

    //Set Dir.
    sAxisNo = (AnsiString)m_iAxisNo ;
    sPath   = EXE_FOLDER + "Util\\Motor_" + sAxisNo + "_Para.INI" ;

    //Save Device.
    UserINI.Load(sPath, "AxtPara"  , "iPhysicalNo   " , AxtPara.iPhysicalNo   );
    UserINI.Load(sPath, "AxtPara"  , "bUseSCurve    " , AxtPara.bUseSCurve    );
    UserINI.Load(sPath, "AxtPara"  , "bEncReverse   " , AxtPara.bEncReverse   );
    UserINI.Load(sPath, "AxtPara"  , "iEncInput     " , AxtPara.iEncInput     );
    UserINI.Load(sPath, "AxtPara"  , "iPulseOutput  " , AxtPara.iPulseOutput  );
    UserINI.Load(sPath, "AxtPara"  , "dSCurveAcPer  " , AxtPara.dSCurveAcPer  );
    UserINI.Load(sPath, "AxtPara"  , "dSCurveDcPer  " , AxtPara.dSCurveDcPer  );
    UserINI.Load(sPath, "AxtPara"  , "bHomeActLevel " , AxtPara.bHomeActLevel );
    UserINI.Load(sPath, "AxtPara"  , "bPLimActLevel " , AxtPara.bPLimActLevel );
    UserINI.Load(sPath, "AxtPara"  , "bNLimActLevel " , AxtPara.bNLimActLevel );
    UserINI.Load(sPath, "AxtPara"  , "bAlarmActLevel" , AxtPara.bAlarmActLevel);
    UserINI.Load(sPath, "AxtPara"  , "bInposActLevel" , AxtPara.bInposActLevel);

    ApplyAxtPara ();
}

void CAxtFS20::UpdateAxtPara(bool _bToTable)
{
     if(_bToTable) {
         if(AxtParaUi.edPhysicalNo    ) AxtParaUi.edPhysicalNo    -> Text      = AxtPara.iPhysicalNo    ;
         if(AxtParaUi.cbUseSCurve     ) AxtParaUi.cbUseSCurve     -> Checked   = AxtPara.bUseSCurve     ;
         if(AxtParaUi.cbEncReverse    ) AxtParaUi.cbEncReverse    -> Checked   = AxtPara.bEncReverse    ;
         if(AxtParaUi.cbEncInput      ) AxtParaUi.cbEncInput      -> ItemIndex = AxtPara.iEncInput      ;
         if(AxtParaUi.cbPulseOutput   ) AxtParaUi.cbPulseOutput   -> ItemIndex = AxtPara.iPulseOutput   ;
         if(AxtParaUi.edSCurveAcPer   ) AxtParaUi.edSCurveAcPer   -> Text      = AxtPara.dSCurveAcPer   ;
         if(AxtParaUi.edSCurveDcPer   ) AxtParaUi.edSCurveDcPer   -> Text      = AxtPara.dSCurveDcPer   ;
         if(AxtParaUi.cbHomeActLevel  ) AxtParaUi.cbHomeActLevel  -> Checked   = AxtPara.bHomeActLevel  ;
         if(AxtParaUi.cbPLimActLevel  ) AxtParaUi.cbPLimActLevel  -> Checked   = AxtPara.bPLimActLevel  ;
         if(AxtParaUi.cbNLimActLevel  ) AxtParaUi.cbNLimActLevel  -> Checked   = AxtPara.bNLimActLevel  ;
         if(AxtParaUi.cbAlarmActLevel ) AxtParaUi.cbAlarmActLevel -> Checked   = AxtPara.bAlarmActLevel ;
         if(AxtParaUi.cbInposActLevel ) AxtParaUi.cbInposActLevel -> Checked   = AxtPara.bInposActLevel ;
     }

     else {
         if(AxtParaUi.edPhysicalNo    ) AxtPara.iPhysicalNo    = AxtParaUi.edPhysicalNo    -> Text.ToInt()    ;
         if(AxtParaUi.cbUseSCurve     ) AxtPara.bUseSCurve     = AxtParaUi.cbUseSCurve     -> Checked         ;
         if(AxtParaUi.cbEncReverse    ) AxtPara.bEncReverse    = AxtParaUi.cbEncReverse    -> Checked         ;
         if(AxtParaUi.cbEncInput      ) AxtPara.iEncInput      = AxtParaUi.cbEncInput      -> ItemIndex       ;
         if(AxtParaUi.cbPulseOutput   ) AxtPara.iPulseOutput   = AxtParaUi.cbPulseOutput   -> ItemIndex       ;
         if(AxtParaUi.edSCurveAcPer   ) AxtPara.dSCurveAcPer   = AxtParaUi.edSCurveAcPer   -> Text.ToDouble() ;
         if(AxtParaUi.edSCurveDcPer   ) AxtPara.dSCurveDcPer   = AxtParaUi.edSCurveDcPer   -> Text.ToDouble() ;
         if(AxtParaUi.cbHomeActLevel  ) AxtPara.bHomeActLevel  = AxtParaUi.cbHomeActLevel  -> Checked         ;
         if(AxtParaUi.cbPLimActLevel  ) AxtPara.bPLimActLevel  = AxtParaUi.cbPLimActLevel  -> Checked         ;
         if(AxtParaUi.cbNLimActLevel  ) AxtPara.bNLimActLevel  = AxtParaUi.cbNLimActLevel  -> Checked         ;
         if(AxtParaUi.cbAlarmActLevel ) AxtPara.bAlarmActLevel = AxtParaUi.cbAlarmActLevel -> Checked         ;
         if(AxtParaUi.cbInposActLevel ) AxtPara.bInposActLevel = AxtParaUi.cbInposActLevel -> Checked         ;
     }
}

void __stdcall TrgCallBack(INT16 nBoardNo, INT16 nModulePos, UINT8 byFlag)
{
    static int iInterruptCnt = 0 ;

    if(byFlag & 0x10000000) iInterruptCnt++;

    Trace("Triger" , AnsiString(iInterruptCnt).c_str());

    if(iInterruptCnt >10000) iInterruptCnt = 0 ;
}

void CAxtFS20::SetTrgPos(int _iPosCnt , double * _dPos , double _dTrgTime , bool _bActual ,bool _bOnLevel)
{
    if(!bInitOk) return ;

    // 인터럽트 메세지 관련 부분.----------------------------------------------------------------------------------
    // 인터럽트 설정 부분(이 예제에서는 인터럽트 발생 횟수를 카운트 하기위해 사용함, 실제 필요하지 않음.)
    static bool bFirst = true ;
    if(bFirst)CFS20SetWindowMessage(NULL, NULL , TrgCallBack);
    bFirst = false ;
    // User Selectable Interrupt Source0번에
    // Trigger 발생시에 인터럽트가 발생하도록 조건을 설정한다.
    CFS20KeSetCommandData(m_iAxisNo, FsUserInterruptSourceSelectRegWrite,(EVENT_NONE << 24)
                                                                        |(EVENT_NONE << 16)
                                                                        |(EVENT_NONE << 8)
                                                                        |(EVENT_TRIGGER_ENABLE) );

    // Interrupt Mask 레지스터에 User Selectable Interrupt Source 0번
    // 인터럽트를 사용하도록 설정한다. ( Bit28 )
    CFS20KeSetCommandData(m_iAxisNo, FsInterruptMaskWrite, 0x10000000);
    //------------------------------------------------------------------------------------------------------------

    CFS20Trig_Set_Level(m_iAxisNo, _bOnLevel?1:0); // 트리거 레벨 설정

    //    double trig_pos[] = {	20000.0, 30000.0, 40000.0, 50000.0, 70000.0, 75000.0, 80000.0, 81000.0};

    CFS20Trig_Absolute_Profile_Set(m_iAxisNo, _iPosCnt , _dPos, _dTrgTime , _bActual);
    //CFS20Trig_Absolute_Profile_Set(m_iAxisNo, 8 , trig_pos, 1000 , _bActual);

}

void CAxtFS20::ResetTrgPos()
{
    if(!bInitOk) return ;
    CFS20Trig_Profile_Reset(m_iAxisNo);
}

void CAxtFS20::OneShotTrg(bool _bUpEg , int _iTime)
{
    if(!bInitOk) return ;
    CFS20Trig_One_Shot_Set(m_iAxisNo, _bUpEg ? TRIG_UPEG : TRIG_DNEG , _iTime);

}
