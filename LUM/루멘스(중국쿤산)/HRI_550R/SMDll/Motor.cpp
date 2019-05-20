//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <math.h>
#include <StdCtrls.hpp>

//---------------------------------------------------------------------------
#include "UserINI.h"
#include "SLogUnit.h"
#include "Motor.h"
#include "IOs.h"
#include "FormMsgOk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#define PULSE_PER_UNIT (Para.iPulsePerRev/Para.dUnitPerRev)

void SetUI (TPanel * _pnBase)
{
    SetAxtUIParent(_pnBase);
}

//---CMotorBase Unit-------------------------------------------------------------
CMotor::CMotor(int _iAxisNo)
{
    m_iAxisNo = _iAxisNo ;

    AxtFS20 = NULL ;

    memset(&Para     , 0 , sizeof(SPara     ));
    memset(&UsrPara  , 0 , sizeof(SUsrPara  ));
    memset(&Stat     , 0 , sizeof(SStat     ));
    memset(&HomeStat , 0 , sizeof(SHomeStat ));
    memset(&RptPara  , 0 , sizeof(SRptPara  ));
    memset(&RptStat  , 0 , sizeof(SRptStat  ));

    AxtFS20 = new CAxtFS20(m_iAxisNo) ;

    Load();

    //SetServo(true) ;

}

CMotor::~CMotor()
{
    //SetServo(false) ;
    delete AxtFS20 ;
}

AnsiString CMotor::GetName()
{
    //if(Para.sMotorName =="") return "" ;
    return Para.sMotorName;
}

int CMotor::GetPhyscAdd()
{
    CAxtFS20::SAxtPara Para ;
    AxtFS20->GetAxtPara (Para);

    return Para.iPhysicalNo ;
}

bool CMotor::GetStop()
{
    return AxtFS20 -> GetStop() ;
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
    Stop();

    //Servo On/Off.
    if (Para.bSrvOnLevelInv) AxtFS20 -> SetServo(!_bOn) ;
    else                     AxtFS20 -> SetServo( _bOn) ;

    SetBreakOff(_bOn);

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
    bRet = AxtFS20 -> GetServo() ;

    if (Para.bSrvOnLevelInv) bRet = !AxtFS20 -> GetServo() ;
    else                     bRet =  AxtFS20 -> GetServo() ;

    return bRet ;
}

void CMotor::SetReset(bool _bOn)
{
    AxtFS20 -> SetReset(_bOn) ;
}

bool CMotor::GetAlarm()
{
    bool bRet = AxtFS20 -> GetAlarm() ;
    if(bRet && GetServo()) SetServo(false);
    return bRet ;
}

bool CMotor::GetHomeSnsr()
{
    return AxtFS20 -> GetHomeSnsr() ;
}

bool CMotor::GetNLimSnsr()
{
    return AxtFS20 -> GetNLimSnsr() ;
}

bool CMotor::GetPLimSnsr()
{
    return AxtFS20 -> GetPLimSnsr() ;
}
bool CMotor::GetZphaseSnsr()
{
    return AxtFS20 -> GetZphaseSnsr() ;
}
bool CMotor::GetPackInPos()
{
    return AxtFS20 -> GetPackInPos() ;
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

    if (fabs(GetCmdPos() - GetEncPos()) > dInp) return false;

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

    dCmdPos  = AxtFS20 -> GetCmdPos();
    dMotrPos = dCmdPos / PULSE_PER_UNIT;

    return dMotrPos  ;
}

