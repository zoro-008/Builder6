//---------------------------------------------------------------------------
#ifndef MotorsH
#define MotorsH

//System Header
//#include <StdCtrls.hpp>

//User header
#include "Motor.h"
#include "SLogUnit.h"
#include "TEdit.h"

#define _NG_RANGE_VOID_   if( _iAxisNo < 0 || _iAxisNo >= m_iMaxMotor) {ShowMessageT("Axis Index Range is wrong"); return       ; }
#define _NG_RANGE_DOUBLE_ if( _iAxisNo < 0 || _iAxisNo >= m_iMaxMotor) {ShowMessageT("Axis Index Range is wrong"); return 0.0   ; }
#define _NG_RANGE_BOOL_   if( _iAxisNo < 0 || _iAxisNo >= m_iMaxMotor) {ShowMessageT("Axis Index Range is wrong"); return false ; }
#define _NG_RANGE_STRING_ if( _iAxisNo < 0 || _iAxisNo >= m_iMaxMotor) {ShowMessageT("Axis Index Range is wrong"); return ""    ; }
#define _NG_RANGE_NULL_   if( _iAxisNo < 0 || _iAxisNo >= m_iMaxMotor) {ShowMessageT("Axis Index Range is wrong"); return NULL  ; }

#define _NG_RANGE_VOID_NO_MSG_   if( _iAxisNo < 0 || _iAxisNo >= m_iMaxMotor) {return       ; }
#define _NG_RANGE_DOUBLE_NO_MSG_ if( _iAxisNo < 0 || _iAxisNo >= m_iMaxMotor) {return 0.0   ; }
#define _NG_RANGE_BOOL_NO_MSG_   if( _iAxisNo < 0 || _iAxisNo >= m_iMaxMotor) {return false ; }
#define _NG_RANGE_STRING_NO_MSG_ if( _iAxisNo < 0 || _iAxisNo >= m_iMaxMotor) {return ""    ; }
#define _NG_RANGE_NULL_NO_MSG_   if( _iAxisNo < 0 || _iAxisNo >= m_iMaxMotor) {return NULL  ; }

//---------------------------------------------------------------------------
class CMotors
{
    public:
        CMotors();
        virtual ~CMotors();

    protected:
        CMotor ** Motor ; int m_iMaxMotor ;

    public:
        //Thread Cycle Func.
        void Update ();

        bool ChangeMotorCnt(int _iMaxMotor);

        //Set Stat Directly Func.
        void SetHomeEnd (int _iAxisNo , bool _bValue ); bool GetHomeEnd   (int _iAxisNo ); bool GetHomeEndAll ();
        void SetServo   (int _iAxisNo , bool _bOn    ); bool GetServo     (int _iAxisNo ); void SetServoAll   (bool _bOn); bool GetServoAll   ();
        void SetBreakOff(int _iAxisNo , bool _bOn    ); bool GetBreakOff  (int _iAxisNo );
        void SetReset   (int _iAxisNo , bool _bOn    );
                                                        bool GetAlarm     (int _iAxisNo );
                                                        bool GetHomeSnsr  (int _iAxisNo );
                                                        bool GetPLimSnsr  (int _iAxisNo );
                                                        bool GetNLimSnsr  (int _iAxisNo );
                                                        bool GetZphaseSnsr(int _iAxisNo );
        //Update Motor Status.
        bool GetStopInpos(int _iAxisNo) ; //Check Motion Done with User Inposition.
        bool GetStop     (int _iAxisNo) ; //Check Motion Done with User Inposition.
        bool GetReady    (int _iAxisNo) ; //Check ready to move.
        bool GetOk       (int _iAxisNo) ; //Check No Limit & Servo Enable.

        //Position Functions.
        double GetTrgPos(int _iAxisNo );                       //Get Target  Position.
        double GetCmdPos(int _iAxisNo );                       //Get Command Position.
        double GetEncPos(int _iAxisNo );                       //Get Encoder Position.
        double GetErrPos(int _iAxisNo );                       //Get Control Error Position.
        void   ClearPos (int _iAxisNo );                       //Clear Position.
        void   SetPos   (int _iAxisNo , double _dPos = 0.0);   //Set Any Position.

        double GetMinPos(int _iAxisNo );
        double GetMaxPos(int _iAxisNo );
        double GetRunVel(int _iAxisNo );
        double GetManVel(int _iAxisNo );

        bool   GoAbsAcDc(int _iAxisNo , double _dPos , double _dVel , double _dAcc , double _dDcc);
        void   GoIncAcDc(int _iAxisNo , double _dPos , double _dVel , double _dAcc , double _dDcc);

        bool   CmprPos(int _iAxisNo , double _dPos , double _dRange = 0.0) ;

        //Motion Functions.
        void Reset      (int _iAxisNo); //Reset
        void ResetAll   (            );
        void Stop       (int _iAxisNo);
        void StopAll    (            );
        void EmgStop    (int _iAxisNo); //Stop Without Deceleration.
        void EmgStopAll (            );

        bool Home      (int _iAxisNo); //Start Home Cycle for Sequense Cycle.
        void DoHome    (int _iAxisNo); //Start Home Cycle for Click Event.
        void SetSubHome(int _iAxisNo , bool _bOn){_NG_RANGE_VOID_ Motor[_iAxisNo]->SetSubHome(_bOn);}

