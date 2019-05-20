/****************************************************************************
*****************************************************************************
**
** File Name
** ---------
**
** AXM.H
**
** COPYRIGHT (c) AJINEXTEK Co., LTD
**
*****************************************************************************
*****************************************************************************
**
** Description
** -----------
** Ajinextek Motion Library Header File
** 
**
*****************************************************************************
*****************************************************************************
**
** Source Change Indices
** ----------------------
** 
** (None)
**
**
*****************************************************************************
*****************************************************************************
**
** Website
** ---------------------
**
** http://www.ajinextek.com
**
*****************************************************************************
*****************************************************************************
*/

#ifndef __AXT_AXM_H__
#define __AXT_AXM_H__

#include "AXHS.h"

#ifdef __cplusplus
extern "C" {
#endif    //__cplusplus

//========== ���� �� ��� Ȯ���Լ�(Info) - Infomation ===============================================================
    // �ش� ���� �����ȣ, ��� ��ġ, ��� ���̵� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmInfoGetAxis(long lAxisNo, long *lpBoardNo, long *lpModulePos, DWORD *upModuleID);
    // ��� ����� �����ϴ��� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmInfoIsMotionModule(DWORD *upStatus);
    // �ش� ���� ��ȿ���� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmInfoIsInvalidAxisNo(long lAxisNo);
    // �ش� ���� ��� ������ �������� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmInfoGetAxisStatus(long lAxisNo);
    // �ý��۳� ��ȿ�� ��� ����� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmInfoGetAxisCount(long *lpAxisCount);
    // �ش� ����/����� ù��° ���ȣ�� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmInfoGetFirstAxisNo(long lBoardNo, long lModulePos, long *lpAxisNo);
    
//========= ���� �� �Լ� ============================================================================================
    // �ʱ� ���¿��� AXM ��� �Լ��� ���ȣ ������ 0 ~ (���� �ý��ۿ� ������ ��� - 1) �������� ��ȿ������
    // �� �Լ��� ����Ͽ� ���� ������ ���ȣ ��� ������ ���ȣ�� �ٲ� �� �ִ�.
    // �� �Լ��� ���� �ý����� H/W ������� �߻��� ���� ���α׷��� �Ҵ�� ���ȣ�� �״�� �����ϰ� ���� ���� ���� 
    // �������� ��ġ�� �����Ͽ� ����� ���� ������� �Լ��̴�.
    // ���ǻ��� : ���� ���� ���� ���ȣ�� ���Ͽ� ���� ��ȣ�� ���� ���� �ߺ��ؼ� �������� ���ƾ� �Ѵ�.
    //            �ߺ� ���ε� ��� ���� ���ȣ�� ���� �ุ ���� ���ȣ�� ���� �� �� ������, 
    //            ������ ���� ������ ��ȣ�� ���ε� ���� ��� �Ұ����ϴ�. 

    // �������� �����Ѵ�.
    DWORD    __stdcall AxmVirtualSetAxisNoMap(long lRealAxisNo, long lVirtualAxisNo);
    // ������ ������ ��ȣ�� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmVirtualGetAxisNoMap(long lRealAxisNo, long *lpVirtualAxisNo);
    // ��Ƽ �������� �����Ѵ�.
    DWORD    __stdcall AxmVirtualSetMultiAxisNoMap(long lSize, long *lpRealAxesNo, long *lpVirtualAxesNo);
    // ������ ��Ƽ ������ ��ȣ�� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmVirtualGetMultiAxisNoMap(long lSize, long *lpRealAxesNo, long *lpVirtualAxesNo);
    // ������ ������ �����Ѵ�.
    DWORD    __stdcall AxmVirtualResetAxisMap();

//========= ���ͷ�Ʈ ���� �Լ� ======================================================================================
    // �ݹ� �Լ� ����� �̺�Ʈ �߻� ������ ��� �ݹ� �Լ��� ȣ�� ������ ���� ������ �̺�Ʈ�� �������� �� �ִ� ������ ������
    // �ݹ� �Լ��� ������ ���� �� ������ ���� ���μ����� ��ü�Ǿ� �ְ� �ȴ�.
    // ��, �ݹ� �Լ� ���� ���ϰ� �ɸ��� �۾��� ���� ��쿡�� ��뿡 ���Ǹ� ���Ѵ�. 
    // �̺�Ʈ ����� ��������� �̿��Ͽ� ���ͷ�Ʈ �߻����θ� ���������� �����ϰ� �ִٰ� ���ͷ�Ʈ�� �߻��ϸ� 
    // ó�����ִ� �������, ������ ������ ���� �ý��� �ڿ��� �����ϰ� �ִ� ������ ������
    // ���� ������ ���ͷ�Ʈ�� �����ϰ� ó������ �� �ִ� ������ �ִ�.
    // �Ϲ������δ� ���� ������ ������, ���ͷ�Ʈ�� ����ó���� �ֿ� ���ɻ��� ��쿡 ���ȴ�. 
    // �̺�Ʈ ����� �̺�Ʈ�� �߻� ���θ� �����ϴ� Ư�� �����带 ����Ͽ� ���� ���μ����� ������ ���۵ǹǷ�
    // MultiProcessor �ý��۵�� �ڿ��� ���� ȿ�������� ����� �� �ְ� �Ǿ� Ư�� �����ϴ� ����̴�.
    // ���ͷ�Ʈ �޽����� �޾ƿ��� ���Ͽ� ������ �޽��� �Ǵ� �ݹ� �Լ��� ����Ѵ�.
    // (�޽��� �ڵ�, �޽��� ID, �ݹ��Լ�, ���ͷ�Ʈ �̺�Ʈ)
    //    hWnd    : ������ �ڵ�, ������ �޼����� ������ ���. ������� ������ NULL�� �Է�.
    //    wMsg    : ������ �ڵ��� �޼���, ������� �ʰų� ����Ʈ���� ����Ϸ��� 0�� �Է�.
    //    proc    : ���ͷ�Ʈ �߻��� ȣ��� �Լ��� ������, ������� ������ NULL�� �Է�.
    //    pEvent  : �̺�Ʈ ������� �̺�Ʈ �ڵ�
    // Ex)
    // AxmInterruptSetAxis(0, Null, 0, AxtInterruptProc, NULL);
    // void __stdcall AxtInterruptProc(long lAxisNo, DWORD dwFlag){
    //     ... ;
    // }
    DWORD    __stdcall AxmInterruptSetAxis(long lAxisNo, HWND hWnd, DWORD uMessage, AXT_INTERRUPT_PROC pProc, HANDLE *pEvent);
    
    // ���� ���� ���ͷ�Ʈ ��� ���θ� �����Ѵ�
    // �ش� �࿡ ���ͷ�Ʈ ���� / Ȯ��
     // uUse : ��� ���� => DISABLE(0), ENABLE(1)
    DWORD    __stdcall AxmInterruptSetAxisEnable(long lAxisNo, DWORD uUse);
    // ���� ���� ���ͷ�Ʈ ��� ���θ� ��ȯ�Ѵ�
    DWORD    __stdcall AxmInterruptGetAxisEnable(long lAxisNo, DWORD *upUse);
    
    //���ͷ�Ʈ�� �̺�Ʈ ������� ����� ��� �ش� ���ͷ�Ʈ ���� �д´�.
    DWORD    __stdcall AxmInterruptRead(long *lpAxisNo, DWORD *upFlag);
    // �ش� ���� ���ͷ�Ʈ �÷��� ���� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmInterruptReadAxisFlag(long lAxisNo, long lBank, DWORD *upFlag);

    // ���� ���� ����ڰ� ������ ���ͷ�Ʈ �߻� ���θ� �����Ѵ�.
    // lBank         : ���ͷ�Ʈ ��ũ ��ȣ (0 - 1) ��������.
    // uInterruptNum : ���ͷ�Ʈ ��ȣ ���� ��Ʈ��ȣ�� ���� hex�� Ȥ�� define�Ȱ��� ����
    // AXHS.h���� IP, QI INTERRUPT_BANK1, 2 DEF�� Ȯ���Ѵ�.
    DWORD    __stdcall AxmInterruptSetUserEnable(long lAxisNo, long lBank, DWORD uInterruptNum);
    // ���� ���� ����ڰ� ������ ���ͷ�Ʈ �߻� ���θ� Ȯ���Ѵ�.
    DWORD    __stdcall AxmInterruptGetUserEnable(long lAxisNo, long lBank, DWORD *upInterruptNum);

//======== ��� �Ķ��Ÿ ���� =======================================================================================
    // AxmMotLoadParaAll�� ������ Load ��Ű�� ������ �ʱ� �Ķ��Ÿ ������ �⺻ �Ķ��Ÿ ����. 
    // ���� PC�� ���Ǵ� ����࿡ �Ȱ��� ����ȴ�. �⺻�Ķ��Ÿ�� �Ʒ��� ����. 
    // 00:AXIS_NO.             =0          01:PULSE_OUT_METHOD.    =4         02:ENC_INPUT_METHOD.    =3     03:INPOSITION.          =2
    // 04:ALARM.               =1          05:NEG_END_LIMIT.       =1         06:POS_END_LIMIT.       =1     07:MIN_VELOCITY.        =1
    // 08:MAX_VELOCITY.        =700000     09:HOME_SIGNAL.         =4         10:HOME_LEVEL.          =1     11:HOME_DIR.            =0
    // 12:ZPHASE_LEVEL.        =1          13:ZPHASE_USE.          =0         14:STOP_SIGNAL_MODE.    =0     15:STOP_SIGNAL_LEVEL.   =1
    // 16:HOME_FIRST_VELOCITY. =100        17:HOME_SECOND_VELOCITY.=100       18:HOME_THIRD_VELOCITY. =20    19:HOME_LAST_VELOCITY.  =1
    // 20:HOME_FIRST_ACCEL.    =400        21:HOME_SECOND_ACCEL.   =400       22:HOME_END_CLEAR_TIME. =1000  23:HOME_END_OFFSET.     =0
    // 24:NEG_SOFT_LIMIT.      =-134217728 25:POS_SOFT_LIMIT.      =134217727 26:MOVE_PULSE.          =1     27:MOVE_UNIT.           =1
    // 28:INIT_POSITION.       =1000       29:INIT_VELOCITY.       =200       30:INIT_ACCEL.          =400   31:INIT_DECEL.          =400
    // 32:INIT_ABSRELMODE.     =0          33:INIT_PROFILEMODE.    =4         34:SVON_LEVEL.          =1     35:ALARM_RESET_LEVEL.   =1
    // 36:ENCODER_TYPE.        =1          37:SOFT_LIMIT_SEL.      =0         38:SOFT_LIMIT_STOP_MODE.=0     39:SOFT_LIMIT_ENABLE.   =0

    // 00=[AXIS_NO             ]: �� (0�� ���� ������)
    // 01=[PULSE_OUT_METHOD    ]: Pulse out method TwocwccwHigh = 6
    // 02=[ENC_INPUT_METHOD    ]: disable = 0, 1ü�� = 1, 2ü�� = 2, 4ü�� = 3, �ἱ ���ù��� ��ü��(-).1ü�� = 11  2ü�� = 12  4ü�� = 13
    // 03=[INPOSITION          ], 04=[ALARM     ], 05,06 =[END_LIMIT   ]  : 0 = B����, 1= A����, 2 = ������, 3 = �������� ����
    // 07=[MIN_VELOCITY        ]: ���� �ӵ�(START VELOCITY)
    // 08=[MAX_VELOCITY        ]: ����̹��� ������ �޾Ƶ��ϼ� �ִ� ���� �ӵ�. ���� �Ϲ� Servo�� 700k
    // Ex> screw : 20mm pitch drive: 10000 pulse ����: 400w
    // 09=[HOME_SIGNAL         ]: 4 - Home in0 , 0 :PosEndLimit , 1 : NegEndLimit // _HOME_SIGNAL����.
    // 10=[HOME_LEVEL          ]: 0 = B����, 1 = A����, 2 = ������, 3 = �������� ����
    // 11=[HOME_DIR            ]: Ȩ ����(HOME DIRECTION) 1:+����, 0:-����
    // 12=[ZPHASE_LEVEL        ]: 0 = B����, 1 = B����, 2 = ������, 3 = �������� ����
    // 13=[ZPHASE_USE          ]: Z���뿩��. 0: ������ , 1: +����, 2: -���� 
    // 14=[STOP_SIGNAL_MODE    ]: ESTOP, SSTOP ���� ��� 0:��������, 1:������ 
    // 15=[STOP_SIGNAL_LEVEL   ]: ESTOP, SSTOP ��� ����.  0 = B����, 1 = A����, 2 = ������, 3 = �������� ���� 
    // 16=[HOME_FIRST_VELOCITY ]: 1�������ӵ� 
    // 17=[HOME_SECOND_VELOCITY]: �����ļӵ� 
    // 18=[HOME_THIRD_VELOCITY ]: ������ �ӵ� 
    // 19=[HOME_LAST_VELOCITY  ]: index�˻��� �����ϰ� �˻��ϱ����� �ӵ�. 
    // 20=[HOME_FIRST_ACCEL    ]: 1�� ���ӵ� , 21=[HOME_SECOND_ACCEL   ] : 2�� ���ӵ� 
    // 22=[HOME_END_CLEAR_TIME ]: ���� �˻� Enc �� Set�ϱ� ���� ���ð�,  23=[HOME_END_OFFSET] : ���������� Offset��ŭ �̵�.
    // 24=[NEG_SOFT_LIMIT      ]: - SoftWare Limit ���� �����ϸ� ������, 25=[POS_SOFT_LIMIT ]: + SoftWare Limit ���� �����ϸ� ������.
    // 26=[MOVE_PULSE          ]: ����̹��� 1ȸ���� �޽���              , 27=[MOVE_UNIT  ]: ����̹� 1ȸ���� �̵��� ��:��ũ�� Pitch
    // 28=[INIT_POSITION       ]: ������Ʈ ���� �ʱ���ġ  , ����ڰ� ���Ƿ� ��밡��
    // 29=[INIT_VELOCITY       ]: ������Ʈ ���� �ʱ�ӵ�  , ����ڰ� ���Ƿ� ��밡��
    // 30=[INIT_ACCEL          ]: ������Ʈ ���� �ʱⰡ�ӵ�, ����ڰ� ���Ƿ� ��밡��
    // 31=[INIT_DECEL          ]: ������Ʈ ���� �ʱⰨ�ӵ�, ����ڰ� ���Ƿ� ��밡��
    // 32=[INIT_ABSRELMODE     ]: ����(0)/���(1) ��ġ ����
    // 33=[INIT_PROFILEMODE    ]: �������ϸ��(0 - 4) ���� ����
    //                            '0': ��Ī Trapezode, '1': ���Ī Trapezode, '2': ��Ī Quasi-S Curve, '3':��Ī S Curve, '4':���Ī S Curve
    // 34=[SVON_LEVEL          ]: 0 = B����, 1 = A����
    // 35=[ALARM_RESET_LEVEL   ]: 0 = B����, 1 = A����
    // 36=[ENCODER_TYPE        ]: 0 = TYPE_INCREMENTAL, 1 = TYPE_ABSOLUTE
    // 37=[SOFT_LIMIT_SEL      ]: 0 = COMMAND, 1 = ACTUAL
    // 38=[SOFT_LIMIT_STOP_MODE]: 0 = EMERGENCY_STOP, 1 = SLOWDOWN_STOP
    // 39=[SOFT_LIMIT_ENABLE   ]: 0 = DISABLE, 1 = ENABLE
    
    // AxmMotSaveParaAll�� ���� �Ǿ��� .mot������ �ҷ��´�. �ش� ������ ����ڰ� Edit �Ͽ� ��� �����ϴ�.
    DWORD    __stdcall AxmMotLoadParaAll(char *szFilePath);
    // ����࿡ ���� ��� �Ķ��Ÿ�� �ະ�� �����Ѵ�. .mot���Ϸ� �����Ѵ�.
    DWORD    __stdcall AxmMotSaveParaAll(char *szFilePath);

    // �Ķ��Ÿ 28 - 31������ ����ڰ� ���α׷�������  �� �Լ��� �̿��� ���� �Ѵ�
    DWORD    __stdcall AxmMotSetParaLoad(long lAxisNo, double dInitPos, double dInitVel, double dInitAccel, double dInitDecel);    
    // �Ķ��Ÿ 28 - 31������ ����ڰ� ���α׷�������  �� �Լ��� �̿��� Ȯ�� �Ѵ�.
    DWORD    __stdcall AxmMotGetParaLoad(long lAxisNo, double *dpInitPos, double *dpInitVel, double *dpInitAccel, double *dpInitDecel);    

    // ���� ���� �޽� ��� ����� �����Ѵ�.
    //uMethod  0 :OneHighLowHigh, 1 :OneHighHighLow, 2 :OneLowLowHigh, 3 :OneLowHighLow, 4 :TwoCcwCwHigh
    //         5 :TwoCcwCwLow,    6 :TwoCwCcwHigh,   7 :TwoCwCcwLow,   8 :TwoPhase,      9 :TwoPhaseReverse
    //    OneHighLowHigh      = 0x0,           // 1�޽� ���, PULSE(Active High), ������(DIR=Low)  / ������(DIR=High)
    //    OneHighHighLow      = 0x1,           // 1�޽� ���, PULSE(Active High), ������(DIR=High) / ������(DIR=Low)
    //    OneLowLowHigh       = 0x2,           // 1�޽� ���, PULSE(Active Low),  ������(DIR=Low)  / ������(DIR=High)
    //    OneLowHighLow       = 0x3,           // 1�޽� ���, PULSE(Active Low),  ������(DIR=High) / ������(DIR=Low)
    //    TwoCcwCwHigh        = 0x4,           // 2�޽� ���, PULSE(CCW:������),  DIR(CW:������),  Active High     
    //    TwoCcwCwLow         = 0x5,           // 2�޽� ���, PULSE(CCW:������),  DIR(CW:������),  Active Low     
    //    TwoCwCcwHigh        = 0x6,           // 2�޽� ���, PULSE(CW:������),   DIR(CCW:������), Active High
    //    TwoCwCcwLow         = 0x7,           // 2�޽� ���, PULSE(CW:������),   DIR(CCW:������), Active Low
    //    TwoPhase            = 0x8,           // 2��(90' ������),  PULSE lead DIR(CW: ������), PULSE lag DIR(CCW:������)
    //    TwoPhaseReverse     = 0x9            // 2��(90' ������),  PULSE lead DIR(CCW: ������), PULSE lag DIR(CW:������)
    DWORD    __stdcall AxmMotSetPulseOutMethod(long lAxisNo, DWORD uMethod);
    // ���� ���� �޽� ��� ��� ������ ��ȯ�Ѵ�,
    DWORD    __stdcall AxmMotGetPulseOutMethod(long lAxisNo, DWORD *upMethod);

    // ���� ���� �ܺ�(Actual) ī��Ʈ�� ���� ���� ������ �����Ͽ� ���� ���� Encoder �Է� ����� �����Ѵ�.
    // uMethod : 0 - 7 ����.
    // ObverseUpDownMode    = 0x0,            // ������ Up/Down
    // ObverseSqr1Mode      = 0x1,            // ������ 1ü��
    // ObverseSqr2Mode      = 0x2,            // ������ 2ü��
    // ObverseSqr4Mode      = 0x3,            // ������ 4ü��
    // ReverseUpDownMode    = 0x4,            // ������ Up/Down
    // ReverseSqr1Mode      = 0x5,            // ������ 1ü��
    // ReverseSqr2Mode      = 0x6,            // ������ 2ü��
    // ReverseSqr4Mode      = 0x7             // ������ 4ü��
    DWORD    __stdcall AxmMotSetEncInputMethod(long lAxisNo, DWORD uMethod);
    // ���� ���� �ܺ�(Actual) ī��Ʈ�� ���� ���� ������ �����Ͽ� ���� ���� Encoder �Է� ����� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmMotGetEncInputMethod(long lAxisNo, DWORD *upMethod);

    // ���� �ӵ� ������ RPM(Revolution Per Minute)���� ���߰� �ʹٸ�.
    // ex>    rpm ���:
    // 4500 rpm ?
    // unit/ pulse = 1 : 1�̸�      pulse/ sec �ʴ� �޽����� �Ǵµ�
    // 4500 rpm�� ���߰� �ʹٸ�     4500 / 60 �� : 75ȸ��/ 1��
    // ���Ͱ� 1ȸ���� �� �޽����� �˾ƾ� �ȴ�. �̰��� Encoder�� Z���� �˻��غ��� �˼��ִ�.
    // 1ȸ��:1800 �޽���� 75 x 1800 = 135000 �޽��� �ʿ��ϰ� �ȴ�.
    // AxmMotSetMoveUnitPerPulse�� Unit = 1, Pulse = 1800 �־� ���۽�Ų��.
    // ���ǻ��� : rpm���� �����ϰ� �ȴٸ� �ӵ��� ���ӵ� �� rpm���� ������ �����Ͽ��� �Ѵ�.
    
    // ���� ���� �޽� �� �����̴� �Ÿ��� �����Ѵ�.
    DWORD    __stdcall AxmMotSetMoveUnitPerPulse(long lAxisNo, double dUnit, long lPulse);
    // ���� ���� �޽� �� �����̴� �Ÿ��� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmMotGetMoveUnitPerPulse(long lAxisNo, double *dpUnit, long *lpPulse);
    
    // ���� �࿡ ���� ���� ����Ʈ ���� ����� �����Ѵ�.
    // uMethod : 0 -1 ����
    // AutoDetect = 0x0 : �ڵ� ������.
    // RestPulse  = 0x1 : ���� ������."
    DWORD    __stdcall AxmMotSetDecelMode(long lAxisNo, DWORD uMethod);
    // ���� ���� ���� ���� ����Ʈ ���� ����� ��ȯ�Ѵ�
    DWORD    __stdcall AxmMotGetDecelMode(long lAxisNo, DWORD *upMethod);
    
    // ���� �࿡ ���� ���� ��忡�� �ܷ� �޽��� �����Ѵ�.
    // �����: ���� AxmMotSetRemainPulse�� 500 �޽��� ����
    //           AxmMoveStartPos�� ��ġ 10000�� ��������쿡 9500�޽����� 
    //           ���� �޽� 500��  AxmMotSetMinVel�� ������ �ӵ��� �����ϸ鼭 ���� �ȴ�.
    DWORD    __stdcall AxmMotSetRemainPulse(long lAxisNo, DWORD uData);
    // ���� ���� ���� ���� ��忡�� �ܷ� �޽��� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmMotGetRemainPulse(long lAxisNo, DWORD *upData);

    // ���� �࿡ ���� ��� ���� �Լ��� �ְ� �ӵ� ���� �� UNIT �������� �����Ѵ�.
    // ���ǻ��� : �Է� �ִ� �ӵ� ���� PPS�� �ƴ϶� UNIT �̴�.
    // ex) �ִ� ��� ���ļ�(PCI-N804/404 : 10 MPPS)
    // ex) �ִ� ��� Unit/Sec(PCI-N804/404 : 10MPPS * Unit/Pulse)
    DWORD    __stdcall AxmMotSetMaxVel(long lAxisNo, double dVel);
    // ���� �࿡ ���� ��� ���� �Լ��� �ְ� �ӵ� ���� ���� ���� UNIT �������� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmMotGetMaxVel(long lAxisNo, double *dpVel);

    // ���� ���� �̵� �Ÿ� ��� ��带 �����Ѵ�.
    //uAbsRelMode : POS_ABS_MODE '0' - ���� ��ǥ��
    //              POS_REL_MODE '1' - ��� ��ǥ��
    DWORD    __stdcall AxmMotSetAbsRelMode(long lAxisNo, DWORD uAbsRelMode);
    // ���� ���� ������ �̵� �Ÿ� ��� ��带 ��ȯ�Ѵ�
    DWORD    __stdcall AxmMotGetAbsRelMode(long lAxisNo, DWORD *upAbsRelMode);

    // ���� ���� ���� �ӵ� �������� ��带 �����Ѵ�.
    // ProfileMode : SYM_TRAPEZOIDE_MODE    '0' - ��Ī Trapezode
    //               ASYM_TRAPEZOIDE_MODE   '1' - ���Ī Trapezode
    //               QUASI_S_CURVE_MODE     '2' - ��Ī Quasi-S Curve
    //               SYM_S_CURVE_MODE       '3' - ��Ī S Curve
    //               ASYM_S_CURVE_MODE      '4' - ���Ī S Curve
    DWORD    __stdcall AxmMotSetProfileMode(long lAxisNo, DWORD uProfileMode);
    // ���� ���� ������ ���� �ӵ� �������� ��带 ��ȯ�Ѵ�.
    DWORD    __stdcall AxmMotGetProfileMode(long lAxisNo, DWORD *upProfileMode);
    
    //���� ���� ���ӵ� ������ �����Ѵ�.
    //AccelUnit : UNIT_SEC2   '0' - ������ ������ unit/sec2 ���
    //            SEC         '1' - ������ ������ sec ���
    DWORD    __stdcall AxmMotSetAccelUnit(long lAxisNo, DWORD uAccelUnit);
    // ���� ���� ������ ���ӵ������� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmMotGetAccelUnit(long lAxisNo, DWORD *upAccelUnit);

    // ���ǻ���: �ּҼӵ��� UNIT/PULSE ���� �۰��� ��� �ּҴ����� UNIT/PULSE�� ���߾����⶧���� �ּ� �ӵ��� UNIT/PULSE �� �ȴ�.
    // ���� �࿡ �ʱ� �ӵ��� �����Ѵ�.
    DWORD    __stdcall AxmMotSetMinVel(long lAxisNo, double dMinVel);
    // ���� ���� �ʱ� �ӵ��� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmMotGetMinVel(long lAxisNo, double *dpMinVel);
    
    // ���� ���� ���� ��ũ���� �����Ѵ�.[%].
    DWORD    __stdcall AxmMotSetAccelJerk(long lAxisNo, double dAccelJerk);
    // ���� ���� ������ ���� ��ũ���� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmMotGetAccelJerk(long lAxisNo, double *dpAccelJerk);
    
    // ���� ���� ���� ��ũ���� �����Ѵ�.[%].
    DWORD    __stdcall AxmMotSetDecelJerk(long lAxisNo, double dDecelJerk);
    // ���� ���� ������ ���� ��ũ���� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmMotGetDecelJerk(long lAxisNo, double *dpDecelJerk);

    // ���� ���� �ӵ� Profile������ �켱����(�ӵ� Or ���ӵ�)�� �����Ѵ�.
    // Priority : PRIORITY_VELOCITY   '0' - �ӵ� Profile������ ������ �ӵ����� �������� �����(�Ϲ���� �� Spinner�� ���).
    //            PRIORITY_ACCELTIME  '1' - �ӵ� Profile������ ������ �����ӽð��� �������� �����(��� ��� ���).
    DWORD    __stdcall AxmMotSetProfilePriority(long lAxisNo, DWORD uPriority);
    // ���� ���� �ӵ� Profile������ �켱����(�ӵ� Or ���ӵ�)�� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmMotGetProfilePriority(long lAxisNo, DWORD *upPriority);

//=========== ����� ��ȣ ���� �����Լ� =============================================================================
    // ���� ���� Z �� Level�� �����Ѵ�.
    // uLevel : LOW(0), HIGH(1)
    DWORD    __stdcall AxmSignalSetZphaseLevel(long lAxisNo, DWORD uLevel);
    // ���� ���� Z �� Level�� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmSignalGetZphaseLevel(long lAxisNo, DWORD *upLevel);

    // ���� ���� Servo-On��ȣ�� ��� ������ �����Ѵ�.
    // uLevel : LOW(0), HIGH(1)
    DWORD    __stdcall AxmSignalSetServoOnLevel(long lAxisNo, DWORD uLevel);
    // ���� ���� Servo-On��ȣ�� ��� ���� ������ ��ȯ�Ѵ�.
    DWORD    __stdcall AxmSignalGetServoOnLevel(long lAxisNo, DWORD *upLevel);

    // ���� ���� Servo-Alarm Reset ��ȣ�� ��� ������ �����Ѵ�.
    // uLevel : LOW(0), HIGH(1)
    DWORD    __stdcall AxmSignalSetServoAlarmResetLevel(long lAxisNo, DWORD uLevel);
    // ���� ���� Servo-Alarm Reset ��ȣ�� ��� ������ ������ ��ȯ�Ѵ�.
    DWORD    __stdcall AxmSignalGetServoAlarmResetLevel(long lAxisNo, DWORD *upLevel);

    // ���� ���� Inpositon ��ȣ ��� ���� �� ��ȣ �Է� ������ �����Ѵ�
    // uLevel : LOW(0), HIGH(1), UNUSED(2), USED(3)
    DWORD    __stdcall AxmSignalSetInpos(long lAxisNo, DWORD uUse);
    // ���� ���� Inpositon ��ȣ ��� ���� �� ��ȣ �Է� ������ ��ȯ�Ѵ�.
    DWORD    __stdcall AxmSignalGetInpos(long lAxisNo, DWORD *upUse);
    // ���� ���� Inpositon ��ȣ �Է� ���¸� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmSignalReadInpos(long lAxisNo, DWORD *upStatus);

    // ���� ���� �˶� ��ȣ �Է� �� ��� ������ ��� ���� �� ��ȣ �Է� ������ �����Ѵ�.
    // uLevel : LOW(0), HIGH(1), UNUSED(2), USED(3)
    DWORD    __stdcall AxmSignalSetServoAlarm(long lAxisNo, DWORD uUse);
    // ���� ���� �˶� ��ȣ �Է� �� ��� ������ ��� ���� �� ��ȣ �Է� ������ ��ȯ�Ѵ�.
    DWORD    __stdcall AxmSignalGetServoAlarm(long lAxisNo, DWORD *upUse);
    // ���� ���� �˶� ��ȣ�� �Է� ������ ��ȯ�Ѵ�.
    DWORD    __stdcall AxmSignalReadServoAlarm(long lAxisNo, DWORD *upStatus);

    // ���� ���� end limit sensor�� ��� ���� �� ��ȣ�� �Է� ������ �����Ѵ�. 
    // end limit sensor ��ȣ �Է� �� �������� �Ǵ� �������� ���� ������ �����ϴ�.
    // uStopMode: EMERGENCY_STOP(0), SLOWDOWN_STOP(1)
    // uPositiveLevel, uNegativeLevel : LOW(0), HIGH(1), UNUSED(2), USED(3)
    DWORD    __stdcall AxmSignalSetLimit(long lAxisNo, DWORD uStopMode, DWORD uPositiveLevel, DWORD uNegativeLevel);
    // ���� ���� end limit sensor�� ��� ���� �� ��ȣ�� �Է� ����, ��ȣ �Է� �� ������带 ��ȯ�Ѵ�
    DWORD    __stdcall AxmSignalGetLimit(long lAxisNo, DWORD *upStopMode, DWORD *upPositiveLevel, DWORD *upNegativeLevel);
    // �������� end limit sensor�� �Է� ���¸� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmSignalReadLimit(long lAxisNo, DWORD *upPositiveStatus, DWORD *upNegativeStatus);

    // ���� ���� Software limit�� ��� ����, ����� ī��Ʈ, �׸��� ��������� �����Ѵ�.
    // uUse       : DISABLE(0), ENABLE(1)
    // uStopMode  : EMERGENCY_STOP(0), SLOWDOWN_STOP(1)
    // uSelection : COMMAND(0), ACTUAL(1)
    // ���ǻ���: �����˻��� �� �Լ��� �̿��Ͽ� ����Ʈ���� ������ �̸� �����ؼ� ������ �����˻��� �����˻��� ���߿� ���߾�������쿡��  Enable�ȴ�. 
    DWORD    __stdcall AxmSignalSetSoftLimit(long lAxisNo, DWORD uUse, DWORD uStopMode, DWORD uSelection, double dPositivePos, double dNegativePos);
    // ���� ���� Software limit�� ��� ����, ����� ī��Ʈ, �׸��� ���� ����� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmSignalGetSoftLimit(long lAxisNo, DWORD *upUse, DWORD *upStopMode, DWORD *upSelection, double *dpPositivePos, double *dpNegativePos);
    // ���� ���� Software limit�� ���� ���¸� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmSignalReadSoftLimit(long lAxisNo, DWORD *upPositiveStatus, DWORD *upNegativeStatus);

    // ��� ���� ��ȣ�� ���� ��� (������/��������) �Ǵ� ��� ������ �����Ѵ�.
    // uStopMode  : EMERGENCY_STOP(0), SLOWDOWN_STOP(1)
    // uLevel : LOW(0), HIGH(1), UNUSED(2), USED(3)
    DWORD    __stdcall AxmSignalSetStop(long lAxisNo, DWORD uStopMode, DWORD uLevel);
    // ��� ���� ��ȣ�� ���� ��� (������/��������) �Ǵ� ��� ������ ��ȯ�Ѵ�.
    DWORD    __stdcall AxmSignalGetStop(long lAxisNo, DWORD *upStopMode, DWORD *upLevel);
    // ��� ���� ��ȣ�� �Է� ���¸� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmSignalReadStop(long lAxisNo, DWORD *upStatus);

    // ���� ���� Servo-On ��ȣ�� ����Ѵ�.
    // uOnOff : FALSE(0), TRUE(1) ( ���� 0��¿� �ش��)
    DWORD    __stdcall AxmSignalServoOn(long lAxisNo, DWORD uOnOff);
    // ���� ���� Servo-On ��ȣ�� ��� ���¸� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmSignalIsServoOn(long lAxisNo, DWORD *upOnOff);

    // ���� ���� Servo-Alarm Reset ��ȣ�� ����Ѵ�.
    // uOnOff : FALSE(0), TRUE(1) ( ���� 1��¿� �ش��)
    DWORD    __stdcall AxmSignalServoAlarmReset(long lAxisNo, DWORD uOnOff);
    
    // ���� ��°��� �����Ѵ�.
    // uValue : Hex Value 0x00
    DWORD    __stdcall AxmSignalWriteOutput(long lAxisNo, DWORD uValue);
    // ���� ��°��� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmSignalReadOutput(long lAxisNo, DWORD *upValue);

    // lBitNo : Bit Number(0 - 4)
    // uOnOff : FALSE(0), TRUE(1)
    // ���� ��°��� ��Ʈ���� �����Ѵ�.
    DWORD    __stdcall AxmSignalWriteOutputBit(long lAxisNo, long lBitNo, DWORD uOnOff);
    // ���� ��°��� ��Ʈ���� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmSignalReadOutputBit(long lAxisNo, long lBitNo, DWORD *upOnOff);

    // ���� �Է°��� Hex������ ��ȯ�Ѵ�.
    DWORD    __stdcall AxmSignalReadInput(long lAxisNo, DWORD *upValue);
    // lBitNo : Bit Number(0 - 4)
    // ���� �Է°��� ��Ʈ���� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmSignalReadInputBit(long lAxisNo, long lBitNo, DWORD *upOn);

    // �Է½�ȣ���� ������ ���Ͱ���� �����Ѵ�.
    // uSignal: END_LIMIT(0), INP_ALARM(1), UIN_00_01(2), UIN_02_04(3)
    // dBandwidthUsec: 0.2uSec~26666usec
    DWORD    __stdcall AxmSignalSetFilterBandwidth(long lAxisNo, DWORD uSignal, double dBandwidthUsec);

//========== ��� ������ �� �����Ŀ� ���� Ȯ���ϴ� �Լ�==============================================================
    // (��������)���� ���� �޽� ��� ���¸� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmStatusReadInMotion(long lAxisNo, DWORD *upStatus);

    // (�޽� ī��Ʈ ��)�������� ���� ���� ���� ���� �޽� ī���� ���� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmStatusReadDrivePulseCount(long lAxisNo, long *lpPulse);
    
    // ���� ���� DriveStatus(����� ����) �������͸� ��ȯ�Ѵ�
    // ���ǻ��� : �� ��ǰ���� �ϵ�������� ��ȣ�� �ٸ��⶧���� �Ŵ��� �� AXHS.xxx ������ �����ؾ��Ѵ�.
    DWORD    __stdcall AxmStatusReadMotion(long lAxisNo, DWORD *upStatus);
    
    // ���� ���� EndStatus(���� ����) �������͸� ��ȯ�Ѵ�.
    // ���ǻ��� : �� ��ǰ���� �ϵ�������� ��ȣ�� �ٸ��⶧���� �Ŵ��� �� AXHS.xxx ������ �����ؾ��Ѵ�.
    DWORD    __stdcall AxmStatusReadStop(long lAxisNo, DWORD *upStatus);
    
    // ���� ���� Mechanical Signal Data(���� ������� ��ȣ����) �� ��ȯ�Ѵ�.
    // ���ǻ��� : �� ��ǰ���� �ϵ�������� ��ȣ�� �ٸ��⶧���� �Ŵ��� �� AXHS.xxx ������ �����ؾ��Ѵ�.
    DWORD    __stdcall AxmStatusReadMechanical(long lAxisNo, DWORD *upStatus);
    
    // ���� ���� ���� ���� �ӵ��� �о�´�.
    DWORD    __stdcall AxmStatusReadVel(long lAxisNo, double *dpVel);
    
    // ���� ���� Command Pos�� Actual Pos�� ���� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmStatusReadPosError(long lAxisNo, double *dpError);
    
    // ���� ����̺�� �̵��ϴ�(�̵���) �Ÿ��� Ȯ�� �Ѵ�
    DWORD    __stdcall AxmStatusReadDriveDistance(long lAxisNo, double *dpUnit);

    // ���� ���� ��ġ ���� ��� ����� ���Ͽ� �����Ѵ�.
    // uPosType  : Actual position �� Command position �� ǥ�� ���
    //    POSITION_LIMIT '0' - �⺻ ����, ��ü ���� ������ ����
    //    POSITION_BOUND '1' - ��ġ ���� �ֱ���, dNegativePos ~ dPositivePos ������ ����
    // ���ǻ���(PCI-Nx04�ش�)
    // - BOUNT������ ī��Ʈ ���� Max���� �ʰ� �� �� Min���̵Ǹ� �ݴ�� Min���� �ʰ� �� �� Max���� �ȴ�.
    // - �ٽø��� ���� ��ġ���� ������ �� �ۿ��� ī��Ʈ �� ���� ���� Min, Max���� ������� �ʴ´�.
    DWORD    __stdcall AxmStatusSetPosType(long lAxisNo, DWORD uPosType, double dPositivePos, double dNegativePos);
    // ���� ���� ��ġ ���� ��� ����� ���Ͽ� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmStatusGetPosType(long lAxisNo, DWORD *upPosType, double *dpPositivePos, double *dpNegativePos);
    // ���� ���� ����ġ ���ڴ� ���� Offset ��ġ�� �����Ѵ�.[PCI-R1604-MLII ����]
    DWORD    __stdcall AxmStatusSetAbsOrgOffset(long lAxisNo, double dOrgOffsetPos);

    // ���� ���� Actual ��ġ�� �����Ѵ�.
    DWORD    __stdcall AxmStatusSetActPos(long lAxisNo, double dPos);
    // ���� ���� Actual ��ġ�� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmStatusGetActPos(long lAxisNo, double *dpPos);

    // ���� ���� Command ��ġ�� �����Ѵ�.
    DWORD    __stdcall AxmStatusSetCmdPos(long lAxisNo, double dPos);
    // ���� ���� Command ��ġ�� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmStatusGetCmdPos(long lAxisNo, double *dpPos);
    // ���� ���� Command ��ġ�� Actual ��ġ�� dPos ������ ��ġ ��Ų��.
    // RTEX ��� ��� ���(RTEX-PM) ���� �Լ�.
    DWORD    __stdcall AxmStatusSetPosMatch(long lAxisNo, double dPos);

    // ���� ���� ��� ����(Cmd, Act, Driver Status, Mechanical Signal, Universal Signal)�� �ѹ��� Ȯ�� �� �� �ִ�.
    // MOTION_INFO ����ü�� dwMask �������� ��� ���� ������ �����Ѵ�.
    // dwMask : ��� ���� ǥ��(6bit) - ex) dwMask = 0x1F ���� �� ��� ���¸� ǥ����.
    //    [0]        |    Command Position Read
    //    [1]        |    Actual Position Read
    //    [2]        |    Mechanical Signal Read
    //    [3]        |    Driver Status Read
    //    [4]        |    Universal Signal Input Read
    //               |    Universal Signal Output Read
    DWORD    __stdcall AxmStatusReadMotionInfo(long lAxisNo, PMOTION_INFO pMI);

    // Network ��ǰ �����Լ�.
    // ������ ���� �����ѿ� AlarmCode�� �о������ ����ϴ� �Լ�.
    DWORD    __stdcall AxmStatusRequestServoAlarm(long lAxisNo);
    // ������ ���� ������ AlarmCode�� �о���� �Լ�.
    // upAlarmCode      : �ش� �������� Alarm Code����
    // uReturnMode      : �Լ��� ��ȯ ���������� ����
    // [0-Immediate]    : �Լ� ���� �� �ٷ� ��ȯ
    // [1-Blocking]     : ���������� ���� �˶� �ڵ带 ���� �� ���� ��ȯ��������
    // [2-Non Blocking] : ���������� ���� �˶� �ڵ带 ���� �� ���� ��ȯ���������� ���α׷� Blocking��������
    DWORD    __stdcall AxmStatusReadServoAlarm(long lAxisNo, DWORD uReturnMode, DWORD *upAlarmCode);
    // ������ �����ڵ忡 �ش��ϴ� Alarm String�� �޾ƿ��� �Լ�
    DWORD    __stdcall AxmStatusGetServoAlarmString(long lAxisNo, DWORD uAlarmCode, long lAlarmStringSize, char *szAlarmString);

    // ������ ���� �����ѿ� Alarm History�� �о������ ����ϴ� �Լ�
    DWORD    __stdcall AxmStatusRequestServoAlarmHistory(long lAxisNo);
    // ������ ���� ������ Alarm History�� �о���� �Լ�.
    // lpCount          : ���� Alarm History ����
    // upAlarmCode      : Alarm History�� ��ȯ�� �迭
    // uReturnMode      : �Լ��� ��ȯ ���������� ����
    // [0-Immediate]    : �Լ� ���� �� �ٷ� ��ȯ
    // [1-Blocking]     : ���������� ���� �˶� �ڵ带 ���� �� ���� ��ȯ��������
    // [2-Non Blocking] : ���������� ���� �˶� �ڵ带 ���� �� ���� ��ȯ���������� ���α׷� Blocking��������
    DWORD    __stdcall AxmStatusReadServoAlarmHistory(long lAxisNo, DWORD uReturnMode, long *lpCount, DWORD *upAlarmCode);
    // ������ ���� ������ Alarm History�� Clear�Ѵ�.
    DWORD    __stdcall AxmStatusClearServoAlarmHistory(long lAxisNo);

//======== Ȩ���� �Լ�===============================================================================================
    // ���� ���� Home ���� Level �� �����Ѵ�.
    // uLevel : LOW(0), HIGH(1)
    DWORD    __stdcall AxmHomeSetSignalLevel(long lAxisNo, DWORD uLevel);
    // ���� ���� Home ���� Level �� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmHomeGetSignalLevel(long lAxisNo, DWORD *upLevel);
    // ���� Ȩ ��ȣ �Է»��¸� Ȯ���Ѵ�. Ȩ��ȣ�� ����ڰ� ���Ƿ� AxmHomeSetMethod �Լ��� �̿��Ͽ� �����Ҽ��ִ�.
    // �Ϲ������� Ȩ��ȣ�� ���� �Է� 0�� ����ϰ������� AxmHomeSetMethod �̿��ؼ� �ٲٸ� + , - Limit�� ����Ҽ����ִ�.
    // upStatus : OFF(0), ON(1)
    DWORD    __stdcall AxmHomeReadSignal(long lAxisNo, DWORD *upStatus);
    
    // �ش� ���� �����˻��� �����ϱ� ���ؼ��� �ݵ�� ���� �˻����� �Ķ��Ÿ���� �����Ǿ� �־�� �˴ϴ�. 
    // ���� MotionPara���� ������ �̿��� �ʱ�ȭ�� ���������� ����ƴٸ� ������ ������ �ʿ����� �ʴ�. 
    // �����˻� ��� �������� �˻� �������, �������� ����� ��ȣ, �������� Active Level, ���ڴ� Z�� ���� ���� ���� ���� �Ѵ�.
    // ���ǻ��� : ������ �߸� ������ -�������� �����ص�  +�������� �����Ҽ� ������, Ȩ�� ã�µ� �־� ������ �ɼ��ִ�.
    // (�ڼ��� ������ AxmMotSaveParaAll ���� �κ� ����)
    // Ȩ������ AxmSignalSetHomeLevel ����Ѵ�.
    // HClrTim : HomeClear Time : ���� �˻� Encoder �� Set�ϱ� ���� ���ð� 
    // HmDir(Ȩ ����): DIR_CCW (0) -���� , DIR_CW(1) +����
    // HOffset - ���������� �̵��Ÿ�.
    // uZphas: 1�� �����˻� �Ϸ� �� ���ڴ� Z�� ���� ���� ����  0: ������ , 1: HmDir�� �ݴ� ����, 2: HmDir�� ���� ����
    // HmSig :  PosEndLimit(0) -> +Limit
    //          NegEndLimit(1) -> -Limit
    //          HomeSensor (4) -> ��������(���� �Է� 0)
    DWORD   __stdcall AxmHomeSetMethod(long lAxisNo, long lHmDir, DWORD uHomeSignal, DWORD uZphas, double dHomeClrTime, double dHomeOffset);
    // �����Ǿ��ִ� Ȩ ���� �Ķ��Ÿ���� ��ȯ�Ѵ�.
    DWORD   __stdcall AxmHomeGetMethod(long lAxisNo, long *lpHmDir, DWORD *upHomeSignal, DWORD *upZphas, double *dpHomeClrTime, double *dpHomeOffset);

    // �����˻� ����� �̼������� �ϴ� �Լ�(�⺻������ �������� �ʾƵ���).
    // dHomeDogDistance[500 pulse]: ù��° Step���� HomeDog�� ������ �����ƴ��� Ȯ���ϱ����� Dog���̸� �Է�.(������ AxmMotSetMoveUnitPerPulse�Լ��� ������ ����)
    // lLevelScanTime[100msec]: 2��° Step(���������� ���������� ����)���� Level���¸� Ȯ���� Scan�ð��� ����(������ msec[1~1000]).
    // dwFineSearchUse[USE]: �⺻ �����˻��� 5 Step�� ����ϴµ� 3 Step�� ����ϵ��� �����Ҷ� 0���� ����.
    // dwHomeClrUse[USE]: �����˻� �� ���ɰ��� Encoder���� 0���� �ڵ� �������θ� ����.
    DWORD   __stdcall AxmHomeSetFineAdjust(long lAxisNo, double dHomeDogLength, long lLevelScanTime, DWORD uFineSearchUse, DWORD uHomeClrUse);
    // �����Ǿ��ִ� Ȩ ���� �̼����� �Ķ��Ÿ���� ��ȯ�Ѵ�.
    DWORD   __stdcall AxmHomeGetFineAdjust(long lAxisNo, double *dpHomeDogLength, long *lpLevelScanTime, DWORD *upFineSearchUse, DWORD *upHomeClrUse);

    // ������ ������ �����ϰ� �˻��ϱ� ���� ���� �ܰ��� �������� �����Ѵ�. �̶� �� ���ǿ� ��� �� �ӵ��� �����Ѵ�. 
    // �� �ӵ����� �������� ���� �����˻� �ð���, �����˻� ���е��� �����ȴ�. 
    // �� ���Ǻ� �ӵ����� ������ �ٲ㰡�鼭 �� ���� �����˻� �ӵ��� �����ϸ� �ȴ�. 
    // (�ڼ��� ������ AxmMotSaveParaAll ���� �κ� ����)
    // �����˻��� ���� �ӵ��� �����ϴ� �Լ�
    // [dVelFirst]- 1�������ӵ�   [dVelSecond]-�����ļӵ�   [dVelThird]- ������ �ӵ�  [dvelLast]- index�˻��� �����ϰ� �˻��ϱ�����. 
    // [dAccFirst]- 1���������ӵ� [dAccSecond]-�����İ��ӵ� 
    DWORD    __stdcall AxmHomeSetVel(long lAxisNo, double dVelFirst, double dVelSecond, double dVelThird, double dVelLast, double dAccFirst, double dAccSecond);
    // �����Ǿ��ִ� �����˻��� ���� �ӵ��� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmHomeGetVel(long lAxisNo, double *dpVelFirst, double *dpVelSecond, double *dpVelThird, double *dpVelLast, double *dpAccFirst, double *dpAccSecond);

    // �����˻��� �����Ѵ�.
    // �����˻� �����Լ��� �����ϸ� ���̺귯�� ���ο��� �ش����� �����˻��� ���� �� �����尡 �ڵ� �����Ǿ� �����˻��� ���������� ������ �� �ڵ� ����ȴ�.
    // ���ǻ��� : �������� �ݴ������ ����Ʈ ������ ���͵� ��������� ������ ACTIVE���������� �����Ѵ�.
    //            ���� �˻��� ���۵Ǿ� ��������� ����Ʈ ������ ������ ����Ʈ ������ �����Ǿ��ٰ� �����ϰ� �����ܰ�� ����ȴ�.
    DWORD    __stdcall AxmHomeSetStart(long lAxisNo);
    // �����˻� ����� ����ڰ� ���Ƿ� �����Ѵ�.
    // �����˻� �Լ��� �̿��� ���������� �����˻��� ����ǰ��� �˻� ����� HOME_SUCCESS�� �����˴ϴ�.
    // �� �Լ��� ����ڰ� �����˻��� ���������ʰ� ����� ���Ƿ� ������ �� �ִ�.
    // uHomeResult ����
    // HOME_SUCCESS          = 0x01    // Ȩ �Ϸ�
    // HOME_SEARCHING        = 0x02    // Ȩ�˻���
    // HOME_ERR_GNT_RANGE    = 0x10    // Ȩ �˻� ������ ��������
    // HOME_ERR_USER_BREAK   = 0x11    // �ӵ� ������ ���Ƿ� ��������� ���������
    // HOME_ERR_VELOCITY     = 0x12    // �ӵ� ���� �߸��������
    // HOME_ERR_AMP_FAULT    = 0x13    // ������ �˶� �߻� ����
    // HOME_ERR_NEG_LIMIT    = 0x14    // (-)���� ������ (+)����Ʈ ���� ���� ����    
    // HOME_ERR_POS_LIMIT    = 0x15    // (+)���� ������ (-)����Ʈ ���� ���� ����
    // HOME_ERR_NOT_DETECT   = 0x16    // ������ ��ȣ �������� �� �� ��� ����
    // HOME_ERR_UNKNOWN      = 0xFF
    DWORD    __stdcall AxmHomeSetResult(long lAxisNo, DWORD uHomeResult);
    // �����˻� ����� ��ȯ�Ѵ�.
    // �����˻� �Լ��� �˻� ����� Ȯ���Ѵ�. �����˻��� ���۵Ǹ� HOME_SEARCHING���� �����Ǹ� �����˻��� �����ϸ� ���п����� �����ȴ�. ���� ������ ������ �� �ٽ� �����˻��� �����ϸ� �ȴ�.
    DWORD    __stdcall AxmHomeGetResult(long lAxisNo, DWORD *upHomeResult);

    // �����˻� ������� ��ȯ�Ѵ�.
    // �����˻� ���۵Ǹ� �������� Ȯ���� �� �ִ�. �����˻��� �Ϸ�Ǹ� �������ο� ������� 100�� ��ȯ�ϰ� �ȴ�. �����˻� �������δ� GetHome Result�Լ��� �̿��� Ȯ���� �� �ִ�.
    // upHomeMainStepNumber                        : Main Step �������̴�. 
    // ��Ʈ�� FALSE�� ���upHomeMainStepNumber     : 0 �϶��� ������ �ุ ��������̰� Ȩ �������� upHomeStepNumber ǥ���Ѵ�.
    // ��Ʈ�� TRUE�� ��� upHomeMainStepNumber     : 0 �϶��� ������ Ȩ�� ��������̰� ������ Ȩ �������� upHomeStepNumber ǥ���Ѵ�.
    // ��Ʈ�� TRUE�� ��� upHomeMainStepNumber     : 10 �϶��� �����̺� Ȩ�� ��������̰� ������ Ȩ �������� upHomeStepNumber ǥ���Ѵ�.
    // upHomeStepNumber                            : ������ �࿡���� �������� ǥ���Ѵ�. 
    // ��Ʈ�� FALSE�� ���                         : ������ �ุ �������� ǥ���Ѵ�.
    // ��Ʈ�� TRUE�� ��� ��������, �����̺��� ������ �������� ǥ�õȴ�.
    DWORD    __stdcall AxmHomeGetRate(long lAxisNo, DWORD *upHomeMainStepNumber, DWORD *upHomeStepNumber);

//========= ��ġ �����Լ� ===========================================================================================
    // ���ǻ���: ��ġ�� �����Ұ�� �ݵ�� UNIT/PULSE�� ���߾ �����Ѵ�.
    //           ��ġ�� UNIT/PULSE ���� �۰��� ��� �ּҴ����� UNIT/PULSE�� ���߾����⶧���� ����ġ���� ������ �ɼ�����.

    // ���� �ӵ� ������ RPM(Revolution Per Minute)���� ���߰� �ʹٸ�.
    // ex>    rpm ���:
    // 4500 rpm ?
    // unit/ pulse = 1 : 1�̸�      pulse/ sec �ʴ� �޽����� �Ǵµ�
    // 4500 rpm�� ���߰� �ʹٸ�     4500 / 60 �� : 75ȸ��/ 1��
    // ���Ͱ� 1ȸ���� �� �޽����� �˾ƾ� �ȴ�. �̰��� Encoder�� Z���� �˻��غ��� �˼��ִ�.
    // 1ȸ��:1800 �޽���� 75 x 1800 = 135000 �޽��� �ʿ��ϰ� �ȴ�.
    // AxmMotSetMoveUnitPerPulse�� Unit = 1, Pulse = 1800 �־� ���۽�Ų��. 

    // ������ �Ÿ���ŭ �Ǵ� ��ġ���� �̵��Ѵ�.
    // ���� ���� ���� ��ǥ/ �����ǥ �� ������ ��ġ���� ������ �ӵ��� �������� ������ �Ѵ�.
    // �ӵ� ���������� AxmMotSetProfileMode �Լ����� �����Ѵ�.
    // �޽��� ��µǴ� �������� �Լ��� �����.
    // AxmMotSetAccelUnit(lAxisNo, 1) �ϰ�� dAccel -> dAccelTime , dDecel -> dDecelTime ���� �ٲ��.
    DWORD    __stdcall AxmMoveStartPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel);

    // ������ �Ÿ���ŭ �Ǵ� ��ġ���� �̵��Ѵ�.
    // ���� ���� ���� ��ǥ/�����ǥ�� ������ ��ġ���� ������ �ӵ��� �������� ������ �Ѵ�.
    // �ӵ� ���������� AxmMotSetProfileMode �Լ����� �����Ѵ�. 
    // �޽� ����� ����Ǵ� �������� �Լ��� �����
    DWORD    __stdcall AxmMovePos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel);

    // ������ �ӵ��� �����Ѵ�.
    // ���� �࿡ ���Ͽ� ������ �ӵ��� �������� ���������� �ӵ� ��� ������ �Ѵ�. 
    // �޽� ����� ���۵Ǵ� �������� �Լ��� �����.
    // Vel���� ����̸� CW, �����̸� CCW �������� ����.
    DWORD    __stdcall AxmMoveVel(long lAxisNo, double dVel, double dAccel, double dDecel);

    // ������ ���࿡ ���Ͽ� ������ �ӵ��� �������� ���������� �ӵ� ��� ������ �Ѵ�.
    // �޽� ����� ���۵Ǵ� �������� �Լ��� �����.
    // Vel���� ����̸� CW, �����̸� CCW �������� ����.
    DWORD    __stdcall AxmMoveStartMultiVel(long lArraySize, long *lpAxesNo, double *dpVel, double *dpAccel, double *dpDecel);

    // ������ ���࿡ ���Ͽ� ������ �ӵ��� ������, SyncMode�� ���� ���������� �ӵ� ��� ������ �Ѵ�.
    // �޽� ����� ���۵Ǵ� �������� �Լ��� �����.
    // Vel���� ����̸� CW, �����̸� CCW �������� ����.
    // dwSyncMode    : ����������� ������(0), �������� ��ɸ� ���(1), �˶��� ���ؼ��� ���� ������ ���(2)
    DWORD    __stdcall AxmMoveStartMultiVelEx(long lArraySize, long *lpAxesNo, double *dpVel, double *dpAccel, double *dpDecel, DWORD dwSyncMode);

    // ������ ���࿡ ���Ͽ� ������ �ӵ��� �������� ���������� �ӵ� ��� ������ �Ѵ�.
    // �޽� ����� ���۵Ǵ� �������� �Լ��� ����� Master����(Distance�� ���� ū) dVel�ӵ��� �����̸�, ������ ����� Distance������ �����δ�. 
    // �ӵ��� �ش� Chip�� �� ��ȣ�� ���� ���� ���� �ӵ��� ����
    DWORD    __stdcall AxmMoveStartLineVel(long lArraySize, long *lpAxesNo, double *dpDis, double dVel, double dAccel, double dDecel);

    // Ư�� Input ��ȣ�� Edge�� �����Ͽ� ������ �Ǵ� ���������ϴ� �Լ�.
    // lDetect Signal : edge ������ �Է� ��ȣ ����.
    // lDetectSignal  : PosEndLimit(0), NegEndLimit(1), HomeSensor(4), EncodZPhase(5), UniInput02(6), UniInput03(7)
    // Signal Edge    : ������ �Է� ��ȣ�� edge ���� ���� (rising or falling edge).
    //                  SIGNAL_DOWN_EDGE(0), SIGNAL_UP_EDGE(1)
    // ��������       : Vel���� ����̸� CW, �����̸� CCW.
    // SignalMethod   : ������ EMERGENCY_STOP(0), �������� SLOWDOWN_STOP(1)
    // ���ǻ���: SignalMethod�� EMERGENCY_STOP(0)�� ����Ұ�� �������� ���õǸ� ������ �ӵ��� ���� �������ϰԵȴ�.
    //           PCI-Nx04�� ����� ��� lDetectSignal�� PosEndLimit , NegEndLimit(0,1) �� ã����� ��ȣ�Ƿ��� Active ���¸� �����ϰԵȴ�.
    DWORD    __stdcall AxmMoveSignalSearch(long lAxisNo, double dVel, double dAccel, long lDetectSignal, long lSignalEdge, long lSignalMethod);
    
    // ���� �࿡�� ������ ��ȣ�� �����ϰ� �� ��ġ�� �����ϱ� ���� �̵��ϴ� �Լ��̴�.
    // ���ϴ� ��ȣ�� ��� ã�� �����̴� �Լ� ã�� ��� �� ��ġ�� ������ѳ��� AxmGetCapturePos����Ͽ� �װ��� �д´�.
    // Signal Edge   : ������ �Է� ��ȣ�� edge ���� ���� (rising or falling edge).
    //                 SIGNAL_DOWN_EDGE(0), SIGNAL_UP_EDGE(1)
    // ��������      : Vel���� ����̸� CW, �����̸� CCW.
    // SignalMethod  : ������ EMERGENCY_STOP(0), �������� SLOWDOWN_STOP(1)
    // lDetect Signal: edge ������ �Է� ��ȣ ����.SIGNAL_DOWN_EDGE(0), SIGNAL_UP_EDGE(1)
    // lDetectSignal : PosEndLimit(0), NegEndLimit(1), HomeSensor(4), EncodZPhase(5), UniInput02(6), UniInput03(7)
    // lTarget       : COMMAND(0), ACTUAL(1)
    // ���ǻ���: SignalMethod�� EMERGENCY_STOP(0)�� ����Ұ�� �������� ���õǸ� ������ �ӵ��� ���� �������ϰԵȴ�.
    //           PCI-Nx04�� ����� ��� lDetectSignal�� PosEndLimit , NegEndLimit(0,1) �� ã����� ��ȣ�Ƿ��� Active ���¸� �����ϰԵȴ�.
    DWORD    __stdcall AxmMoveSignalCapture(long lAxisNo, double dVel, double dAccel, long lDetectSignal, long lSignalEdge, long lTarget, long lSignalMethod);
    // 'AxmMoveSignalCapture' �Լ����� ����� ��ġ���� Ȯ���ϴ� �Լ��̴�.
    // ���ǻ���: �Լ� ���� ����� "AXT_RT_SUCCESS"�϶� ����� ��ġ�� ��ȿ�ϸ�, �� �Լ��� �ѹ� �����ϸ� ���� ��ġ���� �ʱ�ȭ�ȴ�.
    DWORD    __stdcall AxmMoveGetCapturePos(long lAxisNo, double *dpCapPotition);

    // ������ �Ÿ���ŭ �Ǵ� ��ġ���� �̵��ϴ� �Լ�.
    // �Լ��� �����ϸ� �ش� Motion ������ ������ �� Motion �� �Ϸ�ɶ����� ��ٸ��� �ʰ� �ٷ� �Լ��� ����������."
    DWORD    __stdcall AxmMoveStartMultiPos(long lArraySize, long *lpAxisNo, double *dpPos, double *dpVel, double *dpAccel, double *dpDecel);
    
    // ������ ������ �Ÿ���ŭ �Ǵ� ��ġ���� �̵��Ѵ�.
    // ���� ����� ���� ��ǥ�� ������ ��ġ���� ������ �ӵ��� �������� ������ �Ѵ�.
    DWORD    __stdcall AxmMoveMultiPos(long lArraySize, long *lpAxisNo, double *dpPos, double *dpVel, double *dpAccel, double *dpDecel);

    // ������ ��ũ �� �ӵ� ������ ���͸� �����Ѵ�.(MLII, Sigma 5 ���� ���� �Լ�)
    // dTroque        : �ִ� ��� ��ũ�� ���� %��.     
    // ��������       : dTroque���� ����̸� CW, �����̸� CCW.
    // dVel           : �ִ� ���� ���� �ӵ��� ���� %��.
    // dwAccFilterSel : LINEAR_ACCDCEL(0), EXPO_ACCELDCEL(1), SCURVE_ACCELDECEL(2)
    // dwGainSel      : GAIN_1ST(0), GAIN_2ND(1)
    // dwSpdLoopSel   : PI_LOOP(0), P_LOOP(1)
    DWORD    __stdcall AxmMoveStartTorque(long lAxisNo, double dTorque, double dVel, DWORD dwAccFilterSel, DWORD dwGainSel, DWORD dwSpdLoopSel);

    // ���� ���� ��ũ ������ ���� �Ѵ�.
    // AxmMoveStartTorque�� �ݵ�� AxmMoveTorqueStop�� �����Ͽ��� �Ѵ�.
    DWORD    __stdcall AxmMoveTorqueStop(long lAxisNo, DWORD dwMethod);

    // ������ �Ÿ���ŭ �Ǵ� ��ġ���� �̵��Ѵ�.
    // ���� ���� ���� ��ǥ/�����ǥ�� ������ ��ġ���� ������ �ӵ�/�������� ������ �Ѵ�.
    // �ӵ� ���������� ���Ī ��ٸ��÷� �����˴ϴ�.
    // �����ӵ� ���� ������ ����� �����˴ϴ�.
    // dAccel != 0.0 �̰� dDecel == 0.0 �� ��� ���� �ӵ����� ���� ���� ���� �ӵ����� ����.
    // dAccel != 0.0 �̰� dDecel != 0.0 �� ��� ���� �ӵ����� ���� �ӵ����� ������ ��� ���� ����.
    // dAccel == 0.0 �̰� dDecel != 0.0 �� ��� ���� �ӵ����� ���� �ӵ����� ����.

    // ������ ������ �����Ͽ��� �մϴ�.
    // dVel[1] == dVel[3]�� �ݵ�� �����Ͽ��� �Ѵ�.
    // dVel[2]�� ���� ���� ������ �߻��� �� �ֵ��� dPosition�� ����� ū���̾�� �Ѵ�.
    // Ex) dPosition = 10000;
    // dVel[0] = 300., dAccel[0] = 200., dDecel[0] = 0.;    <== ����
    // dVel[1] = 500., dAccel[1] = 100., dDecel[1] = 0.;    <== ����
    // dVel[2] = 700., dAccel[2] = 200., dDecel[2] = 250.;  <== ����, ���, ����
    // dVel[3] = 500., dAccel[3] = 0.,   dDecel[3] = 150.;  <== ����
    // dVel[4] = 200., dAccel[4] = 0.,   dDecel[4] = 350.;  <== ����
    // �޽� ����� ����Ǵ� �������� �Լ��� �����
    DWORD    __stdcall AxmMoveStartPosWithList(long lAxisNo, double dPosition, double *dpVel, double *dpAccel, double *dpDecel, long lListNum);

    // ���� ���� ������ ���ӵ��� ���� ���� �Ѵ�.
    // dDecel : ���� �� ��������
    DWORD    __stdcall AxmMoveStop(long lAxisNo, double dDecel);
    // ���� ���� �� ���� �Ѵ�.
    DWORD    __stdcall AxmMoveEStop(long lAxisNo);
    // ���� ���� ���� �����Ѵ�.
    DWORD    __stdcall AxmMoveSStop(long lAxisNo);

