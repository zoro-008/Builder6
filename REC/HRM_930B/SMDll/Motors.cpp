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

void CMotors::GoInc(int _iAxisNo , double _dPos , double _dVel) //Inc move  ��ǵ� Ȯ���� GetStopInpos() �Լ���.
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]->GoInc(_dPos , _dVel);
}

bool CMotors::GoAbsRun(int _iAxisNo , double _dPos ) //abs move with AutoRun Vel.
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->GoAbsRun(_dPos);
}
void CMotors::GoIncRun(int _iAxisNo , double _dPos ) //Inc move with AutoRun ManualVel. ��ǵ� Ȯ���� GetStopInpos() �Լ���.
{
    _NG_RANGE_VOID_

    return Motor[_iAxisNo]->GoIncRun(_dPos);
}

bool CMotors::GoAbsSlow(int _iAxisNo , double _dPos ) //abs move with AutoRun Vel.
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->GoAbsSlow(_dPos);
}
void CMotors::GoIncSlow(int _iAxisNo , double _dPos ) //Inc move with AutoRun ManualVel. ��ǵ� Ȯ���� GetStopInpos() �Լ���.
{
    _NG_RANGE_VOID_

    return Motor[_iAxisNo]->GoIncSlow(_dPos);
}


bool CMotors::GoAbsMan(int _iAxisNo , double _dPos ) //abs move with Manual Vel.
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->GoAbsMan(_dPos);
}

void CMotors::GoIncMan(int _iAxisNo , double _dPos ) //Inc move with Manual Vel. ��ǵ� Ȯ���� GetStopInpos() �Լ���.
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
            if(i!=j) {                 //���߿� ��Ǹ�� �߰� �Ǹ� ��⺰�� ����ó���ؾ���.
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
        if(i!=_iAxisNo) {                 //���߿� ��Ǹ�� �߰� �Ǹ� ��⺰�� ����ó���ؾ���.
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
            if(i!=j) {                 //���߿� ��Ǹ�� �߰� �Ǹ� ��⺰�� ����ó���ؾ���.
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
                         TComboBox  * _cbDirType       ,
                         TCheckBox  * _cbSrvBreakType  )
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
                                 _cbDirType       ,
                                 _cbSrvBreakType  );
}

void CMotors::UpdateUsrPara(int         _iAxisNo          ,
                            bool        _bToTable         ,
                            TCheckBox * _cbUseRate        ,  //������ ��� ����.
                            TEdit     * _edAcceleration   ,  //������
                            TEdit     * _edDeceleration   ,  //������
                            TEdit     * _edRunSpeed       ,  //�����ӵ�
                            TEdit     * _edSlowSpeed      ,  //�����ӵ�2
                            TEdit     * _edManSpeed       ,  //�޴��� �����ӵ�
                            TEdit     * _edHomeSpeed      ,  //Ȩ���� �ӵ�.
                            TEdit     * _edLastHomeSpeed  ,  //������ Z�� ���� �ӵ�.
                            TEdit     * _edJogSpeed       ,  //���ױ��� �ӵ�.
                            TEdit     * _edInposition     ,  //��������.
                            TEdit     * _edHomeOffset     )  //Ȩ �����V.
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]-> UpdateUsrPara(_bToTable        ,
                                    _cbUseRate       ,  //������ ��� ����.
                                    _edAcceleration  ,  //������
                                    _edDeceleration  ,  //������
                                    _edRunSpeed      ,  //�����ӵ�
                                    _edSlowSpeed     ,  //�����ӵ�2
                                    _edManSpeed      ,  //�޴��� �����ӵ�
                                    _edHomeSpeed     ,  //Ȩ���� �ӵ�.
                                    _edLastHomeSpeed ,  //������ Z�� ���� �ӵ�.
                                    _edJogSpeed      ,  //���ױ��� �ӵ�.
                                    _edInposition    ,  //��������.
                                    _edHomeOffset    );  //Ȩ �����V.
}

