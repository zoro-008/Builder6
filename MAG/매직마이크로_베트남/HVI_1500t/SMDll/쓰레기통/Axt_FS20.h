//---------------------------------------------------------------------------
#ifndef Axt_FS20H
#define Axt_FS20H

#include "Define.h"
#ifdef AJIN_AXT

//SUN Kye Won 2010.03.14
//��� ������ �������� AJIN���� ������ FS20 ������ ���� Ŭ����
//��� ��� ������ ���ϼ��� ���ؼ� �޽� ������ �����ϰ�
//�����ܿ����� ����Ͽ� Unit���� �����Ѵ�.
//---------------------------------------------------------------------------
//System Head.
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>

//Ajin header
#include "AxtLIBDef.h"
#include "CAMCFSDef.h"
#include "TEdit.h"


//UI Set
struct SAxtParaUi {
    TPanel * pnBase ;

    TEdit     * edPhysicalNo    ; TLabel * lbPhysicalNo  ;
    TCheckBox * cbUseSCurve     ;
    TCheckBox * cbEncReverse    ;

    TComboBox * cbEncInput      ; TLabel * lbEncInput    ;
    TComboBox * cbPulseOutput   ; TLabel * lbPulseOutput ;
    TEdit     * edSCurveAcPer   ; TLabel * lbSCurveAcPer ;
    TEdit     * edSCurveDcPer   ; TLabel * lbSCurveDcPer ;

    TCheckBox * cbHomeActLevel  ;
    TCheckBox * cbPLimActLevel  ;
    TCheckBox * cbNLimActLevel  ;
    TCheckBox * cbAlarmActLevel ;
    TCheckBox * cbInposActLevel ;
};
extern SAxtParaUi AxtParaUi ;
void MakeAxtUI      ();

//void SetArrange  (TControl * _ctCon , int _iCol , int _iRow);
void SetAxtArrange (TControl    * _pnPanel    , int _iCol , int _iRow);
//void SetArrange (TComboBox * _cbComboBox , int _iCol , int _iRow);
//void SetArrange (TEdit     * _edEdit     , int _iCol , int _iRow);
//void SetArrange (TLabel    * _lbLabel    , int _iCol , int _iRow);
//void SetArrange (TCheckBox * _cbCheckBox , int _iCol , int _iRow);

//void SetUIParent (TPanel * _pnBase);
void SetAxtUIParent (TPanel * _pnBase);
void DeleteAxtUI    ();


//Global Function
int  GetAxtFS20AxisCnt();
bool InitAxtFS20Board ();
void CloseAxtFS20Board();

class CAxtFS20
{
    public:
        CAxtFS20(int _iAxisNo);
        virtual ~CAxtFS20();
        enum EN_FINDEDGE_TYPE {
            ftPELDnEdge = 0x0, // +Elm(End limit) �ϰ� edge
            ftNELDnEdge = 0x1, // -Elm(End limit) �ϰ� edge
            ftPSLDnEdge = 0x2, // +Slm(Slowdown limit) �ϰ� edge
            ftNSLDnEdge = 0x3, // -Slm(Slowdown limit) �ϰ� edge
            ftOrgDnEdge = 0x4, // IN0(ORG) �ϰ� edge
            ftZphDnEdge = 0x5, // IN1(Z��) �ϰ� edge
            ftIn2DnEdge = 0x6, // IN2(����) �ϰ� edge
            ftIn3DnEdge = 0x7, // IN3(����) �ϰ� edge
            ftPELUpEdge = 0x8, // +Elm(End limit) ��� edge
            ftNELUpEdge = 0x9, // -Elm(End limit) ��� edge
            ftPSLUpEdge = 0xa, // +Slm(Slowdown limit) ��� edge
            ftNSLUpEdge = 0xb, // -Slm(Slowdown limit) ��� edge
            ftOrgUpEdge = 0xc, // IN0(ORG) ��� edge
            ftZphUpEdge = 0xd, // IN1(Z��) ��� edge
            ftIn2UpEdge = 0xe, // IN2(����) ��� edge
            ftIn3UpEdge = 0xf, // IN3(����) ��� edge

            MAX_FINDEDGE_TYPE
        };

