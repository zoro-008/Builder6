#ifndef __AXT_AXDEV_H__
#define __AXT_AXDEV_H__

#include "AXHD.h"

#ifdef __cplusplus
extern "C" {
#endif    //__cplusplus

    // Board Number�� �̿��Ͽ� Board Address ã��
    DWORD    __stdcall AxlGetBoardAddress(long lBoardNo, DWORD *upBoardAddress);
    // Board Number�� �̿��Ͽ� Board ID ã��
    DWORD    __stdcall AxlGetBoardID(long lBoardNo, DWORD *upBoardID);
    // Board Number�� �̿��Ͽ� Board Version ã��
    DWORD    __stdcall AxlGetBoardVersion(long lBoardNo, DWORD *upBoardVersion);
    // Board Number�� Module Position�� �̿��Ͽ� Module ID ã��
    DWORD    __stdcall AxlGetModuleID(long lBoardNo, long lModulePos, DWORD *upModuleID);
    // Board Number�� Module Position�� �̿��Ͽ� Module Version ã��
    DWORD    __stdcall AxlGetModuleVersion(long lBoardNo, long lModulePos, DWORD *upModuleVersion);
    // Board Number�� Module Position�� �̿��Ͽ� AIO Module Number ã��
    DWORD    __stdcall AxaInfoGetModuleNo(long lBoardNo, long lModulePos, long *lpModuleNo);
    // Board Number�� Module Position�� �̿��Ͽ� DIO Module Number ã��
    DWORD    __stdcall AxdInfoGetModuleNo(long lBoardNo, long lModulePos, long *lpModuleNo);

    // ���� �࿡ IPCOMMAND Setting
    DWORD    __stdcall AxmSetCommand(long lAxisNo, IPCOMMAND sCommand);
    // ���� �࿡ 8bit IPCOMMAND Setting
    DWORD    __stdcall AxmSetCommandData08(long lAxisNo, IPCOMMAND sCommand, DWORD uData);
    // ���� �࿡ 8bit IPCOMMAND ��������
    DWORD    __stdcall AxmGetCommandData08(long lAxisNo, IPCOMMAND sCommand, DWORD *upData);
    // ���� �࿡ 16bit IPCOMMAND Setting
    DWORD    __stdcall AxmSetCommandData16(long lAxisNo, IPCOMMAND sCommand, DWORD uData);
    // ���� �࿡ 16bit IPCOMMAND ��������
    DWORD    __stdcall AxmGetCommandData16(long lAxisNo, IPCOMMAND sCommand, DWORD *upData);
    // ���� �࿡ 24bit IPCOMMAND Setting
    DWORD    __stdcall AxmSetCommandData24(long lAxisNo, IPCOMMAND sCommand, DWORD uData);
    // ���� �࿡ 24bit IPCOMMAND ��������
    DWORD    __stdcall AxmGetCommandData24(long lAxisNo, IPCOMMAND sCommand, DWORD *upData);
    // ���� �࿡ 32bit IPCOMMAND Setting
    DWORD    __stdcall AxmSetCommandData32(long lAxisNo, IPCOMMAND sCommand, DWORD uData);
    // ���� �࿡ 32bit IPCOMMAND ��������
    DWORD    __stdcall AxmGetCommandData32(long lAxisNo, IPCOMMAND sCommand, DWORD *upData);
    
    // ���� �࿡ QICOMMAND Setting
    DWORD    __stdcall AxmSetCommandQi(long lAxisNo, QICOMMAND sCommand);
    // ���� �࿡ 8bit QICOMMAND Setting
    DWORD    __stdcall AxmSetCommandData08Qi(long lAxisNo, QICOMMAND sCommand, DWORD uData);
    // ���� �࿡ 8bit QICOMMAND ��������
    DWORD    __stdcall AxmGetCommandData08Qi(long lAxisNo, QICOMMAND sCommand, DWORD *upData);
    // ���� �࿡ 16bit QICOMMAND Setting
    DWORD    __stdcall AxmSetCommandData16Qi(long lAxisNo, QICOMMAND sCommand, DWORD uData);
    // ���� �࿡ 16bit QICOMMAND ��������
    DWORD    __stdcall AxmGetCommandData16Qi(long lAxisNo, QICOMMAND sCommand, DWORD *upData);
    // ���� �࿡ 24bit QICOMMAND Setting
    DWORD    __stdcall AxmSetCommandData24Qi(long lAxisNo, QICOMMAND sCommand, DWORD uData);
    // ���� �࿡ 24bit QICOMMAND ��������
    DWORD    __stdcall AxmGetCommandData24Qi(long lAxisNo, QICOMMAND sCommand, DWORD *upData);
    // ���� �࿡ 32bit QICOMMAND Setting
    DWORD    __stdcall AxmSetCommandData32Qi(long lAxisNo, QICOMMAND sCommand, DWORD uData);
    // ���� �࿡ 32bit QICOMMAND ��������
    DWORD    __stdcall AxmGetCommandData32Qi(long lAxisNo, QICOMMAND sCommand, DWORD *upData);

    // ���� �࿡ Port Data �������� - IP
    DWORD    __stdcall AxmGetPortData(long lAxisNo,  WORD wOffset, DWORD *upData);
    // ���� �࿡ Port Data Setting - IP
    DWORD    __stdcall AxmSetPortData(long lAxisNo, WORD wOffset, DWORD dwData);

    // ���� �࿡ Port Data �������� - QI
    DWORD    __stdcall AxmGetPortDataQi(long lAxisNo, WORD byOffset, WORD *wData);
    // ���� �࿡ Port Data Setting - QI
    DWORD    __stdcall AxmSetPortDataQi(long lAxisNo, WORD byOffset, WORD wData);
        
    // ���� �࿡ ��ũ��Ʈ�� �����Ѵ�. - IP
    // sc    : ��ũ��Ʈ ��ȣ (1 - 4)
    // event : �߻��� �̺�Ʈ SCRCON �� �����Ѵ�.
    //         �̺�Ʈ ���� �హ������, �̺�Ʈ �߻��� ��, �̺�Ʈ ���� 1,2 �Ӽ� �����Ѵ�.
    // cmd   : � ������ �ٲܰ����� ���� SCRCMD�� �����Ѵ�.
    // data  : � Data�� �ٲܰ����� ����
    DWORD    __stdcall AxmSetScriptCaptionIp(long lAxisNo, long sc, DWORD event, DWORD data);
    // ���� �࿡ ��ũ��Ʈ�� ��ȯ�Ѵ�. - IP
    DWORD    __stdcall AxmGetScriptCaptionIp(long lAxisNo, long sc, DWORD *event, DWORD *data);

    // ���� �࿡ ��ũ��Ʈ�� �����Ѵ�. - QI
    // sc    : ��ũ��Ʈ ��ȣ (1 - 4)
    // event : �߻��� �̺�Ʈ SCRCON �� �����Ѵ�.
    //         �̺�Ʈ ���� �హ������, �̺�Ʈ �߻��� ��, �̺�Ʈ ���� 1,2 �Ӽ� �����Ѵ�.
    // cmd   : � ������ �ٲܰ����� ���� SCRCMD�� �����Ѵ�.
    // data  : � Data�� �ٲܰ����� ����
    DWORD    __stdcall AxmSetScriptCaptionQi(long lAxisNo, long sc, DWORD event, DWORD cmd, DWORD data);
    // ���� �࿡ ��ũ��Ʈ�� ��ȯ�Ѵ�. - QI
    DWORD    __stdcall AxmGetScriptCaptionQi(long lAxisNo, long sc, DWORD *event, DWORD *cmd, DWORD *data);

    // ���� �࿡ ��ũ��Ʈ ���� Queue Index�� Clear ��Ų��.
    // uSelect IP. 
    // uSelect(0): ��ũ��Ʈ Queue Index �� Clear�Ѵ�.
    //        (1): ĸ�� Queue�� Index Clear�Ѵ�.

    // uSelect QI. 
    // uSelect(0): ��ũ��Ʈ Queue 1 Index �� Clear�Ѵ�.
    //        (1): ��ũ��Ʈ Queue 2 Index �� Clear�Ѵ�.

    DWORD    __stdcall AxmSetScriptCaptionQueueClear(long lAxisNo, DWORD uSelect);
    
    // ���� �࿡ ��ũ��Ʈ ���� Queue�� Index ��ȯ�Ѵ�. 
    // uSelect IP
    // uSelect(0): ��ũ��Ʈ Queue Index�� �о�´�.
    //        (1): ĸ�� Queue Index�� �о�´�.

    // uSelect QI. 
    // uSelect(0): ��ũ��Ʈ Queue 1 Index�� �о�´�.
    //        (1): ��ũ��Ʈ Queue 2 Index�� �о�´�.

    DWORD    __stdcall AxmGetScriptCaptionQueueCount(long lAxisNo, DWORD *updata, DWORD uSelect);

    // ���� �࿡ ��ũ��Ʈ ���� Queue�� Data���� ��ȯ�Ѵ�. 
    // uSelect IP
    // uSelect(0): ��ũ��Ʈ Queue Data �� �о�´�.
    //        (1): ĸ�� Queue Data�� �о�´�.

    // uSelect QI.
    // uSelect(0): ��ũ��Ʈ Queue 1 Data �о�´�.
    //        (1): ��ũ��Ʈ Queue 2 Data �о�´�.

    DWORD    __stdcall AxmGetScriptCaptionQueueDataCount(long lAxisNo, DWORD *updata, DWORD uSelect);

    // ���� ����Ÿ�� �о�´�.
    DWORD    __stdcall AxmGetOptimizeDriveData(long lAxisNo, double dMinVel, double dVel, double dAccel, double  dDecel, 
            WORD *wRangeData, WORD *wStartStopSpeedData, WORD *wObjectSpeedData, WORD *wAccelRate, WORD *wDecelRate);

    // ���峻�� �������͸� Byte������ ���� �� Ȯ���Ѵ�.
    DWORD    __stdcall AxmBoardWriteByte(long lBoardNo, WORD wOffset, BYTE byData);
    DWORD    __stdcall AxmBoardReadByte(long lBoardNo, WORD wOffset, BYTE *byData);

    // ���峻�� �������͸� Word������ ���� �� Ȯ���Ѵ�.
    DWORD    __stdcall AxmBoardWriteWord(long lBoardNo, WORD wOffset, WORD wData);
    DWORD    __stdcall AxmBoardReadWord(long lBoardNo, WORD wOffset, WORD *wData);

    // ���峻�� �������͸� DWord������ ���� �� Ȯ���Ѵ�.
    DWORD    __stdcall AxmBoardWriteDWord(long lBoardNo, WORD wOffset, DWORD dwData);
    DWORD    __stdcall AxmBoardReadDWord(long lBoardNo, WORD wOffset, DWORD *dwData);

    // ���峻�� ��⿡ �������͸� Byte���� �� Ȯ���Ѵ�.
    DWORD    __stdcall AxmModuleWriteByte(long lBoardNo, long lModulePos, WORD wOffset, BYTE byData);
    DWORD    __stdcall AxmModuleReadByte(long lBoardNo, long lModulePos, WORD wOffset, BYTE *byData);

    // ���峻�� ��⿡ �������͸� Word���� �� Ȯ���Ѵ�.
    DWORD    __stdcall AxmModuleWriteWord(long lBoardNo, long lModulePos, WORD wOffset, WORD wData);
    DWORD    __stdcall AxmModuleReadWord(long lBoardNo, long lModulePos, WORD wOffset, WORD *wData);

    // ���峻�� ��⿡ �������͸� DWord���� �� Ȯ���Ѵ�.
    DWORD    __stdcall AxmModuleWriteDWord(long lBoardNo, long lModulePos, WORD wOffset, DWORD dwData);
    DWORD    __stdcall AxmModuleReadDWord(long lBoardNo, long lModulePos, WORD wOffset, DWORD *dwData);
    

    // �ܺ� ��ġ �񱳱⿡ ���� �����Ѵ�.(Pos = Unit)
    DWORD    __stdcall AxmStatusSetActComparatorPos(long lAxisNo, double dPos);
    // �ܺ� ��ġ �񱳱⿡ ���� ��ȯ�Ѵ�.(Positon = Unit)
    DWORD    __stdcall AxmStatusGetActComparatorPos(long lAxisNo, double *dpPos);

    // ���� ��ġ �񱳱⿡ ���� �����Ѵ�.(Pos = Unit)
    DWORD    __stdcall AxmStatusSetCmdComparatorPos(long lAxisNo, double dPos);
    // ���� ��ġ �񱳱⿡ ���� ��ȯ�Ѵ�.(Pos = Unit)
    DWORD    __stdcall AxmStatusGetCmdComparatorPos(long lAxisNo, double *dpPos);
    
//========== �߰� �Լ� =========================================================================================================
    
    // ���� ���� �� �ӵ��� ������ ���Ѵ�� �����Ѵ�.
    // �ӵ� ������� �Ÿ��� �־��־�� �Ѵ�. 
    DWORD    __stdcall AxmLineMoveVel(long lCoord, double dVel, double dAccel, double dDecel);

//========= ���� ��ġ ���� �Լ�( �ʵ�: IP������ , QI���� ��ɾ���)=========================================================================
    
    // ���� ���� Sensor ��ȣ�� ��� ���� �� ��ȣ �Է� ������ �����Ѵ�.
    // ��� ���� LOW(0), HIGH(1), UNUSED(2), USED(3)
    DWORD    __stdcall AxmSensorSetSignal(long lAxisNo, DWORD uLevel);
    // ���� ���� Sensor ��ȣ�� ��� ���� �� ��ȣ �Է� ������ ��ȯ�Ѵ�.
    DWORD    __stdcall AxmSensorGetSignal(long lAxisNo, DWORD *upLevel);
    // ���� ���� Sensor ��ȣ�� �Է� ���¸� ��ȯ�Ѵ�
    DWORD    __stdcall AxmSensorReadSignal(long lAxisNo, DWORD *upStatus);
    
    // ���� ���� ������ �ӵ��� �������� ���� ��ġ ����̹��� �����Ѵ�.
    // Sensor ��ȣ�� Active level�Է� ���� ��� ��ǥ�� ������ �Ÿ���ŭ ������ �����Ѵ�.
    // �޽��� ��µǴ� �������� �Լ��� �����.
    // lMethod :  0 - �Ϲ� ����, 1 - ���� ��ȣ ���� ���� ���� ����. ��ȣ ���� �� �Ϲ� ����
    //            2 - ���� ����
    DWORD    __stdcall AxmSensorMovePos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, long lMethod);

    // ���� ���� ������ �ӵ��� �������� ���� ��ġ ����̹��� �����Ѵ�.
    // Sensor ��ȣ�� Active level�Է� ���� ��� ��ǥ�� ������ �Ÿ���ŭ ������ �����Ѵ�.
    // �޽� ����� ����Ǵ� �������� �Լ��� �����.
    DWORD    __stdcall AxmSensorStartMovePos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, long lMethod);

