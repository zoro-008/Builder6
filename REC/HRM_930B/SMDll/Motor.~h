//---------------------------------------------------------------------------
#ifndef MotorH
#define MotorH

//---------------------------------------------------------------------------
#include "Ajin_Motion.h"
#include "Timer.h"
#include "SMDllDefine.h"

#define _CHECK_RANGE         if( _iAxisNo < 0 || _iAxisNo > m_iMaxAxtFS20)
#define _CHECK_RETURN_VOID   _CHECK_RANGE return       ;
#define _CHECK_RETURN_DOUBLE _CHECK_RANGE return 0.0   ;
#define _CHECK_RETURN_BOOL   _CHECK_RANGE return false ;

//---------------------------------------------------------------------------
void SetUI (TPanel * _pnBase) ;

class CMotor
{
    public:
        CMotor(int _iAxisNo);
        virtual ~CMotor();

        enum EN_MOTIONBORD_TYPE { mtAjin  = 0 , MAX_MOTIONBORD_TYPE  };
        enum EN_HOME_METHOD     { hmOrigin  = 0 , hmNegative = 1 , hmOriginZ = 2 , hmNegativeZ = 3 , hmZ = 4 , MAX_HOME_METHOD };

        struct SPara { //Master Class Level Para. (INI)
            AnsiString         sMotorName     ; //모터이름 UI혹은 LOG용.
            int                iMotionType    ; //모션 보드 타입.           EN_MOTIONBORD_TYPE
            int                iPulsePerRev   ; //한바퀴의 펄스수.
            double             dUnitPerRev    ; //한바퀴당 이동거리 혹은 각도.
            double             dMaxPos        ; //MAX Position.
            double             dMinPos        ; //MIN Position.
            bool               bExistEnc      ; //엔코더가 있는지 없는지.
            int                iHomeMethod    ; //홈센서 잡는 방법. 0:홈센서 1:-리밋 2:홈&Z 3:-리밋+Z   EN_HOME_METHOD
//          bool               bHomeWithNSnsr ; //홈센서로 홈을 잡을 것인지 -로 홈을 잡을 것인지.
            bool               bSrvOnLevelInv ; //ServoOn Output Level설정.
            int                iBreakAdd      ; //브레이크 타입 브레이크 Address
            int                iDirType       ; //모터 이동방향. 디스플레이용. EN_MTOR_DIRECTION
            bool               bSrvBreakType  ; //서보모터 브레이크 타입 설정.
        } ;

        struct SUsrPara { //Engineer Class User Display Para. (UI)
            bool    bUseRate     ; //감가속율/감가속 시간.  선택.
            double  dAcceleration; //가속시간.
            double  dDeceleration; //
            double  dRunSpeed    ; //구동속도
            double  dSlowSpeed   ; //느린구동속도.
            double  dManSpeed    ; //메뉴얼 구동속도
            double  dHomeSpeed   ; //홈구동 속도.
            double  dLastHomeSpeed  ; //홈구동 속도.
            double  dJogSpeed    ; //조그구동 속도.
            double  dInposition  ; //인포지션.
            double  dHomeOffset  ; //홈 오프셑.
        } ;

        struct SRptPara {  //Repeat Test Para (UI)
            double  dFstPos      ; //첫위치
            double  dScdPos      ; //두번째위치.
            double  dStopDelay   ; //모션돈 후 정지 딜레이.
            double  dTickTime    ; //Tick Time때문에 추가..
        } ;

        struct SStat { //Motor Status for Display.
            //Motion Status.
            bool   bServoOn      ;
            bool   bHomeSnsr     ;
            bool   bPEndLimSnsr  ;
            bool   bNEndLimSnsr  ;
            bool   bHomeEnded    ;
            bool   bAlarm        ;
            bool   bPackInPosn   ;
            bool   bStop         ;
            bool   bReady        ;
            bool   bOk           ;
            bool   bJogP         ;
            bool   bJogN         ;
            bool   bZphase       ;
            bool   bBreakOff     ;
            //Pos Stat
            double dSttPos ; //출발 하기 전에 위치.
            double dTrgPos ;
            double dCmdPos ;
            double dEncPos ;
        } ;

        struct SHomeStat{
            int  iHomeStep     ;
            int  iPreHomeStep  ;
            bool bHomeTimeout  ;
        } ;

        struct SRptStat{
            bool bGoRpt ;
        } ;

    protected:
        int  m_iAxisNo  ;


        CAjinMotion * AjinMotion ;

        void CycleRpt () ; //Repeat Cycle Processing.
        //bool GetStop  () ; //Motion Done. 서보팩 튜닝이 안된상태에서 외부에서 쓰면 위험할수 있어서 protected로 선언. GetStopInpos로 쓸것.
        bool CycleHome() ;
        bool CycleHomeSamei();

        SPara     Para     ;
        SUsrPara  UsrPara  ;
        SRptPara  RptPara  ;
        SStat     Stat     ;
        SHomeStat HomeStat ;
        SRptStat  RptStat  ;

        CDelayTimer  m_tmHomeCycle ;
        CDelayTimer  m_tmTemp ;

    public:
        //Thread Cycle Func.
        void Update ();

        //Get Set Stat Func.
        void SetHomeEnd (bool _bValue ); bool GetHomeEnd ();

        //Signal Input/Output.
        void SetServo   (bool _bOn    ); bool GetServo   ();
        void SetBreakOff(bool _bOn    ); bool GetBreakOff();
        void SetReset   (bool _bOn    );

