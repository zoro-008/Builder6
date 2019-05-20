//---------------------------------------------------------------------------

#ifndef SInterfaceUnitH
#define SInterfaceUnitH
//---------------------------------------------------------------------------

#include "SMDllDefine.h"


/////////////
//Main Form//
/////////////
typedef void       (__stdcall * PFM_MakeWin)(              );
typedef void       (__stdcall * PFM_SetWin )(void * hWnd   );
typedef void       (__stdcall * PFM_DelWin )(              );
typedef void       (__stdcall * PFM_HideWin)(              );
typedef char *     (__stdcall * PFM_GetVer )(              ); //버그 있음. 고처야함. 픽스함 2011.11.8 선계원.
typedef char *     (__stdcall * PFM_GetAge )(              );
typedef EN_LAN_SEL (__stdcall * PFM_GetLan )(              );
extern PFM_MakeWin FM_MakeWin ;
extern PFM_SetWin  FM_SetWin  ;
extern PFM_DelWin  FM_DelWin  ;
extern PFM_HideWin FM_HideWin ;
extern PFM_GetVer  FM_GetVer  ;
extern PFM_GetAge  FM_GetAge  ;
extern PFM_GetLan  FM_GetLan  ;

/////////////
//Sub Form //
/////////////
typedef void (__stdcall * PFM_MsgOk     )(char *     _sCaption , char *     _sMessage);
typedef void (__stdcall * PFM_MsgOkModal)(char *     _sCaption , char *     _sMessage);
typedef int  (__stdcall * PFM_MsgYesNo  )(char *     _sCaption , char *     _sMessage);
typedef void (__stdcall * PFM_MsgTime   )(char *     _sCaption , char *     _sMessage , int _iShowTime);
typedef void (__stdcall * PFM_CloseMsgOk)();
extern PFM_MsgOk      FM_MsgOk      ;
extern PFM_MsgOkModal FM_MsgOkModal ;
extern PFM_MsgYesNo   FM_MsgYesNo   ;
extern PFM_MsgTime    FM_MsgTime    ;
extern PFM_CloseMsgOk FM_CloseMsgOk ;

//////////////////
//Password Form //
//////////////////
//EN_LEVEL
typedef int      (__stdcall * PFM_PswdShow)(EN_LEVEL _iLevel );
typedef EN_LEVEL (__stdcall * PFM_GetLevel)();
typedef EN_LEVEL (__stdcall * PFM_SetLevel)(EN_LEVEL _iSelLevel);
extern PFM_PswdShow FM_PswdShow ;
extern PFM_GetLevel FM_GetLevel ;
extern PFM_SetLevel FM_SetLevel ;

//////////////////
//Sequence      //
//////////////////
typedef void     (__stdcall * PSM_Update  )(EN_SEQ_STAT _iStat);
extern PSM_Update SM_Update ;

////////////////
//Motor Class.//
////////////////
typedef void (__stdcall * PMT_SetHomeEnd    )(int _iAxisNo , bool _bValue ); // 해당축을 강제로 홈엔드혹은 홈엔드 해제 시킨다.
typedef bool (__stdcall * PMT_GetHomeEnd    )(int _iAxisNo );                // 해당축의 홈엔드를 확인 한다.
typedef bool (__stdcall * PMT_GetHomeEndAll )();                             // 모든축의 홈엔드를 확인 한다.
typedef void (__stdcall * PMT_SetServo      )(int _iAxisNo , bool _bOn    ); // 해당축의 서보를 On/Off 시킨다.
typedef bool (__stdcall * PMT_GetServo      )(int _iAxisNo );
typedef bool (__stdcall * PMT_GetServoAll   )();
typedef void (__stdcall * PMT_SetServoAll   )(               bool _bOn    ); // 모든축의 서보온/오프
typedef void (__stdcall * PMT_SetReset      )(int _iAxisNo , bool _bOn    ); // 서보팩에 연결된 하드웨어 리셑 시그널.
typedef bool (__stdcall * PMT_GetAlarm      )(int _iAxisNo );
typedef bool (__stdcall * PMT_GetHomeSnsr   )(int _iAxisNo );
typedef bool (__stdcall * PMT_GetPLimSnsr   )(int _iAxisNo );
typedef bool (__stdcall * PMT_GetNLimSnsr   )(int _iAxisNo );
typedef bool (__stdcall * PMT_GetStopInpos  )(int _iAxisNo ); //Check Motion Done with User Inposition.
typedef bool (__stdcall * PMT_GetStop       )(int _iAxisNo ); //Check Motion Done with User Inposition.
typedef bool (__stdcall * PMT_GetZphaseSnsr )(int _iAxisNo ); //
extern PMT_SetHomeEnd     MT_SetHomeEnd    ;
extern PMT_GetHomeEnd     MT_GetHomeEnd    ;
extern PMT_GetHomeEndAll  MT_GetHomeEndAll ;
extern PMT_SetServo       MT_SetServo      ;
extern PMT_GetServo       MT_GetServo      ;
extern PMT_GetServoAll    MT_GetServoAll   ;
extern PMT_SetServoAll    MT_SetServoAll   ;
extern PMT_SetReset       MT_SetReset      ;
extern PMT_GetAlarm       MT_GetAlarm      ;
extern PMT_GetHomeSnsr    MT_GetHomeSnsr   ;
extern PMT_GetPLimSnsr    MT_GetPLimSnsr   ;
extern PMT_GetNLimSnsr    MT_GetNLimSnsr   ;
extern PMT_GetStopInpos   MT_GetStopInpos  ;
extern PMT_GetStop        MT_GetStop       ;
extern PMT_GetZphaseSnsr  MT_GetZphaseSnsr ;

