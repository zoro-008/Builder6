//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <math.h>
#include <StdCtrls.hpp>

//---------------------------------------------------------------------------
#include "Motor.h"
#include "UserINI.h"
#include "SLogUnit.h"

#include "IOs.h"
#include "FormMsgOk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define PULSE_PER_UNIT (Para.iPulsePerRev/Para.dUnitPerRev)

void SetUI (TPanel * _pnBase)
{
    SetAjinUIParent(_pnBase);
}

//---CMotorBase Unit-------------------------------------------------------------
CMotor::CMotor(int _iAxisNo)
{
    m_iAxisNo = _iAxisNo ;
    m_bSubHome = false ;

    AjinMotion = NULL ;

    memset(&Para     , 0 , sizeof(SPara     ));
    memset(&UsrPara  , 0 , sizeof(SUsrPara  ));
    memset(&Stat     , 0 , sizeof(SStat     ));
    memset(&HomeStat , 0 , sizeof(SHomeStat ));
    memset(&RptPara  , 0 , sizeof(SRptPara  ));
    memset(&RptStat  , 0 , sizeof(SRptStat  ));

    AjinMotion = new CAjinMotion(m_iAxisNo) ;

    Load();

    //SetServo(true) ;

}

CMotor::~CMotor()
{
    //SetServo(false) ;
    delete AjinMotion ;
}

AnsiString CMotor::GetName()
{
    //if(Para.sMotorName =="") return "" ;
    return Para.sMotorName;
}

int CMotor::GetPhyscAdd()
{
    CAjinMotion::SAjinPara Para ;
    AjinMotion->GetAjinPara (Para);

    return Para.iPhysicalNo ;
}

EN_MTOR_DIRECTION CMotor::GetDirType()
{
    return (EN_MTOR_DIRECTION)Para.iDirType ;
}

bool CMotor::GetStop()
{
    return AjinMotion -> GetStop() ;
}

void CMotor::SetHomeEnd(bool _bValue)
{
    if(_bValue){
        Stat.bHomeEnded  = true ;
    }
    else {
        Stat.bHomeEnded  = false;
    }
    Update();
}

bool CMotor::GetHomeEnd ()
{
    return Stat.bHomeEnded ;

}


void CMotor::SetServo(bool _bOn)
{
    //Stop the Motor.
    if(!_bOn )Stop();

    //Servo On/Off.
    if (Para.bSrvOnLevelInv) AjinMotion -> SetServo(!_bOn) ;
    else                     AjinMotion -> SetServo( _bOn) ;

    if(!Para.bSrvBreakType) SetBreakOff(_bOn);

    if(!_bOn && !Para.bExistEnc) Stat.bHomeEnded = false ;
    if( _bOn &&  Para.bExistEnc) Stat.dCmdPos = GetEncPos() ;

    //���߿� ���� �Ǹ� ���⼭ ����.
    //if (!Para.bExistEnc) {Stat.bHomeEnded = false ;
    //else                 {


}

void CMotor::SetBreakOff(bool _bOn)
{
    if(Para.iBreakAdd > -1) IO.SetY(Para.iBreakAdd ,_bOn , true);
}

bool CMotor::GetBreakOff()
{
    return IO.GetY(Para.iBreakAdd);
}



bool CMotor::GetServo()
{
    bool bRet ;
    bRet = AjinMotion -> GetServo() ;

    if (Para.bSrvOnLevelInv) bRet = !AjinMotion -> GetServo() ;
    else                     bRet =  AjinMotion -> GetServo() ;

    return bRet ;
}

void CMotor::SetReset(bool _bOn)
{
    AjinMotion -> SetReset(_bOn) ;
}

bool CMotor::GetAlarm()
{
    bool bRet = AjinMotion -> GetAlarm() ;
    if(bRet && GetServo()) SetServo(false);
    return bRet ;
}

bool CMotor::GetHomeSnsr()
{
    return AjinMotion -> GetHomeSnsr() ;
}

bool CMotor::GetNLimSnsr()
{
    return AjinMotion -> GetNLimSnsr() ;
}

bool CMotor::GetPLimSnsr()
{
    return AjinMotion -> GetPLimSnsr() ;
}
bool CMotor::GetZphaseSnsr()
{
    return AjinMotion -> GetZphaseSnsr() ;
}
bool CMotor::GetPackInPos()
{
    return AjinMotion -> GetPackInPos() ;
}

bool CMotor::GetStopInpos()
{
    if (!GetStop()) {
        return false ;
    }

    if (!Para.bExistEnc) return true ;

    const int MIN_PULSE = 10  ; //10 Pulse �̸��̸� ��ǵ��� �ȵɼ� �ִ�.
    const int MAX_PULSE = 500 ; //500 Pulse �̻��̸� �ϳ����� �ϼ� �ִ�.
    double dMinUnit  = MIN_PULSE / PULSE_PER_UNIT ;
    double dMaxUnit  = MAX_PULSE / PULSE_PER_UNIT ;
    double dInp      = UsrPara.dInposition        ;

    if (dInp < dMinUnit) dInp = dMinUnit ;
    if (dInp > dMaxUnit) dInp = dMaxUnit ;

    double dCmdPos = GetCmdPos();
    double dEncPos = GetEncPos();

    if (fabs(dCmdPos - dEncPos) > dInp) return false;

    return true ;
}
    
bool CMotor::GetReady()
{
    return Stat.bReady ;
}         
                   
bool CMotor::GetOk()
{
    return Stat.bOk ;
}

double CMotor::GetTrgPos()
{
    return Stat.dTrgPos ;
}

double CMotor::GetCmdPos()
{
    if (PULSE_PER_UNIT <= 0) return 0;

    double dCmdPos  ;
    double dMotrPos ;

    dCmdPos  = AjinMotion -> GetCmdPos();
    dMotrPos = dCmdPos / PULSE_PER_UNIT;

    return dMotrPos  ;
}

double CMotor::GetEncPos()
{
    if (PULSE_PER_UNIT <= 0) return 0;

    double dEncPos  ;
    double dMotrPos ;

    //encPos = CFS20get_actual_position (Para.iModulPos); //Servo - Actual Position.

    if (Para.bExistEnc)	dEncPos = AjinMotion -> GetEncPos(); //Servo , Easy Servo - Actual Position.
    else                dEncPos = 0.0                   ; //Step               - Command Position.

    dMotrPos = dEncPos / PULSE_PER_UNIT;

    return dMotrPos;
}

double CMotor::GetErrPos()
{
    return fabs(GetCmdPos() - GetEncPos());
}

void CMotor::ClearPos()
{
    if(!GetStop()) return ;

    Stat.bHomeEnded = false;
    //if (HomeStat.iHomeStep) {
    //    HomeStat.iHomeStep = 0     ;
    //    CFS20abort_home_search(Para.iPhysicalNo, 0) ;
    //}

    RptStat.bGoRpt = false ;

    SetPos(0.0);
}

void CMotor::SetPos (double _dPos )
{
    if(!GetStop()) return ;

    AjinMotion -> SetPos(_dPos);
    Stat.dTrgPos = _dPos ;
}


bool CMotor::CmprPos(double _dPos, double _dRange)
{
    double dRange ;

    if(_dRange) dRange = _dRange ;
    else        dRange = UsrPara.dInposition ;

    bool isOk =    (fabs(GetEncPos() - _dPos) < dRange || !Para.bExistEnc)
                && (fabs(GetCmdPos() - _dPos) < dRange) ;

    return isOk;
}

double CMotor::GetMinPos()
{
    return Para.dMinPos ;
}

double CMotor::GetMaxPos()
{
    return Para.dMaxPos ;
}

double CMotor::GetRunVel()
{
    return UsrPara.dRunSpeed ;
}

double CMotor::GetSlowVel()
{
    return UsrPara.dSlowSpeed ;
}



double CMotor::GetManVel()
{
    return UsrPara.dManSpeed ;
}

