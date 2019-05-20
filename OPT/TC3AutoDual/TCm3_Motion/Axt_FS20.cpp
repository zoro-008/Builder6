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
#include "UserIni.h"
#include "SLogUnit.h"

#pragma package(smart_init)

//define

//#define USE_UNIT_PER_PULSE

//#ifdef USE_UNIT_PER_PULSE
#define PULSE_PER_UNIT (Para.iPulsePerRot/Para.dUnitPerRot)
//#define UNIT_PER_PULSE (Para.dUnitPerRot/((double)Para.iPulsePerRot))

#define START_STOP_SPEED     0.005 //모션프로파일 생성시 처음 치고 나가는 속도. 이보다 느리면 구동이 안됨.
#define LAST_HOME_VEL_RATIO  5.0  //START_STOP_SPEED 보다 느리면 홈도 구동이 안됨.


int GetAxtFS20AxisCnt()
{
    return CFS20get_total_numof_axis();
}

bool InitAxtFS20Board()
{
    if(!AxtIsInitialized()){				// 통합 라이브러리가 사용 가능하지 (초기화가 되었는지) 확인
        if(!AxtInitialize(NULL, NULL)){		        // 통합 라이브러리 초기화
            ShowMessageT("AJIN AXT Lib Loading Error");
            return false;
        }
    }

    if(!AxtIsInitializedBus(BUSTYPE_PCI)){		// PCI BUS가 초기화 되었는지 확인
        if(!AxtOpenDeviceAuto(BUSTYPE_PCI)){		// 새로운 베이스보드를 자동으로 통합라이브러리에 추가
            ShowMessageT("AJIN Axt Baseboard Loading Error");
            return false;
        }
    }

    if(!CFS20IsInitialized()){				// CAMC-FS모듈을 사용가능 한지 확인.
        if(!InitializeCAMCFS20(TRUE)){			// CAMC-FS모듈을 초기화(열려있는 모든베이스보드에서 FS모듈을 검색하여 초기화)
            ShowMessageT("AJIN Axt CAMC-FS Module Loading Error");
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
CAxtFS20::CAxtFS20()
{
    InitLib();

    memset(&AxtPara  , 0 , sizeof(CAxtPara  ));
    memset(&Para     , 0 , sizeof(CPara     ));
    memset(&UsrPara  , 0 , sizeof(CUsrPara  ));
    memset(&Stat     , 0 , sizeof(CStat     ));
    memset(&HomeStat , 0 , sizeof(CHomeStat ));
    memset(&RptPara  , 0 , sizeof(CRptPara  ));
    memset(&RptStat  , 0 , sizeof(CRptStat  ));
    memset(&VibStat  , 0 , sizeof(CVibStat  ));
}

CAxtFS20::~CAxtFS20()
{
    CloseAxtFS20Board();
}


// 통합 라이브러리 및 모듈 초기화
bool CAxtFS20::InitLib()
{
    static bool bFirst = true ;

    if(bFirst)InitAxtFS20Board();

    bFirst = false ;

    return true;
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
bool CAxtFS20::CycleHome()
{
    //Check Status.
    if ( Stat.bAlarm   ) {Trace(Para.sMotorName.c_str() , ("CycleHome  Stat.bAlarm    return "+(AnsiString)HomeStat.iHomeStep).c_str() ); HomeStat.iHomeStep = 0 ; HomeStat.bGoHome = false ; return false;}
    if (!Stat.bServoOn ) {Trace(Para.sMotorName.c_str() , ("CycleHome !Stat.bServoOn  return "+(AnsiString)HomeStat.iHomeStep).c_str() ); HomeStat.iHomeStep = 0 ; HomeStat.bGoHome = false ; return false;}

    //Local Var.
    char   cDetectRising  ;
    char   cDetectFalling ;
    bool   bSensorStat    ; //home sensor stat. (home or -limit)

    const int MIN_PULSE = 500    ;
    const int MAX_PULSE = 100000 ;

    double dMinUnit  = MIN_PULSE / PULSE_PER_UNIT ;
    double dMaxUnit  = MAX_PULSE / PULSE_PER_UNIT ;
    double dVel = UsrPara.dHomeSpeed ;
    double dAcc = UsrPara.dAccTime   ;

    static int         iPreHomeStep = 0 ;
    //static CDelayTimer  m_tmHomeCycle       ;

    //Check Limit.
    if(dVel < dMinUnit) dVel = dMinUnit ;
    if(dVel > dMaxUnit) dVel = dMaxUnit ;

    //홈센서거나 -센서거나.
    if(Para.bUseHomeSnsr) {  //홈센서 레벨은 아진보드에서 레벨 설정이 안되서 가공(CycleHome) 해야 한다.
        bSensorStat    = Stat.bHomeSnsr ;
        cDetectRising  = Para.bHomeLevel ? 0x0C : 0x04;
        cDetectFalling = Para.bHomeLevel ? 0x04 : 0x0C;
    }
    else {   	             //-리밋센서는 아진보드에서 설정 가능하기때문에 가공 필요 없음
        bSensorStat    = Stat.bNEndLimSnsr ;
        cDetectRising  = 0x09 ;
        cDetectFalling = 0x01 ;
    }

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("HomeStat.iHomeStep=%02d" , HomeStat.iHomeStep );
    if (iPreHomeStep != HomeStat.iHomeStep) {
        Trace(Para.sMotorName.c_str(),sTemp.c_str());
    }

    if (m_tmHomeCycle.OnDelay( HomeStat.iHomeStep == iPreHomeStep , 30000 )) { //각 스텝별 10초.
        HomeStat.iHomeStep    = 0     ;
        iPreHomeStep          = 0     ;
        HomeStat.bGoHome      = false ;
        HomeStat.bHomeTimeout = true  ;
        Stop();
        Trace(Para.sMotorName.c_str() , ("Home Time Out"+(AnsiString)HomeStat.iHomeStep).c_str() );
        return false;
    }

    iPreHomeStep = HomeStat.iHomeStep ;

    switch (HomeStat.iHomeStep) {
        default :
            return false ;

        //홈센서로 홈을 잡는 패턴.
        case 10 :
            if(!GetStop()) return false ;
            if(bSensorStat) HomeStat.iHomeStep = 12 ; //홈밍 센서가 감지 되고 있는경우.
            else            HomeStat.iHomeStep = 11 ; //홈밍 센서가 감지 되지 않고 있는 경우.
            return false ;

        case 11 :
            if(Stat.bNEndLimSnsr) { Stop(); HomeStat.iHomeStep++ ; return false ; }
            CFS20start_signal_search1_ex(Para.iPhysicalNo, -1 * dVel * PULSE_PER_UNIT , dAcc , cDetectRising);
            HomeStat.iHomeStep++ ;
            return false ;

        case 12 :
            if(!GetStop()) return false ;
            CFS20start_signal_search1_ex(Para.iPhysicalNo, dVel * PULSE_PER_UNIT , dAcc , cDetectFalling);
            HomeStat.iHomeStep++ ;
            return false ;

        case 13 :
            if(!GetStop()) return false ;
            CFS20start_signal_search1_ex(Para.iPhysicalNo, -1 * dVel * PULSE_PER_UNIT / LAST_HOME_VEL_RATIO , dAcc, cDetectRising);
            HomeStat.iHomeStep++ ;
            return false ;

        case 14 :
            if(!GetStop()) return false ;
            CFS20start_signal_search2(Para.iPhysicalNo,  dVel * PULSE_PER_UNIT / LAST_HOME_VEL_RATIO , cDetectFalling);
            //급정지한후 홈센서 불들어 와있으면 CFS20start_signal_search1_ex함수로 대체해야함.
            HomeStat.iHomeStep++ ;
            return false ;

        case 15 :
            if(!CFS20get_home_done(Para.iPhysicalNo)) return false;
            if(!GetStop()) return false ;

            HomeStat.iHomeStep++ ;
            return false ;

        case 16 :
            SetPos(0.0);
            if(CFS20start_move_ex(Para.iPhysicalNo , UsrPara.dHomeOffset * PULSE_PER_UNIT , dVel * PULSE_PER_UNIT , UsrPara.dAccTime )) {
                Stat.dTrgPos = UsrPara.dHomeOffset;
            }

            HomeStat.iHomeStep++ ;
            return false ;

        case 17 :
            if(!GetStop()) return false ;
            SetPos(0.0);
            HomeStat.iHomeStep = 0 ;

            Stat.bHomeEnded = true ;
            iPreHomeStep       = 0 ;

            Trace(Para.sMotorName.c_str(),"Home Cycle Finished");
            return true ;
    }
}

void CAxtFS20::CycleRpt()
{
    static CDelayTimer RptTimer ;
    static bool        bFstPos  = false ;

    //Check Status.
    if(!Stat.bOk) {Trace(Para.sMotorName.c_str() , ("CycleRpt !Stat.bOk return "+(AnsiString)HomeStat.iHomeStep).c_str() ); return ; }

    //Changing Timer.
    if(RptTimer .OnDelay(GetStop() , RptPara.dStopDelay )) {
        if(bFstPos) {GoAbsRun(RptPara.dScdPos) ; bFstPos = false ; }
        else        {GoAbsRun(RptPara.dFstPos) ; bFstPos = true  ; }
    }
}




bool CAxtFS20::Vibration(double _dRange , double _dVel , int _iCnt , bool _bCntClr)
{
    if(_iCnt <= 0 || !Stat.bOk ) {
        Trace(Para.sMotorName.c_str() , ("_iCnt < 0 || Stat.bOk"+(AnsiString)HomeStat.iHomeStep).c_str() );
        memset(&VibStat , 0 , sizeof(CVibStat));
        return true;
    }

    if(_bCntClr) memset(&VibStat , 0 , sizeof(CVibStat));

    switch (VibStat.iStep){
        case 0 : VibStat.dOriPos = GetCmdPos();
                 VibStat.iStep ++;
                 return false ;

        case 1 : VibStat.iCnt++ ;
                 GoAbs(VibStat.dOriPos + _dRange , _dVel) ;
                 VibStat.iStep ++;
                 return false ;

        case 2 : if(!GetStopInpos())return false ;
                 GoAbs(VibStat.dOriPos - _dRange , _dVel) ;
                 VibStat.iStep ++ ;
                 return false ;

        case 3 : if(!GetStopInpos())return false ;
                 if(VibStat.iCnt < _iCnt) {VibStat.iStep = 1 ; return false ;}
                 GoAbs(VibStat.dOriPos , _dVel) ;
                 VibStat.iStep ++ ;
                 return false ;

        case 4 : if(!GetStopInpos())return false ;
                 VibStat.iStep = 0 ;
                 return true ;
    }
    return true ;
}

bool CAxtFS20::GetStop()
{
    return (CFS20motion_done(Para.iPhysicalNo) != 0);
}

void CAxtFS20::Update()
{
    //Position
    Stat.dCmdPos = GetCmdPos();
    Stat.dEncPos = GetEncPos();

    //If Jog On , set target to current command.
    if (Stat.bJogN || Stat.bJogP) {
        Stat.dTrgPos = Stat.dCmdPos;
    }

    //Servo.
    Stat.bServoOn = Para.bSrvOnLevel ? (CFS20get_servo_enable(Para.iPhysicalNo)) : (!CFS20get_servo_enable(Para.iPhysicalNo)) ;

    //Home Sensor
    BYTE uinput;
    uinput = CFS20get_input(Para.iPhysicalNo);
    if (((uinput >> 0 ) & 0x01) == 0x01) Stat.bHomeSnsr = Para.bHomeLevel ? 1 : 0;
    else                                 Stat.bHomeSnsr = Para.bHomeLevel ? 0 : 1;

    //Get Mechanical Signal.
    BYTE MC_Signal ;
    MC_Signal         = CFS20get_mechanical_signal(Para.iPhysicalNo);
    Stat.bStop        = CFS20motion_done(Para.iPhysicalNo) != 0 ;
    Stat.bBusy        =!Stat.bStop;
    if(Para.bExistEnc) Stat.bPackInPosn  = ((MC_Signal >> 5) & 0x01) == 0x01;
    else               Stat.bPackInPosn  = GetStop() ;
    Stat.bAlarm       =!((MC_Signal >> 4) & 0x01) == 0x01;
    Stat.bPEndLimSnsr = ((MC_Signal >> 0) & 0x01) == 0x01;
    Stat.bNEndLimSnsr = ((MC_Signal >> 1) & 0x01) == 0x01;
    Stat.bOk          =!Stat.bPEndLimSnsr && !Stat.bNEndLimSnsr && !Stat.bAlarm && Stat.bHomeEnded && Stat.bServoOn ;
    Stat.bReady       = Stat.bOk && Stat.bPackInPosn && Stat.bStop;

    //If Servo is off, Set target position to current position.
    if (!Stat.bOk) Stat.dTrgPos = Stat.dEncPos;

    //Check Error.
    if ( Stat.bAlarm  ) { Stop(); Stat.bHomeEnded = false; }
    if (!Stat.bServoOn) {         Stat.bHomeEnded = false; }

    //Check Limit Pos.
    if (/*Para.bExistEnc &&*/ Para.dMaxPos && (Stat.dCmdPos > Para.dMaxPos) && Stat.bServoOn && Stat.bHomeEnded && !Stat.bJogN) {
        //Trace(((AnsiString)Para.iPhysicalNo + "Axis").c_str() , "MaxPosition Over");
        Stop();
    }

    if (/*Para.bExistEnc &&*/                 (Stat.dCmdPos < Para.dMinPos) && Stat.bServoOn && Stat.bHomeEnded && !Stat.bJogP) {
        //Trace(((AnsiString)Para.iPhysicalNo + "Axis").c_str() , "MinPosition Over");

        Stop();
    }

    //    //Check Limit Sensor.
    if (Stat.bPEndLimSnsr && Stat.bServoOn && !Stat.bJogN                       ){
         //Trace(((AnsiString)Para.iPhysicalNo + "Axis").c_str() , "P EndLim Sensor Touched");
         Stop();
    }

    if (Stat.bNEndLimSnsr && Stat.bServoOn && !Stat.bJogP && !HomeStat.iHomeStep) {
         //Trace(((AnsiString)Para.iPhysicalNo + "Axis").c_str() , "N EndLim Sensor Touched");
         Stop();
    }

    //HomeStep.
    if (HomeStat.iHomeStep) CycleHome();
    if (RptStat.bGoRpt    ) CycleRpt ();
}

void CAxtFS20::ApplyAxtPara ()
{
//    CFS20set_moveunit_perpulse (Para.iPhysicalNo, UNIT_PER_PULSE                               ); // Unit당 이동단위를 설정. 이동량 및 속도에 영향을 준다
    CFS20set_startstop_speed   (Para.iPhysicalNo, PULSE_PER_UNIT *       START_STOP_SPEED      ); // 초기속도(Start/Stop speed) 설정
//    CFS20set_max_speed         (Para.iPhysicalNo, PULSE_PER_UNIT *       AxtPara.dMaxSpeed     ); // 최대 구동 속도를 설정
    CFS20set_nend_limit_level  (Para.iPhysicalNo,                        AxtPara.bNEndLimLevel ); // 역방향 리미트(-End limit)의 액티브레벨 설정 홈센서는 아진에서 지원안함.
    CFS20set_pend_limit_level  (Para.iPhysicalNo,                        AxtPara.bPEndLimLevel ); // 정방향 리미트(+End limit)의 액티브레벨 설정
    CFS20set_inposition_level  (Para.iPhysicalNo,                        AxtPara.bInposLevel   ); // 인포지션(Inposition) 신호 액티브레벨 설정
    CFS20set_alarm_level       (Para.iPhysicalNo,                        AxtPara.bAlramLevel   ); // 알람(Alarm) 신호 액티브레벨 설정

    CFS20set_enc_input_method  (Para.iPhysicalNo,                        AxtPara.iEncInput     ); // 엔코더 입력 방식 설정
    CFS20set_pulse_out_method  (Para.iPhysicalNo,                        AxtPara.iPulseOutput  ); // 펄스 출력 방식 설정
    CFS20set_enc_reverse       (Para.iPhysicalNo,                        AxtPara.bEncReverse   ); // 엔코더 방향 전환.

//    CFS20set_end_limit_enable  (Para.iPhysicalNo,                        AxtPara.bEndLimEnable ); // 급정지 리미트 기능 사용유무를 설정
//    CFS20set_alarm_enable      (Para.iPhysicalNo,                        AxtPara.bAlramEnable  ); // 서보 알람 입력신호 기능의 사용유무를 설정
//    CFS20set_inposition_enable (Para.iPhysicalNo,                        AxtPara.bInposEnable  ); // 인포지션(Inposition) 신호 사용유무 설정
//    CFS20set_s_rate            (Para.iPhysicalNo, AxtPara.dSCurveAcPer , AxtPara.dSCurveDcPer  ); // 최대 구동 속도를 설정
}

void CAxtFS20::SetHomeEnd(bool _bValue)
{
    if(_bValue){
        Stat.bHomeEnded  = true ;
    }
    else {
        Stat.bHomeEnded  = false;
        HomeStat.bGoHome = false;
    }
}

bool CAxtFS20::GetHomeEnd ()
{
    return Stat.bHomeEnded ;
}

void CAxtFS20::SetServo(bool _bOn)
{
    //Stop the Motor.
    Stop();

    //Servo On/Off.
    if (Para.bSrvOnLevel) {
        if (_bOn) CFS20set_output_bit  (Para.iPhysicalNo , 0);
        else      CFS20reset_output_bit(Para.iPhysicalNo , 0);
    }
    else {
        if (_bOn) CFS20reset_output_bit(Para.iPhysicalNo , 0);
        else      CFS20set_output_bit  (Para.iPhysicalNo , 0);
    }

    if(!_bOn) SetHomeEnd(false);

}

bool CAxtFS20::GetServo()
{
    return Stat.bServoOn ;
}

void CAxtFS20::SetAlarm(bool _bOn)
{
    if (_bOn) CFS20set_output_bit  (Para.iPhysicalNo , 1);
    else      CFS20reset_output_bit(Para.iPhysicalNo , 1);
}

bool CAxtFS20::GetAlarm()
{
    return Stat.bAlarm ;
}

bool CAxtFS20::GetPLim()
{
    return Stat.bPEndLimSnsr ;
}

bool CAxtFS20::GetNLim()
{
    return Stat.bNEndLimSnsr ;
}

bool CAxtFS20::GetStopInpos()
{
    if (!GetStop     ())

    {
      return false ;

    }
    if (!Para.bExistEnc) return true ;

    const int MIN_PULSE = 10  ; //10 Pulse 미만이면 모션돈이 안될수 있다.
    const int MAX_PULSE = 500 ; //500 Pulse 이상이면 하나마나 일수 있다.
    double dMinUnit  = MIN_PULSE / PULSE_PER_UNIT ;
    double dMaxUnit  = MAX_PULSE / PULSE_PER_UNIT ;
    double dInp      = UsrPara.dInposition       ;

    if (dInp < dMinUnit) dInp = dMinUnit ;
    if (dInp > dMaxUnit) dInp = dMaxUnit ;

    if (fabs(GetCmdPos() - GetEncPos()) > dInp) return false;

    return true ;
}
    
bool CAxtFS20::GetReady()
{
    return Stat.bReady ;
}         
                   
bool CAxtFS20::GetOk() 
{
    return Stat.bOk ;
}

double CAxtFS20::GetTrgPos()
{
    return Stat.dTrgPos ;
}

double CAxtFS20::GetCmdPos()
{
    if (PULSE_PER_UNIT <= 0) return 0;

    double dCmdPos  ;
    double dMotrPos ;

    dCmdPos  = CFS20get_command_position(Para.iPhysicalNo);
    dMotrPos = dCmdPos / PULSE_PER_UNIT;

    return dMotrPos  ;
}

double CAxtFS20::GetEncPos()
{
    if (PULSE_PER_UNIT <= 0) return 0;

    double dEncPos  ;
    double dMotrPos ;

    //encPos = CFS20get_actual_position (Para.iModulPos); //Servo - Actual Position.

    if (Para.bExistEnc)	dEncPos = CFS20get_command_position(Para.iPhysicalNo); //Step               - Command Position.
    else                dEncPos = CFS20get_actual_position (Para.iPhysicalNo); //Servo , Easy Servo - Actual Position.

    dMotrPos = dEncPos / PULSE_PER_UNIT;

    return dMotrPos;
}

double CAxtFS20::GetErrPos()
{
    return fabs(GetCmdPos() - GetEncPos());
}

void CAxtFS20::ClearPos()
{
    if(!GetStop()) return ;

    Stat.bHomeEnded = false;
    if (HomeStat.iHomeStep) {
        HomeStat.bGoHome   = false ;
        HomeStat.iHomeStep = 0     ;
        CFS20abort_home_search(Para.iPhysicalNo, 0) ;
    }

    RptStat.bGoRpt = false ;

    SetPos(0.0);
}

void CAxtFS20::SetPos (double _dPos )
{
    CFS20set_command_position(Para.iPhysicalNo , PULSE_PER_UNIT * _dPos); //Commnad Position.
    CFS20set_actual_position (Para.iPhysicalNo , PULSE_PER_UNIT * _dPos); //Actual Position.
}


bool CAxtFS20::CmprPos(double _dPos, double _dRange)
{
    double dRange ;

    if(_dRange) dRange = _dRange ;
    else        dRange = UsrPara.dInposition ;

    bool isOk =    (fabs(GetEncPos() - _dPos) < dRange || !Para.bExistEnc)
                && (fabs(GetCmdPos() - _dPos) < dRange) ;

    return isOk;
}

void CAxtFS20::Reset()
{
    //Reset homing flag.
    if (GetStop()) {
        HomeStat.bGoHome      = false;
        HomeStat.iHomeStep    = 0    ;
        HomeStat.bHomeTimeout = false;
    }

    RptStat.bGoRpt = false ;

    //If alarm status, clear position.
    if (Stat.bAlarm) ClearPos();

    SetAlarm(false); //reset
}                      

bool CAxtFS20::Stop()
{
    //Check Latch Home.
    if (HomeStat.iHomeStep) {
        HomeStat.bGoHome   = false;
        HomeStat.iHomeStep = 0    ;
        //Stop Home Proc.
        CFS20abort_home_search(Para.iPhysicalNo , 0);
    }

    //Reset Jog Status.
    Stat.bJogN = false;
    Stat.bJogP = false;

    //Reset Repeat Cycle.
    RptStat.bGoRpt = false ;

    Trace(Para.sMotorName.c_str() , "Stop");

    //Stop.
    return CFS20set_stop(Para.iPhysicalNo) != 0 ;
}

bool CAxtFS20::EmgStop()
{
    //Check Latch Home.
    if (HomeStat.iHomeStep) {
        HomeStat.bGoHome   = false;
        HomeStat.iHomeStep = 0    ;
    }

    //Stop Home Proc.
    CFS20abort_home_search(Para.iPhysicalNo , 0);

    //Reset Jog Status.
    Stat.bJogN = false;
    Stat.bJogP = false;

    //Reset Repeat Cycle.
    RptStat.bGoRpt = false ;

    Trace(Para.sMotorName.c_str() , "Emergency Stop");

    //Stop.
    return CFS20set_e_stop(Para.iPhysicalNo) != 0 ;
}

bool CAxtFS20::Home()
{
    if (!HomeStat.bGoHome) {
        HomeStat.iHomeStep = 10    ;
        Stat.bHomeEnded    = false ;
        HomeStat.bGoHome   = true  ;
        return false ;
    }
    else {
        if(Stat.bHomeEnded) HomeStat.bGoHome = false ;
        return Stat.bHomeEnded;
    }
}

void CAxtFS20::DoHome()
{
    HomeStat.iHomeStep  = 10    ;
    Stat.bHomeEnded     = false ;
}

void CAxtFS20::DoRpt()  //Start Home Cycle for Click Event.
{
    if(!Stat.bOk) {ShowMessageT("Please Check Alram or home done."); return ;}
    RptStat.bGoRpt = true ;
}

bool CAxtFS20::JogP()
{
    //Var
    int    iRslt = 0 ;
    double dVel  = UsrPara.dJogSpeed ;
    double dAcc  = UsrPara.dAccTime  ;

    //Check Status.
    if ( Stat.bAlarm  ) {Trace(Para.sMotorName.c_str() , ("JogP  Stat.bAlarm return "  +(AnsiString)HomeStat.iHomeStep).c_str() ); return false;}
    if (!Stat.bServoOn) {Trace(Para.sMotorName.c_str() , ("JogP !Stat.bServoOn return "+(AnsiString)HomeStat.iHomeStep).c_str() ); return false;}

    //Set Jog Status.
    Stat.bJogP = true;

    //Go Jog.
    if (dVel <   0.1) dVel =   0.1;
    if (dVel > 100.0) dVel = 100.0;


    double i = (Para.iPulsePerRot/Para.dUnitPerRot) * dVel;

    iRslt = CFS20v_move_ex(Para.iPhysicalNo , PULSE_PER_UNIT * dVel , dAcc);

    Trace(Para.sMotorName.c_str() , "Jog Positive");

    return (iRslt != 0);
}

bool CAxtFS20::JogN()
{
    //Var.
    int    iRslt = 0 ;
    double dVel  = UsrPara.dJogSpeed ;
    double dAcc  = UsrPara.dAccTime  ;

    //Check Status.
    if ( Stat.bAlarm  ) {Trace(Para.sMotorName.c_str() , ("JogN  Stat.bAlarm return "  +(AnsiString)HomeStat.iHomeStep).c_str() ); return false;}
    if (!Stat.bServoOn) {Trace(Para.sMotorName.c_str() , ("JogN !Stat.bServoOn return "+(AnsiString)HomeStat.iHomeStep).c_str() ); return false;}

    //Set Jog Status.
    Stat.bJogN = true;

    //Go Jog.
    if (dVel <   0.1) dVel =   0.1;
    if (dVel > 100.0) dVel = 100.0;

    iRslt = CFS20v_move_ex(Para.iPhysicalNo , -1 * PULSE_PER_UNIT * dVel , dAcc);

    Trace(Para.sMotorName.c_str() , "Jog Negative");

    return (iRslt != 0);
}

bool CAxtFS20::GoAbs(double _dPos , double _dVel)
{
    //return
    if (!Stat.bOk) {Trace(Para.sMotorName.c_str() , "GoAbs !Stat.bOk return "); return false;}

    //Check Motor Limit.
    if (_dPos > Para.dMaxPos) { Stop(); Trace(Para.sMotorName.c_str() , "GoAbs Para.dMaxPos return "); return 0; }
    if (_dPos < Para.dMinPos) { Stop(); Trace(Para.sMotorName.c_str() , "GoAbs Para.dMinPos return "); return 0; }

    //Local Var.
    int iReceive = 0 ;

    //Reset Jog Status.
    Stat.bJogN = false;
    Stat.bJogP = false;

    //Go Motor.
    if (GetStop()) {
        Trace(Para.sMotorName.c_str() , ("Move to " + AnsiString(_dPos)).c_str());
        if(Para.bUseSCurve) iReceive = CFS20start_s_move_ex(Para.iPhysicalNo , _dPos * PULSE_PER_UNIT , _dVel * PULSE_PER_UNIT , UsrPara.dAccTime );
        else                iReceive = CFS20start_move_ex  (Para.iPhysicalNo , _dPos * PULSE_PER_UNIT , _dVel * PULSE_PER_UNIT , UsrPara.dAccTime );
    }
    else {
        if (Stat.dTrgPos != _dPos) {//Position Override.
            if (fabs(_dPos - Stat.dTrgPos) > UsrPara.dInposition) {
                Trace(Para.sMotorName.c_str() , ("Position Override Move to " + AnsiString(_dPos)).c_str());
                iReceive = CFS20position_override(Para.iPhysicalNo , _dPos * PULSE_PER_UNIT); //감속 프로파일은 전에 무브 동작것을 자동적으로 따라감.
            }
        }
    }

    //Define Target Position.
    if (iReceive != 0) Stat.dTrgPos = _dPos;

    //Check InPos.
    bool bRet = GetStopInpos();
    return bRet ;
}

void CAxtFS20::GoInc(double _dPos , double _dVel )
{
    //Local Var.
    double dCmdPos = GetCmdPos();

    //Go Relative.
    GoAbs(_dPos + dCmdPos , _dVel);
}

bool CAxtFS20::GoAbsRun (double _dPos )
{
    return GoAbs(_dPos , UsrPara.dRunSpeed );
}

void CAxtFS20::GoIncRun (double _dPos )
{
    GoInc(_dPos , UsrPara.dRunSpeed );
}

bool CAxtFS20::GoAbsMan (double _dPos )
{
    return GoAbs(_dPos , UsrPara.dManSpeed );
}
void CAxtFS20::GoIncMan (double _dPos )
{
    GoInc(_dPos , UsrPara.dManSpeed );
}

void CAxtFS20::SetAxtPara  (CAxtPara &_AxtPara ){memcpy(& AxtPara  , &_AxtPara  , sizeof(CAxtPara ));}
void CAxtFS20::GetAxtPara  (CAxtPara &_AxtPara ){memcpy(&_AxtPara  , & AxtPara  , sizeof(CAxtPara ));}
void CAxtFS20::SetPara     (CPara    &_Para    ){memcpy(& Para     , &_Para     , sizeof(CPara    ));}
void CAxtFS20::GetPara     (CPara    &_Para    ){memcpy(&_Para     , & Para     , sizeof(CPara    ));}
void CAxtFS20::SetUsrPara  (CUsrPara &_UsrPara ){memcpy(& UsrPara  , &_UsrPara  , sizeof(CUsrPara ));}
void CAxtFS20::GetUsrPara  (CUsrPara &_UsrPara ){memcpy(&_UsrPara  , & UsrPara  , sizeof(CUsrPara ));}
void CAxtFS20::SetRptPara  (CRptPara &_RptPara ){memcpy(& RptPara  , &_RptPara  , sizeof(CRptPara ));}
void CAxtFS20::GetRptPara  (CRptPara &_RptPara ){memcpy(&_RptPara  , & RptPara  , sizeof(CRptPara ));}

void CAxtFS20::GetStat     (CStat    &_Stat    ){memcpy(&_Stat     , & Stat     , sizeof(CStat    ));}
void CAxtFS20::GetHomeStat (CStat    &_HomeStat){memcpy(&_HomeStat , & HomeStat , sizeof(CHomeStat));}
void CAxtFS20::GetRptStat  (CRptPara &_RptStat ){memcpy(&_RptStat  , & RptStat  , sizeof(CRptStat ));}

void CAxtFS20::Save(int _iAxisNo)
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sAxisNo ;
    AnsiString sPath   ;

    //Set Dir.
    sAxisNo = (AnsiString)_iAxisNo ;
    sPath   = EXE_FOLDER + "Util\\Motor_" + sAxisNo + "_Para.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save Device.
    UserINI.Save(sPath, "AxtPara"  , "iEncInput    " , AxtPara.iEncInput     );
    UserINI.Save(sPath, "AxtPara"  , "iPulseOutput " , AxtPara.iPulseOutput  );
    UserINI.Save(sPath, "AxtPara"  , "bEndLimEnable" , AxtPara.bEndLimEnable );
    UserINI.Save(sPath, "AxtPara"  , "bPEndLimLevel" , AxtPara.bPEndLimLevel );
    UserINI.Save(sPath, "AxtPara"  , "bNEndLimLevel" , AxtPara.bNEndLimLevel );
    UserINI.Save(sPath, "AxtPara"  , "bAlramEnable " , AxtPara.bAlramEnable  );
    UserINI.Save(sPath, "AxtPara"  , "bAlramLevel  " , AxtPara.bAlramLevel   );
    UserINI.Save(sPath, "AxtPara"  , "bInposEnable " , AxtPara.bInposEnable  );
    UserINI.Save(sPath, "AxtPara"  , "bInposLevel  " , AxtPara.bInposLevel   );
    UserINI.Save(sPath, "AxtPara"  , "bEncReverse  " , AxtPara.bEncReverse   );
    UserINI.Save(sPath, "AxtPara"  , "dMaxSpeed    " , AxtPara.dMaxSpeed     );
    UserINI.Save(sPath, "AxtPara"  , "dSCurveAcPer " , AxtPara.dSCurveAcPer  );
    UserINI.Save(sPath, "AxtPara"  , "dSCurveDcPer " , AxtPara.dSCurveDcPer  );

    UserINI.Save(sPath, "Para   "  , "sMotorName   " , Para   .sMotorName    );
    UserINI.Save(sPath, "Para   "  , "iPhysicalNo  " , Para   .iPhysicalNo   );
    UserINI.Save(sPath, "Para   "  , "bExistEnc    " , Para   .bExistEnc     );
    UserINI.Save(sPath, "Para   "  , "iPulsePerRot " , Para   .iPulsePerRot  );
    UserINI.Save(sPath, "Para   "  , "dUnitPerRot  " , Para   .dUnitPerRot   );
    UserINI.Save(sPath, "Para   "  , "bHomeLevel   " , Para   .bHomeLevel    );
    UserINI.Save(sPath, "Para   "  , "bUseHomeSnsr " , Para   .bUseHomeSnsr  );
    UserINI.Save(sPath, "Para   "  , "bUseSCurve   " , Para   .bUseSCurve    );
    UserINI.Save(sPath, "Para   "  , "bSrvOnLevel  " , Para   .bSrvOnLevel   );
    UserINI.Save(sPath, "Para   "  , "dMaxPos      " , Para   .dMaxPos       );
    UserINI.Save(sPath, "Para   "  , "dMinPos      " , Para   .dMinPos       );

    UserINI.Save(sPath, "UsrPara"  , "dAccTime     " , UsrPara.dAccTime      );
    UserINI.Save(sPath, "UsrPara"  , "dRunSpeed    " , UsrPara.dRunSpeed     );
    UserINI.Save(sPath, "UsrPara"  , "dManSpeed    " , UsrPara.dManSpeed     );
    UserINI.Save(sPath, "UsrPara"  , "dHomeSpeed   " , UsrPara.dHomeSpeed    );
    UserINI.Save(sPath, "UsrPara"  , "dJogSpeed    " , UsrPara.dJogSpeed     );
    UserINI.Save(sPath, "UsrPara"  , "dInposition  " , UsrPara.dInposition   );
    UserINI.Save(sPath, "UsrPara"  , "dHomeOffset  " , UsrPara.dHomeOffset   );

    UserINI.Save(sPath, "RptPara"  , "dFstPos      " , RptPara.dFstPos       );
    UserINI.Save(sPath, "RptPara"  , "dScdPos      " , RptPara.dScdPos       );
    UserINI.Save(sPath, "RptPara"  , "dStopDelay   " , RptPara.dStopDelay    );

    ApplyAxtPara ();

}
void CAxtFS20::Load(int _iAxisNo)
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sAxisNo ;
    AnsiString sPath   ;

    //Set Dir.
    sAxisNo = (AnsiString)_iAxisNo ;
    sPath   = EXE_FOLDER + "Util\\Motor_" + sAxisNo + "_Para.INI" ;

    //Save Device.
    UserINI.Load(sPath, "AxtPara"  , "iEncInput    " , AxtPara.iEncInput     );
    UserINI.Load(sPath, "AxtPara"  , "iPulseOutput " , AxtPara.iPulseOutput  );
    UserINI.Load(sPath, "AxtPara"  , "bEndLimEnable" , AxtPara.bEndLimEnable );
    UserINI.Load(sPath, "AxtPara"  , "bPEndLimLevel" , AxtPara.bPEndLimLevel );
    UserINI.Load(sPath, "AxtPara"  , "bNEndLimLevel" , AxtPara.bNEndLimLevel );
    UserINI.Load(sPath, "AxtPara"  , "bAlramEnable " , AxtPara.bAlramEnable  );
    UserINI.Load(sPath, "AxtPara"  , "bAlramLevel  " , AxtPara.bAlramLevel   );
    UserINI.Load(sPath, "AxtPara"  , "bInposEnable " , AxtPara.bInposEnable  );
    UserINI.Load(sPath, "AxtPara"  , "bInposLevel  " , AxtPara.bInposLevel   );
    UserINI.Load(sPath, "AxtPara"  , "bEncReverse  " , AxtPara.bEncReverse   );
    UserINI.Load(sPath, "AxtPara"  , "dMaxSpeed    " , AxtPara.dMaxSpeed     );
    UserINI.Load(sPath, "AxtPara"  , "dSCurveAcPer " , AxtPara.dSCurveAcPer  );
    UserINI.Load(sPath, "AxtPara"  , "dSCurveDcPer " , AxtPara.dSCurveDcPer  );

    UserINI.Load(sPath, "Para   "  , "sMotorName   " , Para   .sMotorName    );
    UserINI.Load(sPath, "Para   "  , "iPhysicalNo  " , Para   .iPhysicalNo   );
    UserINI.Load(sPath, "Para   "  , "bExistEnc    " , Para   .bExistEnc     );
    UserINI.Load(sPath, "Para   "  , "iPulsePerRot " , Para   .iPulsePerRot  );
    UserINI.Load(sPath, "Para   "  , "dUnitPerRot  " , Para   .dUnitPerRot   );
    UserINI.Load(sPath, "Para   "  , "bHomeLevel   " , Para   .bHomeLevel    );
    UserINI.Load(sPath, "Para   "  , "bUseHomeSnsr " , Para   .bUseHomeSnsr  );
    UserINI.Load(sPath, "Para   "  , "bUseSCurve   " , Para   .bUseSCurve    );
    UserINI.Load(sPath, "Para   "  , "bSrvOnLevel  " , Para   .bSrvOnLevel   );
    UserINI.Load(sPath, "Para   "  , "dMaxPos      " , Para   .dMaxPos       );
    UserINI.Load(sPath, "Para   "  , "dMinPos      " , Para   .dMinPos       );

    UserINI.Load(sPath, "UsrPara"  , "dAccTime     " , UsrPara.dAccTime      );
    UserINI.Load(sPath, "UsrPara"  , "dRunSpeed    " , UsrPara.dRunSpeed     );
    UserINI.Load(sPath, "UsrPara"  , "dManSpeed    " , UsrPara.dManSpeed     );
    UserINI.Load(sPath, "UsrPara"  , "dHomeSpeed   " , UsrPara.dHomeSpeed    );
    UserINI.Load(sPath, "UsrPara"  , "dJogSpeed    " , UsrPara.dJogSpeed     );
    UserINI.Load(sPath, "UsrPara"  , "dInposition  " , UsrPara.dInposition   );
    UserINI.Load(sPath, "UsrPara"  , "dHomeOffset  " , UsrPara.dHomeOffset   );

    UserINI.Load(sPath, "RptPara"  , "dFstPos      " , RptPara.dFstPos       );
    UserINI.Load(sPath, "RptPara"  , "dScdPos      " , RptPara.dScdPos       );
    UserINI.Load(sPath, "RptPara"  , "dStopDelay   " , RptPara.dStopDelay    );
}

void CAxtFS20::UpdateStat(TLabel *_lbServoOn     ,
                          TLabel *_lbHomeSnsr    ,
                          TLabel *_lbPEndLimSnsr ,
                          TLabel *_lbNEndLimSnsr ,
                          TLabel *_lbHomeEnded   ,
                          TLabel *_lbAlarm       ,
                          TLabel *_lbEncPos      ,
                          TLabel *_lbPackInPosn  ,
                          TLabel *_lbStop        ,
                          TLabel *_lbBusy        ,
                          TLabel *_lbReady       ,
                          TLabel *_lbOk          ,
                          TLabel *_lbJogP        ,
                          TLabel *_lbJogN        ,
                          TLabel *_lbTrgPos      ,
                          TLabel *_lbCmdPos      )

{
    if(_lbServoOn    ){_lbServoOn     -> Color   = Stat.bServoOn     ? clLime : clSilver ;}
    if(_lbHomeSnsr   ){_lbHomeSnsr    -> Color   = Stat.bHomeSnsr    ? clLime : clSilver ;}
    if(_lbPEndLimSnsr){_lbPEndLimSnsr -> Color   = Stat.bPEndLimSnsr ? clLime : clSilver ;}
    if(_lbNEndLimSnsr){_lbNEndLimSnsr -> Color   = Stat.bNEndLimSnsr ? clLime : clSilver ;}
    if(_lbHomeEnded  ){_lbHomeEnded   -> Color   = Stat.bHomeEnded   ? clLime : clSilver ;}
    if(_lbAlarm      ){_lbAlarm       -> Color   = Stat.bAlarm       ? clLime : clSilver ;}
    if(_lbEncPos     ){_lbEncPos      -> Caption = Stat.dEncPos                          ;                                                                  }
    if(_lbPackInPosn ){_lbPackInPosn  -> Color   = Stat.bPackInPosn  ? clLime : clSilver ;}
    if(_lbStop       ){_lbStop        -> Color   = Stat.bStop        ? clLime : clSilver ;}
    if(_lbBusy       ){_lbBusy        -> Color   = Stat.bBusy        ? clLime : clSilver ;}
    if(_lbReady      ){_lbReady       -> Color   = Stat.bReady       ? clLime : clSilver ;}
    if(_lbOk         ){_lbOk          -> Color   = Stat.bOk          ? clLime : clSilver ;}
    if(_lbJogP       ){_lbJogP        -> Color   = Stat.bJogP        ? clLime : clSilver ;}
    if(_lbJogN       ){_lbJogN        -> Color   = Stat.bJogN        ? clLime : clSilver ;}
    if(_lbTrgPos     ){_lbTrgPos      -> Caption = Stat.dTrgPos                          ;                                                                  }
    if(_lbCmdPos     ){_lbCmdPos      -> Caption = Stat.dCmdPos                          ;                                                                  }
}

void CAxtFS20::UpdatePara(bool    _bToTable     ,
                          TEdit * _edAccTime    ,
                          TEdit * _edRunSpeed   ,
                          TEdit * _edManSpeed   ,
                          TEdit * _edHomeSpeed  ,
                          TEdit * _edJogSpeed   ,
                          TEdit * _edInposition ,
                          TEdit * _edHomeOffset ,

                          TEdit * _edFstPos     ,
                          TEdit * _edScdPos     ,
                          TEdit * _edStopDelay  )
{
     if(_bToTable) {
         if(_edAccTime   ) _edAccTime    -> Text = UsrPara.dAccTime    ;
         if(_edRunSpeed  ) _edRunSpeed   -> Text = UsrPara.dRunSpeed   ;
         if(_edManSpeed  ) _edManSpeed   -> Text = UsrPara.dManSpeed   ;
         if(_edHomeSpeed ) _edHomeSpeed  -> Text = UsrPara.dHomeSpeed  ;
         if(_edJogSpeed  ) _edJogSpeed   -> Text = UsrPara.dJogSpeed   ;
         if(_edInposition) _edInposition -> Text = UsrPara.dInposition ;
         if(_edHomeOffset) _edHomeOffset -> Text = UsrPara.dHomeOffset ;

         if(_edFstPos    ) _edFstPos     -> Text = RptPara.dFstPos     ;
         if(_edScdPos    ) _edScdPos     -> Text = RptPara.dScdPos     ;
         if(_edStopDelay ) _edStopDelay  -> Text = RptPara.dStopDelay  ;
     }

     else {
         if(_edAccTime   ) UsrPara.dAccTime    = _edAccTime    -> Text.ToDouble() ;
         if(_edRunSpeed  ) UsrPara.dRunSpeed   = _edRunSpeed   -> Text.ToDouble() ;
         if(_edManSpeed  ) UsrPara.dManSpeed   = _edManSpeed   -> Text.ToDouble() ;
         if(_edHomeSpeed ) UsrPara.dHomeSpeed  = _edHomeSpeed  -> Text.ToDouble() ;
         if(_edJogSpeed  ) UsrPara.dJogSpeed   = _edJogSpeed   -> Text.ToDouble() ;
         if(_edInposition) UsrPara.dInposition = _edInposition -> Text.ToDouble() ;
         if(_edHomeOffset) UsrPara.dHomeOffset = _edHomeOffset -> Text.ToDouble() ;

         if(_edFstPos    ) RptPara.dFstPos     = _edFstPos     -> Text.ToDouble() ;
         if(_edScdPos    ) RptPara.dScdPos     = _edScdPos     -> Text.ToDouble() ;
         if(_edStopDelay ) RptPara.dStopDelay  = _edStopDelay  -> Text.ToDouble() ;
     }
}
