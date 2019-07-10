//---------------------------------------------------------------------------
#pragma hdrstop
#include "Axt_FS20.h"

//System header
#include <mem.h>
#include <math.h>

//Ajin Lib
#include "AxtLIB.h"
#include "AxtCAMCFS20.h"

//User Header
#include "Timer.h"
#include "UserIni.h"
#include "SLogUnit.h"
#include "FormMsgOk.h"

#pragma package(smart_init)

#pragma comment(lib, "AxtLib.lib")            // Ajinextek Library

//AXT Trigger
#pragma comment(lib, "AxtTriggerLib.lib")     // Ajinextek Trigger Library
#include "AXTKeLIB.h"             // Base Board�� 1�� Low Level �Լ��� ����ϱ� ����
#include "AxtKeCamcFs20.h"        // FS20�� 1�� Low Level �Լ��� ����ϱ� ����
#include "AxtTrigger.h"           // Tirgger Function ����� ���� �������

#define     TRIG_DNEG	0       // Trigger Doun Edge
#define     TRIG_UPEG	1       // Trigger Up Edge

SAxtParaUi AxtParaUi ;

bool bInitOk ;

void MakeAxtUI()
{
    static bool bFirst = true ;
    if(!bFirst) return ;
    bFirst = false ;

    AxtParaUi.pnBase = new TPanel(Application);

    AxtParaUi.edPhysicalNo   = new TEdit    (AxtParaUi.pnBase); AxtParaUi.edPhysicalNo   -> Parent = AxtParaUi.pnBase ;                                                               SetAxtArrange(AxtParaUi.edPhysicalNo   , 1 , 0 );
    AxtParaUi.lbPhysicalNo   = new TLabel(AxtParaUi.pnBase);    AxtParaUi.lbPhysicalNo   -> Parent = AxtParaUi.pnBase ; AxtParaUi.lbPhysicalNo   -> Caption = "Physical No        " ; SetAxtArrange(AxtParaUi.lbPhysicalNo   , 0 , 0 );

    AxtParaUi.cbUseSCurve    = new TCheckBox(AxtParaUi.pnBase); AxtParaUi.cbUseSCurve    -> Parent = AxtParaUi.pnBase ; AxtParaUi.cbUseSCurve    -> Caption = "Use S Curve        " ; SetAxtArrange(AxtParaUi.cbUseSCurve    , 0 , 1 );

    AxtParaUi.cbEncReverse   = new TCheckBox(AxtParaUi.pnBase); AxtParaUi.cbEncReverse   -> Parent = AxtParaUi.pnBase ; AxtParaUi.cbEncReverse   -> Caption = "Enc Reverse        " ; SetAxtArrange(AxtParaUi.cbEncReverse   , 0 , 2 );

    AxtParaUi.cbEncInput     = new TComboBox(AxtParaUi.pnBase); AxtParaUi.cbEncInput     -> Parent = AxtParaUi.pnBase ;                                                               SetAxtArrange(AxtParaUi.cbEncInput     , 1 , 3 );
    AxtParaUi.lbEncInput     = new TLabel(AxtParaUi.pnBase);    AxtParaUi.lbEncInput     -> Parent = AxtParaUi.pnBase ; AxtParaUi.lbEncInput     -> Caption = "Enc Input          " ; SetAxtArrange(AxtParaUi.lbEncInput     , 0 , 3 );

    AxtParaUi.cbPulseOutput  = new TComboBox(AxtParaUi.pnBase); AxtParaUi.cbPulseOutput  -> Parent = AxtParaUi.pnBase ;                                                               SetAxtArrange(AxtParaUi.cbPulseOutput  , 1 , 4 );
    AxtParaUi.lbPulseOutput  = new TLabel(AxtParaUi.pnBase);    AxtParaUi.lbPulseOutput  -> Parent = AxtParaUi.pnBase ; AxtParaUi.lbPulseOutput  -> Caption = "Pulse Output Method" ; SetAxtArrange(AxtParaUi.lbPulseOutput  , 0 , 4 );

    AxtParaUi.edSCurveAcPer  = new TEdit    (AxtParaUi.pnBase); AxtParaUi.edSCurveAcPer  -> Parent = AxtParaUi.pnBase ;                                                               SetAxtArrange(AxtParaUi.edSCurveAcPer  , 1 , 5 );
    AxtParaUi.lbSCurveAcPer  = new TLabel(AxtParaUi.pnBase);    AxtParaUi.lbSCurveAcPer  -> Parent = AxtParaUi.pnBase ; AxtParaUi.lbSCurveAcPer  -> Caption = "S Curve Acc %      " ; SetAxtArrange(AxtParaUi.lbSCurveAcPer  , 0 , 5 );

    AxtParaUi.edSCurveDcPer  = new TEdit    (AxtParaUi.pnBase); AxtParaUi.edSCurveDcPer  -> Parent = AxtParaUi.pnBase ;                                                               SetAxtArrange(AxtParaUi.edSCurveDcPer  , 1 , 6 );
    AxtParaUi.lbSCurveDcPer  = new TLabel(AxtParaUi.pnBase);    AxtParaUi.lbSCurveDcPer  -> Parent = AxtParaUi.pnBase ; AxtParaUi.lbSCurveDcPer  -> Caption = "S Curve Dcc %      " ; SetAxtArrange(AxtParaUi.lbSCurveDcPer  , 0 , 6 );

    AxtParaUi.cbHomeActLevel = new TCheckBox(AxtParaUi.pnBase); AxtParaUi.cbHomeActLevel -> Parent = AxtParaUi.pnBase ; AxtParaUi.cbHomeActLevel -> Caption = "Home Active High   " ; SetAxtArrange(AxtParaUi.cbHomeActLevel , 0 , 7 );

    AxtParaUi.cbPLimActLevel = new TCheckBox(AxtParaUi.pnBase); AxtParaUi.cbPLimActLevel -> Parent = AxtParaUi.pnBase ; AxtParaUi.cbPLimActLevel -> Caption = "+ Limit Active High" ; SetAxtArrange(AxtParaUi.cbPLimActLevel , 0 , 8 );

    AxtParaUi.cbNLimActLevel = new TCheckBox(AxtParaUi.pnBase); AxtParaUi.cbNLimActLevel -> Parent = AxtParaUi.pnBase ; AxtParaUi.cbNLimActLevel -> Caption = "- Limit Active High" ; SetAxtArrange(AxtParaUi.cbNLimActLevel , 0 , 9 );

    AxtParaUi.cbAlarmActLevel= new TCheckBox(AxtParaUi.pnBase); AxtParaUi.cbAlarmActLevel-> Parent = AxtParaUi.pnBase ; AxtParaUi.cbAlarmActLevel-> Caption = "Alarm Active High  " ; SetAxtArrange(AxtParaUi.cbAlarmActLevel, 0 , 10);

    AxtParaUi.cbInposActLevel= new TCheckBox(AxtParaUi.pnBase); AxtParaUi.cbInposActLevel-> Parent = AxtParaUi.pnBase ; AxtParaUi.cbInposActLevel-> Caption = "Inpos Active High  " ; SetAxtArrange(AxtParaUi.cbInposActLevel, 0 , 11);


//    AxtParaUi.edSCurveDcPer -> Left = 1 ;
//    AxtParaUi.edSCurveDcPer -> Top  = 1 ;
/*
    AxtParaUi.cbEncInput    -> AddItem("Up/Down"         ,NULL);
    AxtParaUi.cbEncInput    -> AddItem("1 multiplication",NULL);
    AxtParaUi.cbEncInput    -> AddItem("2 multiplication",NULL);
    AxtParaUi.cbEncInput    -> AddItem("4 multiplication",NULL); //���� 4ü��.

    AxtParaUi.cbPulseOutput -> AddItem("OneHighLowHigh"  ,NULL); // 0x0, 1�޽� ���, PULSE(Active High), ������(DIR=Low)  / ������(DIR=High)
    AxtParaUi.cbPulseOutput -> AddItem("OneHighHighLow"  ,NULL); // 0x1, 1�޽� ���, PULSE(Active High), ������(DIR=High) / ������(DIR=Low)
    AxtParaUi.cbPulseOutput -> AddItem("OneLowLowHigh "  ,NULL); // 0x2, 1�޽� ���, PULSE(Active Low),  ������(DIR=Low)  / ������(DIR=High)
    AxtParaUi.cbPulseOutput -> AddItem("OneLowHighLow "  ,NULL); // 0x3, 1�޽� ���, PULSE(Active Low),  ������(DIR=High) / ������(DIR=Low)
    AxtParaUi.cbPulseOutput -> AddItem("TwoCcwCwHigh  "  ,NULL); // 0x4, 2�޽� ���, PULSE(CCW:������),  DIR(CW:������),  Active High
    AxtParaUi.cbPulseOutput -> AddItem("TwoCcwCwLow   "  ,NULL); // 0x5, 2�޽� ���, PULSE(CCW:������),  DIR(CW:������),  Active Low
    AxtParaUi.cbPulseOutput -> AddItem("TwoCwCcwHigh  "  ,NULL); // 0x6, 2�޽� ���, PULSE(CW:������),   DIR(CCW:������), Active High
    AxtParaUi.cbPulseOutput -> AddItem("TwoCwCcwLow   "  ,NULL); // 0x7, 2�޽� ���, PULSE(CW:������),   DIR(CCW:������), Active Low
*/
}