//Position Functions.
typedef double (__stdcall * PMT_GetTrgPos)(int _iAxisNo );                       //Get Target  Position.
typedef double (__stdcall * PMT_GetCmdPos)(int _iAxisNo );                       //Get Command Position.
typedef double (__stdcall * PMT_GetEncPos)(int _iAxisNo );                       //Get Encoder Position.
typedef double (__stdcall * PMT_GetErrPos)(int _iAxisNo );                       //Get Control Error Position.
typedef void   (__stdcall * PMT_ClearPos )(int _iAxisNo );                       //Clear Position.
typedef void   (__stdcall * PMT_SetPos   )(int _iAxisNo , double _dPos );        //Set Any Position.
typedef bool   (__stdcall * PMT_CmprPos  )(int _iAxisNo , double _dPos );
/*20121123추가*/
typedef double (__stdcall * PMT_GetMinPos)(int _iAxisNo );                       //Get Setted Min Pos
typedef double (__stdcall * PMT_GetMaxPos)(int _iAxisNo );                       //Get Setted Max Pos
typedef double (__stdcall * PMT_GetRunVel)(int _iAxisNo );                       //Get Setted Run Vel
typedef double (__stdcall * PMT_GetManVel)(int _iAxisNo );                       //Get Setted Man Vel
extern PMT_GetTrgPos MT_GetTrgPos ;
extern PMT_GetCmdPos MT_GetCmdPos ;
extern PMT_GetEncPos MT_GetEncPos ;
extern PMT_GetErrPos MT_GetErrPos ;
extern PMT_ClearPos  MT_ClearPos  ;
extern PMT_SetPos    MT_SetPos    ;
extern PMT_CmprPos   MT_CmprPos   ;
extern PMT_GetMinPos MT_GetMinPos ;
extern PMT_GetMaxPos MT_GetMaxPos ;
extern PMT_GetRunVel MT_GetRunVel ;
extern PMT_GetManVel MT_GetManVel ;

//Motion Functions.
typedef void (__stdcall * PMT_Reset    )(int _iAxisNo); //Reset
typedef void (__stdcall * PMT_ResetAll )(            );
typedef void (__stdcall * PMT_Stop     )(int _iAxisNo);
typedef void (__stdcall * PMT_EmgStop  )(int _iAxisNo); //Stop Without Deceleration.
extern PMT_Reset    MT_Reset    ;
extern PMT_ResetAll MT_ResetAll ;
extern PMT_Stop     MT_Stop     ;
extern PMT_EmgStop  MT_EmgStop  ;

typedef bool (__stdcall * PMT_Home      )(int _iAxisNo); //Start Home Cycle for Sequense Cycle.
typedef void (__stdcall * PMT_DoHome    )(int _iAxisNo); //Start Home Cycle for Click Event.
typedef void (__stdcall * PMT_DoRpt     )(int _iAxisNo); //Start Home Cycle for Click Event.
extern PMT_Home       MT_Home       ;
extern PMT_DoHome     MT_DoHome     ;
extern PMT_DoRpt      MT_DoRpt      ;

