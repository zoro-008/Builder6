//---------------------------------------------------------------------------
#pragma hdrstop

#include "SMInterfaceUnit.h"
#include "FormDllMain.h"

#include "FormPassword.h"
#include "FormMsgOk.h"
#include "FormMsgYesNo.h"
#include "FormMsgTime.h"
#include "FormErr.h"
#include "FormUpdate.h"

#include "Motors.h"
#include "Actuators.h"
#include "IOs.h"
#include "ErrMan.h"
#include "TowerLamp.h"
#include "Ajin_AIO.h" //나중에 랩핑 한번 하기.

//---------------------------------------------------------------------------

#pragma package(smart_init)

void __stdcall FM_MakeWin(TPanel * Panel)
{
    FrmDllMain  = new TFrmDllMain (Panel);

    FrmMsgYesNo = new TFrmMsgYesNo(FrmDllMain);
    FrmMsgOk    = new TFrmMsgOk   (FrmDllMain);
    FrmMsgTime  = new TFrmMsgTime (FrmDllMain);
    FrmPassword = new TFrmPassword(FrmDllMain);
    FrmUpdate   = new TFrmUpdate  (FrmDllMain);

    //20180530  FrmDllMain 페어런트로 놓으면 소멸할때 지우면서 문제되어서  NULL 넣음.
    FrmErr      = new TFrmErr     (NULL);
}

void __stdcall FM_SetWin(TPanel * Panel)
{
    FrmDllMain -> Parent = Panel ;
    FrmDllMain -> Show() ;
    FrmDllMain -> Left = 0 ;
    FrmDllMain -> Top  = 0 ;

//    FrmDllMain -> Align = alClient ;
}

void __stdcall FM_DelWin()
{
    //20180529 선 추가 이거 없어서 종료시 FrmErr 소멸자뻑남.
//    delete FrmErr     ;//FrmErr = NULL;
    delete FrmMsgYesNo;
    delete FrmMsgOk   ;
    delete FrmMsgTime ;

    delete FrmPassword;
    delete FrmUpdate  ;

    //20180530  FrmDllMain 페어런트로 놓으면 소멸할때 지우면서 문제되어서  NULL 넣음.
    //delete FrmErr     ;//FrmErr = NULL;




    delete FrmDllMain ;


}

void __stdcall FM_HideWin()
{
    FrmDllMain -> Hide();
}


AnsiString __stdcall FM_GetVer ()
{
    return "Ver " + FrmDllMain -> OnGetVer("FileVersion") ;
}

AnsiString __stdcall FM_GetAge ()
{
    return FrmDllMain -> OnGetAge() ;
}

EN_LAN_SEL __stdcall FM_GetLan ()
{
     return FrmDllMain -> GetLang() ;
}

void __stdcall FM_MsgOk(AnsiString _sCaption , AnsiString _sMessage)
{
    MsgOk(_sCaption , _sMessage) ;
}

void __stdcall FM_MsgOkModal(AnsiString _sCaption , AnsiString _sMessage)
{
    MsgOkModal(_sCaption , _sMessage) ;
}

int __stdcall FM_MsgYesNo(AnsiString _sCaption , AnsiString _sMessage)
{
    return MsgYesNo(_sCaption , _sMessage) ;
}

void __stdcall FM_MsgTime(AnsiString _sCaption , AnsiString _sMessage , int _iShowTime)
{
    MsgTime(_sCaption , _sMessage , _iShowTime);
}

void __stdcall FM_CloseMsgOk()
{
    CloseMsgOk() ;
}

int __stdcall FM_PswdShow(EN_LEVEL _iLevel)
{
    return FrmPassword -> Show(_iLevel);
}

EN_LEVEL __stdcall FM_GetLevel()
{
    return FrmPassword -> GetLevel() ;
}

EN_LEVEL __stdcall FM_SetLevel(EN_LEVEL _iSelLevel)
{
    return FrmPassword -> SetLevel(_iSelLevel) ;
}

void __stdcall SM_Update (EN_SEQ_STAT _iStat)
{
    IO.Update();
    MT.Update();
    AT.Update();
    TL.Update(_iStat);
}