void CMotors::UpdateRptPara(int      _iAxisNo     ,   //Repeat Test Para (UI)
                            bool     _bToTable    ,
                            TEdit *  _edFstPos    ,  //ù��ġ
                            TEdit *  _edScdPos    ,  //�ι�°��ġ.
                            TEdit *  _edStopDelay )  //��ǵ� �� ���� ������.
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]-> UpdateRptPara(_bToTable    ,
                                    _edFstPos    ,  //ù��ġ
                                    _edScdPos    ,  //�ι�°��ġ.
                                    _edStopDelay );  //��ǵ� �� ���� ������.
}

bool CMotors::FindEdgeStop   (int _iAxisNo ,double _dVel ,double _dAcc ,CAjinMotion::EN_FINDEDGE_TYPE _ftFindEdgeType)
{
    return Motor[_iAxisNo]-> FindEdgeStop(_dVel ,_dAcc , _ftFindEdgeType);
}

bool CMotors::FindEdgeEmgStop(int _iAxisNo ,double _dVel ,double _dAcc ,CAjinMotion::EN_FINDEDGE_TYPE _ftFindEdgeType)
{
    return Motor[_iAxisNo]-> FindEdgeEmgStop(_dVel ,_dAcc , _ftFindEdgeType);
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

//������ ���ͷ�Ʈ
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

double CMotors::GetRptTickTime(int _iAxisNo)
{
    _NG_RANGE_BOOL_

    return Motor[_iAxisNo]->GetRptTickTime();
}

void CMotors::SetOverrideMaxSpeed(int _iAxisNo, double _dOverSped)
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]->SetOverrideMaxSpeed(_dOverSped);
}

void CMotors::SetOverrideVel     (int _iAxisNo, double _dOverSped)
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]->SetOverrideVel(_dOverSped);
}

void CMotors::SetOverrideVelAtPos(int _iAxisNo, double _dPos, double _dVel, double _dOverPos, double _dOverSped , bool _bCmd)
{
    _NG_RANGE_VOID_

    Motor[_iAxisNo]->SetOverrideVelAtPos(_dPos, _dVel , _dOverPos, _dOverSped, _bCmd);
}

// ���� ������ ���� �߰�
void CMotors::ContiSetAxisMap(int _iCoord, int _iAxisCnt, int *_ipAxisNo) // _ipAxisNo ���� ���� ��������... �� ���� �ذ��ؾ���...
{
    //_NG_RANGE_VOID_

    int *ipRealAxisNo; // int �� ������ ����
    ipRealAxisNo = new int[_iAxisCnt] ; // Motor ������ŭ int �� ������ �迭 ����

    int iMasterAxisPhyAdd = 9999 ; // ������ �� ������ �ּ� �� 9999�� �ʱ�ȭ

    int iPhyscAdd = 0 ;

    for(int i = 0; i < _iAxisCnt ; i++){
        iPhyscAdd = Motor[_ipAxisNo[i]] -> GetPhyscAdd(); // ���� ������ �ѹ� �������� �Լ�

        if(iPhyscAdd < iMasterAxisPhyAdd) iMasterAxisPhyAdd = iPhyscAdd ; //���� ���� ���� Master�� �ǰ�
        ipRealAxisNo[i] = iPhyscAdd ;
    }

    //AjinMotion -> ContiSetAxisMap(_iCoord, _iAxisCnt, _ipAxisNo) ; // �� �ΰ��� ��������??
    CAjinMotion::ContiSetAxisMap(_iCoord, _iAxisCnt, ipRealAxisNo) ;

    delete []ipRealAxisNo ;
}

void CMotors::ContiSetAbsRelMode(int _iCoord, DWORD _dwAbsRelMode)
{
    //_NG_RANGE_VOID_
    CAjinMotion::ContiSetAbsRelMode(_iCoord, _dwAbsRelMode) ;
}

void CMotors::ContiBeginNode(int _iCoord)
{
//    _NG_RANGE_VOID_
    CAjinMotion::ContiBeginNode(_iCoord) ;
}

void CMotors::ContiEndNode(int _iCoord)
{
//    _NG_RANGE_VOID_
    CAjinMotion::ContiEndNode(_iCoord) ;
}

void CMotors::ContiStart(int _iCoord, DWORD _dwProfileset, int _iAngle)
{
//    _NG_RANGE_VOID_
    CAjinMotion::ContiStart(_iCoord, _dwProfileset, _iAngle) ;
}