        void DoRpt    (int _iAxisNo, double _edFirstPos, double _edSecondPos, int _edStopDelay); //Start Home Cycle for Click Event.

        bool JogP(int _iAxisNo); //Jogging to CW.
        bool JogN(int _iAxisNo); //Jogging to CCW.

        bool GoAbs (int _iAxisNo , double _dPos , double _dVel);  //abs move
        void GoInc (int _iAxisNo , double _dPos , double _dVel);  //Inc move  모션돈 확인은 GetStopInpos() 함수로.

        bool GoAbsRun (int _iAxisNo , double _dPos );  //abs move with AutoRun Vel.
        void GoIncRun (int _iAxisNo , double _dPos );  //Inc move with AutoRun ManualVel. 모션돈 확인은 GetStopInpos() 함수로.

        bool GoAbsSlow(int _iAxisNo , double _dPos );  
        void GoIncSlow(int _iAxisNo , double _dPos );  

        bool GoAbsMan (int _iAxisNo , double _dPos );  //abs move with Manual Vel.
        void GoIncMan (int _iAxisNo , double _dPos );  //Inc move with Manual Vel. 모션돈 확인은 GetStopInpos() 함수로.

        //Parameter Memory Handling Fuctions.
        void SetPara    (int _iAxisNo , CMotor  ::SPara    &_Para    ){_NG_RANGE_VOID_ Motor[_iAxisNo]->SetPara    (_Para    );}
        void SetUsrPara (int _iAxisNo , CMotor  ::SUsrPara &_UsrPara ){_NG_RANGE_VOID_ Motor[_iAxisNo]->SetUsrPara (_UsrPara );}
        void SetRptPara (int _iAxisNo , CMotor  ::SRptPara &_RptPara ){_NG_RANGE_VOID_ Motor[_iAxisNo]->SetRptPara (_RptPara );}

        CMotor::SPara     * GetPara    (int _iAxisNo){_NG_RANGE_NULL_ return Motor[_iAxisNo]->GetPara    ();}
        CMotor::SUsrPara  * GetUsrPara (int _iAxisNo){_NG_RANGE_NULL_ return Motor[_iAxisNo]->GetUsrPara ();}
        CMotor::SRptPara  * GetRptPara (int _iAxisNo){_NG_RANGE_NULL_ return Motor[_iAxisNo]->GetRptPara ();}
        CMotor::SStat     * GetStat    (int _iAxisNo){_NG_RANGE_NULL_ return Motor[_iAxisNo]->GetStat    ();}
        CMotor::SHomeStat * GetHomeStat(int _iAxisNo){_NG_RANGE_NULL_ return Motor[_iAxisNo]->GetHomeStat();}
        CMotor::SRptStat  * GetRptStat (int _iAxisNo){_NG_RANGE_NULL_ return Motor[_iAxisNo]->GetRptStat ();}

        AnsiString GetName(int _iAxisNo);
        int        GetMotorCnt () { return m_iMaxMotor;}

	//Load Save Para Functions.
        void Save(); void Save(int _iAxisNo);
        void Load();

        EN_MTOR_DIRECTION GetDirType(int _iAxisNo);

        //Load Save Dynamic Var.
        void SaveDnmVar();
        void LoadDnmVar();

        void GetMotorList(TComboBox * _cbName);

        //UI Handler
        void CMotors::UpdateStat(int     _iAxisNo       ,
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
                                 TLabel *_lbBreakOff    );

        void CMotors::UpdatePara(int           _iAxisNo        ,
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
                                 TComboBox  * _cbDirType       );

        void CMotors::UpdateUsrPara(int         _iAxisNo          ,
                                    bool        _bToTable         ,
                                    TCheckBox * _cbUseRate        ,  //가감속율 사용 여부.
                                    TEdit     * _edAcceleration   ,  //가감속시간.
                                    TEdit     * _edDeceleration   ,  //가감속시간.
                                    TEdit     * _edRunSpeed       ,  //구동속도
                                    TEdit     * _edSlowSpeed      ,  //구동속도2
                                    TEdit     * _edManSpeed       ,  //메뉴얼 구동속도
                                    TEdit     * _edHomeSpeed      ,  //홈구동 속도.
                                    TEdit     * _edLastHomeSpeed  ,  //마지막 Z상 정지 속도.
                                    TEdit     * _edJogSpeed       ,  //조그구동 속도.
                                    TEdit     * _edInposition     ,  //인포지션.
                                    TEdit     * _edHomeOffset     );  //홈 오프셑.

        void CMotors::UpdateRptPara(int      _iAxisNo     ,
                                    bool     _bToTable    ,
                                    TEdit *  _edFstPos    ,  //첫위치
                                    TEdit *  _edScdPos    ,  //두번째위치.
                                    TEdit *  _edStopDelay );  //모션돈 후 정지 딜레이.

        //For AxtFS20 Only.
        void SetAxtTrgPos  (int _iAxisNo , int _iPosCnt , double * _dPos , double _dTrgTime , bool _bActual ,bool _bOnLevel);
        void ResetAxtTrgPos(int _iAxisNo );
        void OneShotAxtTrg (int _iAxisNo , bool _bUpEg , int _iTime);

        void SetIntrptPos    (int _iAxisNo , double _dPos, PosCallbackFunc _pCallback);
        void ResetIntrpt     (int _iAxisNo );

};

//---------------------------------------------------------------------------
extern CMotors MT;

//---------------------------------------------------------------------------
#endif