typedef bool (__stdcall * PMT_JogP     )(int _iAxisNo); //Jogging to CW.
typedef bool (__stdcall * PMT_JogN     )(int _iAxisNo); //Jogging to CCW.
extern PMT_JogP MT_JogP ;
extern PMT_JogN MT_JogN ;

typedef bool (__stdcall * PMT_GoAbs    )(int _iAxisNo , double _dPos , double _dVel);  //abs move
typedef void (__stdcall * PMT_GoInc    )(int _iAxisNo , double _dPos , double _dVel);  //Inc move  모션돈 확인은 GetStopInpos() 함수로.
typedef bool (__stdcall * PMT_GoAbsRun )(int _iAxisNo , double _dPos );  //abs move with AutoRun Vel.
typedef void (__stdcall * PMT_GoIncRun )(int _iAxisNo , double _dPos );  //Inc move with AutoRun ManualVel. 모션돈 확인은 GetStopInpos() 함수로.
typedef bool (__stdcall * PMT_GoAbsSlow)(int _iAxisNo , double _dPos );  //abs move with AutoRun Vel.
typedef void (__stdcall * PMT_GoIncSlow)(int _iAxisNo , double _dPos );  //Inc move with AutoRun ManualVel. 모션돈 확인은 GetStopInpos() 함수로.
typedef bool (__stdcall * PMT_GoAbsMan )(int _iAxisNo , double _dPos );  //abs move with Manual Vel.
typedef void (__stdcall * PMT_GoIncMan )(int _iAxisNo , double _dPos );  //Inc move with Manual Vel. 모션돈 확인은 GetStopInpos() 함수로.
/*20121123추가*/
typedef bool (__stdcall * PMT_GoAbsAcDc)(int _iAxisNo , double _dPos , double _dVel , double _dAcc , double _dDcc);  //abs move
typedef void (__stdcall * PMT_GoIncAcDc)(int _iAxisNo , double _dPos , double _dVel , double _dAcc , double _dDcc);  //Inc move  모션돈 확인은 GetStopInpos() 함수로.
extern PMT_GoAbs     MT_GoAbs     ;
extern PMT_GoInc     MT_GoInc     ;
extern PMT_GoAbsRun  MT_GoAbsRun  ;
extern PMT_GoIncRun  MT_GoIncRun  ;
extern PMT_GoAbsSlow MT_GoAbsSlow ;
extern PMT_GoIncSlow MT_GoIncSlow ;
extern PMT_GoAbsMan  MT_GoAbsMan  ;
extern PMT_GoIncMan  MT_GoIncMan  ;
extern PMT_GoAbsAcDc MT_GoAbsAcDc ;
extern PMT_GoIncAcDc MT_GoIncAcDc ;

typedef char *            (__stdcall * PMT_GetName     )(int _iAxisNo);
typedef int               (__stdcall * PMT_GetMotorCnt )();
typedef EN_MTOR_DIRECTION (__stdcall * PMT_GetDirType  )(int _iAxisNo);
extern PMT_GetName     MT_GetName     ;
extern PMT_GetMotorCnt MT_GetMotorCnt ;
extern PMT_GetDirType  MT_GetDirType  ;

//Trriger Func For AxtFS20 Only. AXL은 검증 해봐야함.                                                             //마이크로 Sec Max 4000
typedef void (__stdcall * PMT_SetAxtTrgPos  )(int _iAxisNo , int _iPosCnt , double * _dPos , double _dTrgTime , bool _bActual ,bool _bOnLevel);
typedef void (__stdcall * PMT_ResetAxtTrgPos)(int _iAxisNo );
typedef void (__stdcall * PMT_OneShotAxtTrg )(int _iAxisNo , bool _bUpEg , int _iTime);
extern PMT_SetAxtTrgPos   MT_SetAxtTrgPos   ;
extern PMT_ResetAxtTrgPos MT_ResetAxtTrgPos ;
extern PMT_OneShotAxtTrg  MT_OneShotAxtTrg  ;

