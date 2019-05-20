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
            int    iEncInput     ;   //CFS20set_enc_input_method // ���ڴ� �Է� ��� ����(ü�輳��) EXTERNAL_COUNTER_INPUT
            int    iPulseOutput  ;   //CFS20set_pulse_out_method // �޽� ��� ��� ����             PULSE_OUTPUT
            bool   bEndLimEnable ;   //CFS20set_end_limit_enable // ������ ����Ʈ ��� ��������� ����
            bool   bPEndLimLevel ;   //CFS20set_pend_limit_level // ������ ����Ʈ(+End limit)�� ��Ƽ�극�� ����
            bool   bNEndLimLevel ;   //CFS20set_nend_limit_level // ������ ����Ʈ(-End limit)�� ��Ƽ�극�� ����
            bool   bAlramEnable  ;   //CFS20set_alarm_enable     // ���� �˶� �Է½�ȣ ����� ��������� ����
            bool   bAlramLevel   ;   //CFS20set_alarm_level      // �˶�(Alarm) ��ȣ ��Ƽ�극�� ����
            bool   bInposEnable  ;   //CFS20set_inposition_enable// ��������(Inposition) ��ȣ ������� ����
            bool   bInposLevel   ;   //CFS20set_inposition_level // ��������(Inposition) ��ȣ ��Ƽ�극�� ����
            bool   bEncReverse   ;   //CFS20set_enc_reverse      // ���ڴ� �Է� ��ġ ����.
            double dMaxSpeed     ;   //CFS20set_max_speed        // �������ǵ� �Ѱ� ����.
            double dSCurveAcPer  ;   //CFS20set_s_rate           // S-Curve���� Acc Percent , default 100%
            double dSCurveDcPer  ;   //CFS20set_s_rate           // S-Curve���� Dec Percent , default 100%
        } ;

        struct CPara { //Class Level Para. (INI)
            AnsiString sMotorName   ; //�����̸� UI��.
            int        iPhysicalNo  ; //���� ���� ������ ��ġ.
            bool       bExistEnc    ; //���ڴ��� �ִ��� ������.
            int        iPulsePerRot ; //�ѹ����� �޽���.
            double     dUnitPerRot  ; //�ѹ����� �̵��Ÿ� Ȥ�� ����.
            bool       bHomeLevel   ; //Ȩ������ ����.
            bool       bUseHomeSnsr ; //Ȩ������ Ȩ�� ���� ������ -�� Ȩ�� ���� ������.
            bool       bUseSCurve   ; //S-Curve Profile �̵����.
            bool       bSrvOnLevel  ; //ServoOn Output Level����.
            double     dMaxPos      ; //MAX Position.
            double     dMinPos      ; //MIN Position.
        } ;

        struct CUsrPara { //User Display Para. (UI)
            double  dAccTime    ; //�����ӽð�.
            double  dRunSpeed   ; //�����ӵ�
            double  dManSpeed   ; //�޴��� �����ӵ�
            double  dHomeSpeed  ; //Ȩ���� �ӵ�.
            double  dJogSpeed   ; //���ױ��� �ӵ�.
            double  dInposition ; //��������.
            double  dHomeOffset ; //Ȩ �����V.
        } ;

        struct CRptPara {  //Repeat Test Para (UI)
            double  dFstPos      ; //ù��ġ
            double  dScdPos      ; //�ι�°��ġ.
            double  dStopDelay   ; //��ǵ� �� ���� ������.
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
        bool GetStop  () ; //Motion Done. ������ Ʃ���� �ȵȻ��¿��� �ܺο��� ���� �����Ҽ� �־ protected�� ����. GetStopInpos�� ����.

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
        void GoInc (double _dPos , double _dVel);  //Inc move  ��ǵ� Ȯ���� GetStopInpos() �Լ���.

        bool GoAbsRun (double _dPos );  //abs move with AutoRun Vel.
        void GoIncRun (double _dPos );  //Inc move with AutoRun ManualVel. ��ǵ� Ȯ���� GetStopInpos() �Լ���.

        bool GoAbsMan (double _dPos );  //abs move with Manual Vel.
        void GoIncMan (double _dPos );  //Inc move with Manual Vel. ��ǵ� Ȯ���� GetStopInpos() �Լ���.

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
