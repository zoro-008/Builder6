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
    SML_API char *     __stdcall FM_GetVer (             ); //���� ����. ��ó����. �Ƚ��� 2011.11.8 �����.
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
    SML_API void __stdcall MT_SetHomeEnd    (int _iAxisNo , bool _bValue ); // �ش����� ������ Ȩ����Ȥ�� Ȩ���� ���� ��Ų��.
    SML_API bool __stdcall MT_GetHomeEnd    (int _iAxisNo );                // �ش����� Ȩ���带 Ȯ�� �Ѵ�.
    SML_API bool __stdcall MT_GetHomeEndAll ();                             // ������� Ȩ���带 Ȯ�� �Ѵ�.
    SML_API void __stdcall MT_SetServo      (int _iAxisNo , bool _bOn    ); // �ش����� ������ On/Off ��Ų��.
    SML_API bool __stdcall MT_GetServo      (int _iAxisNo );
    SML_API bool __stdcall MT_GetServoAll   ();
    SML_API void __stdcall MT_SetServoAll   (               bool _bOn    ); // ������� ������/����
    SML_API void __stdcall MT_SetReset      (int _iAxisNo , bool _bOn    ); // �����ѿ� ����� �ϵ���� ���V �ñ׳�.
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
    /*20121123�߰�*/
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
    SML_API void __stdcall MT_GoInc    (int _iAxisNo , double _dPos , double _dVel);  //Inc move  ��ǵ� Ȯ���� GetStopInpos() �Լ���.
    SML_API bool __stdcall MT_GoAbsRun (int _iAxisNo , double _dPos );  //abs move with AutoRun Vel.
    SML_API void __stdcall MT_GoIncRun (int _iAxisNo , double _dPos );  //Inc move with AutoRun ManualVel. ��ǵ� Ȯ���� GetStopInpos() �Լ���.
    SML_API bool __stdcall MT_GoAbsSlow(int _iAxisNo , double _dPos );  //abs move with AutoRun Vel.
    SML_API void __stdcall MT_GoIncSlow(int _iAxisNo , double _dPos );  //Inc move with AutoRun ManualVel. ��ǵ� Ȯ���� GetStopInpos() �Լ���.
    SML_API bool __stdcall MT_GoAbsMan (int _iAxisNo , double _dPos );  //abs move with Manual Vel.
    SML_API void __stdcall MT_GoIncMan (int _iAxisNo , double _dPos );  //Inc move with Manual Vel. ��ǵ� Ȯ���� GetStopInpos() �Լ���.
    /*20121123�߰�*/
    SML_API bool __stdcall MT_GoAbsAcDc(int _iAxisNo , double _dPos , double _dVel , double _dAcc , double _dDcc);  //abs move
    SML_API void __stdcall MT_GoIncAcDc(int _iAxisNo , double _dPos , double _dVel , double _dAcc , double _dDcc);  //Inc move  ��ǵ� Ȯ���� GetStopInpos() �Լ���.


    SML_API char *            __stdcall MT_GetName     (int _iAxisNo);
    SML_API int               __stdcall MT_GetMotorCnt ();
    SML_API EN_MTOR_DIRECTION __stdcall MT_GetDirType  (int _iAxisNo);

    //Trriger Func For AxtFS20 Only. AXL�� ���� �غ�����.                                                             //����ũ�� Sec Max 4000
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
    //register �� �����.
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
    SML_API void __stdcall IO_SetY    (int _iNo , bool _bVal); //Direct = true�ϰ�� 1����Ŭ ���� ������ ������.
    SML_API void __stdcall IO_SetYDrct(int _iNo , bool _bVal); //Direct = true�ϰ�� 1����Ŭ ���� ������ ������.
    SML_API bool __stdcall IO_GetY  (int _iNo );
    SML_API bool __stdcall IO_GetYDn(int _iNo );
    SML_API bool __stdcall IO_GetYUp(int _iNo );

    SML_API bool __stdcall IO_GetX    (int _iNo ); //Direct = true�ϰ�� 1����Ŭ ���� ������ ������.
    SML_API bool __stdcall IO_GetXDrct(int _iNo ); //Direct = true�ϰ�� 1����Ŭ ���� ������ ������.
    SML_API bool __stdcall IO_GetXDn(int _iNo );
    SML_API bool __stdcall IO_GetXUp(int _iNo );

    SML_API void __stdcall IO_SetTestMode(bool _bOn) ;
    SML_API bool __stdcall IO_GetTestMode(         ) ;

    SML_API char * __stdcall IO_GetXName(int _iNo ) ;
    SML_API char * __stdcall IO_GetYName(int _iNo ) ;

    //////////////
    //ERR Class.//
    //////////////                    
    SML_API void       __stdcall EM_Clear         (void     ); //��翡�� Ŭ���� ��Ű��.
    SML_API void       __stdcall EM_SetErr        (int  _iNo); //�ش� ���� true ��Ű��.
    SML_API void       __stdcall EM_SetDisp       (bool _bOn); //m_bShowForm �� false ��Ų��.

    SML_API bool       __stdcall EM_IsErr         (void    ); //���������� Ȯ��.
    SML_API int        __stdcall EM_GetLastErr    (        ); //�ึ������ Set�� ���� ��ȣ ����.
    SML_API char *     __stdcall EM_GetLastErrName(        );
    SML_API char *     __stdcall EM_GetErrName    (int _iNo);
    SML_API char *     __stdcall EM_GetErrAction  (int _iNo);

    SML_API int        __stdcall EM_GetErrCnt     (        ); //�� ���� �߻� ����.
    SML_API int        __stdcall EM_GetMaxErr     (        ); //�Ҵ� �Ǿ� �ִ� �� �������� ����.
    SML_API bool       __stdcall EM_GetErr        (int _iNo); //�ش��ȣ�� ������ ���ִ���.
    //SML_API void       __stdcall EM_DisplayList   (TListBox * _lbList); //���̺귯�� ����ȭ ���� ���� �Լ��� ��ü.

    ////////////////////
    //TowerLamp Class.//
    ////////////////////
    SML_API void       __stdcall TL_MuteSnd       (); //Ÿ������ �Ҹ� ����.


    //////////////
    //AIO Class.//
    //////////////  2012.11.23 �߰�.�����.
    SML_API void      __stdcall AI_SetY  (int _iNo , double _dVal );
    SML_API double    __stdcall AI_GetY  (int _iNo );
    SML_API double    __stdcall AI_GetX  (int _iNo );

};

#endif


