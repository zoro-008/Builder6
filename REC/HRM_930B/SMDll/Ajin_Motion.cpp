//---------------------------------------------------------------------------
#pragma hdrstop
#include "Ajin_Motion.h"

//System header
#include <mem.h>
#include <math.h>
/*
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
*/



//User Header
#include "Timer.h"
#include "UserIni.h"
#include "SLogUnit.h"
#include "FormMsgOk.h"

#pragma package(smart_init)





#define TRIG_DNEG 0       // Trigger Doun Edge
#define TRIG_UPEG 1       // Trigger Up Edge

SAjinParaUi AjinParaUi ;

bool bInitOk ;

void MakeAjinUI()
{
    static bool bFirst = true ;
    if(!bFirst) return ;
    bFirst = false ;

    AjinParaUi.pnBase = new TPanel(Application);

    AjinParaUi.edPhysicalNo   = new TEdit    (AjinParaUi.pnBase); AjinParaUi.edPhysicalNo   -> Parent = AjinParaUi.pnBase ;                                                                 SetAjinArrange(AjinParaUi.edPhysicalNo   , 1 , 0 );
    AjinParaUi.lbPhysicalNo   = new TLabel   (AjinParaUi.pnBase); AjinParaUi.lbPhysicalNo   -> Parent = AjinParaUi.pnBase ; AjinParaUi.lbPhysicalNo    -> Caption = "Physical No        " ; SetAjinArrange(AjinParaUi.lbPhysicalNo   , 0 , 0 );

    AjinParaUi.cbUseSCurve    = new TCheckBox(AjinParaUi.pnBase); AjinParaUi.cbUseSCurve    -> Parent = AjinParaUi.pnBase ; AjinParaUi.cbUseSCurve     -> Caption = "Use S Curve        " ; SetAjinArrange(AjinParaUi.cbUseSCurve    , 0 , 1 );

    AjinParaUi.cbEncReverse   = new TCheckBox(AjinParaUi.pnBase); AjinParaUi.cbEncReverse   -> Parent = AjinParaUi.pnBase ; AjinParaUi.cbEncReverse    -> Caption = "Enc Reverse        " ; SetAjinArrange(AjinParaUi.cbEncReverse   , 1 , 1 );
    AjinParaUi.cbUse360Count  = new TCheckBox(AjinParaUi.pnBase); AjinParaUi.cbUse360Count  -> Parent = AjinParaUi.pnBase ; AjinParaUi.cbUse360Count   -> Caption = "Enc 360 Reset      " ; SetAjinArrange(AjinParaUi.cbUse360Count  , 1 , 1 );

    AjinParaUi.cbUseGntr      = new TCheckBox(AjinParaUi.pnBase); AjinParaUi.cbUseGntr      -> Parent = AjinParaUi.pnBase ; AjinParaUi.cbUseGntr       -> Caption = "Use Gentry         " ; SetAjinArrange(AjinParaUi.cbUseGntr      , 0 , 2 );
    AjinParaUi.cbGntrMathod   = new TComboBox(AjinParaUi.pnBase); AjinParaUi.cbGntrMathod   -> Parent = AjinParaUi.pnBase ;                                                                 SetAjinArrange(AjinParaUi.cbGntrMathod   , 1 , 2 );

    AjinParaUi.lbGntrSubAdd   = new TLabel   (AjinParaUi.pnBase); AjinParaUi.lbGntrSubAdd   -> Parent = AjinParaUi.pnBase ; AjinParaUi.lbGntrSubAdd    -> Caption = "Gentry Sub Phy No  " ; SetAjinArrange(AjinParaUi.lbGntrSubAdd   , 0 , 3 );
    AjinParaUi.edGntrSubAdd   = new TEdit    (AjinParaUi.pnBase); AjinParaUi.edGntrSubAdd   -> Parent = AjinParaUi.pnBase ;                                                                 SetAjinArrange(AjinParaUi.edGntrSubAdd   , 1 , 3 );

    AjinParaUi.lbGntrOfs      = new TLabel   (AjinParaUi.pnBase); AjinParaUi.lbGntrOfs      -> Parent = AjinParaUi.pnBase ; AjinParaUi.lbGntrOfs       -> Caption = "Gentry Offset      " ; SetAjinArrange(AjinParaUi.lbGntrOfs      , 0 , 4 );
    AjinParaUi.edGntrOfs      = new TEdit    (AjinParaUi.pnBase); AjinParaUi.edGntrOfs      -> Parent = AjinParaUi.pnBase ;                                                                 SetAjinArrange(AjinParaUi.edGntrOfs      , 1 , 4 );

    AjinParaUi.lbGntrOfsRange = new TLabel   (AjinParaUi.pnBase); AjinParaUi.lbGntrOfsRange -> Parent = AjinParaUi.pnBase ; AjinParaUi.lbGntrOfsRange  -> Caption = "Gentry Offset Range" ; SetAjinArrange(AjinParaUi.lbGntrOfsRange , 0 , 5 );
    AjinParaUi.edGntrOfsRange = new TEdit    (AjinParaUi.pnBase); AjinParaUi.edGntrOfsRange -> Parent = AjinParaUi.pnBase ;                                                                 SetAjinArrange(AjinParaUi.edGntrOfsRange , 1 , 5 );

    AjinParaUi.cbEncInput     = new TComboBox(AjinParaUi.pnBase); AjinParaUi.cbEncInput     -> Parent = AjinParaUi.pnBase ;                                                                 SetAjinArrange(AjinParaUi.cbEncInput     , 1 , 6 );
    AjinParaUi.lbEncInput     = new TLabel   (AjinParaUi.pnBase); AjinParaUi.lbEncInput     -> Parent = AjinParaUi.pnBase ; AjinParaUi.lbEncInput      -> Caption = "Enc Input          " ; SetAjinArrange(AjinParaUi.lbEncInput     , 0 , 6 );

    AjinParaUi.cbPulseOutput  = new TComboBox(AjinParaUi.pnBase); AjinParaUi.cbPulseOutput  -> Parent = AjinParaUi.pnBase ;                                                                 SetAjinArrange(AjinParaUi.cbPulseOutput  , 1 , 7 );
    AjinParaUi.lbPulseOutput  = new TLabel   (AjinParaUi.pnBase); AjinParaUi.lbPulseOutput  -> Parent = AjinParaUi.pnBase ; AjinParaUi.lbPulseOutput   -> Caption = "Pulse Output Method" ; SetAjinArrange(AjinParaUi.lbPulseOutput  , 0 , 7 );

    AjinParaUi.edSCurveAcPer  = new TEdit    (AjinParaUi.pnBase); AjinParaUi.edSCurveAcPer  -> Parent = AjinParaUi.pnBase ;                                                                 SetAjinArrange(AjinParaUi.edSCurveAcPer  , 1 , 8 );
    AjinParaUi.lbSCurveAcPer  = new TLabel   (AjinParaUi.pnBase); AjinParaUi.lbSCurveAcPer  -> Parent = AjinParaUi.pnBase ; AjinParaUi.lbSCurveAcPer   -> Caption = "S Curve Acc %      " ; SetAjinArrange(AjinParaUi.lbSCurveAcPer  , 0 , 8 );

    AjinParaUi.edSCurveDcPer  = new TEdit    (AjinParaUi.pnBase); AjinParaUi.edSCurveDcPer  -> Parent = AjinParaUi.pnBase ;                                                                 SetAjinArrange(AjinParaUi.edSCurveDcPer  , 1 , 9 );
    AjinParaUi.lbSCurveDcPer  = new TLabel   (AjinParaUi.pnBase); AjinParaUi.lbSCurveDcPer  -> Parent = AjinParaUi.pnBase ; AjinParaUi.lbSCurveDcPer   -> Caption = "S Curve Dcc %      " ; SetAjinArrange(AjinParaUi.lbSCurveDcPer  , 0 , 9 );

    AjinParaUi.cbHomeActLevel = new TCheckBox(AjinParaUi.pnBase); AjinParaUi.cbHomeActLevel -> Parent = AjinParaUi.pnBase ; AjinParaUi.cbHomeActLevel  -> Caption = "Home Active High   " ; SetAjinArrange(AjinParaUi.cbHomeActLevel , 0 , 10);

    AjinParaUi.cbPLimActLevel = new TCheckBox(AjinParaUi.pnBase); AjinParaUi.cbPLimActLevel -> Parent = AjinParaUi.pnBase ; AjinParaUi.cbPLimActLevel  -> Caption = "+ Limit Active High" ; SetAjinArrange(AjinParaUi.cbPLimActLevel , 0 , 11);

    AjinParaUi.cbNLimActLevel = new TCheckBox(AjinParaUi.pnBase); AjinParaUi.cbNLimActLevel -> Parent = AjinParaUi.pnBase ; AjinParaUi.cbNLimActLevel  -> Caption = "- Limit Active High" ; SetAjinArrange(AjinParaUi.cbNLimActLevel , 0 , 12);

    AjinParaUi.cbAlarmActLevel= new TCheckBox(AjinParaUi.pnBase); AjinParaUi.cbAlarmActLevel-> Parent = AjinParaUi.pnBase ; AjinParaUi.cbAlarmActLevel -> Caption = "Alarm Active High  " ; SetAjinArrange(AjinParaUi.cbAlarmActLevel, 1 , 10);

    AjinParaUi.cbInposActLevel= new TCheckBox(AjinParaUi.pnBase); AjinParaUi.cbInposActLevel-> Parent = AjinParaUi.pnBase ; AjinParaUi.cbInposActLevel -> Caption = "Inpos Active High  " ; SetAjinArrange(AjinParaUi.cbInposActLevel, 1 , 11);

    AjinParaUi.cbZphaActLevel = new TCheckBox(AjinParaUi.pnBase); AjinParaUi.cbZphaActLevel -> Parent = AjinParaUi.pnBase ; AjinParaUi.cbZphaActLevel  -> Caption = "Z Active High  "     ; SetAjinArrange(AjinParaUi.cbZphaActLevel , 1 , 12);
}