void SetAxtUIParent (TPanel * _pnBase)
{
//  AxtParaUi.pnBase -> Parent = _pnBase ;
//  AxtParaUi.pnBase -> ParentWindow = _pnse ;
//  Parent = dynamic_cast<TWinControl *>(Owner);
//    AxtParaUi.pnBase = new TPanel(_pnBase);
//    AxtParaUi.pnBase -> Parent = _pnBase ;

    AxtParaUi.pnBase -> Parent = _pnBase  ;

    AxtParaUi.pnBase -> Align  = alClient ;

//    AxtParaUi.pnBase -> ParentFont = true ;

    AxtParaUi.cbEncInput    -> AddItem("Up/Down"         ,NULL);
    AxtParaUi.cbEncInput    -> AddItem("1 multiplication",NULL);
    AxtParaUi.cbEncInput    -> AddItem("2 multiplication",NULL);
    AxtParaUi.cbEncInput    -> AddItem("4 multiplication",NULL); //���� 4ü��.

    AxtParaUi.cbPulseOutput -> AddItem("OneHighLowHigh"  ,NULL); // 0x0, 1�޽� ���, PULSE(Active High), ������(DIR=Low)  / ������(DIR=High)
    AxtParaUi.cbPulseOutput -> AddItem("OneHighHighLow"  ,NULL); // 0x1, 1�޽� ���, PULSE(Active High), ������(DIR=High) / ������(DIR=Low)
    AxtParaUi.cbPulseOutput -> AddItem("OneLowLowHigh "  ,NULL); // 0x2, 1�޽� ���, PULSE(Active Low),  ������(DIR=Low)  / ������(DIR=High)
    AxtParaUi.cbPulseOutput -> AddItem("OneLowHighLow "  ,NULL); // 0x3, 1�޽� ���, PULSE(Active Low),  ������(DIR=High) / ������(DIR=Low)
    AxtParaUi.cbPulseOutput -> AddItem("TwoCcwCwHigh  "  ,NULL); // 0x4, 2�޽� ���, PULSE(CCW:������),  DIR(CW:������),  Active High
    AxtParaUi.cbPulseOutput -> AddItem("TwoCcwCwLow   "  ,NULL); // 0x5, 2�޽� ���, PULSE(CCW:������),  DIR(CW:������),  Active Low
    AxtParaUi.cbPulseOutput -> AddItem("TwoCwCcwHigh  "  ,NULL); // 0x6, 2�޽� ���, PULSE(CW:������),   DIR(CCW:������), Active High
    AxtParaUi.cbPulseOutput -> AddItem("TwoCwCcwLow   "  ,NULL); // 0x7, 2�޽� ���, PULSE(CW:������),   DIR(CCW:������), Active Low

}

