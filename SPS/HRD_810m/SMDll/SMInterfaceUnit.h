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
    __declspec(dllexport) void       __stdcall FM_MakeWin(TPanel * Panel);
    __declspec(dllexport) void       __stdcall FM_SetWin (TPanel * Panel);
    __declspec(dllexport) void       __stdcall FM_DelWin (              );
    __declspec(dllexport) void       __stdcall FM_HideWin(              );
    __declspec(dllexport) AnsiString __stdcall FM_GetVer (              ); //���� ����. ��ó����. �Ƚ��� 2011.11.8 �����.
    __declspec(dllexport) AnsiString __stdcall FM_GetAge (              );
    __declspec(dllexport) EN_LAN_SEL __stdcall FM_GetLan (              );


    /////////////
    //Sub Form //
    /////////////
    __declspec(dllexport) void __stdcall FM_MsgOk     (AnsiString _sCaption , AnsiString _sMessage);
    __declspec(dllexport) void __stdcall FM_MsgOkModal(AnsiString _sCaption , AnsiString _sMessage);
    __declspec(dllexport) int  __stdcall FM_MsgYesNo  (AnsiString _sCaption , AnsiString _sMessage);
    __declspec(dllexport) void __stdcall FM_MsgTime   (AnsiString _sCaption , AnsiString _sMessage , int _iShowTime);
    __declspec(dllexport) void __stdcall FM_CloseMsgOk();

    //////////////////
    //Password Form //
    //////////////////
    __declspec(dllexport) int      __stdcall FM_PswdShow(EN_LEVEL _iLevel );
    __declspec(dllexport) EN_LEVEL __stdcall FM_GetLevel();
    __declspec(dllexport) EN_LEVEL __stdcall FM_SetLevel(EN_LEVEL _iSelLevel);

    //////////////////
    //Sequence      //
    //////////////////
    __declspec(dllexport) void     __stdcall SM_Update  (EN_SEQ_STAT _iStat);



    ////////////////
    //Motor Class.//
    ////////////////
    __declspec(dllexport) void __stdcall MT_SetHomeEnd    (int _iAxisNo , bool _bValue ); // �ش����� ������ Ȩ����Ȥ�� Ȩ���� ���� ��Ų��.
    __declspec(dllexport) bool __stdcall MT_GetHomeEnd    (int _iAxisNo );                // �ش����� Ȩ���带 Ȯ�� �Ѵ�.
    __declspec(dllexport) bool __stdcall MT_GetHomeEndAll ();                             // ������� Ȩ���带 Ȯ�� �Ѵ�.
    __declspec(dllexport) void __stdcall MT_SetServo      (int _iAxisNo , bool _bOn    ); // �ش����� ������ On/Off ��Ų��.
    __declspec(dllexport) bool __stdcall MT_GetServo      (int _iAxisNo );
    __declspec(dllexport) bool __stdcall MT_GetServoAll   ();
    __declspec(dllexport) void __stdcall MT_SetServoAll   (               bool _bOn    ); // ������� ������/����
    __declspec(dllexport) void __stdcall MT_SetReset      (int _iAxisNo , bool _bOn    ); // �����ѿ� ����� �ϵ���� ���V �ñ׳�.
    __declspec(dllexport) bool __stdcall MT_GetAlarm      (int _iAxisNo );
    __declspec(dllexport) bool __stdcall MT_GetHomeSnsr   (int _iAxisNo );
    __declspec(dllexport) bool __stdcall MT_GetPLimSnsr   (int _iAxisNo );
    __declspec(dllexport) bool __stdcall MT_GetNLimSnsr   (int _iAxisNo );
    __declspec(dllexport) bool __stdcall MT_GetStopInpos  (int _iAxisNo ); //Check Motion Done with User Inposition.
    __declspec(dllexport) bool __stdcall MT_GetStop       (int _iAxisNo ); //Check Motion Done with User Inposition.
    __declspec(dllexport) bool __stdcall MT_GetZphaseSnsr (int _iAxisNo ); //

    //Position Functions.
    __declspec(dllexport) double __stdcall MT_GetTrgPos(int _iAxisNo );                       //Get Target  Position.
    __declspec(dllexport) double __stdcall MT_GetCmdPos(int _iAxisNo );                       //Get Command Position.
    __declspec(dllexport) double __stdcall MT_GetEncPos(int _iAxisNo );                       //Get Encoder Position.
    __declspec(dllexport) double __stdcall MT_GetErrPos(int _iAxisNo );                       //Get Control Error Position.
    __declspec(dllexport) void   __stdcall MT_ClearPos (int _iAxisNo );                       //Clear Position.
    __declspec(dllexport) void   __stdcall MT_SetPos   (int _iAxisNo , double _dPos );        //Set Any Position.
    __declspec(dllexport) bool   __stdcall MT_CmprPos  (int _iAxisNo , double _dPos );
    /*20121123�߰�*/
    __declspec(dllexport) double __stdcall MT_GetMinPos(int _iAxisNo );                       //Get Setted Min Pos
    __declspec(dllexport) double __stdcall MT_GetMaxPos(int _iAxisNo );                       //Get Setted Max Pos
    __declspec(dllexport) double __stdcall MT_GetRunVel(int _iAxisNo );                       //Get Setted Run Vel
    __declspec(dllexport) double __stdcall MT_GetManVel(int _iAxisNo );                       //Get Setted Man Vel


    //Motion Functions.
    __declspec(dllexport) void __stdcall MT_Reset    (int _iAxisNo); //Reset
    __declspec(dllexport) void __stdcall MT_ResetAll (            );
    __declspec(dllexport) void __stdcall MT_Stop     (int _iAxisNo);
    __declspec(dllexport) void __stdcall MT_EmgStop  (int _iAxisNo); //Stop Without Deceleration.

    __declspec(dllexport) bool __stdcall MT_Home      (int _iAxisNo); //Start Home Cycle for Sequense Cycle.
    __declspec(dllexport) void __stdcall MT_DoHome    (int _iAxisNo); //Start Home Cycle for Click Event.
    __declspec(dllexport) void __stdcall MT_SetSubHome(int _iAxisNo , bool _bOn); //Start Home Cycle for Click Event.
    __declspec(dllexport) void __stdcall MT_DoRpt     (int _iAxisNo); //Start Home Cycle for Click Event.

    __declspec(dllexport) bool __stdcall MT_JogP     (int _iAxisNo); //Jogging to CW.
    __declspec(dllexport) bool __stdcall MT_JogN     (int _iAxisNo); //Jogging to CCW.

    __declspec(dllexport) bool __stdcall MT_GoAbs    (int _iAxisNo , double _dPos , double _dVel);  //abs move
    __declspec(dllexport) void __stdcall MT_GoInc    (int _iAxisNo , double _dPos , double _dVel);  //Inc move  ��ǵ� Ȯ���� GetStopInpos() �Լ���.
    __declspec(dllexport) bool __stdcall MT_GoAbsRun (int _iAxisNo , double _dPos );  //abs move with AutoRun Vel.
    __declspec(dllexport) void __stdcall MT_GoIncRun (int _iAxisNo , double _dPos );  //Inc move with AutoRun ManualVel. ��ǵ� Ȯ���� GetStopInpos() �Լ���.
    __declspec(dllexport) bool __stdcall MT_GoAbsSlow(int _iAxisNo , double _dPos );  //abs move with AutoRun Vel.
    __declspec(dllexport) void __stdcall MT_GoIncSlow(int _iAxisNo , double _dPos );  //Inc move with AutoRun ManualVel. ��ǵ� Ȯ���� GetStopInpos() �Լ���.
    __declspec(dllexport) bool __stdcall MT_GoAbsMan (int _iAxisNo , double _dPos );  //abs move with Manual Vel.
    __declspec(dllexport) void __stdcall MT_GoIncMan (int _iAxisNo , double _dPos );  //Inc move with Manual Vel. ��ǵ� Ȯ���� GetStopInpos() �Լ���.
    /*20121123�߰�*/
    __declspec(dllexport) bool __stdcall MT_GoAbsAcDc(int _iAxisNo , double _dPos , double _dVel , double _dAcc , double _dDcc);  //abs move
    __declspec(dllexport) void __stdcall MT_GoIncAcDc(int _iAxisNo , double _dPos , double _dVel , double _dAcc , double _dDcc);  //Inc move  ��ǵ� Ȯ���� GetStopInpos() �Լ���.


    __declspec(dllexport) AnsiString        __stdcall MT_GetName     (int _iAxisNo);
    __declspec(dllexport) int               __stdcall MT_GetMotorCnt ();
    __declspec(dllexport) EN_MTOR_DIRECTION __stdcall MT_GetDirType  (int _iAxisNo);

    //Trriger Func For AxtFS20 Only. AXL�� ���� �غ�����.                                                             //����ũ�� Sec Max 4000
    __declspec(dllexport) void __stdcall MT_SetAxtTrgPos  (int _iAxisNo , int _iPosCnt , double * _dPos , double _dTrgTime , bool _bActual ,bool _bOnLevel);
    __declspec(dllexport) void __stdcall MT_ResetAxtTrgPos(int _iAxisNo );
    __declspec(dllexport) void __stdcall MT_OneShotAxtTrg (int _iAxisNo , bool _bUpEg , int _iTime);

    __declspec(dllexport) void __stdcall MT_SetIntrptPos    (int _iAxisNo , double _dPos, PosCallbackFunc _pCallback);
    __declspec(dllexport) void __stdcall MT_ResetIntrpt     (int _iAxisNo);