//========= �������̵� �Լ� =========================================================================================
    // ��ġ �������̵� �Ѵ�.
    // ���� ���� ������ ����Ǳ� �� ������ ��� �޽� ���� �����Ѵ�.
    // PCI-Nx04 �������ǻ���: �������̵��� ��ġ�� �������� ���� ������ ��ġ�� ���������� Relative ������ ��ġ������ �־��ش�.
    //                          ���������� ���������� ��� �������̵带 ����Ҽ������� �ݴ�������� �������̵��Ұ�쿡�� �������̵带 ����Ҽ�����.
    DWORD    __stdcall AxmOverridePos(long lAxisNo, double dOverridePos);

    // ���� ���� �ӵ��������̵� �ϱ����� �������̵��� �ְ�ӵ��� �����Ѵ�.
    // ������ : �ӵ��������̵带 5���Ѵٸ� ���߿� �ְ� �ӵ��� �����ؾߵȴ�. 
    DWORD    __stdcall AxmOverrideSetMaxVel(long lAxisNo, double dOverrideMaxVel);
    // �ӵ� �������̵� �Ѵ�.
    // ���� ���� ���� �߿� �ӵ��� ���� �����Ѵ�. (�ݵ�� ��� �߿� ���� �����Ѵ�.)
    // ������: AxmOverrideVel �Լ��� ����ϱ�����. AxmOverrideMaxVel �ְ�� �����Ҽ��ִ� �ӵ��� �����س��´�.
    // EX> �ӵ��������̵带 �ι��Ѵٸ� 
    // 1. �ΰ��߿� ���� �ӵ��� AxmOverrideMaxVel ���� �ְ� �ӵ��� ����.
    // 2. AxmMoveStartPos ���� ���� ���� ���� ��(Move�Լ� ��� ����)�� �ӵ��� ù��° �ӵ��� AxmOverrideVel ���� �����Ѵ�.
    // 3. ���� ���� ���� ��(Move�Լ� ��� ����)�� �ӵ��� �ι�° �ӵ��� AxmOverrideVel ���� �����Ѵ�.
    DWORD    __stdcall AxmOverrideVel(long lAxisNo, double dOverrideVel);
    // ���ӵ�, �ӵ�, ���ӵ���  �������̵� �Ѵ�.
    // ���� ���� ���� �߿� ���ӵ�, �ӵ�, ���ӵ��� ���� �����Ѵ�. (�ݵ�� ��� �߿� ���� �����Ѵ�.)
    // ������: AxmOverrideAccelVelDecel �Լ��� ����ϱ�����. AxmOverrideMaxVel �ְ�� �����Ҽ��ִ� �ӵ��� �����س��´�.
    // EX> �ӵ��������̵带 �ι��Ѵٸ� 
    // 1. �ΰ��߿� ���� �ӵ��� AxmOverrideMaxVel ���� �ְ� �ӵ��� ����.
    // 2. AxmMoveStartPos ���� ���� ���� ���� ��(Move�Լ� ��� ����)�� ���ӵ�, �ӵ�, ���ӵ��� ù��° �ӵ��� AxmOverrideAccelVelDecel ���� �����Ѵ�.
    // 3. ���� ���� ���� ��(Move�Լ� ��� ����)�� ���ӵ�, �ӵ�, ���ӵ��� �ι�° �ӵ��� AxmOverrideAccelVelDecel ���� �����Ѵ�.
    DWORD    __stdcall AxmOverrideAccelVelDecel(long lAxisNo, double dOverrideVelocity, double dMaxAccel, double dMaxDecel);
    // ��� �������� �ӵ� �������̵� �Ѵ�.
    // ��� ��ġ ������ �������̵��� �ӵ��� �Է½��� ����ġ���� �ӵ��������̵� �Ǵ� �Լ�
    // lTarget : COMMAND(0), ACTUAL(1)
    // ������  : AxmOverrideVelAtPos �Լ��� ����ϱ�����. AxmOverrideMaxVel �ְ�� �����Ҽ��ִ� �ӵ��� �����س��´�.
    DWORD    __stdcall AxmOverrideVelAtPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel,double dOverridePos, double dOverrideVel, long lTarget);
    // ������ �����鿡�� ������ �ӵ��� �������̵� �Ѵ�.
    // lArraySize     : �������̵� �� ��ġ�� ������ ����.
    // *dpOverridePos : �������̵� �� ��ġ�� �迭(lArraySize���� ������ �������� ���ų� ũ�� �����ؾߵ�)
    // *dpOverrideVel : �������̵� �� ��ġ���� ���� �� �ӵ� �迭(lArraySize���� ������ �������� ���ų� ũ�� �����ؾߵ�)
    // lTarget        : COMMAND(0), ACTUAL(1) 
    // dwOverrideMode : �������̵� ���� ����� ������.
    //                : OVERRIDE_POS_START(0) ������ ��ġ���� ������ �ӵ��� �������̵� ������        
    //                : OVERRIDE_POS_END(1) ������ ��ġ���� ������ �ӵ��� �ǵ��� �̸� �������̵� ������
    DWORD    __stdcall AxmOverrideVelAtMultiPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, long lArraySize, double* dpOverridePos, double* dpOverrideVel, long lTarget, DWORD dwOverrideMode);