const int iControlWidth   = 130;//165 ;
const int iControlHeight  = 17 ;//25  ;
const int iStartRowOffset = 10 ;//15  ;
const int iStartColOffset = 15 ;//20  ;

void SetAxtArrange (TControl * _tcControl , int _iCol , int _iRow)
{
    _tcControl -> Left = _iCol * iControlWidth  * 1.1 + _iCol + iStartColOffset ; _tcControl -> Width = iControlWidth  ;
    _tcControl -> Top  = _iRow * iControlHeight * 1.1 + _iRow + iStartRowOffset ; _tcControl -> Height= iControlHeight ;
}

void DeleteAxtUI()
{
    if(AxtParaUi.pnBase) {
        delete AxtParaUi.pnBase ; //������Ʈ�� �����ֳ� Parent�����ÿ� ��� ���� ��.
        AxtParaUi.pnBase = NULL ;
    }
}


int GetAxtFS20AxisCnt()
{
    return CFS20get_total_numof_axis();
}

bool InitAxtFS20Board()
{
    bInitOk = true ;

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

    return true ;
}

void CloseAxtFS20Board()
{
    AxtCloseDeviceAll();
    AxtClose();
}

//---CAxtFS20 Unit-------------------------------------------------------------
CAxtFS20::CAxtFS20(int _iAxisNo)
{
    InitLib();
    MakeAxtUI();

    memset(&AxtPara , 0 , sizeof(SAxtPara));

    m_iAxisNo = _iAxisNo ;

    Load();
}