bool CMotor::GoAbsAcDc(double _dPos , double _dVel , double _dAcc , double _dDcc)
{
    //return
    if (!Stat.bReady) {Trace(Para.sMotorName.c_str() , "GoAbs !Stat.bReady return "); return false;}

    //Check Motor Limit.
    if (_dPos > Para.dMaxPos) {
        if(m_dLastTgrPs != _dPos) {MsgOk("Warrning","Motor Max Position is larger than Move Position.");}
        Stop(); Trace(Para.sMotorName.c_str() , (String(_dPos) + " GoAbs Para.dMaxPos return ").c_str()); return false;
    }

    if (_dPos < Para.dMinPos) {
        if(m_dLastTgrPs != _dPos) {MsgOk("Warrning","Motor Min Position is small than Move Position." );}
        Stop(); Trace(Para.sMotorName.c_str() , (String(_dPos) + " GoAbs Para.dMinPos return ").c_str()); return false;
    }

    //Local Var.
    int iReceive = 0 ;

    //Reset Jog Status.
    Stat.bJogN = false;
    Stat.bJogP = false;

    //Go Motor.
    if (GetStop()) {
    //if (GetStopInpos()) { //���������� �ȵ��ͼ� �ٷ� else �� ���� �������̵��� Ÿ�� �Ͷ����� �ּ�ó�� (�����ÿ��� �̷���...;;; dd
        Stat.dSttPos = Stat.dTrgPos ;
        if(UsrPara.bUseRate) iReceive = AjinMotion -> GoAbs(_dPos * PULSE_PER_UNIT , _dVel * PULSE_PER_UNIT , _dAcc  * PULSE_PER_UNIT, _dDcc * PULSE_PER_UNIT);
        else                 iReceive = AjinMotion -> GoAbs(_dPos * PULSE_PER_UNIT , _dVel * PULSE_PER_UNIT , _dAcc                  , _dDcc                 );

        if(GetTrgPos() != _dPos){
            double dTemp1 = GetTrgPos() ;
            Trace(Para.sMotorName.c_str() , ("Move to " + AnsiString(_dPos)).c_str());
        }
    }
    else {
        if (Stat.dTrgPos != _dPos) {//Position Override.
            if (fabs(_dPos - Stat.dTrgPos) > UsrPara.dInposition) {
                Trace(Para.sMotorName.c_str() , ("Position Override Move to " + AnsiString(_dPos)).c_str());
                iReceive = AjinMotion -> GoOvr(Stat.dSttPos * PULSE_PER_UNIT , Stat.dTrgPos * PULSE_PER_UNIT , _dPos * PULSE_PER_UNIT); //���� ���������� ���� ���� ���۰��� �ڵ������� ����.
            }
        }
    }

    //Define Target Position.
    Stat.dTrgPos = _dPos;
    m_dLastTgrPs = Stat.dTrgPos;

    //Check InPos.
    bool bRet = GetStopInpos();

    return bRet ;
}

void CMotor::GoIncAcDc(double _dPos , double _dVel , double _dAcc , double _dDcc)
{
    //Local Var.
    double dCmdPos = GetCmdPos();

    //Go Relative.
    GoAbsAcDc(_dPos + dCmdPos , _dVel , _dAcc , _dDcc);
}

void CMotor::Reset()
{
    //Reset homing flag.
    if (GetStop()) {
        HomeStat.iHomeStep    = 0    ;
        HomeStat.bHomeTimeout = false;
    }

    RptStat.bGoRpt = false ;

    //If alarm status, clear position.
    if (GetAlarm()) {
        AjinMotion -> SetReset(true) ;
        ClearPos();
        Sleep(10);
        AjinMotion -> SetReset(false) ;
    }

    //clear AjinMotion -> SetReset(false) ;
}

void CMotor::Stop()
{
    //Check Latch Home.
    if (HomeStat.iHomeStep) {
        //HomeStat.iHomeStep = 0    ;
    }

    //Reset Jog Status.
    Stat.bJogN = false;
    Stat.bJogP = false;

    //Reset Repeat Cycle.
    RptStat.bGoRpt = false ;

    Trace(Para.sMotorName.c_str() , "Stop");

    //Stop.
    AjinMotion -> Stop() ;
}

void CMotor::EmgStop()
{
    //Check Latch Home.
    if (HomeStat.iHomeStep) {
        HomeStat.iHomeStep = 0    ;
    }

    //Reset Jog Status.
    Stat.bJogN = false;
    Stat.bJogP = false;

    //Reset Repeat Cycle.
    RptStat.bGoRpt = false ;

    Trace(Para.sMotorName.c_str() , "Emergency Stop");

    //Stop.
    AjinMotion -> EmgStop() ;
}

bool CMotor::Home()
{
    if (!Stat.bHomeEnded) {
        HomeStat.iHomeStep    = 10 ;
        HomeStat.iPreHomeStep = 0  ;
        Stat.bHomeEnded   = false ;
        SetServo(true);
        return false ;
    }
    else {
        return Stat.bHomeEnded ;
    }
}

void CMotor::DoHome()
{
    if(!Stat.bOk) {
        MsgOk("CONFIRM","Please Check Alarm or Servo On.");
        Trace(Para.sMotorName.c_str() , "Homming return Alarm or Not Servo On return ");
        return ;
    }

    //SetServo(true);
    HomeStat.iHomeStep    = 10  ;
    HomeStat.iPreHomeStep = 0   ;
    Stat.bHomeEnded   = false ; //dd
}

void CMotor::DoRpt(double _edFirstPos, double _edSecondPos, int _edStopDelay)  //Start Home Cycle for Click Event.
{
    if(!Stat.bReady) {
        MsgOk("CONFIRM","Please Check bReady.");
        Trace(Para.sMotorName.c_str() , "Repeat return Alarm or Not Servo On return ");
        return ;
    }
    RptPara.dFstPos = _edFirstPos;
    RptPara.dScdPos = _edSecondPos;
    RptPara.dStopDelay = _edStopDelay;
    RptStat.bGoRpt = true ;
}

bool CMotor::JogP()
{
    if(!Stat.bOk) {
        MsgOk("CONFIRM","Please Check Alarm or Servo On.");
        Trace(Para.sMotorName.c_str() , "JogN Alarm or Not Servo On return ");
        return false;
    }

    //Var
    double dVel  = UsrPara.dJogSpeed     ;
    double dAcc  = UsrPara.dAcceleration ;
    double dDec  = UsrPara.dDeceleration ;

    //Set Jog Status.
    Stat.bJogP = true;

    //Go Jog.
    if (dVel <   0.1) dVel =   0.1;
    if (dVel > 100.0) dVel = 100.0;


    double i = (Para.iPulsePerRev/Para.dUnitPerRev) * dVel;

    Trace(Para.sMotorName.c_str() , "Jog Positive");
    if(UsrPara.bUseRate) return AjinMotion -> JogP(PULSE_PER_UNIT * dVel , dAcc * PULSE_PER_UNIT, dDec * PULSE_PER_UNIT);
    else                 return AjinMotion -> JogP(PULSE_PER_UNIT * dVel , dAcc                 , dDec                 );

}

bool CMotor::JogN()
{
    if(!Stat.bOk) {
    //ShowMessageT("Please Check Alarm or home done.");
        MsgOk("CONFIRM","Please Check Alarm or Servo On.");
        Trace(Para.sMotorName.c_str() , "JogN Alarm or Not Servo On return ");
        return false;
    }

    //Var.
    double dVel  = UsrPara.dJogSpeed     ;
    double dAcc  = UsrPara.dAcceleration ;
    double dDec  = UsrPara.dDeceleration ;

    //Set Jog Status.
    Stat.bJogN = true;

    //Go Jog.
    if (dVel <   0.1) dVel =   0.1;
    if (dVel > 100.0) dVel = 100.0;

    Trace(Para.sMotorName.c_str() , "Jog Negative");

    if(UsrPara.bUseRate) return AjinMotion -> JogN(PULSE_PER_UNIT * dVel , dAcc * PULSE_PER_UNIT, dDec * PULSE_PER_UNIT);
    else                 return AjinMotion -> JogN(PULSE_PER_UNIT * dVel , dAcc                 , dDec                 );
}

bool CMotor::GoHomeOffset (double _dPos , double _dVel)
{
    //return
    if (!HomeStat.iHomeStep ) { Trace(Para.sMotorName.c_str() , "GoHomeOffset !HomeStat.iHomeStep return "); return false;}

    //Check Motor Limit.
    if (_dPos > Para.dMaxPos) {
        if(m_dLastTgrPs != _dPos) {MsgOk("Warrning","Motor Max Position is larger than Move Position."); }
        Stop(); Trace(Para.sMotorName.c_str() , "GoAbs Para.dMaxPos return "); return false;
    }
    if (_dPos < Para.dMinPos) {
        if(m_dLastTgrPs != _dPos) {MsgOk("Warrning","Motor Min Position is small than Move Position." ); }
        Stop(); Trace(Para.sMotorName.c_str() , "GoAbs Para.dMinPos return "); return false;
    }

    //if (_dPos > Para.dMaxPos) { Stop(); Trace(Para.sMotorName.c_str() , "GoHomeOffset Para.dMaxPos return "); return false; }
    //if (_dPos < Para.dMinPos) { Stop(); Trace(Para.sMotorName.c_str() , "GoHomeOffset Para.dMinPos return "); return false; }

    //Local Var.
    int iReceive = 0 ;

    //Go Motor.
    if (GetStop()) {
        Trace(Para.sMotorName.c_str() , ("GoHomeOffset to " + AnsiString(_dPos)).c_str());
        if(UsrPara.bUseRate) iReceive = AjinMotion -> GoAbs(_dPos * PULSE_PER_UNIT , _dVel * PULSE_PER_UNIT , UsrPara.dAcceleration * PULSE_PER_UNIT, UsrPara.dDeceleration * PULSE_PER_UNIT);
        else                 iReceive = AjinMotion -> GoAbs(_dPos * PULSE_PER_UNIT , _dVel * PULSE_PER_UNIT , UsrPara.dAcceleration                 , UsrPara.dDeceleration                 );
    }

    //Define Target Position.
    Stat.dTrgPos = _dPos;

    //Check InPos.
    bool bRet = GetStopInpos();
    return bRet ;
}