//    __declspec(dllexport) void __stdcall MT_SetCntrlPn       (int _iAxisNo , TPanel * _pnBase , EN_MOTR_CNTRL_TYPE _iType );
//    __declspec(dllexport) void __stdcall MT_SetCntrlJogUnit  (int _iAxisNo , double   _dValue );
//    __declspec(dllexport) void __stdcall MT_MoveCntrlPn      (int _iAxisNo , TPanel * _pnBase );
//    __declspec(dllexport) void __stdcall MT_DelCntrlPn       (int _iAxisNo                    );

    ///////////////////
    //Actuator Class.//
    ///////////////////
    //ResetAll -> Reset
    //register �� �����.
    __declspec(dllexport) void               __stdcall AT_Reset       (void                                                        );
    __declspec(dllexport) bool               __stdcall AT_Err         (int _iNo                                                    );
    __declspec(dllexport) bool               __stdcall AT_Complete    (int _iNo , bool _bCmd                                       );
    __declspec(dllexport) bool               __stdcall AT_Done        (int _iNo                                                    );
    __declspec(dllexport) AnsiString         __stdcall AT_GetName     (int _iNo                                                    );
    __declspec(dllexport) bool               __stdcall AT_MoveCyl     (int _iNo , bool _bCmd                                       );
    __declspec(dllexport) bool               __stdcall AT_GetCmd      (int _iNo                                                    );
    __declspec(dllexport) int                __stdcall AT_GetActCnt   (void                                                        );
    __declspec(dllexport) EN_ACTR_DIRECTION  __stdcall AT_GetDirType  (int _iNo                                                    );