void              __stdcall MT_SetHomeEnd    (int _iAxisNo , bool _bValue                  ){       MT.SetHomeEnd    (_iAxisNo ,_bValue        );}
bool              __stdcall MT_GetHomeEnd    (int _iAxisNo                                 ){return MT.GetHomeEnd    (_iAxisNo                 );}
bool              __stdcall MT_GetHomeEndAll (                                             ){return MT.GetHomeEndAll (                         );}
void              __stdcall MT_SetServo      (int _iAxisNo , bool _bOn                     ){       MT.SetServo      (_iAxisNo ,_bOn           );}
bool              __stdcall MT_GetServo      (int _iAxisNo                                 ){return MT.GetServo      (_iAxisNo                 );}
bool              __stdcall MT_GetServoAll   (                                             ){return MT.GetServoAll   (                         );}
void              __stdcall MT_SetServoAll   (               bool _bOn                     ){return MT.SetServoAll   (          _bOn           );}
void              __stdcall MT_SetReset      (int _iAxisNo , bool _bOn                     ){       MT.SetReset      (_iAxisNo ,_bOn           );}
bool              __stdcall MT_GetAlarm      (int _iAxisNo                                 ){return MT.GetAlarm      (_iAxisNo                 );}
bool              __stdcall MT_GetHomeSnsr   (int _iAxisNo                                 ){return MT.GetHomeSnsr   (_iAxisNo                 );}
bool              __stdcall MT_GetPLimSnsr   (int _iAxisNo                                 ){return MT.GetPLimSnsr   (_iAxisNo                 );}
bool              __stdcall MT_GetNLimSnsr   (int _iAxisNo                                 ){return MT.GetNLimSnsr   (_iAxisNo                 );}
bool              __stdcall MT_GetStopInpos  (int _iAxisNo                                 ){return MT.GetStopInpos  (_iAxisNo                 );}
bool              __stdcall MT_GetStop       (int _iAxisNo                                 ){return MT.GetStop       (_iAxisNo                 );}
bool              __stdcall MT_GetZphaseSnsr (int _iAxisNo                                 ){return MT.GetZphaseSnsr (_iAxisNo                 );}
double            __stdcall MT_GetTrgPos     (int _iAxisNo                                 ){return MT.GetTrgPos     (_iAxisNo                 );}
double            __stdcall MT_GetCmdPos     (int _iAxisNo                                 ){return MT.GetCmdPos     (_iAxisNo                 );}
double            __stdcall MT_GetEncPos     (int _iAxisNo                                 ){return MT.GetEncPos     (_iAxisNo                 );}
double            __stdcall MT_GetErrPos     (int _iAxisNo                                 ){return MT.GetErrPos     (_iAxisNo                 );}
void              __stdcall MT_ClearPos      (int _iAxisNo                                 ){       MT.ClearPos      (_iAxisNo                 );}
void              __stdcall MT_SetPos        (int _iAxisNo , double _dPos                  ){       MT.SetPos        (_iAxisNo ,_dPos          );}
bool              __stdcall MT_CmprPos       (int _iAxisNo , double _dPos                  ){return MT.CmprPos       (_iAxisNo ,_dPos          );}
void              __stdcall MT_Reset         (int _iAxisNo                                 ){       MT.Reset         (_iAxisNo                 );}
void              __stdcall MT_ResetAll      (                                             ){       MT.ResetAll      (                         );}
void              __stdcall MT_Stop          (int _iAxisNo                                 ){       MT.Stop          (_iAxisNo                 );}
void              __stdcall MT_EmgStop       (int _iAxisNo                                 ){       MT.EmgStop       (_iAxisNo                 );}
bool              __stdcall MT_Home          (int _iAxisNo                                 ){return MT.Home          (_iAxisNo                 );}
void              __stdcall MT_DoHome        (int _iAxisNo                                 ){       MT.DoHome        (_iAxisNo                 );}
void              __stdcall MT_SetSubHome    (int _iAxisNo , bool _bOn                     ){       MT.SetSubHome    (_iAxisNo ,_bOn           );}
//void              __stdcall MT_DoRpt         (int _iAxisNo , double _dFPos, double _dSPos, int _dTime ){       MT.DoRpt         (_iAxisNo ,_dFPos,_dSPos,_dTime  );}
bool              __stdcall MT_JogP          (int _iAxisNo                                 ){return MT.JogP          (_iAxisNo                 );}
bool              __stdcall MT_JogN          (int _iAxisNo                                 ){return MT.JogN          (_iAxisNo                 );}
bool              __stdcall MT_GoAbs         (int _iAxisNo , double _dPos , double _dVel   ){return MT.GoAbs         (_iAxisNo ,_dPos ,_dVel   );}
void              __stdcall MT_GoInc         (int _iAxisNo , double _dPos , double _dVel   ){       MT.GoInc         (_iAxisNo ,_dPos ,_dVel   );}
bool              __stdcall MT_GoAbsRun      (int _iAxisNo , double _dPos                  ){return MT.GoAbsRun      (_iAxisNo ,_dPos          );}
void              __stdcall MT_GoIncRun      (int _iAxisNo , double _dPos                  ){       MT.GoIncRun      (_iAxisNo ,_dPos          );}
bool              __stdcall MT_GoAbsSlow     (int _iAxisNo , double _dPos                  ){return MT.GoAbsSlow     (_iAxisNo ,_dPos          );}
void              __stdcall MT_GoIncSlow     (int _iAxisNo , double _dPos                  ){       MT.GoIncSlow     (_iAxisNo ,_dPos          );}
bool              __stdcall MT_GoAbsMan      (int _iAxisNo , double _dPos                  ){return MT.GoAbsMan      (_iAxisNo ,_dPos          );}
void              __stdcall MT_GoIncMan      (int _iAxisNo , double _dPos                  ){       MT.GoIncMan      (_iAxisNo ,_dPos          );}
AnsiString        __stdcall MT_GetName       (int _iAxisNo                                 ){return MT.GetName       (_iAxisNo                 );}
int               __stdcall MT_GetMotorCnt   (                                             ){return MT.GetMotorCnt   (                         );}
EN_MTOR_DIRECTION __stdcall MT_GetDirType    (int _iAxisNo                                 ){return MT.GetDirType    (_iAxisNo                 );}