bool CMotor::GoAbs(double _dPos , double _dVel)
{
    return GoAbsAcDc(_dPos , _dVel , UsrPara.dAcceleration , UsrPara.dDeceleration);
    /*
    //return
    if (!Stat.bReady) {Trace(Para.sMotorName.c_str() , "GoAbs !Stat.bReady return "); return false;}

    //Check Motor Limit.
    if (_dPos > Para.dMaxPos) { Stop(); Trace(Para.sMotorName.c_str() , "GoAbs Para.dMaxPos return "); return 0; }
    if (_dPos < Para.dMinPos) { Stop(); Trace(Para.sMotorName.c_str() , "GoAbs Para.dMinPos return "); return 0; }

    //Local Var.
    int iReceive = 0 ;

    //Reset Jog Status.
    Stat.bJogN = false;
    Stat.bJogP = false;

    //Go Motor.
    //if (GetStop()) {
    if (GetStopInpos()) {
        iReceive = AjinMotion -> GoAbs(_dPos * PULSE_PER_UNIT , _dVel * PULSE_PER_UNIT , UsrPara.dAcceleration , UsrPara.dDeceleration);
        Trace(Para.sMotorName.c_str() , ("Move to " + AnsiString(_dPos)).c_str());
    }
    else {
        if (Stat.dTrgPos != _dPos) {//Position Override.
            if (fabs(_dPos - Stat.dTrgPos) > UsrPara.dInposition) {
                Trace(Para.sMotorName.c_str() , ("Position Override Move to " + AnsiString(_dPos)).c_str());
                iReceive = AjinMotion -> GoOvr(_dPos * PULSE_PER_UNIT); //���� ���������� ���� ���� ���۰��� �ڵ������� ����.
            }
        }
    }

    //Define Target Position.
    Stat.dTrgPos = _dPos;

    //Check InPos.
    bool bRet = GetStopInpos();

    return bRet ;*/
}

void CMotor::GoInc(double _dPos , double _dVel )
{
    //Local Var.
    double dCmdPos = GetCmdPos();

    //Go Relative.
    GoAbsAcDc(_dPos + dCmdPos , _dVel , UsrPara.dAcceleration , UsrPara.dDeceleration );
}

bool CMotor::GoAbsRun (double _dPos )
{
    return GoAbsAcDc(_dPos , UsrPara.dRunSpeed , UsrPara.dAcceleration , UsrPara.dDeceleration);
}

void CMotor::GoIncRun (double _dPos )
{
    GoIncAcDc(_dPos , UsrPara.dRunSpeed , UsrPara.dAcceleration , UsrPara.dDeceleration);
}

bool CMotor::GoAbsSlow (double _dPos )
{
    return GoAbsAcDc(_dPos , UsrPara.dSlowSpeed , UsrPara.dAcceleration , UsrPara.dDeceleration);
}

void CMotor::GoIncSlow (double _dPos )
{
    GoIncAcDc(_dPos , UsrPara.dSlowSpeed , UsrPara.dAcceleration , UsrPara.dDeceleration);
}

bool CMotor::GoAbsMan (double _dPos )
{
    return GoAbsAcDc(_dPos , UsrPara.dManSpeed , UsrPara.dAcceleration , UsrPara.dDeceleration);
}
void CMotor::GoIncMan (double _dPos )
{
    GoIncAcDc(_dPos , UsrPara.dManSpeed , UsrPara.dAcceleration , UsrPara.dDeceleration);
}

bool CMotor::FindEdgeStop (double _dVel ,double _dAcc ,CAjinMotion::EN_FINDEDGE_TYPE _ftFindEdgeType)
{
    if(UsrPara.bUseRate) return AjinMotion -> FindEdgeStop(_dVel * PULSE_PER_UNIT, _dAcc * PULSE_PER_UNIT, _ftFindEdgeType);
    else                 return AjinMotion -> FindEdgeStop(_dVel * PULSE_PER_UNIT, _dAcc                 , _ftFindEdgeType);
}

bool CMotor::FindEdgeEmgStop(double _dVel ,double _dAcc ,CAjinMotion::EN_FINDEDGE_TYPE _ftFindEdgeType)
{
    if(UsrPara.bUseRate) return AjinMotion -> FindEdgeEmgStop(_dVel * PULSE_PER_UNIT,_dAcc * PULSE_PER_UNIT, _ftFindEdgeType);
    else                 return AjinMotion -> FindEdgeEmgStop(_dVel * PULSE_PER_UNIT,_dAcc                 , _ftFindEdgeType);
}

void CMotor::SetPara    (SPara    &_Para    ){memcpy(& Para     , &_Para     , sizeof(SPara    ));}
void CMotor::SetUsrPara (SUsrPara &_UsrPara ){memcpy(& UsrPara  , &_UsrPara  , sizeof(SUsrPara ));}
void CMotor::SetRptPara (SRptPara &_RptPara ){memcpy(& RptPara  , &_RptPara  , sizeof(SRptPara ));}

CMotor::SPara     * CMotor::GetPara    (){return &Para    ;}
CMotor::SUsrPara  * CMotor::GetUsrPara (){return &UsrPara ;}
CMotor::SRptPara  * CMotor::GetRptPara (){return &RptPara ;}

CMotor::SStat     * CMotor::GetStat    (){return &Stat    ;}
CMotor::SHomeStat * CMotor::GetHomeStat(){return &HomeStat;}
CMotor::SRptStat  * CMotor::GetRptStat (){return &RptStat ;}

void CMotor::Save()
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sAxisNo ;
    AnsiString sPath   ;

    //Set Dir.
    sAxisNo = (AnsiString)m_iAxisNo ;
    sPath   = EXE_FOLDER + "Util\\Motor_" + sAxisNo + "_Para.INI" ;

    UserINI.ClearFile(sPath) ;

    //Save Device.
    UserINI.Save(sPath, "Para   "  , "sMotorName    " , Para   .sMotorName     );
    UserINI.Save(sPath, "Para   "  , "iMotionType   " , Para   .iMotionType    );
    UserINI.Save(sPath, "Para   "  , "iPulsePerRev  " , Para   .iPulsePerRev   );
    UserINI.Save(sPath, "Para   "  , "dUnitPerRev   " , Para   .dUnitPerRev    ); 
    UserINI.Save(sPath, "Para   "  , "dMaxPos       " , Para   .dMaxPos        );
    UserINI.Save(sPath, "Para   "  , "dMinPos       " , Para   .dMinPos        );
    UserINI.Save(sPath, "Para   "  , "bExistEnc     " , Para   .bExistEnc      );
    UserINI.Save(sPath, "Para   "  , "iHomeMethod   " , Para   .iHomeMethod    );
//  UserINI.Save(sPath, "Para   "  , "bHomeWithNSnsr" , Para   .bHomeWithNSnsr );
    UserINI.Save(sPath, "Para   "  , "bSrvOnLevelInv" , Para   .bSrvOnLevelInv );
    UserINI.Save(sPath, "Para   "  , "iBreakAdd     " , Para   .iBreakAdd      );
    UserINI.Save(sPath, "Para   "  , "iDirType      " , Para   .iDirType       );
    UserINI.Save(sPath, "Para   "  , "bSrvBreakType " , Para   .bSrvBreakType  );

  //UserINI.Save(sPath, "UsrPara"  , "dAccTime      " , UsrPara.dAccTime       );
    UserINI.Save(sPath, "UsrPara"  , "bUseRate      " , UsrPara.bUseRate       );
    UserINI.Save(sPath, "UsrPara"  , "dAcceleration " , UsrPara.dAcceleration  );
    UserINI.Save(sPath, "UsrPara"  , "dDeceleration " , UsrPara.dDeceleration  );
    UserINI.Save(sPath, "UsrPara"  , "dRunSpeed     " , UsrPara.dRunSpeed      );
    UserINI.Save(sPath, "UsrPara"  , "dSlowSpeed    " , UsrPara.dSlowSpeed     );
    UserINI.Save(sPath, "UsrPara"  , "dManSpeed     " , UsrPara.dManSpeed      );
    UserINI.Save(sPath, "UsrPara"  , "dHomeSpeed    " , UsrPara.dHomeSpeed     );
    UserINI.Save(sPath, "UsrPara"  , "dLastHomeSpeed" , UsrPara.dLastHomeSpeed );
    UserINI.Save(sPath, "UsrPara"  , "dJogSpeed     " , UsrPara.dJogSpeed      );
    UserINI.Save(sPath, "UsrPara"  , "dInposition   " , UsrPara.dInposition    );
    UserINI.Save(sPath, "UsrPara"  , "dHomeOffset   " , UsrPara.dHomeOffset    );

    UserINI.Save(sPath, "RptPara"  , "dFstPos       " , RptPara.dFstPos        );
    UserINI.Save(sPath, "RptPara"  , "dScdPos       " , RptPara.dScdPos        );
    UserINI.Save(sPath, "RptPara"  , "dStopDelay    " , RptPara.dStopDelay     );

    //AXT SUB UI SAVE
    if(AjinMotion) {
        AjinMotion->SetUseAccRate(UsrPara.bUseRate);
        AjinMotion->Save();
    }
}
void CMotor::Load()
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sAxisNo ;
    AnsiString sPath   ;

    //Set Dir.
    sAxisNo = (AnsiString)m_iAxisNo ;
    sPath   = EXE_FOLDER + "Util\\Motor_" + sAxisNo + "_Para.INI" ;

    //Save Device.
    UserINI.Load(sPath, "Para   "  , "sMotorName    " , Para   .sMotorName     );
    UserINI.Load(sPath, "Para   "  , "iMotionType   " , Para   .iMotionType    );
    UserINI.Load(sPath, "Para   "  , "iPulsePerRev  " , Para   .iPulsePerRev   );
    UserINI.Load(sPath, "Para   "  , "dUnitPerRev   " , Para   .dUnitPerRev    ); if(!Para.dUnitPerRev) Para.dUnitPerRev = 1 ;
    UserINI.Load(sPath, "Para   "  , "dMaxPos       " , Para   .dMaxPos        );
    UserINI.Load(sPath, "Para   "  , "dMinPos       " , Para   .dMinPos        );
    UserINI.Load(sPath, "Para   "  , "bExistEnc     " , Para   .bExistEnc      );
    UserINI.Load(sPath, "Para   "  , "iHomeMethod   " , Para   .iHomeMethod    );
