//---------------------------------------------------------------------------
#ifndef Ajin_MotionH
#define Ajin_MotionH


//SUN Kye Won 2010.03.14
//��� ������ �������� AJIN���� ������ FS20 ������ ���� Ŭ����
//��� ��� ������ ���ϼ��� ���ؼ� �޽� ������ �����ϰ�
//�����ܿ����� ����Ͽ� Unit���� �����Ѵ�.
//---------------------------------------------------------------------------
//System Head.
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>

#include "TEdit.h"
#include "SMDllDefine.h"

#include "Define.h"
#ifdef AJIN_AXT

    #include "AxtCAMCFS20.h"

    //AXT Trigger
    #pragma comment(lib, "AxtTriggerLib.lib")     // Ajinextek Trigger Library
    #include "AXTKeLIB.h"             // Base Board�� 1�� Low Level �Լ��� ����ϱ� ����
    #include "AxtKeCamcFs20.h"        // FS20�� 1�� Low Level �Լ��� ����ϱ� ����
    #include "AxtTrigger.h"           // Tirgger Function ����� ���� �������

#endif //AJIN_AXT

#ifdef AJIN_AXL
    #include "AXM.h"

    #include "AXDev.h"
    #include "AXHD.h"
    #include "AXHS.h"
#endif //AJIN_AXL


//UI Set
struct SAjinParaUi {
    TPanel * pnBase ;

    TEdit     * edPhysicalNo    ; TLabel * lbPhysicalNo  ;
    TCheckBox * cbUseSCurve     ;
    TCheckBox * cbEncReverse    ;
    TCheckBox * cbUse360Count   ;

    TComboBox * cbEncInput      ; TLabel * lbEncInput    ;
    TComboBox * cbPulseOutput   ; TLabel * lbPulseOutput ;
    TEdit     * edSCurveAcPer   ; TLabel * lbSCurveAcPer ;
    TEdit     * edSCurveDcPer   ; TLabel * lbSCurveDcPer ;

    TCheckBox * cbHomeActLevel  ;
    TCheckBox * cbPLimActLevel  ;
    TCheckBox * cbNLimActLevel  ;
    TCheckBox * cbAlarmActLevel ;
    TCheckBox * cbInposActLevel ;
    TCheckBox * cbZphaActLevel  ;
};
extern SAjinParaUi AjinParaUi ;
void MakeAjinUI      ();

//void SetArrange  (TControl * _ctCon , int _iCol , int _iRow);
void SetAjinArrange (TControl    * _pnPanel    , int _iCol , int _iRow);
//void SetArrange (TComboBox * _cbComboBox , int _iCol , int _iRow);
//void SetArrange (TEdit     * _edEdit     , int _iCol , int _iRow);
//void SetArrange (TLabel    * _lbLabel    , int _iCol , int _iRow);
//void SetArrange (TCheckBox * _cbCheckBox , int _iCol , int _iRow);

//void SetUIParent (TPanel * _pnBase);
void SetAjinUIParent (TPanel * _pnBase);
void DeleteAjinUI    ();


//Global Function
int  GetAjinAxisCnt();
bool InitAjinBoard ();
void CloseAjinBoard();

class CAjinMotion
{
    public:
        CAjinMotion(int _iAxisNo);
        virtual ~CAjinMotion();
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

        struct SAjinPara{ //Board Level Direct Access AjinPara. (INI)
            int    iPhysicalNo   ; //���� ���� ������ ��ġ.
            bool   bUseSCurve    ; //S-Curve Profile �̵����.
            bool   bEncReverse   ; //CFS20set_enc_reverse      // ���ڴ� �Է� ��ġ ����. RM������ �Ⱦ���.
            bool   bUse360Count  ; //0~360�� ������ �������� ���� ��. RM������ �س���.
            int    iEncInput     ; //CFS20set_enc_input_method // ���ڴ� �Է� ��� ����(ü�輳��) EXTERNAL_COUNTER_INPUT
            int    iPulseOutput  ; //CFS20set_pulse_out_method // �޽� ��� ��� ����             PULSE_OUTPUT
            double dSCurveAcPer  ; //CFS20set_s_rate           // S-Curve���� Acc Percent , default 100%
            double dSCurveDcPer  ; //CFS20set_s_rate           // S-Curve���� Dec Percent , default 100%
            bool   bPLimActLevel ; //CFS20set_pend_limit_level // ������ ����Ʈ(+End limit)�� ��Ƽ�극�� ����
            bool   bNLimActLevel ; //CFS20set_nend_limit_level // ������ ����Ʈ(-End limit)�� ��Ƽ�극�� ����
            bool   bAlarmActLevel; //CFS20set_alarm_level      // �˶�(Alarm) ��ȣ ��Ƽ�극�� ����
            bool   bInposActLevel; //CFS20set_inposition_level // ��������(Inposition) ��ȣ ��Ƽ�극�� ����

            //�������� ���� �ϵ��� ����.
            bool   bHomeActLevel ; //
            bool   bZphaActLevel ; //CFS20set_nend_limit_level // ������ ����Ʈ(-End limit)�� ��Ƽ�극�� ����



