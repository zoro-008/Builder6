//---------------------------------------------------------------------------

//#include <vcl.h>
#include <windows.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//   Important note about DLL memory management when your DLL uses the
//   static version of the RunTime Library:
//
//   If your DLL exports any functions that pass String objects (or structs/
//   classes containing nested Strings) as parameter or function results,
//   you will need to add the library MEMMGR.LIB to both the DLL project and
//   any other projects that use the DLL.  You will also need to use MEMMGR.LIB
//   if any other projects which use the DLL will be performing new or delete
//   operations on any non-TObject-derived classes which are exported from the
//   DLL. Adding MEMMGR.LIB to your project will change the DLL and its calling
//   EXE's to use the BORLNDMM.DLL as their memory manager.  In these cases,
//   the file BORLNDMM.DLL should be deployed along with your DLL.
//
//   To avoid using BORLNDMM.DLL, pass string information using "char *" or
//   ShortString parameters.
//
//   If your DLL uses the dynamic version of the RTL, you do not need to
//   explicitly add MEMMGR.LIB as this will be done implicitly for you
//---------------------------------------------------------------------------

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
        return 1;
}
//---------------------------------------------------------------------------





#include "SMDll.h"
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

extern "C" SML_API
void __stdcall FM_MakeWin()
{

    FrmDllMain  = new TFrmDllMain (NULL);//

    FrmMsgYesNo = new TFrmMsgYesNo(NULL);//FrmDllMain);
    FrmMsgOk    = new TFrmMsgOk   (NULL);//FrmDllMain);
    FrmMsgTime  = new TFrmMsgTime (NULL);//FrmDllMain);
    FrmErr      = new TFrmErr     (NULL);//FrmDllMain);
    FrmPassword = new TFrmPassword(NULL);//FrmDllMain);
    FrmUpdate   = new TFrmUpdate  (NULL);//FrmDllMain);

}

extern "C" SML_API
void __stdcall FM_SetWin(void * _hWnd)
{
    //Test 정규 Dll화.
    if(FrmDllMain == NULL) {
        return ;
    }
    if(_hWnd == Application) {
        FrmDllMain -> BorderStyle = bsDialog ;
        FrmDllMain -> Show() ;
        return ;

    }

    FrmDllMain -> ParentWindow = _hWnd ;

    //SetParent()

    //FrmDllMain -> Parent = Panel ;
    FrmDllMain -> Show() ;

    RECT   rt     ;
    //LPRECT lpRect ;
    GetClientRect(_hWnd , &rt);

    FrmDllMain -> Left = 0 ;
    FrmDllMain -> Top  = 0 ;
    FrmDllMain -> Width = rt.right  ;
    FrmDllMain -> Height= rt.bottom ;

//    FrmDllMain -> Align = alClient ;
}

extern "C" SML_API
void __stdcall FM_DelWin()
{
    if(FrmDllMain==NULL) return ;
    delete FrmMsgYesNo ; FrmMsgYesNo = NULL ;
    delete FrmMsgOk    ; FrmMsgOk    = NULL ;
    delete FrmMsgTime  ; FrmMsgTime  = NULL ;
    delete FrmErr      ; FrmErr      = NULL ;
    delete FrmPassword ; FrmPassword = NULL ;
    delete FrmUpdate   ; FrmUpdate   = NULL ;


    delete FrmDllMain ; //나머지폼들은 Owner 가 파괴되어서 자동 파괴.
    FrmDllMain = NULL ;
}

extern "C" SML_API
void __stdcall FM_HideWin()
{
    FrmDllMain -> Hide();
}


extern "C" SML_API
char * __stdcall FM_GetVer ()
{
    return ("Ver " + FrmDllMain -> OnGetVer("FileVersion")).c_str() ;
}

extern "C" SML_API
char * __stdcall FM_GetAge ()
{
    return (FrmDllMain -> OnGetAge()).c_str() ;
}

extern "C" SML_API
EN_LAN_SEL __stdcall FM_GetLan ()
{
     return FrmDllMain -> GetLang() ;
}

extern "C" SML_API
void __stdcall FM_MsgOk(char * _sCaption , char * _sMessage)
{
    MsgOk(_sCaption , _sMessage) ;
}

