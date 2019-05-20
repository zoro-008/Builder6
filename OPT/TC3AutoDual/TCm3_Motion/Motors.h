//---------------------------------------------------------------------------
#ifndef MotorsH
#define MotorsH

//System Header
//#include <StdCtrls.hpp>

//User header
#include "Axt_FS20.h"
#include "SLogUnit.h"
#include "TEdit.h"

#define _NG_RANGE_VOID_   if( _iAxisNo < 0 || _iAxisNo >= m_iMaxMotor) {ShowMessageT("Axis Index Range is wrong"); return       ; }
#define _NG_RANGE_DOUBLE_ if( _iAxisNo < 0 || _iAxisNo >= m_iMaxMotor) {ShowMessageT("Axis Index Range is wrong"); return 0.0   ; }
#define _NG_RANGE_BOOL_   if( _iAxisNo < 0 || _iAxisNo >= m_iMaxMotor) {ShowMessageT("Axis Index Range is wrong"); return false ; }
#define _NG_RANGE_STRING_ if( _iAxisNo < 0 || _iAxisNo >= m_iMaxMotor) {ShowMessageT("Axis Index Range is wrong"); return ""    ; }

/*class CMotor
{
    public:
        CMotor();
        virtual ~CMotor();

    protected:
        CAxtFS20 * AxtFS20 ;

    public:

        //Thread Cycle Func.
        void Update ();

        //Set Stat Directly Func.
        void SetHomeEnd (bool _bValue ); bool GetHomeEnd ();
        void SetServo   (bool _bOn    ); bool GetServo   ();
        void SetAlarm   (bool _bOn    ); bool GetAlarm   ();

        //Update Motor Status.
        bool GetStopInpos() ; //Check Motion Done with User Inposition.
        bool GetReady    () ; //Check ready to move.
        bool GetOk       () ; //Check No Limit & Servo Enable.

        //Position Functions.
        double GetTrgPos( );                       //Get Target  Position.
        double GetCmdPos( );                       //Get Command Position.
        double GetEncPos( );                       //Get Encoder Position.
        double GetErrPos( );                       //Get Control Error Position.
        void   ClearPos ( );                       //Clear Position.
        void   SetPos   (double _dPos = 0.0);   //Set Any Position.

        //Motion Functions.
        void Reset   (); //Reset
        bool Stop    ();
        bool EmgStop (); //Stop Without Deceleration.

        bool Home   (); //Start Home Cycle for Sequense Cycle.
        void DoHome (); //Start Home Cycle for Click Event.

        void DoRpt  (); //Start Home Cycle for Click Event.

        bool JogP(); //Jogging to CW.
        bool JogN(); //Jogging to CCW.

        bool GoAbs (double _dPos , double _dVel);  //abs move
        void GoInc (double _dPos , double _dVel);  //Inc move  모션돈 확인은 GetStopInpos() 함수로.

        bool GoAbsRun (double _dPos );  //abs move with AutoRun Vel.
        void GoIncRun (double _dPos );  //Inc move with AutoRun ManualVel. 모션돈 확인은 GetStopInpos() 함수로.

        bool GoAbsMan (double _dPos );  //abs move with Manual Vel.
        void GoIncMan (double _dPos );  //Inc move with Manual Vel. 모션돈 확인은 GetStopInpos() 함수로.

        //Parameter Memory Handling Fuctions.
        void SetAxtPara (CAxtFS20::CAxtPara &_AxtPara ){_NG_RANGE_VOID_ AxtFS20[_iAxisNo].SetAxtPara (_AxtPara );}
        void GetAxtPara (CAxtFS20::CAxtPara &_AxtPara ){_NG_RANGE_VOID_ AxtFS20[_iAxisNo].GetAxtPara (_AxtPara );}

        void SetPara    (CAxtFS20::CPara    &_Para    ){_NG_RANGE_VOID_ AxtFS20[_iAxisNo].SetPara    (_Para    );}
        void GetPara    (CAxtFS20::CPara    &_Para    ){_NG_RANGE_VOID_ AxtFS20[_iAxisNo].GetPara    (_Para    );}

        void SetUsrPara (CAxtFS20::CUsrPara &_UsrPara ){_NG_RANGE_VOID_ AxtFS20[_iAxisNo].SetUsrPara (_UsrPara );}
        void GetUsrPara (CAxtFS20::CUsrPara &_UsrPara ){_NG_RANGE_VOID_ AxtFS20[_iAxisNo].GetUsrPara (_UsrPara );}

        void SetRptPara (CAxtFS20::CRptPara &_RptPara ){_NG_RANGE_VOID_ AxtFS20[_iAxisNo].SetRptPara (_RptPara );}
        void GetRptPara (CAxtFS20::CRptPara &_RptPara ){_NG_RANGE_VOID_ AxtFS20[_iAxisNo].GetRptPara (_RptPara );}

        void GetStat    (CAxtFS20::CStat    &_Stat    ){_NG_RANGE_VOID_ AxtFS20[_iAxisNo].GetStat    (_Stat    );}

        void GetHomeStat(CAxtFS20::CStat    &_HomeStat){_NG_RANGE_VOID_ AxtFS20[_iAxisNo].GetHomeStat(_HomeStat);}

        void GetRptStat (CAxtFS20::CRptPara &_RptPara ){_NG_RANGE_VOID_ AxtFS20[_iAxisNo].GetRptStat (_RptPara );}

	//Load Save Para Functions.
        void Save();
        void Load();

        //Load Save Dynamic Var.
        void SaveDnmVar();
        void LoadDnmVar();

        void GetMotorList(TComboBox * _cbName);

        //UI Handler
        void UpdateStat(int     _iAxisNo       ,
                        TLabel *_lbServoOn     ,
                        TLabel *_lbHomeSnsr    ,
                        TLabel *_lbPEndLimSnsr ,
                        TLabel *_lbNEndLimSnsr ,
                        TLabel *_lbHomeEnded   ,
                        TLabel *_lbAlarm       ,
                        TLabel *_lbEncPos      ,
                        TLabel *_lbPackInPosn  = NULL,
                        TLabel *_lbStop        = NULL,
                        TLabel *_lbBusy        = NULL,
                        TLabel *_lbReady       = NULL,
                        TLabel *_lbOk          = NULL,
                        TLabel *_lbJogP        = NULL,
                        TLabel *_lbJogN        = NULL,
                        TLabel *_lbTrgPos      = NULL,
                        TLabel *_lbCmdPos      = NULL);

};*/
//---------------------------------------------------------------------------
class CMotors
{
    public:
        CMotors();
        virtual ~CMotors();