void SetAjinUIParent (TPanel * _pnBase)
{

    AjinParaUi.pnBase -> Parent = _pnBase  ;


    AjinParaUi.pnBase -> Align  = alClient ;
#ifdef AJIN_AXT
    AjinParaUi.cbEncInput    -> AddItem("Up/Down"         ,NULL);
    AjinParaUi.cbEncInput    -> AddItem("1 multiplication",NULL);
    AjinParaUi.cbEncInput    -> AddItem("2 multiplication",NULL);
    AjinParaUi.cbEncInput    -> AddItem("4 multiplication",NULL); //보통 4체배.
    AjinParaUi.cbUse360Count -> Visible = false ;
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AjinParaUi.cbEncInput    -> AddItem("+ Up/Down"         ,NULL);
    AjinParaUi.cbEncInput    -> AddItem("+ 1 multiplication",NULL);
    AjinParaUi.cbEncInput    -> AddItem("+ 2 multiplication",NULL);
    AjinParaUi.cbEncInput    -> AddItem("+ 4 multiplication",NULL); // 보통 4체배.
    AjinParaUi.cbEncInput    -> AddItem("- Up/Down"         ,NULL); // 보통 4체배.
    AjinParaUi.cbEncInput    -> AddItem("- 1 multiplication",NULL); // 보통 4체배.
    AjinParaUi.cbEncInput    -> AddItem("- 2 multiplication",NULL); // 보통 4체배.
    AjinParaUi.cbEncInput    -> AddItem("- 4 multiplication",NULL); // 보통 4체배.
    AjinParaUi.cbEncReverse -> Visible = false ;
#endif //AJIN_AXL


    AjinParaUi.cbPulseOutput -> AddItem("OneHighLowHigh"  ,NULL); // 0x0, 1펄스 방식, PULSE(Active High), 정방향(DIR=Low)  / 역방향(DIR=High)
    AjinParaUi.cbPulseOutput -> AddItem("OneHighHighLow"  ,NULL); // 0x1, 1펄스 방식, PULSE(Active High), 정방향(DIR=High) / 역방향(DIR=Low)
    AjinParaUi.cbPulseOutput -> AddItem("OneLowLowHigh "  ,NULL); // 0x2, 1펄스 방식, PULSE(Active Low),  정방향(DIR=Low)  / 역방향(DIR=High)
    AjinParaUi.cbPulseOutput -> AddItem("OneLowHighLow "  ,NULL); // 0x3, 1펄스 방식, PULSE(Active Low),  정방향(DIR=High) / 역방향(DIR=Low)
    AjinParaUi.cbPulseOutput -> AddItem("TwoCcwCwHigh  "  ,NULL); // 0x4, 2펄스 방식, PULSE(CCW:역방향),  DIR(CW:정방향),  Active High
    AjinParaUi.cbPulseOutput -> AddItem("TwoCcwCwLow   "  ,NULL); // 0x5, 2펄스 방식, PULSE(CCW:역방향),  DIR(CW:정방향),  Active Low
    AjinParaUi.cbPulseOutput -> AddItem("TwoCwCcwHigh  "  ,NULL); // 0x6, 2펄스 방식, PULSE(CW:정방향),   DIR(CCW:역방향), Active High
    AjinParaUi.cbPulseOutput -> AddItem("TwoCwCcwLow   "  ,NULL); // 0x7, 2펄스 방식, PULSE(CW:정방향),   DIR(CCW:역방향), Active Low

    AjinParaUi.cbGntrMathod  -> AddItem("NotUse"          ,NULL);
    AjinParaUi.cbGntrMathod  -> AddItem("BothUse"         ,NULL);
    AjinParaUi.cbGntrMathod  -> AddItem("MasureOffset"    ,NULL);


}

const int iControlWidth   = 165 ;
const int iControlHeight  = 25  ;
const int iStartRowOffset = 15  ;
const int iStartColOffset = 20  ;

void SetAjinArrange (TControl * _tcControl , int _iCol , int _iRow)
{
    _tcControl -> Left = _iCol * iControlWidth  * 1.1 + _iCol + iStartColOffset ; _tcControl -> Width = iControlWidth  ;
    _tcControl -> Top  = _iRow * iControlHeight * 1.1 + _iRow + iStartRowOffset ; _tcControl -> Height= iControlHeight ;
}

void DeleteAjinUI()
{
    if(AjinParaUi.pnBase) {
        delete AjinParaUi.pnBase ; //컴포넌트는 소유주나 Parent삭제시에 모두 제거 됨.
        AjinParaUi.pnBase = NULL ;
    }
}


int GetAjinAxisCnt()
{
#ifdef AJIN_AXT
    return CFS20get_total_numof_axis();
#endif //AJIN_AXT
#ifdef AJIN_AXL
    long lRet ;
    AxmInfoGetAxisCount(&lRet);
    return lRet ;
#endif //AJIN_AXL

}

bool InitAjinBoard()
{
    bInitOk = true ;

#ifdef AJIN_AXT
    if(!AxtIsInitialized()){				// 통합 라이브러리가 사용 가능하지 (초기화가 되었는지) 확인
        if(!AxtInitialize(NULL, NULL)){		        // 통합 라이브러리 초기화
            ShowMessageT("AJIN AXT Lib Loading Error");
            bInitOk = false ;
            return false;
        }
    }

    if(!AxtIsInitializedBus(BUSTYPE_PCI)){		// PCI BUS가 초기화 되었는지 확인
        if(!AxtOpenDeviceAuto(BUSTYPE_PCI)){		// 새로운 베이스보드를 자동으로 통합라이브러리에 추가
            ShowMessageT("AJIN Axt Baseboard Loading Error");
            bInitOk = false ;
            return false;
        }
    }

    if(!CFS20IsInitialized()){				// CAMC-FS모듈을 사용가능 한지 확인.
        if(!InitializeCAMCFS20(TRUE)){			// CAMC-FS모듈을 초기화(열려있는 모든베이스보드에서 FS모듈을 검색하여 초기화)
            ShowMessageT("AJIN Axt CAMC-FS Module Loading Error");
            bInitOk = false ;
            return false;
        }
    }
#endif //AJIN_AXT
#ifdef AJIN_AXL
    if(!AxlIsOpened()){				// 통합 라이브러리가 사용 가능하지 (초기화가 되었는지) 확인
        if(AxlOpenNoReset(7) != AXT_RT_SUCCESS){			// 통합 라이브러리 초기화
            ShowMessageT("AJIN AXL Lib Loading Error");
            return false ;
        }

        DWORD dwStatus;
        DWORD dwRet = AxmInfoIsMotionModule(&dwStatus);
        if(dwRet != AXT_RT_SUCCESS){
            ShowMessageT("AJIN AXL IO Module Loading Error");
            return false ;
        }
    }
#endif //AJIN_AXL

    return true ;
}

void CloseAjinBoard()
{
#ifdef AJIN_AXT
    AxtCloseDeviceAll();
    AxtClose();
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxlClose();
#endif //AJIN_AXL
}

//---CAjinMotion Unit-------------------------------------------------------------
CAjinMotion::CAjinMotion(int _iAxisNo)
{
    InitLib();
    MakeAjinUI();

    memset(&AjinPara , 0 , sizeof(SAjinPara));

    m_iAxisNo = _iAxisNo ;


    Load();
}

CAjinMotion::~CAjinMotion()
{
//    DeleteAjinUI();
    CloseAjinBoard();
}


// 통합 라이브러리 및 모듈 초기화
bool CAjinMotion::InitLib()
{
    static bool bFirst = true ;
    if(!bFirst) return true ;
    bFirst = false ;

    InitAjinBoard();
    return true ;
}

// 모터 센서 구성은 여러가지 경우가 있으나 작업효율상 일반적인 경우 몇개만 추려 구현 하겠다.
// 1. - h + : 가장 이상적인 형태 home센서로 홈잡는다. - h 센서 사이에 도그가 들어와서 - h 사이에 있지만 둘다 들어 오지 않는 경우가 잇을수 있다.
//                                                    모터가 기구적으로 항상 - + 사이에 있도록 설계해야함.
//
// 2. -   + :                    -   센서로 홈잡는다.  모터가 기구적으로 항상 - + 사이에 있도록 설계해야함.
// 3. h   + :                    home센서로 홈잡는다.  모터가 기구적으로 항상 h + 사이에 있도록 설계해야함.
// 4. h     :                    home센서로 홈잡는다.  않좋은 상태 예외처리 불가능.
/*
CFS20start_signal_search1(INT16 axis, double velocity, double acceleration, UINT8 detect_signal);
UINT8 detect_signal 인자 설명.
// detect_signal : 검색 신호 설정(typedef : DETECT_DESTINATION_SIGNAL)
// PElmNegativeEdge    = 0x0,        // +Elm(End limit) 하강 edge
// NElmNegativeEdge    = 0x1,        // -Elm(End limit) 하강 edge
// PSlmNegativeEdge    = 0x2,        // +Slm(Slowdown limit) 하강 edge
// NSlmNegativeEdge    = 0x3,        // -Slm(Slowdown limit) 하강 edge
// In0DownEdge         = 0x4,        // IN0(ORG) 하강 edge
// In1DownEdge         = 0x5,        // IN1(Z상) 하강 edge
// In2DownEdge         = 0x6,        // IN2(범용) 하강 edge
// In3DownEdge         = 0x7,        // IN3(범용) 하강 edge
// PElmPositiveEdge    = 0x8,        // +Elm(End limit) 상승 edge
   NElmPositiveEdge    = 0x9,        // -Elm(End limit) 상승 edge
// PSlmPositiveEdge    = 0xa,        // +Slm(Slowdown limit) 상승 edge
// NSlmPositiveEdge    = 0xb,        // -Slm(Slowdown limit) 상승 edge
// In0UpEdge           = 0xc,        // IN0(ORG) 상승 edge
// In1UpEdge           = 0xd,        // IN1(Z상) 상승 edge
// In2UpEdge           = 0xe,        // IN2(범용) 상승 edge
// In3UpEdge           = 0xf         // IN3(범용) 상승 edge
*/
void CAjinMotion::SetUseAccRate(bool _bUseRate)
{
    m_bUseAccRate = _bUseRate ;
#ifdef AJIN_AXT
#endif //AJIN_AXT

#ifdef AJIN_AXL
    if(_bUseRate)AxmMotSetAccelUnit(AjinPara.iPhysicalNo , UNIT_SEC2);
    else         AxmMotSetAccelUnit(AjinPara.iPhysicalNo , SEC      );
#endif
}