extern "C" SML_API
void __stdcall FM_MsgOkModal(char * _sCaption , char * _sMessage)
{
    MsgOkModal(_sCaption , _sMessage) ;
}

extern "C" SML_API
int __stdcall FM_MsgYesNo(char * _sCaption , char * _sMessage)
{
    return MsgYesNo(_sCaption , _sMessage) ;
}

extern "C" SML_API
void __stdcall FM_MsgTime(char * _sCaption , char * _sMessage , int _iShowTime)
{
    MsgTime(_sCaption , _sMessage , _iShowTime);
}

extern "C" SML_API
void __stdcall FM_CloseMsgOk()
{
    CloseMsgOk() ;
}

extern "C" SML_API
int __stdcall FM_PswdShow(EN_LEVEL _iLevel)
{
    return FrmPassword -> Show(_iLevel);
}

extern "C" SML_API
EN_LEVEL __stdcall FM_GetLevel()
{
    return FrmPassword -> GetLevel() ;
}

extern "C" SML_API
EN_LEVEL __stdcall FM_SetLevel(EN_LEVEL _iSelLevel)
{
    return FrmPassword -> SetLevel(_iSelLevel) ;
}

extern "C" SML_API
void __stdcall SM_Update (EN_SEQ_STAT _iStat)
{
    IO.Update();
    MT.Update();
    AT.Update();
    TL.Update(_iStat);
}

