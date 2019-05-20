#ifndef SMDLLH
#define SMDLLH

#define SML_API		__declspec(dllexport)
#include "SMDllDefine.h"


extern "C" {

    /////////////
    //Main Form//
    /////////////
    SML_API void       __stdcall FM_MakeWin(             );
    SML_API void       __stdcall FM_SetWin (void * _hWnd );
    SML_API void       __stdcall FM_DelWin (             );
    SML_API void       __stdcall FM_HideWin(             );
    SML_API char *     __stdcall FM_GetVer (             ); //버그 있음. 고처야함. 픽스함 2011.11.8 선계원.
    SML_API char *     __stdcall FM_GetAge (             );
    SML_API EN_LAN_SEL __stdcall FM_GetLan (             );


    /////////////
    //Sub Form //
    /////////////
    SML_API void __stdcall FM_MsgOk     (char *     _sCaption , char *     _sMessage);
    SML_API void __stdcall FM_MsgOkModal(char *     _sCaption , char *     _sMessage);
    SML_API int  __stdcall FM_MsgYesNo  (char *     _sCaption , char *     _sMessage);
    SML_API void __stdcall FM_MsgTime   (char *     _sCaption , char *     _sMessage , int _iShowTime);
    SML_API void __stdcall FM_CloseMsgOk();

    //////////////////
    //Password Form //
    //////////////////
    SML_API int      __stdcall FM_PswdShow(EN_LEVEL _iLevel );
    SML_API EN_LEVEL __stdcall FM_GetLevel();
    SML_API EN_LEVEL __stdcall FM_SetLevel(EN_LEVEL _iSelLevel);

    //////////////////
    //Sequence      //
    //////////////////
    SML_API void     __stdcall SM_Update  (EN_SEQ_STAT _iStat);



    ////////////////
    //Motor Class.//
    ////////////////
    SML_API void __stdcall MT_SetHomeEnd    (int _iAxisNo , bool _bValue ); // 해당축을 강제로 홈엔드혹은 홈엔드 해제 시킨다.
    SML_API bool __stdcall MT_GetHomeEnd    (int _iAxisNo );                // 해당축의 홈엔드를 확인 한다.
    SML_API bool __stdcall MT_GetHomeEndAll ();                             // 모든축의 홈엔드를 확인 한다.
    SML_API void __stdcall MT_SetServo      (int _iAxisNo , bool _bOn    ); // 해당축의 서보를 On/Off 시킨다.
    SML_API bool __stdcall MT_GetServo      (int _iAxisNo );
    SML_API bool __stdcall MT_GetServoAll   ();
    SML_API void __stdcall MT_SetServoAll   (               bool _bOn    ); // 모든축의 서보온/오프
    SML_API void __stdcall MT_SetReset      (int _iAxisNo , bool _bOn    ); // 서보팩에 연결된 하드웨어 리셑 시그널.
    SML_API bool __stdcall MT_GetAlarm      (int _iAxisNo );
    SML_API bool __stdcall MT_GetHomeSnsr   (int _iAxisNo );
    SML_API bool __stdcall MT_GetPLimSnsr   (int _iAxisNo );
    SML_API bool __stdcall MT_GetNLimSnsr   (int _iAxisNo );
    SML_API bool __stdcall MT_GetStopInpos  (int _iAxisNo ); //Check Motion Done with User Inposition.
    SML_API bool __stdcall MT_GetStop       (int _iAxisNo ); //Check Motion Done with User Inposition.
    SML_API bool __stdcall MT_GetZphaseSnsr (int _iAxisNo ); //

    //Position Functions.
    SML_API double __stdcall MT_GetTrgPos(int _iAxisNo );                       //Get Target  Position.
    SML_API double __stdcall MT_GetCmdPos(int _iAxisNo );                       //Get Command Position.
    SML_API double __stdcall MT_GetEncPos(int _iAxisNo );                       //Get Encoder Position.
    SML_API double __stdcall MT_GetErrPos(int _iAxisNo );                       //Get Control Error Position.
    SML_API void   __stdcall MT_ClearPos (int _iAxisNo );                       //Clear Position.
    SML_API void   __stdcall MT_SetPos   (int _iAxisNo , double _dPos );        //Set Any Position.
    SML_API bool   __stdcall MT_CmprPos  (int _iAxisNo , double _dPos );
    /*20121123추가*/
    SML_API double __stdcall MT_GetMinPos(int _iAxisNo );                       //Get Setted Min Pos
    SML_API double __stdcall MT_GetMaxPos(int _iAxisNo );                       //Get Setted Max Pos
    SML_API double __stdcall MT_GetRunVel(int _iAxisNo );                       //Get Setted Run Vel
    SML_API double __stdcall MT_GetManVel(int _iAxisNo );                       //Get Setted Man Vel


    //Motion Functions.
    SML_API void __stdcall MT_Reset    (int _iAxisNo); //Reset
    SML_API void __stdcall MT_ResetAll (            );
    SML_API void __stdcall MT_Stop     (int _iAxisNo);
    SML_API void __stdcall MT_EmgStop  (int _iAxisNo); //Stop Without Deceleration.

    SML_API bool __stdcall MT_Home      (int _iAxisNo); //Start Home Cycle for Sequense Cycle.
    SML_API void __stdcall MT_DoHome    (int _iAxisNo); //Start Home Cycle for Click Event.
    SML_API void __stdcall MT_SetSubHome(int _iAxisNo , bool _bOn); //Start Home Cycle for Click Event.
    SML_API void __stdcall MT_DoRpt     (int _iAxisNo); //Start Home Cycle for Click Event.

    SML_API bool __stdcall MT_JogP     (int _iAxisNo); //Jogging to CW.
    SML_API bool __stdcall MT_JogN     (int _iAxisNo); //Jogging to CCW.

    SML_API bool __stdcall MT_GoAbs    (int _iAxisNo , double _dPos , double _dVel);  //abs move
    SML_API void __stdcall MT_GoInc    (int _iAxisNo , double _dPos , double _dVel);  //Inc move  모션돈 확인은 GetStopInpos() 함수로.
    SML_API bool __stdcall MT_GoAbsRun (int _iAxisNo , double _dPos );  //abs move with AutoRun Vel.
    SML_API void __stdcall MT_GoIncRun (int _iAxisNo , double _dPos );  //Inc move with AutoRun ManualVel. 모션돈 확인은 GetStopInpos() 함수로.
    SML_API bool __stdcall MT_GoAbsSlow(int _iAxisNo , double _dPos );  //abs move with AutoRun Vel.
    SML_API void __stdcall MT_GoIncSlow(int _iAxisNo , double _dPos );  //Inc move with AutoRun ManualVel. 모션돈 확인은 GetStopInpos() 함수로.
    SML_API bool __stdcall MT_GoAbsMan (int _iAxisNo , double _dPos );  //abs move with Manual Vel.
    SML_API void __stdcall MT_GoIncMan (int _iAxisNo , double _dPos );  //Inc move with Manual Vel. 모션돈 확인은 GetStopInpos() 함수로.
    /*20121123추가*/
    SML_API bool __stdcall MT_GoAbsAcDc(int _iAxisNo , double _dPos , double _dVel , double _dAcc , double _dDcc);  //abs move
    SML_API void __stdcall MT_GoIncAcDc(int _iAxisNo , double _dPos , double _dVel , double _dAcc , double _dDcc);  //Inc move  모션돈 확인은 GetStopInpos() 함수로.


    SML_API char *            __stdcall MT_GetName     (int _iAxisNo);
    SML_API int               __stdcall MT_GetMotorCnt ();
    SML_API EN_MTOR_DIRECTION __stdcall MT_GetDirType  (int _iAxisNo);

    //Trriger Func For AxtFS20 Only. AXL은 검증 해봐야함.                                                             //마이크로 Sec Max 4000
    SML_API void __stdcall MT_SetAxtTrgPos  (int _iAxisNo , int _iPosCnt , double * _dPos , double _dTrgTime , bool _bActual ,bool _bOnLevel);
    SML_API void __stdcall MT_ResetAxtTrgPos(int _iAxisNo );
    SML_API void __stdcall MT_OneShotAxtTrg (int _iAxisNo , bool _bUpEg , int _iTime);

    SML_API void __stdcall MT_SetIntrptPos    (int _iAxisNo , double _dPos, PosCallbackFunc _pCallback);
    SML_API void __stdcall MT_ResetIntrpt     (int _iAxisNo);





//    SML_API void __stdcall MT_SetCntrlPn       (int _iAxisNo , TPanel * _pnBase , EN_MOTR_CNTRL_TYPE _iType );
//    SML_API void __stdcall MT_SetCntrlJogUnit  (int _iAxisNo , double   _dValue );
//    SML_API void __stdcall MT_MoveCntrlPn      (int _iAxisNo , TPanel * _pnBase );
//    SML_API void __stdcall MT_DelCntrlPn       (int _iAxisNo                    );

    ///////////////////
    //Actuator Class.//
    ///////////////////
    //ResetAll -> Reset
    //register 다 지우기.
    SML_API void               __stdcall AT_Reset       (void                                                        );
    SML_API bool               __stdcall AT_Err         (int _iNo                                                    );
    SML_API bool               __stdcall AT_Complete    (int _iNo , bool _bCmd                                       );
    SML_API bool               __stdcall AT_Done        (int _iNo                                                    );
    SML_API char *             __stdcall AT_GetName     (int _iNo                                                    );
    SML_API bool               __stdcall AT_MoveCyl     (int _iNo , bool _bCmd                                       );
    SML_API bool               __stdcall AT_GetCmd      (int _iNo                                                    );
    SML_API int                __stdcall AT_GetActCnt   (void                                                        );
    SML_API EN_ACTR_DIRECTION  __stdcall AT_GetDirType  (int _iNo                                                    );
//    SML_API void       __stdcall AT_SetCntrlPn  (int _iActrNo , TPanel * _pnBase , EN_MOTR_CNTRL_TYPE _iType );
//    SML_API void       __stdcall AT_MoveCntrlPn (int _iActrNo , TPanel * _pnBase                             );
//    SML_API void       __stdcall AT_DelCntrlPn  (int _iActrNo                                                );



    /////////////
    //DIO Class.//
    /////////////
    SML_API void __stdcall IO_SetY    (int _iNo , bool _bVal); //Direct = true일경우 1싸이클 동안 엣지는 못본다.
    SML_API void __stdcall IO_SetYDrct(int _iNo , bool _bVal); //Direct = true일경우 1싸이클 동안 엣지는 못본다.
    SML_API bool __stdcall IO_GetY  (int _iNo );
    SML_API bool __stdcall IO_GetYDn(int _iNo );
    SML_API bool __stdcall IO_GetYUp(int _iNo );

    SML_API bool __stdcall IO_GetX    (int _iNo ); //Direct = true일경우 1싸이클 동안 엣지는 못본다.
    SML_API bool __stdcall IO_GetXDrct(int _iNo ); //Direct = true일경우 1싸이클 동안 엣지는 못본다.
    SML_API bool __stdcall IO_GetXDn(int _iNo );
    SML_API bool __stdcall IO_GetXUp(int _iNo );

    SML_API void __stdcall IO_SetTestMode(bool _bOn) ;
    SML_API bool __stdcall IO_GetTestMode(         ) ;

    SML_API char * __stdcall IO_GetXName(int _iNo ) ;
    SML_API char * __stdcall IO_GetYName(int _iNo ) ;

    //////////////
    //ERR Class.//
    //////////////                    
    SML_API void       __stdcall EM_Clear         (void     ); //모든에러 클리어 시키기.
    SML_API void       __stdcall EM_SetErr        (int  _iNo); //해당 에러 true 시키기.
    SML_API void       __stdcall EM_SetDisp       (bool _bOn); //m_bShowForm 를 false 시킨다.

    SML_API bool       __stdcall EM_IsErr         (void    ); //에러중인지 확인.
    SML_API int        __stdcall EM_GetLastErr    (        ); //멘마지막에 Set된 에러 번호 리턴.
    SML_API char *     __stdcall EM_GetLastErrName(        );
    SML_API char *     __stdcall EM_GetErrName    (int _iNo);
    SML_API char *     __stdcall EM_GetErrAction  (int _iNo);

    SML_API int        __stdcall EM_GetErrCnt     (        ); //총 에러 발생 갯수.
    SML_API int        __stdcall EM_GetMaxErr     (        ); //할당 되어 있는 총 에러갯수 리턴.
    SML_API bool       __stdcall EM_GetErr        (int _iNo); //해당번호의 에러가 떠있는지.
    //SML_API void       __stdcall EM_DisplayList   (TListBox * _lbList); //라이브러리 정규화 위해 위의 함수로 대체.

    ////////////////////
    //TowerLamp Class.//
    ////////////////////
    SML_API void       __stdcall TL_MuteSnd       (); //타워램프 소리 끄기.


    //////////////
    //AIO Class.//
    //////////////  2012.11.23 추가.선계원.
    SML_API void      __stdcall AI_SetY  (int _iNo , double _dVal );
    SML_API double    __stdcall AI_GetY  (int _iNo );
    SML_API double    __stdcall AI_GetX  (int _iNo );

};

#endif


