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

//���ߴܰ� ������ �α� 
//#define USE_LOG

//#ifdef USE_LOG
//#include "SLogUnit.h"
//#endif


//#define UNIT_TO_PULSE  (Para.iPulsePerRot/Para.dUnitPerRot)
#define PULSE_PER_UNIT (Para.iPulsePerRot/Para.dUnitPerRot)

//#define PULSE_TO_UNIT  (Para.dUnitPerRot/((double)Para.iPulsePerRot))
#define UNIT_PER_PULSE (Para.dUnitPerRot/((double)Para.iPulsePerRot))

#define START_STOP_SPEED 0.005 //����������� ������ ó�� ġ�� ������ �ӵ�. �̺��� ������ ������ �ȵ�.
#define LAST_HOME_VEL    0.005 //START_STOP_SPEED ���� ������ Ȩ�� ������ �ȵ�.

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


// ���� ���̺귯�� �� ��� �ʱ�ȭ 
bool CCamcFS20::InitLib()
{
	if(!AxtIsInitialized()){						// ���� ���̺귯���� ��� �������� (�ʱ�ȭ�� �Ǿ�����) Ȯ��
		if(!AxtInitialize(this, NULL)){			    // ���� ���̺귯�� �ʱ�ȭ
			AfxMessageBox( "Can not open Library!!");
			return false;
		}   
	}		

	if(!AxtIsInitializedBus(BUSTYPE_PCI)){			// PCI BUS�� �ʱ�ȭ �Ǿ����� Ȯ��
        if(!AxtOpenDeviceAuto(BUSTYPE_PCI)){		// ���ο� ���̽����带 �ڵ����� ���ն��̺귯���� �߰�		
			AfxMessageBox("Can not open Baseboard!!");
			return false;
		}
	 }

	if(!CFS20IsInitialized()){						// CAMC-FS����� ����� �� �ֵ��� ���̺귯���� �ʱ�ȭ�Ǿ����� Ȯ��
		if(!InitializeCAMCFS20(TRUE)){				// CAMC-FS����� �ʱ�ȭ(�����ִ� ��纣�̽����忡�� FS����� �˻��Ͽ� �ʱ�ȭ)
			AfxMessageBox("Can not open CAMC-FS module!!");
			return false;
		}
	}

	return true;
}