    protected:
        CAxtFS20 * AxtFS20 ; int m_iMaxMotor ;

    public:

        //Thread Cycle Func.
        void Update ();

        //Set Stat Directly Func.
        void SetHomeEnd (int _iAxisNo , bool _bValue ); bool GetHomeEnd (int _iAxisNo ); bool GetHomeEndAll ();
        void SetServo   (int _iAxisNo , bool _bOn    ); bool GetServo   (int _iAxisNo );
        void SetAlarm   (int _iAxisNo , bool _bOn    ); bool GetAlarm   (int _iAxisNo );
                                                        bool GetPLim    (int _iAxisNo );
                                                        bool GetNLim    (int _iAxisNo );

        //Update Motor Status.
        bool GetStopInpos(int _iAxisNo) ; //Check Motion Done with User Inposition.
        bool GetReady    (int _iAxisNo) ; //Check ready to move.
        bool GetOk       (int _iAxisNo) ; //Check No Limit & Servo Enable.

        //Position Functions.
        double GetTrgPos(int _iAxisNo );                       //Get Target  Position.
        double GetCmdPos(int _iAxisNo );                       //Get Command Position.
        double GetEncPos(int _iAxisNo );                       //Get Encoder Position.
        double GetErrPos(int _iAxisNo );                       //Get Control Error Position.
        void   ClearPos (int _iAxisNo );                       //Clear Position.
        void   SetPos   (int _iAxisNo , double _dPos = 0.0);   //Set Any Position.

        bool   CmprPos(int _iAxisNo , double _dPos , double _dRange = 0.0) ;

        //Motion Functions.
        void Reset   (int _iAxisNo); //Reset
        void ResetAll(            );
        bool Stop    (int _iAxisNo);
        bool EmgStop (int _iAxisNo); //Stop Without Deceleration.