//  UserINI.Load(sPath, "Para   "  , "bHomeWithNSnsr" , Para   .bHomeWithNSnsr );
    UserINI.Load(sPath, "Para   "  , "bSrvOnLevelInv" , Para   .bSrvOnLevelInv );
    UserINI.Load(sPath, "Para   "  , "iBreakAdd     " , Para   .iBreakAdd      );
    UserINI.Load(sPath, "Para   "  , "iDirType      " , Para   .iDirType       );
    UserINI.Load(sPath, "Para   "  , "bSrvBreakType " , Para   .bSrvBreakType  );

  //UserINI.Load(sPath, "UsrPara"  , "dAccTime      " , UsrPara.dAccTime       );
    UserINI.Load(sPath, "UsrPara"  , "bUseRate      " , UsrPara.bUseRate       );
    UserINI.Load(sPath, "UsrPara"  , "dAcceleration " , UsrPara.dAcceleration  ); //Update.... exptional
    UserINI.Load(sPath, "UsrPara"  , "dDeceleration " , UsrPara.dDeceleration  );
    UserINI.Load(sPath, "UsrPara"  , "dRunSpeed     " , UsrPara.dRunSpeed      );
    UserINI.Load(sPath, "UsrPara"  , "dSlowSpeed    " , UsrPara.dSlowSpeed     );
    UserINI.Load(sPath, "UsrPara"  , "dManSpeed     " , UsrPara.dManSpeed      );
    UserINI.Load(sPath, "UsrPara"  , "dHomeSpeed    " , UsrPara.dHomeSpeed     );
    UserINI.Load(sPath, "UsrPara"  , "dLastHomeSpeed" , UsrPara.dLastHomeSpeed );
    UserINI.Load(sPath, "UsrPara"  , "dJogSpeed     " , UsrPara.dJogSpeed      );
    UserINI.Load(sPath, "UsrPara"  , "dInposition   " , UsrPara.dInposition    );
    UserINI.Load(sPath, "UsrPara"  , "dHomeOffset   " , UsrPara.dHomeOffset    );

    UserINI.Load(sPath, "RptPara"  , "dFstPos       " , RptPara.dFstPos        );
    UserINI.Load(sPath, "RptPara"  , "dScdPos       " , RptPara.dScdPos        );
    UserINI.Load(sPath, "RptPara"  , "dStopDelay    " , RptPara.dStopDelay     );

    //AXT SUB UI LOAD
    if(AjinMotion) {
        AjinMotion->Load();
        AjinMotion->SetUseAccRate(UsrPara.bUseRate);
    }
}


void CMotor::Update()
{
    //Position
    Stat.dCmdPos = GetCmdPos();
    Stat.dEncPos = GetEncPos();

    //If Jog On , set target to current command.
    if (Stat.bJogN || Stat.bJogP) {
        Stat.dTrgPos = Stat.dCmdPos;
    }

    //Servo.
    Stat.bServoOn     = GetServo   ();
    Stat.bHomeSnsr    = GetHomeSnsr();

    //Get Mechanical Signal.
    Stat.bStop        = GetStop() ;
    Stat.bPackInPosn  = Para.bExistEnc ? GetPackInPos() : GetStop();
    Stat.bAlarm       = GetAlarm     () ; //CFS20set_alarm_enable      (AjinPara.iPhysicalNo, AjinPara.bAlarmEnable  ); Check!!!!!!!!!!
    Stat.bPEndLimSnsr = GetPLimSnsr  () ;
    Stat.bNEndLimSnsr = GetNLimSnsr  () ;
    Stat.bZphase      = GetZphaseSnsr() ;
    Stat.bBreakOff    = IO.GetY(Para.iBreakAdd) ;
    //Stat.bOk          =!Stat.bPEndLimSnsr && !Stat.bNEndLimSnsr && !Stat.bAlarm && Stat.bHomeEnded && Stat.bServoOn ;  //OK for Equip running
    //Stat.bReady       = Stat.bOk && Stat.bPackInPosn && Stat.bStop;                                                        //OK for Move Again.
    Stat.bOk          =!Stat.bAlarm &&  Stat.bServoOn ;  //OK for Equip running
    Stat.bReady       = Stat.bOk && !Stat.bPEndLimSnsr && !Stat.bNEndLimSnsr && Stat.bHomeEnded ;                                                        //OK for Move Again.

    //If Servo is off, Set target position to current position.
    if (!Stat.bOk) Stat.dTrgPos = Stat.dEncPos;



    //Check Error.
    if ( Stat.bAlarm  ) { if(!GetStop())Stop(); Stat.bHomeEnded = false; }
    if (!Stat.bServoOn) { if(!Para.bExistEnc)   Stat.bHomeEnded = false; }

    //Check Limit Software Pos.
    if(Para.bExistEnc) {
        if (Para.dMaxPos && (Stat.dEncPos > Para.dMaxPos + UsrPara.dInposition) && Stat.bServoOn && Stat.bHomeEnded && !Stat.bJogN) {
            if(!GetStop()) {
                Stop();
                Trace("Motr Stop","Para.dMaxPos && (Stat.dEncPos > Para.dMaxPos + UsrPara.dInposition) && Stat.bServoOn && Stat.bHomeEnded && !Stat.bJogN");
            }
        }
        if (                (Stat.dEncPos < Para.dMinPos - UsrPara.dInposition) && Stat.bServoOn && Stat.bHomeEnded && !Stat.bJogP) {
            if(!GetStop()) {
                Stop();
                Trace("Motr Stop","                (Stat.dEncPos < Para.dMinPos - UsrPara.dInposition) && Stat.bServoOn && Stat.bHomeEnded && !Stat.bJogP");
            }
        }
    }
    else {
        if (Para.dMaxPos && (Stat.dCmdPos > Para.dMaxPos + UsrPara.dInposition) && Stat.bServoOn && Stat.bHomeEnded && !Stat.bJogN) {Stop(); Trace("Motr Stop","Para.dMaxPos && (Stat.dCmdPos > Para.dMaxPos + UsrPara.dInposition) && Stat.bServoOn && Stat.bHomeEnded && !Stat.bJogN");}
        if (                (Stat.dCmdPos < Para.dMinPos - UsrPara.dInposition) && Stat.bServoOn && Stat.bHomeEnded && !Stat.bJogP) {Stop(); Trace("Motr Stop","                (Stat.dCmdPos < Para.dMinPos - UsrPara.dInposition) && Stat.bServoOn && Stat.bHomeEnded && !Stat.bJogP");}
    }

    //Check Limit Sensor.
    //Ȩ���ܿ��� ��� �Ǹ� Z�� ������ ���� ġ�� ������ Ȩ�� ������.
    if (Stat.bPEndLimSnsr && Stat.bServoOn && !Stat.bJogN && !HomeStat.iHomeStep){
         //
         if(!GetStop()){
             EmgStop();
             Trace(((AnsiString)Para.sMotorName + " Axis").c_str() , "P EndLim Sensor Touched");
         }
    }

    if (Stat.bNEndLimSnsr && Stat.bServoOn && !Stat.bJogP && !HomeStat.iHomeStep) {
         //
         if(!GetStop()){
             EmgStop();
             Trace(((AnsiString)Para.sMotorName + " Axis").c_str() , "N EndLim Sensor Touched");
         }
    }

    //HomeStep.
         if (HomeStat.iHomeStep ) CycleHome();    
//         if (HomeStat.iHomeStep && !m_bSubHome ) CycleHome();
//    else if (HomeStat.iHomeStep &&  m_bSubHome ) CycleHomeSub();

    if (RptStat.bGoRpt    ) CycleRpt ();
}