extern "C" SML_API void              __stdcall MT_SetHomeEnd    (int _iAxisNo , bool _bValue                  ){       MT.SetHomeEnd    (_iAxisNo ,_bValue        );}
extern "C" SML_API bool              __stdcall MT_GetHomeEnd    (int _iAxisNo                                 ){return MT.GetHomeEnd    (_iAxisNo                 );}
extern "C" SML_API bool              __stdcall MT_GetHomeEndAll (                                             ){return MT.GetHomeEndAll (                         );}
extern "C" SML_API void              __stdcall MT_SetServo      (int _iAxisNo , bool _bOn                     ){       MT.SetServo      (_iAxisNo ,_bOn           );}
extern "C" SML_API bool              __stdcall MT_GetServo      (int _iAxisNo                                 ){return MT.GetServo      (_iAxisNo                 );}
extern "C" SML_API bool              __stdcall MT_GetServoAll   (                                             ){return MT.GetServoAll   (                         );}
extern "C" SML_API void              __stdcall MT_SetServoAll   (               bool _bOn                     ){return MT.SetServoAll   (          _bOn           );}
extern "C" SML_API void              __stdcall MT_SetReset      (int _iAxisNo , bool _bOn                     ){       MT.SetReset      (_iAxisNo ,_bOn           );}
extern "C" SML_API bool              __stdcall MT_GetAlarm      (int _iAxisNo                                 ){return MT.GetAlarm      (_iAxisNo                 );}
extern "C" SML_API bool              __stdcall MT_GetHomeSnsr   (int _iAxisNo                                 ){return MT.GetHomeSnsr   (_iAxisNo                 );}
extern "C" SML_API bool              __stdcall MT_GetPLimSnsr   (int _iAxisNo                                 ){return MT.GetPLimSnsr   (_iAxisNo                 );}
extern "C" SML_API bool              __stdcall MT_GetNLimSnsr   (int _iAxisNo                                 ){return MT.GetNLimSnsr   (_iAxisNo                 );}
extern "C" SML_API bool              __stdcall MT_GetStopInpos  (int _iAxisNo                                 ){return MT.GetStopInpos  (_iAxisNo                 );}
extern "C" SML_API bool              __stdcall MT_GetStop       (int _iAxisNo                                 ){return MT.GetStop       (_iAxisNo                 );}
extern "C" SML_API bool              __stdcall MT_GetZphaseSnsr (int _iAxisNo                                 ){return MT.GetZphaseSnsr (_iAxisNo                 );}
extern "C" SML_API double            __stdcall MT_GetTrgPos     (int _iAxisNo                                 ){return MT.GetTrgPos     (_iAxisNo                 );}
extern "C" SML_API double            __stdcall MT_GetCmdPos     (int _iAxisNo                                 ){return MT.GetCmdPos     (_iAxisNo                 );}
extern "C" SML_API double            __stdcall MT_GetEncPos     (int _iAxisNo                                 ){return MT.GetEncPos     (_iAxisNo                 );}
extern "C" SML_API double            __stdcall MT_GetErrPos     (int _iAxisNo                                 ){return MT.GetErrPos     (_iAxisNo                 );}
extern "C" SML_API void              __stdcall MT_ClearPos      (int _iAxisNo                                 ){       MT.ClearPos      (_iAxisNo                 );}
extern "C" SML_API void              __stdcall MT_SetPos        (int _iAxisNo , double _dPos                  ){       MT.SetPos        (_iAxisNo ,_dPos          );}
extern "C" SML_API bool              __stdcall MT_CmprPos       (int _iAxisNo , double _dPos                  ){return MT.CmprPos       (_iAxisNo ,_dPos          );}
extern "C" SML_API void              __stdcall MT_Reset         (int _iAxisNo                                 ){       MT.Reset         (_iAxisNo                 );}
extern "C" SML_API void              __stdcall MT_ResetAll      (                                             ){       MT.ResetAll      (                         );}
extern "C" SML_API void              __stdcall MT_Stop          (int _iAxisNo                                 ){       MT.Stop          (_iAxisNo                 );}
extern "C" SML_API void              __stdcall MT_EmgStop       (int _iAxisNo                                 ){       MT.EmgStop       (_iAxisNo                 );}
extern "C" SML_API bool              __stdcall MT_Home          (int _iAxisNo                                 ){return MT.Home          (_iAxisNo                 );}
extern "C" SML_API void              __stdcall MT_DoHome        (int _iAxisNo                                 ){       MT.DoHome        (_iAxisNo                 );}
//extern "C" SML_API void              __stdcall MT_DoRpt         (int _iAxisNo , double _dFPos, double _dSPos, int _dTime ){       MT.DoRpt         (_iAxisNo ,_dFPos,_dSPos,_dTime  );}
extern "C" SML_API bool              __stdcall MT_JogP          (int _iAxisNo                                 ){return MT.JogP          (_iAxisNo                 );}
extern "C" SML_API bool              __stdcall MT_JogN          (int _iAxisNo                                 ){return MT.JogN          (_iAxisNo                 );}
extern "C" SML_API bool              __stdcall MT_GoAbs         (int _iAxisNo , double _dPos , double _dVel   ){return MT.GoAbs         (_iAxisNo ,_dPos ,_dVel   );}
extern "C" SML_API void              __stdcall MT_GoInc         (int _iAxisNo , double _dPos , double _dVel   ){       MT.GoInc         (_iAxisNo ,_dPos ,_dVel   );}
extern "C" SML_API bool              __stdcall MT_GoAbsRun      (int _iAxisNo , double _dPos                  ){return MT.GoAbsRun      (_iAxisNo ,_dPos          );}
extern "C" SML_API void              __stdcall MT_GoIncRun      (int _iAxisNo , double _dPos                  ){       MT.GoIncRun      (_iAxisNo ,_dPos          );}
extern "C" SML_API bool              __stdcall MT_GoAbsSlow     (int _iAxisNo , double _dPos                  ){return MT.GoAbsSlow     (_iAxisNo ,_dPos          );}
extern "C" SML_API void              __stdcall MT_GoIncSlow     (int _iAxisNo , double _dPos                  ){       MT.GoIncSlow     (_iAxisNo ,_dPos          );}
extern "C" SML_API bool              __stdcall MT_GoAbsMan      (int _iAxisNo , double _dPos                  ){return MT.GoAbsMan      (_iAxisNo ,_dPos          );}
extern "C" SML_API void              __stdcall MT_GoIncMan      (int _iAxisNo , double _dPos                  ){       MT.GoIncMan      (_iAxisNo ,_dPos          );}
extern "C" SML_API char *            __stdcall MT_GetName       (int _iAxisNo                                 ){return MT.GetName       (_iAxisNo                 ).c_str();}
extern "C" SML_API int               __stdcall MT_GetMotorCnt   (                                             ){return MT.GetMotorCnt   (                         );}
extern "C" SML_API EN_MTOR_DIRECTION __stdcall MT_GetDirType    (int _iAxisNo                                 ){return MT.GetDirType    (_iAxisNo                 );}