double CMotor::GetEncPos()
{
    if (PULSE_PER_UNIT <= 0) return 0;

    double dEncPos  ;
    double dMotrPos ;

    //encPos = CFS20get_actual_position (Para.iModulPos); //Servo - Actual Position.

    if (Para.bExistEnc)	dEncPos = AxtFS20 -> GetEncPos(); //Servo , Easy Servo - Actual Position.
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

    AxtFS20 -> SetPos(_dPos);
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
        AxtFS20 -> SetReset(true) ;
        ClearPos();
        Sleep(10);
        AxtFS20 -> SetReset(false) ;
    }

    //clear AxtFS20 -> SetReset(false) ;
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
    AxtFS20 -> Stop() ;
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
    AxtFS20 -> EmgStop() ;
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
    Stat.bHomeEnded   = false ;
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
    double dVel  = UsrPara.dJogSpeed ;
    double dAcc  = UsrPara.dAccTime  ;

    //Set Jog Status.
    Stat.bJogP = true;

    //Go Jog.
    if (dVel <   0.1) dVel =   0.1;
    if (dVel > 100.0) dVel = 100.0;


    double i = (Para.iPulsePerRev/Para.dUnitPerRev) * dVel;

    Trace(Para.sMotorName.c_str() , "Jog Positive");

    return AxtFS20 -> JogP(PULSE_PER_UNIT * dVel , dAcc);
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
    double dVel  = UsrPara.dJogSpeed ;
    double dAcc  = UsrPara.dAccTime  ;

    //Set Jog Status.
    Stat.bJogN = true;

    //Go Jog.
    if (dVel <   0.1) dVel =   0.1;
    if (dVel > 100.0) dVel = 100.0;

    Trace(Para.sMotorName.c_str() , "Jog Negative");

    return AxtFS20 -> JogN(PULSE_PER_UNIT * dVel , dAcc);
}

bool CMotor::GoHomeOffset (double _dPos , double _dVel)
{
    //return
    if (!HomeStat.iHomeStep ) { Trace(Para.sMotorName.c_str() , "GoHomeOffset !HomeStat.iHomeStep return "); return false;}

    //Check Motor Limit.
    if (_dPos > Para.dMaxPos) { Stop(); Trace(Para.sMotorName.c_str() , "GoHomeOffset Para.dMaxPos return "); return false; }
    if (_dPos < Para.dMinPos) { Stop(); Trace(Para.sMotorName.c_str() , "GoHomeOffset Para.dMinPos return "); return false; }

    //Local Var.
    int iReceive = 0 ;

    //Go Motor.
    if (GetStop()) {
        Trace(Para.sMotorName.c_str() , ("GoHomeOffset to " + AnsiString(_dPos)).c_str());
        iReceive = AxtFS20 -> GoAbs(_dPos * PULSE_PER_UNIT , _dVel * PULSE_PER_UNIT , UsrPara.dAccTime );
    }

    //Define Target Position.
    Stat.dTrgPos = _dPos;

    //Check InPos.
    bool bRet = GetStopInpos();
    return bRet ;
}

bool CMotor::GoAbs(double _dPos , double _dVel)
{
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
        iReceive = AxtFS20 -> GoAbs(_dPos * PULSE_PER_UNIT , _dVel * PULSE_PER_UNIT , UsrPara.dAccTime );
        Trace(Para.sMotorName.c_str() , ("Move to " + AnsiString(_dPos)).c_str());
    }
    else {
        if (Stat.dTrgPos != _dPos) {//Position Override.
            if (fabs(_dPos - Stat.dTrgPos) > UsrPara.dInposition) {
                Trace(Para.sMotorName.c_str() , ("Position Override Move to " + AnsiString(_dPos)).c_str());
                iReceive = AxtFS20 -> GoOvr(_dPos * PULSE_PER_UNIT); //���� ���������� ���� ���� ���۰��� �ڵ������� ����.
            }
        }
    }

    //Define Target Position.
    Stat.dTrgPos = _dPos;

    //Check InPos.
    bool bRet = GetStopInpos();

    return bRet ;
}

void CMotor::GoInc(double _dPos , double _dVel )
{
    //Local Var.
    double dCmdPos = GetCmdPos();

    //Go Relative.
    GoAbs(_dPos + dCmdPos , _dVel);
}

bool CMotor::GoAbsRun (double _dPos )
{
    return GoAbs(_dPos , UsrPara.dRunSpeed );
}

void CMotor::GoIncRun (double _dPos )
{
    GoInc(_dPos , UsrPara.dRunSpeed );
}

bool CMotor::GoAbsMan (double _dPos )
{
    return GoAbs(_dPos , UsrPara.dManSpeed );
}
void CMotor::GoIncMan (double _dPos )
{
    GoInc(_dPos , UsrPara.dManSpeed );
}

bool CMotor::FindEdgeStop (double _dVel ,double _dAcc ,CAxtFS20::EN_FINDEDGE_TYPE _ftFindEdgeType)
{
    return AxtFS20 -> FindEdgeStop(_dVel * PULSE_PER_UNIT, _dAcc , _ftFindEdgeType);
}

