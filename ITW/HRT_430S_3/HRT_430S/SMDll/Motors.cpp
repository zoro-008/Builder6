//---------------------------------------------------------------------------
#pragma hdrstop
#include "Motors.h"

//System header

//User Header
#include "UserINI.h"
#include "FormMsgOk.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)


//---------------------------------------------------------------------------
CMotors MT;

CMotors::CMotors()
{
    LoadDnmVar();

    Motor = new CMotor * [m_iMaxMotor] ;

    static int a = 0;

    for(int i = 0 ; i < m_iMaxMotor ; i++) {


        Motor[i] = new CMotor(i) ;

        a ++ ;
    }
}

CMotors::~CMotors()
{
    for(int i = 0 ; i < m_iMaxMotor ; i++) {
        delete Motor[i] ;
        Motor[i] = NULL ;
    }
    delete [] Motor ;
    Motor = NULL ;
    Trace("","");
}

bool CMotors::ChangeMotorCnt(int _iMaxMotor)
{
    for(int i = 0 ; i < m_iMaxMotor ; i++) {
        delete Motor[i] ;
        Motor[i] = NULL ;
    }
    delete [] Motor ;

    m_iMaxMotor = _iMaxMotor ;

    SaveDnmVar();

    Motor = new CMotor * [m_iMaxMotor] ;

    for(int i = 0 ; i < m_iMaxMotor ; i++) {
        Motor[i] = new CMotor(i) ;
    }
    return true ;
}

//Thread Cycle Func.
void CMotors::Update ()
{
    for(int i = 0 ; i < m_iMaxMotor ; i++)
        Motor[i]->Update() ;
}

//Set Stat Directly Func.
void CMotors::SetHomeEnd(int _iAxisNo , bool _bValue)
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]->SetHomeEnd(_bValue);
}

bool CMotors::GetHomeEnd(int _iAxisNo)
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->GetHomeEnd() ;
}

bool CMotors::GetHomeEndAll()
{
    for(int i = 0 ; i < m_iMaxMotor ; i++) {
        if(!Motor[i]->GetHomeEnd()) return false ;
    }

    return true ;
}

void CMotors::SetServo(int _iAxisNo , bool _bOn)
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]->SetServo(_bOn);
}

bool CMotors::GetServo(int _iAxisNo)
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->GetServo();
}

bool CMotors::GetServoAll()
{
    for(int i = 0 ; i < m_iMaxMotor ; i++) if(!Motor[i]->GetServo()) return false ;

    return true ;

}

void CMotors::SetServoAll(bool _bOn)
{
    for(int i = 0 ; i < m_iMaxMotor ; i++) Motor[i]->SetServo(_bOn);
}

void CMotors::SetBreakOff(int _iAxisNo , bool _bOn)
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]->SetBreakOff(_bOn) ;
}

bool CMotors::GetBreakOff(int _iAxisNo )
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->GetBreakOff() ;
}

void CMotors::SetReset(int _iAxisNo , bool _bOn)
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]->SetReset(_bOn);
}

bool CMotors::GetAlarm(int _iAxisNo )
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->GetAlarm();
}
bool CMotors::GetHomeSnsr(int _iAxisNo )
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->GetHomeSnsr() ;
}
bool CMotors::GetPLimSnsr(int _iAxisNo )
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->GetPLimSnsr() ;
}

bool CMotors::GetNLimSnsr(int _iAxisNo )
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->GetNLimSnsr() ;
}

bool CMotors::GetZphaseSnsr(int _iAxisNo )
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->GetZphaseSnsr() ;
}

//Update Motor Status
bool CMotors::GetStopInpos(int _iAxisNo) //Check Motion Done with User Inposition
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->GetStopInpos();
}

bool CMotors::GetStop(int _iAxisNo)
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->GetStop();
}

bool CMotors::GetReady(int _iAxisNo) //Check ready to move.
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->GetReady();
}

bool CMotors::GetOk(int _iAxisNo) //Check No Limit & Servo Enable.
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->GetOk();
}

//Position Functions.
double CMotors::GetTrgPos(int _iAxisNo ) //Get Target  Position.
{
    _NG_RANGE_DOUBLE_

    return Motor[_iAxisNo]->GetTrgPos();
}