//========= ������, �����̺�  ����� ���� �Լ� ====================================================================
    // Electric Gear ��忡�� Master ��� Slave ����� ���� �����Ѵ�.
    // dSlaveRatio : �������࿡ ���� �����̺��� ����( 0 : 0% , 0.5 : 50%, 1 : 100%)
    DWORD    __stdcall AxmLinkSetMode(long lMasterAxisNo, long lSlaveAxisNo, double dSlaveRatio);
    // Electric Gear ��忡�� ������ Master ��� Slave ����� ���� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmLinkGetMode(long lMasterAxisNo, long *lpSlaveAxisNo, double *dpGearRatio);
    // Master ��� Slave�ణ�� ���ڱ��� ���� ���� �Ѵ�.
    DWORD    __stdcall AxmLinkResetMode(long lMasterAxisNo);

//======== ��Ʈ�� ���� �Լ�==========================================================================================
    // ��Ǹ���� �� ���� �ⱸ������ Link�Ǿ��ִ� ��Ʈ�� �����ý��� ��� �����Ѵ�. 
    // �� �Լ��� �̿��� Master���� ��Ʈ�� ����� �����ϸ� �ش� Slave���� Master��� ����Ǿ� �����˴ϴ�. 
    // ���� ��Ʈ�� ���� ���� Slave�࿡ ��������̳� ���� ��ɵ��� ������ ��� ���õ˴ϴ�.
    // uSlHomeUse     : �������� Ȩ��� ��� ( 0 - 2)
    //             (0 : �����̺��� Ȩ�� �����ϰ� ���������� Ȩ�� ã�´�.)
    //             (1 : �������� , �����̺��� Ȩ�� ã�´�. �����̺� dSlOffset �� �����ؼ� ������.)
    //             (2 : �������� , �����̺��� Ȩ�� ã�´�. �����̺� dSlOffset �� �����ؼ� ��������.)
    // dSlOffset      : �����̺��� �ɼ°�
    // dSlOffsetRange : �����̺��� �ɼ°� ������ ����
    // PCI-Nx04 �������ǻ���: ��Ʈ�� ENABLE�� �����̺����� ����� AxmStatusReadMotion �Լ��� Ȯ���ϸ� True(Motion ���� ��)�� Ȯ�εǾ� �������̴�. 
    //                   �����̺��࿡ AxmStatusReadMotion�� Ȯ�������� InMotion �� False�̸� Gantry Enable�� �ȵȰ��̹Ƿ� �˶� Ȥ�� ����Ʈ ���� ���� Ȯ���Ѵ�.
    DWORD    __stdcall AxmGantrySetEnable(long lMasterAxisNo, long lSlaveAxisNo, DWORD uSlHomeUse, double dSlOffset, double dSlOffsetRange);

    // Slave���� Offset���� �˾Ƴ��¹��.
    // A. ������, �����̺긦 ��� �������� ��Ų��.         
    // B. AxmGantrySetEnable�Լ����� uSlHomeUse = 2�� ������ AxmHomeSetStart�Լ��� �̿��ؼ� Ȩ�� ã�´�. 
    // C. Ȩ�� ã�� ���� ���������� Command���� �о�� ��������� �����̺����� Ʋ���� Offset���� �����ִ�.
    // D. Offset���� �о AxmGantrySetEnable�Լ��� dSlOffset���ڿ� �־��ش�. 
    // E. dSlOffset���� �־��ٶ� �������࿡ ���� �����̺� �� ���̱⶧���� ��ȣ�� �ݴ�� -dSlOffset �־��ش�.
    // F. dSIOffsetRange �� Slave Offset�� Range ������ ���ϴµ� Range�� �Ѱ踦 �����Ͽ� �Ѱ踦 ����� ������ �߻���ų�� ����Ѵ�.        
    // G. AxmGantrySetEnable�Լ��� Offset���� �־�������  AxmGantrySetEnable�Լ����� uSlHomeUse = 1�� ������ AxmHomeSetStart�Լ��� �̿��ؼ� Ȩ�� ã�´�.         

    // ��Ʈ�� ������ �־� ����ڰ� ������ �Ķ��Ÿ�� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmGantryGetEnable(long lMasterAxisNo, DWORD *upSlHomeUse, double *dpSlOffset, double *dpSlORange, DWORD *upGatryOn);
    // ��� ����� �� ���� �ⱸ������ Link�Ǿ��ִ� ��Ʈ�� �����ý��� ��� �����Ѵ�.
    DWORD    __stdcall AxmGantrySetDisable(long lMasterAxisNo, long lSlaveAxisNo);