        struct SAxtPara{ //Board Level Direct Access AxtPara. (INI)
            int    iPhysicalNo   ; //���� ���� ������ ��ġ.
            bool   bUseSCurve    ; //S-Curve Profile �̵����.
            bool   bEncReverse   ; //CFS20set_enc_reverse      // ���ڴ� �Է� ��ġ ����.
            int    iEncInput     ; //CFS20set_enc_input_method // ���ڴ� �Է� ��� ����(ü�輳��) EXTERNAL_COUNTER_INPUT
            int    iPulseOutput  ; //CFS20set_pulse_out_method // �޽� ��� ��� ����             PULSE_OUTPUT
            double dSCurveAcPer  ; //CFS20set_s_rate           // S-Curve���� Acc Percent , default 100%
            double dSCurveDcPer  ; //CFS20set_s_rate           // S-Curve���� Dec Percent , default 100%

            //�������� ���� �ϵ��� ����.
            bool   bHomeActLevel ; //
            bool   bPLimActLevel ; //CFS20set_pend_limit_level // ������ ����Ʈ(+End limit)�� ��Ƽ�극�� ����
            bool   bNLimActLevel ; //CFS20set_nend_limit_level // ������ ����Ʈ(-End limit)�� ��Ƽ�극�� ����
//          bool   bZphaActLevel ; //CFS20set_nend_limit_level // ������ ����Ʈ(-End limit)�� ��Ƽ�극�� ����
            bool   bAlarmActLevel; //CFS20set_alarm_level      // �˶�(Alarm) ��ȣ ��Ƽ�극�� ����
            bool   bInposActLevel; //CFS20set_inposition_level // ��������(Inposition) ��ȣ ��Ƽ�극�� ����

          //bool   bEndLimEnable ; //CFS20set_end_limit_enable // ������ ����Ʈ ��� ��������� ����
          //bool   bAlarmEnable  ; //CFS20set_alarm_enable     // ���� �˶� �Է½�ȣ ����� ��������� ����
          //bool   bInposEnable  ; //CFS20set_inposition_enable// ��������(Inposition) ��ȣ ������� ����
          //double dMaxSpeed     ; //CFS20set_max_speed        // �������ǵ� �Ѱ� ����.
        };

    protected:
        int      m_iAxisNo ;

        SAxtPara AxtPara   ;

        //Apply AXT Liblary Directly Access AxtPara
        void ApplyAxtPara ();
        bool InitLib (); //Library Initial.

    public:
        bool GetStop (); //Motion Done.

        //Extended IO Bit.
        void SetServo(bool _bOn) ; bool GetServo();
        void SetReset(bool _bOn) ;

        //Position Functions.
        double GetCmdPos(); //Get Command PULSE.
        double GetEncPos(); //Get Encoder PULSE.
        void   SetPos   (double _dPos); //Set Any Position.

        //Signal Input.
        bool GetHomeSnsr  ();
        bool GetNLimSnsr  ();
        bool GetPLimSnsr  ();
        bool GetZphaseSnsr();
        bool GetPackInPos (); //Servo Pack InPosition Signal.
        bool GetAlarm     ();

        bool GetHomeDone ();

        //Motion Functions.
        bool Stop   ();
        bool EmgStop(); //Stop Without Deceleration.

        bool JogP(double _dVel , double _dAcc); //Jogging to CW.
        bool JogN(double _dVel , double _dAcc); //Jogging to CCW.

        bool GoAbs(double _dPos , double _dVel , double _dAcc);  //abs move
        bool GoOvr(double _dPos                              );  //Override move

        bool FindEdgeStop   (double _dVel ,double _dAcc ,EN_FINDEDGE_TYPE _ftFindEdgeType);
        bool FindEdgeEmgStop(double _dVel ,double _dAcc ,EN_FINDEDGE_TYPE _ftFindEdgeType);

        //AxtParameter Memory Handling Fuctions.
        void SetAxtPara (SAxtPara &_AxtPara );
        void GetAxtPara (SAxtPara &_AxtPara );

	//Save AxtPara Functions.
        void Save();
        void Load();

        void UpdateAxtPara(bool _bToTable);

        void SetTrgPos  (int _iPosCnt , double * _dPos , double _dTrgTime , bool _bActual ,bool _bOnLevel);    //Target Actual Position or Command Position.
        void ResetTrgPos();
        void OneShotTrg (bool _bUpEg , int _iTime);
};

#endif // AJIN_AXT
#endif
