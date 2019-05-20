//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrameMotr.h" //너의 이름은 Frm 이여야 하나 ㅠㅠ ...쩝...dd 선대리님이 Flm 이랫음 dd 잇힝 어느새 바껴 있넹 ㅎ
//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "SlogUnit.h"
#include "PstnMan.h"
//---------------------------------------------------------------------------
//Dll Reference Header
//Log Manager
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
bool TFraMotr::SetIdType(EN_MOTR_ID _iId , EN_MTOR_DIRECTION _iType )
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

    if(_iType == mdLR || _iType == mdRL)
    {
        btLeft -> Caption   = "LEFT"    ; btLeft -> Glyph = m_pBwimg ; btLeft    -> OnMouseUp = btMtStop  ;
        btRigh -> Caption   = "RIGHT"   ; btRigh -> Glyph = m_pFwimg ; btRigh    -> OnMouseUp = btMtStop  ;
    }
    else if(_iType == mdBF || _iType == mdFB)
    {
        btLeft -> Caption   = "BACKWARD"; btLeft -> Glyph = m_pUpimg ; btLeft    -> OnMouseUp = btMtStop  ;
        btRigh -> Caption   = "FORWARD" ; btRigh -> Glyph = m_pDnimg ; btRigh    -> OnMouseUp = btMtStop  ;
    }
    else if(_iType == mdDU || _iType == mdUD)
    {
        btLeft -> Caption   = "DOWN"    ; btLeft -> Glyph = m_pDnimg ; btLeft    -> OnMouseUp = btMtStop  ;
        btRigh -> Caption   = "UP"      ; btRigh -> Glyph = m_pUpimg ; btRigh    -> OnMouseUp = btMtStop  ;
    }
    else if(_iType == mdAC || _iType == mdCA)
    {
        btLeft -> Caption   = "ActiC"   ; btLeft -> Glyph = m_pLlimg ; btLeft    -> OnMouseUp = btMtStop  ;
        btRigh -> Caption   = "Clock"   ; btRigh -> Glyph = m_pLrimg ; btRigh    -> OnMouseUp = btMtStop  ;
    }

    if(_iType == mdLR || _iType == mdBF || _iType == mdDU || _iType == mdCA )
    {
        btLeft -> OnMouseDown = btBwMove ; //이게 틀릴가능성이 매우 높음.
        btRigh -> OnMouseDown = btFwMove ;
    }
    else if(_iType == mdRL || _iType == mdFB || _iType == mdUD || _iType == mdAC )
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
    LbStat2 -> Color   = MT_GetHomeSnsr (m_iId) ? clLime : clSilver ;
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

    if(_iId == miLDR_XPsh ) if(!AT_Complete(aiLDR_Pusher , ccBwd)) { sMsg = "Pusher is not Bwd" ; bRet = false ;}
    if(_iId == miLDR_YFed ) if(!AT_Complete(aiLDR_Pusher , ccBwd)) { sMsg = "Pusher is not Bwd" ; bRet = false ;}
    if(_iId == miLDR_XPck ) if( MT_GetCmdPos(miLDR_ZPck) != PM.GetValue(miLDR_ZPck , pvLDR_ZPckMove   )) { sMsg = "Z Motor is Not Move Position" ; bRet = false;}
    if(_iId == miLDR_ZPck ) bRet = true;
    if(_iId == miWK1_YFlp ) if(MT_GetCmdPos(miWRK_YVsn) != PM.GetValue(miWRK_YVsn , pvWRK_YVsnWait   )) { sMsg = "Vision X Motor is Not Wait Position" ; bRet = false;}
    if(_iId == miWRK_XVsn ) if(!IO_GetX(xWR1_TurnDetect)) { sMsg = "Flipper is Not Move Position" ; bRet = false;}
    if(_iId == miWRK_YVsn ) if(!IO_GetX(xWR1_TurnDetect)) { sMsg = "Flipper is Not Move Position" ; bRet = false;}
    if(_iId == miPSB_XMrk ) if(!AT_Complete(aiPSB_Marking , ccBwd)) { sMsg = "Marking Cylinder is Not Bwd" ; bRet = false; }
    if(_iId == miPSB_YMrk ) if(!AT_Complete(aiPSB_Marking , ccBwd)) { sMsg = "Marking Cylinder is Not Bwd" ; bRet = false; }
    if(_iId == miWK1_XIns ) if(!IO_GetX(xWR1_TurnDetect)) { sMsg = "Flipper is Not Move Position" ; bRet = false;}
    if(_iId == miULD_ZPck ) bRet = true;
    if(_iId == miULD_YPck ) if(MT_GetCmdPos(miULD_ZPck) != PM.GetValue(miULD_ZPck , pvULD_ZPckMove   )) { sMsg = "Pick Z Motor is Not Move Position" ; bRet = false;}
    if(_iId == miULD_ZTrI ) bRet = true;
    if(_iId == miULD_ZTrO ) bRet = true;
    if(_iId == miULD_XStp ) bRet = true;
    if(_iId == miULD_ZPpr ) bRet = true;

    if(!bRet){
        Trace(MT_GetName(_iId).c_str(), sMsg.c_str());
        FM_MsgOk("CheckSafe",sMsg);
    }
    return bRet ;
}