//====�Ϲ� �����Լ� =================================================================================================
    // ���ǻ���1: AxmContiSetAxisMap�Լ��� �̿��Ͽ� ������Ŀ� ������������� ������ �ϸ鼭 ����ؾߵȴ�.
    //           ��ȣ������ ��쿡�� �ݵ�� ������������� ��迭�� �־�� ���� �����ϴ�.
    
    // ���ǻ���2: ��ġ�� �����Ұ�� �ݵ�� ��������� �����̺� ���� UNIT/PULSE�� ���߾ �����Ѵ�.
    //           ��ġ�� UNIT/PULSE ���� �۰� ������ ��� �ּҴ����� UNIT/PULSE�� ���߾����⶧���� ����ġ���� ������ �ɼ�����.

    // ���ǻ���3: ��ȣ ������ �Ұ�� �ݵ�� ��Ĩ������ ������ �ɼ������Ƿ� 
    //            4�೻������ �����ؼ� ����ؾߵȴ�.

    // ���ǻ���4: ���� ���� ����/�߿� ������ ���� ����(+- Limit��ȣ, ���� �˶�, ������� ��)�� �߻��ϸ� 
    //            ���� ���⿡ ������� ������ �������� �ʰų� ���� �ȴ�.

    // ���� ���� �Ѵ�.
    // �������� �������� �����Ͽ� ���� ���� ���� �����ϴ� �Լ��̴�. ���� ���� �� �Լ��� �����.
    // AxmContiBeginNode, AxmContiEndNode�� ���̻��� ������ ��ǥ�迡 �������� �������� �����Ͽ� ���� ���� �����ϴ� Queue�� �����Լ����ȴ�. 
    // ���� �������� ���� ���� ������ ���� ���� Queue�� �����Ͽ� AxmContiStart�Լ��� ����ؼ� �����Ѵ�.
    DWORD    __stdcall AxmLineMove(long lCoord, double *dpEndPos, double dVel, double dAccel, double dDecel);

    // 2�� ��ȣ���� �Ѵ�.
    // ������, �������� �߽����� �����Ͽ� ��ȣ ���� �����ϴ� �Լ��̴�. ���� ���� �� �Լ��� �����.
    // AxmContiBeginNode, AxmContiEndNode, �� ���̻��� ������ ��ǥ�迡 ������, �������� �߽����� �����Ͽ� �����ϴ� ��ȣ ���� Queue�� �����Լ����ȴ�.
    // �������� ��ȣ ���� ���� ������ ���� ���� Queue�� �����Ͽ� AxmContiStart�Լ��� ����ؼ� �����Ѵ�.
    // lAxisNo = ���� �迭 , dCenterPos = �߽��� X,Y �迭 , dEndPos = ������ X,Y �迭.
    // uCWDir   DIR_CCW(0): �ݽð����, DIR_CW(1) �ð����
    DWORD   __stdcall AxmCircleCenterMove(long lCoord, long *lAxisNo, double *dCenterPos, double *dEndPos, double dVel, double dAccel, double dDecel, DWORD uCWDir);

    // �߰���, �������� �����Ͽ� ��ȣ ���� �����ϴ� �Լ��̴�. ���� ���� �� �Լ��� �����.
    // AxmContiBeginNode, AxmContiEndNode�� ���̻��� ������ ��ǥ�迡 �߰���, �������� �����Ͽ� �����ϴ� ��ȣ ���� Queue�� �����Լ����ȴ�.
    // �������� ��ȣ ���� ���� ������ ���� ���� Queue�� �����Ͽ� AxmContiStart�Լ��� ����ؼ� �����Ѵ�.
    // lAxisNo = ���� �迭 , dMidPos = �߰��� X,Y �迭 , dEndPos = ������ X,Y �迭, lArcCircle = ��ũ(0), ��(1)
    DWORD    __stdcall AxmCirclePointMove(long lCoord, long *lAxisNo, double *dMidPos, double *dEndPos, double dVel, double dAccel, double dDecel, long lArcCircle);

    // ������, �������� �������� �����Ͽ� ��ȣ ���� �����ϴ� �Լ��̴�. ���� ���� �� �Լ��� �����.
    // AxmContiBeginNode, AxmContiEndNode�� ���̻��� ������ ��ǥ�迡 ������, �������� �������� �����Ͽ� ��ȣ ���� �����ϴ� Queue�� �����Լ����ȴ�.
    // �������� ��ȣ ���� ���� ������ ���� ���� Queue�� �����Ͽ� AxmContiStart�Լ��� ����ؼ� �����Ѵ�.
    // lAxisNo = ���� �迭 , dRadius = ������, dEndPos = ������ X,Y �迭 , uShortDistance = ������(0), ū��(1)
    // uCWDir   DIR_CCW(0): �ݽð����, DIR_CW(1) �ð����
    DWORD   __stdcall AxmCircleRadiusMove(long lCoord, long *lAxisNo, double dRadius, double *dEndPos, double dVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance);

    // ������, ȸ�������� �������� �����Ͽ� ��ȣ ���� �����ϴ� �Լ��̴�. ���� ���� �� �Լ��� �����.
    // AxmContiBeginNode, AxmContiEndNode�� ���̻��� ������ ��ǥ�迡 ������, ȸ�������� �������� �����Ͽ� ��ȣ ���� �����ϴ� Queue�� �����Լ����ȴ�.
    // �������� ��ȣ ���� ���� ������ ���� ���� Queue�� �����Ͽ� AxmContiStart�Լ��� ����ؼ� �����Ѵ�.
    // lAxisNo = ���� �迭 , dCenterPos = �߽��� X,Y �迭 , dAngle = ����.
    // uCWDir   DIR_CCW(0): �ݽð����, DIR_CW(1) �ð����
    DWORD   __stdcall AxmCircleAngleMove(long lCoord, long *lAxisNo, double *dCenterPos, double dAngle, double dVel, double dAccel, double dDecel, DWORD uCWDir);