int CMotors::GetContCrntIdx(int _iCoord)
{
    return CAjinMotion::GetContCrntIdx(_iCoord);

}

void CMotors::LineMove(int _iCoord, int *_ipAxisNo, double *_dpEndPos, double _dVel, double _dAcc, double _dDec, int _iAxisCnt)
{
/*
    int *iPulsePer, *iUnitPer ;

    iPulsePer = new int[_iAxisCnt] ;
    iUnitPer  = new int[_iAxisCnt] ;

    for(int i = 0; i < _iAxisCnt ; i++){
        iPulsePer[i] = Motor[i] -> Para.iPulsePerRev ;
        iUnitPer [i] = Motor[i] -> Para.dUnitPerRev  ;
        _dpEndPos[i] = _dpEndPos[i] * (iPulsePer[i] / iUnitPer[i]) ;
    }

    delete []iPulsePer ;
    delete []iUnitPer  ;
//    _NG_RANGE_VOID_
    AjinMotion -> LineMove(_iCoord, _dpEndPos, _dVel, _dAcc, _dDec) ;
*/

/*
�̷��� ���������� �ӵ������� �� �� ���� /�����ǥ ������ ���� ������ ���������Ǵ� ����߿�����������ȣ�����󰡵��ϵǾ������Ƿ�
����߿�����������ȣ�����󰡵��ϵǾ������Ƿ� �� ���� �����ϱ� �ٶ��� .
�������� �������� �������� �������� : ���� ���� ��� ���� ���� ���� ������ ������ ������ ���� �ϴ� ����� ����� �����
Unit/PulseUnit/PulseUnit/Pulse Unit/PulseUnit/PulseUnit/PulseUnit/PulseUnit/PulseUnit/Pulse������ ������ ������ ������ �����Ѵ� �����Ѵ� �����Ѵ� �����Ѵ� .���� �� �ӵ�
Library User Manual Rev.2.0 ���������Լ�(Line, Circle)
AJINEXTEK CO.,LTD. 529
�� ���ӵ� ���� ���� ��� �� ���� ���� �����̴� �����̴� �����̴� �����̴� �Ÿ� (�޽� ���� )�� �������� �������� �������� �������� �������µ� �������µ� �������µ� �������µ� �������µ� , Unit/Pulse , Unit/Pulse , Unit/Pulse , Unit/Pulse , Unit/Pulse , Unit/Pulse , Unit/Pulse , Unit/Pulse , Unit/Pulse , Unit/Pulse , Unit/Pulse , Unit/Pulse �� �ٸ� ��� ���� ���� �� ���� ���� ����� ����� ����� �� Unit/PulseUnit/PulseUnit/Pulse Unit/PulseUnit/PulseUnit/PulseUnit/PulseUnit/PulseUnit/Pulse�� ���̿� ���� �߻��ϴ� �߻��ϴ� �߻��ϴ� �߻��ϴ� ������ ������ ������ ������ ������ ������ ������ ���� �ǵ����� �ǵ����� �ǵ����� �ǵ����� ���� �ӵ� ������ ������ ������ �̷�� �� �� �ִ� .
���� ��� ���� ���� ���� ������ ������ ������ ���� �ϴ� ����� ����� ����� Unit/PulseUnit/PulseUnit/Pulse Unit/PulseUnit/PulseUnit/PulseUnit/PulseUnit/PulseUnit/Pulse������ ������ ������ ������ �����Ѵ� �����Ѵ� �����Ѵ� �����Ѵ�
*/

    int *ipRealAxisNo;
    ipRealAxisNo = new int[_iAxisCnt] ;

    int iMasterAxisPhyAdd = 9999 ;

    int iPhyscAdd = 0 ;

    for(int i = 0 ; i < _iAxisCnt ; i++){
        //iPhyscAdd = Motor[_ipAxisNo[i]] -> GetPhyscAdd(); // ���� ������ �ѹ� �ּ� �������� �Լ�
        iPhyscAdd = Motor[_ipAxisNo[i]] -> GetPhyscAdd(); // ���� ������ �ѹ� �ּ� �������� �Լ�

        if(iPhyscAdd < iMasterAxisPhyAdd) iMasterAxisPhyAdd = iPhyscAdd ; //���� ���� ���� Master�� �ǰ�
        ipRealAxisNo[i] = iPhyscAdd ;
    }

    //HRD-220a ���� ����ũ�� ��ġ�� ��ġ ���� �ʰ�. 10mm,4mm
    //�����ѿ��� 32768�� ���� �־�...
    //10:4�� �����⿡�� �ָ��� ��Ȳ.
    //�׷��� ���� ���� �̵� �ϴ� ���� �������� �ӵ��� ���� �ش�.
    //�߸Ű� �������� �����ؾ���...

    //�߸�.....���ι��길 ����� �׳� �Ѵ�.
    static double dPreLineMovePos[99] ;



//    for (int j = 0 ; '\0' != _dpEndPos[j] ; j++) { _dpEndPos[j] = Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dpEndPos[j]) ; }



    //10:4�� �����⿡�� �ָ��� ��Ȳ.

    for (int j = 0 ; j < _iAxisCnt ; j++) {

        _dpEndPos[j]    = Motor[ipRealAxisNo[j]] -> GetPulseFromUnit(_dpEndPos[j]) ;
        dPreLineMovePos[j] = _dpEndPos[j] ;
    }

    CAjinMotion::LineMove(_iCoord, _dpEndPos, Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dVel), // ���� * ( Pulse / Unit ), Unit�� Pulse�� �ٲپ���
                                              Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dAcc), // pulses / Sec^2 ??
                                              Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dDec)) ;

    delete []ipRealAxisNo ;

    Trace("LineMove _dVel" , String(_dVel).c_str());
    Trace("LineMove _dVel Pulse" , String(Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dVel)).c_str());
}

