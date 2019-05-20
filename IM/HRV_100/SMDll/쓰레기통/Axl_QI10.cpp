//---------------------------------------------------------------------------
#pragma hdrstop
#include "Axl_QI10.h"

#ifdef AJIN_AXL

//System header
#include <mem.h>
#include <math.h>

//Ajin Lib
#include "AXL.h"
#include "AXM.h"

//User Header
#include "Timer.h"
#include "UserIni.h"
#include "SLogUnit.h"
#include "FormMsgOk.h"

#pragma package(smart_init)

#pragma comment(lib, "AXL.lib")            // Ajinextek Library

//AXT Trigger
//#pragma comment(lib, "AxtTriggerLib.lib")     // Ajinextek Trigger Library
//#include "AXTKeLIB.h"             // Base Board�� 1�� Low Level �Լ��� ����ϱ� ����
//#include "AxtKeCamcFs20.h"        // FS20�� 1�� Low Level �Լ��� ����ϱ� ����
//#include "AxtTrigger.h"           // Tirgger Function ����� ���� �������

#define     TRIG_DNEG	0       // Trigger Doun Edge
#define     TRIG_UPEG	1       // Trigger Up Edge

SAxlParaUi AxlParaUi ;

bool bInitOk ;

void MakeAxlUI()
{
    static bool bFirst = true ;
    if(!bFirst) return ;
    bFirst = false ;

    AxlParaUi.pnBase = new TPanel(Application);

    AxlParaUi.edPhysicalNo   = new TEdit    (AxlParaUi.pnBase); AxlParaUi.edPhysicalNo   -> Parent = AxlParaUi.pnBase ;                                                               SetAxlArrange(AxlParaUi.edPhysicalNo   , 1 , 0 );
    AxlParaUi.lbPhysicalNo   = new TLabel   (AxlParaUi.pnBase); AxlParaUi.lbPhysicalNo   -> Parent = AxlParaUi.pnBase ; AxlParaUi.lbPhysicalNo   -> Caption = "Physical No        " ; SetAxlArrange(AxlParaUi.lbPhysicalNo   , 0 , 0 );

    AxlParaUi.cbUseSCurve    = new TCheckBox(AxlParaUi.pnBase); AxlParaUi.cbUseSCurve    -> Parent = AxlParaUi.pnBase ; AxlParaUi.cbUseSCurve    -> Caption = "Use S Curve        " ; SetAxlArrange(AxlParaUi.cbUseSCurve    , 0 , 1 );

    AxlParaUi.cbEncReverse   = new TCheckBox(AxlParaUi.pnBase); AxlParaUi.cbEncReverse   -> Parent = AxlParaUi.pnBase ; AxlParaUi.cbEncReverse   -> Caption = "Enc Reverse        " ; SetAxlArrange(AxlParaUi.cbEncReverse   , 0 , 2 );

    AxlParaUi.cbEncInput     = new TComboBox(AxlParaUi.pnBase); AxlParaUi.cbEncInput     -> Parent = AxlParaUi.pnBase ;                                                               SetAxlArrange(AxlParaUi.cbEncInput     , 1 , 3 );
    AxlParaUi.lbEncInput     = new TLabel   (AxlParaUi.pnBase); AxlParaUi.lbEncInput     -> Parent = AxlParaUi.pnBase ; AxlParaUi.lbEncInput     -> Caption = "Enc Input          " ; SetAxlArrange(AxlParaUi.lbEncInput     , 0 , 3 );

    AxlParaUi.cbPulseOutput  = new TComboBox(AxlParaUi.pnBase); AxlParaUi.cbPulseOutput  -> Parent = AxlParaUi.pnBase ;                                                               SetAxlArrange(AxlParaUi.cbPulseOutput  , 1 , 4 );
    AxlParaUi.lbPulseOutput  = new TLabel   (AxlParaUi.pnBase); AxlParaUi.lbPulseOutput  -> Parent = AxlParaUi.pnBase ; AxlParaUi.lbPulseOutput  -> Caption = "Pulse Output Method" ; SetAxlArrange(AxlParaUi.lbPulseOutput  , 0 , 4 );

    AxlParaUi.edSCurveAcPer  = new TEdit    (AxlParaUi.pnBase); AxlParaUi.edSCurveAcPer  -> Parent = AxlParaUi.pnBase ;                                                               SetAxlArrange(AxlParaUi.edSCurveAcPer  , 1 , 5 );
    AxlParaUi.lbSCurveAcPer  = new TLabel   (AxlParaUi.pnBase); AxlParaUi.lbSCurveAcPer  -> Parent = AxlParaUi.pnBase ; AxlParaUi.lbSCurveAcPer  -> Caption = "S Curve Acc %      " ; SetAxlArrange(AxlParaUi.lbSCurveAcPer  , 0 , 5 );

    AxlParaUi.edSCurveDcPer  = new TEdit    (AxlParaUi.pnBase); AxlParaUi.edSCurveDcPer  -> Parent = AxlParaUi.pnBase ;                                                               SetAxlArrange(AxlParaUi.edSCurveDcPer  , 1 , 6 );
    AxlParaUi.lbSCurveDcPer  = new TLabel   (AxlParaUi.pnBase); AxlParaUi.lbSCurveDcPer  -> Parent = AxlParaUi.pnBase ; AxlParaUi.lbSCurveDcPer  -> Caption = "S Curve Dcc %      " ; SetAxlArrange(AxlParaUi.lbSCurveDcPer  , 0 , 6 );

    AxlParaUi.cbHomeActLevel = new TCheckBox(AxlParaUi.pnBase); AxlParaUi.cbHomeActLevel -> Parent = AxlParaUi.pnBase ; AxlParaUi.cbHomeActLevel -> Caption = "Home Active High   " ; SetAxlArrange(AxlParaUi.cbHomeActLevel , 0 , 7 );

    AxlParaUi.cbPLimActLevel = new TCheckBox(AxlParaUi.pnBase); AxlParaUi.cbPLimActLevel -> Parent = AxlParaUi.pnBase ; AxlParaUi.cbPLimActLevel -> Caption = "+ Limit Active High" ; SetAxlArrange(AxlParaUi.cbPLimActLevel , 0 , 8 );

    AxlParaUi.cbNLimActLevel = new TCheckBox(AxlParaUi.pnBase); AxlParaUi.cbNLimActLevel -> Parent = AxlParaUi.pnBase ; AxlParaUi.cbNLimActLevel -> Caption = "- Limit Active High" ; SetAxlArrange(AxlParaUi.cbNLimActLevel , 0 , 9 );

    AxlParaUi.cbAlarmActLevel= new TCheckBox(AxlParaUi.pnBase); AxlParaUi.cbAlarmActLevel-> Parent = AxlParaUi.pnBase ; AxlParaUi.cbAlarmActLevel-> Caption = "Alarm Active High  " ; SetAxlArrange(AxlParaUi.cbAlarmActLevel, 0 , 10);

    AxlParaUi.cbInposActLevel= new TCheckBox(AxlParaUi.pnBase); AxlParaUi.cbInposActLevel-> Parent = AxlParaUi.pnBase ; AxlParaUi.cbInposActLevel-> Caption = "Inpos Active High  " ; SetAxlArrange(AxlParaUi.cbInposActLevel, 0 , 11);




//    AxlParaUi.edSCurveDcPer -> Left = 1 ;
//    AxlParaUi.edSCurveDcPer -> Top  = 1 ;
/*
    AxlParaUi.cbEncInput    -> AddItem("Up/Down"         ,NULL);
    AxlParaUi.cbEncInput    -> AddItem("1 multiplication",NULL);
    AxlParaUi.cbEncInput    -> AddItem("2 multiplication",NULL);
    AxlParaUi.cbEncInput    -> AddItem("4 multiplication",NULL); //���� 4ü��.

    AxlParaUi.cbPulseOutput -> AddItem("OneHighLowHigh"  ,NULL); // 0x0, 1�޽� ���, PULSE(Active High), ������(DIR=Low)  / ������(DIR=High)
    AxlParaUi.cbPulseOutput -> AddItem("OneHighHighLow"  ,NULL); // 0x1, 1�޽� ���, PULSE(Active High), ������(DIR=High) / ������(DIR=Low)
    AxlParaUi.cbPulseOutput -> AddItem("OneLowLowHigh "  ,NULL); // 0x2, 1�޽� ���, PULSE(Active Low),  ������(DIR=Low)  / ������(DIR=High)
    AxlParaUi.cbPulseOutput -> AddItem("OneLowHighLow "  ,NULL); // 0x3, 1�޽� ���, PULSE(Active Low),  ������(DIR=High) / ������(DIR=Low)
    AxlParaUi.cbPulseOutput -> AddItem("TwoCcwCwHigh  "  ,NULL); // 0x4, 2�޽� ���, PULSE(CCW:������),  DIR(CW:������),  Active High
    AxlParaUi.cbPulseOutput -> AddItem("TwoCcwCwLow   "  ,NULL); // 0x5, 2�޽� ���, PULSE(CCW:������),  DIR(CW:������),  Active Low
    AxlParaUi.cbPulseOutput -> AddItem("TwoCwCcwHigh  "  ,NULL); // 0x6, 2�޽� ���, PULSE(CW:������),   DIR(CCW:������), Active High
    AxlParaUi.cbPulseOutput -> AddItem("TwoCwCcwLow   "  ,NULL); // 0x7, 2�޽� ���, PULSE(CW:������),   DIR(CCW:������), Active Low
*/
}

