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
    AjinParaUi.cbEncInput    -> AddItem("4 multiplication",NULL); //���� 4ü��.
    AjinParaUi.cbUse360Count -> Visible = false ;
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AjinParaUi.cbEncInput    -> AddItem("+ Up/Down"         ,NULL);
    AjinParaUi.cbEncInput    -> AddItem("+ 1 multiplication",NULL);
    AjinParaUi.cbEncInput    -> AddItem("+ 2 multiplication",NULL);
    AjinParaUi.cbEncInput    -> AddItem("+ 4 multiplication",NULL); // ���� 4ü��.
    AjinParaUi.cbEncInput    -> AddItem("- Up/Down"         ,NULL); // ���� 4ü��.
    AjinParaUi.cbEncInput    -> AddItem("- 1 multiplication",NULL); // ���� 4ü��.
    AjinParaUi.cbEncInput    -> AddItem("- 2 multiplication",NULL); // ���� 4ü��.
    AjinParaUi.cbEncInput    -> AddItem("- 4 multiplication",NULL); // ���� 4ü��.
    AjinParaUi.cbEncReverse -> Visible = false ;
#endif //AJIN_AXL


    AjinParaUi.cbPulseOutput -> AddItem("OneHighLowHigh"  ,NULL); // 0x0, 1�޽� ���, PULSE(Active High), ������(DIR=Low)  / ������(DIR=High)
    AjinParaUi.cbPulseOutput -> AddItem("OneHighHighLow"  ,NULL); // 0x1, 1�޽� ���, PULSE(Active High), ������(DIR=High) / ������(DIR=Low)
    AjinParaUi.cbPulseOutput -> AddItem("OneLowLowHigh "  ,NULL); // 0x2, 1�޽� ���, PULSE(Active Low),  ������(DIR=Low)  / ������(DIR=High)
    AjinParaUi.cbPulseOutput -> AddItem("OneLowHighLow "  ,NULL); // 0x3, 1�޽� ���, PULSE(Active Low),  ������(DIR=High) / ������(DIR=Low)
    AjinParaUi.cbPulseOutput -> AddItem("TwoCcwCwHigh  "  ,NULL); // 0x4, 2�޽� ���, PULSE(CCW:������),  DIR(CW:������),  Active High
    AjinParaUi.cbPulseOutput -> AddItem("TwoCcwCwLow   "  ,NULL); // 0x5, 2�޽� ���, PULSE(CCW:������),  DIR(CW:������),  Active Low
    AjinParaUi.cbPulseOutput -> AddItem("TwoCwCcwHigh  "  ,NULL); // 0x6, 2�޽� ���, PULSE(CW:������),   DIR(CCW:������), Active High
    AjinParaUi.cbPulseOutput -> AddItem("TwoCwCcwLow   "  ,NULL); // 0x7, 2�޽� ���, PULSE(CW:������),   DIR(CCW:������), Active Low

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
        delete AjinParaUi.pnBase ; //������Ʈ�� �����ֳ� Parent�����ÿ� ��� ���� ��.
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
    if(!AxtIsInitialized()){				// ���� ���̺귯���� ��� �������� (�ʱ�ȭ�� �Ǿ�����) Ȯ��
        if(!AxtInitialize(NULL, NULL)){		        // ���� ���̺귯�� �ʱ�ȭ
            ShowMessageT("AJIN AXT Lib Loading Error");
            bInitOk = false ;
            return false;
        }
    }

    if(!AxtIsInitializedBus(BUSTYPE_PCI)){		// PCI BUS�� �ʱ�ȭ �Ǿ����� Ȯ��
        if(!AxtOpenDeviceAuto(BUSTYPE_PCI)){		// ���ο� ���̽����带 �ڵ����� ���ն��̺귯���� �߰�
            ShowMessageT("AJIN Axt Baseboard Loading Error");
            bInitOk = false ;
            return false;
        }
    }

    if(!CFS20IsInitialized()){				// CAMC-FS����� ��밡�� ���� Ȯ��.
        if(!InitializeCAMCFS20(TRUE)){			// CAMC-FS����� �ʱ�ȭ(�����ִ� ��纣�̽����忡�� FS����� �˻��Ͽ� �ʱ�ȭ)
            ShowMessageT("AJIN Axt CAMC-FS Module Loading Error");
            bInitOk = false ;
            return false;
        }
    }