extern "C" SML_API 
bool __stdcall MT_GoAbsAcDc (int _iAxisNo , double _dPos , double _dVel , double _dAcc , double _dDcc)
{
    return MT.GoAbsAcDc(_iAxisNo , _dPos , _dVel , _dAcc , _dDcc) ;

}

extern "C" SML_API 
void __stdcall MT_GoIncAcDc (int _iAxisNo , double _dPos , double _dVel , double _dAcc , double _dDcc)
{
   MT.GoIncAcDc(_iAxisNo , _dPos , _dVel , _dAcc , _dDcc) ;
}




/**/
extern "C" SML_API double            __stdcall MT_GetMinPos     (int _iAxisNo                                 ){return MT.GetMinPos     (_iAxisNo                 );}
extern "C" SML_API double            __stdcall MT_GetMaxPos     (int _iAxisNo                                 ){return MT.GetMaxPos     (_iAxisNo                 );}
extern "C" SML_API double            __stdcall MT_GetRunVel     (int _iAxisNo                                 ){return MT.GetRunVel     (_iAxisNo                 );}
extern "C" SML_API double            __stdcall MT_GetManVel     (int _iAxisNo                                 ){return MT.GetManVel     (_iAxisNo                 );}



extern "C" SML_API 
void __stdcall MT_SetAxtTrgPos  (int _iAxisNo , int _iPosCnt , double * _dPos , double _dTrgTime , bool _bActual ,bool _bOnLevel)
{
    MT.SetAxtTrgPos(_iAxisNo , _iPosCnt , _dPos , _dTrgTime , _bActual ,_bOnLevel);
}

extern "C" SML_API 
void __stdcall MT_ResetAxtTrgPos(int _iAxisNo )
{
    MT.ResetAxtTrgPos(_iAxisNo) ;
}

extern "C" SML_API 
void __stdcall MT_OneShotAxtTrg (int _iAxisNo , bool _bUpEg , int _iTime)
{
    MT.OneShotAxtTrg(_iAxisNo , _bUpEg , _iTime) ;
}

extern "C" SML_API 
void __stdcall MT_SetIntrptPos(int _iAxisNo , double _dPos, PosCallbackFunc _pCallback)
{
    MT.SetIntrptPos(_iAxisNo,_dPos,_pCallback);
}

extern "C" SML_API 
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

extern "C" SML_API void               __stdcall AT_Reset       (void                                                        ){       AT.Reset     (            );}
extern "C" SML_API bool               __stdcall AT_Err         (int _iNo                                                    ){return AT.Err       (_iNo        );}
extern "C" SML_API bool               __stdcall AT_Complete    (int _iNo , bool _bCmd                                       ){return AT.Complete  (_iNo , _bCmd);}
extern "C" SML_API bool               __stdcall AT_Done        (int _iNo                                                    ){return AT.Complete  (_iNo        );}
extern "C" SML_API char *             __stdcall AT_GetName     (int _iNo                                                    ){return AT.GetName   (_iNo        ).c_str();}
extern "C" SML_API bool               __stdcall AT_MoveCyl     (int _iNo , bool _bCmd                                       ){return AT.Move      (_iNo , _bCmd);}
extern "C" SML_API bool               __stdcall AT_GetCmd      (int _iNo                                                    ){return AT.GetCmd    (_iNo        );}
extern "C" SML_API int                __stdcall AT_GetActCnt   (void                                                        ){return AT.GetActrCnt(            );}
extern "C" SML_API EN_ACTR_DIRECTION  __stdcall AT_GetDirType  (int _iNo                                                    ){return AT.GetDirType(_iNo        );}
//void        __stdcall AT_SetCntrlPn  (int _iActrNo , TPanel * _pnBase , EN_MOTR_CNTRL_TYPE _iType ){AT.SetCntrlPn (_iActrNo ,_pnBase , _iType ); }
//void        __stdcall AT_MoveCntrlPn (int _iActrNo , TPanel * _pnBase                             ){AT.MoveCntrlPn(_iActrNo ,_pnBase          );}
//void        __stdcall AT_DelCntrlPn  (int _iActrNo                                                ){AT.DelCntrlPn (_iActrNo                   );}
//
extern "C" SML_API void __stdcall IO_SetY    (int _iNo , bool _bVal){if(!IO.GetTestMode())IO.SetY  (_iNo ,_bVal , false);}
extern "C" SML_API void __stdcall IO_SetYDrct(int _iNo , bool _bVal){if(!IO.GetTestMode())IO.SetY  (_iNo ,_bVal , true );}