        bool Home   (int _iAxisNo); //Start Home Cycle for Sequense Cycle.
        void DoHome (int _iAxisNo); //Start Home Cycle for Click Event.

        bool Vibration(int _iAxisNo , double _dRange , double _dVel , int _iCnt, bool _bCntClr);
        void DoRpt    (int _iAxisNo); //Start Home Cycle for Click Event.

        bool JogP(int _iAxisNo); //Jogging to CW.
        bool JogN(int _iAxisNo); //Jogging to CCW.

        bool GoAbs (int _iAxisNo , double _dPos , double _dVel);  //abs move
        void GoInc (int _iAxisNo , double _dPos , double _dVel);  //Inc move  모션돈 확인은 GetStopInpos() 함수로.

        bool GoAbsRun (int _iAxisNo , double _dPos );  //abs move with AutoRun Vel.
        void GoIncRun (int _iAxisNo , double _dPos );  //Inc move with AutoRun ManualVel. 모션돈 확인은 GetStopInpos() 함수로.

        bool GoAbsMan (int _iAxisNo , double _dPos );  //abs move with Manual Vel.
        void GoIncMan (int _iAxisNo , double _dPos );  //Inc move with Manual Vel. 모션돈 확인은 GetStopInpos() 함수로.

        //Parameter Memory Handling Fuctions.
        void SetAxtPara (int _iAxisNo , CAxtFS20::CAxtPara &_AxtPara ){_NG_RANGE_VOID_ AxtFS20[_iAxisNo].SetAxtPara (_AxtPara );}
        void GetAxtPara (int _iAxisNo , CAxtFS20::CAxtPara &_AxtPara ){_NG_RANGE_VOID_ AxtFS20[_iAxisNo].GetAxtPara (_AxtPara );}

        void SetPara    (int _iAxisNo , CAxtFS20::CPara    &_Para    ){_NG_RANGE_VOID_ AxtFS20[_iAxisNo].SetPara    (_Para    );}
        void GetPara    (int _iAxisNo , CAxtFS20::CPara    &_Para    ){_NG_RANGE_VOID_ AxtFS20[_iAxisNo].GetPara    (_Para    );}

        void SetUsrPara (int _iAxisNo , CAxtFS20::CUsrPara &_UsrPara ){_NG_RANGE_VOID_ AxtFS20[_iAxisNo].SetUsrPara (_UsrPara );}
        void GetUsrPara (int _iAxisNo , CAxtFS20::CUsrPara &_UsrPara ){_NG_RANGE_VOID_ AxtFS20[_iAxisNo].GetUsrPara (_UsrPara );}

        void SetRptPara (int _iAxisNo , CAxtFS20::CRptPara &_RptPara ){_NG_RANGE_VOID_ AxtFS20[_iAxisNo].SetRptPara (_RptPara );}
        void GetRptPara (int _iAxisNo , CAxtFS20::CRptPara &_RptPara ){_NG_RANGE_VOID_ AxtFS20[_iAxisNo].GetRptPara (_RptPara );}

        void GetStat    (int _iAxisNo , CAxtFS20::CStat    &_Stat    ){_NG_RANGE_VOID_ AxtFS20[_iAxisNo].GetStat    (_Stat    );}

        void GetHomeStat(int _iAxisNo , CAxtFS20::CStat    &_HomeStat){_NG_RANGE_VOID_ AxtFS20[_iAxisNo].GetHomeStat(_HomeStat);}

        void GetRptStat (int _iAxisNo , CAxtFS20::CRptPara &_RptPara ){_NG_RANGE_VOID_ AxtFS20[_iAxisNo].GetRptStat (_RptPara );}

        AnsiString GetName(int _iAxisNo);
        int        GetMotorCnt () { return m_iMaxMotor;}

	//Load Save Para Functions.
        void Save();
        void Load();
        void ApplyPara();


        //Load Save Dynamic Var.
        void SaveDnmVar();
        void LoadDnmVar();

        void GetMotorList(TComboBox * _cbName);

        //UI Handler
        void UpdateStat(int     _iAxisNo       ,
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
                        TLabel *_lbCmdPos      );

        void UpdatePara(int     _iAxisNo      ,
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
                        TEdit * _edStopDelay  );

};

//---------------------------------------------------------------------------
extern CMotors MT;

//---------------------------------------------------------------------------
#endif
