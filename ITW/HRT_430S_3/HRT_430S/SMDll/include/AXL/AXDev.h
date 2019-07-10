#ifndef __AXT_AXDEV_H__
#define __AXT_AXDEV_H__

#include "AXHD.h"

#ifdef __cplusplus
extern "C" {
#endif    //__cplusplus

    // Board Number를 이용하여 Board Address 찾기
    DWORD    __stdcall AxlGetBoardAddress(long lBoardNo, DWORD *upBoardAddress);
    // Board Number를 이용하여 Board ID 찾기
    DWORD    __stdcall AxlGetBoardID(long lBoardNo, DWORD *upBoardID);
    // Board Number를 이용하여 Board Version 찾기
    DWORD    __stdcall AxlGetBoardVersion(long lBoardNo, DWORD *upBoardVersion);
    // Board Number와 Module Position을 이용하여 Module ID 찾기
    DWORD    __stdcall AxlGetModuleID(long lBoardNo, long lModulePos, DWORD *upModuleID);
    // Board Number와 Module Position을 이용하여 Module Version 찾기
    DWORD    __stdcall AxlGetModuleVersion(long lBoardNo, long lModulePos, DWORD *upModuleVersion);
    // Board Number와 Module Position을 이용하여 Network Node 정보 확인
    DWORD    __stdcall AxlGetModuleNodeInfo(long lBoardNo, long lModulePos, long *upNetNo, DWORD *upNodeAddr);

    // Board에 내장된 범용 Data Flash Write (PCI-R1604[RTEX master board]전용)
    // lPageAddr(0 ~ 199)
    // lByteNum(1 ~ 120)
    // 주의) Flash에 데이타를 기입할 때는 일정 시간(최대 17mSec)이 소요되기때문에 연속 쓰기시 지연 시간이 필요함.
    DWORD    __stdcall AxlSetDataFlash(long lBoardNo, long lPageAddr, long lBytesNum, BYTE *bpSetData);
    
    // Board에 내장된 범용 Data Flash Read(PCI-R1604[RTEX master board]전용)
    // lPageAddr(0 ~ 199)
    // lByteNum(1 ~ 120)
    DWORD    __stdcall AxlGetDataFlash(long lBoardNo, long lPageAddr, long lBytesNum, BYTE *bpGetData);

    // Board Number와 Module Position을 이용하여 AIO Module Number 찾기
    DWORD    __stdcall AxaInfoGetModuleNo(long lBoardNo, long lModulePos, long *lpModuleNo);
    // Board Number와 Module Position을 이용하여 DIO Module Number 찾기
    DWORD    __stdcall AxdInfoGetModuleNo(long lBoardNo, long lModulePos, long *lpModuleNo);

    // 지정 축에 IPCOMMAND Setting
    DWORD    __stdcall AxmSetCommand(long lAxisNo, IPCOMMAND sCommand);
    // 지정 축에 8bit IPCOMMAND Setting
    DWORD    __stdcall AxmSetCommandData08(long lAxisNo, IPCOMMAND sCommand, DWORD uData);
    // 지정 축에 8bit IPCOMMAND 가져오기
    DWORD    __stdcall AxmGetCommandData08(long lAxisNo, IPCOMMAND sCommand, DWORD *upData);
    // 지정 축에 16bit IPCOMMAND Setting
    DWORD    __stdcall AxmSetCommandData16(long lAxisNo, IPCOMMAND sCommand, DWORD uData);
    // 지정 축에 16bit IPCOMMAND 가져오기
    DWORD    __stdcall AxmGetCommandData16(long lAxisNo, IPCOMMAND sCommand, DWORD *upData);
    // 지정 축에 24bit IPCOMMAND Setting
    DWORD    __stdcall AxmSetCommandData24(long lAxisNo, IPCOMMAND sCommand, DWORD uData);
    // 지정 축에 24bit IPCOMMAND 가져오기
    DWORD    __stdcall AxmGetCommandData24(long lAxisNo, IPCOMMAND sCommand, DWORD *upData);
    // 지정 축에 32bit IPCOMMAND Setting
    DWORD    __stdcall AxmSetCommandData32(long lAxisNo, IPCOMMAND sCommand, DWORD uData);
    // 지정 축에 32bit IPCOMMAND 가져오기
    DWORD    __stdcall AxmGetCommandData32(long lAxisNo, IPCOMMAND sCommand, DWORD *upData);
    
    // 지정 축에 QICOMMAND Setting
    DWORD    __stdcall AxmSetCommandQi(long lAxisNo, QICOMMAND sCommand);
    // 지정 축에 8bit QICOMMAND Setting
    DWORD    __stdcall AxmSetCommandData08Qi(long lAxisNo, QICOMMAND sCommand, DWORD uData);
    // 지정 축에 8bit QICOMMAND 가져오기
    DWORD    __stdcall AxmGetCommandData08Qi(long lAxisNo, QICOMMAND sCommand, DWORD *upData);
    // 지정 축에 16bit QICOMMAND Setting
    DWORD    __stdcall AxmSetCommandData16Qi(long lAxisNo, QICOMMAND sCommand, DWORD uData);
    // 지정 축에 16bit QICOMMAND 가져오기
    DWORD    __stdcall AxmGetCommandData16Qi(long lAxisNo, QICOMMAND sCommand, DWORD *upData);
    // 지정 축에 24bit QICOMMAND Setting
    DWORD    __stdcall AxmSetCommandData24Qi(long lAxisNo, QICOMMAND sCommand, DWORD uData);
    // 지정 축에 24bit QICOMMAND 가져오기
    DWORD    __stdcall AxmGetCommandData24Qi(long lAxisNo, QICOMMAND sCommand, DWORD *upData);
    // 지정 축에 32bit QICOMMAND Setting
    DWORD    __stdcall AxmSetCommandData32Qi(long lAxisNo, QICOMMAND sCommand, DWORD uData);
    // 지정 축에 32bit QICOMMAND 가져오기
    DWORD    __stdcall AxmGetCommandData32Qi(long lAxisNo, QICOMMAND sCommand, DWORD *upData);

    // 지정 축에 Port Data 가져오기 - IP
    DWORD    __stdcall AxmGetPortData(long lAxisNo,  WORD wOffset, DWORD *upData);
    // 지정 축에 Port Data Setting - IP
    DWORD    __stdcall AxmSetPortData(long lAxisNo, WORD wOffset, DWORD dwData);
    // 지정 축에 Port Data 가져오기 - QI
    DWORD    __stdcall AxmGetPortDataQi(long lAxisNo, WORD byOffset, WORD *wData);
    // 지정 축에 Port Data Setting - QI
    DWORD    __stdcall AxmSetPortDataQi(long lAxisNo, WORD byOffset, WORD wData);

    // 지정 축에 스크립트를 설정한다. - IP
    // sc    : 스크립트 번호 (1 - 4)
    // event : 발생할 이벤트 SCRCON 을 정의한다.
    //         이벤트 설정 축갯수설정, 이벤트 발생할 축, 이벤트 내용 1,2 속성 설정한다.
    // cmd   : 어떤 내용을 바꿀것인지 선택 SCRCMD를 정의한다.
    // data  : 어떤 Data를 바꿀것인지 선택
    DWORD    __stdcall AxmSetScriptCaptionIp(long lAxisNo, long sc, DWORD event, DWORD data);
    // 지정 축에 스크립트를 반환한다. - IP
    DWORD    __stdcall AxmGetScriptCaptionIp(long lAxisNo, long sc, DWORD *event, DWORD *data);

    // 지정 축에 스크립트를 설정한다. - QI
    // sc    : 스크립트 번호 (1 - 4)
    // event : 발생할 이벤트 SCRCON 을 정의한다.
    //         이벤트 설정 축갯수설정, 이벤트 발생할 축, 이벤트 내용 1,2 속성 설정한다.
    // cmd   : 어떤 내용을 바꿀것인지 선택 SCRCMD를 정의한다.
    // data  : 어떤 Data를 바꿀것인지 선택
    DWORD    __stdcall AxmSetScriptCaptionQi(long lAxisNo, long sc, DWORD event, DWORD cmd, DWORD data);
    // 지정 축에 스크립트를 반환한다. - QI
    DWORD    __stdcall AxmGetScriptCaptionQi(long lAxisNo, long sc, DWORD *event, DWORD *cmd, DWORD *data);

    // 지정 축에 스크립트 내부 Queue Index를 Clear 시킨다.
    // uSelect IP. 
    // uSelect(0): 스크립트 Queue Index 를 Clear한다.
    //        (1): 캡션 Queue를 Index Clear한다.

    // uSelect QI. 
    // uSelect(0): 스크립트 Queue 1 Index 을 Clear한다.
    //        (1): 스크립트 Queue 2 Index 를 Clear한다.

    DWORD    __stdcall AxmSetScriptCaptionQueueClear(long lAxisNo, DWORD uSelect);
    
    // 지정 축에 스크립트 내부 Queue의 Index 반환한다. 
    // uSelect IP
    // uSelect(0): 스크립트 Queue Index를 읽어온다.
    //        (1): 캡션 Queue Index를 읽어온다.

    // uSelect QI. 
    // uSelect(0): 스크립트 Queue 1 Index을 읽어온다.
    //        (1): 스크립트 Queue 2 Index를 읽어온다.

    DWORD    __stdcall AxmGetScriptCaptionQueueCount(long lAxisNo, DWORD *updata, DWORD uSelect);

    // 지정 축에 스크립트 내부 Queue의 Data갯수 반환한다. 
    // uSelect IP
    // uSelect(0): 스크립트 Queue Data 를 읽어온다.
    //        (1): 캡션 Queue Data를 읽어온다.

    // uSelect QI.
    // uSelect(0): 스크립트 Queue 1 Data 읽어온다.
    //        (1): 스크립트 Queue 2 Data 읽어온다.

    DWORD    __stdcall AxmGetScriptCaptionQueueDataCount(long lAxisNo, DWORD *updata, DWORD uSelect);

    // 내부 데이타를 읽어온다.
    DWORD    __stdcall AxmGetOptimizeDriveData(long lAxisNo, double dMinVel, double dVel, double dAccel, double  dDecel, 
            WORD *wRangeData, WORD *wStartStopSpeedData, WORD *wObjectSpeedData, WORD *wAccelRate, WORD *wDecelRate);

    // 보드내에 레지스터를 Byte단위로 설정 및 확인한다.
    DWORD    __stdcall AxmBoardWriteByte(long lBoardNo, WORD wOffset, BYTE byData);
    DWORD    __stdcall AxmBoardReadByte(long lBoardNo, WORD wOffset, BYTE *byData);

    // 보드내에 레지스터를 Word단위로 설정 및 확인한다.
    DWORD    __stdcall AxmBoardWriteWord(long lBoardNo, WORD wOffset, WORD wData);
    DWORD    __stdcall AxmBoardReadWord(long lBoardNo, WORD wOffset, WORD *wData);

    // 보드내에 레지스터를 DWord단위로 설정 및 확인한다.
    DWORD    __stdcall AxmBoardWriteDWord(long lBoardNo, WORD wOffset, DWORD dwData);
    DWORD    __stdcall AxmBoardReadDWord(long lBoardNo, WORD wOffset, DWORD *dwData);

    // 보드내에 모듈에 레지스터를 Byte설정 및 확인한다.
    DWORD    __stdcall AxmModuleWriteByte(long lBoardNo, long lModulePos, WORD wOffset, BYTE byData);
    DWORD    __stdcall AxmModuleReadByte(long lBoardNo, long lModulePos, WORD wOffset, BYTE *byData);

    // 보드내에 모듈에 레지스터를 Word설정 및 확인한다.
    DWORD    __stdcall AxmModuleWriteWord(long lBoardNo, long lModulePos, WORD wOffset, WORD wData);
    DWORD    __stdcall AxmModuleReadWord(long lBoardNo, long lModulePos, WORD wOffset, WORD *wData);

    // 보드내에 모듈에 레지스터를 DWord설정 및 확인한다.
    DWORD    __stdcall AxmModuleWriteDWord(long lBoardNo, long lModulePos, WORD wOffset, DWORD dwData);
    DWORD    __stdcall AxmModuleReadDWord(long lBoardNo, long lModulePos, WORD wOffset, DWORD *dwData);

    // 외부 위치 비교기에 값을 설정한다.(Pos = Unit)
    DWORD    __stdcall AxmStatusSetActComparatorPos(long lAxisNo, double dPos);
    // 외부 위치 비교기에 값을 반환한다.(Positon = Unit)
    DWORD    __stdcall AxmStatusGetActComparatorPos(long lAxisNo, double *dpPos);

    // 내부 위치 비교기에 값을 설정한다.(Pos = Unit)
    DWORD    __stdcall AxmStatusSetCmdComparatorPos(long lAxisNo, double dPos);
    // 내부 위치 비교기에 값을 반환한다.(Pos = Unit)
    DWORD    __stdcall AxmStatusGetCmdComparatorPos(long lAxisNo, double *dpPos);
    
//========== 추가 함수 =========================================================================================================
    
    // 직선 보간 을 속도만 가지고 무한대로 증가한다.
    // 속도 비율대로 거리를 넣어주어야 한다. 
    DWORD    __stdcall AxmLineMoveVel(long lCoord, double dVel, double dAccel, double dDecel);

//========= 센서 위치 구동 함수( 필독: IP만가능 , QI에는 기능없음)==============================================================
    
    // 지정 축의 Sensor 신호의 사용 유무 및 신호 입력 레벨을 설정한다.
    // 사용 유무 LOW(0), HIGH(1), UNUSED(2), USED(3)
    DWORD    __stdcall AxmSensorSetSignal(long lAxisNo, DWORD uLevel);
    // 지정 축의 Sensor 신호의 사용 유무 및 신호 입력 레벨을 반환한다.
    DWORD    __stdcall AxmSensorGetSignal(long lAxisNo, DWORD *upLevel);
    // 지정 축의 Sensor 신호의 입력 상태를 반환한다
    DWORD    __stdcall AxmSensorReadSignal(long lAxisNo, DWORD *upStatus);
    
    // 지정 축의 설정된 속도와 가속율로 센서 위치 드라이버를 구동한다.
    // Sensor 신호의 Active level입력 이후 상대 좌표로 설정된 거리만큼 구동후 정지한다.
    // 펄스가 출력되는 시점에서 함수를 벗어난다.
    // lMethod :  0 - 일반 구동, 1 - 센서 신호 검출 전은 저속 구동. 신호 검출 후 일반 구동
    //            2 - 저속 구동
    DWORD    __stdcall AxmSensorMovePos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, long lMethod);

    // 지정 축의 설정된 속도와 가속율로 센서 위치 드라이버를 구동한다.
    // Sensor 신호의 Active level입력 이후 상대 좌표로 설정된 거리만큼 구동후 정지한다.
    // 펄스 출력이 종료되는 시점에서 함수를 벗어난다.
    DWORD    __stdcall AxmSensorStartMovePos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, long lMethod);

    // 원점검색 진행스탭 변화의 기록을 반환한다.
    // *lpStepCount      : 기록된 Step의 개수
    // *upMainStepNumber : 기록된 MainStepNumber 정보의 배열포인트
    // *upStepNumber     : 기록된 StepNumber 정보의 배열포인트
    // *upStepBranch     : 기록된 Step별 Branch 정보의 배열포인트
    // 주의: 배열개수는 50개로 고정
    DWORD    __stdcall AxmHomeGetStepTrace(long lAxisNo, long *lpStepCount, DWORD *upMainStepNumber, DWORD *upStepNumber, DWORD *upStepBranch);