          //bool   bEndLimEnable ; //CFS20set_end_limit_enable // ������ ����Ʈ ��� ��������� ����
          //bool   bAlarmEnable  ; //CFS20set_alarm_enable     // ���� �˶� �Է½�ȣ ����� ��������� ����
          //bool   bInposEnable  ; //CFS20set_inposition_enable// ��������(Inposition) ��ȣ ������� ����
          //double dMaxSpeed     ; //CFS20set_max_speed        // �������ǵ� �Ѱ� ����.
        };

        SAjinPara AjinPara   ;
    protected:
        int      m_iAxisNo ;

//        SAjinPara AjinPara   ;

        bool     m_bUseAccRate ; //�������� ���� ����.

        //Apply AXT Liblary Directly Access AjinPara
        void ApplyAjinPara ();
        bool InitLib (); //Library Initial.

    public:
        void SetUseAccRate(bool _bUseRate);
        void SetSignalInpos(DWORD _dwSel);  //0 : ����  1:������ 3:�Ⱦ�.

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

        bool JogP(double _dVel , double _dAcc , double _dDec); //Jogging to CW.
        bool JogN(double _dVel , double _dAcc , double _dDec); //Jogging to CCW.

        bool GoAbs(double _dPos , double _dVel , double _dAcc , double _dDec);  //abs move
        bool GoOvr(double _dSttPos , double _dOriTrgPos , double _dNewTrgPos);  //Override move

        bool FindEdgeStop   (double _dVel ,double _dAcc , EN_FINDEDGE_TYPE _ftFindEdgeType);
        bool FindEdgeEmgStop(double _dVel ,double _dAcc , EN_FINDEDGE_TYPE _ftFindEdgeType);

        //AjinParameter Memory Handling Fuctions.
        void SetAjinPara (SAjinPara &_AjinPara );
        void GetAjinPara (SAjinPara &_AjinPara );

	//Save AjinPara Functions.
        void Save();
        void Load();

        void UpdateAjinPara(bool _bToTable);

        void SetTrgPos  (int _iPosCnt , double * _dPos , double _dTrgTime , bool _bActual ,bool _bOnLevel);    //Target Actual Position or Command Position.
        void ResetTrgPos();
        void OneShotTrg (bool _bUpEg , int _iTime);

        //AXL �� ����.
        void SetIntrptPos    (double _dPos , PosCallbackFunc _pCallback);
        void ResetIntrpt     ();
        void Set360Count     (bool _bEnable , int _iMinPosPulse , int _iMaxPosPulse);

        //AXM
        void SetOverrideMaxSpeed(double _dOverSped);
        void SetOverrideVel     (double _dOverSped);
        void SetOverrideVelAtPos(double _dPos, double _dVel, double _dAcc, double _dDec, double _dOverPos, double _dOverSped, bool _bCmd = false);

        // ���� ������ ���� �߰�
        static void ContiSetAxisMap    (int _iCoord, int _iAxisCnt, int *_ipAxisNo);
        static void ContiSetAbsRelMode (int _iCoord , DWORD _dwAbsRelMode);
        static void ContiBeginNode     (int _iCoord);
        static void ContiEndNode       (int _iCoord);
        static void ContiStart         (int _iCoord, DWORD _dwProfileset, int _iAngle);
        static int  GetContCrntIdx     (int _iCoord);
        static void LineMove           (int _iCoord, double *_dpEndPos, double _dVel, double _dAcc, double _dDec);
        static void CircleCenterMove   (int _iCoord, int *_ipAxisNo, double *_dpCenterPos, double *_dpEndPos, double _dVel, double _dAcc, double _dDec, DWORD _dwCWDir);
        static void CirclePointMove    (int _iCoord, int *_ipAxisNo, double *_dpMidPos, double *_dpEndPos, double _dVel, double _dAcc, double _dDec, int _iArcCircle);
        static void CircleRadiusMove   (int _iCoord, int *_ipAxisNo, double _dRadius, double *_dpEndPos, double _dVel, double _dAcc, double _dDec, DWORD _dwCWDir, DWORD _dwShortDistance);
        static void CircleAngleMove    (int _iCoord, int *_ipAxisNo, double *_dCenterPos, double _dAngle, double _dVel, double _dAcc, double _dDec, DWORD _dwCWDir);

        //��Ʈ��..����.
        static void SetGantryEnable    (int _iMasterAxis , int _iSlaveAxis , int _iHomeUse , double _dSlaveOffset , double _dSlOffsetRange);
        static void SetGantryDisable   (int _iMasterAxis , int _iSlaveAxis);

        //��Ʈ�� ���� ������ ��ߵǴ�  Ȩ�Լ���.
        static void SetHomeMethod      (int _iAxis , int _iHomeDir , DWORD _dwHomeSignal , DWORD _dwUseZ , double _dHomeClrTime , double _dHomeOffset);
        static void SetHomeVel         (int _iAxis , double _dVel1 , double _dVel2 , double _dVel3 , double _dLast ,double _dAcc1 , double _dAcc2);
        static void SetHomeStart       (int _iAxis);
        static void GetHomeResult      (int _iAxis , DWORD * _dwResult);
};

#endif
