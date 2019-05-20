//---------------------------------------------------------------------------

#ifndef Axt_FS20H
#define Axt_FS20H
//SUN Kye Won 2010.12.20
//---------------------------------------------------------------------------
//System Head.
//#include <system.hpp>
#include <StdCtrls.hpp>
//#include <Classes.hpp>

//Ajin header
#include "AxtLIBDef.h"
#include "CAMCFSDef.h"
#include "Timer.h"
#include "TEdit.h"


//Global Function
int  GetAxtFS20AxisCnt();
bool InitAxtFS20Board ();
void CloseAxtFS20Board();

class CAxtFS20
{
    public:
        CAxtFS20();
        virtual ~CAxtFS20();

        struct CAxtPara{ //Board Level Direct Access Para. (INI)
            int    iEncInput     ;   //CFS20set_enc_input_method // 엔코더 입력 방식 설정(체배설정) EXTERNAL_COUNTER_INPUT
            int    iPulseOutput  ;   //CFS20set_pulse_out_method // 펄스 출력 방식 설정             PULSE_OUTPUT
            bool   bEndLimEnable ;   //CFS20set_end_limit_enable // 급정지 리미트 기능 사용유무를 설정
            bool   bPEndLimLevel ;   //CFS20set_pend_limit_level // 정방향 리미트(+End limit)의 액티브레벨 설정
            bool   bNEndLimLevel ;   //CFS20set_nend_limit_level // 역방향 리미트(-End limit)의 액티브레벨 설정
            bool   bAlramEnable  ;   //CFS20set_alarm_enable     // 서보 알람 입력신호 기능의 사용유무를 설정
            bool   bAlramLevel   ;   //CFS20set_alarm_level      // 알람(Alarm) 신호 액티브레벨 설정
            bool   bInposEnable  ;   //CFS20set_inposition_enable// 인포지션(Inposition) 신호 사용유무 설정
            bool   bInposLevel   ;   //CFS20set_inposition_level // 인포지션(Inposition) 신호 액티브레벨 설정
            bool   bEncReverse   ;   //CFS20set_enc_reverse      // 엔코더 입력 수치 반전.
            double dMaxSpeed     ;   //CFS20set_max_speed        // 구동스피드 한계 설정.
            double dSCurveAcPer  ;   //CFS20set_s_rate           // S-Curve구동 Acc Percent , default 100%
            double dSCurveDcPer  ;   //CFS20set_s_rate           // S-Curve구동 Dec Percent , default 100%
        } ;

        struct CPara { //Class Level Para. (INI)
            AnsiString sMotorName   ; //모터이름 UI용.
            int        iPhysicalNo  ; //축의 실제 물리적 위치.
            bool       bExistEnc    ; //엔코더가 있는지 없는지.
            int        iPulsePerRot ; //한바퀴의 펄스수.
            double     dUnitPerRot  ; //한바퀴당 이동거리 혹은 각도.
            bool       bHomeLevel   ; //홈센서의 레벨.
            bool       bUseHomeSnsr ; //홈센서로 홈을 잡을 것인지 -로 홈을 잡을 것인지.
            bool       bUseSCurve   ; //S-Curve Profile 이동사용.
            bool       bSrvOnLevel  ; //ServoOn Output Level설정.
            double     dMaxPos      ; //MAX Position.
            double     dMinPos      ; //MIN Position.
        } ;

        struct CUsrPara { //User Display Para. (UI)
            double  dAccTime    ; //가감속시간.
            double  dRunSpeed   ; //구동속도
            double  dManSpeed   ; //메뉴얼 구동속도
            double  dHomeSpeed  ; //홈구동 속도.
            double  dJogSpeed   ; //조그구동 속도.
            double  dInposition ; //인포지션.
            double  dHomeOffset ; //홈 오프셑.
        } ;

        struct CRptPara {  //Repeat Test Para (UI)
            double  dFstPos      ; //첫위치
            double  dScdPos      ; //두번째위치.
            double  dStopDelay   ; //모션돈 후 정지 딜레이.
        } ;

        struct CStat { //Motor Status for Display.
            //Motion Status.
            bool   bServoOn      ;
            bool   bHomeSnsr     ;
            bool   bPEndLimSnsr  ;
            bool   bNEndLimSnsr  ;
            bool   bHomeEnded    ;
            bool   bAlarm        ;
            bool   bPackInPosn   ;
            bool   bStop         ;
            bool   bBusy         ;
            bool   bReady        ;
            bool   bOk           ;
            bool   bJogP         ;
            bool   bJogN         ;