//====���� ���� �Լ� ================================================================================================
    //������ ��ǥ�迡 ���Ӻ��� �� ������ �����Ѵ�.
    //(����� ��ȣ�� 0 ���� ����))
    // ������:  ������Ҷ��� �ݵ�� ���� ���ȣ�� ���� ���ں��� ū���ڸ� �ִ´�.
    //          ������ ���� �Լ��� ����Ͽ��� �� �������ȣ�� ���� ���ȣ�� ���� �� ���� lpAxesNo�� ���� ���ؽ��� �Է��Ͽ��� �Ѵ�.
    //          ������ ���� �Լ��� ����Ͽ��� �� �������ȣ�� �ش��ϴ� ���� ���ȣ�� �ٸ� ���̶�� �Ѵ�.
    //          ���� ���� �ٸ� Coordinate�� �ߺ� �������� ���ƾ� �Ѵ�.
    DWORD    __stdcall AxmContiSetAxisMap(long lCoord, long lSize, long *lpAxesNo);
    //������ ��ǥ�迡 ���Ӻ��� �� ������ ��ȯ�Ѵ�.
    DWORD    __stdcall AxmContiGetAxisMap(long lCoord, long *lpSize, long *lpAxesNo);

    // ������ ��ǥ�迡 ���Ӻ��� �� ����/��� ��带 �����Ѵ�.
    // (������ : �ݵ�� ����� �ϰ� ��밡��)
    // ���� ���� �̵� �Ÿ� ��� ��带 �����Ѵ�.
    //uAbsRelMode : POS_ABS_MODE '0' - ���� ��ǥ��
    //              POS_REL_MODE '1' - ��� ��ǥ��
    DWORD    __stdcall AxmContiSetAbsRelMode(long lCoord, DWORD uAbsRelMode);
    // ������ ��ǥ�迡 ���Ӻ��� �� ����/��� ��带 ��ȯ�Ѵ�.
    DWORD    __stdcall AxmContiGetAbsRelMode(long lCoord, DWORD *upAbsRelMode);

    // ������ ��ǥ�迡 ���� ������ ���� ���� Queue�� ��� �ִ��� Ȯ���ϴ� �Լ��̴�.
    DWORD    __stdcall AxmContiReadFree(long lCoord, DWORD *upQueueFree);
    // ������ ��ǥ�迡 ���� ������ ���� ���� Queue�� ����Ǿ� �ִ� ���� ���� ������ Ȯ���ϴ� �Լ��̴�.
    DWORD    __stdcall AxmContiReadIndex(long lCoord, long *lpQueueIndex);

    // ������ ��ǥ�迡 ���� ���� ������ ���� ����� ���� Queue�� ��� �����ϴ� �Լ��̴�.
    DWORD    __stdcall AxmContiWriteClear(long lCoord);

    // ������ ��ǥ�迡 ���Ӻ������� ������ �۾����� ����� �����Ѵ�. ���Լ��� ȣ������,
    // AxmContiEndNode�Լ��� ȣ��Ǳ� ������ ����Ǵ� ��� ����۾��� ���� ����� �����ϴ� ���� �ƴ϶� ���Ӻ��� ������� ��� �Ǵ� ���̸�,
    // AxmContiStart �Լ��� ȣ��� �� ��μ� ��ϵȸ���� ������ ����ȴ�.
    DWORD    __stdcall AxmContiBeginNode(long lCoord);
    // ������ ��ǥ�迡�� ���Ӻ����� ������ �۾����� ����� �����Ѵ�.
    DWORD    __stdcall AxmContiEndNode(long lCoord);

    // ���� ���� ���� �Ѵ�.
    // dwProfileset(CONTI_NODE_VELOCITY(0) : ���� ���� ���, CONTI_NODE_MANUAL(1) : �������� ���� ���, CONTI_NODE_AUTO(2) : �ڵ� �������� ����, 3 : �ӵ����� ��� ���) 
    DWORD    __stdcall AxmContiStart(long lCoord, DWORD dwProfileset, long lAngle); 
    // ������ ��ǥ�迡 ���� ���� ���� ������ Ȯ���ϴ� �Լ��̴�.
    DWORD    __stdcall AxmContiIsMotion(long lCoord, DWORD *upInMotion);

    // ������ ��ǥ�迡 ���� ���� ���� �� ���� �������� ���� ���� �ε��� ��ȣ�� Ȯ���ϴ� �Լ��̴�.
    DWORD    __stdcall AxmContiGetNodeNum(long lCoord, long *lpNodeNum);
    // ������ ��ǥ�迡 ������ ���� ���� ���� �� �ε��� ������ Ȯ���ϴ� �Լ��̴�.
    DWORD    __stdcall AxmContiGetTotalNodeNum(long lCoord, long *lpNodeNum);

