//---------------------------------------------------------------------------
#pragma hdrstop
#include "Motors.h"

//System header

//User Header
#include "UserINI.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)


//---------------------------------------------------------------------------
CMotors MT;

CMotors::CMotors()
{
    LoadDnmVar();

    AxtFS20 = new CAxtFS20[m_iMaxMotor] ;

    memset(AxtFS20 , 0 , sizeof(CAxtFS20)*m_iMaxMotor);

    //Load();                 n


}

CMotors::~CMotors()
{
    delete [] AxtFS20 ;
    AxtFS20 = NULL ;
}

//Thread Cycle Func.
void CMotors::Update ()
{
    for(int i = 0 ; i < m_iMaxMotor ; i++)
        AxtFS20[i].Update() ;
}

//Set Stat Directly Func.
void CMotors::SetHomeEnd(int _iAxisNo , bool _bValue)
{
    _NG_RANGE_VOID_

    AxtFS20[_iAxisNo].SetHomeEnd(_bValue);
}

bool CMotors::GetHomeEnd(int _iAxisNo)
{
    _NG_RANGE_BOOL_

    return AxtFS20[_iAxisNo].GetHomeEnd() ;
}

bool CMotors::GetHomeEndAll()
{
    for(int i = 0 ; i < m_iMaxMotor ; i++) {
        if(!AxtFS20[i].GetHomeEnd()) return false ;
    }

    return true ;
}

void CMotors::SetServo(int _iAxisNo , bool _bOn)
{
    _NG_RANGE_VOID_

    AxtFS20[_iAxisNo].SetServo(_bOn);
}

bool CMotors::GetServo(int _iAxisNo)
{
    _NG_RANGE_BOOL_

    return AxtFS20[_iAxisNo].GetServo();
}

void CMotors::SetAlarm(int _iAxisNo , bool _bOn)
{
    _NG_RANGE_VOID_

    AxtFS20[_iAxisNo].SetAlarm(_bOn);
}

bool CMotors::GetAlarm(int _iAxisNo )
{
    _NG_RANGE_BOOL_

    return AxtFS20[_iAxisNo].GetAlarm();
}
bool CMotors::GetPLim(int _iAxisNo )
{
    _NG_RANGE_BOOL_

    return AxtFS20[_iAxisNo].GetPLim() ;
}

bool CMotors::GetNLim(int _iAxisNo )
{
    _NG_RANGE_BOOL_

    return AxtFS20[_iAxisNo].GetPLim() ;
}

//Update Motor Status.
bool CMotors::GetStopInpos(int _iAxisNo) //Check Motion Done with User Inposition.
{
    _NG_RANGE_BOOL_

    return AxtFS20[_iAxisNo].GetStopInpos();
}

bool CMotors::GetReady(int _iAxisNo) //Check ready to move.
{
    _NG_RANGE_BOOL_

    return AxtFS20[_iAxisNo].GetReady();
}

bool CMotors::GetOk(int _iAxisNo) //Check No Limit & Servo Enable.
{
    _NG_RANGE_BOOL_

    return AxtFS20[_iAxisNo].GetOk();
}

//Position Functions.
double CMotors::GetTrgPos(int _iAxisNo ) //Get Target  Position.
{
    _NG_RANGE_DOUBLE_

    return AxtFS20[_iAxisNo].GetTrgPos();
}

double CMotors::GetCmdPos(int _iAxisNo ) //Get Command Position.
{
    _NG_RANGE_DOUBLE_

    return AxtFS20[_iAxisNo].GetCmdPos();
}
double CMotors::GetEncPos(int _iAxisNo ) //Get Encoder Position.
{
    _NG_RANGE_DOUBLE_

    return AxtFS20[_iAxisNo].GetEncPos();
}

double CMotors::GetErrPos(int _iAxisNo ) //Get Control Error Position.
{
    _NG_RANGE_DOUBLE_

    return AxtFS20[_iAxisNo].GetErrPos();
}

void CMotors::ClearPos(int _iAxisNo ) //Clear Position.
{
    _NG_RANGE_VOID_

    AxtFS20[_iAxisNo].ClearPos();
}

void CMotors::SetPos(int _iAxisNo , double _dPos) //Set Any Position.
{
    _NG_RANGE_VOID_

    AxtFS20[_iAxisNo].SetPos(_dPos);
}

bool CMotors::CmprPos(int _iAxisNo , double _dPos , double _dRange)
{
    _NG_RANGE_BOOL_

    return AxtFS20[_iAxisNo].CmprPos( _dPos , _dRange) ;
}

//Motion Functions.
void CMotors::Reset(int _iAxisNo) //Reset
{
    _NG_RANGE_VOID_

    AxtFS20[_iAxisNo].Reset();
}

void CMotors::ResetAll()
{
    for(int i = 0 ; i < m_iMaxMotor ; i++) {
        AxtFS20[i].Reset();
    }
}

bool CMotors::Stop(int _iAxisNo)
{
    _NG_RANGE_BOOL_

    return AxtFS20[_iAxisNo].Stop();
}

bool CMotors::EmgStop(int _iAxisNo) //Stop Without Deceleration.
{
    _NG_RANGE_BOOL_

    return AxtFS20[_iAxisNo].EmgStop();
}

bool CMotors::Home(int _iAxisNo) //Start Home Cycle for Sequense Cycle.
{
    _NG_RANGE_BOOL_

    return AxtFS20[_iAxisNo].Home();
}
void CMotors::DoHome(int _iAxisNo) //Start Home Cycle for Click Event.
{
    _NG_RANGE_VOID_

    AxtFS20[_iAxisNo].DoHome();
}

