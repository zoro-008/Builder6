//---------------------------------------------------------------------------

#ifndef SInterfaceUnitH
#define SInterfaceUnitH
//---------------------------------------------------------------------------

//#include <windows.h>
#include <ExtCtrls.hpp>

#include "SMDllDefine.h"

extern "C" {

    /////////////
    //Main Form//
    /////////////
    __declspec(dllexport) void __stdcall FM_MakeWin(TPanel * Panel);
    __declspec(dllexport) void __stdcall FM_SetWin (TPanel * Panel);
    __declspec(dllexport) void __stdcall FM_DelWin (              );

    /////////////
    //Sub Form //
    /////////////
    __declspec(dllexport) void       __stdcall FM_MsgOk        (AnsiString _sCaption , AnsiString _sMessage);
    __declspec(dllexport) void       __stdcall FM_MsgOkModal   (AnsiString _sCaption , AnsiString _sMessage);
    __declspec(dllexport) AnsiString __stdcall FM_GetMsgOkModal(void);
    __declspec(dllexport) void       __stdcall FM_CloseMsgOk   (void);


    __declspec(dllexport) int        __stdcall FM_MsgYesNo     (AnsiString _sCaption , AnsiString _sMessage);
    __declspec(dllexport) AnsiString __stdcall FM_GetMsgYesNo  (void);
    __declspec(dllexport) void       __stdcall FM_CloseMsgYesNo(void);


    //////////////////
    //Password Form //
    //////////////////
    __declspec(dllexport) int      __stdcall FM_PswdShow(EN_LEVEL _iLevel );
    __declspec(dllexport) EN_LEVEL __stdcall FM_GetLevel();
    __declspec(dllexport) EN_LEVEL __stdcall FM_SetLevel(EN_LEVEL _iSelLevel);

    //////////////////
    //Password Form //
    //////////////////
    __declspec(dllexport) void     __stdcall SM_Update  (EN_SEQ_STAT _iStat);



    ////////////////
    //Motor Class.//
    ////////////////
    __declspec(dllexport) void __stdcall MT_SetHomeEnd    (int _iAxisNo , bool _bValue ); // 해당축을 강제로 홈엔드혹은 홈엔드 해제 시킨다.
    __declspec(dllexport) bool __stdcall MT_GetHomeEnd    (int _iAxisNo );                // 해당축의 홈엔드를 확인 한다.
    __declspec(dllexport) bool __stdcall MT_GetHomeEndAll ();                             // 모든축의 홈엔드를 확인 한다.
    __declspec(dllexport) void __stdcall MT_SetServo      (int _iAxisNo , bool _bOn    ); // 해당축의 서보를 On/Off 시킨다.
    __declspec(dllexport) bool __stdcall MT_GetServo      (int _iAxisNo );
    __declspec(dllexport) bool __stdcall MT_GetServoAll   ();
    __declspec(dllexport) void __stdcall MT_SetServoAll   (               bool _bOn    ); // 모든축의 서보온/오프
    __declspec(dllexport) void __stdcall MT_SetReset      (int _iAxisNo , bool _bOn    ); // 서보팩에 연결된 하드웨어 리셑 시그널.
    __declspec(dllexport) bool __stdcall MT_GetAlarm      (int _iAxisNo );
    __declspec(dllexport) bool __stdcall MT_GetHomeSnsr   (int _iAxisNo );
    __declspec(dllexport) bool __stdcall MT_GetPLimSnsr   (int _iAxisNo );
    __declspec(dllexport) bool __stdcall MT_GetNLimSnsr   (int _iAxisNo );
    __declspec(dllexport) bool __stdcall MT_GetStopInpos  (int _iAxisNo ); //Check Motion Done with User Inposition.
    __declspec(dllexport) bool __stdcall MT_GetStop       (int _iAxisNo ); //Check Motion Done with User Inposition.

    //Position Functions.
    __declspec(dllexport) double __stdcall MT_GetTrgPos(int _iAxisNo );                       //Get Target  Position.
    __declspec(dllexport) double __stdcall MT_GetCmdPos(int _iAxisNo );                       //Get Command Position.
    __declspec(dllexport) double __stdcall MT_GetEncPos(int _iAxisNo );                       //Get Encoder Position.
    __declspec(dllexport) double __stdcall MT_GetErrPos(int _iAxisNo );                       //Get Control Error Position.
    __declspec(dllexport) void   __stdcall MT_ClearPos (int _iAxisNo );                       //Clear Position.
    __declspec(dllexport) void   __stdcall MT_SetPos   (int _iAxisNo , double _dPos );        //Set Any Position.
    __declspec(dllexport) bool   __stdcall MT_CmprPos  (int _iAxisNo , double _dPos );

    //Motion Functions.
    __declspec(dllexport) void __stdcall MT_Reset    (int _iAxisNo); //Reset
    __declspec(dllexport) void __stdcall MT_ResetAll (            );
    __declspec(dllexport) void __stdcall MT_Stop     (int _iAxisNo);
    __declspec(dllexport) void __stdcall MT_EmgStop  (int _iAxisNo); //Stop Without Deceleration.

    __declspec(dllexport) bool __stdcall MT_Home     (int _iAxisNo); //Start Home Cycle for Sequense Cycle.
    __declspec(dllexport) void __stdcall MT_DoHome   (int _iAxisNo); //Start Home Cycle for Click Event.
    __declspec(dllexport) void __stdcall MT_DoRpt    (int _iAxisNo); //Start Home Cycle for Click Event.

    __declspec(dllexport) bool __stdcall MT_JogP     (int _iAxisNo); //Jogging to CW.
    __declspec(dllexport) bool __stdcall MT_JogN     (int _iAxisNo); //Jogging to CCW.

    __declspec(dllexport) bool __stdcall MT_GoAbs    (int _iAxisNo , double _dPos , double _dVel);  //abs move
    __declspec(dllexport) void __stdcall MT_GoInc    (int _iAxisNo , double _dPos , double _dVel);  //Inc move  모션돈 확인은 GetStopInpos() 함수로.
    __declspec(dllexport) bool __stdcall MT_GoAbsRun (int _iAxisNo , double _dPos );  //abs move with AutoRun Vel.
    __declspec(dllexport) void __stdcall MT_GoIncRun (int _iAxisNo , double _dPos );  //Inc move with AutoRun ManualVel. 모션돈 확인은 GetStopInpos() 함수로.
    __declspec(dllexport) bool __stdcall MT_GoAbsMan (int _iAxisNo , double _dPos );  //abs move with Manual Vel.
    __declspec(dllexport) void __stdcall MT_GoIncMan (int _iAxisNo , double _dPos );  //Inc move with Manual Vel. 모션돈 확인은 GetStopInpos() 함수로.

    __declspec(dllexport) AnsiString __stdcall MT_GetName(int _iAxisNo);
    __declspec(dllexport) int        __stdcall MT_GetMotorCnt ();

    //Trriger Func For AxtFS20 Only.                                                                      //마이크로 Sec Max 4000
    __declspec(dllexport) void __stdcall MT_SetAxtTrgPos  (int _iAxisNo , int _iPosCnt , double * _dPos , double _dTrgTime , bool _bActual ,bool _bOnLevel);
    __declspec(dllexport) void __stdcall MT_ResetAxtTrgPos(int _iAxisNo );
    __declspec(dllexport) void __stdcall MT_OneShotAxtTrg (int _iAxisNo , bool _bUpEg , int _iTime);


    ///////////////////
    //Actuator Class.//
    ///////////////////
    //ResetAll -> Reset
    //register 다 지우기.
    __declspec(dllexport) void       __stdcall AT_Reset    (void                 );
    __declspec(dllexport) bool       __stdcall AT_Err      (int _iNo             );
    __declspec(dllexport) bool       __stdcall AT_Complete (int _iNo , bool _bCmd);
    __declspec(dllexport) bool       __stdcall AT_Done     (int _iNo             );
    __declspec(dllexport) AnsiString __stdcall AT_GetName  (int _iNo             );
    __declspec(dllexport) bool       __stdcall AT_MoveCyl  (int _iNo , bool _bCmd);
    __declspec(dllexport) bool       __stdcall AT_GetCmd   (int _iNo             ); 



    /////////////
    //IO Class.//
    /////////////
    __declspec(dllexport) void __stdcall IO_SetY  (int _iNo , bool _bVal   ); //Direct = true일경우 1싸이클 동안 엣지는 못본다.
    __declspec(dllexport) bool __stdcall IO_GetY  (int _iNo );
    __declspec(dllexport) bool __stdcall IO_GetYDn(int _iNo );
    __declspec(dllexport) bool __stdcall IO_GetYUp(int _iNo );

    __declspec(dllexport) bool __stdcall IO_GetX  (int _iNo , bool _bDirect = false); //Direct = true일경우 1싸이클 동안 엣지는 못본다.
    __declspec(dllexport) bool __stdcall IO_GetXDn(int _iNo );
    __declspec(dllexport) bool __stdcall IO_GetXUp(int _iNo );        


    //////////////
    //ERR Class.//
    //////////////                    
    __declspec(dllexport) void       __stdcall EM_Clear         (void     ); //모든에러 클리어 시키기.
    __declspec(dllexport) void       __stdcall EM_SetErr        (int  _iNo); //해당 에러 true 시키기.
    __declspec(dllexport) void       __stdcall EM_SetDisp       (bool _bOn); //m_bShowForm 를 false 시킨다.

    __declspec(dllexport) bool       __stdcall EM_IsErr         (void    ); //에러중인지 확인.
    __declspec(dllexport) int        __stdcall EM_GetLastErr    (        ); //멘마지막에 Set된 에러 번호 리턴.
    __declspec(dllexport) AnsiString __stdcall EM_GetLastErrName(        );
    __declspec(dllexport) AnsiString __stdcall EM_GetErrName    (int _iNo);

    __declspec(dllexport) void       __stdcall EM_DisplayList   (TListBox * _lbList);

    ////////////////////
    //TowerLamp Class.//
    ////////////////////
    __declspec(dllexport) void       __stdcall TL_MuteSnd       (); //해당 에러 true 시키기.




};

#endif