//    __declspec(dllexport) void       __stdcall AT_SetCntrlPn  (int _iActrNo , TPanel * _pnBase , EN_MOTR_CNTRL_TYPE _iType );
//    __declspec(dllexport) void       __stdcall AT_MoveCntrlPn (int _iActrNo , TPanel * _pnBase                             );
//    __declspec(dllexport) void       __stdcall AT_DelCntrlPn  (int _iActrNo                                                );



    /////////////
    //DIO Class.//
    /////////////
    __declspec(dllexport) void __stdcall IO_SetY  (int _iNo , bool _bVal ,bool _bDirect = false); //Direct = true�ϰ�� 1����Ŭ ���� ������ ������.
    __declspec(dllexport) bool __stdcall IO_GetY  (int _iNo );
    __declspec(dllexport) bool __stdcall IO_GetYDn(int _iNo );
    __declspec(dllexport) bool __stdcall IO_GetYUp(int _iNo );

    __declspec(dllexport) bool __stdcall IO_GetX  (int _iNo , bool _bDirect = false); //Direct = true�ϰ�� 1����Ŭ ���� ������ ������.
    __declspec(dllexport) bool __stdcall IO_GetXDn(int _iNo );
    __declspec(dllexport) bool __stdcall IO_GetXUp(int _iNo );

    __declspec(dllexport) void __stdcall IO_SetTestMode(bool _bOn) ;
    __declspec(dllexport) bool __stdcall IO_GetTestMode(         ) ;

    __declspec(dllexport) String __stdcall IO_GetXName(int _iNo ) ;
    __declspec(dllexport) String __stdcall IO_GetYName(int _iNo ) ;

    //////////////
    //ERR Class.//
    //////////////                    
    __declspec(dllexport) void       __stdcall EM_Clear         (void     ); //��翡�� Ŭ���� ��Ű��.
    __declspec(dllexport) void       __stdcall EM_SetErr        (int  _iNo); //�ش� ���� true ��Ű��.
    __declspec(dllexport) void       __stdcall EM_SetDisp       (bool _bOn); //m_bShowForm �� false ��Ų��.

    __declspec(dllexport) bool       __stdcall EM_IsErr         (void    ); //���������� Ȯ��.
    __declspec(dllexport) bool       __stdcall EM_GetErr        (int _iNo); //Ư�� ���� ���� Ȯ��.
    __declspec(dllexport) int        __stdcall EM_GetLastErr    (        ); //�ึ������ Set�� ���� ��ȣ ����.
    __declspec(dllexport) AnsiString __stdcall EM_GetLastErrName(        );
    __declspec(dllexport) AnsiString __stdcall EM_GetErrName    (int _iNo);
    __declspec(dllexport) AnsiString __stdcall EM_GetErrAction  (int _iNo);

    __declspec(dllexport) void       __stdcall EM_DisplayList   (TListBox * _lbList);

    ////////////////////
    //TowerLamp Class.//
    ////////////////////
    __declspec(dllexport) void       __stdcall TL_MuteSnd       (); //Ÿ������ �Ҹ� ����.


    //////////////
    //AIO Class.//
    //////////////  2012.11.23 �߰�.�����.
    __declspec(dllexport) void      __stdcall AI_SetY  (int _iNo , double _dVal );
    __declspec(dllexport) double    __stdcall AI_GetY  (int _iNo );
    __declspec(dllexport) double    __stdcall AI_GetX  (int _iNo );

};

#endif