//0 : 비접  1:에이접 3:안씀.
//서보 아닐때 인포지션 시그널은 UNUSE로 해야 라이브러리에 오류가 없음.
void CAjinMotion::SetSignalInpos(DWORD _dwSel)
{
#ifdef AJIN_AXT
//여기는 확인 안되었음.

#endif //AJIN_AXT

#ifdef AJIN_AXL
    
    AxmSignalSetInpos(AjinPara.iPhysicalNo , _dwSel);
#endif
}


bool CAjinMotion::GetStop()
{
#ifdef AJIN_AXT
    return CFS20motion_done(AjinPara.iPhysicalNo) ;
#endif

#ifdef AJIN_AXL
    DWORD	dwInMotion;
	BOOL	bRet;
    AxmStatusReadInMotion(AjinPara.iPhysicalNo, &dwInMotion );
	bRet = !dwInMotion;
	return bRet;
#endif //AJIN_AXL

}

void CAjinMotion::ApplyAjinPara ()
{
    const int iStart_Stop_speed = 1     ; //모션프로파일 생성시 처음 치고 나가는 속도. 이보다 느리면 구동이 안됨.
    int iMax_Speed_unut   = 40000 ; //초당 2000유닛.  mm유닛이면 초당 2000mm 즉 2m 가는 것이 최고 속도.
    int iMax_Speed_Pulse  = iMax_Speed_unut * 1000 ;  // 최대 구동 속도를 설정  40000mm/sec * Pulse/Unit   10000 pulse / 10mm

    //iMax_Speed_Pulse = 700000 ;
    iMax_Speed_Pulse = 6000000 ;


#ifdef AJIN_AXT
    CFS20set_startstop_speed   (AjinPara.iPhysicalNo, iStart_Stop_speed     ); // 초기속도(Start/Stop speed) 설정
    CFS20set_end_limit_enable  (AjinPara.iPhysicalNo, false                 ); // 급정지 리미트 기능 사용유무를 설정 윗단에서 해결 함.
    CFS20set_alarm_enable      (AjinPara.iPhysicalNo, false                 );

    CFS20set_enc_reverse       (AjinPara.iPhysicalNo, AjinPara.bEncReverse   ); // 엔코더 방향 전환.
    CFS20set_enc_input_method  (AjinPara.iPhysicalNo, AjinPara.iEncInput     ); // 엔코더 입력 방식 설정
    CFS20set_pulse_out_method  (AjinPara.iPhysicalNo, AjinPara.iPulseOutput  ); // 펄스 출력 방식 설정
    CFS20set_s_rate            (AjinPara.iPhysicalNo, AjinPara.dSCurveAcPer ,
                                                      AjinPara.dSCurveDcPer  ); // S Curve 세팅.

    CFS20set_pend_limit_level  (AjinPara.iPhysicalNo, AjinPara.bPLimActLevel ); // 정방향 리미트(+End limit)의 액티브레벨 설정
    CFS20set_nend_limit_level  (AjinPara.iPhysicalNo, AjinPara.bNLimActLevel ); // 역방향 리미트(-End limit)의 액티브레벨 설정
    CFS20set_alarm_level       (AjinPara.iPhysicalNo, AjinPara.bAlarmActLevel); // 알람(Alarm) 신호 액티브레벨 설정
    CFS20set_inposition_level  (AjinPara.iPhysicalNo, AjinPara.bInposActLevel); // 인포지션(Inposition) 신호 액티브레벨 설정

    CFS20set_max_speed         (AjinPara.iPhysicalNo, iMax_Speed_Pulse      );

    //AXL버전은 검증 안함.. 찾아서 넣어야 한다.
    //360도 링카운트.
    //AxmStatusSetPosType(AjinPara.iPhysicalNo,_bEnable ? 1 : 0,_iMinPosPulse,_iMaxPosPulse);


    //요놈들은 안쓰거나 윗단에서 해결 함.
//  CFS20set_moveunit_perpulse (AjinPara.iPhysicalNo, UNIT_PER_PULSE        ); // Unit당 이동단위를 설정. 이동량 및 속도에 영향을 준다 안씀.

//  CFS20set_end_limit_enable  (AjinPara.iPhysicalNo, AjinPara.bEndLimEnable ); // 급정지 리미트 기능 사용유무를 설정
//  CFS20set_alarm_enable      (AjinPara.iPhysicalNo, AjinPara.bAlarmEnable  ); // 서보 알람 입력신호 기능의 사용유무를 설정
//  CFS20set_inposition_enable (AjinPara.iPhysicalNo, AjinPara.bInposEnable  ); // 인포지션(Inposition) 신호 사용유무 설정
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmMotSetMinVel         (AjinPara.iPhysicalNo , iStart_Stop_speed             ); //모션프로파일 생성시 처음 치고 나가는 속도. 이보다 느리면 구동이 안됨.
    AxmMotSetMaxVel         (AjinPara.iPhysicalNo , iMax_Speed_Pulse              ); //최대 구동 속도를 설정

    AxmSignalSetZphaseLevel (AjinPara.iPhysicalNo , AjinPara.bZphaActLevel        ); //Z상 접점.

    AxmHomeSetSignalLevel   (AjinPara.iPhysicalNo , AjinPara.bHomeActLevel        ); //Home 접점.
    AxmSignalSetLimit       (AjinPara.iPhysicalNo , EMERGENCY_STOP, AjinPara.bPLimActLevel,
                                                      AjinPara.bNLimActLevel);

    AxmSignalSetServoAlarm  (AjinPara.iPhysicalNo , AjinPara.bAlarmActLevel       ); //알람시에 비상정지 않씀 직접 세움.

    AxmMotSetEncInputMethod (AjinPara.iPhysicalNo , AjinPara.iEncInput            ); //엔코더 입력 방식 설정 엔코더 방향 전환.
    AxmMotSetPulseOutMethod (AjinPara.iPhysicalNo , AjinPara.iPulseOutput         ); //펄스 출력 방식 설정

    //AxmMotSetAccelUnit      (AjinPara.iPhysicalNo , SEC                           );
    AxmSignalSetInpos       (AjinPara.iPhysicalNo , AjinPara.bInposActLevel       ); //인포지션(Inposition) 신호 액티브레벨 설정
    //360도 펄스 계산 하기 때문에 상위에서 호출해야한다.
    //AxmStatusSetPosType     (AjinPara.iPhysicalNo,_bEnable ? 1 : 0,_iMinPosPulse,_iMaxPosPulse);

    if(AjinPara.dSCurveAcPer == 0 && AjinPara.dSCurveDcPer == 0 ) {
        //AxmMotSetProfileMode (AjinPara.iPhysicalNo, SYM_TRAPEZOIDE_MODE );   //대칭
        AxmMotSetProfileMode (AjinPara.iPhysicalNo, ASYM_TRAPEZOIDE_MODE);     //비대칭
    }
    else if(AjinPara.dSCurveAcPer == AjinPara.dSCurveDcPer ) {
        AxmMotSetProfileMode(AjinPara.iPhysicalNo , SYM_S_CURVE_MODE    );
        AxmMotSetAccelJerk  (AjinPara.iPhysicalNo , AjinPara.dSCurveAcPer);
        AxmMotSetDecelJerk  (AjinPara.iPhysicalNo , AjinPara.dSCurveDcPer);
    }
    else {
        AxmMotSetProfileMode(AjinPara.iPhysicalNo , ASYM_S_CURVE_MODE   );
        AxmMotSetAccelJerk  (AjinPara.iPhysicalNo , AjinPara.dSCurveAcPer);
        AxmMotSetDecelJerk  (AjinPara.iPhysicalNo , AjinPara.dSCurveDcPer);
    }
#endif //AJIN_AXL
}

void CAjinMotion::SetServo(bool _bOn)
{
#ifdef AJIN_AXT
    //Stop the Motor.
    Stop();

    //Servo On/Off.
    if (_bOn) CFS20set_output_bit  (AjinPara.iPhysicalNo , 0);
    else      CFS20reset_output_bit(AjinPara.iPhysicalNo , 0);
#endif //AJIN_AXT
#ifdef AJIN_AXL
    const int iServoOnBit = 0 ;

    //Stop the Motor.
    Stop();

    //Servo On/Off.
    //이놈으로 하면 겐트리 슬레이브 같이 안켜짐.
    //AxmSignalWriteOutputBit(AjinPara.iPhysicalNo, iServoOnBit, _bOn);
    AxmSignalServoOn(AjinPara.iPhysicalNo, _bOn);
#endif //AJIN_AXL

}

bool CAjinMotion::GetServo()
{
#ifdef AJIN_AXT
    return CFS20get_servo_enable(AjinPara.iPhysicalNo) ;
#endif //AJIN_AXT
#ifdef AJIN_AXL
    const int iServoOnBit = 0 ;

    DWORD dwpServoOnLevel ;
    DWORD dwRet = AxmSignalReadOutputBit(AjinPara.iPhysicalNo,iServoOnBit,&dwpServoOnLevel);
    return dwpServoOnLevel ;
#endif //AJIN_AXL

}