bool __stdcall MT_GoAbsAcDc (int _iAxisNo , double _dPos , double _dVel , double _dAcc , double _dDcc)
{
    return MT.GoAbsAcDc(_iAxisNo , _dPos , _dVel , _dAcc , _dDcc) ;

}
void __stdcall MT_GoIncAcDc (int _iAxisNo , double _dPos , double _dVel , double _dAcc , double _dDcc)
{
   MT.GoIncAcDc(_iAxisNo , _dPos , _dVel , _dAcc , _dDcc) ;
}




/**/
double            __stdcall MT_GetMinPos     (int _iAxisNo                                 ){return MT.GetMinPos     (_iAxisNo                 );}
double            __stdcall MT_GetMaxPos     (int _iAxisNo                                 ){return MT.GetMaxPos     (_iAxisNo                 );}
double            __stdcall MT_GetRunVel     (int _iAxisNo                                 ){return MT.GetRunVel     (_iAxisNo                 );}
double            __stdcall MT_GetManVel     (int _iAxisNo                                 ){return MT.GetManVel     (_iAxisNo                 );}




void __stdcall MT_SetAxtTrgPos  (int _iAxisNo , int _iPosCnt , double * _dPos , double _dTrgTime , bool _bActual ,bool _bOnLevel)
{
    MT.SetAxtTrgPos(_iAxisNo , _iPosCnt , _dPos , _dTrgTime , _bActual ,_bOnLevel);
}
void __stdcall MT_ResetAxtTrgPos(int _iAxisNo )
{
    MT.ResetAxtTrgPos(_iAxisNo) ;
}
void __stdcall MT_OneShotAxtTrg (int _iAxisNo , bool _bUpEg , int _iTime)
{
    MT.OneShotAxtTrg(_iAxisNo , _bUpEg , _iTime) ;
}


void __stdcall MT_SetIntrptPos(int _iAxisNo , double _dPos, PosCallbackFunc _pCallback)
{
    MT.SetIntrptPos(_iAxisNo,_dPos,_pCallback);
}

void __stdcall MT_ResetIntrpt(int _iAxisNo)
{
    MT.ResetIntrpt(_iAxisNo);
}

//void __stdcall MT_SetCntrlPn       (int _iAxisNo , TPanel * _pnBase , EN_MOTR_CNTRL_TYPE _iType )
//{
//    MT.SetCntrlPn(_iAxisNo , _pnBase , _iType );
//}
//
//void __stdcall MT_SetCntrlJogUnit (int _iAxisNo , double _dValue )
//{
//    MT.SetCntrlJogUnit(_iAxisNo , _dValue );
//}
//void __stdcall MT_MoveCntrlPn (int _iAxisNo , TPanel * _pnBase )
//{
//    MT.MoveCntrlPn(_iAxisNo , _pnBase );
//}
//void __stdcall MT_DelCntrlPn (int _iAxisNo )
//{
//    MT.DelCntrlPn(_iAxisNo );
//}

