//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FlmMotr.h" //너의 이름은 Frm 이여야 하나 ㅠㅠ ...쩝...dd 선대리님이 Flm 이랫음 dd
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
TFrameMotr *FrameMotr;
//---------------------------------------------------------------------------
//__fastcall TFrameMotr::TFrameMotr(TComponent* Owner)
//        : TFrame(Owner)
//{
__fastcall TFrameMotr::TFrameMotr(void) : TFrame(Owner)
{

    m_iId      = 0 ;
    m_iType    = 0 ;
    tmUpdate -> Enabled = false ;
}
//---------------------------------------------------------------------------
bool TFrameMotr::SetIdType(EN_MOTR_ID _iId , EN_DISP_TYPE _iType )
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

    switch(_iType){
        case dtCCwCw_X :
            btBwdMove -> Caption = "LEFT(MOVE)"     ; btBwdMove -> Glyph = m_pBwimg ;
            btFwdMove -> Caption = "RIGHT(MOVE)"    ; btFwdMove -> Glyph = m_pFwimg ;
            btBwdJog  -> Caption = "LEFT(JOG)"      ; btBwdJog  -> Glyph = m_pBwimg ;
            btFwdJog  -> Caption = "RIGHT(JOG)"     ; btFwdJog  -> Glyph = m_pFwimg ;

            btBwdMove -> OnClick     = btBwMoveClick ; //이게 틀릴가능성이 매우 높음.
            btFwdMove -> OnClick     = btFwMoveClick ;
//            btBwdMove -> OnMouseUp   = btMtStop   ;
//            btFwdMove -> OnMouseUp   = btMtStop   ;

            btBwdJog  -> OnMouseDown = btBwJogDown;
            btFwdJog  -> OnMouseDown = btFwJogDown;
            btBwdJog  -> OnMouseUp   = btMtStop   ;
            btFwdJog  -> OnMouseUp   = btMtStop   ;

        break ;
        case dtCwCCw_X :
            btBwdMove -> Caption = "LEFT(MOVE)"     ; btBwdMove -> Glyph = m_pBwimg ;
            btFwdMove -> Caption = "RIGHT(MOVE)"    ; btFwdMove -> Glyph = m_pFwimg ;
            btBwdJog  -> Caption = "LEFT(JOG)"      ; btBwdJog  -> Glyph = m_pBwimg ;
            btFwdJog  -> Caption = "RIGHT(JOG)"     ; btFwdJog  -> Glyph = m_pFwimg ;

            btBwdMove -> OnClick     = btFwMoveClick ;
            btFwdMove -> OnClick     = btBwMoveClick ;

            btBwdJog  -> OnMouseUp   = btMtStop      ;
            btFwdJog  -> OnMouseUp   = btMtStop      ;
            btBwdJog  -> OnMouseDown = btFwJogDown ;
            btFwdJog  -> OnMouseDown = btBwJogDown ;

        break ;
        case dtCCwCw_Y :
            btBwdMove -> Caption = "BACKWARD(MOVE)" ; btBwdMove -> Glyph = m_pUpimg ;
            btFwdMove -> Caption = "FORWARD(MOVE)"  ; btFwdMove -> Glyph = m_pDnimg ;
            btBwdJog  -> Caption = "BACKWARD(JOG)"  ; btBwdJog  -> Glyph = m_pUpimg ;
            btFwdJog  -> Caption = "FORWARD(JOG)"   ; btFwdJog  -> Glyph = m_pDnimg ;

            btBwdMove -> OnClick     = btBwMoveClick ;
            btFwdMove -> OnClick     = btFwMoveClick ;

            btBwdJog -> OnMouseUp   = btMtStop   ;
            btFwdJog -> OnMouseUp   = btMtStop   ;

            btBwdJog  -> OnMouseDown = btBwJogDown ;
            btFwdJog  -> OnMouseDown = btFwJogDown ;

        break ;
        case dtCwCCw_Y :
            btBwdMove -> Caption = "BACKWARD(MOVE)" ; btBwdMove -> Glyph = m_pUpimg ;
            btFwdMove -> Caption = "FORWARD(MOVE)"  ; btFwdMove -> Glyph = m_pDnimg ;
            btBwdJog  -> Caption = "BACKWARD(JOG)"  ; btBwdJog  -> Glyph = m_pUpimg ;
            btFwdJog  -> Caption = "FORWARD(JOG)"   ; btFwdJog  -> Glyph = m_pDnimg ;

            btBwdMove -> OnClick     = btFwMoveClick ;
            btFwdMove -> OnClick     = btBwMoveClick ;

            btBwdJog -> OnMouseUp   = btMtStop   ;
            btFwdJog -> OnMouseUp   = btMtStop   ;

            btBwdJog  -> OnMouseDown = btFwJogDown ;
            btFwdJog  -> OnMouseDown = btBwJogDown ;

        break ;
        case dtCCwCw_Z :
            btBwdMove -> Caption = "UP(MOVE)"       ; btBwdMove -> Glyph = m_pUpimg ;
            btFwdMove -> Caption = "DOWN(MOVE)"     ; btFwdMove -> Glyph = m_pDnimg ;
            btBwdJog  -> Caption = "UP(JOG)"        ; btBwdJog  -> Glyph = m_pUpimg ;
            btFwdJog  -> Caption = "DOWN(JOG)"      ; btFwdJog  -> Glyph = m_pDnimg ;

            btBwdMove -> OnClick     = btBwMoveClick ;
            btFwdMove -> OnClick     = btFwMoveClick ;
            btBwdJog -> OnMouseUp   = btMtStop   ;
            btFwdJog -> OnMouseUp   = btMtStop   ;

            btBwdJog  -> OnMouseDown = btBwJogDown ;
            btFwdJog  -> OnMouseDown = btFwJogDown ;

        break ;
        case dtCwCCw_Z :
            btBwdMove -> Caption = "UP(MOVE)"       ; btBwdMove -> Glyph = m_pUpimg ;
            btFwdMove -> Caption = "DOWN(MOVE)"     ; btFwdMove -> Glyph = m_pDnimg ;
            btBwdJog  -> Caption = "UP(JOG)"        ; btBwdJog  -> Glyph = m_pUpimg ;
            btFwdJog  -> Caption = "DOWN(JOG)"      ; btFwdJog  -> Glyph = m_pDnimg ;

            btBwdMove -> OnClick     = btFwMoveClick ;
            btFwdMove -> OnClick     = btBwMoveClick ;
            btBwdJog -> OnMouseUp   = btMtStop   ;
            btFwdJog -> OnMouseUp   = btMtStop   ;

            btBwdJog  -> OnMouseDown = btFwJogDown ;
            btFwdJog  -> OnMouseDown = btBwJogDown ;

        break ;
        case dtCCwCw_L :
            btBwdMove -> Caption = "LEFT(MOVE)"     ; btBwdMove -> Glyph = m_pLlimg ;
            btFwdMove -> Caption = "RIGHT(MOVE)"    ; btFwdMove -> Glyph = m_pLrimg ;
            btBwdJog  -> Caption = " LEFT(JOG)"     ; btBwdJog  -> Glyph = m_pLlimg ;
            btFwdJog  -> Caption = "RIGHT(JOG)"     ; btFwdJog  -> Glyph = m_pLrimg ;

            btBwdMove -> OnClick     = btBwMoveClick ;
            btFwdMove -> OnClick     = btFwMoveClick ;
            btBwdJog -> OnMouseUp   = btMtStop   ;
            btFwdJog -> OnMouseUp   = btMtStop   ;

            btBwdJog  -> OnMouseDown = btBwJogDown ;
            btFwdJog  -> OnMouseDown = btFwJogDown ;

        break ;
        case dtCwCCw_L :
            btBwdMove -> Caption = "LEFT(MOVE)"     ; btBwdMove -> Glyph = m_pLlimg ;
            btFwdMove -> Caption = "RIGHT(MOVE)"    ; btFwdMove -> Glyph = m_pLrimg ;
            btBwdJog  -> Caption = " LEFT(JOG)"     ; btBwdJog  -> Glyph = m_pLlimg ;
            btFwdJog  -> Caption = "RIGHT(JOG)"     ; btFwdJog  -> Glyph = m_pLrimg ;

            btBwdMove -> OnClick     = btFwMoveClick ;
            btFwdMove -> OnClick     = btBwMoveClick ;
            btBwdJog -> OnMouseUp   = btMtStop   ;
            btFwdJog -> OnMouseUp   = btMtStop   ;

            btBwdJog  -> OnMouseDown = btFwJogDown ;
            btFwdJog  -> OnMouseDown = btBwJogDown ;

        break ;
    }

    tmUpdate -> Enabled = true ;

    delete m_pFwimg ; m_pFwimg = NULL ;
    delete m_pBwimg ; m_pBwimg = NULL ;
    delete m_pUpimg ; m_pUpimg = NULL ;
    delete m_pDnimg ; m_pDnimg = NULL ;
    delete m_pLlimg ; m_pLlimg = NULL ;
    delete m_pLrimg ; m_pLrimg = NULL ;

    return true ;
}
void __fastcall TFrameMotr::btBwJogDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(!CheckSafe(m_iId))
    {
        String sTemp = (String)m_iId +" Motor CheckSafe Fail" ;
        Trace("TFrameMotr",sTemp.c_str());
        return ;
    }
    MT_JogN(m_iId) ;
}
void __fastcall TFrameMotr::btFwJogDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(!CheckSafe(m_iId))
    {
        String sTemp = (String)m_iId +" Motor CheckSafe Fail" ;
        Trace("TFrameMotr",sTemp.c_str());
        return ;
    }
    MT_JogP(m_iId) ;
}
void __fastcall TFrameMotr::btBwMoveClick(TObject *Sender)
{
    if(!CheckSafe(m_iId))
    {
        String sTemp = (String)m_iId +" Motor CheckSafe Fail" ;
        Trace("TFrameMotr",sTemp.c_str());
        return ;
    }

    double dUnit = 0.0 ;
    dUnit = edMtIncPos -> Text.ToDouble();
    MT_GoIncMan(m_iId, -dUnit);
}
void __fastcall TFrameMotr::btFwMoveClick(TObject *Sender)
{
    if(!CheckSafe(m_iId))
    {
        String sTemp = (String)m_iId +" Motor CheckSafe Fail" ;
        Trace("TFrameMotr",sTemp.c_str());
        return ;
    }

    double dUnit = 0.0 ;
    dUnit = edMtIncPos -> Text.ToDouble();
    MT_GoIncMan(m_iId,  dUnit);
}
void __fastcall TFrameMotr::btMtStop(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    MT_Stop(m_iId) ;
}