void CMotors::CircleCenterMove(int _iCoord, int *_ipAxisNo, int _iAxisCnt, double *_dpCenterPos, double *_dpEndPos, double _dVel, double _dAcc, double _dDec, DWORD _dwCWDir)
{
    int *ipRealAxisNo;
    ipRealAxisNo = new int[_iAxisCnt] ;

    int iMasterAxisPhyAdd = 9999 ;

    int iPhyscAdd = 0 ;

    for(int i = 0 ; i < _iAxisCnt ; i++){
        iPhyscAdd = Motor[_ipAxisNo[i]] -> GetPhyscAdd(); // ���� ������ �ѹ� �ּ� �������� �Լ�

        if(iPhyscAdd < iMasterAxisPhyAdd) iMasterAxisPhyAdd = iPhyscAdd ; //���� ���� ���� Master�� �ǰ�
        ipRealAxisNo[i] = iPhyscAdd ;
    }

    for (int j = 0 ; '\0' != _dpCenterPos[j] ; j++) { _dpCenterPos[j] = Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dpCenterPos[j]) ;
                                                      _dpEndPos[j]    = Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dpEndPos[j]   ) ; }

    CAjinMotion::CircleCenterMove(_iCoord, ipRealAxisNo, _dpCenterPos, _dpEndPos, Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dVel),
                                                                                     Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dAcc),
                                                                                     Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dDec), _dwCWDir) ;
    delete []ipRealAxisNo ;
}

void CMotors::CirclePointMove(int _iCoord, int *_ipAxisNo, int _iAxisCnt, double *_dpMidPos, double *_dpEndPos, double _dVel, double _dAcc, double _dDec, int _iArcCircle)
{
    int *ipRealAxisNo;
    ipRealAxisNo = new int[_iAxisCnt] ;

    int iMasterAxisPhyAdd = 9999 ;

    int iPhyscAdd = 0 ;

    for(int i = 0; i < _iAxisCnt ; i++){
        iPhyscAdd = Motor[_ipAxisNo[i]] -> GetPhyscAdd(); // ���� ������ �ѹ� �ּ� �������� �Լ�

        if(iPhyscAdd < iMasterAxisPhyAdd) iMasterAxisPhyAdd = iPhyscAdd ; //���� ���� ���� Master�� �ǰ�
        ipRealAxisNo[i] = iPhyscAdd ;
    }

    for (int j = 0 ; '\0' != _dpMidPos[j] ; j++) { _dpMidPos[j] = Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dpMidPos[j]) ;
                                                   _dpEndPos[j] = Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dpEndPos[j]) ; }

    CAjinMotion::CirclePointMove(_iCoord, ipRealAxisNo, _dpMidPos, _dpEndPos, Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dVel),
                                                                              Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dAcc),
                                                                              Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dDec), _iArcCircle);
    delete []ipRealAxisNo ;
}