// ���� ���� ������ �������� ��찡 ������ �۾�ȿ���� �Ϲ����� ��� ��� �߷� ���� �ϰڴ�. 
// 1. - h + : ���� �̻����� ���� home������ Ȩ��´�. - h ���� ���̿� ���װ� ���ͼ� - h ���̿� ������ �Ѵ� ��� ���� �ʴ� ��찡 ������ �ִ�. 
//                                                    ���Ͱ� �ⱸ������ �׻� - + ���̿� �ֵ��� �����ؾ���.
//                                                     
// 2. -   + :                    -   ������ Ȩ��´�.  ���Ͱ� �ⱸ������ �׻� - + ���̿� �ֵ��� �����ؾ���.
// 3. h   + :                    home������ Ȩ��´�.  ���Ͱ� �ⱸ������ �׻� h + ���̿� �ֵ��� �����ؾ���. 
// 4. h     :                    home������ Ȩ��´�.  ������ ���� ����ó�� �Ұ���.
/*
CFS20start_signal_search1(INT16 axis, double velocity, double acceleration, UINT8 detect_signal);
UINT8 detect_signal ���� ����.
// detect_signal : �˻� ��ȣ ����(typedef : DETECT_DESTINATION_SIGNAL)
// PElmNegativeEdge    = 0x0,        // +Elm(End limit) �ϰ� edge
// NElmNegativeEdge    = 0x1,        // -Elm(End limit) �ϰ� edge
// PSlmNegativeEdge    = 0x2,        // +Slm(Slowdown limit) �ϰ� edge
// NSlmNegativeEdge    = 0x3,        // -Slm(Slowdown limit) �ϰ� edge
// In0DownEdge         = 0x4,        // IN0(ORG) �ϰ� edge
// In1DownEdge         = 0x5,        // IN1(Z��) �ϰ� edge
// In2DownEdge         = 0x6,        // IN2(����) �ϰ� edge
// In3DownEdge         = 0x7,        // IN3(����) �ϰ� edge
// PElmPositiveEdge    = 0x8,        // +Elm(End limit) ��� edge
// NElmPositiveEdge    = 0x9,        // -Elm(End limit) ��� edge
// PSlmPositiveEdge    = 0xa,        // +Slm(Slowdown limit) ��� edge
// NSlmPositiveEdge    = 0xb,        // -Slm(Slowdown limit) ��� edge
// In0UpEdge           = 0xc,        // IN0(ORG) ��� edge
// In1UpEdge           = 0xd,        // IN1(Z��) ��� edge
// In2UpEdge           = 0xe,        // IN2(����) ��� edge
// In3UpEdge           = 0xf         // IN3(����) ��� edge
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
		
	//Ȩ�����ų� -�����ų�.
    if(Para.bUseHomeSnsr) {  //Ȩ���� ������ �������忡�� ���� ������ �ȵǼ� ����(CycleHome) �ؾ� �Ѵ�. 
		bSensorStat    = Stat.bHomeSnsr ; 
	    cDetectRising  = Para.bHomeLevel ? 0x0C : 0x04;						
	    cDetectFalling = Para.bHomeLevel ? 0x04 : 0x0C;						
    }																			
    else {   	             //-���Լ����� �������忡�� ���� �����ϱ⶧���� ���� �ʿ� ����
		bSensorStat    = Stat.bNEndLimSnsr ;
	    cDetectRising  = 0x09 ;						
	    cDetectFalling = 0x01 ;					
    }

	if (HomeTimer.OnDelay( HomeStat.iHomeStep == iPreHomeStep , 10000 )) { //�� ���ܺ� 10��.
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

		//Ȩ������ Ȩ�� ��� ����.
		case 10 : 
			if(!GetStop()) return false ;
            if(bSensorStat) HomeStat.iHomeStep = 12 ; //Ȩ�� ������ ���� �ǰ� �ִ°��.
		    else            HomeStat.iHomeStep = 11 ; //Ȩ�� ������ ���� ���� �ʰ� �ִ� ���.
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
			//���������� Ȩ���� �ҵ�� �������� CFS20start_signal_search1_ex�Լ��� ��ü�ؾ���.
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
	CFS20set_moveunit_perpulse (m_iAxisNo, UNIT_PER_PULSE                               ); // Unit�� �̵������� ����. �̵��� �� �ӵ��� ������ �ش�
	CFS20set_startstop_speed   (m_iAxisNo, UNIT_PER_PULSE       * START_STOP_SPEED      ); // �ʱ�ӵ�(Start/Stop speed) ����	

    CFS20set_enc_input_method  (m_iAxisNo,                        AxtPara.iEncInput     ); // ���ڴ� �Է� ��� ����	
	CFS20set_pulse_out_method  (m_iAxisNo,                        AxtPara.iPulseOutput  ); // �޽� ��� ��� ����
	CFS20set_end_limit_enable  (m_iAxisNo,                        AxtPara.bEndLimEnable ); // ������ ����Ʈ ��� ��������� ����
	CFS20set_pend_limit_level  (m_iAxisNo,                        AxtPara.bPEndLimLevel ); // ������ ����Ʈ(+End limit)�� ��Ƽ�극�� ����	
	CFS20set_nend_limit_level  (m_iAxisNo,                        AxtPara.bNEndLimLevel ); // ������ ����Ʈ(-End limit)�� ��Ƽ�극�� ���� Ȩ������ �������� ��������.
	CFS20set_alarm_enable      (m_iAxisNo,                        AxtPara.bAlramEnable  ); // ���� �˶� �Է½�ȣ ����� ��������� ����
	CFS20set_alarm_level       (m_iAxisNo,                        AxtPara.bAlramLevel   ); // �˶�(Alarm) ��ȣ ��Ƽ�극�� ����
	CFS20set_inposition_enable (m_iAxisNo,                        AxtPara.bInposEnable  ); // ��������(Inposition) ��ȣ ������� ����
	CFS20set_inposition_level  (m_iAxisNo,                        AxtPara.bInposLevel   ); // ��������(Inposition) ��ȣ ��Ƽ�극�� ����
    CFS20set_enc_reverse       (m_iAxisNo,                        AxtPara.bEncReverse   ); // ���ڴ� ���� ��ȯ.
	CFS20set_max_speed         (m_iAxisNo, UNIT_PER_PULSE       * AxtPara.dMaxSpeed     ); // �ִ� ���� �ӵ��� ���� 
    CFS20set_s_rate            (m_iAxisNo, AxtPara.dSCurveAcPer , AxtPara.dSCurveDcPer  ); // �ִ� ���� �ӵ��� ���� 
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

	const int MIN_PULSE = 10  ; //10 Pulse �̸��̸� ��ǵ��� �ȵɼ� �ִ�.
	const int MAX_PULSE = 500 ; //500 Pulse �̻��̸� �ϳ����� �ϼ� �ִ�.
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
                iReceive = CFS20position_override(m_iAxisNo , _dPos * PULSE_PER_UNIT); //���� ���������� ���� ���� ���۰��� �ڵ������� ����.
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