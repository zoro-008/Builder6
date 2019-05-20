// CamcFS20.cpp: implementation of the CCamcFS20 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "hvsystem.h"
#include "Timer.h"
#include "CamcFS20.h"


#include "..\\IncludeAxt/AxtLIB.h"
#include "..\\IncludeAxt/AxtKeLIB.h"
#include "..\\IncludeAxt/AxtCAMCFS20.h"	
#include "..\\IncludeAxt/AxtKeCAMCFS20.h"
#include "..\\IncludeAxt/AxtTriggerLib.h"		
#include "..\\IncludeAxt/AxtTriggerDef.h"
#pragma comment(lib, "..\\LibraryAxt/AxtLib.lib")
#pragma comment(lib, "..\\LibraryAxt/AxtTriggerLib.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



///////////////////////////////////////////////////////////////////////
//CamcFS20.cpp
//Hanra Precision All Right Reserved.
//
//SUN Kye Won 2010.12.22 Ver 1.0
///////////////////////////////////////////////////////////////////////

//개발단계 디버깅용 로그 
//#define USE_LOG

//#ifdef USE_LOG
//#include "SLogUnit.h"
//#endif


//#define UNIT_TO_PULSE  (Para.iPulsePerRot/Para.dUnitPerRot)
#define PULSE_PER_UNIT (Para.iPulsePerRot/Para.dUnitPerRot)

//#define PULSE_TO_UNIT  (Para.dUnitPerRot/((double)Para.iPulsePerRot))
#define UNIT_PER_PULSE (Para.dUnitPerRot/((double)Para.iPulsePerRot))

#define START_STOP_SPEED 0.005 //모션프로파일 생성시 처음 치고 나가는 속도. 이보다 느리면 구동이 안됨.
#define LAST_HOME_VEL    0.005 //START_STOP_SPEED 보다 느리면 홈도 구동이 안됨.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCamcFS20::CCamcFS20(int _iAxisNo)
{
    InitLib();

	m_iAxisNo  = _iAxisNo ;
    
	memset(&AxtPara  , 0 , sizeof(CAxtPara  ));
	memset(&Para     , 0 , sizeof(CPara     ));
	memset(&UsrPara  , 0 , sizeof(CUsrPara  ));
	memset(&Stat     , 0 , sizeof(CStat     ));
	memset(&HomeStat , 0 , sizeof(CHomeStat ));
}

CCamcFS20::~CCamcFS20()
{

}