typedef void (__stdcall * PMT_SetIntrptPos  )(int _iAxisNo , double _dPos, PosCallbackFunc _pCallback);
typedef void (__stdcall * PMT_ResetIntrpt   )(int _iAxisNo);
extern PMT_SetIntrptPos MT_SetIntrptPos ;
extern PMT_ResetIntrpt  MT_ResetIntrpt  ;

typedef void (__stdcall * PMT_ContiWriteClear   )(int _iCoord);
typedef void (__stdcall * PMT_ContiSetAxisMap   )(int _iCoord, int _iPosSize, int * _ipAxisNo);
typedef void (__stdcall * PMT_ContiSetAbsRelMode)(int _iCoord, DWORD _dwAbsRelMode);
typedef void (__stdcall * PMT_ContiBeginNode    )(int _iCoord);
typedef void (__stdcall * PMT_ContiEndNode      )(int _iCoord);
typedef void (__stdcall * PMT_ContiStart        )(int _iCoord, DWORD _dwProfileset, int _iAngle);
typedef void (__stdcall * PMT_LineMove          )(int _iCoord, int *_ipAxisNo, int _iPosSize, double *_dpEndPos, double _dVel, double _dAccel, double _dDecel);
typedef void (__stdcall * PMT_CircleCenterMove  )(int _iCoord, int *_ipAxisNo, int _iPosSize, double *_dCenterPos, double *_dpEndPos, double _dVel, double _dAccel, double _dDecel, DWORD _dwCWDir);

// JH_
extern PMT_ContiWriteClear    JH_MT_WriteClear         ;
extern PMT_ContiSetAxisMap    JH_MT_ContiSetAxisMap    ;
extern PMT_ContiSetAbsRelMode JH_MT_ContiSetAbsRelMode ;
extern PMT_ContiBeginNode     JH_MT_ContiBeginNode     ;
extern PMT_ContiEndNode       JH_MT_ContiEndNode       ;
extern PMT_ContiStart         JH_MT_ContiStart         ;
extern PMT_LineMove           JH_MT_LineMove           ;
extern PMT_CircleCenterMove   JH_MT_CircleCenterMove   ;


///////////////////
//Actuator Class.//
///////////////////
//ResetAll -> Reset
//register 다 지우기.
typedef void               (__stdcall * PAT_Reset       )(void                                                        );
typedef bool               (__stdcall * PAT_Err         )(int _iNo                                                    );
typedef bool               (__stdcall * PAT_Complete    )(int _iNo , bool _bCmd                                       );
typedef bool               (__stdcall * PAT_Done        )(int _iNo                                                    );
typedef char *             (__stdcall * PAT_GetName     )(int _iNo                                                    );
typedef bool               (__stdcall * PAT_MoveCyl     )(int _iNo , bool _bCmd                                       );
typedef bool               (__stdcall * PAT_GetCmd      )(int _iNo                                                    );
typedef int                (__stdcall * PAT_GetActCnt   )(void                                                        );
typedef EN_ACTR_DIRECTION  (__stdcall * PAT_GetDirType  )(int _iNo                                                    );
extern PAT_Reset      AT_Reset      ;
extern PAT_Err        AT_Err        ;
extern PAT_Complete   AT_Complete   ;
extern PAT_Done       AT_Done       ;
extern PAT_GetName    AT_GetName    ;
extern PAT_MoveCyl    AT_MoveCyl    ;
extern PAT_GetCmd     AT_GetCmd     ;
extern PAT_GetActCnt  AT_GetActCnt  ;
extern PAT_GetDirType AT_GetDirType ;

/////////////
//DIO Class.//
/////////////
typedef void (__stdcall * PIO_SetY    )(int _iNo , bool _bVal); //Direct = true일경우 1싸이클 동안 엣지는 못본다.
typedef void (__stdcall * PIO_SetYDrct)(int _iNo , bool _bVal); //Direct = true일경우 1싸이클 동안 엣지는 못본다.
typedef bool (__stdcall * PIO_GetY    )(int _iNo );
typedef bool (__stdcall * PIO_GetYDn  )(int _iNo );
typedef bool (__stdcall * PIO_GetYUp  )(int _iNo );
extern PIO_SetY   IO_SetY   ;
extern PIO_GetY   IO_GetY   ;
extern PIO_GetYDn IO_GetYDn ;
extern PIO_GetYUp IO_GetYUp ;