// ���� ���� ������ �������� ��찡 ������ �۾�ȿ���� �Ϲ����� ��� ��� �߷� ���� �ϰڴ�.
// 1. - h + : ���� �̻����� ���� home������ Ȩ��´�. - h ���� ���̿� ���װ� ���ͼ� - h ���̿� ������ �Ѵ� ��� ���� �ʴ� ��찡 ������ �ִ�.
//                                                    ���Ͱ� �ⱸ������ �׻� - + ���̿� �ֵ��� �����ؾ���.
//
// 2. -   + :                    -   ������ Ȩ��´�.  ���Ͱ� �ⱸ������ �׻� - + ���̿� �ֵ��� �����ؾ���.
// 3. h   + :                    home������ Ȩ��´�.  ���Ͱ� �ⱸ������ �׻� h + ���̿� �ֵ��� �����ؾ���.
// 4. h     :                    home������ Ȩ��´�.  ������ ���� ����ó�� �Ұ���.

//CFS20start_signal_search1(INT16 axis, double velocity, double acceleration, UINT8 detect_signal);
//UINT8 detect_signal ���� ����.
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

//������ �ϰ�...������� ����.
bool CMotor::CycleHomeSub()
{
    //Check Status.
    if ( Stat.bAlarm   ) {Trace(Para.sMotorName.c_str() , ("CycleHome  Stat.bAlarm    return "+(AnsiString)HomeStat.iHomeStep).c_str() ); HomeStat.iHomeStep = 0 ; return false;}
    if (!Stat.bServoOn ) {Trace(Para.sMotorName.c_str() , ("CycleHome !Stat.bServoOn  return "+(AnsiString)HomeStat.iHomeStep).c_str() ); HomeStat.iHomeStep = 0 ; return false;}

    //Local Var.
    const int MIN_PULSE = 500    ;
    const int MAX_PULSE = 100000 ;

    double dMinUnit  = MIN_PULSE / PULSE_PER_UNIT ;
    double dMaxUnit  = MAX_PULSE / PULSE_PER_UNIT ;
    double dVel      = UsrPara.dHomeSpeed         ;
    double dLastVel  = UsrPara.dLastHomeSpeed     ;
    double dAcc      = UsrPara.dAcceleration      ;
    double dDec      = UsrPara.dDeceleration      ;

    bool  bSensorStat ; //home sensor stat. (home or -limit)
    bool  bNLimSnsr  = GetNLimSnsr();

    //For Ajin
    bool   bUseZphase ; //Ȩ���� �ϰ� Z���� ã�� ���.
    CAjinMotion::EN_FINDEDGE_TYPE ftSnsrUpEdge ;
    CAjinMotion::EN_FINDEDGE_TYPE ftSnsrDnEdge ;
    CAjinMotion::EN_FINDEDGE_TYPE ftZPhsUpEdge ;
    CAjinMotion::EN_FINDEDGE_TYPE ftZPhsDnEdge ;
    switch(Para.iHomeMethod) {
        case hmOrigin    :  //Ȩ���� ������ �������忡�� ���� ������ �ȵǼ� AjinMotion Ŭ�������� ����(CycleHome) �ؾ� �Ѵ�.
            bSensorStat  = Stat.bHomeSnsr ;
            ftSnsrUpEdge = CAjinMotion::ftOrgUpEdge ;
            ftSnsrDnEdge = CAjinMotion::ftOrgDnEdge ;
            ftZPhsUpEdge = CAjinMotion::ftZphUpEdge ;
            ftZPhsDnEdge = CAjinMotion::ftZphDnEdge ;
            bUseZphase   = false ;
            break ;
        case hmNegative  :  //-���Լ����� �������忡�� ���� ���� ����
            bSensorStat = Stat.bNEndLimSnsr      ;
            ftSnsrUpEdge = CAjinMotion::ftNELUpEdge ;
            ftSnsrDnEdge = CAjinMotion::ftNELDnEdge ;
            ftZPhsUpEdge = CAjinMotion::ftZphUpEdge ;
            ftZPhsDnEdge = CAjinMotion::ftZphDnEdge ;
            bUseZphase   = false ;
            break ;
        case hmOriginZ   :
            bSensorStat = Stat.bHomeSnsr ;
            ftSnsrUpEdge = CAjinMotion::ftOrgUpEdge ;
            ftSnsrDnEdge = CAjinMotion::ftOrgDnEdge ;
            ftZPhsUpEdge = CAjinMotion::ftZphUpEdge ;
            ftZPhsDnEdge = CAjinMotion::ftZphDnEdge ;
            bUseZphase   = true ;
            break ;
        case hmNegativeZ :
            bSensorStat = Stat.bNEndLimSnsr      ;
            ftSnsrUpEdge = CAjinMotion::ftNELUpEdge ;
            ftSnsrDnEdge = CAjinMotion::ftNELDnEdge ;
            ftZPhsUpEdge = CAjinMotion::ftZphUpEdge ;
            ftZPhsDnEdge = CAjinMotion::ftZphDnEdge ;
            bUseZphase   = true ;
            break ;
        case hmZ :
            bSensorStat = Stat.bZphase ;
            ftSnsrUpEdge = CAjinMotion::ftZphUpEdge ;
            ftSnsrDnEdge = CAjinMotion::ftZphDnEdge ;
            ftZPhsUpEdge = CAjinMotion::ftZphUpEdge ; //�ʿ� ����.
            ftZPhsDnEdge = CAjinMotion::ftZphDnEdge ; //�ʿ� ����.
            bUseZphase   = false ; //Ȩ������ Z������ ���� �ϹǷ� Z�� ã�� ������ ���Ѵ�.
    }



    //Check Limit.
//    if(dVel < dMinUnit) {dVel = dMinUnit ; Trace(Para.sMotorName.c_str() , "dVel < dMinUnit"); }
//    if(dVel > dMaxUnit) {dVel = dMaxUnit ; Trace(Para.sMotorName.c_str() , "dVel > dMaxUnit"); }

    if(UsrPara.bUseRate) {dVel = PULSE_PER_UNIT * dVel ;dAcc = dAcc * PULSE_PER_UNIT; dDec = dDec * PULSE_PER_UNIT;}
    else                 {dVel = PULSE_PER_UNIT * dVel ;}


    // �����˻� ����� �����ϴ� �Լ�
    long lAxisNo = AjinMotion->AjinPara.iPhysicalNo; // ���� �˻��� �̷���� �� ��ȣ
    long nHmDir = 0; // �����˻� 1�ܰ��� �̵�����, (-)������ �Է�
    DWORD uHomeSignal = CAjinMotion::ftZphDnEdge; // ��ȣ�˻����������� ��ȣ���� ���
    DWORD uZphas = Para.iHomeMethod == hmZ ; // Z�� ���� ����, 1�̸� ���� �ϰ�, 0�̸� ����
    double dHomeClrTime = 2000.0; // ���� �˻� Enc �� Set�ϱ� ���� ���ð�
    double dHomeOffset = UsrPara.dHomeOffset * PULSE_PER_UNIT; // �����˻��� Offset����ŭ �߰������� �̵�

    AxmHomeSetMethod(lAxisNo,nHmDir,uHomeSignal,uZphas,dHomeClrTime,dHomeOffset);

    // �����˻��� ���� �ӵ��� ����
    double dVelFirst = dVel; // 1�ܰ迡���� �ӵ�
    double dVelSecond = dLastVel; // 2�ܰ迡���� �ӵ�
    double dVelThird  = dLastVel; // 3�ܰ迡���� �ӵ�
    double dvelLast   = dLastVel; // index�˻��� �����ϰ� �˻��ϱ����� �ӵ� (Offset�� �̵��� ����)
    double dAccFirst  = UsrPara.dAcceleration; // 1�ܰ迡���� ���ӵ�
    double dAccSecond = UsrPara.dDeceleration; // 2�ܰ迡���� ���ӵ�
    AxmHomeSetVel(lAxisNo, dVelFirst, dVelSecond, dVelThird, dvelLast, dAccFirst, dAccSecond);


    AnsiString sTemp ;
    static DWORD uHomeResult;
    DWORD uHomeMainStepNumber, uHomeStepNumber;
    sTemp = sTemp.sprintf("HomeStat.iHomeStep=%02d" , HomeStat.iHomeStep );
    if (HomeStat.iPreHomeStep != HomeStat.iHomeStep) {
        Trace(Para.sMotorName.c_str(),sTemp.c_str());
    }

//    if (m_tmHomeCycle.OnDelay( HomeStat.iHomeStep == HomeStat.iPreHomeStep && uHomeResult != HOME_SEARCHING , 5000 )) { //���� ���ܿ� �ӹ��� �ְ� ���Ͱ� �������� ������ 5�ʸ� Ÿ�Ӿƿ�.
//        HomeStat.iHomeStep    = 0     ;
//        HomeStat.iPreHomeStep = 0     ;
//        HomeStat.bHomeTimeout = true  ;
//        Stop();
//        Trace(Para.sMotorName.c_str() , ("Home Time Out"+(AnsiString)HomeStat.iHomeStep).c_str() );
//        return false;
//    }

    HomeStat.iPreHomeStep = HomeStat.iHomeStep ;

    //static int iReturnCnt = 0 ;

    switch (HomeStat.iHomeStep) {
        default :
            return true ;

        //Ȩ������ Ȩ�� ��� ����. //������ ���ݴ� �Ȱ� �� �������� ���� �Ƕ�.dd
        case 10 :
            if(!GetStop()) return false ;
            // 0���� �����˻��� �����Ѵ�.
            AxmHomeSetStart(lAxisNo);

// 0���� Ȩ�˻� ������� Ȯ���Ѵ�.
//long lAxisNo = 0;
//DWORD uHomeMainStepNumber, uHomeStepNumber;
//AxmHomeGetRate(lAxisNo, &uHomeMainStepNumber , &uHomeStepNumber);
//printf(������ ����� : %d ��, uHomeStepNumber);
// uHomeMainStepNumber�� ��Ʈ���� ��� ������,�����̺갡 �����ϴ��� Ȯ���� �� �̿��Ѵ�. // �������� ������ ���� 0�� ����, �����̺굿���� ���� 10�� �����Ѵ�.

            HomeStat.iHomeStep++ ;
            return false ;

        case 11 : //AxmHomeGetRate(lAxisNo, &uHomeMainStepNumber , &uHomeStepNumber);
            if(!GetStop()){
                return false ;
            }
            // �����˻��� ����� ��ȯ�Ѵ�.
            AxmHomeGetResult(lAxisNo, &uHomeResult);
            if(uHomeResult == HOME_SUCCESS) {
                HomeStat.iHomeStep    = 0 ;
                HomeStat.iPreHomeStep = 0 ;
                Stat.bHomeEnded   = true ;
                Trace(Para.sMotorName.c_str(),"Home Cycle Finished");
                return true ;
            }
            else return false ;
    }
}
bool CMotor::CycleHome()
{
    //Check Status.
    if ( Stat.bAlarm   ) {Trace(Para.sMotorName.c_str() , ("CycleHome  Stat.bAlarm    return "+(AnsiString)HomeStat.iHomeStep).c_str() ); HomeStat.iHomeStep = 0 ; return false;}
    if (!Stat.bServoOn ) {Trace(Para.sMotorName.c_str() , ("CycleHome !Stat.bServoOn  return "+(AnsiString)HomeStat.iHomeStep).c_str() ); HomeStat.iHomeStep = 0 ; return false;}

    //Local Var.
    const int MIN_PULSE = 500    ;
    const int MAX_PULSE = 100000 ;

    double dMinUnit  = MIN_PULSE / PULSE_PER_UNIT ;
    double dMaxUnit  = MAX_PULSE / PULSE_PER_UNIT ;
    double dVel      = UsrPara.dHomeSpeed         ;
    double dLastVel  = UsrPara.dLastHomeSpeed     ;
    double dAcc      = UsrPara.dAcceleration      ;
    double dDec      = UsrPara.dDeceleration      ;

    bool  bSensorStat ; //home sensor stat. (home or -limit)
    bool  bNLimSnsr  = GetNLimSnsr();

    bool bRet ;



    //For Ajin
    bool   bUseZphase ; //Ȩ���� �ϰ� Z���� ã�� ���.
    CAjinMotion::EN_FINDEDGE_TYPE ftSnsrUpEdge ;
    CAjinMotion::EN_FINDEDGE_TYPE ftSnsrDnEdge ;
    CAjinMotion::EN_FINDEDGE_TYPE ftZPhsUpEdge ;
    CAjinMotion::EN_FINDEDGE_TYPE ftZPhsDnEdge ;
    switch(Para.iHomeMethod) {
        case hmOrigin    :  //Ȩ���� ������ �������忡�� ���� ������ �ȵǼ� AjinMotion Ŭ�������� ����(CycleHome) �ؾ� �Ѵ�.
            bSensorStat  = Stat.bHomeSnsr ;
            ftSnsrUpEdge = CAjinMotion::ftOrgUpEdge ;
            ftSnsrDnEdge = CAjinMotion::ftOrgDnEdge ;
            ftZPhsUpEdge = CAjinMotion::ftZphUpEdge ;
            ftZPhsDnEdge = CAjinMotion::ftZphDnEdge ;
            bUseZphase   = false ;
            break ;
        case hmNegative  :  //-���Լ����� �������忡�� ���� ���� ����
            bSensorStat = Stat.bNEndLimSnsr      ;
            ftSnsrUpEdge = CAjinMotion::ftNELUpEdge ;
            ftSnsrDnEdge = CAjinMotion::ftNELDnEdge ;
            ftZPhsUpEdge = CAjinMotion::ftZphUpEdge ;
            ftZPhsDnEdge = CAjinMotion::ftZphDnEdge ;
            bUseZphase   = false ;
            break ;
        case hmOriginZ   :
            bSensorStat = Stat.bHomeSnsr ;
            ftSnsrUpEdge = CAjinMotion::ftOrgUpEdge ;
            ftSnsrDnEdge = CAjinMotion::ftOrgDnEdge ;
            ftZPhsUpEdge = CAjinMotion::ftZphUpEdge ;
            ftZPhsDnEdge = CAjinMotion::ftZphDnEdge ;
            bUseZphase   = true ;
            break ;
        case hmNegativeZ :
            bSensorStat = Stat.bNEndLimSnsr      ;
            ftSnsrUpEdge = CAjinMotion::ftNELUpEdge ;
            ftSnsrDnEdge = CAjinMotion::ftNELDnEdge ;
            ftZPhsUpEdge = CAjinMotion::ftZphUpEdge ;
            ftZPhsDnEdge = CAjinMotion::ftZphDnEdge ;
            bUseZphase   = true ;
            break ;
        case hmZ :
            bSensorStat = Stat.bZphase ;
            ftSnsrUpEdge = CAjinMotion::ftZphUpEdge ;
            ftSnsrDnEdge = CAjinMotion::ftZphDnEdge ;
            ftZPhsUpEdge = CAjinMotion::ftZphUpEdge ; //�ʿ� ����.
            ftZPhsDnEdge = CAjinMotion::ftZphDnEdge ; //�ʿ� ����.
            bUseZphase   = false ; //Ȩ������ Z������ ���� �ϹǷ� Z�� ã�� ������ ���Ѵ�.
    }


    //Check Limit.
    if(dVel < dMinUnit) {dVel = dMinUnit ; Trace(Para.sMotorName.c_str() , "dVel < dMinUnit"); }
    if(dVel > dMaxUnit) {dVel = dMaxUnit ; Trace(Para.sMotorName.c_str() , "dVel > dMaxUnit"); }

    AnsiString sTemp ;
    sTemp = sTemp.sprintf("HomeStat.iHomeStep=%02d" , HomeStat.iHomeStep );
    if (HomeStat.iPreHomeStep != HomeStat.iHomeStep) {
        Trace(Para.sMotorName.c_str(),sTemp.c_str());
    }

    if (m_tmHomeCycle.OnDelay( HomeStat.iHomeStep == HomeStat.iPreHomeStep && GetStop(), 5000 )) { //���� ���ܿ� �ӹ��� �ְ� ���Ͱ� �������� ������ 5�ʸ� Ÿ�Ӿƿ�.
        HomeStat.iHomeStep    = 0     ;
        HomeStat.iPreHomeStep = 0     ;
        HomeStat.bHomeTimeout = true  ;
        Stop();
        Trace(Para.sMotorName.c_str() , ("Home Time Out"+(AnsiString)HomeStat.iHomeStep).c_str() );
        return false;
    }

    HomeStat.iPreHomeStep = HomeStat.iHomeStep ;


    if(GetCmdPos() > 260) m_dPosGap = 360.0 ;



    switch (HomeStat.iHomeStep) {
        default :
            return true ;

        //Ȩ������ Ȩ�� ��� ����.
        case 10 :
            if(!GetStop()) return false ;
            if(bSensorStat) HomeStat.iHomeStep = 15 ; //Ȩ�� ������ ���� �ǰ� �ִ°��.
            else            HomeStat.iHomeStep = 11 ; //Ȩ�� ������ ���� ���� �ʰ� �ִ� ���.

            m_dPosGap = 0.0 ;

            m_iReturnCnt = 0 ;
            return false ;

        case 11 :
            HomeStat.iHomeStep++ ;
            return false ;


        case 12 :
            if(bNLimSnsr) {
                Stop();
                HomeStat.iHomeStep++ ;
                return false ;
            } //Ȩ������ -������ ���̿� ���װ� ��ġ�� ���¿��� Ȩ������.
            FindEdgeStop(-1 * dVel , dAcc , ftSnsrUpEdge);
            //
            //
            //

            //


            //FindEdgeStop(-1 * dVel , dAcc , ftSnsrDnEdge);
            HomeStat.iHomeStep++ ;
            return false ;

        case 13 :
            //if(GetStop()) {
                //m_iReturnCnt ++;
                //return false ; //���̺��� T�� ���� ������....

            //}

            HomeStat.iHomeStep++ ;
            return false ;



        case 14 :
            if((Para.iHomeMethod == hmOrigin || Para.iHomeMethod == hmOriginZ || Para.iHomeMethod == hmZ) && bNLimSnsr){ //Ȩ���� �������ٰ� -���� ģ���.
                Stop();
            }
            if(!GetStop()){
                return false ;
            }
            HomeStat.iHomeStep++;
            return false ;

        case 15 :
            HomeStat.iHomeStep++ ;
            return false ;

        case 16 :
            FindEdgeStop(dVel , dAcc , ftSnsrDnEdge);
            HomeStat.iHomeStep++ ;
            return false ;
        case 17 :
            HomeStat.iHomeStep++ ;
            return false ;

        case 18 :
            if(!GetStop()) return false ;
            if(bUseZphase) FindEdgeStop(     dVel , dAcc , ftZPhsDnEdge);
            else           FindEdgeStop(-1 * dVel , dAcc , ftSnsrUpEdge);   //#define LAST_HOME_VEL_RATIO  5.0  //START_STOP_SPEED ���� ������ Ȩ�� ������ �ȵ�.
            HomeStat.iHomeStep++ ;
            return false ;

        case 19 :
            HomeStat.iHomeStep++ ;
            return false ;

        case 20 :
            if(!GetStop()) return false ;
            if(bUseZphase) bRet = FindEdgeEmgStop(-1 * dLastVel , dAcc  , ftZPhsUpEdge);
            else           bRet = FindEdgeEmgStop(     dLastVel , dAcc  , ftSnsrDnEdge);
            //���������� Ȩ���� �ҵ�� �������� CFS20start_signal_search1_ex�Լ��� ��ü�ؾ���.
            HomeStat.iHomeStep++ ;
            return false ;

        case 21 :
            HomeStat.iHomeStep++ ;
            return false ;

        case 22 :
            //if(!`()) return false;
            if(!GetStop()) return false ;
            HomeStat.iHomeStep++ ;
            return false ;

        case 23 :
            HomeStat.iHomeStep++ ;
            return false ;

        case 24 :






            SetPos(0.0);
            //�̤�
            if(m_bSubHome && m_dPosGap != 0.0) GoHomeOffset(UsrPara.dHomeOffset - m_dPosGap, dVel );
            else                               GoHomeOffset(UsrPara.dHomeOffset , dVel );
            HomeStat.iHomeStep++ ;
            return false ;

        case 25 :
            HomeStat.iHomeStep++ ;
            return false ;

        case 26 :
            if(!GetStop()) return false ;
            //�̤� �����....
            if(m_bSubHome && m_dPosGap != 0.0) {
                HomeStat.iHomeStep = 10;
                return false ;
            }



            m_tmTemp.Clear();
            HomeStat.iHomeStep++ ;
            return false ;

        case 27 :
            if(!m_tmTemp.OnDelay(true , 1000)) return false;
            SetPos(0.0);
            HomeStat.iHomeStep    = 0 ;
            HomeStat.iPreHomeStep = 0 ;
            Stat.bHomeEnded   = true ;
            Trace(Para.sMotorName.c_str(),"Home Cycle Finished");
            return true ;


    }
}