double CMotors::GetCmdPos(int _iAxisNo ) //Get Command Position.
{
    _NG_RANGE_DOUBLE_

    return Motor[_iAxisNo]->GetCmdPos();
}
double CMotors::GetEncPos(int _iAxisNo ) //Get Encoder Position.
{
    _NG_RANGE_DOUBLE_

    return Motor[_iAxisNo]->GetEncPos();
}

double CMotors::GetErrPos(int _iAxisNo ) //Get Control Error Position.
{
    _NG_RANGE_DOUBLE_

    return Motor[_iAxisNo]->GetErrPos();
}

void CMotors::ClearPos(int _iAxisNo ) //Clear Position.
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]->ClearPos();
}

void CMotors::SetPos(int _iAxisNo , double _dPos) //Set Any Position.
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]->SetPos(_dPos);
}

double CMotors::GetMinPos(int _iAxisNo )
{
    _NG_RANGE_DOUBLE_

    return Motor[_iAxisNo]->GetMinPos();
}

double CMotors::GetMaxPos(int _iAxisNo )
{
    _NG_RANGE_DOUBLE_

    return Motor[_iAxisNo]->GetMaxPos();
}

double CMotors::GetRunVel(int _iAxisNo )
{
    _NG_RANGE_DOUBLE_

    return Motor[_iAxisNo]->GetRunVel();
}

double CMotors::GetManVel(int _iAxisNo )
{
    _NG_RANGE_DOUBLE_

    return Motor[_iAxisNo]->GetManVel();
}

bool CMotors::GoAbsAcDc(int _iAxisNo , double _dPos , double _dVel , double _dAcc , double _dDcc)
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->GoAbsAcDc(_dPos , _dVel , _dAcc , _dDcc);

}
void CMotors::GoIncAcDc(int _iAxisNo , double _dPos , double _dVel , double _dAcc , double _dDcc)
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]->GoIncAcDc(_dPos , _dVel , _dAcc , _dDcc);
}

bool CMotors::CmprPos(int _iAxisNo , double _dPos , double _dRange)
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->CmprPos( _dPos , _dRange) ;
}

//Motion Functions.
void CMotors::Reset(int _iAxisNo) //Reset
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]->Reset();
}

void CMotors::ResetAll()
{
    for(int i = 0 ; i < m_iMaxMotor ; i++) {
        Motor[i]->Reset();
    }
}

void CMotors::Stop(int _iAxisNo)
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]->Stop();
}

void CMotors::StopAll()
{

    for(int i = 0 ; i < m_iMaxMotor ; i++) Motor[i]->Stop();
}

void CMotors::EmgStop(int _iAxisNo) //Stop Without Deceleration.
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]->EmgStop();
}

void CMotors::EmgStopAll() //Stop Without Deceleration.
{

    for(int i = 0 ; i < m_iMaxMotor ; i++) Motor[i]->EmgStop();
}

bool CMotors::Home(int _iAxisNo) //Start Home Cycle for Sequense Cycle.
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->Home();
}
void CMotors::DoHome(int _iAxisNo) //Start Home Cycle for Click Event.
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]->DoHome();
}

//void CMotors::DoRpt(int _iAxisNo) //Start Home Cycle for Click Event.
void CMotors::DoRpt(int _iAxisNo, double _edFirstPos, double _edSecondPos, int _edStopDelay) //Start Home Cycle for Click Event.
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]->DoRpt(_edFirstPos, _edSecondPos, _edStopDelay);
}

bool CMotors::JogP(int _iAxisNo) //Jogging to CW.
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->JogP();
}

bool CMotors::JogN(int _iAxisNo) //Jogging to CCW.
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->JogN();
}

bool CMotors::GoAbs(int _iAxisNo , double _dPos , double _dVel) //abs move
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->GoAbs(_dPos , _dVel);
}

void CMotors::GoInc(int _iAxisNo , double _dPos , double _dVel) //Inc move  모션돈 확인은 GetStopInpos() 함수로.
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]->GoInc(_dPos , _dVel);
}

