//---------------------------------------------------------------------------
#ifndef Ajin_MotionH
#define Ajin_MotionH


//SUN Kye Won 2010.03.14
//모션 최하위 유닛으로 AJIN사의 구형인 FS20 버젼의 랩핑 클래스
//모든 모션 단위는 통일성을 위해서 펄스 단위로 구동하고
//상위단에서는 계산하여 Unit으로 구동한다.
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
    #include "AXTKeLIB.h"             // Base Board의 1차 Low Level 함수를 사용하기 위해
    #include "AxtKeCamcFs20.h"        // FS20의 1차 Low Level 함수를 사용하기 위해
    #include "AxtTrigger.h"           // Tirgger Function 사용을 위한 헤더파일

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
            ftPELDnEdge = 0x0, // +Elm(End limit) 하강 edge
            ftNELDnEdge = 0x1, // -Elm(End limit) 하강 edge
            ftPSLDnEdge = 0x2, // +Slm(Slowdown limit) 하강 edge
            ftNSLDnEdge = 0x3, // -Slm(Slowdown limit) 하강 edge
            ftOrgDnEdge = 0x4, // IN0(ORG) 하강 edge
            ftZphDnEdge = 0x5, // IN1(Z상) 하강 edge
            ftIn2DnEdge = 0x6, // IN2(범용) 하강 edge
            ftIn3DnEdge = 0x7, // IN3(범용) 하강 edge
            ftPELUpEdge = 0x8, // +Elm(End limit) 상승 edge
            ftNELUpEdge = 0x9, // -Elm(End limit) 상승 edge
            ftPSLUpEdge = 0xa, // +Slm(Slowdown limit) 상승 edge
            ftNSLUpEdge = 0xb, // -Slm(Slowdown limit) 상승 edge
            ftOrgUpEdge = 0xc, // IN0(ORG) 상승 edge
            ftZphUpEdge = 0xd, // IN1(Z상) 상승 edge
            ftIn2UpEdge = 0xe, // IN2(범용) 상승 edge
            ftIn3UpEdge = 0xf, // IN3(범용) 상승 edge

            MAX_FINDEDGE_TYPE
        };

        struct SAjinPara{ //Board Level Direct Access AjinPara. (INI)
            int    iPhysicalNo   ; //축의 실제 물리적 위치.
            bool   bUseSCurve    ; //S-Curve Profile 이동사용.
            bool   bEncReverse   ; //CFS20set_enc_reverse      // 엔코더 입력 수치 반전.
            int    iEncInput     ; //CFS20set_enc_input_method // 엔코더 입력 방식 설정(체배설정) EXTERNAL_COUNTER_INPUT
            int    iPulseOutput  ; //CFS20set_pulse_out_method // 펄스 출력 방식 설정             PULSE_OUTPUT
            double dSCurveAcPer  ; //CFS20set_s_rate           // S-Curve구동 Acc Percent , default 100%
            double dSCurveDcPer  ; //CFS20set_s_rate           // S-Curve구동 Dec Percent , default 100%
            bool   bPLimActLevel ; //CFS20set_pend_limit_level // 정방향 리미트(+End limit)의 액티브레벨 설정
            bool   bNLimActLevel ; //CFS20set_nend_limit_level // 역방향 리미트(-End limit)의 액티브레벨 설정
            bool   bAlarmActLevel; //CFS20set_alarm_level      // 알람(Alarm) 신호 액티브레벨 설정
            bool   bInposActLevel; //CFS20set_inposition_level // 인포지션(Inposition) 신호 액티브레벨 설정

            //상위에서 제어 하도록 빠짐.
            bool   bHomeActLevel ; //
            bool   bZphaActLevel ; //CFS20set_nend_limit_level // 역방향 리미트(-End limit)의 액티브레벨 설정



          //bool   bEndLimEnable ; //CFS20set_end_limit_enable // 급정지 리미트 기능 사용유무를 설정
          //bool   bAlarmEnable  ; //CFS20set_alarm_enable     // 서보 알람 입력신호 기능의 사용유무를 설정
          //bool   bInposEnable  ; //CFS20set_inposition_enable// 인포지션(Inposition) 신호 사용유무 설정
          //double dMaxSpeed     ; //CFS20set_max_speed        // 구동스피드 한계 설정.
        };

        SAjinPara AjinPara   ;
    protected:
        int      m_iAxisNo ;

//        SAjinPara AjinPara   ;

        bool     m_bUseAccRate ; //가속율로 쓰는 설정.

        //Apply AXT Liblary Directly Access AjinPara
        void ApplyAjinPara ();
        bool InitLib (); //Library Initial.

    public:
        void SetUseAccRate(bool _bUseRate);

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

        //AXL 트리거 검증 안함.
        void SetTrgPos  (int _iPosCnt , double * _dPos , double _dTrgTime , bool _bActual ,bool _bOnLevel);    //Target Actual Position or Command Position.
        void ResetTrgPos();
        void OneShotTrg (bool _bUpEg , int _iTime);

        //AXL 만 구현.
        void SetIntrptPos    (double _dPos , PosCallbackFunc _pCallback);
        void ResetIntrpt     ();
};

#endif