#endif //AJIN_AXT
#ifdef AJIN_AXL
    if(!AxlIsOpened()){				// ���� ���̺귯���� ��� �������� (�ʱ�ȭ�� �Ǿ�����) Ȯ��
        if(AxlOpenNoReset(7) != AXT_RT_SUCCESS){			// ���� ���̺귯�� �ʱ�ȭ
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


// ���� ���̺귯�� �� ��� �ʱ�ȭ
bool CAjinMotion::InitLib()
{
    static bool bFirst = true ;
    if(!bFirst) return true ;
    bFirst = false ;

    InitAjinBoard();
    return true ;
}

// ���� ���� ������ �������� ��찡 ������ �۾�ȿ���� �Ϲ����� ��� ��� �߷� ���� �ϰڴ�.
// 1. - h + : ���� �̻����� ���� home������ Ȩ��´�. - h ���� ���̿� ���װ� ���ͼ� - h ���̿� ������ �Ѵ� ��� ���� �ʴ� ��찡 ������ �ִ�.
//                                                    ���Ͱ� �ⱸ������ �׻� - + ���̿� �ֵ��� �����ؾ���.
//
// 2. -   + :                    -   ������ Ȩ��´�.  ���Ͱ� �ⱸ������ �׻� - + ���̿� �ֵ��� �����ؾ���.
// 3. h   + :                    home������ Ȩ��´�.  ���Ͱ� �ⱸ������ �׻� h + ���̿� �ֵ��� �����ؾ���.
// 4. h     :                    home������ Ȩ��´�.  ������ ���� ����ó�� �Ұ���.
/*
CFS20start_signal_search1(INT16 axis, double velocity, double acceleration, UINT8 detect_signal);
UINT8 detect_signal ���� ����.
// detect_signal : �˻� ��ȣ ����(typedef : DETECT_DESTINATION_SIGNAL)
// PElmNegativeEdge    = 0x0,        // +Elm(End limit) �ϰ� edge
// NElmNegativeEdge    = 0x1,        // -Elm(End limit) �ϰ� edge
// PSlmNegativeEdge    = 0x2,        // +Slm(Slowdown limit) �ϰ� edge
// NSlmNegativeEdge    = 0x3,        // -Slm(Slowdown limit) �ϰ� edge
// In0DownEdge         = 0x4,        // IN0(ORG) �ϰ� edge
// In1DownEdge         = 0x5,        // IN1(Z��) �ϰ� edge
// In2DownEdge         = 0x6,        // IN2(����) �ϰ� edge
// In3DownEdge         = 0x7,        // IN3(����) �ϰ� edge
// PElmPositiveEdge    = 0x8,        // +Elm(End limit) ��� edge
   NElmPositiveEdge    = 0x9,        // -Elm(End limit) ��� edge
// PSlmPositiveEdge    = 0xa,        // +Slm(Slowdown limit) ��� edge
// NSlmPositiveEdge    = 0xb,        // -Slm(Slowdown limit) ��� edge
// In0UpEdge           = 0xc,        // IN0(ORG) ��� edge
// In1UpEdge           = 0xd,        // IN1(Z��) ��� edge
// In2UpEdge           = 0xe,        // IN2(����) ��� edge
// In3UpEdge           = 0xf         // IN3(����) ��� edge
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

//0 : ����  1:������ 3:�Ⱦ�.
//���� �ƴҶ� �������� �ñ׳��� UNUSE�� �ؾ� ���̺귯���� ������ ����.
void CAjinMotion::SetSignalInpos(DWORD _dwSel)
{
#ifdef AJIN_AXT
//����� Ȯ�� �ȵǾ���.

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
    const int iStart_Stop_speed = 1     ; //����������� ������ ó�� ġ�� ������ �ӵ�. �̺��� ������ ������ �ȵ�.
    int iMax_Speed_unut   = 40000 ; //�ʴ� 2000����.  mm�����̸� �ʴ� 2000mm �� 2m ���� ���� �ְ� �ӵ�.
    int iMax_Speed_Pulse  = iMax_Speed_unut * 1000 ;  // �ִ� ���� �ӵ��� ����  40000mm/sec * Pulse/Unit   10000 pulse / 10mm

    //iMax_Speed_Pulse = 700000 ;
    iMax_Speed_Pulse = 6000000 ;


#ifdef AJIN_AXT
    CFS20set_startstop_speed   (AjinPara.iPhysicalNo, iStart_Stop_speed     ); // �ʱ�ӵ�(Start/Stop speed) ����
    CFS20set_end_limit_enable  (AjinPara.iPhysicalNo, false                 ); // ������ ����Ʈ ��� ��������� ���� ���ܿ��� �ذ� ��.
    CFS20set_alarm_enable      (AjinPara.iPhysicalNo, false                 );

    CFS20set_enc_reverse       (AjinPara.iPhysicalNo, AjinPara.bEncReverse   ); // ���ڴ� ���� ��ȯ.
    CFS20set_enc_input_method  (AjinPara.iPhysicalNo, AjinPara.iEncInput     ); // ���ڴ� �Է� ��� ����
    CFS20set_pulse_out_method  (AjinPara.iPhysicalNo, AjinPara.iPulseOutput  ); // �޽� ��� ��� ����
    CFS20set_s_rate            (AjinPara.iPhysicalNo, AjinPara.dSCurveAcPer ,
                                                      AjinPara.dSCurveDcPer  ); // S Curve ����.

    CFS20set_pend_limit_level  (AjinPara.iPhysicalNo, AjinPara.bPLimActLevel ); // ������ ����Ʈ(+End limit)�� ��Ƽ�극�� ����
    CFS20set_nend_limit_level  (AjinPara.iPhysicalNo, AjinPara.bNLimActLevel ); // ������ ����Ʈ(-End limit)�� ��Ƽ�극�� ����
    CFS20set_alarm_level       (AjinPara.iPhysicalNo, AjinPara.bAlarmActLevel); // �˶�(Alarm) ��ȣ ��Ƽ�극�� ����
    CFS20set_inposition_level  (AjinPara.iPhysicalNo, AjinPara.bInposActLevel); // ��������(Inposition) ��ȣ ��Ƽ�극�� ����

    CFS20set_max_speed         (AjinPara.iPhysicalNo, iMax_Speed_Pulse      );

    //AXL������ ���� ����.. ã�Ƽ� �־�� �Ѵ�.
    //360�� ��ī��Ʈ.
    //AxmStatusSetPosType(AjinPara.iPhysicalNo,_bEnable ? 1 : 0,_iMinPosPulse,_iMaxPosPulse);


    //������ �Ⱦ��ų� ���ܿ��� �ذ� ��.
//  CFS20set_moveunit_perpulse (AjinPara.iPhysicalNo, UNIT_PER_PULSE        ); // Unit�� �̵������� ����. �̵��� �� �ӵ��� ������ �ش� �Ⱦ�.

//  CFS20set_end_limit_enable  (AjinPara.iPhysicalNo, AjinPara.bEndLimEnable ); // ������ ����Ʈ ��� ��������� ����
//  CFS20set_alarm_enable      (AjinPara.iPhysicalNo, AjinPara.bAlarmEnable  ); // ���� �˶� �Է½�ȣ ����� ��������� ����
//  CFS20set_inposition_enable (AjinPara.iPhysicalNo, AjinPara.bInposEnable  ); // ��������(Inposition) ��ȣ ������� ����
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmMotSetMinVel         (AjinPara.iPhysicalNo , iStart_Stop_speed             ); //����������� ������ ó�� ġ�� ������ �ӵ�. �̺��� ������ ������ �ȵ�.
    AxmMotSetMaxVel         (AjinPara.iPhysicalNo , iMax_Speed_Pulse              ); //�ִ� ���� �ӵ��� ����

    AxmSignalSetZphaseLevel (AjinPara.iPhysicalNo , AjinPara.bZphaActLevel        ); //Z�� ����.

    AxmHomeSetSignalLevel   (AjinPara.iPhysicalNo , AjinPara.bHomeActLevel        ); //Home ����.
    AxmSignalSetLimit       (AjinPara.iPhysicalNo , EMERGENCY_STOP, AjinPara.bPLimActLevel,
                                                      AjinPara.bNLimActLevel);

    AxmSignalSetServoAlarm  (AjinPara.iPhysicalNo , AjinPara.bAlarmActLevel       ); //�˶��ÿ� ������� �ʾ� ���� ����.

    AxmMotSetEncInputMethod (AjinPara.iPhysicalNo , AjinPara.iEncInput            ); //���ڴ� �Է� ��� ���� ���ڴ� ���� ��ȯ.
    AxmMotSetPulseOutMethod (AjinPara.iPhysicalNo , AjinPara.iPulseOutput         ); //�޽� ��� ��� ����

    //AxmMotSetAccelUnit      (AjinPara.iPhysicalNo , SEC                           );
    AxmSignalSetInpos       (AjinPara.iPhysicalNo , AjinPara.bInposActLevel       ); //��������(Inposition) ��ȣ ��Ƽ�극�� ����
    //360�� �޽� ��� �ϱ� ������ �������� ȣ���ؾ��Ѵ�.
    //AxmStatusSetPosType     (AjinPara.iPhysicalNo,_bEnable ? 1 : 0,_iMinPosPulse,_iMaxPosPulse);

    if(AjinPara.dSCurveAcPer == 0 && AjinPara.dSCurveDcPer == 0 ) {
        //AxmMotSetProfileMode (AjinPara.iPhysicalNo, SYM_TRAPEZOIDE_MODE );   //��Ī
        AxmMotSetProfileMode (AjinPara.iPhysicalNo, ASYM_TRAPEZOIDE_MODE);     //���Ī
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
    //�̳����� �ϸ� ��Ʈ�� �����̺� ���� ������.
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

    //������ ��ȯ�ϴ� �Լ��� ����.
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

    //������������ ��ȯ.
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
    //���� ���� ���� �ϰ� ����.
    //CFS20position_override(AjinPara.iPhysicalNo , _dNewTrgPos ); //���� ���������� ���� ���� ���۰��� �ڵ������� ����.

    //Check InPos.
    bool bRet = GetStop();
    return bRet ;
#endif //AJIN_AXT
#ifdef AJIN_AXL
    DWORD dwRet ;
    //Go Motor.           //��밪 ���� ... ���� ������ �ۿ�.
    dwRet = AxmOverridePos(AjinPara.iPhysicalNo , _dNewTrgPos - _dSttPos ); //���� ���������� ���� ���� ���۰��� �ڵ������� ����.

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
    //- lim �̰��� �������� ���.
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
    //- lim �̰��� �������� ���.
    //else if(_ftFindEdgeType == ftNELUpEdge || _ftFindEdgeType == ftNELDnEdge) {
    //    if(AjinPara.bNLimActLevel) {
    //        ftFindEdgeType = _ftFindEdgeType ;
    //    }
    //    else {
    //        ftFindEdgeType = (_ftFindEdgeType == ftNELUpEdge) ? ftNELDnEdge : ftNELUpEdge ;
    //    }
    //}

    //Z ������ �������� �Լ��� ���� �ؼ� �ڵ� ����. ���� �˾Ҵµ� �𸣰���....
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
     PosEndLimit                            = 0x0,            // +Elm(End limit) +���� ����Ʈ ���� ��ȣ
     NegEndLimit                            = 0x1,            // -Elm(End limit) -���� ����Ʈ ���� ��ȣ
     PosSloLimit                            = 0x2,            // +Slm(Slow Down limit) ��ȣ - ������� ����
     NegSloLimit                            = 0x3,            // -Slm(Slow Down limit) ��ȣ - ������� ����
     HomeSensor                             = 0x4,            // IN0(ORG)  ���� ���� ��ȣ
     EncodZPhase                            = 0x5,            // IN1(Z��)  Encoder Z�� ��ȣ
     UniInput02                             = 0x6,            // IN2(����) ���� �Է� 2�� ��ȣ
     UniInput03                             = 0x7,            // IN3(����) ���� �Է� 3�� ��ȣ

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
    */

    // ���� ���� ���� ���� : ��ٸ��� ����, ���� �ð� ��� ���, ���� ���� ���

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
    //- lim �̰��� �������� ���.
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
    //- lim �̰��� �������� ���.
    //else if(_ftFindEdgeType == ftNELUpEdge || _ftFindEdgeType == ftNELDnEdge) {
    //    if(AjinPara.bNLimActLevel) {
    //        ftFindEdgeType = _ftFindEdgeType ;
    //    }
    //    else {
    //        ftFindEdgeType = (_ftFindEdgeType == ftNELUpEdge) ? ftNELDnEdge : ftNELUpEdge ;
    //    }
    //}

    //Z ������ �Լ��� �����Ͽ� �ڵ� ��ȯ. �ڵ����� �ȵǴ� �� ��.......
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



    // ���� ���� ���� ���� : ��ٸ��� ����, ���� �ð� ��� ���, ���� ���� ���
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

    // ���ͷ�Ʈ �޼��� ���� �κ�.----------------------------------------------------------------------------------
    // ���ͷ�Ʈ ���� �κ�(�� ���������� ���ͷ�Ʈ �߻� Ƚ���� ī��Ʈ �ϱ����� �����, ���� �ʿ����� ����.)
    static bool bFirst = true ;
    if(bFirst)CFS20SetWindowMessage(NULL, NULL , TrgCallBack);
    bFirst = false ;
    // User Selectable Interrupt Source0����
    // Trigger �߻��ÿ� ���ͷ�Ʈ�� �߻��ϵ��� ������ �����Ѵ�.
    CFS20KeSetCommandData(m_iAxisNo, FsUserInterruptSourceSelectRegWrite,(EVENT_NONE << 24)
                                                                        |(EVENT_NONE << 16)
                                                                        |(EVENT_NONE << 8)
                                                                        |(EVENT_TRIGGER_ENABLE) );

    // Interrupt Mask �������Ϳ� User Selectable Interrupt Source 0��
    // ���ͷ�Ʈ�� ����ϵ��� �����Ѵ�. ( Bit28 )
    CFS20KeSetCommandData(m_iAxisNo, FsInterruptMaskWrite, 0x10000000);
    //------------------------------------------------------------------------------------------------------------

    CFS20Trig_Set_Level(m_iAxisNo, _bOnLevel?1:0); // Ʈ���� ���� ����

    //    double trig_pos[] = {	20000.0, 30000.0, 40000.0, 50000.0, 70000.0, 75000.0, 80000.0, 81000.0};

    CFS20Trig_Absolute_Profile_Set(m_iAxisNo, _iPosCnt , _dPos, _dTrgTime , _bActual);
    //CFS20Trig_Absolute_Profile_Set(m_iAxisNo, 8 , trig_pos, 1000 , _bActual);
#endif //AJIN_AXT
#ifdef AJIN_AXL
    //�����غ�����.

    // 0�࿡ Ʈ���� ��ȣ�� ������ ���ӽð��� �����Ѵ�.
    double dTrigTime = _dTrgTime; //1~50000(50ms)
    WORD uTriggerLevel = _bOnLevel ? HIGH : LOW ;
    DWORD uSelect = _bActual ? 1 : 0; // Encorder or Command Position �� �������� Ʈ���� �߻�
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
    // 0�࿡ Ʈ���� ��ȣ�� ������ ���ӽð��� �����Ѵ�.
    double dTrigTime = _iTime; // 4ms D
    WORD uTriggerLevel = HIGH ; //_bOnLevel ? HIGH : LOW ;
    DWORD uSelect = 1 ;//_bActual ? 1 : 0; // Encorder or Command Position �� �������� Ʈ���� �߻�
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
    1.���ͷ�Ʈ�� �߻��� ACT POS �� ����
    2.QiCNTC2Write = ATC POS �񱳱��� adress
    3.���� : �ش� ��,QiCNTC2Write, ���ͷ�Ʈ�� �߻��� ��ġ(�޽� ������ ���� ���־�� �Ѵ�.)
    */
    AxmSetCommandData32Qi(AjinPara.iPhysicalNo,QiCNTC2Write,_dPos);
    /*
    ��ü ���ͷ�Ʈ Enable
    */
    AxlInterruptEnable();
    /*
    �ش��� ���ͷ�Ʈ Enable
    */
    AxmInterruptSetAxisEnable(AjinPara.iPhysicalNo, ENABLE);
    /*
    AxmInterruptSetUserEnable
    1.���ͷ�Ʈ �� � �̺�Ʈ�� �߻����� ���� �Ѵ�
    2.QIINTBANK1_7 = ACT POS ���� ���� ���� �� �񱳱� ���� ��ġ�ϸ� �߻�
    3.���� : �ش���,BANK ��ȣ ->  QIINTBANK1_7 �� BANK1 �� ���� �ǹǷ� 0 ,QIINTBANK1_7
    */
    AxmInterruptSetUserEnable(AjinPara.iPhysicalNo, 0, QIINTBANK1_7);
    /*
    ���ͷ�Ʈ �߻��� ȣ��� �ù� �Լ� ���
    */
    AxmInterruptSetAxis(AjinPara.iPhysicalNo, NULL, NULL, _pCallback,NULL);


    /*
    ���ͷ�Ʈ ����� �ʿ����� ���� ��쿡�� ���ͷ�Ʈ�� Disable ���ֽñ� �ٶ��ϴ�.
    ���� �ڷ�� AXM �޴��� ���� �ٶ��ϴ�.

    ���� �Լ����� ��� �ϱ� ���Ͽ�
    ÷���� AXDev.h , AXHD.h ,AXHS.h �� include ���ֽñ� �ٶ��ϴ�.

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
    �ش��� ���ͷ�Ʈ Enable
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


/* ���� ������ ���� �߰� 20150410 juhyeon */

// ���� ������ ���� ����� ��ǥ��� ������ �����ϴ� �Լ�
// _iCoord = ��ǥ�� ��ȣ
// _iAxisCnt = ���� ������ ����� ���� ����
// _ipAxisNo = ���� ������ ����� ���� �ѹ�
void CAjinMotion::ContiSetAxisMap (int _iCoord, int _iAxisCnt, int *_ipAxisNo)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmContiWriteClear(_iCoord);
    AxmContiSetAxisMap(_iCoord, _iAxisCnt, (long *)_ipAxisNo); // int -> long
#endif //AJIN_AXL
}

// ���� ������ �־ ���� ��� = 0 & ��� ��� = 1 ����
void CAjinMotion::ContiSetAbsRelMode (int _iCoord, DWORD _dwAbsRelMode)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmContiSetAbsRelMode(_iCoord, _dwAbsRelMode);
#endif //AJIN_AXL
}