bool CMotors::GoAbsRun(int _iAxisNo , double _dPos ) //abs move with AutoRun Vel.
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->GoAbsRun(_dPos);
}
void CMotors::GoIncRun(int _iAxisNo , double _dPos ) //Inc move with AutoRun ManualVel. 모션돈 확인은 GetStopInpos() 함수로.
{
    _NG_RANGE_VOID_

    return Motor[_iAxisNo]->GoIncRun(_dPos);
}

bool CMotors::GoAbsSlow(int _iAxisNo , double _dPos ) //abs move with AutoRun Vel.
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->GoAbsSlow(_dPos);
}
void CMotors::GoIncSlow(int _iAxisNo , double _dPos ) //Inc move with AutoRun ManualVel. 모션돈 확인은 GetStopInpos() 함수로.
{
    _NG_RANGE_VOID_

    return Motor[_iAxisNo]->GoIncSlow(_dPos);
}


bool CMotors::GoAbsMan(int _iAxisNo , double _dPos ) //abs move with Manual Vel.
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->GoAbsMan(_dPos);
}

void CMotors::GoIncMan(int _iAxisNo , double _dPos ) //Inc move with Manual Vel. 모션돈 확인은 GetStopInpos() 함수로.
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]->GoIncMan(_dPos);
    Trace("3","");
}

AnsiString CMotors::GetName(int _iAxisNo)
{
    _NG_RANGE_STRING_

    return Motor[_iAxisNo]->GetName();
}

//Save Para Functions.
void CMotors::Save()
{
    for( int i = 0 ; i < m_iMaxMotor ; i++) Motor[i]->Save() ;

    for( int i = 0 ; i < m_iMaxMotor ; i++) {
        for( int j = 0 ; j < m_iMaxMotor ; j++) {
            if(i!=j) {                 //나중에 모션모듈 추가 되면 모듈별로 옝외처리해야함.
                if(Motor[i]-> GetPhyscAdd() == Motor[j]->GetPhyscAdd()) {
                     MsgOk("Warning","Please Check Motor's Physical Addresses");
                }
            }
        }
    }
}

void CMotors::Save(int _iAxisNo)
{
    for( int i = 0 ; i < m_iMaxMotor ; i++) {
        if(i!=_iAxisNo) {                 //나중에 모션모듈 추가 되면 모듈별로 옝외처리해야함.
            if(Motor[i]-> GetPhyscAdd() == Motor[_iAxisNo]->GetPhyscAdd()) {
                 MsgOk("Warning","There is Same Physical Addresses.");
            }
        }
    }

    Motor[_iAxisNo]->Save() ;
}

void CMotors::Load()
{
    for( int i = 0 ; i < m_iMaxMotor ; i++) Motor[i]->Load() ;

    for( int i = 0 ; i < m_iMaxMotor ; i++) {
        for( int j = 0 ; j < m_iMaxMotor ; j++) {
            if(i!=j) {                 //나중에 모션모듈 추가 되면 모듈별로 옝외처리해야함.
                if(Motor[i]-> GetPhyscAdd() == Motor[j]->GetPhyscAdd()) {
                     MsgOk("Warning","Please Check Motor's Physical Addresses");
                }
            }
        }
    }

}

EN_MTOR_DIRECTION CMotors::GetDirType(int _iAxisNo)
{
    return Motor[_iAxisNo]->GetDirType() ;
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
        if(Motor[i]->GetName() =="") _cbName -> AddItem(AnsiString(i)+"_Axis",NULL) ;
        else                         _cbName -> AddItem(Motor[i]->GetName()  ,NULL) ;
    }
    _cbName -> ItemIndex = 0 ;
}

void CMotors::UpdateStat(int      _iAxisNo      ,
                         TLabel *_lbServoOn     ,
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
    _NG_RANGE_VOID_NO_MSG_

    Motor[_iAxisNo]-> UpdateStat(_lbServoOn     ,
                                 _lbHomeSnsr    ,
                                 _lbPEndLimSnsr ,
                                 _lbNEndLimSnsr ,
                                 _lbHomeEnded   ,
                                 _lbAlarm       ,

                                 _lbPackInPosn  ,
                                 _lbStop        ,
                                 _lbReady       ,
                                 _lbOk          ,
                                 _lbJogP        ,
                                 _lbJogN        ,
                                 _lbTrgPos      ,
                                 _lbCmdPos      ,
                                 _lbEncPos      ,
                                 _lbZphase      ,
                                 _lbBreakOff    );

}