extern "C" SML_API bool __stdcall IO_GetY  (int _iNo                ){return IO.GetY  (_iNo          );}
extern "C" SML_API bool __stdcall IO_GetYDn(int _iNo                ){return IO.GetYDn(_iNo          );}
extern "C" SML_API bool __stdcall IO_GetYUp(int _iNo                ){return IO.GetYUp(_iNo          );}

extern "C" SML_API bool __stdcall IO_GetX    (int _iNo ){return IO.GetX  (_iNo ,false   );}
extern "C" SML_API bool __stdcall IO_GetXDrct(int _iNo ){return IO.GetX  (_iNo ,true    );}
extern "C" SML_API bool __stdcall IO_GetXDn  (int _iNo ){return IO.GetXDn(_iNo          );}
extern "C" SML_API bool __stdcall IO_GetXUp  (int _iNo ){return IO.GetXUp(_iNo          );}

extern "C" SML_API void  __stdcall IO_SetTestMode(bool _bOn) {IO.SetTestMode(_bOn);}
extern "C" SML_API bool  __stdcall IO_GetTestMode(         ) {return IO.GetTestMode();}

extern "C" SML_API char * __stdcall IO_GetXName  (int _iNo ) { return IO.GetXName(_iNo).c_str(); }
extern "C" SML_API char * __stdcall IO_GetYName  (int _iNo ) { return IO.GetYName(_iNo).c_str(); }

extern "C" SML_API void   __stdcall EM_Clear         (void     )                  { EM.Clear     (    )           ;}
extern "C" SML_API void   __stdcall EM_SetErr        (int  _iNo)                  { EM.SetErr    (_iNo)           ;}
extern "C" SML_API void   __stdcall EM_SetDisp       (bool _bOn)                  { EM.SetDisp   (_bOn)           ;}
extern "C" SML_API void   __stdcall EM_SetErrMsg     (int  _iNo , char * _sErrMsg){ EM.SetErrMsg (_iNo , _sErrMsg);}
extern "C" SML_API char * __stdcall EM_GetErrMsg     (int _iNo)                   { return EM.GetErrMsg (_iNo ).c_str(); }

extern "C" SML_API bool       __stdcall EM_IsErr         (void     ){return EM.IsErr         (    );}
extern "C" SML_API int        __stdcall EM_GetLastErr    (         ){return EM.GetLastErr    (    );}
extern "C" SML_API char *     __stdcall EM_GetLastErrName(         ){return EM.GetLastErrName(    ).c_str();}
extern "C" SML_API char *     __stdcall EM_GetErrName    (int _iNo ){return EM.GetErrName    (_iNo).c_str();}
extern "C" SML_API char *     __stdcall EM_GetErrAction  (int _iNo ){return EM.GetErrAction  (_iNo).c_str();}

extern "C" SML_API int        __stdcall EM_GetErrCnt     (        ){return EM.GetErrCnt();}
extern "C" SML_API int        __stdcall EM_GetMaxErr     (        ){return EM.GetMaxErr();}
extern "C" SML_API bool       __stdcall EM_GetErr        (int _iNo){return EM.GetErr(_iNo);}
//extern "C" SML_API void       __stdcall EM_DisplayList   (TListBox * _lbList){EM.DispErr(_lbList);}





extern "C" SML_API void       __stdcall TL_MuteSnd       (bool _bOn ){TL._bBuzzOff = _bOn ; }


extern "C" SML_API void   __stdcall AI_SetY  (int _iNo , double _dVal ){AI.SetOut(_iNo , _dVal);    }
extern "C" SML_API double __stdcall AI_GetY  (int _iNo                ){return AI.GetOut(_iNo );    }
extern "C" SML_API double __stdcall AI_GetX  (int _iNo                ){return AI.GetIn (_iNo );    }