// ���� ������ �ϱ� ���� �۾����� ����� �����ϴ� �Լ�
void CAjinMotion::ContiBeginNode (int _iCoord)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmContiBeginNode(_iCoord);
#endif //AJIN_AXL
}

// ���� ������ �ϱ� ���� �۾����� ����� �����ϴ� �Լ�
void CAjinMotion::ContiEndNode (int _iCoord)
{
#ifdef AJIN_AXT
#endif //AJIN_AXT
#ifdef AJIN_AXL
    AxmContiEndNode(_iCoord);
#endif //AJIN_AXL
}

// ť�� ��ϵ� �۾����� ���� ������ �����ϱ� ���� �Լ�
// _dwProfileset = 0, �Ϲ� ���� ���
// _dwProfileset = 1, ��� ������ ���� ���
// _dwProfileset = 2, �ڵ� ������ ���� ���
// _iAngle = 0 ~ 360��, �ڵ� ������ ���� ��� ���� Angle ��
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

// ���� ���� ���
// _dpEndPos = ������ ���� ����
// _dVel = �ӵ�
// _dAcc = ���ӵ�
// _dDec = ���ӵ�
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
    //���α׷� �Ѱ� ���� ���� Ȩ ������ �������� �������� �ȵȻ����� ��찡 �־ �߸ŷ� �̷��� �Ѵ�.
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

    //[0000] AXT_RT_SUCCESS : AXL ���̺귯���ʱ�ȭ����
    //[4053] AXT_RT_MOTION_NOT_INITIAL_AXIS_NO : �ش������ʱ�ȭ����
    //[4101] AXT_RT_MOTION_INVALID_AXIS_NO : �ش�����������������
    unsigned long ulRet ;
    ulRet = AxmHomeGetResult(AjinPara.iPhysicalNo , _dwResult);

    AXT_MOTION_HOME_RESULT_DEF
#endif //AJIN_AXL

}
