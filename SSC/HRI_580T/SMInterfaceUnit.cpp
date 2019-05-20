//---------------------------------------------------------------------------
#pragma hdrstop

#include "SMInterfaceUnit.h"
#include "FormDllMain.h"

#include "FormPassword.h"
#include "FormMsgOk.h"
#include "FormMsgYesNo.h"
#include "FormErr.h"
#include "FormUpdate.h"

#include "Motors.h"
#include "Actuators.h"
#include "IOs.h"
#include "ErrMan.h"
#include "TowerLamp.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void __stdcall FM_MakeWin(TPanel * Panel)
{
    FrmDllMain  = new TFrmDllMain (Panel);

    FrmMsgYesNo = new TFrmMsgYesNo(FrmDllMain);
    FrmMsgOk    = new TFrmMsgOk   (FrmDllMain);
    FrmErr      = new TFrmErr     (FrmDllMain);
    FrmPassword = new TFrmPassword(FrmDllMain);
    FrmUpdate   = new TFrmUpdate  (FrmDllMain);
}

void __stdcall FM_SetWin(TPanel * Panel)
{
    FrmDllMain -> Parent = Panel ;
    FrmDllMain -> Show() ;
    FrmDllMain -> Align = alClient ;
}

void __stdcall FM_DelWin()
{
    delete FrmDllMain ;
}


void __stdcall FM_MsgOk(AnsiString _sCaption , AnsiString _sMessage)
{
    MsgOk(_sCaption , _sMessage) ;
}