void CMotor::CycleRpt()
{
    static CDelayTimer RptTimer ;
    static bool        bFstPos  = false ;
    static bool        bTickTimeCheck = false;
    static double      dPreRptTickTime ;
           double      dCurTick        ;
    //Check Status.
    if(!Stat.bOk) {Trace(Para.sMotorName.c_str() , ("CycleRpt !Stat.bOk return "+(AnsiString)HomeStat.iHomeStep).c_str() ); return ; }

    if(bTickTimeCheck != bFstPos) {
        bTickTimeCheck = bFstPos;
        dCurTick = Now();
    }
    //Changing Timer.
    if(GetStop()) {
        RptPara.dTickTime = dCurTick - dPreRptTickTime;
        dPreRptTickTime = dCurTick;
    }

    if(RptTimer .OnDelay(GetStop() , RptPara.dStopDelay )) {
        if(bFstPos) {GoAbsRun(RptPara.dScdPos) ; bFstPos = false ; }
        else        {GoAbsRun(RptPara.dFstPos) ; bFstPos = true  ; }
    }
}

void CMotor::SetAjinTrgPos(int _iPosCnt , double * _dPos , double _dTrgTime , bool _bActual, bool _bOnLevel)
{
    if(Para.iMotionType != mtAjin) return ;


    double * dTrgPos = new double[_iPosCnt] ;

    for(int i = 0 ; i < _iPosCnt ; i++) {
        dTrgPos[i] = _dPos[i] * PULSE_PER_UNIT ;
    }

    //if(AjinMotion)
    AjinMotion -> SetTrgPos(_iPosCnt , dTrgPos , _dTrgTime , _bActual, _bOnLevel) ;

    delete [] dTrgPos ;
}
void CMotor::ResetAjinTrgPos()
{
    if(Para.iMotionType != mtAjin) return ;

    AjinMotion -> ResetTrgPos();
}

