//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrameMotr.h" //너의 이름은 Frm 이여야 하나 ㅠㅠ ...쩝...dd 선대리님이 Flm 이랫음 dd 잇힝 어느새 바껴 있넹 ㅎ
//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "SlogUnit.h"
//---------------------------------------------------------------------------
//Dll Reference Header
//Log Manager
//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "SlogUnit.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"
TFraMotr *FraMotr;
//---------------------------------------------------------------------------
//__fastcall TFrameMotr::TFrameMotr(TComponent* Owner)
//        : TFrame(Owner)
//{
__fastcall TFraMotr::TFraMotr(void) : TFrame(Owner)
{

    m_iId      = 0 ;
    m_iType    = 0 ;
    tmUpdate -> Enabled = false ;
}
//---------------------------------------------------------------------------
bool TFraMotr::SetIdType(EN_MOTR_ID _iId , EN_MOTR_DISP_TYPE _iType )
{
    m_iId      = (int)_iId  ;
    m_iType    = _iType ;

    m_pFwimg = new Graphics::TBitmap() ;
    m_pBwimg = new Graphics::TBitmap() ;
    m_pUpimg = new Graphics::TBitmap() ;
    m_pDnimg = new Graphics::TBitmap() ;
    m_pLlimg = new Graphics::TBitmap() ;
    m_pLrimg = new Graphics::TBitmap() ;

    ImgLstBt -> GetBitmap(0,m_pBwimg  ); //레프트 라이트 업 다운 레프트R 라이트R
    ImgLstBt -> GetBitmap(1,m_pFwimg  );
    ImgLstBt -> GetBitmap(2,m_pUpimg  );
    ImgLstBt -> GetBitmap(3,m_pDnimg  );
    ImgLstBt -> GetBitmap(4,m_pLlimg  );
    ImgLstBt -> GetBitmap(5,m_pLrimg  );

    if(_iType == mdtCCwCw_X || _iType == mdtCwCCw_X)
    {
        btLeft -> Caption   = "LEFT"    ; btLeft -> Glyph = m_pBwimg ; btLeft    -> OnMouseUp = btMtStop  ;
        btRigh -> Caption   = "RIGHT"   ; btRigh -> Glyph = m_pFwimg ; btRigh    -> OnMouseUp = btMtStop  ;
    }
    else if(_iType == mdtCCwCw_Y || _iType == mdtCwCCw_Y)
    {
        btLeft -> Caption   = "FORWARD" ; btLeft -> Glyph = m_pUpimg ; btLeft    -> OnMouseUp = btMtStop  ;
        btRigh -> Caption   = "BACKWARD"; btRigh -> Glyph = m_pDnimg ; btRigh    -> OnMouseUp = btMtStop  ;
    }
    else if(_iType == mdtCCwCw_Z || _iType == mdtCwCCw_Z)
    {
        btLeft -> Caption   = "UP"      ; btLeft -> Glyph = m_pUpimg ; btLeft    -> OnMouseUp = btMtStop  ;
        btRigh -> Caption   = "DOWN"    ; btRigh -> Glyph = m_pDnimg ; btRigh    -> OnMouseUp = btMtStop  ;
    }
    else if(_iType == mdtCCwCw_L || _iType == mdtCwCCw_L)
    {
        btLeft -> Caption   = "LEFT"    ; btLeft -> Glyph = m_pLlimg ; btLeft    -> OnMouseUp = btMtStop  ;
        btRigh -> Caption   = "RIGHT"   ; btRigh -> Glyph = m_pLrimg ; btRigh    -> OnMouseUp = btMtStop  ;
    }

    if(_iType == mdtCCwCw_X || _iType == mdtCCwCw_Y || _iType == mdtCCwCw_Z || _iType == mdtCCwCw_L )
    {
        btLeft -> OnMouseDown = btBwMove ; //이게 틀릴가능성이 매우 높음.
        btRigh -> OnMouseDown = btFwMove ;
    }
    else if(_iType == mdtCwCCw_X || _iType == mdtCwCCw_Y || _iType == mdtCwCCw_Z || _iType == mdtCwCCw_L )
    {
        btLeft -> OnMouseDown = btFwMove ;
        btRigh -> OnMouseDown = btBwMove ;
    }
    else return false ;

    tmUpdate -> Enabled = true ;

    delete m_pFwimg ; m_pFwimg = NULL ;
    delete m_pBwimg ; m_pBwimg = NULL ;
    delete m_pUpimg ; m_pUpimg = NULL ;
    delete m_pDnimg ; m_pDnimg = NULL ;
    delete m_pLlimg ; m_pLlimg = NULL ;
    delete m_pLrimg ; m_pLrimg = NULL ;

    return true ;
}
void TFraMotr::SetUnit(EN_UNIT_TYPE _iUnitType , double _dUnit)
{
    dUnit     = _dUnit     ;
    iUnitType = _iUnitType ;
}
void __fastcall TFraMotr::btBwMove(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(!CheckSafe(m_iId)) return ;

         if(iUnitType == utJog  ) MT_JogN    (m_iId) ;
    else if(iUnitType == utMove ) MT_GoIncMan(m_iId , -dUnit) ;
    else return ;

}
void __fastcall TFraMotr::btFwMove(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(!CheckSafe(m_iId)) return ;

         if(iUnitType == utJog  ) MT_JogP    (m_iId) ;
    else if(iUnitType == utMove ) MT_GoIncMan(m_iId ,  dUnit) ;
    else return ;
}
void __fastcall TFraMotr::btMtStop(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(iUnitType == utJog  ) MT_Stop(m_iId) ;
}

void __fastcall TFraMotr::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    LbStat1 -> Color   = MT_GetNLimSnsr (m_iId) ? clLime : clSilver ;
    LbStat2 -> Color   = MT_GetHomeEnd  (m_iId) ? clLime : clSilver ;
    LbStat3 -> Color   = MT_GetPLimSnsr (m_iId) ? clLime : clSilver ;
    LbStat4 -> Color   = MT_GetAlarm    (m_iId) ? clLime : clSilver ;
    LbStat5 -> Color   = MT_GetServo    (m_iId) ? clLime : clSilver ;
    LbStat6 -> Color   = MT_GetStopInpos(m_iId) ? clLime : clSilver ;
    LbStat7 -> Color   = MT_GetHomeEnd  (m_iId) ? clLime : clSilver ;

    LbCmdPos   -> Caption = MT_GetCmdPos   (m_iId) ;
    LbEncPos   -> Caption = MT_GetEncPos   (m_iId) ;

    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------
bool TFraMotr::CheckSafe(int _iId)
{
    bool bRet = true ;
    AnsiString sMsg = "";

         if(_iId == (int)miLDR_Z) bRet = true ;
    else if(_iId == (int)miPCK_Y) {
        if(!AT_Complete(aiPCK_PckrDU,ccBwd)) {sMsg = "During Picker Down"; bRet = false ;}
    }
    else if(_iId == (int)miRAL_X) bRet = true ;
    else if(_iId == (int)miSRT_X) {
        if(!AT_Complete(aiSRT_FlipFB , ccBwd)) { sMsg = "Flipper is not Bwd" ; bRet = false ; }
    }

    if(!bRet){
        Trace(MT_GetName(_iId).c_str(), sMsg.c_str());
        FM_MsgOk("CheckSafe",sMsg);
    }
    return bRet ;
}