void CAjinMotion::SetReset(bool _bOn)
{
#ifdef AJIN_AXT
    if (_bOn) CFS20set_output_bit  (AjinPara.iPhysicalNo , 1);
    else      CFS20reset_output_bit(AjinPara.iPhysicalNo , 1);
#endif //AJIN_AXT
#ifdef AJIN_AXL
    const int iResetBit = 1 ;
    //Servo On/Off.
    AxmSignalWriteOutputBit(AjinPara.iPhysicalNo, iResetBit, _bOn);
#endif //AJIN_AXL

}

double CAjinMotion::GetCmdPos()
{
#ifdef AJIN_AXT
    return CFS20get_command_position(AjinPara.iPhysicalNo);
#endif //AJIN_AXT
#ifdef AJIN_AXL
    double dPos;
    AxmStatusGetCmdPos(AjinPara.iPhysicalNo, &dPos);
    return dPos;
#endif //AJIN_AXL

}

double CAjinMotion::GetEncPos()
{
#ifdef AJIN_AXT
    return CFS20get_actual_position (AjinPara.iPhysicalNo); //Servo , Easy Servo - Actual Position.
#endif //AJIN_AXT
#ifdef AJIN_AXL
    double dPos;
    AxmStatusGetActPos(AjinPara.iPhysicalNo, &dPos);
    return dPos;
#endif //AJIN_AXL

}

void CAjinMotion::SetPos (double _dPos )
{
#ifdef AJIN_AXT
    CFS20set_command_position(AjinPara.iPhysicalNo , _dPos); //Commnad Position.
    CFS20set_actual_position (AjinPara.iPhysicalNo , _dPos); //Actual Position.
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmStatusSetCmdPos(AjinPara.iPhysicalNo , _dPos); //Commnad Position.
    AxmStatusSetActPos(AjinPara.iPhysicalNo , _dPos); //Actual Position.
#endif //AJIN_AXL

}

bool CAjinMotion::GetHomeSnsr()
{
#ifdef AJIN_AXT
    //Home Sensor
    bool bRet ;

    bRet = ((CFS20get_input(AjinPara.iPhysicalNo) >> 0 ) & 0x01) ;
    bRet = AjinPara.bHomeActLevel ?  bRet : !bRet ;

    return bRet ;
#endif //AJIN_AXT
#ifdef AJIN_AXL
    //Home Sensor
    bool bRet ;
    BYTE bInput ;
    unsigned long lInput ;

    AxmHomeReadSignal(AjinPara.iPhysicalNo, &lInput);
    //SignalReadInput(AjinPara.iPhysicalNo, &lInput);
    bInput = (BYTE)lInput ;

    bRet = (bInput >> 0 ) & 0x01 ;
    //bRet = AjinPara.bHomeActLevel ?  bRet : !bRet ;

    return bRet ;
#endif //AJIN_AXL

}

bool CAjinMotion::GetNLimSnsr()
{
#ifdef AJIN_AXT
    //Get Mechanical Signal.
    bool bRet ;

    bRet = (CFS20get_mechanical_signal(AjinPara.iPhysicalNo) >> 1) & 0x01;

    return bRet ;
#endif //AJIN_AXT
#ifdef AJIN_AXL
    //Get Mechanical Signal.
    bool bRet ;
    BYTE bInput ;
    unsigned long lInput ;

	AxmStatusReadMechanical(AjinPara.iPhysicalNo, &lInput);
    bInput = (BYTE)lInput ;

    bRet = (bInput >> 1) & 0x01;

    return bRet ;
#endif //AJIN_AXL

}

bool CAjinMotion::GetPLimSnsr()
{
#ifdef AJIN_AXT
    //Get Mechanical Signal.
    bool bRet ;

    bRet = (CFS20get_mechanical_signal(AjinPara.iPhysicalNo) >> 0) & 0x01;

    return bRet ;
#endif //AJIN_AXT
#ifdef AJIN_AXL
//Get Mechanical Signal.
    bool bRet ;
    BYTE bInput ;
    unsigned long lInput ;

	AxmStatusReadMechanical(AjinPara.iPhysicalNo, &lInput);
    bInput = (BYTE)lInput ;

    bRet = (bInput >> 0) & 0x01;

    return bRet ;
#endif //AJIN_AXL

}

bool CAjinMotion::GetZphaseSnsr()
{
#ifdef AJIN_AXT
    //Get ZPhase Signal.
    bool bRet ;

    bRet = ((CFS20get_input(AjinPara.iPhysicalNo) >> 1 ) & 0x01) ;

    //구형은 전환하는 함수가 없음.
    bRet = AjinPara.bZphaActLevel ?  bRet : !bRet ;

    return bRet ;
#endif //AJIN_AXT
#ifdef AJIN_AXL
    //Home Sensor
    bool bRet ;
    BYTE bInput ;
    unsigned long lInput ;

    AxmSignalReadInput(AjinPara.iPhysicalNo, &lInput);
    bInput = (BYTE)lInput ;

    bRet = (bInput >> 1 ) & 0x01 ;

    //아진세팅으로 전환.
    //bRet = AjinPara.bZphaActLevel ?  bRet : !bRet ;

    return bRet ;
#endif //AJIN_AXL

}

bool CAjinMotion::GetPackInPos()
{
#ifdef AJIN_AXT
    //Get Mechanical Signal.
    bool bRet ;

    bRet = (CFS20get_mechanical_signal(AjinPara.iPhysicalNo) >> 5) & 0x01;

    return bRet ;
#endif //AJIN_AXT
#ifdef AJIN_AXL
    //Get Mechanical Signal.
    bool bRet ;
    BYTE bInput ;
    unsigned long lInput ;

	AxmStatusReadMechanical(AjinPara.iPhysicalNo, &lInput);
    bInput = (BYTE)lInput ;

    bRet = (bInput >> 5) & 0x01;

    return bRet ;
#endif //AJIN_AXL

}

bool CAjinMotion::GetAlarm()
{
#ifdef AJIN_AXT
    bool bRet ;

    bRet = (CFS20get_mechanical_signal(AjinPara.iPhysicalNo) >> 4) & 0x01;

    return bRet ;
#endif //AJIN_AXT
#ifdef AJIN_AXL
    //Get Mechanical Signal.
    bool bRet ;
    BYTE bInput ;
    unsigned long lInput ;

    AxmStatusReadMechanical(AjinPara.iPhysicalNo, &lInput);
    bInput = (BYTE)lInput ;

    bRet = (bInput >> 4) & 0x01;

    return bRet ;
#endif //AJIN_AXL

}

bool CAjinMotion::GetHomeDone()
{
#ifdef AJIN_AXT
    bool  bRet ;

    bRet = CFS20get_home_done(AjinPara.iPhysicalNo) ;

    return bRet ;
#endif //AJIN_AXT
#ifdef AJIN_AXL
    bool  bRet ;
    unsigned long lRet ;

    AxmHomeGetResult(AjinPara.iPhysicalNo, &lRet);

    bRet = (lRet == HOME_SUCCESS) ;

    return bRet ;
#endif //AJIN_AXL

}

bool CAjinMotion::Stop()
{
#ifdef AJIN_AXT
    //Stop.
    CFS20abort_home_search(AjinPara.iPhysicalNo , 0);

    return CFS20set_stop(AjinPara.iPhysicalNo) ;
#endif //AJIN_AXT
#ifdef AJIN_AXL
    return (AXT_RT_SUCCESS ==AxmMoveSStop(AjinPara.iPhysicalNo)) ;
#endif //AJIN_AXL

}

bool CAjinMotion::EmgStop()
{
#ifdef AJIN_AXT
    //Stop Home Proc.
    CFS20abort_home_search(AjinPara.iPhysicalNo , 0);

    //Stop.
    return CFS20set_e_stop(AjinPara.iPhysicalNo) ;
#endif //AJIN_AXT
#ifdef AJIN_AXL
    return (AXT_RT_SUCCESS ==AxmMoveEStop(AjinPara.iPhysicalNo));
#endif //AJIN_AXL

}

bool CAjinMotion::JogP(double _dVel , double _dAcc , double _dDec )
{
#ifdef AJIN_AXT      //  CFSv_a_move_ex
    if(m_bUseAccRate) return CFS20v_a_move   (AjinPara.iPhysicalNo , _dVel , _dAcc , _dDec);
    else              return CFS20v_a_move_ex(AjinPara.iPhysicalNo , _dVel , _dAcc , _dDec);
#endif //AJIN_AXT
#ifdef AJIN_AXL
    bool Rslt = false;
	DWORD	dwRet;
	dwRet	= AxmMoveVel(AjinPara.iPhysicalNo,  _dVel , _dAcc, _dDec);
	Rslt	= (BOOL)dwRet == AXT_RT_SUCCESS ? TRUE : FALSE;

    return Rslt;
#endif //AJIN_AXL

}

bool CAjinMotion::JogN(double _dVel , double _dAcc , double _dDec)
{
#ifdef AJIN_AXT
    if(m_bUseAccRate) return CFS20v_a_move   (AjinPara.iPhysicalNo , -1 * _dVel , _dAcc , _dDec);
    else              return CFS20v_a_move_ex(AjinPara.iPhysicalNo , -1 * _dVel , _dAcc , _dDec);
#endif //AJIN_AXT
#ifdef AJIN_AXL
    bool Rslt = false;
	DWORD	dwRet;
	dwRet	= AxmMoveVel(AjinPara.iPhysicalNo,  -1 * _dVel , _dAcc, _dDec);
	Rslt	= (BOOL)dwRet == AXT_RT_SUCCESS ? TRUE : FALSE;

    return Rslt;
#endif //AJIN_AXL

}