void CMotor::OneShotAjinTrg (bool _bUpEg , int _iTime)
{
    if(Para.iMotionType != mtAjin) return ;

    AjinMotion -> OneShotTrg(_bUpEg,_iTime);
}


void CMotor::SetIntrptPos(double _dPos, PosCallbackFunc _pCallback)
{
    if(Para.iMotionType != mtAjin) return ;

    AjinMotion -> SetIntrptPos(_dPos * PULSE_PER_UNIT , _pCallback) ;
}

void CMotor::ResetIntrpt()
{
    if(Para.iMotionType != mtAjin) return ;

    AjinMotion -> ResetIntrpt() ;

}



void CMotor::UpdatePara(bool         _bToTable        ,  //SUB UI ���÷��� OR ����.)
                        TEdit      * _edMotorName     ,
                        TComboBox  * _cbMotionType    ,
                        TEdit      * _edPulsePerRev   ,
                        TEdit      * _edUnitPerRev    ,
                        TEdit      * _edMaxPos        ,
                        TEdit      * _edMinPos        ,
                        TCheckBox  * _cbExistEnc      ,
                        TComboBox  * _cbHomeMethod    ,
                        TCheckBox  * _cbSrvOnLevelInv ,
                        TEdit      * _edBreakAdd      ,
                        TComboBox  * _cbDirType       ,
                        TCheckBox  * _cbSrvBreakType  )
{
     if(_bToTable) {
         if(_edMotorName    ) _edMotorName     -> Text      = Para.sMotorName     ;
         if(_cbMotionType   ) _cbMotionType    -> ItemIndex = Para.iMotionType    ;
         if(_edPulsePerRev  ) _edPulsePerRev   -> Text      = Para.iPulsePerRev   ;
         if(_edUnitPerRev   ) _edUnitPerRev    -> Text      = Para.dUnitPerRev    ;
         if(_edMaxPos       ) _edMaxPos        -> Text      = Para.dMaxPos        ;
         if(_edMinPos       ) _edMinPos        -> Text      = Para.dMinPos        ;
         if(_cbExistEnc     ) _cbExistEnc      -> Checked   = Para.bExistEnc      ;
         if(_cbMotionType   ) _cbHomeMethod    -> ItemIndex = Para.iHomeMethod    ;
         if(_cbSrvOnLevelInv) _cbSrvOnLevelInv -> Checked   = Para.bSrvOnLevelInv ;
         if(_edBreakAdd     ) _edBreakAdd      -> Text      = Para.iBreakAdd      ;
         if(_cbDirType      ) _cbDirType       -> ItemIndex = Para.iDirType       ;
         if(_cbSrvBreakType ) _cbSrvBreakType  -> Checked   = Para.bSrvBreakType  ;
     }

     else {
         if(_edMotorName    ) Para.sMotorName     = _edMotorName     -> Text            ;
         if(_cbMotionType   ) Para.iMotionType    = _cbMotionType    -> ItemIndex       ;
         if(_edPulsePerRev  ) Para.iPulsePerRev   = _edPulsePerRev   -> Text.ToInt()    ;
         if(_edUnitPerRev   ) Para.dUnitPerRev    = _edUnitPerRev    -> Text.ToDouble() ;
         if(_edMaxPos       ) Para.dMaxPos        = _edMaxPos        -> Text.ToDouble() ;
         if(_edMinPos       ) Para.dMinPos        = _edMinPos        -> Text.ToDouble() ;
         if(_cbExistEnc     ) Para.bExistEnc      = _cbExistEnc      -> Checked         ;
         if(_cbHomeMethod   ) Para.iHomeMethod    = _cbHomeMethod    -> ItemIndex       ;
         if(_cbSrvOnLevelInv) Para.bSrvOnLevelInv = _cbSrvOnLevelInv -> Checked         ;
         if(_edBreakAdd     ) Para.iBreakAdd      = _edBreakAdd      -> Text.ToInt()    ;
         if(_cbDirType      ) Para.iDirType       = _cbDirType       -> ItemIndex       ;
         if(_cbSrvBreakType ) Para.bSrvBreakType  = _cbSrvBreakType  -> Checked         ;
     }
    //AjinMotion UI ������Ʈ .
    if(AjinMotion) AjinMotion->UpdateAjinPara(_bToTable);
}