bool CMotors::Vibration(int _iAxisNo , double _dRange , double _dVel , int _iCnt , bool _bCntClr)
{
    _NG_RANGE_BOOL_

    return AxtFS20[_iAxisNo].Vibration(_dRange , _dVel , _iCnt , _bCntClr) ;
}

void CMotors::DoRpt(int _iAxisNo) //Start Home Cycle for Click Event.
{
    _NG_RANGE_VOID_

    AxtFS20[_iAxisNo].DoRpt();
}

bool CMotors::JogP(int _iAxisNo) //Jogging to CW.
{
    _NG_RANGE_BOOL_

    return AxtFS20[_iAxisNo].JogP();
}

bool CMotors::JogN(int _iAxisNo) //Jogging to CCW.
{
    _NG_RANGE_BOOL_

    return AxtFS20[_iAxisNo].JogN();
}

bool CMotors::GoAbs(int _iAxisNo , double _dPos , double _dVel) //abs move
{
    _NG_RANGE_BOOL_

    return AxtFS20[_iAxisNo].GoAbs(_dPos , _dVel);
}

void CMotors::GoInc(int _iAxisNo , double _dPos , double _dVel) //Inc move  모션돈 확인은 GetStopInpos() 함수로.
{
    _NG_RANGE_VOID_

    AxtFS20[_iAxisNo].GoInc(_dPos , _dVel);
}

bool CMotors::GoAbsRun(int _iAxisNo , double _dPos ) //abs move with AutoRun Vel.
{
    _NG_RANGE_BOOL_

    return AxtFS20[_iAxisNo].GoAbsRun(_dPos);
}
void CMotors::GoIncRun(int _iAxisNo , double _dPos ) //Inc move with AutoRun ManualVel. 모션돈 확인은 GetStopInpos() 함수로.
{
    _NG_RANGE_VOID_

    return AxtFS20[_iAxisNo].GoIncRun(_dPos);
}

bool CMotors::GoAbsMan(int _iAxisNo , double _dPos ) //abs move with Manual Vel.
{
    _NG_RANGE_BOOL_

    return AxtFS20[_iAxisNo].GoAbsMan(_dPos);
}

void CMotors::GoIncMan(int _iAxisNo , double _dPos ) //Inc move with Manual Vel. 모션돈 확인은 GetStopInpos() 함수로.
{
    _NG_RANGE_VOID_

    AxtFS20[_iAxisNo].GoIncMan(_dPos);
}

AnsiString CMotors::GetName(int _iAxisNo)
{
    _NG_RANGE_STRING_

    return AxtFS20[_iAxisNo].GetName();
}

//Save Para Functions.
void CMotors::Save()
{
    for( int i = 0 ; i < m_iMaxMotor ; i++) AxtFS20[i].Save(i) ;
}

void CMotors::Load()
{
    for( int i = 0 ; i < m_iMaxMotor ; i++)
        AxtFS20[i].Load(i) ;

    ApplyPara();
}

void CMotors::ApplyPara()
{
    for( int i = 0 ; i < m_iMaxMotor ; i++) AxtFS20[i].ApplyAxtPara() ;
}





void CMotors::SaveDnmVar()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "Util\\DmnVar.INI" ;

    //Load Device.
    UserINI.Save(sPath , "CMotors"  , "m_iMaxMotor" , m_iMaxMotor);
}

void CMotors::LoadDnmVar()
{
    //Local Var.
    TUserINI   UserINI;
    AnsiString sPath  ;

    //Set Dir.
    sPath = EXE_FOLDER + "Util\\DmnVar.INI" ;

    //Load Device.
    UserINI.Load(sPath , "CMotors"  , "m_iMaxMotor" , m_iMaxMotor);
}

void CMotors::GetMotorList(TComboBox * _cbName)
{
    for(int i = 0 ; i < m_iMaxMotor ; i++) {
        _cbName -> AddItem(AxtFS20[i].GetName(),NULL) ;
    }

    _cbName -> ItemIndex = 0 ;
}

//UI Handler
void CMotors::UpdateStat(int     _iAxisNo       ,
                         TLabel *_lbServoOn     ,
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
    _NG_RANGE_VOID_

    AxtFS20[_iAxisNo].UpdateStat(_lbServoOn     ,
                                 _lbHomeSnsr    ,
                                 _lbPEndLimSnsr ,
                                 _lbNEndLimSnsr ,
                                 _lbHomeEnded   ,
                                 _lbAlarm       ,
                                 _lbEncPos      ,
                                 _lbPackInPosn  ,
                                 _lbStop        ,
                                 _lbBusy        ,
                                 _lbReady       ,
                                 _lbOk          ,
                                 _lbJogP        ,
                                 _lbJogN        ,
                                 _lbTrgPos      ,
                                 _lbCmdPos      );
}

void CMotors::UpdatePara(int     _iAxisNo      ,
                         bool    _bToTable     ,
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
    _NG_RANGE_VOID_

    AxtFS20[_iAxisNo].UpdatePara(_bToTable     ,
                                 _edAccTime    ,
                                 _edRunSpeed   ,
                                 _edManSpeed   ,
                                 _edHomeSpeed  ,
                                 _edJogSpeed   ,
                                 _edInposition ,
                                 _edHomeOffset ,

                                 _edFstPos     ,
                                 _edScdPos     ,
                                 _edStopDelay  );
}

