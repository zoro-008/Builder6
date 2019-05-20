//---------------------------------------------------------------------------
#ifndef MotorH
#define MotorH

//---------------------------------------------------------------------------
#include "Axt_FS20.h"
#include "Timer.h"

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

        enum EN_MOTIONBORD_TYPE { mtAxtFS20 = 0 , MAX_MOTIONBORD_TYPE  };
        enum EN_HOME_METHOD     { hmOrigin  = 0 , hmNegative = 1 , hmOriginZ = 2 , hmNegativeZ = 3 , MAX_HOME_METHOD };

        struct SPara { //Master Class Level Para. (INI)
            AnsiString         sMotorName     ; //�����̸� UIȤ�� LOG��.
            int                iMotionType    ; //��� ���� Ÿ��.           EN_MOTIONBORD_TYPE
            int                iPulsePerRev   ; //�ѹ����� �޽���.
            double             dUnitPerRev    ; //�ѹ����� �̵��Ÿ� Ȥ�� ����.
            double             dMaxPos        ; //MAX Position.
            double             dMinPos        ; //MIN Position.
            bool               bExistEnc      ; //���ڴ��� �ִ��� ������.
            int                iHomeMethod    ; //Ȩ���� ��� ���. 0:Ȩ���� 1:-���� 2:Ȩ&Z 3:-����+Z   EN_HOME_METHOD
//          bool               bHomeWithNSnsr ; //Ȩ������ Ȩ�� ���� ������ -�� Ȩ�� ���� ������.
            bool               bSrvOnLevelInv ; //ServoOn Output Level����.
            int                iBreakAdd      ; //�극��ũ Ÿ�� �극��ũ Address
        } ;

        struct SUsrPara { //Engineer Class User Display Para. (UI)
            double  dAccTime    ; //�����ӽð�.
            double  dRunSpeed   ; //�����ӵ�
            double  dManSpeed   ; //�޴��� �����ӵ�
            double  dHomeSpeed  ; //Ȩ���� �ӵ�.
            double  dLastHomeSpeed  ; //Ȩ���� �ӵ�.
            double  dJogSpeed   ; //���ױ��� �ӵ�.
            double  dInposition ; //��������.
            double  dHomeOffset ; //Ȩ �����V.
        } ;

        struct SRptPara {  //Repeat Test Para (UI)
            double  dFstPos      ; //ù��ġ
            double  dScdPos      ; //�ι�°��ġ.
            double  dStopDelay   ; //��ǵ� �� ���� ������.
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
        int  m_iAxisNo ;

        CAxtFS20 * AxtFS20 ;

        void CycleRpt () ; //Repeat Cycle Processing.
        //bool GetStop  () ; //Motion Done. ������ Ʃ���� �ȵȻ��¿��� �ܺο��� ���� �����Ҽ� �־ protected�� ����. GetStopInpos�� ����.
        bool CycleHome() ;

        SPara     Para     ;
        SUsrPara  UsrPara  ;
        SRptPara  RptPara  ;
        SStat     Stat     ;
        SHomeStat HomeStat ;
        SRptStat  RptStat  ;

        CDelayTimer  m_tmHomeCycle ;

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

        //Motion Functions.
        void Reset  (); //Reset
        void Stop   ();
        void EmgStop(); //Stop Without Deceleration.

        bool Home  (); //Start Home Cycle for Sequense Cycle.
        void DoHome(); //Start Home Cycle for Click Event.

        void DoRpt(double _edFirstPos, double _edSecondPos, int _edStopDelay); //Start Home Cycle for Click Event.

        bool JogP(); //Jogging to CW.
        bool JogN(); //Jogging to CCW.

        bool GoHomeOffset (double _dPos , double _dVel);
        bool GoAbs        (double _dPos , double _dVel); //abs move
        void GoInc        (double _dPos , double _dVel); //Inc move  ��ǵ� Ȯ���� GetStopInpos() �Լ���.

        bool GoAbsRun (double _dPos );  //abs move with AutoRun Vel.
        void GoIncRun (double _dPos );  //Inc move with AutoRun ManualVel. ��ǵ� Ȯ���� GetStopInpos() �Լ���.

        bool GoAbsMan (double _dPos );  //abs move with Manual Vel.
        void GoIncMan (double _dPos );  //Inc move with Manual Vel. ��ǵ� Ȯ���� GetStopInpos() �Լ���.

        bool FindEdgeStop   (double _dVel ,double _dAcc ,CAxtFS20::EN_FINDEDGE_TYPE _ftFindEdgeType);
        bool FindEdgeEmgStop(double _dVel               ,CAxtFS20::EN_FINDEDGE_TYPE _ftFindEdgeType);

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

        void UpdatePara(
                        bool         _bToTable        ,
                        TEdit      * _edMotorName     ,
                        TComboBox  * _cbMotionType    ,
                        TEdit      * _edPulsePerRev   ,
                        TEdit      * _edUnitPerRev    ,
                        TEdit      * _edMaxPos        ,
                        TEdit      * _edMinPos        ,
                        TCheckBox  * _cbExistEnc      ,
                        TComboBox  * _cbHomeMethod    ,
//                      TCheckBox  * _cbHomeWithNSnsr ,
                        TCheckBox  * _cbSrvOnLevelInv ,
                        TEdit      * _edBreakAdd      );
        void UpdateUsrPara(
                           bool     _bToTable         ,
                           TEdit *  _edAccTime        ,  //�����ӽð�.
                           TEdit *  _edRunSpeed       ,  //�����ӵ�
                           TEdit *  _edManSpeed       ,  //�޴��� �����ӵ�
                           TEdit *  _edHomeSpeed      ,  //Ȩ���� �ӵ�.
                           TEdit *  _edLastHomeSpeed  ,  //��ƮȨ���� �ӵ�.
                           TEdit *  _edJogSpeed       ,  //���ױ��� �ӵ�.
                           TEdit *  _edInposition     ,  //��������.
                           TEdit *  _edHomeOffset     ); //Ȩ �����V.

        void UpdateRptPara(   //Repeat Test Para (UI)
                           bool     _bToTable    ,
                           TEdit *  _edFstPos    ,  //ù��ġ
                           TEdit *  _edScdPos    ,  //�ι�°��ġ.
                           TEdit *  _edStopDelay ); //��ǵ� �� ���� ������.

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

        //For AxtFS20 Only.
        void SetAxtTrgPos  (int _iPosCnt , double * _dPos , double _dTrgTime , bool _bActual ,bool _bOnLevel);
        void ResetAxtTrgPos();
        void OneShotAxtTrg (bool _bUpEg , int _iTime);

};

//---------------------------------------------------------------------------
#endif