//====================Ʈ���� �Լ� ===================================================================================
    // ���ǻ���:Ʈ���� ��ġ�� �����Ұ�� �ݵ�� UNIT/PULSE�� ���߾ �����Ѵ�.
    //            ��ġ�� UNIT/PULSE ���� �۰��� ��� �ּҴ����� UNIT/PULSE�� ���߾����⶧���� ����ġ�� ����Ҽ�����.

    // ���� �࿡ Ʈ���� ����� ��� ����, ��� ����, ��ġ �񱳱�, Ʈ���� ��ȣ ���� �ð� �� Ʈ���� ��� ��带 �����Ѵ�.
    // Ʈ���� ��� ����� ���ؼ��� ����  AxmTriggerSetTimeLevel �� ����Ͽ� ���� ��� ������ ���� �Ͽ��� �Ѵ�.
    // dTrigTime       : Ʈ���� ��� �ð�, 1usec - �ִ� 50msec ( 1 - 50000 ���� ����)
    // upTriggerLevel  : Ʈ���� ��� ���� ����  => LOW(0),     HIGH(1)
    // uSelect         : ����� ���� ��ġ       => COMMAND(0), ACTUAL(1)
    // uInterrupt      : ���ͷ�Ʈ ����          => DISABLE(0), ENABLE(1)
    
    // ���� �࿡ Ʈ���� ��ȣ ���� �ð� �� Ʈ���� ��� ����, Ʈ���� ��¹���� �����Ѵ�.
    DWORD    __stdcall AxmTriggerSetTimeLevel(long lAxisNo, double dTrigTime, DWORD uTriggerLevel, DWORD uSelect, DWORD uInterrupt);
    // ���� �࿡ Ʈ���� ��ȣ ���� �ð� �� Ʈ���� ��� ����, Ʈ���� ��¹���� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmTriggerGetTimeLevel(long lAxisNo, double *dpTrigTime, DWORD *upTriggerLevel, DWORD *upSelect, DWORD *upInterrupt);
    
    // ���� ���� Ʈ���� ��� ����� �����Ѵ�.
    // uMethod: PERIOD_MODE  0x0 : ���� ��ġ�� �������� dPos�� ��ġ �ֱ�� ����� �ֱ� Ʈ���� ���
    //          ABS_POS_MODE 0x1 : Ʈ���� ���� ��ġ���� Ʈ���� �߻�, ���� ��ġ ���
    // dPos : �ֱ� ���ý� : ��ġ������ġ���� ����ϱ⶧���� �� ��ġ
    //        ���� ���ý� : ����� �� ��ġ, �� ��ġ�Ͱ����� ������ ����� ������.
    // ���ǻ���: AxmTriggerSetAbsPeriod�� �ֱ���� �����Ұ�� ó�� ����ġ�� ���� �ȿ� �����Ƿ� Ʈ���� ����� �ѹ� �߻��Ѵ�.
    DWORD    __stdcall AxmTriggerSetAbsPeriod(long lAxisNo, DWORD uMethod, double dPos);
    // ���� �࿡ Ʈ���� ����� ��� ����, ��� ����, ��ġ �񱳱�, Ʈ���� ��ȣ ���� �ð� �� Ʈ���� ��� ��带 ��ȯ�Ѵ�.
    DWORD    __stdcall AxmTriggerGetAbsPeriod(long lAxisNo, DWORD *upMethod, double *dpPos);

    // ����ڰ� ������ ������ġ���� ������ġ���� ������������ Ʈ���Ÿ� ��� �Ѵ�.
    DWORD    __stdcall AxmTriggerSetBlock(long lAxisNo, double dStartPos, double dEndPos, double dPeriodPos);
    // 'AxmTriggerSetBlock' �Լ��� ������ ���� �д´�..
    DWORD    __stdcall AxmTriggerGetBlock(long lAxisNo, double *dpStartPos, double *dpEndPos, double *dpPeriodPos);

    // ����ڰ� �� ���� Ʈ���� �޽��� ����Ѵ�.
    DWORD    __stdcall AxmTriggerOneShot(long lAxisNo);
    // ����ڰ� �� ���� Ʈ���� �޽��� ���� �ð� �Ŀ� ����Ѵ�.
    DWORD    __stdcall AxmTriggerSetTimerOneshot(long lAxisNo, long lmSec);
    // �Է��� ������ġ���� ������ �ش� ��ġ�� ������ Ʈ���� ��ȣ�� ����Ѵ�.
    DWORD    __stdcall AxmTriggerOnlyAbs(long lAxisNo,long lTrigNum, double* dpTrigPos);
    // Ʈ���� ��� ������ �ʱ�ȭ �Ѵ�.
    DWORD    __stdcall AxmTriggerSetReset(long lAxisNo); 