void CMotors::UpdatePara(int          _iAxisNo         ,
                         bool         _bToTable        ,
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
                         TComboBox  * _cbDirType       )
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]-> UpdatePara(_bToTable        ,
                                 _edMotorName     ,
                                 _cbMotionType    ,
                                 _edPulsePerRev   ,
                                 _edUnitPerRev    ,
                                 _edMaxPos        ,
                                 _edMinPos        ,
                                 _cbExistEnc      ,
                                 _cbHomeMethod    ,
                                 _cbSrvOnLevelInv ,
                                 _edBreakAdd      ,
                                 _cbDirType       );
}

void CMotors::UpdateUsrPara(int         _iAxisNo          ,
                            bool        _bToTable         ,
                            TCheckBox * _cbUseRate        ,  //가속율 사용 유무.
                            TEdit     * _edAcceleration   ,  //가감속
                            TEdit     * _edDeceleration   ,  //가감속
                            TEdit     * _edRunSpeed       ,  //구동속도
                            TEdit     * _edSlowSpeed      ,  //구동속도2
                            TEdit     * _edManSpeed       ,  //메뉴얼 구동속도
                            TEdit     * _edHomeSpeed      ,  //홈구동 속도.
                            TEdit     * _edLastHomeSpeed  ,  //마지막 Z상 정지 속도.
                            TEdit     * _edJogSpeed       ,  //조그구동 속도.
                            TEdit     * _edInposition     ,  //인포지션.
                            TEdit     * _edHomeOffset     )  //홈 오프셑.
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]-> UpdateUsrPara(_bToTable        ,
                                    _cbUseRate       ,  //가속율 사용 유무.
                                    _edAcceleration  ,  //가감속
                                    _edDeceleration  ,  //가감속
                                    _edRunSpeed      ,  //구동속도
                                    _edSlowSpeed     ,  //구동속도2
                                    _edManSpeed      ,  //메뉴얼 구동속도
                                    _edHomeSpeed     ,  //홈구동 속도.
                                    _edLastHomeSpeed ,  //마지막 Z상 정지 속도.
                                    _edJogSpeed      ,  //조그구동 속도.
                                    _edInposition    ,  //인포지션.
                                    _edHomeOffset    );  //홈 오프셑.
}

void CMotors::UpdateRptPara(int      _iAxisNo     ,   //Repeat Test Para (UI)
                            bool     _bToTable    ,
                            TEdit *  _edFstPos    ,  //첫위치
                            TEdit *  _edScdPos    ,  //두번째위치.
                            TEdit *  _edStopDelay )  //모션돈 후 정지 딜레이.
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]-> UpdateRptPara(_bToTable    ,
                                    _edFstPos    ,  //첫위치
                                    _edScdPos    ,  //두번째위치.
                                    _edStopDelay );  //모션돈 후 정지 딜레이.
}

void CMotors::SetAxtTrgPos (int _iAxisNo ,int _iPosCnt , double * _dPos , double _dTrgTime , bool _bActual ,bool _bOnLevel)
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]-> SetAjinTrgPos(_iPosCnt , _dPos , _dTrgTime , _bActual , _bOnLevel) ;

}
void CMotors::ResetAxtTrgPos(int _iAxisNo )
{
    _NG_RANGE_VOID_
    Motor[_iAxisNo]-> ResetAjinTrgPos() ;
}

void CMotors::OneShotAxtTrg (int _iAxisNo , bool _bUpEg , int _iTime)
{
    _NG_RANGE_VOID_
    Motor[_iAxisNo]-> OneShotAjinTrg(_bUpEg , _iTime);
}

//포지션 인터럽트
void CMotors::SetIntrptPos(int _iAxisNo , double _dPos, PosCallbackFunc _pCallback)
{
    _NG_RANGE_VOID_
    Motor[_iAxisNo] -> SetIntrptPos(_dPos , _pCallback) ;
}

void CMotors::ResetIntrpt(int _iAxisNo )
{
    _NG_RANGE_VOID_
    Motor[_iAxisNo] -> ResetIntrpt() ;

}