// 통합 라이브러리 및 모듈 초기화 
bool CCamcFS20::InitLib()
{
	if(!AxtIsInitialized()){						// 통합 라이브러리가 사용 가능하지 (초기화가 되었는지) 확인
		if(!AxtInitialize(this, NULL)){			    // 통합 라이브러리 초기화
			AfxMessageBox( "Can not open Library!!");
			return false;
		}   
	}		

	if(!AxtIsInitializedBus(BUSTYPE_PCI)){			// PCI BUS가 초기화 되었는지 확인
        if(!AxtOpenDeviceAuto(BUSTYPE_PCI)){		// 새로운 베이스보드를 자동으로 통합라이브러리에 추가		
			AfxMessageBox("Can not open Baseboard!!");
			return false;
		}
	 }

	if(!CFS20IsInitialized()){						// CAMC-FS모듈을 사용할 수 있도록 라이브러리가 초기화되었는지 확인
		if(!InitializeCAMCFS20(TRUE)){				// CAMC-FS모듈을 초기화(열려있는 모든베이스보드에서 FS모듈을 검색하여 초기화)
			AfxMessageBox("Can not open CAMC-FS module!!");
			return false;
		}
	}

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
// NElmPositiveEdge    = 0x9,        // -Elm(End limit) 상승 edge
// PSlmPositiveEdge    = 0xa,        // +Slm(Slowdown limit) 상승 edge
// NSlmPositiveEdge    = 0xb,        // -Slm(Slowdown limit) 상승 edge
// In0UpEdge           = 0xc,        // IN0(ORG) 상승 edge
// In1UpEdge           = 0xd,        // IN1(Z상) 상승 edge
// In2UpEdge           = 0xe,        // IN2(범용) 상승 edge
// In3UpEdge           = 0xf         // IN3(범용) 상승 edge
*/	    
bool CCamcFS20::CycleHome()
{
    //Check Status.
    if ( Stat.bAlarm   ) {HomeStat.iHomeStep = 0 ; HomeStat.bGoHome = false ; return false;}
    if (!Stat.bServoOn ) {HomeStat.iHomeStep = 0 ; HomeStat.bGoHome = false ; return false;}

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
	static CDelayTimer  HomeTimer       ;

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

	if (HomeTimer.OnDelay( HomeStat.iHomeStep == iPreHomeStep , 10000 )) { //각 스텝별 10초.
		HomeStat.iHomeStep    = 0     ;
		iPreHomeStep          = 0     ;  
        HomeStat.bGoHome      = false ;
		HomeStat.bHomeTimeout = true  ;
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
            CFS20start_signal_search1_ex(m_iAxisNo, -1 * dVel * PULSE_PER_UNIT , dAcc , cDetectRising);
			HomeStat.iHomeStep++ ;
			return false ; 

		case 12 :
            if(!GetStop()) return false ;
			CFS20start_signal_search1_ex(m_iAxisNo, dVel * PULSE_PER_UNIT , dAcc , cDetectFalling);
			HomeStat.iHomeStep++ ;
			return false ; 

		case 13 :
            if(!GetStop()) return false ;
			CFS20start_signal_search1_ex(m_iAxisNo, -1 * LAST_HOME_VEL, dAcc, cDetectRising);
			HomeStat.iHomeStep++ ;
			return false ; 

		case 14 :
            if(!GetStop()) return false ;
			CFS20start_signal_search2(m_iAxisNo,  LAST_HOME_VEL , cDetectFalling);
			//급정지한후 홈센서 불들어 와있으면 CFS20start_signal_search1_ex함수로 대체해야함.
			HomeStat.iHomeStep++ ;
			return false ; 

		case 15 :
			if(!CFS20get_home_done(m_iAxisNo)) return false;
            SetPos(0.0);
            CFS20start_move_ex(m_iAxisNo , UsrPara.dHomeOffset * PULSE_PER_UNIT , dVel * PULSE_PER_UNIT , UsrPara.dAccTime );
			HomeStat.iHomeStep++ ;
			return false ; 

		case 16 :
            if(!GetStop()) return false ; 
            SetPos(0.0);
			HomeStat.iHomeStep = 0 ;

			Stat.bHomeEnded = true ;
			iPreHomeStep       = 0 ;  
			return true ;
	}
}

void CCamcFS20::CycleRpt()
{
    static CDelayTimer RptTimer ;
	static bool        bFstPos  = false ;

	//Check Status.
    if(!Stat.bOk) return ;

    //Changing Timer.
    if(RptTimer .OnDelay(GetStop() , RptPara.dStopDelay )) {
        if(bFstPos) MoveAbsRun(RptPara.dScdPos) ;
		else        MoveAbsRun(RptPara.dFstPos) ;
	}
}

bool CCamcFS20::GetStop() 
{
	return (CFS20motion_done(m_iAxisNo) != 0);
}   

void CCamcFS20::Update()
{
	//Position
    Stat.dCmdPos = GetCmdPos();
    Stat.dEncPos = GetEncPos();

    //If Jog On , set target to current command.
    if (Stat.bJogN || Stat.bJogP) Stat.dTrgPos = Stat.dCmdPos;

    //Servo.
    Stat.bServoOn = Para.bSrvOnLevel ? (CFS20get_servo_enable(m_iAxisNo)==0) : (CFS20get_servo_enable(m_iAxisNo)!=0) ;

    //Home Sensor
    BYTE uinput;
    uinput = CFS20get_input(m_iAxisNo);
    if (((uinput >> 0 ) & 0x01) == 0x01) Stat.bHomeSnsr = Para.bHomeLevel ? 1 : 0;
    else                                 Stat.bHomeSnsr = Para.bHomeLevel ? 0 : 1;

    //Get Mechanical Signal.
    BYTE MC_Signal ;
    MC_Signal = CFS20get_mechanical_signal(m_iAxisNo);
    Stat.bStop        = CFS20motion_done(m_iAxisNo) != 0 ;
    Stat.bBusy        =!Stat.bStop;
    Stat.bPackInPosn  = ((MC_Signal >> 5) & 0x01) == 0x01;
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
    if ((Stat.dEncPos > Para.dMaxPos) && Stat.bServoOn && Stat.bHomeEnded && !Stat.bJogN) Stop();
    if ((Stat.dEncPos < Para.dMinPos) && Stat.bServoOn && Stat.bHomeEnded && !Stat.bJogP) Stop();

	//Check Limit Sensor.
	if (Stat.bPEndLimSnsr && Stat.bServoOn && !Stat.bJogN                       ) Stop();
    if (Stat.bNEndLimSnsr && Stat.bServoOn && !Stat.bJogP && !HomeStat.iHomeStep) Stop();

	//HomeStep.    
	if (HomeStat.iHomeStep) CycleHome();
	if (RptStat.bGoRpt    ) CycleRpt ();
}

void CCamcFS20::ApplyAxtPara ()
{
	CFS20set_moveunit_perpulse (m_iAxisNo, UNIT_PER_PULSE                               ); // Unit당 이동단위를 설정. 이동량 및 속도에 영향을 준다
	CFS20set_startstop_speed   (m_iAxisNo, UNIT_PER_PULSE       * START_STOP_SPEED      ); // 초기속도(Start/Stop speed) 설정	

    CFS20set_enc_input_method  (m_iAxisNo,                        AxtPara.iEncInput     ); // 엔코더 입력 방식 설정	
	CFS20set_pulse_out_method  (m_iAxisNo,                        AxtPara.iPulseOutput  ); // 펄스 출력 방식 설정
	CFS20set_end_limit_enable  (m_iAxisNo,                        AxtPara.bEndLimEnable ); // 급정지 리미트 기능 사용유무를 설정
	CFS20set_pend_limit_level  (m_iAxisNo,                        AxtPara.bPEndLimLevel ); // 정방향 리미트(+End limit)의 액티브레벨 설정	
	CFS20set_nend_limit_level  (m_iAxisNo,                        AxtPara.bNEndLimLevel ); // 역방향 리미트(-End limit)의 액티브레벨 설정 홈센서는 아진에서 지원안함.
	CFS20set_alarm_enable      (m_iAxisNo,                        AxtPara.bAlramEnable  ); // 서보 알람 입력신호 기능의 사용유무를 설정
	CFS20set_alarm_level       (m_iAxisNo,                        AxtPara.bAlramLevel   ); // 알람(Alarm) 신호 액티브레벨 설정
	CFS20set_inposition_enable (m_iAxisNo,                        AxtPara.bInposEnable  ); // 인포지션(Inposition) 신호 사용유무 설정
	CFS20set_inposition_level  (m_iAxisNo,                        AxtPara.bInposLevel   ); // 인포지션(Inposition) 신호 액티브레벨 설정
    CFS20set_enc_reverse       (m_iAxisNo,                        AxtPara.bEncReverse   ); // 엔코더 방향 전환.
	CFS20set_max_speed         (m_iAxisNo, UNIT_PER_PULSE       * AxtPara.dMaxSpeed     ); // 최대 구동 속도를 설정 
    CFS20set_s_rate            (m_iAxisNo, AxtPara.dSCurveAcPer , AxtPara.dSCurveDcPer  ); // 최대 구동 속도를 설정 
}

void CCamcFS20::SetHomeEnd(bool _bValue)
{
	if(_bValue){
	    Stat.bHomeEnded  = true ;
	}
	else {
        Stat.bHomeEnded  = false;
		HomeStat.bGoHome = false;
    }
}

void CCamcFS20::SetServo(bool _bOn)
{
    //Stop the Motor.
    Stop();

    //Servo On/Off.
    if (Para.bSrvOnLevel) {
        if (_bOn) CFS20set_output_bit  (m_iAxisNo , 0);
        else      CFS20reset_output_bit(m_iAxisNo , 0);
        }
    else {
        if (_bOn) CFS20reset_output_bit(m_iAxisNo , 0);
        else      CFS20set_output_bit  (m_iAxisNo , 0);
    }

}

void CCamcFS20::SetAlarm(bool _bOn)
{
    if (_bOn) CFS20set_output_bit  (m_iAxisNo , 1);
    else     CFS20reset_output_bit(m_iAxisNo , 1);
}     

bool CCamcFS20::GetStopInpos()
{
	if (!GetStop())return false ;

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
    
bool CCamcFS20::GetReady()
{
    return Stat.bReady ;
}         
                   
bool CCamcFS20::GetOk() 
{
    return Stat.bOk ;
}                 

double CCamcFS20::GetTrgPos()
{
    return Stat.dTrgPos ;
}

double CCamcFS20::GetCmdPos()
{
    if (PULSE_PER_UNIT <= 0) return 0;

    double dCmdPos  ;
    double dMotrPos ;

    dCmdPos  = CFS20get_command_position(m_iAxisNo);	
    dMotrPos = dCmdPos / PULSE_PER_UNIT;

    return dMotrPos  ;
}
                         
double CCamcFS20::GetEncPos()
{
    if (PULSE_PER_UNIT <= 0) return 0;

    double dEncPos  ;
    double dMotrPos ;

	//encPos = CFS20get_actual_position (Para.iModulPos); //Servo - Actual Position.

    if (Para.bStepMotr)	dEncPos = CFS20get_command_position(m_iAxisNo); //Step  - Command Position.
    else                dEncPos = CFS20get_actual_position (m_iAxisNo); //Servo - Actual Position.

    dMotrPos = dEncPos / PULSE_PER_UNIT;

    return dMotrPos;
}                             

double CCamcFS20::GetErrPos()
{
    return fabs(GetCmdPos() - GetEncPos());
}                                   

void CCamcFS20::ClearPos()
{
    if(!GetStop()) return ;

    Stat.bHomeEnded = false;
    if (HomeStat.iHomeStep) {
        HomeStat.bGoHome   = false ;
        HomeStat.iHomeStep = 0     ;
		CFS20abort_home_search(m_iAxisNo, 0) ;
    }
	
    RptStat.bGoRpt = false ;

	SetPos(0.0);
}                    

void CCamcFS20::SetPos (double _dPos )
{
    CFS20set_command_position(m_iAxisNo , PULSE_PER_UNIT * _dPos); //Commnad Position.
    CFS20set_actual_position (m_iAxisNo , PULSE_PER_UNIT * _dPos); //Actual Position.    
}   

void CCamcFS20::Reset()
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

bool CCamcFS20::Stop()
{
    //Check Latch Home.
    if (HomeStat.iHomeStep) {
        HomeStat.bGoHome   = false;
        HomeStat.iHomeStep = 0    ;
		//Stop Home Proc.
        CFS20abort_home_search(m_iAxisNo , 0);
    }

    //Reset Jog Status.
    Stat.bJogN = false;
    Stat.bJogP = false;

	//Reset Repeat Cycle.
	RptStat.bGoRpt = false ;

    //Stop.
    return CFS20set_stop(m_iAxisNo) != 0 ;
}

bool CCamcFS20::EmgStop()
{
    //Check Latch Home.
    if (HomeStat.iHomeStep) {
        HomeStat.bGoHome   = false;
        HomeStat.iHomeStep = 0    ;
    }

    //Stop Home Proc.
    CFS20abort_home_search(m_iAxisNo , 0);

    //Reset Jog Status.
    Stat.bJogN = false;
    Stat.bJogP = false;

	//Reset Repeat Cycle.
	RptStat.bGoRpt = false ;

    //Stop.
    return CFS20set_e_stop(m_iAxisNo) != 0 ;
}

bool CCamcFS20::MoveHome()
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

    return true;
}

void CCamcFS20::DoHome()
{
	HomeStat.iHomeStep  = 10    ;
	Stat.bHomeEnded     = false ;
}

void CCamcFS20::DoRpt()  //Start Home Cycle for Click Event.
{
    RptStat.bGoRpt = true ;
}

bool CCamcFS20::MoveJogP()
{
	//Var
    int    iRslt = 0 ;
    double dVel  = UsrPara.dJogSpeed ;
	double dAcc  = UsrPara.dAccTime  ;
    
	//Check Status.
    if ( Stat.bAlarm  ) return false;
    if (!Stat.bServoOn) return false;

    //Set Jog Status.
    Stat.bJogP = true;

    //Move Jog.
    if (dVel <   0.1) dVel =   0.1;
    if (dVel > 100.0) dVel = 100.0;

    iRslt = CFS20v_move_ex(m_iAxisNo , PULSE_PER_UNIT * dVel , dAcc);

    return (iRslt != 0);
}

bool CCamcFS20::MoveJogN()
{
	//Var.
    int    iRslt = 0 ;
    double dVel  = UsrPara.dJogSpeed ;
	double dAcc  = UsrPara.dAccTime  ;

    //Check Status.
    if ( Stat.bAlarm  ) return false;
    if (!Stat.bServoOn) return false;

    //Set Jog Status.
    Stat.bJogN = true;

    //Move Jog.
    if (dVel <   0.1) dVel =   0.1;
    if (dVel > 100.0) dVel = 100.0;

    iRslt = CFS20v_move_ex(m_iAxisNo , -1 * PULSE_PER_UNIT * dVel , dAcc);

    return (iRslt != 0);
}

bool CCamcFS20::MoveAbs(double _dPos , double _dVel)
{
	//return
	if (!Stat.bOk) return false;

    //Check Motor Limit.
    if (_dPos > Para.dMaxPos) { Stop(); return 0; }
    if (_dPos < Para.dMinPos) { Stop(); return 0; }

    //Local Var.
    int iReceive = 0 ;

    //Reset Jog Status.
    Stat.bJogN = false;
    Stat.bJogP = false;

    //Move Motor.
    if (GetStop()) {
		if(Para.bUseSCurve) iReceive = CFS20start_s_move_ex(m_iAxisNo , _dPos * PULSE_PER_UNIT , _dVel * PULSE_PER_UNIT , UsrPara.dAccTime ); 
		else                iReceive = CFS20start_move_ex  (m_iAxisNo , _dPos * PULSE_PER_UNIT , _dVel * PULSE_PER_UNIT , UsrPara.dAccTime );
	}
    else { 
        if (Stat.dTrgPos != _dPos) {//Position Override.
            if (fabs(_dPos - Stat.dTrgPos) > UsrPara.dInposition) {
                iReceive = CFS20position_override(m_iAxisNo , _dPos * PULSE_PER_UNIT); //감속 프로파일은 전에 무브 동작것을 자동적으로 따라감.
            }
        }
    }
       
	//Define Target Position.
    if (iReceive != 0) Stat.dTrgPos = _dPos;

    //Check InPos.
    return GetStopInpos();
}

void CCamcFS20::MoveInc(double _dPos , double _dVel )
{
    //Local Var.
    double dCmdPos = GetCmdPos();

    //Move Relative.
    MoveAbs(_dPos + dCmdPos , _dVel);
}  

bool CCamcFS20::MoveAbsRun (double _dPos )
{
    return MoveAbs(_dPos , UsrPara.dRunSpeed );
}

void CCamcFS20::MoveIncRun (double _dPos )
{
    MoveInc(_dPos , UsrPara.dRunSpeed );
}

bool CCamcFS20::MoveAbsMan (double _dPos )
{
    return MoveAbs(_dPos , UsrPara.dManSpeed );
}
void CCamcFS20::MoveIncMan (double _dPos )
{
    MoveInc(_dPos , UsrPara.dManSpeed );
}

void CCamcFS20::SetAxtPara  (CAxtPara &_AxtPara ){memcpy(& AxtPara  , &_AxtPara  , sizeof(CAxtPara ));}
void CCamcFS20::GetAxtPara  (CAxtPara &_AxtPara ){memcpy(&_AxtPara  , & AxtPara  , sizeof(CAxtPara ));}
void CCamcFS20::SetPara     (CPara    &_Para    ){memcpy(& Para     , &_Para     , sizeof(CPara    ));}
void CCamcFS20::GetPara     (CPara    &_Para    ){memcpy(&_Para     , & Para     , sizeof(CPara    ));}
void CCamcFS20::SetUsrPara  (CUsrPara &_UsrPara ){memcpy(& UsrPara  , &_UsrPara  , sizeof(CUsrPara ));}
void CCamcFS20::GetUsrPara  (CUsrPara &_UsrPara ){memcpy(&_UsrPara  , & UsrPara  , sizeof(CUsrPara ));}
void CCamcFS20::SetRptPara  (CRptPara &_RptPara ){memcpy(& RptPara  , &_RptPara  , sizeof(CRptPara ));} 
void CCamcFS20::GetRptPara  (CRptPara &_RptPara ){memcpy(&_RptPara  , & RptPara  , sizeof(CRptPara ));}

void CCamcFS20::GetStat     (CStat    &_Stat    ){memcpy(&_Stat     , & Stat     , sizeof(CStat    ));}
void CCamcFS20::GetHomeStat (CStat    &_HomeStat){memcpy(&_HomeStat , & HomeStat , sizeof(CHomeStat));}
void CCamcFS20::GetRptStat  (CRptPara &_RptStat ){memcpy(&_RptStat  , & RptStat  , sizeof(CRptStat ));}

void CCamcFS20::SavePara(CString _sFilePath)
{
	
}
void CCamcFS20::LoadPara(CString _sFilePath)
{

}