//=======추가 홈 서치 (PI-N804/404에만 해당됨.)=================================================================================
    // 사용자가 지정한 축의 홈설정 파라메타를 설정한다.(QI칩 전용 레지스터 이용).
    // uZphasCount : 홈 완료후에 Z상 카운트(0 - 15)
    // lHomeMode   : 홈 설정 모드( 0 - 12)
    // lClearSet   : 위치 클리어 , 잔여펄스 클리어 사용 선택 (0 - 3)
    //               0: 위치클리어 사용않함, 잔여펄스 클리어 사용 안함
    //                 1: 위치클리어 사용함, 잔여펄스 클리어 사용 안함
    //               2: 위치클리어 사용안함, 잔여펄스 클리어 사용함
    //               3: 위치클리어 사용함, 잔여펄스 클리어 사용함.
    // dOrgVel : 홈관련 Org  Speed 설정 
    // dLastVel: 홈관련 Last Speed 설정 
    DWORD    __stdcall AxmHomeSetConfig(long lAxisNo, DWORD uZphasCount, long lHomeMode, long lClearSet, double dOrgVel, double dLastVel, double dLeavePos);
    // 사용자가 지정한 축의 홈설정 파라메타를 반환한다.
    DWORD    __stdcall AxmHomeGetConfig(long lAxisNo, DWORD *upZphasCount, long *lpHomeMode, long *lpClearSet, double *dpOrgVel, double *dpLastVel, double *dpLeavePos); //KKJ(070215)
    
    // 사용자가 지정한 축의 홈 서치를 시작한다.
    // lHomeMode 사용시 설정 : 0 - 5 설정 (Move Return후에 Search를  시작한다.)
    // lHomeMode -1로 그대로 사용시 HomeConfig에서 사용한대로 그대로 설정됨.
    // 구동방향      : Vel값이 양수이면 CW, 음수이면 CCW.
    DWORD    __stdcall AxmHomeSetMoveSearch(long lAxisNo, double dVel, double dAccel, double dDecel);

    // 사용자가 지정한 축의 홈 리턴을 시작한다.
    // lHomeMode 사용시 설정 : 0 - 12 설정 
    // lHomeMode -1로 그대로 사용시 HomeConfig에서 사용한대로 그대로 설정됨.
    // 구동방향      : Vel값이 양수이면 CW, 음수이면 CCW.
    DWORD    __stdcall AxmHomeSetMoveReturn(long lAxisNo, double dVel, double dAccel, double dDecel);
    
    // 사용자가 지정한 축의 홈 이탈을 시작한다.
    // 구동방향      : Vel값이 양수이면 CW, 음수이면 CCW.
    DWORD    __stdcall AxmHomeSetMoveLeave(long lAxisNo, double dVel, double dAccel, double dDecel);

    // 사용자가 지정한 다축의 홈 서치을 시작한다.
    // lHomeMode 사용시 설정 : 0 - 5 설정 (Move Return후에 Search를  시작한다.)
    // lHomeMode -1로 그대로 사용시 HomeConfig에서 사용한대로 그대로 설정됨.
    // 구동방향      : Vel값이 양수이면 CW, 음수이면 CCW.
    DWORD    __stdcall AxmHomeSetMultiMoveSearch(long lArraySize, long *lpAxesNo, double *dpVel, double *dpAccel, double *dpDecel);

    //지정된 좌표계의 구동 속도 프로파일 모드를 설정한다.
    // (주의점 : 반드시 축맵핑 하고 사용가능)
    // ProfileMode : '0' - 대칭 Trapezode
    //               '1' - 비대칭 Trapezode
    //               '2' - 대칭 Quasi-S Curve
    //               '3' - 대칭 S Curve
    //               '4' - 비대칭 S Curve
    DWORD    __stdcall AxmContiSetProfileMode(long lCoord, DWORD uProfileMode);
    // 지정된 좌표계의 구동 속도 프로파일 모드를 반환한다.
    DWORD    __stdcall AxmContiGetProfileMode(long lCoord, DWORD *upProfileMode);

    //========== DIO 인터럽트 플래그 레지스트 읽기
    // 지정한 입력 접점 모듈, Interrupt Flag Register의 Offset 위치에서 bit 단위로 인터럽트 발생 상태 값을 읽음
    DWORD    __stdcall AxdiInterruptFlagReadBit(long lModuleNo, long lOffset, DWORD *upValue);
    // 지정한 입력 접점 모듈, Interrupt Flag Register의 Offset 위치에서 byte 단위로 인터럽트 발생 상태 값을 읽음
    DWORD    __stdcall AxdiInterruptFlagReadByte(long lModuleNo, long lOffset, DWORD *upValue);
    // 지정한 입력 접점 모듈, Interrupt Flag Register의 Offset 위치에서 word 단위로 인터럽트 발생 상태 값을 읽음
    DWORD    __stdcall AxdiInterruptFlagReadWord(long lModuleNo, long lOffset, DWORD *upValue);
    // 지정한 입력 접점 모듈, Interrupt Flag Register의 Offset 위치에서 double word 단위로 인터럽트 발생 상태 값을 읽음
    DWORD    __stdcall AxdiInterruptFlagReadDword(long lModuleNo, long lOffset, DWORD *upValue);
    // 전체 입력 접점 모듈, Interrupt Flag Register의 Offset 위치에서 bit 단위로 인터럽트 발생 상태 값을 읽음
    DWORD    __stdcall AxdiInterruptFlagRead(long lOffset, DWORD *upValue);

    //========= 로그 관련 함수 ==========================================================================================
    // 현재 자동으로 설정됨.
    // 설정 축의 함수 실행 결과를 EzSpy에서 모니터링 할 수 있도록 설정 또는 해제하는 함수이다.
    // uUse : 사용 유무 => DISABLE(0), ENABLE(1)
    DWORD    __stdcall AxmLogSetAxis(long lAxisNo, DWORD uUse);
    
    // EzSpy에서의 설정 축 함수 실행 결과 모니터링 여부를 확인하는 함수이다.
    DWORD    __stdcall AxmLogGetAxis(long lAxisNo, DWORD *upUse);