bool CAjinMotion::GoAbs(double _dPos , double _dVel , double _dAcc , double _dDec)
{
#ifdef AJIN_AXT
    //Go Motor.
    if (GetStop()) {            //CFSstart_ra_move_ex
        if(m_bUseAccRate) {
            if(AjinPara.bUseSCurve) CFS20start_as_move   (AjinPara.iPhysicalNo , _dPos , _dVel , _dAcc , _dDec);
            else                    CFS20start_as_move   (AjinPara.iPhysicalNo , _dPos , _dVel , _dAcc , _dDec);
        }
        else {
            if(AjinPara.bUseSCurve) CFS20start_as_move_ex(AjinPara.iPhysicalNo , _dPos , _dVel , _dAcc , _dDec);
            else                    CFS20start_as_move_ex(AjinPara.iPhysicalNo , _dPos , _dVel , _dAcc , _dDec);
        }
    }

    //Check InPos.
    bool bRet = GetStop();
    return bRet ;
#endif //AJIN_AXT
#ifdef AJIN_AXL
    //AxmMotSetAbsRelMode 0, POS_ABS_MODE
    //Go Motor.
    if (GetStop()) {
	AxmMoveStartPos(AjinPara.iPhysicalNo, _dPos , _dVel , _dAcc , _dDec );
    }

    //Check InPos.
    bool bRet = GetStop();
    return bRet ;
#endif //AJIN_AXL

}

bool CAjinMotion::GoOvr(double _dSttPos , double _dOriTrgPos , double _dNewTrgPos)
{
#ifdef AJIN_AXT
    //Go Motor.
    //아진 구형 검증 하고 쓰기.
    //CFS20position_override(AjinPara.iPhysicalNo , _dNewTrgPos ); //감속 프로파일은 전에 무브 동작것을 자동적으로 따라감.

    //Check InPos.
    bool bRet = GetStop();
    return bRet ;
#endif //AJIN_AXT
#ifdef AJIN_AXL
    DWORD dwRet ;
    //Go Motor.           //상대값 인지 ... 뭔지 오동작 작열.
    dwRet = AxmOverridePos(AjinPara.iPhysicalNo , _dNewTrgPos - _dSttPos ); //감속 프로파일은 전에 무브 동작것을 자동적으로 따라감.

    //Check InPos.
    bool bRet = dwRet != AXT_RT_SUCCESS;
    //AXT_RT_SUCCESS = 0 ;
    //AXT_RT_MOTION_NOT_INITIAL_AXIS_NO = 4053
    //AXT_RT_MOTION_ERROR_IN_MOTION     = 4151

    return bRet ;
#endif //AJIN_AXL

}

bool CAjinMotion::FindEdgeStop (double _dVel ,double _dAcc , EN_FINDEDGE_TYPE _ftFindEdgeType)
{
    EN_FINDEDGE_TYPE ftFindEdgeType ;


#ifdef AJIN_AXT
    //Ori
    if(_ftFindEdgeType == ftOrgUpEdge || _ftFindEdgeType == ftOrgDnEdge) {
        if(AjinPara.bHomeActLevel) {
            ftFindEdgeType = _ftFindEdgeType ;
        }
        else {
            ftFindEdgeType = (_ftFindEdgeType == ftOrgUpEdge) ? ftOrgDnEdge : ftOrgUpEdge ;
        }
    }
    //- lim 이것은 아진꺼로 사용.
    //else if(_ftFindEdgeType == ftNELUpEdge || _ftFindEdgeType == ftNELDnEdge) {
    //    if(AjinPara.bNLimActLevel) {
    //        ftFindEdgeType = _ftFindEdgeType ;
    //    }
    //    else {
    //        ftFindEdgeType = (_ftFindEdgeType == ftNELUpEdge) ? ftNELDnEdge : ftNELUpEdge ;
    //    }
    //}

    //Z
    else if(_ftFindEdgeType == ftZphUpEdge || _ftFindEdgeType == ftZphDnEdge) {
        if(AjinPara.bZphaActLevel) {
            ftFindEdgeType = _ftFindEdgeType ;
        }
        else {
            ftFindEdgeType = (_ftFindEdgeType == ftZphUpEdge) ? ftZphDnEdge : ftZphUpEdge ;
        }
    }
    else {
        ftFindEdgeType = _ftFindEdgeType ;
    }

    if(m_bUseAccRate)return CFS20start_signal_search1   (AjinPara.iPhysicalNo, _dVel , _dAcc , ftFindEdgeType);
    else             return CFS20start_signal_search1_ex(AjinPara.iPhysicalNo, _dVel , _dAcc , ftFindEdgeType);
#endif //AJIN_AXT
#ifdef AJIN_AXL
    //Ori
    if(_ftFindEdgeType == ftOrgUpEdge || _ftFindEdgeType == ftOrgDnEdge) {
        if(AjinPara.bHomeActLevel) {
            ftFindEdgeType = _ftFindEdgeType ;
        }
        else {
            ftFindEdgeType = (_ftFindEdgeType == ftOrgUpEdge) ? ftOrgDnEdge : ftOrgUpEdge ;
        }
    }
    //- lim 이것은 아진꺼로 사용.
    //else if(_ftFindEdgeType == ftNELUpEdge || _ftFindEdgeType == ftNELDnEdge) {
    //    if(AjinPara.bNLimActLevel) {
    //        ftFindEdgeType = _ftFindEdgeType ;
    //    }
    //    else {
    //        ftFindEdgeType = (_ftFindEdgeType == ftNELUpEdge) ? ftNELDnEdge : ftNELUpEdge ;
    //    }
    //}

    //Z 신형은 아진세팅 함수로 반전 해서 자동 적용. 인줄 알았는데 모르겠음....
    else if(_ftFindEdgeType == ftZphUpEdge || _ftFindEdgeType == ftZphDnEdge) {
        if(AjinPara.bZphaActLevel) {
            ftFindEdgeType = _ftFindEdgeType ;
        }
        else {
            ftFindEdgeType = (_ftFindEdgeType == ftZphUpEdge) ? ftZphDnEdge : ftZphUpEdge ;
        }
    }
    else {
        ftFindEdgeType = _ftFindEdgeType ;
    }

    const int iSensorKind = 8 ;

    AXT_MOTION_HOME_DETECT_SIGNAL iSignal ;
    AXT_MOTION_EDGE               iEdge   ;

    iSignal = (AXT_MOTION_HOME_DETECT_SIGNAL) (ftFindEdgeType % iSensorKind) ;
    iEdge   = (ftFindEdgeType / iSensorKind) ? SIGNAL_UP_EDGE  : SIGNAL_DOWN_EDGE ;

    /*
     PosEndLimit                            = 0x0,            // +Elm(End limit) +방향 리미트 센서 신호
     NegEndLimit                            = 0x1,            // -Elm(End limit) -방향 리미트 센서 신호
     PosSloLimit                            = 0x2,            // +Slm(Slow Down limit) 신호 - 사용하지 않음
     NegSloLimit                            = 0x3,            // -Slm(Slow Down limit) 신호 - 사용하지 않음
     HomeSensor                             = 0x4,            // IN0(ORG)  원점 센서 신호
     EncodZPhase                            = 0x5,            // IN1(Z상)  Encoder Z상 신호
     UniInput02                             = 0x6,            // IN2(범용) 범용 입력 2번 신호
     UniInput03                             = 0x7,            // IN3(범용) 범용 입력 3번 신호

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
    */

    // 원점 센서 검출 시작 : 사다리꼴 구동, 가속 시간 모드 사용, 감속 정지 방식

    DWORD iRet = AxmMoveSignalSearch(AjinPara.iPhysicalNo, _dVel, _dAcc, iSignal, iEdge , SLOWDOWN_STOP) ;

    if(AjinPara.iPhysicalNo == 18 && iRet != AXT_RT_SUCCESS) ShowMessage("FindEdgeStop" + iRet);

    return iRet ;
#endif //AJIN_AXL    AxmMoveSignalSearch(Para.iModulPos, -1 * AprSpeed, Accel, lSignalQi, lRisingEdgeQi, SLOWDOWN_STOP);

}