void SetAxlUIParent (TPanel * _pnBase)
{
//  AxlParaUi.pnBase -> Parent = _pnBase ;
//  AxlParaUi.pnBase -> ParentWindow = _pnse ;
//  Parent = dynamic_cast<TWinControl *>(Owner);
//    AxlParaUi.pnBase = new TPanel(_pnBase);
//    AxlParaUi.pnBase -> Parent = _pnBase ;

    AxlParaUi.pnBase -> Parent = _pnBase  ;

    AxlParaUi.pnBase -> Align  = alClient ;

//    AxlParaUi.pnBase -> ParentFont = true ;

    AxlParaUi.cbEncInput    -> AddItem("+ Up/Down"         ,NULL);
    AxlParaUi.cbEncInput    -> AddItem("+ 1 multiplication",NULL);
    AxlParaUi.cbEncInput    -> AddItem("+ 2 multiplication",NULL);
    AxlParaUi.cbEncInput    -> AddItem("+ 4 multiplication",NULL); // ���� 4ü��.
    AxlParaUi.cbEncInput    -> AddItem("- Up/Down"         ,NULL); // ���� 4ü��.
    AxlParaUi.cbEncInput    -> AddItem("- 1 multiplication",NULL); // ���� 4ü��.
    AxlParaUi.cbEncInput    -> AddItem("- 2 multiplication",NULL); // ���� 4ü��.
    AxlParaUi.cbEncInput    -> AddItem("- 4 multiplication",NULL); // ���� 4ü��.

    AxlParaUi.cbPulseOutput -> AddItem("OneHighLowHigh"  ,NULL); // 0x0, 1�޽� ���, PULSE(Active High), ������(DIR=Low)  / ������(DIR=High)
    AxlParaUi.cbPulseOutput -> AddItem("OneHighHighLow"  ,NULL); // 0x1, 1�޽� ���, PULSE(Active High), ������(DIR=High) / ������(DIR=Low)
    AxlParaUi.cbPulseOutput -> AddItem("OneLowLowHigh "  ,NULL); // 0x2, 1�޽� ���, PULSE(Active Low),  ������(DIR=Low)  / ������(DIR=High)
    AxlParaUi.cbPulseOutput -> AddItem("OneLowHighLow "  ,NULL); // 0x3, 1�޽� ���, PULSE(Active Low),  ������(DIR=High) / ������(DIR=Low)
    AxlParaUi.cbPulseOutput -> AddItem("TwoCcwCwHigh  "  ,NULL); // 0x4, 2�޽� ���, PULSE(CCW:������),  DIR(CW:������),  Active High
    AxlParaUi.cbPulseOutput -> AddItem("TwoCcwCwLow   "  ,NULL); // 0x5, 2�޽� ���, PULSE(CCW:������),  DIR(CW:������),  Active Low
    AxlParaUi.cbPulseOutput -> AddItem("TwoCwCcwHigh  "  ,NULL); // 0x6, 2�޽� ���, PULSE(CW:������),   DIR(CCW:������), Active High
    AxlParaUi.cbPulseOutput -> AddItem("TwoCwCcwLow   "  ,NULL); // 0x7, 2�޽� ���, PULSE(CW:������),   DIR(CCW:������), Active Low

}