//=========== 로그 출력 관련 함수
    //지정한 입력 채널의 EzSpy에 로그 출력 여부를 설정한다.
    DWORD    __stdcall AxaiLogSetChannel(long lChannelNo, DWORD uUse);
    //지정한 입력 채널의 EzSpy에 로그 출력 여부를 확인한다.
    DWORD    __stdcall AxaiLogGetChannel(long lChannelNo, DWORD *upUse);

//==지정한 출력 채널의 EzSpy 로그 출력 
    //지정한 출력 채널의 EzSpy에 로그 출력 여부를 설정한다.
    DWORD    __stdcall AxaoLogSetChannel(long lChannelNo, DWORD uUse);
    //지정한 출력 채널의 EzSpy에 로그 출력 여부를 확인한다.
    DWORD    __stdcall AxaoLogGetChannel(long lChannelNo, DWORD *upUse);

//==Log
    // 지정한 모듈의 EzSpy에 로그 출력 여부 설정
    DWORD    __stdcall AxdLogSetModule(long lModuleNo, DWORD uUse);
    // 지정한 모듈의 EzSpy에 로그 출력 여부 확인
    DWORD    __stdcall AxdLogGetModule(long lModuleNo, DWORD *upUse);

    // 지정한 보드가 RTEX 모드일 때 그 보드의 firmware 버전을 확인한다.
    DWORD    __stdcall AxlGetFirmwareVersion(long lBoardNo, PTCHAR szVersion);
    // 지정한 보드로 Firmware를 전송 한다.
    DWORD    __stdcall AxlSetFirmwareCopy(long lBoardNo, WORD *wData, WORD *wCmdData);
    // 지정한 보드로 Firmware Update를 수행한다.
    DWORD    __stdcall AxlSetFirmwareUpdate(long lBoardNo);
    // 지정한 보드의 현재 RTEX 초기화 상태를 확인 한다.
    DWORD    __stdcall AxlCheckStatus(long lBoardNo, DWORD* dwStatus);
    // 지정한 축에 RTEX Master board에 범용 명령을 실행 합니다.
    DWORD    __stdcall AxlRtexUniversalCmd(long lBoardNo, WORD wCmd, WORD wOffset, WORD *wData);
    // 지정한 축의 RTEX 통신 명령을 실행한다.
    DWORD    __stdcall AxmRtexSlaveCmd(long lAxisNo, DWORD dwCmdCode, DWORD dwTypeCode, DWORD dwIndexCode, DWORD dwCmdConfigure, DWORD dwValue);
    // 지정한 축에 실행한 RTEX 통신 명령의 결과값을 확인한다.
    DWORD    __stdcall AxmRtexGetSlaveCmdResult(long lAxisNo, DWORD* dwIndex, DWORD *dwValue);
    // 지정한 축에 RTEX 상태 정보를 확인한다.
    DWORD   __stdcall AxmRtexGetAxisStatus(long lAxisNo, DWORD *dwStatus);
    // 지정한 축에 RTEX 통신 리턴 정보를 확인한다.(Actual position, Velocity, Torque)
    DWORD   __stdcall AxmRtexGetAxisReturnData(long lAxisNo,  DWORD *dwReturn1, DWORD *dwReturn2, DWORD *dwReturn3);
    // 지정한 축에 RTEX Slave 축의 현재 상태 정보를 확인한다.(mechanical, Inposition and etc)
    DWORD   __stdcall AxmRtexGetAxisSlaveStatus(long lAxisNo,  DWORD *dwStatus);
    
    // 지정한 축에 MLII Slave 축에 범용 네트웍 명령어를 기입한다.
    DWORD   __stdcall AxmSetAxisCmd(long lAxisNo, DWORD* tagCommand);
    // 지정한 축에 MLII Slave 축에 범용 네트웍 명령의 결과를 확인한다.
    DWORD   __stdcall AxmGetAxisCmdResult(long lAxisNo, DWORD* tagCommand);
    
    DWORD   __stdcall AxlGetDpRamData(long lBoardNo, WORD wAddress, DWORD* dwpRdData);
    DWORD   __stdcall AxlBoardReadDpramWord(long lBoardNo, WORD wOffset, DWORD* dwpRdData);

    DWORD   __stdcall AxlSetSendBoardCommand(long lBoardNo, DWORD dwCommand, DWORD* dwpSendData, DWORD dwLength);
    DWORD   __stdcall AxlGetResponseBoardCommand(long lBoardNo, DWORD* dwpReadData);

    // Network Type Master 보드에서 Slave 들의 Firmware Version을 읽어 오는 함수.
    // ucaFirmwareVersion unsigned char 형의 Array로 선언하고 크기가 4이상이 되도록 선언 해야 한다.
    DWORD   __stdcall AxmInfoGetFirmwareVersion(long lAxisNo, unsigned char* ucaFirmwareVersion);
    DWORD   __stdcall AxaInfoGetFirmwareVersion(long lModuleNo, unsigned char* ucaFirmwareVersion);
    DWORD   __stdcall AxdInfoGetFirmwareVersion(long lModuleNo, unsigned char* ucaFirmwareVersion);
    
    //======== PCI-R1604-MLII 전용 함수=========================================================================== 
    // INTERPOLATE and LATCH Command의 Option Field의 Torq Feed Forward의 값을 설정 하도록 합니다.
    // 기본값은 MAX로 설정되어 있습니다.
    // 설정값은 0 ~ 4000H까지 설정 할 수 있습니다.
    // 설정값은 4000H이상으로 설정하면 설정은 그 이상으로 설정되나 동작은 4000H값이 적용 됩니다.
    DWORD   __stdcall AxmSetTorqFeedForward(long lAxisNo, DWORD dwTorqFeedForward);
    
    // INTERPOLATE and LATCH Command의 Option Field의 Torq Feed Forward의 값을 읽어오는 함수 입니다.
    // 기본값은 MAX로 설정되어 있습니다.
    DWORD   __stdcall AxmGetTorqFeedForward(long lAxisNo, DWORD* dwpTorqFeedForward);
    
    // INTERPOLATE and LATCH Command의 VFF Field의 Velocity Feed Forward의 값을 설정 하도록 합니다.
    // 기본값은 '0'로 설정되어 있습니다.
    // 설정값은 0 ~ FFFFH까지 설정 할 수 있습니다.
    DWORD   __stdcall AxmSetVelocityFeedForward(long lAxisNo, DWORD dwVelocityFeedForward);
    
    // INTERPOLATE and LATCH Command의 VFF Field의 Velocity Feed Forward의 값을 읽어오는 함수 입니다.
    DWORD   __stdcall AxmGetVelocityFeedForward(long lAxisNo, DWORD* dwpVelocityFeedForward);

    // Encoder type을 설정한다.
    // 기본값은 0(TYPE_INCREMENTAL)로 설정되어 있습니다.
    // 설정값은 0 ~ 1까지 설정 할 수 있습니다.
    // 설정값 : 0(TYPE_INCREMENTAL), 1(TYPE_ABSOLUTE).
    DWORD   __stdcall AxmSignalSetEncoderType(long lAxisNo, DWORD dwEncoderType);

    // Encoder type을 확인한다.
    DWORD   __stdcall AxmSignalGetEncoderType(long lAxisNo, DWORD* dwpEncoderType);
    //======================================================================================================== 

    // Slave Firmware Update를 위해 추가
    //DWORD   __stdcall AxmSetSendAxisCommand(long lAxisNo, WORD wCommand, WORD* wpSendData, WORD wLength);

    //======== PCI-R1604-RTEX, RTEX-PM 전용 함수============================================================== 
    // 범용 입력 2,3번 입력시 JOG 구동 속도를 설정한다. 
    // 구동에 관련된 모든 설정(Ex, PulseOutMethod, MoveUnitPerPulse 등)들이 완료된 이후 한번만 실행하여야 한다.
    DWORD    __stdcall AxmMotSetUserMotion(long lAxisNo, double dVelocity, double dAccel, double dDecel);

    // 범용 입력 2,3번 입력시 JOG 구동 동작 사용 가부를 설정한다.
    // 설정값 :  0(DISABLE), 1(ENABLE)
    DWORD    __stdcall AxmMotSetUserMotionUsage(long lAxisNo, DWORD dwUsage);

    // MPGP 입력을 사용하여 Load/UnLoad 위치를 자동으로 이동하는 기능 설정. 
    DWORD    __stdcall  AxmMotSetUserPosMotion(long lAxisNo, double dVelocity, double dAccel, double dDecel, double dLoadPos, double dUnLoadPos, DWORD dwFilter, DWORD dwDelay);

    // MPGP 입력을 사용하여 Load/UnLoad 위치를 자동으로 이동하는 기능 설정. 
    // 설정값 :  0(DISABLE), 1(Position 기능 A 사용), 2(Position 기능 B 사용)
    DWORD    __stdcall  AxmMotSetUserPosMotionUsage(long lAxisNo, DWORD dwUsage);
    //======================================================================================================== 

#ifdef __cplusplus
}
#endif    //__cplusplus

#endif    //__AXT_AXDEV_H__