bool CAjinMotion::FindEdgeEmgStop(double _dVel ,double _dAcc ,EN_FINDEDGE_TYPE _ftFindEdgeType)
{
    EN_FINDEDGE_TYPE ftFindEdgeType ;


#ifdef AJIN_AXT
    //Ori
    if(_ftFindEdgeType == ftOrgUpEdge || _ftFindEdgeType == ftOrgDnEdge) {
        if(AjinPara.bHomeActLevel) {
            ftFindEdgeType = _ftFindEdgeType ;
        }
        else {
            ftFindEdgeType = (_ftFindEdgeType == ftOrgUpEdge) ? ftOrgDnEdge : ftOrgUpEdge ;
        }
    }
    //- lim 이것은 아진꺼로 사용.
    //else if(_ftFindEdgeType == ftNELUpEdge || _ftFindEdgeType == ftNELDnEdge) {
    //    if(AjinPara.bNLimActLevel) {
    //        ftFindEdgeType = _ftFindEdgeType ;
    //    }
    //    else {
    //        ftFindEdgeType = (_ftFindEdgeType == ftNELUpEdge) ? ftNELDnEdge : ftNELUpEdge ;
    //    }
    //}

    //Z
    else if(_ftFindEdgeType == ftZphUpEdge || _ftFindEdgeType == ftZphDnEdge) {
        if(AjinPara.bZphaActLevel) {
            ftFindEdgeType = _ftFindEdgeType ;
        }
        else {
            ftFindEdgeType = (_ftFindEdgeType == ftZphUpEdge) ? ftZphDnEdge : ftZphUpEdge ;
        }
    }
    else {
        ftFindEdgeType = _ftFindEdgeType ;
    }
    return CFS20start_signal_search2(AjinPara.iPhysicalNo, _dVel , ftFindEdgeType);
#endif //AJIN_AXT
#ifdef AJIN_AXL
    //Ori
    if(_ftFindEdgeType == ftOrgUpEdge || _ftFindEdgeType == ftOrgDnEdge) {
        if(AjinPara.bHomeActLevel) {
            ftFindEdgeType = _ftFindEdgeType ;
        }
        else {
            ftFindEdgeType = (_ftFindEdgeType == ftOrgUpEdge) ? ftOrgDnEdge : ftOrgUpEdge ;
        }
    }
    //- lim 이것은 아진꺼로 사용.
    //else if(_ftFindEdgeType == ftNELUpEdge || _ftFindEdgeType == ftNELDnEdge) {
    //    if(AjinPara.bNLimActLevel) {
    //        ftFindEdgeType = _ftFindEdgeType ;
    //    }
    //    else {
    //        ftFindEdgeType = (_ftFindEdgeType == ftNELUpEdge) ? ftNELDnEdge : ftNELUpEdge ;
    //    }
    //}

    //Z 신형은 함수로 세팅하여 자동 변환. 자동지원 안되는 듯 함.......
    else if(_ftFindEdgeType == ftZphUpEdge || _ftFindEdgeType == ftZphDnEdge) {
        if(AjinPara.bZphaActLevel) {
            ftFindEdgeType = _ftFindEdgeType ;
        }
        else {
            ftFindEdgeType = (_ftFindEdgeType == ftZphUpEdge) ? ftZphDnEdge : ftZphUpEdge ;
        }
    }
    else {
        ftFindEdgeType = _ftFindEdgeType ;
    }

    const int iSensorKind = 8 ;

    AXT_MOTION_HOME_DETECT_SIGNAL iSignal ;
    AXT_MOTION_EDGE               iEdge   ;

    iSignal = (AXT_MOTION_HOME_DETECT_SIGNAL) (ftFindEdgeType % iSensorKind) ;
    iEdge   = (ftFindEdgeType / iSensorKind) ? SIGNAL_UP_EDGE  : SIGNAL_DOWN_EDGE ;



    // 원점 센서 검출 시작 : 사다리꼴 구동, 가속 시간 모드 사용, 감속 정지 방식
    DWORD iRet = AxmMoveSignalSearch(AjinPara.iPhysicalNo, _dVel, _dAcc, iSignal, iEdge , EMERGENCY_STOP) ;





    if(AjinPara.iPhysicalNo == 18 && iRet != AXT_RT_SUCCESS) ShowMessage("FindEdgeStopEMG" + iRet);

    return (AXT_RT_SUCCESS ==iRet ) ;
#endif //AJIN_AXL

}

void CAjinMotion::SetAjinPara (SAjinPara &_AjinPara ){memcpy(& AjinPara , &_AjinPara , sizeof(SAjinPara ));}
void CAjinMotion::GetAjinPara (SAjinPara &_AjinPara ){memcpy(&_AjinPara , & AjinPara , sizeof(SAjinPara ));}


void CAjinMotion::Save()
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sAxisNo ;
    AnsiString sPath   ;

    //Set Dir.
    sAxisNo = (AnsiString)m_iAxisNo ;
    sPath   = EXE_FOLDER + "Util\\Motor_" + sAxisNo + "_Para.INI" ;

//    UserINI.ClearFile(sPath) ;

    //Save Device.
    UserINI.Save(sPath, "AjinPara"  , "iPhysicalNo   " , AjinPara.iPhysicalNo   );
    UserINI.Save(sPath, "AjinPara"  , "bUseSCurve    " , AjinPara.bUseSCurve    );
    UserINI.Save(sPath, "AjinPara"  , "bEncReverse   " , AjinPara.bEncReverse   );
    UserINI.Save(sPath, "AjinPara"  , "iEncInput     " , AjinPara.iEncInput     );

    UserINI.Save(sPath, "AjinPara"  , "bUseGntr      " , AjinPara.bUseGntr      );
    UserINI.Save(sPath, "AjinPara"  , "iGntrMathod   " , AjinPara.iGntrMathod   );
    UserINI.Save(sPath, "AjinPara"  , "iGntrSubAdd   " , AjinPara.iGntrSubAdd   );
    UserINI.Save(sPath, "AjinPara"  , "dGntrOfs      " , AjinPara.dGntrOfs      );
    UserINI.Save(sPath, "AjinPara"  , "dGntrOfsRange " , AjinPara.dGntrOfsRange );

    UserINI.Save(sPath, "AjinPara"  , "iPulseOutput  " , AjinPara.iPulseOutput  );
    UserINI.Save(sPath, "AjinPara"  , "dSCurveAcPer  " , AjinPara.dSCurveAcPer  );
    UserINI.Save(sPath, "AjinPara"  , "dSCurveDcPer  " , AjinPara.dSCurveDcPer  );
    UserINI.Save(sPath, "AjinPara"  , "bHomeActLevel " , AjinPara.bHomeActLevel );
    UserINI.Save(sPath, "AjinPara"  , "bPLimActLevel " , AjinPara.bPLimActLevel );
    UserINI.Save(sPath, "AjinPara"  , "bNLimActLevel " , AjinPara.bNLimActLevel );
    UserINI.Save(sPath, "AjinPara"  , "bAlarmActLevel" , AjinPara.bAlarmActLevel);
    UserINI.Save(sPath, "AjinPara"  , "bUse360Count  " , AjinPara.bUse360Count  );
    UserINI.Save(sPath, "AjinPara"  , "bInposActLevel" , AjinPara.bInposActLevel);
    UserINI.Save(sPath, "AjinPara"  , "bZphaActLevel " , AjinPara.bZphaActLevel );

    ApplyAjinPara ();

}
void CAjinMotion::Load()
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sAxisNo ;
    AnsiString sPath   ;

    //Set Dir.
    sAxisNo = (AnsiString)m_iAxisNo ;
    sPath   = EXE_FOLDER + "Util\\Motor_" + sAxisNo + "_Para.INI" ;

    //Save Device.
    UserINI.Load(sPath, "AjinPara"  , "iPhysicalNo   " , AjinPara.iPhysicalNo   );
    UserINI.Load(sPath, "AjinPara"  , "bUseSCurve    " , AjinPara.bUseSCurve    );
    UserINI.Load(sPath, "AjinPara"  , "bEncReverse   " , AjinPara.bEncReverse   );
    UserINI.Load(sPath, "AjinPara"  , "iEncInput     " , AjinPara.iEncInput     );

    UserINI.Load(sPath, "AjinPara"  , "bUseGntr      " , AjinPara.bUseGntr      );
    UserINI.Load(sPath, "AjinPara"  , "iGntrMathod   " , AjinPara.iGntrMathod   );
    UserINI.Load(sPath, "AjinPara"  , "iGntrSubAdd   " , AjinPara.iGntrSubAdd   );
    UserINI.Load(sPath, "AjinPara"  , "dGntrOfs      " , AjinPara.dGntrOfs      );
    UserINI.Load(sPath, "AjinPara"  , "dGntrOfsRange " , AjinPara.dGntrOfsRange );




    UserINI.Load(sPath, "AjinPara"  , "iPulseOutput  " , AjinPara.iPulseOutput  );
    UserINI.Load(sPath, "AjinPara"  , "dSCurveAcPer  " , AjinPara.dSCurveAcPer  );
    UserINI.Load(sPath, "AjinPara"  , "dSCurveDcPer  " , AjinPara.dSCurveDcPer  );
    UserINI.Load(sPath, "AjinPara"  , "bHomeActLevel " , AjinPara.bHomeActLevel );
    UserINI.Load(sPath, "AjinPara"  , "bPLimActLevel " , AjinPara.bPLimActLevel );
    UserINI.Load(sPath, "AjinPara"  , "bNLimActLevel " , AjinPara.bNLimActLevel );
    UserINI.Load(sPath, "AjinPara"  , "bAlarmActLevel" , AjinPara.bAlarmActLevel);
    UserINI.Load(sPath, "AjinPara"  , "bUse360Count  " , AjinPara.bUse360Count  );
    UserINI.Load(sPath, "AjinPara"  , "bInposActLevel" , AjinPara.bInposActLevel);
    UserINI.Load(sPath, "AjinPara"  , "bZphaActLevel " , AjinPara.bZphaActLevel );  

    ApplyAjinPara ();
}