            //Pos Stat
            double dTrgPos       ;
            double dCmdPos       ;
            double dEncPos       ;
        } ;

        struct CHomeStat{
            int  iHomeStep     ;
            bool bGoHome       ;
            bool bHomeTimeout  ;
        } ;

        struct CRptStat{
            bool bGoRpt        ;
        } ;

        struct CVibStat{
            int    iStep   ;
            int    iCnt    ;
            double dOriPos ;
        } ;



    protected:
        bool InitLib  () ; //Library Initial.
        bool CycleHome() ; //Home Cycle Processing.
        void CycleRpt () ; //Repeat Cycle Processing.
        bool GetStop  () ; //Motion Done. 서보팩 튜닝이 안된상태에서 외부에서 쓰면 위험할수 있어서 protected로 선언. GetStopInpos로 쓸것.

        CAxtPara   AxtPara  ;
        CPara      Para     ;
        CUsrPara   UsrPara  ;
        CStat      Stat     ;
        CHomeStat  HomeStat ;
        CRptPara   RptPara  ;
        CRptStat   RptStat  ;
        CVibStat   VibStat  ;

        CDelayTimer  m_tmHomeCycle ;

    public:
        //Thread Cycle Func.
        void Update ();

        //Apply AXT Liblary Directly Access Para
        void ApplyAxtPara ();

        //Set Stat Directly Func.
        void SetHomeEnd (bool _bValue ); bool GetHomeEnd ();
        void SetServo   (bool _bOn    ); bool GetServo   ();
        void SetAlarm   (bool _bOn    ); bool GetAlarm   ();
                                         bool GetNLim    ();
                                         bool GetPLim    ();

        //Update Motor Status.
        bool GetStopInpos() ; //Check Motion Done with User Inposition.
        bool GetReady    () ; //Check ready to move.
        bool GetOk       () ; //Check No Limit & Servo Enable.

        //Position Functions.
        double GetTrgPos();                                       //Get Target  Position.
        double GetCmdPos();                                       //Get Command Position.
        double GetEncPos();                                       //Get Encoder Position.
        double GetErrPos();                                       //Get Control Error Position.
        void   ClearPos ();                                       //Clear Position.
        void   SetPos   (double _dPos = 0.0);                     //Set Any Position.

        //---------------------------------------------------------------------------
        bool CmprPos(double _dPos, double _dRange = 0.0);

        //Motion Functions.
        void Reset   (); //Reset
        bool Stop    ();
        bool EmgStop (); //Stop Without Deceleration.

        bool Home   (); //Start Home Cycle for Sequense Cycle.
        void DoHome (); //Start Home Cycle for Click Event.

        void DoRpt   (); //Start Home Cycle for Click Event.
        bool Vibration(double _dRange , double _dVel , int _iCnt , bool _bCntClr);

        bool JogP(); //Jogging to CW.
        bool JogN(); //Jogging to CCW.

        bool GoAbs (double _dPos , double _dVel);  //abs move
        void GoInc (double _dPos , double _dVel);  //Inc move  모션돈 확인은 GetStopInpos() 함수로.

        bool GoAbsRun (double _dPos );  //abs move with AutoRun Vel.
        void GoIncRun (double _dPos );  //Inc move with AutoRun ManualVel. 모션돈 확인은 GetStopInpos() 함수로.

        bool GoAbsMan (double _dPos );  //abs move with Manual Vel.
        void GoIncMan (double _dPos );  //Inc move with Manual Vel. 모션돈 확인은 GetStopInpos() 함수로.

        //Parameter Memory Handling Fuctions.
        void SetAxtPara  (CAxtPara &_AxtPara); void GetAxtPara  (CAxtPara &_AxtPara );
        void SetPara     (CPara    &_Para   ); void GetPara     (CPara    &_Para    );
        void SetUsrPara  (CUsrPara &_UsrPara); void GetUsrPara  (CUsrPara &_UsrPara );
        void SetRptPara  (CRptPara &_RptPara); void GetRptPara  (CRptPara &_RptPara );
                                               void GetStat     (CStat    &_Stat    );
                                               void GetHomeStat (CStat    &_HomeStat);
                                               void GetRptStat  (CRptPara &_RptPara );

	//Save Para Functions.
        void Save(int _iAxisNo);
        void Load(int _iAxisNo);

        AnsiString GetName(){return Para.sMotorName;}

        void UpdateStat(TLabel *_lbServoOn     ,
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

        void UpdatePara(bool    _bToTable     ,
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
#endif