CAxtFS20::~CAxtFS20()
{
//    DeleteAxtUI();
    CloseAxtFS20Board();
}


// ���� ���̺귯�� �� ��� �ʱ�ȭ
bool CAxtFS20::InitLib()
{
    static bool bFirst = true ;
    if(!bFirst) return true ;
    bFirst = false ;

    InitAxtFS20Board();
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

bool CAxtFS20::GetStop()
{
    return CFS20motion_done(AxtPara.iPhysicalNo) ;
}

void CAxtFS20::ApplyAxtPara ()
{
    const int iStart_Stop_speed = 10 ; //����������� ������ ó�� ġ�� ������ �ӵ�. �̺��� ������ ������ �ȵ�.
    CFS20set_startstop_speed   (AxtPara.iPhysicalNo, iStart_Stop_speed     ); // �ʱ�ӵ�(Start/Stop speed) ����
    CFS20set_end_limit_enable  (AxtPara.iPhysicalNo, false                 ); // ������ ����Ʈ ��� ��������� ���� ���ܿ��� �ذ� ��.
    CFS20set_alarm_enable      (AxtPara.iPhysicalNo, false                 );

    CFS20set_enc_reverse       (AxtPara.iPhysicalNo, AxtPara.bEncReverse   ); // ���ڴ� ���� ��ȯ.
    CFS20set_enc_input_method  (AxtPara.iPhysicalNo, AxtPara.iEncInput     ); // ���ڴ� �Է� ��� ����
    CFS20set_pulse_out_method  (AxtPara.iPhysicalNo, AxtPara.iPulseOutput  ); // �޽� ��� ��� ����
    CFS20set_s_rate            (AxtPara.iPhysicalNo, AxtPara.dSCurveAcPer ,
                                                     AxtPara.dSCurveDcPer  ); // S Curve ����.

    CFS20set_pend_limit_level  (AxtPara.iPhysicalNo, AxtPara.bPLimActLevel ); // ������ ����Ʈ(+End limit)�� ��Ƽ�극�� ����
    CFS20set_nend_limit_level  (AxtPara.iPhysicalNo, AxtPara.bNLimActLevel ); // ������ ����Ʈ(-End limit)�� ��Ƽ�극�� ����
    CFS20set_alarm_level       (AxtPara.iPhysicalNo, AxtPara.bAlarmActLevel); // �˶�(Alarm) ��ȣ ��Ƽ�극�� ����
    CFS20set_inposition_level  (AxtPara.iPhysicalNo, AxtPara.bInposActLevel); // ��������(Inposition) ��ȣ ��Ƽ�극�� ����

    CFS20set_max_speed         (AxtPara.iPhysicalNo, 4000 * 1000 ); // �ִ� ���� �ӵ��� ����  4000mm/sec * Unit/Pulse

    //������ �Ⱦ��ų� ���ܿ��� �ذ� ��.
//  CFS20set_moveunit_perpulse (AxtPara.iPhysicalNo, UNIT_PER_PULSE        ); // Unit�� �̵������� ����. �̵��� �� �ӵ��� ������ �ش� �Ⱦ�.

//  CFS20set_end_limit_enable  (AxtPara.iPhysicalNo, AxtPara.bEndLimEnable ); // ������ ����Ʈ ��� ��������� ����
//  CFS20set_alarm_enable      (AxtPara.iPhysicalNo, AxtPara.bAlarmEnable  ); // ���� �˶� �Է½�ȣ ����� ��������� ����
//  CFS20set_inposition_enable (AxtPara.iPhysicalNo, AxtPara.bInposEnable  ); // ��������(Inposition) ��ȣ ������� ����
}

void CAxtFS20::SetServo(bool _bOn)
{
    //Stop the Motor.
    Stop();

    //Servo On/Off.
    if (_bOn) CFS20set_output_bit  (AxtPara.iPhysicalNo , 0);
    else      CFS20reset_output_bit(AxtPara.iPhysicalNo , 0);

}

bool CAxtFS20::GetServo()
{
    return CFS20get_servo_enable(AxtPara.iPhysicalNo) ;
}

void CAxtFS20::SetReset(bool _bOn)
{
    if (_bOn) CFS20set_output_bit  (AxtPara.iPhysicalNo , 1);
    else      CFS20reset_output_bit(AxtPara.iPhysicalNo , 1);
}

double CAxtFS20::GetCmdPos()
{
    return CFS20get_command_position(AxtPara.iPhysicalNo);
}

double CAxtFS20::GetEncPos()
{
    return CFS20get_actual_position (AxtPara.iPhysicalNo); //Servo , Easy Servo - Actual Position.
}

void CAxtFS20::SetPos (double _dPos )
{
    CFS20set_command_position(AxtPara.iPhysicalNo , _dPos); //Commnad Position.
    CFS20set_actual_position (AxtPara.iPhysicalNo , _dPos); //Actual Position.
}

bool CAxtFS20::GetHomeSnsr()
{
    //Home Sensor
    bool bRet ;

    bRet = ((CFS20get_input(AxtPara.iPhysicalNo) >> 0 ) & 0x01) ;
    bRet = AxtPara.bHomeActLevel ?  bRet : !bRet ;

    return bRet ;
}

bool CAxtFS20::GetNLimSnsr()
{
    //Get Mechanical Signal.
    bool bRet ;

    bRet = (CFS20get_mechanical_signal(AxtPara.iPhysicalNo) >> 1) & 0x01;

    return bRet ;
}

bool CAxtFS20::GetPLimSnsr()
{
    //Get Mechanical Signal.
    bool bRet ;

    bRet = (CFS20get_mechanical_signal(AxtPara.iPhysicalNo) >> 0) & 0x01;

    return bRet ;
}

bool CAxtFS20::GetZphaseSnsr()
{
    //Get ZPhase Signal.
    bool bRet ;

    bRet = ((CFS20get_input(AxtPara.iPhysicalNo) >> 1 ) & 0x01) ;
    bRet = !bRet ;//ra.bZphaActLevel ?  bRet : !bRet ;

    //if(bRet)MsgOk("ZZZZZ","GeeGeeGeeGee Baby Baby");

    return bRet ;
}

bool CAxtFS20::GetPackInPos()
{
    //Get Mechanical Signal.
    bool bRet ;

    bRet = (CFS20get_mechanical_signal(AxtPara.iPhysicalNo) >> 5) & 0x01;

    return bRet ;
}

bool CAxtFS20::GetAlarm()
{
    bool bRet ;

    bRet = (CFS20get_mechanical_signal(AxtPara.iPhysicalNo) >> 4) & 0x01;

    return bRet ;
}

bool CAxtFS20::GetHomeDone()
{
    bool  bRet ;

    bRet = CFS20get_home_done(AxtPara.iPhysicalNo) ;

    return bRet ;
}

bool CAxtFS20::Stop()
{
    //Stop.
    CFS20abort_home_search(AxtPara.iPhysicalNo , 0);
    
    return CFS20set_stop(AxtPara.iPhysicalNo) ;
}

bool CAxtFS20::EmgStop()
{
    //Stop Home Proc.
    CFS20abort_home_search(AxtPara.iPhysicalNo , 0);

    //Stop.
    return CFS20set_e_stop(AxtPara.iPhysicalNo) ;
}

bool CAxtFS20::JogP(double _dVel , double _dAcc)
{
    return CFS20v_move_ex(AxtPara.iPhysicalNo , _dVel , _dAcc);
}

bool CAxtFS20::JogN(double _dVel , double _dAcc)
{
    return CFS20v_move_ex(AxtPara.iPhysicalNo , -1 * _dVel , _dAcc);
}

bool CAxtFS20::GoAbs(double _dPos , double _dVel , double _dAcc)
{
    //Go Motor.
    if (GetStop()) {
        if(AxtPara.bUseSCurve) CFS20start_s_move_ex(AxtPara.iPhysicalNo , _dPos , _dVel , _dAcc );
        else                   CFS20start_move_ex  (AxtPara.iPhysicalNo , _dPos , _dVel , _dAcc );
    }

    //Check InPos.
    bool bRet = GetStop();
    return bRet ;
}

bool CAxtFS20::GoOvr(double _dPos)
{
    //Go Motor.
    CFS20position_override(AxtPara.iPhysicalNo , _dPos ); //���� ���������� ���� ���� ���۰��� �ڵ������� ����.

    //Check InPos.
    bool bRet = GetStop();
    return bRet ;
}

bool CAxtFS20::FindEdgeStop (double _dVel ,double _dAcc ,EN_FINDEDGE_TYPE _ftFindEdgeType)
{
    EN_FINDEDGE_TYPE ftFindEdgeType ;

    if(_ftFindEdgeType == ftOrgUpEdge || _ftFindEdgeType == ftOrgDnEdge) {
        if(AxtPara.bHomeActLevel) {
            ftFindEdgeType = _ftFindEdgeType ;
        }
        else {
            ftFindEdgeType = (_ftFindEdgeType == ftOrgUpEdge) ? ftOrgDnEdge : ftOrgUpEdge ;
        }
    }
    else {
        ftFindEdgeType = _ftFindEdgeType ;
    }
    return CFS20start_signal_search1_ex(AxtPara.iPhysicalNo, _dVel , _dAcc , ftFindEdgeType);
}

bool CAxtFS20::FindEdgeEmgStop(double _dVel ,EN_FINDEDGE_TYPE _ftFindEdgeType)
{
    EN_FINDEDGE_TYPE ftFindEdgeType ;

    if(_ftFindEdgeType == ftOrgUpEdge || _ftFindEdgeType == ftOrgDnEdge) {
        if(AxtPara.bHomeActLevel) {
            ftFindEdgeType = _ftFindEdgeType ;
        }
        else {
            ftFindEdgeType = (_ftFindEdgeType == ftOrgUpEdge) ? ftOrgDnEdge : ftOrgUpEdge ;
        }
    }
    else {
        ftFindEdgeType = _ftFindEdgeType ;
    }

    return CFS20start_signal_search2(AxtPara.iPhysicalNo, _dVel , ftFindEdgeType);
}

void CAxtFS20::SetAxtPara (SAxtPara &_AxtPara ){memcpy(& AxtPara , &_AxtPara , sizeof(SAxtPara ));}
void CAxtFS20::GetAxtPara (SAxtPara &_AxtPara ){memcpy(&_AxtPara , & AxtPara , sizeof(SAxtPara ));}


void CAxtFS20::Save()
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
    UserINI.Save(sPath, "AxtPara"  , "iPhysicalNo   " , AxtPara.iPhysicalNo   );
    UserINI.Save(sPath, "AxtPara"  , "bUseSCurve    " , AxtPara.bUseSCurve    );
    UserINI.Save(sPath, "AxtPara"  , "bEncReverse   " , AxtPara.bEncReverse   );
    UserINI.Save(sPath, "AxtPara"  , "iEncInput     " , AxtPara.iEncInput     );
    UserINI.Save(sPath, "AxtPara"  , "iPulseOutput  " , AxtPara.iPulseOutput  );
    UserINI.Save(sPath, "AxtPara"  , "dSCurveAcPer  " , AxtPara.dSCurveAcPer  );
    UserINI.Save(sPath, "AxtPara"  , "dSCurveDcPer  " , AxtPara.dSCurveDcPer  );
    UserINI.Save(sPath, "AxtPara"  , "bHomeActLevel " , AxtPara.bHomeActLevel );
    UserINI.Save(sPath, "AxtPara"  , "bPLimActLevel " , AxtPara.bPLimActLevel );
    UserINI.Save(sPath, "AxtPara"  , "bNLimActLevel " , AxtPara.bNLimActLevel );
    UserINI.Save(sPath, "AxtPara"  , "bAlarmActLevel" , AxtPara.bAlarmActLevel);
    UserINI.Save(sPath, "AxtPara"  , "bInposActLevel" , AxtPara.bInposActLevel);

    ApplyAxtPara ();

}
void CAxtFS20::Load()
{
    //Local Var.
    TUserINI   UserINI ;
    AnsiString sAxisNo ;
    AnsiString sPath   ;

    //Set Dir.
    sAxisNo = (AnsiString)m_iAxisNo ;
    sPath   = EXE_FOLDER + "Util\\Motor_" + sAxisNo + "_Para.INI" ;

    //Save Device.
    UserINI.Load(sPath, "AxtPara"  , "iPhysicalNo   " , AxtPara.iPhysicalNo   );
    UserINI.Load(sPath, "AxtPara"  , "bUseSCurve    " , AxtPara.bUseSCurve    );
    UserINI.Load(sPath, "AxtPara"  , "bEncReverse   " , AxtPara.bEncReverse   );
    UserINI.Load(sPath, "AxtPara"  , "iEncInput     " , AxtPara.iEncInput     );
    UserINI.Load(sPath, "AxtPara"  , "iPulseOutput  " , AxtPara.iPulseOutput  );
    UserINI.Load(sPath, "AxtPara"  , "dSCurveAcPer  " , AxtPara.dSCurveAcPer  );
    UserINI.Load(sPath, "AxtPara"  , "dSCurveDcPer  " , AxtPara.dSCurveDcPer  );
    UserINI.Load(sPath, "AxtPara"  , "bHomeActLevel " , AxtPara.bHomeActLevel );
    UserINI.Load(sPath, "AxtPara"  , "bPLimActLevel " , AxtPara.bPLimActLevel );
    UserINI.Load(sPath, "AxtPara"  , "bNLimActLevel " , AxtPara.bNLimActLevel );
    UserINI.Load(sPath, "AxtPara"  , "bAlarmActLevel" , AxtPara.bAlarmActLevel);
    UserINI.Load(sPath, "AxtPara"  , "bInposActLevel" , AxtPara.bInposActLevel);

    ApplyAxtPara ();
}