//======== CRC( �ܿ� �޽� Ŭ���� �Լ�)===============================================================================
    //Level   : LOW(0), HIGH(1), UNUSED(2), USED(3) 
    //uMethod : �ܿ��޽� ���� ��� ��ȣ �޽� �� 2 - 6���� ��������.(PCI-Nx04 ���� �Լ�)
    //          0 : Don't care, 1 : Don't care, 2: 500 uSec, 3:1 mSec, 4:10 mSec, 5:50 mSec, 6:100 mSec
    //���� �࿡ CRC ��ȣ ��� ���� �� ��� ������ �����Ѵ�.
    DWORD    __stdcall AxmCrcSetMaskLevel(long lAxisNo, DWORD uLevel, DWORD uMethod);
    // ���� ���� CRC ��ȣ ��� ���� �� ��� ������ ��ȯ�Ѵ�.
    DWORD    __stdcall AxmCrcGetMaskLevel(long lAxisNo, DWORD *upLevel, DWORD *upMethod);

    //uOnOff  : CRC ��ȣ�� Program���� �߻� ����  (FALSE(0),TRUE(1))
    // ���� �࿡ CRC ��ȣ�� ������ �߻� ��Ų��.
    DWORD    __stdcall AxmCrcSetOutput(long lAxisNo, DWORD uOnOff);
    // ���� ���� CRC ��ȣ�� ������ �߻� ���θ� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmCrcGetOutput(long lAxisNo, DWORD *upOnOff);

//======MPG(Manual Pulse Generation) �Լ�============================================================================
    // lInputMethod  : 0-3 ���� ��������. 0:OnePhase, 1:TwoPhase1(IP������, QI��������) , 2:TwoPhase2, 3:TwoPhase4
    // lDriveMode    : 0�� ��������(0 :MPG ���Ӹ��)
    // MPGPos        : MPG �Է½�ȣ���� �̵��ϴ� �Ÿ�
    // MPGdenominator: MPG(���� �޽� �߻� ��ġ �Է�)���� �� ������ ��
    // dMPGnumerator : MPG(���� �޽� �߻� ��ġ �Է�)���� �� ���ϱ� ��
    // dwNumerator   : �ִ�(1 ����    64) ���� ���� ����
    // dwDenominator : �ִ�(1 ����  4096) ���� ���� ����
    // dMPGdenominator = 4096, MPGnumerator=1 �� �ǹ��ϴ� ���� 
    // MPG �ѹ����� 200�޽��� �״�� 1:1�� 1�޽��� ����� �ǹ��Ѵ�. 
    // ���� dMPGdenominator = 4096, MPGnumerator=2 �� �������� 1:2�� 2�޽��� ����� �������ٴ��ǹ��̴�. 
    // ���⿡ MPG PULSE = ((Numerator) * (Denominator)/ 4096 ) Ĩ���ο� ��³����� �����̴�.
    // ���ǻ���     : AxmStatusReadInMotion �Լ� ���� ����� �����Ѵ�.  (AxmMPGReset �ϱ� ������ ���� ���¿����� ��� ���� �� ����.)

    // ���� �࿡ MPG �Է¹��, ����̺� ���� ���, �̵� �Ÿ�, MPG �ӵ� ���� �����Ѵ�.
    DWORD    __stdcall AxmMPGSetEnable(long lAxisNo, long lInputMethod, long lDriveMode, double dMPGPos, double dVel, double dAccel);
    // ���� �࿡ MPG �Է¹��, ����̺� ���� ���, �̵� �Ÿ�, MPG �ӵ� ���� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmMPGGetEnable(long lAxisNo, long *lpInputMethod, long *lpDriveMode, double *dpMPGPos, double *dpVel, double *dAccel);

    // PCI-Nx04 �Լ� ����.
    // ���� �࿡ MPG ����̺� ���� ��忡�� ���޽��� �̵��� �޽� ������ �����Ѵ�.
    DWORD    __stdcall AxmMPGSetRatio(long lAxisNo, DWORD uMPGnumerator, DWORD uMPGdenominator);
    // ���� �࿡ MPG ����̺� ���� ��忡�� ���޽��� �̵��� �޽� ������ ��ȯ�Ѵ�.
    DWORD    __stdcall AxmMPGGetRatio(long lAxisNo, DWORD *upMPGnumerator, DWORD *upMPGdenominator);
    // ���� �࿡ MPG ����̺� ������ �����Ѵ�.
    DWORD    __stdcall AxmMPGReset(long lAxisNo);