const int iControlWidth   = 165 ;
const int iControlHeight  = 25  ;
const int iStartRowOffset = 15   ;
const int iStartColOffset = 20   ;

void SetAxlArrange (TControl * _tcControl , int _iCol , int _iRow)
{
    _tcControl -> Left = _iCol * iControlWidth  * 1.1 + _iCol + iStartColOffset ; _tcControl -> Width = iControlWidth  ;
    _tcControl -> Top  = _iRow * iControlHeight * 1.1 + _iRow + iStartRowOffset ; _tcControl -> Height= iControlHeight ;
}

void DeleteAxlUI()
{
    if(AxlParaUi.pnBase) {
        delete AxlParaUi.pnBase ; //������Ʈ�� �����ֳ� Parent�����ÿ� ��� ���� ��.
        AxlParaUi.pnBase = NULL ;
    }
}


int GetAxlQI10AxisCnt()
{

}

bool InitAxlQI10Board()
{
    bInitOk = true ;

    if(!AxlIsOpened()){				// ���� ���̺귯���� ��� �������� (�ʱ�ȭ�� �Ǿ�����) Ȯ��
        if(AxlOpen(7) != AXT_RT_SUCCESS){			// ���� ���̺귯�� �ʱ�ȭ
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
    return true ;
}

void CloseAxlQI10Board()
{
    AxlClose();
}

//---CAxlQI10 Unit-------------------------------------------------------------
CAxlQI10::CAxlQI10(int _iAxisNo)
{
    InitLib();
    MakeAxlUI();

    memset(&AxlPara , 0 , sizeof(SAxlPara));

    m_iAxisNo = _iAxisNo ;

    Load();
}

CAxlQI10::~CAxlQI10()
{
//    DeleteAxlUI();
    CloseAxlQI10Board();
}


// ���� ���̺귯�� �� ��� �ʱ�ȭ
bool CAxlQI10::InitLib()
{
    static bool bFirst = true ;
    if(!bFirst) return true ;
    bFirst = false ;

    InitAxlQI10Board();
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

bool CAxlQI10::GetStop()
{
    //return CFS20motion_done(AxlPara.iPhysicalNo) ;

    DWORD	dwInMotion;
	BOOL	bRet;
    AxmStatusReadInMotion(AxlPara.iPhysicalNo, &dwInMotion);
	bRet = !dwInMotion;
	return bRet;
}

void CAxlQI10::ApplyAxlPara ()
{
    const int iStart_Stop_speed = 10    ; //����������� ������ ó�� ġ�� ������ �ӵ�. �̺��� ������ ������ �ȵ�.
    const int iMax_Speed_unut   = 40000 ; //�ʴ� 2000����.  mm�����̸� �ʴ� 2000mm �� 2m ���� ���� �ְ� �ӵ�.
    const int iMax_Speed_Pulse  = iMax_Speed_unut * 1000 ;  // �ִ� ���� �ӵ��� ����  40000mm/sec * Pulse/Unit   10000 pulse / 10mm
    AxmMotSetMinVel         (AxlPara.iPhysicalNo , iStart_Stop_speed             ); //����������� ������ ó�� ġ�� ������ �ӵ�. �̺��� ������ ������ �ȵ�.
    AxmMotSetMaxVel         (AxlPara.iPhysicalNo , iMax_Speed_Pulse              ); //�ִ� ���� �ӵ��� ����

	AxmSignalSetLimit       (AxlPara.iPhysicalNo , EMERGENCY_STOP, UNUSED, UNUSED); //-,+�Ѵ� �Ⱦ�. ���� ����.
    AxmSignalSetServoAlarm  (AxlPara.iPhysicalNo , UNUSED                        ); //�˶��ÿ� ������� �ʾ� ���� ����.
    AxmMotSetEncInputMethod (AxlPara.iPhysicalNo , AxlPara.iEncInput             ); //���ڴ� �Է� ��� ���� ���ڴ� ���� ��ȯ.
    AxmMotSetPulseOutMethod (AxlPara.iPhysicalNo , AxlPara.iPulseOutput          ); //�޽� ��� ��� ����
    AxmSignalSetInpos       (AxlPara.iPhysicalNo , AxlPara.bInposActLevel        ); //��������(Inposition) ��ȣ ��Ƽ�극�� ����
    AxmMotSetAccelUnit      (AxlPara.iPhysicalNo , SEC                           );

    if(AxlPara.dSCurveAcPer == 0 && AxlPara.dSCurveDcPer == 0 ) {
        AxmMotSetProfileMode (AxlPara.iPhysicalNo, SYM_TRAPEZOIDE_MODE );
    }
    else if(AxlPara.dSCurveAcPer == AxlPara.dSCurveDcPer ) {
        AxmMotSetProfileMode(AxlPara.iPhysicalNo , SYM_S_CURVE_MODE    );
        AxmMotSetAccelJerk  (AxlPara.iPhysicalNo , AxlPara.dSCurveAcPer);
        AxmMotSetDecelJerk  (AxlPara.iPhysicalNo , AxlPara.dSCurveDcPer);
    }
    else {
        AxmMotSetProfileMode(AxlPara.iPhysicalNo , ASYM_S_CURVE_MODE   );
        AxmMotSetAccelJerk  (AxlPara.iPhysicalNo , AxlPara.dSCurveAcPer);
        AxmMotSetDecelJerk  (AxlPara.iPhysicalNo , AxlPara.dSCurveDcPer);
    }
}

//�������.
void CAxlQI10::SetServo(bool _bOn)
{
    const int iServoOnBit = 0 ;

    //Stop the Motor.
    Stop();

    //Servo On/Off.
    AxmSignalWriteOutputBit(AxlPara.iPhysicalNo, iServoOnBit, _bOn);

}

bool CAxlQI10::GetServo()
{
    const int iServoOnBit = 0 ;

    DWORD dwpServoOnLevel ;
    AxmSignalReadOutputBit(AxlPara.iPhysicalNo,iServoOnBit,&dwpServoOnLevel);
	return dwpServoOnLevel ;
}

void CAxlQI10::SetReset(bool _bOn)
{
    const int iResetBit = 1 ;
    //Servo On/Off.
    AxmSignalWriteOutputBit(AxlPara.iPhysicalNo, iResetBit, _bOn);
}

double CAxlQI10::GetCmdPos()
{
    double dPos;
    AxmStatusGetCmdPos(AxlPara.iPhysicalNo, &dPos);
    return dPos;
}

double CAxlQI10::GetEncPos()
{
    double dPos;
    AxmStatusGetActPos(AxlPara.iPhysicalNo, &dPos);
    return dPos;

}

void CAxlQI10::SetPos (double _dPos )
{
    AxmStatusSetCmdPos(AxlPara.iPhysicalNo , _dPos); //Commnad Position.
    AxmStatusSetActPos(AxlPara.iPhysicalNo , _dPos); //Actual Position.
}

bool CAxlQI10::GetHomeSnsr()
{
    //Home Sensor
    bool bRet ;
    BYTE bInput ;
    unsigned long lInput ;

    AxmSignalReadInput(AxlPara.iPhysicalNo, &lInput);
    bInput = (BYTE)lInput ;

    bRet = (bInput >> 0 ) & 0x01 ;
    bRet = AxlPara.bHomeActLevel ?  bRet : !bRet ;

    return bRet ;
}

bool CAxlQI10::GetNLimSnsr()
{
    //Get Mechanical Signal.
    bool bRet ;
    BYTE bInput ;
    unsigned long lInput ;

	AxmStatusReadMechanical(AxlPara.iPhysicalNo, &lInput);
    bInput = (BYTE)lInput ;

    bRet = (bInput >> 1) & 0x01;

    return bRet ;
}

bool CAxlQI10::GetPLimSnsr()
{
    //Get Mechanical Signal.
    bool bRet ;
    BYTE bInput ;
    unsigned long lInput ;

	AxmStatusReadMechanical(AxlPara.iPhysicalNo, &lInput);
    bInput = (BYTE)lInput ;

    bRet = (bInput >> 0) & 0x01;

    return bRet ;
}

bool CAxlQI10::GetZphaseSnsr()
{
    //Home Sensor
    bool bRet ;
    BYTE bInput ;
    unsigned long lInput ;

    AxmSignalReadInput(AxlPara.iPhysicalNo, &lInput);
    bInput = (BYTE)lInput ;

    bRet = (bInput >> 1 ) & 0x01 ;

    return bRet ;
}

bool CAxlQI10::GetPackInPos()
{
    //Get Mechanical Signal.
    bool bRet ;
    BYTE bInput ;
    unsigned long lInput ;

	AxmStatusReadMechanical(AxlPara.iPhysicalNo, &lInput);
    bInput = (BYTE)lInput ;

    bRet = (bInput >> 5) & 0x01;

    return bRet ;
}

bool CAxlQI10::GetAlarm()
{
        //Get Mechanical Signal.
    bool bRet ;
    BYTE bInput ;
    unsigned long lInput ;

	AxmStatusReadMechanical(AxlPara.iPhysicalNo, &lInput);
    bInput = (BYTE)lInput ;

    bRet = (bInput >> 4) & 0x01;

    return bRet ;
}

bool CAxlQI10::GetHomeDone()
{
    bool  bRet ;
    unsigned long lRet ;

    AxmHomeGetResult(AxlPara.iPhysicalNo, &lRet);

    bRet = (lRet == HOME_SUCCESS) ;

    return bRet ;
}

bool CAxlQI10::Stop()
{
	return (AXT_RT_SUCCESS ==AxmMoveSStop(AxlPara.iPhysicalNo)) ;
    //AxmMoveSStop(AxlPara.iPhysicalNo);
}

bool CAxlQI10::EmgStop()
{
	return (AXT_RT_SUCCESS ==AxmMoveEStop(AxlPara.iPhysicalNo));
    //AxmMoveEStop(AxlPara.iPhysicalNo);
}

bool CAxlQI10::JogP(double _dVel , double _dAcc)
{
    bool Rslt = false;
	DWORD	dwRet;
	dwRet	= AxmMoveVel(AxlPara.iPhysicalNo,  _dVel , _dAcc, _dAcc);
	Rslt	= (BOOL)dwRet == AXT_RT_SUCCESS ? TRUE : FALSE;

    return Rslt;
}

bool CAxlQI10::JogN(double _dVel , double _dAcc)
{
    bool Rslt = false;
	DWORD	dwRet;
	dwRet	= AxmMoveVel(AxlPara.iPhysicalNo,  -1 * _dVel , _dAcc, _dAcc);
	Rslt	= (BOOL)dwRet == AXT_RT_SUCCESS ? TRUE : FALSE;

    return Rslt;
}

bool CAxlQI10::GoAbs(double _dPos , double _dVel , double _dAcc)
{
    //Go Motor.
    if (GetStop()) {
	    AxmMoveStartPos(AxlPara.iPhysicalNo, _dPos , _dVel , _dAcc , _dAcc );
    }

    //Check InPos.
    bool bRet = GetStop();
    return bRet ;
}

bool CAxlQI10::GoOvr(double _dPos)
{
    //Go Motor.
    AxmOverridePos(AxlPara.iPhysicalNo , _dPos ); //���� ���������� ���� ���� ���۰��� �ڵ������� ����.

    //Check InPos.
    bool bRet = GetStop();
    return bRet ;
}

bool CAxlQI10::FindEdgeStop (double _dVel ,double _dAcc ,EN_FINDEDGE_TYPE _ftFindEdgeType)
{
    const int iSensorKind = 8 ;

    AXT_MOTION_HOME_DETECT_SIGNAL iSignal ;
    AXT_MOTION_EDGE               iEdge   ;

    iSignal = (AXT_MOTION_HOME_DETECT_SIGNAL) (_ftFindEdgeType % iSensorKind) ;
    iEdge   = (_ftFindEdgeType / iSensorKind) ? SIGNAL_UP_EDGE  : SIGNAL_DOWN_EDGE ;



    // ���� ���� ���� ���� : ��ٸ��� ����, ���� �ð� ��� ���, ���� ���� ���
    return (AXT_RT_SUCCESS ==AxmMoveSignalSearch(AxlPara.iPhysicalNo, _dVel, _dAcc, iSignal, iEdge , SLOWDOWN_STOP));
}

bool CAxlQI10::FindEdgeEmgStop(double _dVel  ,double _dAcc ,EN_FINDEDGE_TYPE _ftFindEdgeType)
{
    const int iSensorKind = 8 ;

    AXT_MOTION_HOME_DETECT_SIGNAL iSignal ;
    AXT_MOTION_EDGE               iEdge   ;

    iSignal = (AXT_MOTION_HOME_DETECT_SIGNAL) (_ftFindEdgeType % iSensorKind) ;
    iEdge   = (_ftFindEdgeType / iSensorKind) ? SIGNAL_UP_EDGE  : SIGNAL_DOWN_EDGE ;



    // ���� ���� ���� ���� : ��ٸ��� ����, ���� �ð� ��� ���, ���� ���� ���
    return (AXT_RT_SUCCESS == AxmMoveSignalSearch(AxlPara.iPhysicalNo, _dVel, _dAcc, iSignal, iEdge , EMERGENCY_STOP)) ;
}

void CAxlQI10::SetAxlPara (SAxlPara &_AxlPara ){memcpy(& AxlPara , &_AxlPara , sizeof(SAxlPara ));}
void CAxlQI10::GetAxlPara (SAxlPara &_AxlPara ){memcpy(&_AxlPara , & AxlPara , sizeof(SAxlPara ));}


void CAxlQI10::Save()
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
    UserINI.Save(sPath, "AxlPara"  , "iPhysicalNo   " , AxlPara.iPhysicalNo   );
    UserINI.Save(sPath, "AxlPara"  , "bUseSCurve    " , AxlPara.bUseSCurve    );
    UserINI.Save(sPath, "AxlPara"  , "bEncReverse   " , AxlPara.bEncReverse   );
    UserINI.Save(sPath, "AxlPara"  , "iEncInput     " , AxlPara.iEncInput     );
    UserINI.Save(sPath, "AxlPara"  , "iPulseOutput  " , AxlPara.iPulseOutput  );
    UserINI.Save(sPath, "AxlPara"  , "dSCurveAcPer  " , AxlPara.dSCurveAcPer  );
    UserINI.Save(sPath, "AxlPara"  , "dSCurveDcPer  " , AxlPara.dSCurveDcPer  );
    UserINI.Save(sPath, "AxlPara"  , "bHomeActLevel " , AxlPara.bHomeActLevel );
    UserINI.Save(sPath, "AxlPara"  , "bPLimActLevel " , AxlPara.bPLimActLevel );
    UserINI.Save(sPath, "AxlPara"  , "bNLimActLevel " , AxlPara.bNLimActLevel );
    UserINI.Save(sPath, "AxlPara"  , "bAlarmActLevel" , AxlPara.bAlarmActLevel);
    UserINI.Save(sPath, "AxlPara"  , "bInposActLevel" , AxlPara.bInposActLevel);

    ApplyAxlPara ();

}
void CAxlQI10::Load()
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sAxisNo ;
    AnsiString sPath   ;

    //Set Dir.
    sAxisNo = (AnsiString)m_iAxisNo ;
    sPath   = EXE_FOLDER + "Util\\Motor_" + sAxisNo + "_Para.INI" ;

    //Save Device.
    UserINI.Load(sPath, "AxlPara"  , "iPhysicalNo   " , AxlPara.iPhysicalNo   );
    UserINI.Load(sPath, "AxlPara"  , "bUseSCurve    " , AxlPara.bUseSCurve    );
    UserINI.Load(sPath, "AxlPara"  , "bEncReverse   " , AxlPara.bEncReverse   );
    UserINI.Load(sPath, "AxlPara"  , "iEncInput     " , AxlPara.iEncInput     );
    UserINI.Load(sPath, "AxlPara"  , "iPulseOutput  " , AxlPara.iPulseOutput  );
    UserINI.Load(sPath, "AxlPara"  , "dSCurveAcPer  " , AxlPara.dSCurveAcPer  );
    UserINI.Load(sPath, "AxlPara"  , "dSCurveDcPer  " , AxlPara.dSCurveDcPer  );
    UserINI.Load(sPath, "AxlPara"  , "bHomeActLevel " , AxlPara.bHomeActLevel );
    UserINI.Load(sPath, "AxlPara"  , "bPLimActLevel " , AxlPara.bPLimActLevel );
    UserINI.Load(sPath, "AxlPara"  , "bNLimActLevel " , AxlPara.bNLimActLevel );
    UserINI.Load(sPath, "AxlPara"  , "bAlarmActLevel" , AxlPara.bAlarmActLevel);
    UserINI.Load(sPath, "AxlPara"  , "bInposActLevel" , AxlPara.bInposActLevel);

    ApplyAxlPara ();
}

void CAxlQI10::UpdateAxlPara(bool _bToTable)
{
     if(_bToTable) {
         if(AxlParaUi.edPhysicalNo    ) AxlParaUi.edPhysicalNo    -> Text      = AxlPara.iPhysicalNo    ;
         if(AxlParaUi.cbUseSCurve     ) AxlParaUi.cbUseSCurve     -> Checked   = AxlPara.bUseSCurve     ;
         if(AxlParaUi.cbEncReverse    ) AxlParaUi.cbEncReverse    -> Checked   = AxlPara.bEncReverse    ;
         if(AxlParaUi.cbEncInput      ) AxlParaUi.cbEncInput      -> ItemIndex = AxlPara.iEncInput      ;
         if(AxlParaUi.cbPulseOutput   ) AxlParaUi.cbPulseOutput   -> ItemIndex = AxlPara.iPulseOutput   ;
         if(AxlParaUi.edSCurveAcPer   ) AxlParaUi.edSCurveAcPer   -> Text      = AxlPara.dSCurveAcPer   ;
         if(AxlParaUi.edSCurveDcPer   ) AxlParaUi.edSCurveDcPer   -> Text      = AxlPara.dSCurveDcPer   ;
         if(AxlParaUi.cbHomeActLevel  ) AxlParaUi.cbHomeActLevel  -> Checked   = AxlPara.bHomeActLevel  ;
         if(AxlParaUi.cbPLimActLevel  ) AxlParaUi.cbPLimActLevel  -> Checked   = AxlPara.bPLimActLevel  ;
         if(AxlParaUi.cbNLimActLevel  ) AxlParaUi.cbNLimActLevel  -> Checked   = AxlPara.bNLimActLevel  ;
         if(AxlParaUi.cbAlarmActLevel ) AxlParaUi.cbAlarmActLevel -> Checked   = AxlPara.bAlarmActLevel ;
         if(AxlParaUi.cbInposActLevel ) AxlParaUi.cbInposActLevel -> Checked   = AxlPara.bInposActLevel ;
     }

     else {
         if(AxlParaUi.edPhysicalNo    ) AxlPara.iPhysicalNo    = AxlParaUi.edPhysicalNo    -> Text.ToInt()    ;
         if(AxlParaUi.cbUseSCurve     ) AxlPara.bUseSCurve     = AxlParaUi.cbUseSCurve     -> Checked         ;
         if(AxlParaUi.cbEncReverse    ) AxlPara.bEncReverse    = AxlParaUi.cbEncReverse    -> Checked         ;
         if(AxlParaUi.cbEncInput      ) AxlPara.iEncInput      = AxlParaUi.cbEncInput      -> ItemIndex       ;
         if(AxlParaUi.cbPulseOutput   ) AxlPara.iPulseOutput   = AxlParaUi.cbPulseOutput   -> ItemIndex       ;
         if(AxlParaUi.edSCurveAcPer   ) AxlPara.dSCurveAcPer   = AxlParaUi.edSCurveAcPer   -> Text.ToDouble() ;
         if(AxlParaUi.edSCurveDcPer   ) AxlPara.dSCurveDcPer   = AxlParaUi.edSCurveDcPer   -> Text.ToDouble() ;
         if(AxlParaUi.cbHomeActLevel  ) AxlPara.bHomeActLevel  = AxlParaUi.cbHomeActLevel  -> Checked         ;
         if(AxlParaUi.cbPLimActLevel  ) AxlPara.bPLimActLevel  = AxlParaUi.cbPLimActLevel  -> Checked         ;
         if(AxlParaUi.cbNLimActLevel  ) AxlPara.bNLimActLevel  = AxlParaUi.cbNLimActLevel  -> Checked         ;
         if(AxlParaUi.cbAlarmActLevel ) AxlPara.bAlarmActLevel = AxlParaUi.cbAlarmActLevel -> Checked         ;
         if(AxlParaUi.cbInposActLevel ) AxlPara.bInposActLevel = AxlParaUi.cbInposActLevel -> Checked         ;
     }
}

void __stdcall TrgCallBack(INT16 nBoardNo, INT16 nModulePos, UINT8 byFlag)
{
    static int iInterruptCnt = 0 ;

    if(byFlag & 0x10000000) iInterruptCnt++;

    Trace("Triger" , AnsiString(iInterruptCnt).c_str());

    if(iInterruptCnt >10000) iInterruptCnt = 0 ;
}

void CAxlQI10::SetTrgPos(int _iPosCnt , double * _dPos , double _dTrgTime , bool _bActual ,bool _bOnLevel)
{
    if(!bInitOk) return ;

    return ;

    /*  ������.
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
    */
}

void CAxlQI10::ResetTrgPos()
{
    if(!bInitOk) return ;
    //CFS20Trig_Profile_Reset(m_iAxisNo);
}

void CAxlQI10::OneShotTrg(bool _bUpEg , int _iTime)
{
    if(!bInitOk) return ;
    //CFS20Trig_One_Shot_Set(m_iAxisNo, _bUpEg ? TRIG_UPEG : TRIG_DNEG , _iTime);

}

#endif// AJIN_AXL