        bool GetHomeSnsr  ();
        bool GetNLimSnsr  ();
        bool GetPLimSnsr  ();
        bool GetZphaseSnsr();
        bool GetPackInPos (); //Servo Pack InPosition Signal.
        bool GetAlarm     ();

        //Update Motor Status.
        bool GetStopInpos() ; //Check Motion Done with User Inposition.
        bool GetStop     () ; //Check Motion Done with User Inposition.
        bool GetReady    () ; //Check ready to move.
        bool GetOk       () ; //Check No Limit & Servo Enable.

        //Position Functions.
        double GetTrgPos();                                   //Get Target  Position.
        double GetCmdPos();                                   //Get Command Position.
        double GetEncPos();                                   //Get Encoder Position.
        double GetErrPos();                                   //Get Control Error Position.
        void   ClearPos ();                                   //Clear Position.
        void   SetPos   (double _dPos = 0.0);                 //Set Any Position.
        bool   CmprPos  (double _dPos, double _dRange = 0.0);

        double GetMinPos();
        double GetMaxPos();
        double GetRunVel();
        double GetManVel();
        double GetSlowVel();


        //Motion Functions.
        void Reset  (); //Reset
        void Stop   ();
        void EmgStop(); //Stop Without Deceleration.

        bool Home        (); //Start Home Cycle for Sequense Cycle.
        void DoHome      (); //Start Home Cycle for Click Event.

        void DoRpt(double _edFirstPos, double _edSecondPos, int _edStopDelay); //Start Home Cycle for Click Event.

        bool JogP(); //Jogging to CW.
        bool JogN(); //Jogging to CCW.

        bool GoHomeOffset (double _dPos , double _dVel);
        bool GoAbs        (double _dPos , double _dVel); //abs move
        void GoInc        (double _dPos , double _dVel); //Inc move  모션돈 확인은 GetStopInpos() 함수로.

        bool GoAbsRun (double _dPos );  //abs move with AutoRun Vel.
        void GoIncRun (double _dPos );  //Inc move with AutoRun ManualVel. 모션돈 확인은 GetStopInpos() 함수로.

        bool GoAbsSlow(double _dPos );  //abs move with AutoRun Slow Vel.
        void GoIncSlow(double _dPos );  //Inc move with AutoRun Slow ManualVel. 모션돈 확인은 GetStopInpos() 함수로.

        bool GoAbsMan (double _dPos );  //abs move with Manual Vel.
        void GoIncMan (double _dPos );  //Inc move with Manual Vel. 모션돈 확인은 GetStopInpos() 함수로.

        bool GoAbsAcDc(double _dPos , double _dVel , double _dAcc , double _dDcc);
        void GoIncAcDc(double _dPos , double _dVel , double _dAcc , double _dDcc);


        bool FindEdgeStop   (double _dVel ,double _dAcc ,CAjinMotion::EN_FINDEDGE_TYPE _ftFindEdgeType);
        bool FindEdgeEmgStop(double _dVel ,double _dAcc ,CAjinMotion::EN_FINDEDGE_TYPE _ftFindEdgeType);

        //Parameter Memory Handling Fuctions.
        void SetPara    (SPara    &_Para   ); SPara     * GetPara    ();
        void SetUsrPara (SUsrPara &_UsrPara); SUsrPara  * GetUsrPara ();
        void SetRptPara (SRptPara &_RptPara); SRptPara  * GetRptPara ();
                                              SStat     * GetStat    ();
                                              SHomeStat * GetHomeStat();
                                              SRptStat  * GetRptStat ();

	//Save Para Functions.
        void Save();
        void Load();

        AnsiString GetName();//{return Para.sMotorName;}
        int        GetPhyscAdd();

        EN_MTOR_DIRECTION GetDirType();

        //For AxtFS20 Only.
        void SetAjinTrgPos  (int _iPosCnt , double * _dPos , double _dTrgTime , bool _bActual ,bool _bOnLevel);
        void ResetAjinTrgPos();
        void OneShotAjinTrg (bool _bUpEg , int _iTime);

        //AXL
        void SetIntrptPos    (double _dPos, PosCallbackFunc _pCallback);
        void ResetIntrpt     ();



        void UpdateStat(TLabel *_lbServoOn     ,
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


        void UpdatePara(bool         _bToTable        ,
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
                        TCheckBox  * _cbSrvBreakType  );

        void UpdateUsrPara(bool        _bToTable         ,
                           TCheckBox * _cbUseRate        ,  //가속율 사용.
                           TEdit     * _edAcceleration   ,  //가속
                           TEdit     * _edDeceleration   ,  //감속
                           TEdit     * _edRunSpeed       ,  //구동속도
                           TEdit     * _edSlowSpeed      ,  //구동느린속도.
                           TEdit     * _edManSpeed       ,  //메뉴얼 구동속도
                           TEdit     * _edHomeSpeed      ,  //홈구동 속도.
                           TEdit     * _edLastHomeSpeed  ,  //라스트홈구동 속도.
                           TEdit     * _edJogSpeed       ,  //조그구동 속도.
                           TEdit     * _edInposition     ,  //인포지션.
                           TEdit     * _edHomeOffset     ); //홈 오프셑.

        void UpdateRptPara(   //Repeat Test Para (UI)
                           bool     _bToTable    ,
                           TEdit *  _edFstPos    ,  //첫위치
                           TEdit *  _edScdPos    ,  //두번째위치.
                           TEdit *  _edStopDelay ); //모션돈 후 정지 딜레이.


        //산메이 모터....삽질 세트

        double     GetRptTickTime();

};

//---------------------------------------------------------------------------
#endif