bool CMotor::FindEdgeEmgStop(double _dVel ,CAxtFS20::EN_FINDEDGE_TYPE _ftFindEdgeType)
{
    return AxtFS20 -> FindEdgeEmgStop(_dVel * PULSE_PER_UNIT, _ftFindEdgeType);
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

    UserINI.Save(sPath, "UsrPara"  , "dAccTime      " , UsrPara.dAccTime       );
    UserINI.Save(sPath, "UsrPara"  , "dRunSpeed     " , UsrPara.dRunSpeed      );
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
    if(AxtFS20) AxtFS20->Save();
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
    UserINI.Load(sPath, "Para   "  , "dUnitPerRev   " , Para   .dUnitPerRev    );
    UserINI.Load(sPath, "Para   "  , "dMaxPos       " , Para   .dMaxPos        );
    UserINI.Load(sPath, "Para   "  , "dMinPos       " , Para   .dMinPos        );
    UserINI.Load(sPath, "Para   "  , "bExistEnc     " , Para   .bExistEnc      );
    UserINI.Load(sPath, "Para   "  , "iHomeMethod   " , Para   .iHomeMethod    );
//   UserINI.Load(sPath, "Para   "  , "bHomeWithNSnsr" , Para   .bHomeWithNSnsr );
    UserINI.Load(sPath, "Para   "  , "bSrvOnLevelInv" , Para   .bSrvOnLevelInv );
    UserINI.Load(sPath, "Para   "  , "iBreakAdd     " , Para   .iBreakAdd      );

    UserINI.Load(sPath, "UsrPara"  , "dAccTime      " , UsrPara.dAccTime       );
    UserINI.Load(sPath, "UsrPara"  , "dRunSpeed     " , UsrPara.dRunSpeed      );
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
    if(AxtFS20) AxtFS20->Load();
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
    if(_lbHomeEnded  ){_lbHomeEnded   -> Color   = Stat.bHomeEnded   ? clLime : clSilver ;}
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

void CMotor::UpdatePara(
                        bool         _bToTable        ,
                        TEdit      * _edMotorName     ,
                        TComboBox  * _cbMotionType    ,
                        TEdit      * _edPulsePerRev   ,
                        TEdit      * _edUnitPerRev    ,
                        TEdit      * _edMaxPos        ,
                        TEdit      * _edMinPos        ,
                        TCheckBox  * _cbExistEnc      ,
                        TComboBox  * _cbHomeMethod    ,
//                        TCheckBox  * _cbHomeWithNSnsr ,
                        TCheckBox  * _cbSrvOnLevelInv ,
                        TEdit      * _edBreakAdd      )  //SUB UI ���÷��� OR ����.)
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
         //         if(_cbHomeWithNSnsr) _cbHomeWithNSnsr -> Checked   = Para.bHomeWithNSnsr ;
         if(_cbSrvOnLevelInv) _cbSrvOnLevelInv -> Checked   = Para.bSrvOnLevelInv ;
         if(_edBreakAdd     ) _edBreakAdd      -> Text      = Para.iBreakAdd      ;
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
//         if(_cbHomeWithNSnsr) Para.bHomeWithNSnsr = _cbHomeWithNSnsr -> Checked         ;
         if(_cbSrvOnLevelInv) Para.bSrvOnLevelInv = _cbSrvOnLevelInv -> Checked         ;
         if(_edBreakAdd     ) Para.iBreakAdd      = _edBreakAdd      -> Text.ToInt()    ;
     }
    //AxtFS20 UI ������Ʈ .
    if(AxtFS20) AxtFS20->UpdateAxtPara(_bToTable);
}

void CMotor::UpdateUsrPara(
                           bool     _bToTable         ,
                           TEdit *  _edAccTime        ,  //�����ӽð�.
                           TEdit *  _edRunSpeed       ,  //�����ӵ�
                           TEdit *  _edManSpeed       ,  //�޴��� �����ӵ�
                           TEdit *  _edHomeSpeed      ,  //Ȩ���� �ӵ�.
                           TEdit *  _edLastHomeSpeed  ,  //��ƮȨ���� �ӵ�.
                           TEdit *  _edJogSpeed       ,  //���ױ��� �ӵ�.
                           TEdit *  _edInposition     ,  //��������.
                           TEdit *  _edHomeOffset     )  //Ȩ �����V.