void CAjinMotion::UpdateAjinPara(bool _bToTable)
{
     if(_bToTable) {
         if(AjinParaUi.edPhysicalNo    ) AjinParaUi.edPhysicalNo    -> Text      = AjinPara.iPhysicalNo    ;
         if(AjinParaUi.cbUseSCurve     ) AjinParaUi.cbUseSCurve     -> Checked   = AjinPara.bUseSCurve     ;
         if(AjinParaUi.cbEncReverse    ) AjinParaUi.cbEncReverse    -> Checked   = AjinPara.bEncReverse    ;
         if(AjinParaUi.cbUse360Count   ) AjinParaUi.cbUse360Count   -> Checked   = AjinPara.bUse360Count   ;

         //Gentry Add.
         if(AjinParaUi.cbUseGntr       ) AjinParaUi.cbUseGntr       -> Checked   = AjinPara.bUseGntr       ;
         if(AjinParaUi.cbGntrMathod    ) AjinParaUi.cbGntrMathod    -> ItemIndex = AjinPara.iGntrMathod    ;
         if(AjinParaUi.edGntrSubAdd    ) AjinParaUi.edGntrSubAdd    -> Text      = AjinPara.iGntrSubAdd    ;
         if(AjinParaUi.edGntrOfs       ) AjinParaUi.edGntrOfs       -> Text      = AjinPara.dGntrOfs       ;
         if(AjinParaUi.edGntrOfsRange  ) AjinParaUi.edGntrOfsRange  -> Text      = AjinPara.dGntrOfsRange  ;

         if(AjinParaUi.cbEncInput      ) AjinParaUi.cbEncInput      -> ItemIndex = AjinPara.iEncInput      ;
         if(AjinParaUi.cbPulseOutput   ) AjinParaUi.cbPulseOutput   -> ItemIndex = AjinPara.iPulseOutput   ;
         if(AjinParaUi.edSCurveAcPer   ) AjinParaUi.edSCurveAcPer   -> Text      = AjinPara.dSCurveAcPer   ;
         if(AjinParaUi.edSCurveDcPer   ) AjinParaUi.edSCurveDcPer   -> Text      = AjinPara.dSCurveDcPer   ;
         if(AjinParaUi.cbHomeActLevel  ) AjinParaUi.cbHomeActLevel  -> Checked   = AjinPara.bHomeActLevel  ;
         if(AjinParaUi.cbPLimActLevel  ) AjinParaUi.cbPLimActLevel  -> Checked   = AjinPara.bPLimActLevel  ;
         if(AjinParaUi.cbNLimActLevel  ) AjinParaUi.cbNLimActLevel  -> Checked   = AjinPara.bNLimActLevel  ;
         if(AjinParaUi.cbAlarmActLevel ) AjinParaUi.cbAlarmActLevel -> Checked   = AjinPara.bAlarmActLevel ;
         if(AjinParaUi.cbInposActLevel ) AjinParaUi.cbInposActLevel -> Checked   = AjinPara.bInposActLevel ;
         if(AjinParaUi.cbZphaActLevel  ) AjinParaUi.cbZphaActLevel  -> Checked   = AjinPara.bZphaActLevel  ;
     }

     else {
         if(AjinParaUi.edPhysicalNo    ) AjinPara.iPhysicalNo    = AjinParaUi.edPhysicalNo    -> Text.ToInt()    ;
         if(AjinParaUi.cbUseSCurve     ) AjinPara.bUseSCurve     = AjinParaUi.cbUseSCurve     -> Checked         ;
         if(AjinParaUi.cbEncReverse    ) AjinPara.bEncReverse    = AjinParaUi.cbEncReverse    -> Checked         ;
         if(AjinParaUi.cbUse360Count   ) AjinPara.bUse360Count   = AjinParaUi.cbUse360Count   -> Checked         ;

         if(AjinParaUi.cbUseGntr       ) AjinPara.bUseGntr       = AjinParaUi.cbUseGntr       -> Checked         ;
         if(AjinParaUi.cbGntrMathod    ) AjinPara.iGntrMathod    = AjinParaUi.cbGntrMathod    -> ItemIndex       ;
         if(AjinParaUi.edGntrSubAdd    ) AjinPara.iGntrSubAdd    = AjinParaUi.edGntrSubAdd    -> Text.ToInt()    ;
         if(AjinParaUi.edGntrOfs       ) AjinPara.dGntrOfs       = AjinParaUi.edGntrOfs       -> Text.ToDouble() ;
         if(AjinParaUi.edGntrOfsRange  ) AjinPara.dGntrOfsRange  = AjinParaUi.edGntrOfsRange  -> Text.ToDouble() ;

         if(AjinParaUi.cbEncInput      ) AjinPara.iEncInput      = AjinParaUi.cbEncInput      -> ItemIndex       ;
         if(AjinParaUi.cbPulseOutput   ) AjinPara.iPulseOutput   = AjinParaUi.cbPulseOutput   -> ItemIndex       ;
         if(AjinParaUi.edSCurveAcPer   ) AjinPara.dSCurveAcPer   = AjinParaUi.edSCurveAcPer   -> Text.ToDouble() ;
         if(AjinParaUi.edSCurveDcPer   ) AjinPara.dSCurveDcPer   = AjinParaUi.edSCurveDcPer   -> Text.ToDouble() ;
         if(AjinParaUi.cbHomeActLevel  ) AjinPara.bHomeActLevel  = AjinParaUi.cbHomeActLevel  -> Checked         ;
         if(AjinParaUi.cbPLimActLevel  ) AjinPara.bPLimActLevel  = AjinParaUi.cbPLimActLevel  -> Checked         ;
         if(AjinParaUi.cbNLimActLevel  ) AjinPara.bNLimActLevel  = AjinParaUi.cbNLimActLevel  -> Checked         ;
         if(AjinParaUi.cbAlarmActLevel ) AjinPara.bAlarmActLevel = AjinParaUi.cbAlarmActLevel -> Checked         ;
         if(AjinParaUi.cbInposActLevel ) AjinPara.bInposActLevel = AjinParaUi.cbInposActLevel -> Checked         ;
         if(AjinParaUi.cbZphaActLevel  ) AjinPara.bZphaActLevel  = AjinParaUi.cbZphaActLevel  -> Checked         ;
     }
}

void __stdcall TrgCallBack(INT16 nBoardNo, INT16 nModulePos, UINT8 byFlag)
{
#ifdef AJIN_AXT
    static int iInterruptCnt = 0 ;

    if(byFlag & 0x10000000) iInterruptCnt++;

    Trace("Triger" , AnsiString(iInterruptCnt).c_str());

    if(iInterruptCnt >10000) iInterruptCnt = 0 ;
#endif //AJIN_AXT
#ifdef AJIN_AXL
#endif //AJIN_AXL

}

void CAjinMotion::SetTrgPos(int _iPosCnt , double * _dPos , double _dTrgTime , bool _bActual ,bool _bOnLevel)
{
#ifdef AJIN_AXT
    if(!bInitOk) return ;

    // 인터럽트 메세지 관련 부분.----------------------------------------------------------------------------------
    // 인터럽트 설정 부분(이 예제에서는 인터럽트 발생 횟수를 카운트 하기위해 사용함, 실제 필요하지 않음.)
    static bool bFirst = true ;
    if(bFirst)CFS20SetWindowMessage(NULL, NULL , TrgCallBack);
    bFirst = false ;
    // User Selectable Interrupt Source0번에
    // Trigger 발생시에 인터럽트가 발생하도록 조건을 설정한다.
    CFS20KeSetCommandData(m_iAxisNo, FsUserInterruptSourceSelectRegWrite,(EVENT_NONE << 24)
                                                                        |(EVENT_NONE << 16)
                                                                        |(EVENT_NONE << 8)
                                                                        |(EVENT_TRIGGER_ENABLE) );

    // Interrupt Mask 레지스터에 User Selectable Interrupt Source 0번
    // 인터럽트를 사용하도록 설정한다. ( Bit28 )
    CFS20KeSetCommandData(m_iAxisNo, FsInterruptMaskWrite, 0x10000000);
    //------------------------------------------------------------------------------------------------------------

    CFS20Trig_Set_Level(m_iAxisNo, _bOnLevel?1:0); // 트리거 레벨 설정

    //    double trig_pos[] = {	20000.0, 30000.0, 40000.0, 50000.0, 70000.0, 75000.0, 80000.0, 81000.0};

    CFS20Trig_Absolute_Profile_Set(m_iAxisNo, _iPosCnt , _dPos, _dTrgTime , _bActual);
    //CFS20Trig_Absolute_Profile_Set(m_iAxisNo, 8 , trig_pos, 1000 , _bActual);
#endif //AJIN_AXT
#ifdef AJIN_AXL
    //검증해봐야함.

    // 0축에 트리거 신호의 레벨과 지속시간을 설정한다.
    double dTrigTime = _dTrgTime; //1~50000(50ms)
    WORD uTriggerLevel = _bOnLevel ? HIGH : LOW ;
    DWORD uSelect = _bActual ? 1 : 0; // Encorder or Command Position 을 기준으로 트리거 발생
    DWORD uInterrupt = DISABLE ;
    DWORD dwRet = AxmTriggerSetTimeLevel (m_iAxisNo, dTrigTime, uTriggerLevel, uSelect, uInterrupt);

    AxmTriggerOnlyAbs(m_iAxisNo,_iPosCnt, _dPos);
#endif //AJIN_AXL


}

void CAjinMotion::ResetTrgPos()
{
#ifdef AJIN_AXT

    if(!bInitOk) return ;

    CFS20Trig_Profile_Reset(m_iAxisNo);
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmTriggerSetReset(m_iAxisNo);
#endif //AJIN_AXL
}

void CAjinMotion::OneShotTrg(bool _bUpEg , int _iTime)
{
    if(!bInitOk) return ;
#ifdef AJIN_AXT
    CFS20Trig_One_Shot_Set(m_iAxisNo, _bUpEg ? TRIG_UPEG : TRIG_DNEG , _iTime);
#endif //AJIN_AXT
#ifdef AJIN_AXL
    // 0축에 트리거 신호의 레벨과 지속시간을 설정한다.
    double dTrigTime = _iTime; // 4ms D
    WORD uTriggerLevel = HIGH ; //_bOnLevel ? HIGH : LOW ;
    DWORD uSelect = 1 ;//_bActual ? 1 : 0; // Encorder or Command Position 을 기준으로 트리거 발생
    DWORD uInterrupt = DISABLE ;
    AxmTriggerSetTimeLevel (m_iAxisNo, dTrigTime, uTriggerLevel, uSelect, uInterrupt);
    AxmTriggerOneShot(m_iAxisNo);
#endif //AJIN_AXL
}