void __fastcall TFrameMotr::tmUpdateTimer(TObject *Sender)
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
void __fastcall TFrameMotr::UpDownChangingEx(TObject *Sender,
      bool &AllowChange, short NewValue, TUpDownDirection Direction)
{
    double Temp;
    double Inc  = (double)(UpDown->Increment)/1000;
    int    iSel;

    int iDirection = Direction ; //1 이 Up 이고 2 가 Down

    if(iDirection == 1)
    {
        Temp = edMtIncPos -> Text.ToDouble();
        Temp += Inc;
        edMtIncPos->Text = Temp;
    }
    else
    {
        Temp = edMtIncPos -> Text.ToDouble();
        Temp -= Inc;
        edMtIncPos->Text = Temp;
    }
}
//---------------------------------------------------------------------------
bool TFrameMotr::CheckSafe(int _iId)
{
    bool bRet = true ;
         if(_iId == miWRK_ZPch) bRet = true ;
    else if(_iId == miPRB_XIdx) bRet = true ;
    else if(_iId == miPRB_TFlp) bRet = true ;
    else if(_iId == miWRK_XSrt) bRet = true ;
    else if(_iId == miPRI_XIdx) bRet = true ;
    else if(_iId == miPSI_XIdx) bRet = true ;

    return bRet ;
}
void __fastcall TFrameMotr::btStopClick(TObject *Sender)
{
    MT_Stop(m_iId) ;
}
//---------------------------------------------------------------------------