void CMotors::CircleRadiusMove(int _iCoord, int *_ipAxisNo, int _iAxisCnt, double _dRadius, double *_dpEndPos, double _dVel, double _dAcc, double _dDec, DWORD _dwCWDir, DWORD _dwShortDistance)
{
    int *ipRealAxisNo;
    ipRealAxisNo = new int[_iAxisCnt] ;

    int iMasterAxisPhyAdd = 9999 ;

    int iPhyscAdd = 0 ;

    for(int i = 0; i < _iAxisCnt ; i++){
        iPhyscAdd = Motor[_ipAxisNo[i]] -> GetPhyscAdd(); // ���� ������ �ѹ� �ּ� �������� �Լ�

        if(iPhyscAdd < iMasterAxisPhyAdd) iMasterAxisPhyAdd = iPhyscAdd ; //���� ���� ���� Master�� �ǰ�
        ipRealAxisNo[i] = iPhyscAdd ;
    }

    for (int j = 0 ; '\0' != _dpEndPos[j] ; j++) { _dpEndPos[j] = Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dpEndPos[j]) ; }

    CAjinMotion::CircleRadiusMove(_iCoord, ipRealAxisNo, _dRadius, _dpEndPos, Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dVel),
                                                                                 Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dAcc),
                                                                                 Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dDec), _dwCWDir, _dwShortDistance);
    delete []ipRealAxisNo ;
}

void CMotors::CircleAngleMove(int _iCoord, int *_ipAxisNo, int _iAxisCnt, double *_dCenterPos, double _dAngle, double _dVel, double _dAcc, double _dDec, DWORD _dwCWDir)
{
    int *ipRealAxisNo;
    ipRealAxisNo = new int[_iAxisCnt] ;

    int iMasterAxisPhyAdd = 9999 ;

    int iPhyscAdd = 0 ;

    for(int i = 0; i < _iAxisCnt ; i++){
        iPhyscAdd = Motor[_ipAxisNo[i]] -> GetPhyscAdd(); // ���� ������ �ѹ� �ּ� �������� �Լ�

        if(iPhyscAdd < iMasterAxisPhyAdd) iMasterAxisPhyAdd = iPhyscAdd ; //���� ���� ���� Master�� �ǰ�
        ipRealAxisNo[i] = iPhyscAdd ;
    }

    for (int j = 0 ; '\0' != _dCenterPos[j] ; j++) { _dCenterPos[j] = Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dCenterPos[j]) ; }

    CAjinMotion::CircleAngleMove(_iCoord, ipRealAxisNo, _dCenterPos, _dAngle, Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dVel),
                                                                              Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dAcc),
                                                                              Motor[iMasterAxisPhyAdd] -> GetPulseFromUnit(_dDec), _dwCWDir);
    delete []ipRealAxisNo ;

}


//��Ʈ��.
void CMotors::SetGantryEnable(int _iMasterAxis , int _iSlaveAxis , int _iHomeUse , double _dSlaveOffset , double _dSlOffsetRange)
{
    //CAjinMotion::SetGantryEnable(_iMasterAxis , _iSlaveAxis , _iHomeUse , _dSlaveOffset , _dSlOffsetRange);
}
void CMotors::SetGantryDisable(int _iMasterAxis , int _iSlaveAxis)
{
    //CAjinMotion::SetGantryDisable(_iMasterAxis , _iSlaveAxis);
}