//======= �︮�� �̵� ===============================================================================================
    // ���ǻ��� : Helix�� ���Ӻ��� ���� Spline, ���������� ��ȣ������ ���� ����Ҽ�����.
    
    // ������ ��ǥ�迡 ������, �������� �߽����� �����Ͽ� �︮�� ���� �����ϴ� �Լ��̴�.
    // AxmContiBeginNode, AxmContiEndNode�� ���̻��� ������ ��ǥ�迡 ������, �������� �߽����� �����Ͽ� �︮�� ���Ӻ��� �����ϴ� �Լ��̴�. 
    // ��ȣ ���� ���� ������ ���� ���� Queue�� �����ϴ� �Լ��̴�. AxmContiStart�Լ��� ����ؼ� �����Ѵ�. (���Ӻ��� �Լ��� ���� �̿��Ѵ�)
    // dCenterPos = �߽��� X,Y  , dEndPos = ������ X,Y .
    // uCWDir DIR_CCW(0): �ݽð����, DIR_CW(1) �ð����    
    DWORD    __stdcall AxmHelixCenterMove(long lCoord, double dCenterXPos, double dCenterYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dAccel, double dDecel, DWORD uCWDir);

    // ������ ��ǥ�迡 ������, �������� �������� �����Ͽ� �︮�� ���� �����ϴ� �Լ��̴�. 
    // AxmContiBeginNode, AxmContiEndNode�� ���̻��� ������ ��ǥ�迡 �߰���, �������� �����Ͽ� �︮�ÿ��� ���� �����ϴ� �Լ��̴�. 
    // ��ȣ ���� ���� ������ ���� ���� Queue�� �����ϴ� �Լ��̴�. AxmContiStart�Լ��� ����ؼ� �����Ѵ�. (���Ӻ��� �Լ��� ���� �̿��Ѵ�.)
    // dMidPos = �߰��� X,Y  , dEndPos = ������ X,Y 
    DWORD    __stdcall AxmHelixPointMove(long lCoord, double dMidXPos, double dMidYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dAccel, double dDecel);

    // ������ ��ǥ�迡 ������, �������� �������� �����Ͽ� �︮�� ���� �����ϴ� �Լ��̴�.
    // AxmContiBeginNode, AxmContiEndNode�� ���̻��� ������ ��ǥ�迡 ������, �������� �������� �����Ͽ� �︮�ÿ��� ���� �����ϴ� �Լ��̴�. 
    // ��ȣ ���� ���� ������ ���� ���� Queue�� �����ϴ� �Լ��̴�. AxmContiStart�Լ��� ����ؼ� �����Ѵ�. (���Ӻ��� �Լ��� ���� �̿��Ѵ�.)
    // dRadius = ������, dEndPos = ������ X,Y  , uShortDistance = ������(0), ū��(1)
    // uCWDir   DIR_CCW(0): �ݽð����, DIR_CW(1) �ð����    
    DWORD    __stdcall AxmHelixRadiusMove(long lCoord, double dRadius, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance);

    // ������ ��ǥ�迡 ������, ȸ�������� �������� �����Ͽ� �︮�� ���� �����ϴ� �Լ��̴�
    // AxmContiBeginNode, AxmContiEndNode�� ���̻��� ������ ��ǥ�迡 ������, ȸ�������� �������� �����Ͽ� �︮�ÿ��� ���� �����ϴ� �Լ��̴�. 
    // ��ȣ ���� ���� ������ ���� ���� Queue�� �����ϴ� �Լ��̴�. AxmContiStart�Լ��� ����ؼ� �����Ѵ�. (���Ӻ��� �Լ��� ���� �̿��Ѵ�.)
    //dCenterPos = �߽��� X,Y  , dAngle = ����.
    // uCWDir   DIR_CCW(0): �ݽð����, DIR_CW(1) �ð����
    DWORD    __stdcall AxmHelixAngleMove(long lCoord, double dCenterXPos, double dCenterYPos, double dAngle, double dZPos, double dVel, double dAccel, double dDecel, DWORD uCWDir);

//======== ���ö��� �̵� (PCI-Nx04 ���� �Լ�)========================================================================
    // ���ǻ��� : Spline�� ���Ӻ��� ���� Helix , ���������� ��ȣ������ ���� ����Ҽ�����.

    // AxmContiBeginNode, AxmContiEndNode�� ���̻�����. 
    // ���ö��� ���� ���� �����ϴ� �Լ��̴�. ��ȣ ���� ���� ������ ���� ���� Queue�� �����ϴ� �Լ��̴�.
    // AxmContiStart�Լ��� ����ؼ� �����Ѵ�. (���Ӻ��� �Լ��� ���� �̿��Ѵ�.)    
    // lPosSize : �ּ� 3�� �̻�.
    // 2������ ���� dPoZ���� 0���� �־��ָ� ��.
    // 3������ ���� ������� 3���� dPosZ ���� �־��ش�.
    DWORD    __stdcall AxmSplineWrite(long lCoord, long lPosSize, double *dpPosX, double *dpPosY, double dVel, double dAccel, double dDecel, double dPosZ, long lPointFactor);

//======== PCI-R1604-MLII ���� �Լ�==================================================================================
    // ��ġ ���� ���̺� ��ɿ� �ʿ��� ������ �����Ѵ�.
    DWORD __stdcall AxmCompensationSet(long lAxisNo, long lNumEntry, double dStartPos, double *dpPosition, double *dpCorrection, DWORD dwRollOver);
    // ��ġ ���� ���̺� ��� ���� ������ ��ȯ�Ѵ�.
    DWORD __stdcall AxmCompensationGet(long lAxisNo, long *lpNumEntry, double *dpStartPos, double *dpPosition, double *dpCorrection, DWORD *dwpRollOver);

    // ��ġ ���� ���̺� ����� ������θ� �����Ѵ�.
    DWORD __stdcall AxmCompensationEnable(long lAxisNo, DWORD dwEnable);
    // ��ġ ���� ���̺� ����� ��������� ���� ���� ���¸� ��ȯ�Ѵ�.
    DWORD __stdcall AxmCompensationIsEnable(long lAxisNo, DWORD *dwpEnable);

    // ECAM ��ɿ� �ʿ��� ������ �����Ѵ�.
    DWORD __stdcall AxmEcamSet(long lAxisNo, long lMasterAxis, long lNumEntry, double dMasterStartPos, double *dpMasterPos, double *dpSlavePos);
    // ECAM ��� ���� ������ ��ȯ�Ѵ�.
    DWORD __stdcall AxmEcamGet(long lAxisNo, long *lpMasterAxis, long *lpNumEntry, double *dpMasterStartPos, double *dpMasterPos, double *dpSlavePos);

    // ECAM ����� ��� ������ �����Ѵ�.
    DWORD __stdcall AxmEcamEnableBySlave(long lAxisNo, DWORD dwEnable);
    // ECAM ����� ��� ������ ������ Master �࿡ ����� ��� Slave �࿡ ���Ͽ� �����Ѵ�.
    DWORD __stdcall AxmEcamEnableByMaster(long lAxisNo, DWORD dwEnable);
    // ECAM ����� ��� ������ ���� ���� ���¸� ��ȯ�Ѵ�.
    DWORD __stdcall AxmEcamIsSlaveEnable(long lAxisNo, DWORD *dwpEnable);

//======== Servo Status Monitor =====================================================================================
    // ���� ���� ���� ó�� ��ɿ� ���� �����Ѵ�.(MLII, Sigma-5 ����)
    // dwSelMon(0~3): ���� ���� ����.
    //          [0] : Torque 
    //          [1] : Velocity of motor
    //          [2] : Accel. of motor
    //          [3] : Decel. of motor
    //          [4] : Position error between Cmd. position and Act. position.
    // dwActionValue: �̻� ���� ���� ���� �� ����. �� ������ ���� ���� ���� �ǹ̰� ����.
    //          [0] : dwSelMon���� ������ ���� ������ ���Ͽ� ���� ó�� ���� ����.
    //         [>0] : dwSelMon���� ������ ���� ������ ���Ͽ� ���� ó�� ��� ����.
    // dwAction(0~3): dwActionValue �̻����� ���� ������ Ȯ�εǾ����� ����ó�� ��� ����.
    //          [0] : Warning(setting flag only) 
    //          [1] : Warning(setting flag) + Slow-down stop
    //          [2] : Warning(setting flag) + Emergency stop
    //          [3] : Warning(setting flag) + Emergency stop + Servo-Off
    // �� ����: 5���� SelMon ������ ���� ���� ����ó�� ������ �����ϸ�, ����� ����ó���� ���������� ���
    //          �ݵ�� �ش� SelMon ������ ActionValue���� 0���� ������ ���ñ���� Disable �ص�.
    DWORD __stdcall AxmStatusSetServoMonitor(long lAxisNo, DWORD dwSelMon, double dActionValue, DWORD dwAction);
    // ���� ���� ���� ó�� ��ɿ� ���� ���� ���¸� ��ȯ�Ѵ�.(MLII, Sigma-5 ����)
    DWORD __stdcall AxmStatusGetServoMonitor(long lAxisNo, DWORD dwSelMon, double *dpActionValue, DWORD *dwpAction);
    // ���� ���� ���� ó�� ��ɿ� ���� ��� ������ �����Ѵ�.(MLII, Sigma-5 ����)
    DWORD __stdcall AxmStatusSetServoMonitorEnable(long lAxisNo, DWORD dwEnable);
    // ���� ���� ���� ó�� ��ɿ� ���� ��� ������ ��ȯ�Ѵ�.(MLII, Sigma-5 ����)
    DWORD __stdcall AxmStatusGetServoMonitorEnable(long lAxisNo, DWORD *dwpEnable);

    // ���� ���� ���� ó�� ��� ���� ��� �÷��� ���� ��ȯ�Ѵ�. �Լ� ���� �� �ڵ� �ʱ�ȭ.(MLII, Sigma-5 ����)
    DWORD __stdcall AxmStatusReadServoMonitorFlag(long lAxisNo, DWORD dwSelMon, DWORD *dwpMonitorFlag, double *dpMonitorValue);
    // ���� ���� ���� ó�� ����� ���� ���� ������ ��ȯ�Ѵ�.(MLII, Sigma-5 ����)
    DWORD __stdcall AxmStatusReadServoMonitorValue(long lAxisNo, DWORD dwSelMon, double *dpMonitorValue);
	// ���� ���� �������� ���� �� �ֵ��� ���� �մϴ�.(MLII, Sigma-5 ����)
	// dwSelMon = 0 : Accumulated load ratio
	// dwSelMon = 1 : Regenerative load ratio
	DWORD __stdcall AxmStatusSetReadServoLoadRatio(long lAxisNo, DWORD dwSelMon);
	// ���� ���� �������� ��ȯ�Ѵ�.(MLII, Sigma-5 ����)
	DWORD __stdcall AxmStatusReadServoLoadRatio(long lAxisNo, double *dpMonitorValue);

//======== PCI-R1604-RTEX ���� �Լ�==================================================================================
    // RTEX A4Nx ���� Scale Coefficient�� �����Ѵ�.(RTEX, A4Nx ����)
    DWORD __stdcall AxmMotSetScaleCoeff(long lAxisNo, long lScaleCoeff);
    // RTEX A4Nx ���� Scale Coefficient �� Ȯ���Ѵ�.(RTEX, A4Nx ����)
    DWORD __stdcall AxmMotGetScaleCoeff(long lAxisNo, long *lpScaleCoeff);

	// Ư�� Input ��ȣ�� Edge�� �����Ͽ� ������ �Ǵ� ���������ϴ� �Լ�.
	// lDetect Signal : edge ������ �Է� ��ȣ ����.
	// lDetectSignal  : PosEndLimit(0), NegEndLimit(1), HomeSensor(4)
	// Signal Edge    : ������ �Է� ��ȣ�� edge ���� ���� (rising or falling edge).
	//                  SIGNAL_DOWN_EDGE(0), SIGNAL_UP_EDGE(1)
	// ��������       : Vel���� ����̸� CW, �����̸� CCW.
	// SignalMethod   : ������ EMERGENCY_STOP(0), �������� SLOWDOWN_STOP(1)
	// ���ǻ���: SignalMethod�� EMERGENCY_STOP(0)�� ����Ұ�� �������� ���õǸ� ������ �ӵ��� ���� �������ϰԵȴ�.
	//           PCI-Nx04�� ����� ��� lDetectSignal�� PosEndLimit , NegEndLimit(0,1) �� ã����� ��ȣ�Ƿ��� Active ���¸� �����ϰԵȴ�.
	DWORD    __stdcall AxmMoveSignalSearchEx(long lAxisNo, double dVel, double dAccel, long lDetectSignal, long lSignalEdge, long lSignalMethod);
//-------------------------------------------------------------------------------------------------------------------

//======== PCI-R1604-MLII ���� �Լ�==================================================================================
    // ������ ���� ��ġ�� �̵��Ѵ�.
    // �ӵ� ���������� ����� �������� �����Ѵ�.
    // �޽��� ��µǴ� �������� �Լ��� �����.
    // �׻� ��ġ �� �ӵ�, �����ӵ��� ���� �����ϸ�, �ݴ���� ��ġ ���� ����� �����Ѵ�.
    DWORD    __stdcall AxmMoveToAbsPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel);
    // ���� ���� ���� ���� �ӵ��� �о�´�.
    DWORD    __stdcall AxmStatusReadVelEx(long lAxisNo, double *dpVel);
//-------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif    //__cplusplus

#endif    //__AXT_AXM_H__