void               __stdcall AT_Reset       (void                                                        ){       AT.Reset     (            );}
bool               __stdcall AT_Err         (int _iNo                                                    ){return AT.Err       (_iNo        );}
bool               __stdcall AT_Complete    (int _iNo , bool _bCmd                                       ){return AT.Complete  (_iNo , _bCmd);}
bool               __stdcall AT_Done        (int _iNo                                                    ){return AT.Complete  (_iNo        );}
AnsiString         __stdcall AT_GetName     (int _iNo                                                    ){return AT.GetName   (_iNo        );}
bool               __stdcall AT_MoveCyl     (int _iNo , bool _bCmd                                       ){return AT.Move      (_iNo , _bCmd);}
bool               __stdcall AT_GetCmd      (int _iNo                                                    ){return AT.GetCmd    (_iNo        );}
int                __stdcall AT_GetActCnt   (void                                                        ){return AT.GetActrCnt(            );}
EN_ACTR_DIRECTION  __stdcall AT_GetDirType  (int _iNo                                                    ){return AT.GetDirType(_iNo        );}
//void        __stdcall AT_SetCntrlPn  (int _iActrNo , TPanel * _pnBase , EN_MOTR_CNTRL_TYPE _iType ){AT.SetCntrlPn (_iActrNo ,_pnBase , _iType ); }
//void        __stdcall AT_MoveCntrlPn (int _iActrNo , TPanel * _pnBase                             ){AT.MoveCntrlPn(_iActrNo ,_pnBase          );}
//void        __stdcall AT_DelCntrlPn  (int _iActrNo                                                ){AT.DelCntrlPn (_iActrNo                   );}
//
void __stdcall IO_SetY  (int _iNo , bool _bVal , bool _bDirect){if(!IO.GetTestMode())IO.SetY  (_iNo ,_bVal , _bDirect);}
bool __stdcall IO_GetY  (int _iNo                ){return IO.GetY  (_iNo          );}
bool __stdcall IO_GetYDn(int _iNo                ){return IO.GetYDn(_iNo          );}
bool __stdcall IO_GetYUp(int _iNo                ){return IO.GetYUp(_iNo          );}

bool __stdcall IO_GetX  (int _iNo , bool _bDirect){return IO.GetX  (_iNo ,_bDirect);}
bool __stdcall IO_GetXDn(int _iNo                ){return IO.GetXDn(_iNo          );}
bool __stdcall IO_GetXUp(int _iNo                ){return IO.GetXUp(_iNo          );}

void  __stdcall IO_SetTestMode(bool _bOn) {IO.SetTestMode(_bOn);}
bool  __stdcall IO_GetTestMode(         ) {return IO.GetTestMode();}

String __stdcall IO_GetXName  (int _iNo ) { return IO.GetXName(_iNo); }
String __stdcall IO_GetYName  (int _iNo ) { return IO.GetYName(_iNo); }

void       __stdcall EM_Clear         (void     ){/*FrmErr -> Close();*/ EM.Clear();}
void       __stdcall EM_SetErr        (int  _iNo){       EM.SetErr        (_iNo);}
void       __stdcall EM_SetDisp       (bool _bOn){       EM.SetDisp       (_bOn);}

bool       __stdcall EM_IsErr         (void     ){return EM.IsErr         (    );}
int        __stdcall EM_GetLastErr    (         ){return EM.GetLastErr    (    );}
AnsiString __stdcall EM_GetLastErrName(         ){return EM.GetLastErrName(    );}
AnsiString __stdcall EM_GetErrName    (int _iNo ){return EM.GetErrName    (_iNo);}
AnsiString __stdcall EM_GetErrAction  (int _iNo ){return EM.GetErrAction  (_iNo);}

void       __stdcall EM_DisplayList   (TListBox * _lbList){EM.DispErr(_lbList);}





void       __stdcall TL_MuteSnd       (         ){TL._bBuzzOff = false ; }


void   __stdcall AI_SetY  (int _iNo , double _dVal ){AI.SetOut(_iNo , _dVal);    }
double __stdcall AI_GetY  (int _iNo                ){return AI.GetOut(_iNo );    }
double __stdcall AI_GetX  (int _iNo                ){return AI.GetIn (_iNo );    }