void CMotor::UpdateStat(TLabel *_lbServoOn     ,
                        TLabel *_lbHomeSnsr    ,
                        TLabel *_lbPEndLimSnsr ,
                        TLabel *_lbNEndLimSnsr ,
                        TLabel *_lbHomeEnded   ,
                        TLabel *_lbAlarm       ,

                        TLabel *_lbPackInPosn  ,
                        TLabel *_lbStop        ,
                        TLabel *_lbReady       ,
                        TLabel *_lbOk          ,
                        TLabel *_lbJogP        ,
                        TLabel *_lbJogN        ,
                        TLabel *_lbTrgPos      ,
                        TLabel *_lbCmdPos      ,
                        TLabel *_lbEncPos      ,
                        TLabel *_lbZphase      ,
                        TLabel *_lbBreakOff    )
{
    if(_lbServoOn    ){_lbServoOn     -> Color   = Stat    .bServoOn     ? clLime : clSilver ;}
    if(_lbHomeSnsr   ){_lbHomeSnsr    -> Color   = Stat    .bHomeSnsr    ? clLime : clSilver ;}
    if(_lbPEndLimSnsr){_lbPEndLimSnsr -> Color   = Stat    .bPEndLimSnsr ? clLime : clSilver ;}
    if(_lbNEndLimSnsr){_lbNEndLimSnsr -> Color   = Stat    .bNEndLimSnsr ? clLime : clSilver ;}
    if(_lbHomeEnded  ){_lbHomeEnded   -> Color   = Stat    .bHomeEnded   ? clLime : clSilver ;}
    if(_lbAlarm      ){_lbAlarm       -> Color   = Stat    .bAlarm       ? clLime : clSilver ;}
    if(_lbEncPos     ){_lbEncPos      -> Caption = Stat    .dEncPos                          ;}
    if(_lbPackInPosn ){_lbPackInPosn  -> Color   = Stat    .bPackInPosn  ? clLime : clSilver ;}
    if(_lbStop       ){_lbStop        -> Color   = Stat    .bStop        ? clLime : clSilver ;}
    if(_lbReady      ){_lbReady       -> Color   = Stat    .bReady       ? clLime : clSilver ;}
    if(_lbOk         ){_lbOk          -> Color   = Stat    .bOk          ? clLime : clSilver ;}
    if(_lbJogP       ){_lbJogP        -> Color   = Stat    .bJogP        ? clLime : clSilver ;}
    if(_lbJogN       ){_lbJogN        -> Color   = Stat    .bJogN        ? clLime : clSilver ;}
    if(_lbTrgPos     ){_lbTrgPos      -> Caption = Stat    .dTrgPos                          ;}
    if(_lbCmdPos     ){_lbCmdPos      -> Caption = Stat    .dCmdPos                          ;}
    if(_lbZphase     ){_lbZphase      -> Color   = Stat    .bZphase      ? clLime : clSilver ;}
    if(_lbBreakOff   ){_lbBreakOff    -> Color   = Stat    .bBreakOff    ? clLime : clSilver ;}

}

void CMotor::UpdateUsrPara(
                           bool        _bToTable         ,
                           TCheckBox * _cbUseRate        ,  //�������� ���..
                           TEdit     * _edAcceleration   ,  //�����ӽð�.
                           TEdit     * _edDeceleration   ,  //�����ӽð�.
                           TEdit     * _edRunSpeed       ,  //�����ӵ�
                           TEdit     * _edSlowSpeed      ,  //�����ӵ�2
                           TEdit     * _edManSpeed       ,  //�޴��� �����ӵ�
                           TEdit     * _edHomeSpeed      ,  //Ȩ���� �ӵ�.
                           TEdit     * _edLastHomeSpeed  ,  //��ƮȨ���� �ӵ�.
                           TEdit     * _edJogSpeed       ,  //���ױ��� �ӵ�.
                           TEdit     * _edInposition     ,  //��������.
                           TEdit     * _edHomeOffset     )  //Ȩ �����V.

{
     if(_bToTable) {
         if(_cbUseRate       ) _cbUseRate        -> Checked = UsrPara.bUseRate       ;
         if(_edAcceleration  ) _edAcceleration   -> Text    = UsrPara.dAcceleration  ;
         if(_edDeceleration  ) _edDeceleration   -> Text    = UsrPara.dDeceleration  ;
         if(_edRunSpeed      ) _edRunSpeed       -> Text    = UsrPara.dRunSpeed      ;
         if(_edSlowSpeed     ) _edSlowSpeed      -> Text    = UsrPara.dSlowSpeed     ;
         if(_edManSpeed      ) _edManSpeed       -> Text    = UsrPara.dManSpeed      ;
         if(_edHomeSpeed     ) _edHomeSpeed      -> Text    = UsrPara.dHomeSpeed     ;
         if(_edLastHomeSpeed ) _edLastHomeSpeed  -> Text    = UsrPara.dLastHomeSpeed ;
         if(_edJogSpeed      ) _edJogSpeed       -> Text    = UsrPara.dJogSpeed      ;
         if(_edInposition    ) _edInposition     -> Text    = UsrPara.dInposition    ;
         if(_edHomeOffset    ) _edHomeOffset     -> Text    = UsrPara.dHomeOffset    ;
     }

     else {
         if(_cbUseRate       ) UsrPara.bUseRate        = _cbUseRate        -> Checked         ;
         if(_edAcceleration  ) UsrPara.dAcceleration   = _edAcceleration   -> Text.ToDouble() ;
         if(_edDeceleration  ) UsrPara.dDeceleration   = _edDeceleration   -> Text.ToDouble() ;
         if(_edRunSpeed      ) UsrPara.dRunSpeed       = _edRunSpeed       -> Text.ToDouble() ;
         if(_edSlowSpeed     ) UsrPara.dSlowSpeed      = _edSlowSpeed      -> Text.ToDouble() ;
         if(_edManSpeed      ) UsrPara.dManSpeed       = _edManSpeed       -> Text.ToDouble() ;
         if(_edHomeSpeed     ) UsrPara.dHomeSpeed      = _edHomeSpeed      -> Text.ToDouble() ;
         if(_edLastHomeSpeed ) UsrPara.dLastHomeSpeed  = _edLastHomeSpeed  -> Text.ToDouble() ;
         if(_edJogSpeed      ) UsrPara.dJogSpeed       = _edJogSpeed       -> Text.ToDouble() ;
         if(_edInposition    ) UsrPara.dInposition     = _edInposition     -> Text.ToDouble() ;
         if(_edHomeOffset    ) UsrPara.dHomeOffset     = _edHomeOffset     -> Text.ToDouble() ;
     }
}

void CMotor::UpdateRptPara(   //Repeat Test Para (UI)
                           bool     _bToTable    ,
                           TEdit *  _edFstPos    ,  //ù��ġ
                           TEdit *  _edScdPos    ,  //�ι�°��ġ.
                           TEdit *  _edStopDelay )  //��ǵ� �� ���� ������.
{
     if(_bToTable) {
         if(_edFstPos   ) _edFstPos    -> Text = RptPara.dFstPos    ;
         if(_edScdPos   ) _edScdPos    -> Text = RptPara.dScdPos    ;
         if(_edStopDelay) _edStopDelay -> Text = RptPara.dStopDelay ;
     }

     else {
         if(_edFstPos   ) RptPara.dFstPos    = _edFstPos    -> Text.ToDouble() ;
         if(_edScdPos   ) RptPara.dScdPos    = _edScdPos    -> Text.ToDouble() ;
         if(_edStopDelay) RptPara.dStopDelay = _edStopDelay -> Text.ToDouble() ;
     }
}

double CMotor::GetRptTickTime()
{
    return RptPara.dTickTime ;
}