void CAjinMotion::SetIntrptPos(double _dPos , PosCallbackFunc _pCallback)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    /*
    AxmSetCommandData32Qi
    1.인터럽트가 발생할 ACT POS 값 설정
    2.QiCNTC2Write = ATC POS 비교기의 adress
    3.인자 : 해당 축,QiCNTC2Write, 인터럽트가 발생할 위치(펄스 단위로 설정 해주어야 한다.)
    */
    AxmSetCommandData32Qi(AjinPara.iPhysicalNo,QiCNTC2Write,_dPos);
    /*
    전체 인터럽트 Enable
    */
    AxlInterruptEnable();
    /*
    해당축 인터럽트 Enable
    */
    AxmInterruptSetAxisEnable(AjinPara.iPhysicalNo, ENABLE);
    /*
    AxmInterruptSetUserEnable
    1.인터럽트 가 어떤 이벤트에 발생할지 설정 한다
    2.QIINTBANK1_7 = ACT POS 값과 위에 설정 한 비교기 값이 일치하면 발생
    3.인자 : 해당축,BANK 번호 ->  QIINTBANK1_7 는 BANK1 에 포함 되므로 0 ,QIINTBANK1_7
    */
    AxmInterruptSetUserEnable(AjinPara.iPhysicalNo, 0, QIINTBANK1_7);
    /*
    인터럽트 발생시 호출될 컬백 함수 등록
    */
    AxmInterruptSetAxis(AjinPara.iPhysicalNo, NULL, NULL, _pCallback,NULL);


    /*
    인터럽트 사용이 필요하지 않을 경우에는 인터럽트를 Disable 해주시기 바랍니다.
    관련 자료는 AXM 메뉴얼 참고 바랍니다.

    위의 함수들을 사용 하기 위하여
    첨부한 AXDev.h , AXHD.h ,AXHS.h 를 include 해주시길 바랍니다.

    #include "AXDev.h"
    #include "AXHD.h"
    #include "AXHS.h"
    */

#endif //AJIN_AXL
}

void CAjinMotion::ResetIntrpt()
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    //AxlInterruptDisable();

    /*
    해당축 인터럽트 Enable
    */
    //AxmInterruptSetAxisEnable(AjinPara.iPhysicalNo, ENABLE);
    AxmInterruptSetAxisEnable(AjinPara.iPhysicalNo, DISABLE);


#endif //AJIN_AXL
}

void CAjinMotion::Set360Count(bool _bEnable , int _iMinPosPulse , int _iMaxPosPulse)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmStatusSetPosType(AjinPara.iPhysicalNo,_bEnable ? 1 : 0,_iMinPosPulse,_iMaxPosPulse);


#endif //AJIN_AXL




}

void CAjinMotion::SetOverrideMaxSpeed(double _dOverSped)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmOverrideSetMaxVel(AjinPara.iPhysicalNo, _dOverSped);

#endif //AJIN_AXL

}

void CAjinMotion::SetOverrideVel(double _dOverSped)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmOverrideVel(AjinPara.iPhysicalNo, _dOverSped);

#endif //AJIN_AXL

}

void CAjinMotion::SetOverrideVelAtPos(double _dPos, double _dVel, double _dAcc, double _dDec, double _dOverPos, double _dOverSped, bool _bCmd)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmOverrideVelAtPos(AjinPara.iPhysicalNo, _dPos, _dVel, _dAcc, _dDec, _dOverPos, _dOverSped, _bCmd);

#endif //AJIN_AXL

}


/* 보간 구동을 위해 추가 20150410 juhyeon */

// 보간 구동할 모터 축들을 좌표계로 맵핑을 설정하는 함수
// _iCoord = 좌표계 번호
// _iAxisCnt = 보간 구동에 사용할 모터 개수
// _ipAxisNo = 보간 구동에 사용할 모터 넘버
void CAjinMotion::ContiSetAxisMap (int _iCoord, int _iAxisCnt, int *_ipAxisNo)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmContiWriteClear(_iCoord);
    AxmContiSetAxisMap(_iCoord, _iAxisCnt, (long *)_ipAxisNo); // int -> long
#endif //AJIN_AXL
}

// 보간 구동에 있어서 절대 모드 = 0 & 상대 모드 = 1 설정
void CAjinMotion::ContiSetAbsRelMode (int _iCoord, DWORD _dwAbsRelMode)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmContiSetAbsRelMode(_iCoord, _dwAbsRelMode);
#endif //AJIN_AXL
}

// 보간 구동을 하기 위한 작업들의 등록을 시작하는 함수
void CAjinMotion::ContiBeginNode (int _iCoord)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmContiBeginNode(_iCoord);
#endif //AJIN_AXL
}

// 보간 구동을 하기 위한 작업들의 등록을 종료하는 함수
void CAjinMotion::ContiEndNode (int _iCoord)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmContiEndNode(_iCoord);
#endif //AJIN_AXL
}

// 큐에 등록된 작업들의 보간 구동을 시작하기 위한 함수
// _dwProfileset = 0, 일반 보간 모드
// _dwProfileset = 1, 노드 가감속 보간 모드
// _dwProfileset = 2, 자동 가감속 보간 모드
// _iAngle = 0 ~ 360도, 자동 가감속 보간 모드 사용시 Angle 값
void CAjinMotion::ContiStart (int _iCoord, DWORD _dwProfileset, int _iAngle)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmContiStart(_iCoord, _dwProfileset, _iAngle);
#endif //AJIN_AXL
}

int CAjinMotion::GetContCrntIdx(int _iCoord)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    long _lIdx = 0 ;
    AxmContiGetNodeNum(_iCoord, &_lIdx);
    return ((int)_lIdx);
#endif //AJIN_AXL

}

// 직선 보간 모드
// _dpEndPos = 직선의 종료 지점
// _dVel = 속도
// _dAcc = 가속도
// _dDec = 감속도
void CAjinMotion::LineMove(int _iCoord, double *_dpEndPos, double _dVel, double _dAcc, double _dDec)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmLineMove(_iCoord, _dpEndPos, _dVel, _dAcc, _dDec);
#endif //AJIN_AXL
}

void CAjinMotion::CircleCenterMove(int _iCoord, int *_ipAxisNo, double *_dpCenterPos, double *_dpEndPos, double _dVel, double _dAcc, double _dDec, DWORD _dwCWDir)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmCircleCenterMove(_iCoord, (long *)_ipAxisNo, _dpCenterPos, _dpEndPos, _dVel, _dAcc, _dDec, _dwCWDir);
#endif //AJIN_AXL
}

void CAjinMotion::CirclePointMove(int _iCoord, int *_ipAxisNo, double *_dpMidPos, double *_dpEndPos, double _dVel, double _dAcc, double _dDec, int _iArcCircle)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmCirclePointMove(_iCoord, (long *)_ipAxisNo, _dpMidPos, _dpEndPos, _dVel, _dAcc, _dDec, _iArcCircle);
#endif //AJIN_AXL
}

void CAjinMotion::CircleRadiusMove(int _iCoord, int *_ipAxisNo, double _dRadius, double *_dpEndPos, double _dVel, double _dAcc, double _dDec, DWORD _dwCWDir, DWORD _dwShortDistance)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmCircleRadiusMove(_iCoord, (long *)_ipAxisNo, _dRadius, _dpEndPos,_dVel, _dAcc, _dDec, _dwCWDir, _dwShortDistance);
#endif //AJIN_AXL
}

void CAjinMotion::CircleAngleMove(int _iCoord, int *_ipAxisNo, double *_dCenterPos, double _dAngle, double _dVel, double _dAcc, double _dDec, DWORD _dwCWDir)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmCircleAngleMove(_iCoord, (long *)_ipAxisNo, _dCenterPos, _dAngle, _dVel, _dAcc, _dDec, _dwCWDir);
#endif //AJIN_AXL
}


void CAjinMotion::SetGantryEnable(int _iSlaveAxis , int _iHomeUse , double _dSlaveOffset , double _dSlOffsetRange)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    SetGantryDisable(_iSlaveAxis) ;
    int iRet = AxmGantrySetEnable(AjinPara.iPhysicalNo , _iSlaveAxis , _iHomeUse , _dSlaveOffset , _dSlOffsetRange);
    //프로그램 켜고 가끔 개별 홈 잡으면 서브축이 서보온이 안된상태일 경우가 있어서 야매로 이렇게 한다.
    //const int iServoOnBit = 0 ;
    //Servo On/Off.
    //iRet = AxmSignalWriteOutputBit(_iSlaveAxis, iServoOnBit, true);
#endif //AJIN_AXL
}

void CAjinMotion::SetGantryDisable(int _iSlaveAxis)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    int iRet = AxmGantrySetDisable(AjinPara.iPhysicalNo , _iSlaveAxis);
#endif //AJIN_AXL
}

void CAjinMotion::SetHomeMethod(int _iHomeDir , DWORD _dwHomeSignal , DWORD _dwUseZ , double _dHomeClrTime , double _dHomeOffset)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    DWORD dwRet = AxmHomeSetMethod(AjinPara.iPhysicalNo , _iHomeDir , _dwHomeSignal , _dwUseZ , _dHomeClrTime , _dHomeOffset);
#endif //AJIN_AXL
}

void CAjinMotion::SetHomeVel(double _dVel1 , double _dVel2 , double _dVel3 , double _dLast ,double _dAcc1 , double _dAcc2)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmHomeSetVel(AjinPara.iPhysicalNo , _dVel1 , _dVel2 , _dVel3 , _dLast , _dAcc1 ,  _dAcc2);
#endif //AJIN_AXL
}

void CAjinMotion::SetHomeStart()
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmHomeSetStart(AjinPara.iPhysicalNo );
#endif //AJIN_AXL
}

void CAjinMotion::GetHomeResult(DWORD *_dwResult)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL

    //[0000] AXT_RT_SUCCESS : AXL 라이브러리초기화성공
    //[4053] AXT_RT_MOTION_NOT_INITIAL_AXIS_NO : 해당축모션초기화실패
    //[4101] AXT_RT_MOTION_INVALID_AXIS_NO : 해당축이존재하지않음
    unsigned long ulRet ;
    ulRet = AxmHomeGetResult(AjinPara.iPhysicalNo , _dwResult);

    AXT_MOTION_HOME_RESULT_DEF
#endif //AJIN_AXL

}