//=======�߰� Ȩ ��ġ (PI-N804/404���� �ش��.)=================================================================================

    // ����ڰ� ������ ���� Ȩ���� �Ķ��Ÿ�� �����Ѵ�.(QIĨ ���� �������� �̿�).
    // uZphasCount : Ȩ �Ϸ��Ŀ� Z�� ī��Ʈ(0 - 15)
    // lHomeMode   : Ȩ ���� ���( 0 - 12)
    // lClearSet   : ��ġ Ŭ���� , �ܿ��޽� Ŭ���� ��� ���� (0 - 3)
    //               0: ��ġŬ���� ������, �ܿ��޽� Ŭ���� ��� ����
    //                 1: ��ġŬ���� �����, �ܿ��޽� Ŭ���� ��� ����
    //               2: ��ġŬ���� ������, �ܿ��޽� Ŭ���� �����
    //               3: ��ġŬ���� �����, �ܿ��޽� Ŭ���� �����.
    // dOrgVel : Ȩ���� Org  Speed ���� 
    // dLastVel: Ȩ���� Last Speed ���� 
    DWORD    __stdcall AxmHomeSetConfig(long lAxisNo, DWORD uZphasCount, long lHomeMode, long lClearSet, double dOrgVel, double dLastVel, double dLeavePos);
    // ����ڰ� ������ ���� Ȩ���� �Ķ��Ÿ�� ��ȯ�Ѵ�.
    DWORD    __stdcall AxmHomeGetConfig(long lAxisNo, DWORD *upZphasCount, long *lpHomeMode, long *lpClearSet, double *dpOrgVel, double *dpLastVel, double *dpLeavePos); //KKJ(070215)
    
    // ����ڰ� ������ ���� Ȩ ��ġ�� �����Ѵ�.
    // lHomeMode ���� ���� : 0 - 5 ���� (Move Return�Ŀ� Search��  �����Ѵ�.)
    // lHomeMode -1�� �״�� ���� HomeConfig���� ����Ѵ�� �״�� ������.
    // ��������      : Vel���� ����̸� CW, �����̸� CCW.
    DWORD    __stdcall AxmHomeSetMoveSearch(long lAxisNo, double dVel, double dAccel, double dDecel);

    // ����ڰ� ������ ���� Ȩ ������ �����Ѵ�.
    // lHomeMode ���� ���� : 0 - 12 ���� 
    // lHomeMode -1�� �״�� ���� HomeConfig���� ����Ѵ�� �״�� ������.
    // ��������      : Vel���� ����̸� CW, �����̸� CCW.
    DWORD    __stdcall AxmHomeSetMoveReturn(long lAxisNo, double dVel, double dAccel, double dDecel);
    
    // ����ڰ� ������ ���� Ȩ ��Ż�� �����Ѵ�.
    // ��������      : Vel���� ����̸� CW, �����̸� CCW.
    DWORD    __stdcall AxmHomeSetMoveLeave(long lAxisNo, double dVel, double dAccel, double dDecel);

    // ����ڰ� ������ ������ Ȩ ��ġ�� �����Ѵ�.
    // lHomeMode ���� ���� : 0 - 5 ���� (Move Return�Ŀ� Search��  �����Ѵ�.)
    // lHomeMode -1�� �״�� ���� HomeConfig���� ����Ѵ�� �״�� ������.
    // ��������      : Vel���� ����̸� CW, �����̸� CCW.
    DWORD    __stdcall AxmHomeSetMultiMoveSearch(long lArraySize, long *lpAxesNo, double *dpVel, double *dpAccel, double *dpDecel);

    //������ ��ǥ���� ���� �ӵ� �������� ��带 �����Ѵ�.
    // (������ : �ݵ�� ����� �ϰ� ��밡��)
    // ProfileMode : '0' - ��Ī Trapezode
    //               '1' - ���Ī Trapezode
    //               '2' - ��Ī Quasi-S Curve
    //               '3' - ��Ī S Curve
    //               '4' - ���Ī S Curve
    DWORD    __stdcall AxmContiSetProfileMode(long lCoord, DWORD uProfileMode);
    // ������ ��ǥ���� ���� �ӵ� �������� ��带 ��ȯ�Ѵ�.
    DWORD    __stdcall AxmContiGetProfileMode(long lCoord, DWORD *upProfileMode);

    DWORD    __stdcall AxmMoveProfilePos(long lAxisNo, long lProfileSize, double *dpPos, double *dpMinVel, double *dpVel, double *dpAccel, double *dpDecel);

    //========== DIO ���ͷ�Ʈ �÷��� ������Ʈ �б�
    // ������ �Է� ���� ���, Interrupt Flag Register�� Offset ��ġ���� bit ������ ���ͷ�Ʈ �߻� ���� ���� ����
    DWORD    __stdcall AxdiInterruptFlagReadBit(long lModuleNo, long lOffset, DWORD *upValue);
    // ������ �Է� ���� ���, Interrupt Flag Register�� Offset ��ġ���� byte ������ ���ͷ�Ʈ �߻� ���� ���� ����
    DWORD    __stdcall AxdiInterruptFlagReadByte(long lModuleNo, long lOffset, DWORD *upValue);
    // ������ �Է� ���� ���, Interrupt Flag Register�� Offset ��ġ���� word ������ ���ͷ�Ʈ �߻� ���� ���� ����
    DWORD    __stdcall AxdiInterruptFlagReadWord(long lModuleNo, long lOffset, DWORD *upValue);
    // ������ �Է� ���� ���, Interrupt Flag Register�� Offset ��ġ���� double word ������ ���ͷ�Ʈ �߻� ���� ���� ����
    DWORD    __stdcall AxdiInterruptFlagReadDword(long lModuleNo, long lOffset, DWORD *upValue);
    // ��ü �Է� ���� ���, Interrupt Flag Register�� Offset ��ġ���� bit ������ ���ͷ�Ʈ �߻� ���� ���� ����
    DWORD    __stdcall AxdiInterruptFlagRead(long lOffset, DWORD *upValue);


    //========= �α� ���� �Լ� ==========================================================================================    
    // ���� �ڵ����� ������.
    // ���� ���� �Լ� ���� ����� EzSpy���� ����͸� �� �� �ֵ��� ���� �Ǵ� �����ϴ� �Լ��̴�.
    // uUse : ��� ���� => DISABLE(0), ENABLE(1)
    DWORD    __stdcall AxmLogSetAxis(long lAxisNo, DWORD uUse);
    
    // EzSpy������ ���� �� �Լ� ���� ��� ����͸� ���θ� Ȯ���ϴ� �Լ��̴�.
    DWORD    __stdcall AxmLogGetAxis(long lAxisNo, DWORD *upUse);