typedef bool (__stdcall * PIO_GetX    )(int _iNo ); //Direct = true일경우 1싸이클 동안 엣지는 못본다.
typedef bool (__stdcall * PIO_GetXDrct)(int _iNo ); //Direct = true일경우 1싸이클 동안 엣지는 못본다.
typedef bool (__stdcall * PIO_GetXDn  )(int _iNo );
typedef bool (__stdcall * PIO_GetXUp  )(int _iNo );
extern PIO_GetX   IO_GetX   ;
extern PIO_GetXDn IO_GetXDn ;
extern PIO_GetXUp IO_GetXUp ;

typedef void (__stdcall * PIO_SetTestMode)(bool _bOn) ;
typedef bool (__stdcall * PIO_GetTestMode)(         ) ;
extern PIO_SetTestMode IO_SetTestMode ;
extern PIO_GetTestMode IO_GetTestMode ;

typedef char * (__stdcall * PIO_GetXName)(int _iNo ) ;
typedef char * (__stdcall * PIO_GetYName)(int _iNo ) ;
extern PIO_GetXName IO_GetXName ;
extern PIO_GetYName IO_GetYName ;

//////////////
//ERR Class.//
//////////////
typedef void       (__stdcall * PEM_Clear         )(void                           ); //모든에러 클리어 시키기.
typedef void       (__stdcall * PEM_SetErr        )(int  _iNo                      ); //해당 에러 true 시키기.
typedef void       (__stdcall * PEM_SetDisp       )(bool _bOn                      ); //m_bShowForm 를 false 시킨다.
typedef void       (__stdcall * PEM_SetErrMsg     )(int  _iNo , char * _sErrMsg);
typedef char *     (__stdcall * PEM_GetErrMsg     )(int  _iNo                  );

extern PEM_Clear     EM_Clear     ;
extern PEM_SetErr    EM_SetErr    ;
extern PEM_SetDisp   EM_SetDisp   ;
extern PEM_SetErrMsg EM_SetErrMsg ;
extern PEM_GetErrMsg EM_GetErrMsg ;

typedef bool       (__stdcall * PEM_IsErr         )(void    ); //에러중인지 확인.
typedef int        (__stdcall * PEM_GetLastErr    )(        ); //멘마지막에 Set된 에러 번호 리턴.
typedef char *     (__stdcall * PEM_GetLastErrName)(        );
typedef char *     (__stdcall * PEM_GetErrName    )(int _iNo);
typedef char *     (__stdcall * PEM_GetErrAction  )(int _iNo);
extern PEM_IsErr          EM_IsErr          ;
extern PEM_GetLastErr     EM_GetLastErr     ;
extern PEM_GetLastErrName EM_GetLastErrName ;
extern PEM_GetErrName     EM_GetErrName     ;
extern PEM_GetErrAction   EM_GetErrAction   ;

typedef int        (__stdcall * PEM_GetErrCnt     )(        );
typedef int        (__stdcall * PEM_GetMaxErr     )(        );
typedef bool       (__stdcall * PEM_GetErr        )(int _iNo);
extern PEM_GetErrCnt      EM_GetErrCnt ;
extern PEM_GetMaxErr      EM_GetMaxErr ;
extern PEM_GetErr         EM_GetErr    ;




//정규화.. 삭제...
//typedef void       (__stdcall * PEM_DisplayList   )(TListBox * _lbList);

////////////////////
//TowerLamp Class.//
////////////////////
typedef void       (__stdcall * PTL_MuteSnd       )(bool _bOn); //타워램프 소리 끄기.
extern PTL_MuteSnd TL_MuteSnd ;

//////////////
//AIO Class.//
//////////////  2012.11.23 추가.선계원.
typedef void      (__stdcall * PAI_SetY  )(int _iNo , double _dVal );
typedef double    (__stdcall * PAI_GetY  )(int _iNo );
typedef double    (__stdcall * PAI_GetX  )(int _iNo );
extern PAI_SetY AI_SetY ;
extern PAI_GetY AI_GetY ;
extern PAI_GetX AI_GetX ;




//#ifdef UNICODE
    //Visual Studio 2008
//    bool LoadSML_W(LPCWSTR _pFileName);
//#else
    //builder 6
    bool LoadSML(char * _pFileName);
//#endif // !UNICODE

bool FreeSML();

#endif