//��Ʈ���� Ȩ�Լ�.
//void CMotors::SetHomeMethod(int _iAxis , int _iHomeDir , DWORD _dwHomeSignal , DWORD _dwUseZ , double _dHomeClrTime , double _dHomeOffset)
//{
//
//}
//void CMotors::SetHomeVel(int _iAxis , double _dVel1 , double _dVel2 , double _dVel3 , double _dLast ,double _dAcc1 , double _dAcc2)
//{
//
//}
void CMotors::SetHomeStart(int _iAxis)
{
/*
    int iPhyscAdd = Motor[_iAxis] -> GetPhyscAdd(); // ���� ������ �ѹ� �ּ� �������� �Լ�

    const int    iHomeDir     = -1 ; //-1 :-���� 1:+����
    DWORD  dwHomeSignal ; //0:+Lim ,1:-Lim , 4:Ori , 5,Z
    DWORD  dwHomeUseZ   ; //0:������ 1:+���� 2:-����
    const double dHomeClrTime = 1000 ; //ms
    const double dHomeOffset  = Motor[_iAxis] ->GetPulseFromUnit(0.1);

    //enum EN_HOME_METHOD     { hmOrigin  = 0 , hmNegative = 1 , hmOriginZ = 2 , hmNegativeZ = 3 , hmZ = 4 , MAX_HOME_METHOD };
    if(Motor[_iAxis] ->GetPara()-> iHomeMethod == CMotor::hmOrigin){
        dwHomeSignal = 4 ;
        dwHomeUseZ   = 0 ;
    }
    else if(Motor[_iAxis] ->GetPara()-> iHomeMethod == CMotor::hmNegative){
        dwHomeSignal = 1 ;
        dwHomeUseZ   = 0 ;
    }
    else if(Motor[_iAxis] ->GetPara()-> iHomeMethod == CMotor::hmOriginZ){
        dwHomeSignal = 4 ;
        dwHomeUseZ   = 1 ;
    }
    else if(Motor[_iAxis] ->GetPara()-> iHomeMethod == CMotor::hmNegativeZ){
        dwHomeSignal = 1 ;
        dwHomeUseZ   = 1 ;
    }
    else if(Motor[_iAxis] ->GetPara()-> iHomeMethod == CMotor::hmZ){
        dwHomeSignal = 5 ;
        dwHomeUseZ   = 0 ;
    }
    //int _iAxis , int _iHomeDir , DWORD _dwHomeSignal , DWORD _dwUseZ , double _dHomeClrTime , double _dHomeOffset)
    CAjinMotion::SetHomeMethod(iPhyscAdd    ,
                               iHomeDir     ,
                               dwHomeSignal ,
                               dwHomeUseZ   ,
                               dHomeClrTime ,
                               dHomeOffset  );



    //int _iAxis , double _dVel1 , double _dVel2 , double _dVel3 , double _dLast ,double _dAcc1 , double _dAcc2
    const double dVel1 = Motor[_iAxis] ->GetPulseFromUnit(Motor[_iAxis] ->GetUsrPara()->dHomeSpeed );
    const double dVel2 = Motor[_iAxis] ->GetPulseFromUnit(Motor[_iAxis] ->GetUsrPara()->dHomeSpeed );
    const double dVel3 = Motor[_iAxis] ->GetPulseFromUnit(Motor[_iAxis] ->GetUsrPara()->dLastHomeSpeed );
    const double dLast = Motor[_iAxis] ->GetPulseFromUnit(Motor[_iAxis] ->GetUsrPara()->dLastHomeSpeed );
    const double dAcc1 = Motor[_iAxis] ->GetPulseFromUnit(Motor[_iAxis] ->GetUsrPara()->dAcceleration  );
    const double dAcc2 = Motor[_iAxis] ->GetPulseFromUnit(Motor[_iAxis] ->GetUsrPara()->dAcceleration  );

    CAjinMotion::SetHomeVel(iPhyscAdd ,
                            dVel1 ,
                            dVel2 ,
                            dVel3 ,
                            dLast ,
                            dAcc1 ,
                            dAcc2);

    CAjinMotion::SetHomeStart(_iAxis);
*/
}
bool CMotors::GetHomeResult(int _iAxis)
{
/*
    DWORD dwResult ;
    CAjinMotion::GetHomeResult(_iAxis , &dwResult);

    return dwResult == 0;
*/
return false ;
}