//=========== �α� ��� ���� �Լ�
    //������ �Է� ä���� EzSpy�� �α� ��� ���θ� �����Ѵ�.
    DWORD    __stdcall AxaiLogSetChannel(long lChannelNo, DWORD uUse);
    //������ �Է� ä���� EzSpy�� �α� ��� ���θ� Ȯ���Ѵ�.
    DWORD    __stdcall AxaiLogGetChannel(long lChannelNo, DWORD *upUse);

//==������ ��� ä���� EzSpy �α� ��� 
    //������ ��� ä���� EzSpy�� �α� ��� ���θ� �����Ѵ�.
    DWORD    __stdcall AxaoLogSetChannel(long lChannelNo, DWORD uUse);
    //������ ��� ä���� EzSpy�� �α� ��� ���θ� Ȯ���Ѵ�.
    DWORD    __stdcall AxaoLogGetChannel(long lChannelNo, DWORD *upUse);

//==Log
    // ������ ����� EzSpy�� �α� ��� ���� ����
    DWORD    __stdcall AxdLogSetModule(long lModuleNo, DWORD uUse);
    // ������ ����� EzSpy�� �α� ��� ���� Ȯ��
    DWORD    __stdcall AxdLogGetModule(long lModuleNo, DWORD *upUse);

#ifdef __cplusplus
}
#endif    //__cplusplus

#endif    //__AXT_AXDEV_H__