void CAxtFS20::UpdateAxtPara(bool _bToTable)
{
     if(_bToTable) {
         if(AxtParaUi.edPhysicalNo    ) AxtParaUi.edPhysicalNo    -> Text      = AxtPara.iPhysicalNo    ;
         if(AxtParaUi.cbUseSCurve     ) AxtParaUi.cbUseSCurve     -> Checked   = AxtPara.bUseSCurve     ;
         if(AxtParaUi.cbEncReverse    ) AxtParaUi.cbEncReverse    -> Checked   = AxtPara.bEncReverse    ;
         if(AxtParaUi.cbEncInput      ) AxtParaUi.cbEncInput      -> ItemIndex = AxtPara.iEncInput      ;
         if(AxtParaUi.cbPulseOutput   ) AxtParaUi.cbPulseOutput   -> ItemIndex = AxtPara.iPulseOutput   ;
         if(AxtParaUi.edSCurveAcPer   ) AxtParaUi.edSCurveAcPer   -> Text      = AxtPara.dSCurveAcPer   ;
         if(AxtParaUi.edSCurveDcPer   ) AxtParaUi.edSCurveDcPer   -> Text      = AxtPara.dSCurveDcPer   ;
         if(AxtParaUi.cbHomeActLevel  ) AxtParaUi.cbHomeActLevel  -> Checked   = AxtPara.bHomeActLevel  ;
         if(AxtParaUi.cbPLimActLevel  ) AxtParaUi.cbPLimActLevel  -> Checked   = AxtPara.bPLimActLevel  ;
         if(AxtParaUi.cbNLimActLevel  ) AxtParaUi.cbNLimActLevel  -> Checked   = AxtPara.bNLimActLevel  ;
         if(AxtParaUi.cbAlarmActLevel ) AxtParaUi.cbAlarmActLevel -> Checked   = AxtPara.bAlarmActLevel ;
         if(AxtParaUi.cbInposActLevel ) AxtParaUi.cbInposActLevel -> Checked   = AxtPara.bInposActLevel ;
     }

     else {
         if(AxtParaUi.edPhysicalNo    ) AxtPara.iPhysicalNo    = AxtParaUi.edPhysicalNo    -> Text.ToInt()    ;
         if(AxtParaUi.cbUseSCurve     ) AxtPara.bUseSCurve     = AxtParaUi.cbUseSCurve     -> Checked         ;
         if(AxtParaUi.cbEncReverse    ) AxtPara.bEncReverse    = AxtParaUi.cbEncReverse    -> Checked         ;
         if(AxtParaUi.cbEncInput      ) AxtPara.iEncInput      = AxtParaUi.cbEncInput      -> ItemIndex       ;
         if(AxtParaUi.cbPulseOutput   ) AxtPara.iPulseOutput   = AxtParaUi.cbPulseOutput   -> ItemIndex       ;
         if(AxtParaUi.edSCurveAcPer   ) AxtPara.dSCurveAcPer   = AxtParaUi.edSCurveAcPer   -> Text.ToDouble() ;
         if(AxtParaUi.edSCurveDcPer   ) AxtPara.dSCurveDcPer   = AxtParaUi.edSCurveDcPer   -> Text.ToDouble() ;
         if(AxtParaUi.cbHomeActLevel  ) AxtPara.bHomeActLevel  = AxtParaUi.cbHomeActLevel  -> Checked         ;
         if(AxtParaUi.cbPLimActLevel  ) AxtPara.bPLimActLevel  = AxtParaUi.cbPLimActLevel  -> Checked         ;
         if(AxtParaUi.cbNLimActLevel  ) AxtPara.bNLimActLevel  = AxtParaUi.cbNLimActLevel  -> Checked         ;
         if(AxtParaUi.cbAlarmActLevel ) AxtPara.bAlarmActLevel = AxtParaUi.cbAlarmActLevel -> Checked         ;
         if(AxtParaUi.cbInposActLevel ) AxtPara.bInposActLevel = AxtParaUi.cbInposActLevel -> Checked         ;
     }
}

void __stdcall TrgCallBack(INT16 nBoardNo, INT16 nModulePos, UINT8 byFlag)
{
    static int iInterruptCnt = 0 ;

    if(byFlag & 0x10000000) iInterruptCnt++;

    Trace("Triger" , AnsiString(iInterruptCnt).c_str());

    if(iInterruptCnt >10000) iInterruptCnt = 0 ;
}

void CAxtFS20::SetTrgPos(int _iPosCnt , double * _dPos , double _dTrgTime , bool _bActual ,bool _bOnLevel)
{
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

}

void CAxtFS20::ResetTrgPos()
{
    if(!bInitOk) return ;
    CFS20Trig_Profile_Reset(m_iAxisNo);
}

void CAxtFS20::OneShotTrg(bool _bUpEg , int _iTime)
{
    if(!bInitOk) return ;
    CFS20Trig_One_Shot_Set(m_iAxisNo, _bUpEg ? TRIG_UPEG : TRIG_DNEG , _iTime);

}