int __stdcall FM_MsgYesNo(AnsiString _sCaption , AnsiString _sMessage)
{
    return MsgYesNo(_sCaption , _sMessage) ;
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

void       __stdcall MT_SetHomeEnd    (int _iAxisNo , bool _bValue                  ){       MT.SetHomeEnd    (_iAxisNo ,_bValue        );}
bool       __stdcall MT_GetHomeEnd    (int _iAxisNo                                 ){return MT.GetHomeEnd    (_iAxisNo                 );}
bool       __stdcall MT_GetHomeEndAll (                                             ){return MT.GetHomeEndAll (                         );}
void       __stdcall MT_SetServo      (int _iAxisNo , bool _bOn                     ){       MT.SetServo      (_iAxisNo ,_bOn           );}
bool       __stdcall MT_GetServo      (int _iAxisNo                                 ){return MT.GetServo      (_iAxisNo                 );}
bool       __stdcall MT_GetServoAll   (                                             ){return MT.GetServoAll   (                         );}
void       __stdcall MT_SetServoAll   (               bool _bOn                     ){return MT.SetServoAll   (          _bOn           );}
void       __stdcall MT_SetReset      (int _iAxisNo , bool _bOn                     ){       MT.SetReset      (_iAxisNo ,_bOn           );}
bool       __stdcall MT_GetAlarm      (int _iAxisNo                                 ){return MT.GetAlarm      (_iAxisNo                 );}
bool       __stdcall MT_GetPLimSnsr   (int _iAxisNo                                 ){return MT.GetPLimSnsr   (_iAxisNo                 );}
bool       __stdcall MT_GetNLimSnsr   (int _iAxisNo                                 ){return MT.GetNLimSnsr   (_iAxisNo                 );}
bool       __stdcall MT_GetStopInpos  (int _iAxisNo                                 ){return MT.GetStopInpos  (_iAxisNo                 );}
double     __stdcall MT_GetTrgPos     (int _iAxisNo                                 ){return MT.GetTrgPos     (_iAxisNo                 );}
double     __stdcall MT_GetCmdPos     (int _iAxisNo                                 ){return MT.GetCmdPos     (_iAxisNo                 );}
double     __stdcall MT_GetEncPos     (int _iAxisNo                                 ){return MT.GetEncPos     (_iAxisNo                 );}
double     __stdcall MT_GetErrPos     (int _iAxisNo                                 ){return MT.GetErrPos     (_iAxisNo                 );}
void       __stdcall MT_ClearPos      (int _iAxisNo                                 ){       MT.ClearPos      (_iAxisNo                 );}
void       __stdcall MT_SetPos        (int _iAxisNo , double _dPos                  ){       MT.SetPos        (_iAxisNo ,_dPos          );}
bool       __stdcall MT_CmprPos       (int _iAxisNo , double _dPos                  ){return MT.CmprPos       (_iAxisNo ,_dPos          );}
void       __stdcall MT_Reset         (int _iAxisNo                                 ){       MT.Reset         (_iAxisNo                 );}
void       __stdcall MT_ResetAll      (                                             ){       MT.ResetAll      (                         );}
bool       __stdcall MT_Stop          (int _iAxisNo                                 ){return MT.Stop          (_iAxisNo                 );}
bool       __stdcall MT_EmgStop       (int _iAxisNo                                 ){return MT.EmgStop       (_iAxisNo                 );}
bool       __stdcall MT_Home          (int _iAxisNo                                 ){return MT.Home          (_iAxisNo                 );}
void       __stdcall MT_DoHome        (int _iAxisNo                                 ){       MT.DoHome        (_iAxisNo                 );}
//void       __stdcall MT_DoRpt         (int _iAxisNo , double _dFPos, double _dSPos, int _dTime ){       MT.DoRpt         (_iAxisNo ,_dFPos,_dSPos,_dTime  );}
bool       __stdcall MT_JogP          (int _iAxisNo                                 ){return MT.JogP          (_iAxisNo                 );}
bool       __stdcall MT_JogN          (int _iAxisNo                                 ){return MT.JogN          (_iAxisNo                 );}
bool       __stdcall MT_GoAbs         (int _iAxisNo , double _dPos , double _dVel   ){return MT.GoAbs         (_iAxisNo ,_dPos ,_dVel   );}
void       __stdcall MT_GoInc         (int _iAxisNo , double _dPos , double _dVel   ){       MT.GoInc         (_iAxisNo ,_dPos ,_dVel   );}
bool       __stdcall MT_GoAbsRun      (int _iAxisNo , double _dPos                  ){return MT.GoAbsRun      (_iAxisNo ,_dPos          );}
void       __stdcall MT_GoIncRun      (int _iAxisNo , double _dPos                  ){       MT.GoIncRun      (_iAxisNo ,_dPos          );}
bool       __stdcall MT_GoAbsMan      (int _iAxisNo , double _dPos                  ){return MT.GoAbsMan      (_iAxisNo ,_dPos          );}
void       __stdcall MT_GoIncMan      (int _iAxisNo , double _dPos                  ){       MT.GoIncMan      (_iAxisNo ,_dPos          );}
AnsiString __stdcall MT_GetName       (int _iAxisNo                                 ){return MT.GetName       (_iAxisNo                 );}
int        __stdcall MT_GetMotorCnt   (                                             ){return MT.GetMotorCnt   (                         );}

void __stdcall SetAxtTrgPos  (int _iAxisNo , int _iPosCnt , double * _dPos , double _dTrgTime , bool _bActual ,bool _bOnLevel)
{
    MT.SetAxtTrgPos(_iAxisNo , _iPosCnt , _dPos , _dTrgTime , _bActual ,_bOnLevel);
}
void __stdcall ResetAxtTrgPos(int _iAxisNo )
{
    MT.ResetAxtTrgPos(_iAxisNo) ;
}


void        __stdcall AT_Reset    (void                 ){       AT.Reset    (            );}
bool        __stdcall AT_Err      (int _iNo             ){return AT.Err      (_iNo        );}
bool        __stdcall AT_Complete (int _iNo , bool _bCmd){return AT.Complete (_iNo , _bCmd);}
bool        __stdcall AT_Done     (int _iNo             ){return AT.Complete (_iNo        );}
AnsiString  __stdcall AT_GetName  (int _iNo             ){return AT.GetName  (_iNo        );}
bool        __stdcall AT_MoveCyl  (int _iNo , bool _bCmd){return AT.Move     (_iNo , _bCmd);}


void __stdcall IO_SetY  (int _iNo , bool _bVal   ){       IO.SetY  (_iNo ,_bVal   );}
bool __stdcall IO_GetY  (int _iNo                ){return IO.GetY  (_iNo          );}
bool __stdcall IO_GetYDn(int _iNo                ){return IO.GetYDn(_iNo          );}
bool __stdcall IO_GetYUp(int _iNo                ){return IO.GetYUp(_iNo          );}

bool __stdcall IO_GetX  (int _iNo , bool _bDirect){return IO.GetX  (_iNo ,_bDirect);}
bool __stdcall IO_GetXDn(int _iNo                ){return IO.GetXDn(_iNo          );}
bool __stdcall IO_GetXUp(int _iNo                ){return IO.GetXUp(_iNo          );}


void       __stdcall EM_Clear         (void     ){       EM.Clear         (    );}
void       __stdcall EM_SetErr        (int  _iNo){       EM.SetErr        (_iNo);}
void       __stdcall EM_SetDisp       (bool _bOn){       EM.SetDisp       (_bOn);}

bool       __stdcall EM_IsErr         (void     ){return EM.IsErr         (    );}
int        __stdcall EM_GetLastErr    (         ){return EM.GetLastErr    (    );}
AnsiString __stdcall EM_GetLastErrName(         ){return EM.GetLastErrName(    );}
AnsiString __stdcall EM_GetErrName    (int _iNo ){return EM.GetErrName    (_iNo);}

void       __stdcall EM_DisplayList   (TListBox * _lbList){EM.DispErr(_lbList);}





void       __stdcall TL_MuteSnd       (         ){TL._bBuzzOff = false ; }