{
     if(_bToTable) {
         if(_edAccTime       ) _edAccTime        -> Text = UsrPara.dAccTime        ;
         if(_edRunSpeed      ) _edRunSpeed       -> Text = UsrPara.dRunSpeed       ;
         if(_edManSpeed      ) _edManSpeed       -> Text = UsrPara.dManSpeed       ;
         if(_edHomeSpeed     ) _edHomeSpeed      -> Text = UsrPara.dHomeSpeed      ;
         if(_edLastHomeSpeed ) _edLastHomeSpeed  -> Text = UsrPara.dLastHomeSpeed  ;
         if(_edJogSpeed      ) _edJogSpeed       -> Text = UsrPara.dJogSpeed       ;
         if(_edInposition    ) _edInposition     -> Text = UsrPara.dInposition     ;
         if(_edHomeOffset    ) _edHomeOffset     -> Text = UsrPara.dHomeOffset     ;
     }

     else {
         if(_edAccTime       ) UsrPara.dAccTime        = _edAccTime        -> Text.ToDouble() ;
         if(_edRunSpeed      ) UsrPara.dRunSpeed       = _edRunSpeed       -> Text.ToDouble() ;
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
    Stat.bAlarm       = GetAlarm     () ; //CFS20set_alarm_enable      (AxtPara.iPhysicalNo, AxtPara.bAlarmEnable  ); Check!!!!!!!!!!
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
    if (HomeStat.iHomeStep) CycleHome();
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
    double dAcc      = UsrPara.dAccTime           ;

    bool  bSensorStat ; //home sensor stat. (home or -limit)
    bool  bNLimSnsr  = GetNLimSnsr();

    //For Ajin
    bool   bUseZphase ;
    CAxtFS20::EN_FINDEDGE_TYPE ftSnsrUpEdge ;
    CAxtFS20::EN_FINDEDGE_TYPE ftSnsrDnEdge ;
    CAxtFS20::EN_FINDEDGE_TYPE ftZPhsUpEdge ;
    CAxtFS20::EN_FINDEDGE_TYPE ftZPhsDnEdge ;
    switch(Para.iHomeMethod) {
        case hmOrigin    :  //Ȩ���� ������ �������忡�� ���� ������ �ȵǼ� AxtFS20 Ŭ�������� ����(CycleHome) �ؾ� �Ѵ�.
            bSensorStat  = Stat.bHomeSnsr ;
            ftSnsrUpEdge = CAxtFS20::ftOrgUpEdge ;
            ftSnsrDnEdge = CAxtFS20::ftOrgDnEdge ;
            ftZPhsUpEdge = CAxtFS20::ftZphUpEdge ;
            ftZPhsDnEdge = CAxtFS20::ftZphDnEdge ;
            bUseZphase   = false ;
            break ;
        case hmNegative  :  //-���Լ����� �������忡�� ���� ���� ����
            bSensorStat = Stat.bNEndLimSnsr      ;
            ftSnsrUpEdge = CAxtFS20::ftNELUpEdge ;
            ftSnsrDnEdge = CAxtFS20::ftNELDnEdge ;
            ftZPhsUpEdge = CAxtFS20::ftZphUpEdge ;
            ftZPhsDnEdge = CAxtFS20::ftZphDnEdge ;
            bUseZphase   = false ;
            break ;
        case hmOriginZ   :
            bSensorStat = Stat.bHomeSnsr ;
            ftSnsrUpEdge = CAxtFS20::ftOrgUpEdge ;
            ftSnsrDnEdge = CAxtFS20::ftOrgDnEdge ;
            ftZPhsUpEdge = CAxtFS20::ftZphUpEdge ;
            ftZPhsDnEdge = CAxtFS20::ftZphDnEdge ;
            bUseZphase   = true ;
            break ;
        case hmNegativeZ :
            bSensorStat = Stat.bNEndLimSnsr      ;
            ftSnsrUpEdge = CAxtFS20::ftNELUpEdge ;
            ftSnsrDnEdge = CAxtFS20::ftNELDnEdge ;
            ftZPhsUpEdge = CAxtFS20::ftZphUpEdge ;
            ftZPhsDnEdge = CAxtFS20::ftZphDnEdge ;
            bUseZphase   = true ;
            break ;
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

    switch (HomeStat.iHomeStep) {
        default :
            return true ;

        //Ȩ������ Ȩ�� ��� ����.
        case 10 :
            if(!GetStop()) return false ;
            if(bSensorStat) HomeStat.iHomeStep = 12 ; //Ȩ�� ������ ���� �ǰ� �ִ°��.
            else            HomeStat.iHomeStep = 11 ; //Ȩ�� ������ ���� ���� �ʰ� �ִ� ���.
            return false ;

        case 11 :
            if(bNLimSnsr) {
                Stop();
                HomeStat.iHomeStep++ ;
                return false ;
            } //Ȩ������ -������ ���̿� ���װ� ��ġ�� ���¿��� Ȩ������.
            FindEdgeStop( -1 * dVel , dAcc , ftSnsrUpEdge);
            HomeStat.iHomeStep++ ;
            return false ;

        case 12 :
            if((Para.iHomeMethod == hmOrigin || Para.iHomeMethod == hmOriginZ) && bNLimSnsr){
                Stop();
            }
            if(!GetStop()){
                return false ;
            }
            HomeStat.iHomeStep++;
            return false ;

        case 13 :
            FindEdgeStop(dVel , dAcc , ftSnsrDnEdge);
            HomeStat.iHomeStep++ ;
            return false ;

        case 14 :
            if(!GetStop()) return false ;
            if(bUseZphase) FindEdgeStop(     dVel , dAcc , ftZPhsDnEdge);
            else           FindEdgeStop(-1 * dVel , dAcc , ftSnsrUpEdge);   //#define LAST_HOME_VEL_RATIO  5.0  //START_STOP_SPEED ���� ������ Ȩ�� ������ �ȵ�.
            HomeStat.iHomeStep++ ;
            return false ;

        case 15 :
            if(!GetStop()) return false ;
            if(bUseZphase) FindEdgeEmgStop(-1 * dLastVel , ftZPhsUpEdge);
            else           FindEdgeEmgStop(     dLastVel , ftSnsrDnEdge);
            //���������� Ȩ���� �ҵ�� �������� CFS20start_signal_search1_ex�Լ��� ��ü�ؾ���.
            HomeStat.iHomeStep++ ;
            return false ;

        case 16 :
            //if(!`()) return false;
            if(!GetStop()) return false ;
            HomeStat.iHomeStep++ ;
            return false ;

        case 17 :
            SetPos(0.0);
            GoHomeOffset(UsrPara.dHomeOffset , dVel );
            HomeStat.iHomeStep++ ;
            return false ;

        case 18 :
            if(!GetStop()) return false ;
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

    //Check Status.
    if(!Stat.bOk) {Trace(Para.sMotorName.c_str() , ("CycleRpt !Stat.bOk return "+(AnsiString)HomeStat.iHomeStep).c_str() ); return ; }

    //Changing Timer.
    if(RptTimer .OnDelay(GetStop() , RptPara.dStopDelay )) {
        if(bFstPos) {GoAbsRun(RptPara.dScdPos) ; bFstPos = false ; }
        else        {GoAbsRun(RptPara.dFstPos) ; bFstPos = true  ; }
    }
}

void CMotor::SetAxtTrgPos(int _iPosCnt , double * _dPos , double _dTrgTime , bool _bActual, bool _bOnLevel)
{
    if(Para.iMotionType != mtAxtFS20) return ;


    double * dTrgPos = new double[_iPosCnt] ;

    for(int i = 0 ; i < _iPosCnt ; i++) {
        dTrgPos[i] = _dPos[i] * PULSE_PER_UNIT ;
    }

    //if(AxtFS20)
    AxtFS20 -> SetTrgPos(_iPosCnt , dTrgPos , _dTrgTime , _bActual, _bOnLevel) ;

    delete [] dTrgPos ;
}
void CMotor::ResetAxtTrgPos()
{
    if(Para.iMotionType != mtAxtFS20) return ;

    AxtFS20 -> ResetTrgPos();
}

void CMotor::OneShotAxtTrg (bool _bUpEg , int _iTime)
{
    if(Para.iMotionType != mtAxtFS20) return ;

    AxtFS20 -> OneShotTrg(_bUpEg,_iTime);
}

