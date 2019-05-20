//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrameMotr.h" //너의 이름은 Frm 이여야 하나 ㅠㅠ ...쩝...dd 선대리님이 Flm 이랫음 dd 잇힝 어느새 바껴 있넹 ㅎ
//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "SlogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFraMotr *FraMotr;
//---------------------------------------------------------------------------
__fastcall TFraMotr::TFraMotr(TComponent* Owner)
        : TFrame(Owner)
{
//__fastcall TFraMotr::TFraMotr(void) : TFrame(Owner)
//{
//
    m_iId      = 0 ;
    m_iType    = 0 ;
    dPitch     = 0.0 ;
    tmUpdate -> Enabled = false ;
}




//---------------------------------------------------------------------------
bool TFraMotr::SetIdType(EN_MOTR_ID _iId , EN_MTOR_DIRECTION _iType )
{
    m_iId      = (int)_iId  ;
    m_iType    = _iType ;                                                                 
                                                                                          
    m_pLtimg = new Graphics::TBitmap() ;
    m_pRtimg = new Graphics::TBitmap() ;
    m_pUpimg = new Graphics::TBitmap() ;
    m_pDnimg = new Graphics::TBitmap() ;
    m_pClimg = new Graphics::TBitmap() ;
    m_pAcimg = new Graphics::TBitmap() ;

    ImgLstBt -> GetBitmap(0,m_pLtimg ); //레프트 라이트 업 다운 레프트R 라이트R
    ImgLstBt -> GetBitmap(1,m_pRtimg );
    ImgLstBt -> GetBitmap(2,m_pUpimg );
    ImgLstBt -> GetBitmap(3,m_pDnimg );
    ImgLstBt -> GetBitmap(4,m_pAcimg );
    ImgLstBt -> GetBitmap(5,m_pClimg );

    /*
    //정면에서   봤을때 Left 가 - Right가 +
    //정면에서   봤을때 Right가 - Left 가 +
    //정면에서   봤을때 Bwd  가 - Fwd  가 +
    //정면에서   봤을때 Fwd  가 - Bwd  가 +
    //정면에서   봤을때 Up   가 - Down 가 +
    //정면에서   봤을때 Down 가 - Up   가 +
    //회전축에서 봤을때 Clock가 - AntiC가 +
    //회전축에서 봤을때 AntiC가 - Clock가 +
    */

    /*
        case mdLR : btJogN -> Glyph = pLtImg ; btJogP -> Glyph = pRtImg ; break ;
        case mdRL : btJogN -> Glyph = pRtImg ; btJogP -> Glyph = pLtImg ; break ;
        case mdBF : btJogN -> Glyph = pUpImg ; btJogP -> Glyph = pDnImg ; break ;
        case mdFB : btJogN -> Glyph = pDnImg ; btJogP -> Glyph = pUpImg ; break ;
        case mdUD : btJogN -> Glyph = pUpImg ; btJogP -> Glyph = pDnImg ; break ;
        case mdDU : btJogN -> Glyph = pDnImg ; btJogP -> Glyph = pUpImg ; break ;
        case mdCA : btJogN -> Glyph = pCwImg ; btJogP -> Glyph = pAwImg ; break ;
        case mdAC : btJogN -> Glyph = pAwImg ; btJogP -> Glyph = pCwImg ; break ;
    */

    //mdLR   L=Fwd , R=Bwd
    if(_iType == mdLR)
    {
        btNeg -> Caption   = "LEFT"    ; btNeg -> Glyph = m_pLtimg ; //btFwd    -> OnMouseUp = btMtStop  ;
        btPos -> Caption   = "RIGHT"   ; btPos -> Glyph = m_pRtimg ; //btNeg    -> OnMouseUp = btMtStop  ;

    }
    else if(_iType == mdRL)
    {
        btNeg -> Caption   = "RIGHT"   ; btNeg -> Glyph = m_pRtimg ; //btNeg    -> OnMouseUp = btMtStop  ;
        btPos -> Caption   = "LEFT"    ; btPos -> Glyph = m_pLtimg ; //btPos    -> OnMouseUp = btMtStop  ;
    }
    else if(_iType == mdBF)
    {
        btNeg -> Caption   = "BWD"     ; btNeg -> Glyph = m_pUpimg ; //btNeg    -> OnMouseUp = btMtStop  ;
        btPos -> Caption   = "FWD"     ; btPos -> Glyph = m_pDnimg ; //btPos    -> OnMouseUp = btMtStop  ;
    }
    else if(_iType == mdFB)
    {
        btNeg -> Caption   = "FWD"     ; btNeg -> Glyph = m_pDnimg ; //btNeg    -> OnMouseUp = btMtStop  ;
        btPos -> Caption   = "BWD"     ; btPos -> Glyph = m_pUpimg ; //btPos    -> OnMouseUp = btMtStop  ;
    }
    else if(_iType == mdDU)
    {
        btNeg -> Caption   = "DN"      ; btNeg -> Glyph = m_pDnimg ; //btNeg    -> OnMouseUp = btMtStop  ;
        btPos -> Caption   = "UP"      ; btPos -> Glyph = m_pUpimg ; //btPos    -> OnMouseUp = btMtStop  ;
    }
    else if(_iType == mdUD)
    {
        btNeg -> Caption   = "UP"      ; btNeg -> Glyph = m_pUpimg ; //btNeg    -> OnMouseUp = btMtStop  ;
        btPos -> Caption   = "DN"      ; btPos -> Glyph = m_pDnimg ; //btPos    -> OnMouseUp = btMtStop  ;
    }
    else if(_iType == mdAC)
    {
        btNeg -> Caption   = "CCW"     ; btNeg -> Glyph = m_pAcimg ; //btNeg    -> OnMouseUp = btMtStop  ;
        btPos -> Caption   = "CW"      ; btPos -> Glyph = m_pClimg ; //btPos    -> OnMouseUp = btMtStop  ;
    }
    else if(_iType == mdCA)
    {
        btNeg -> Caption   = "CW"      ; btNeg -> Glyph = m_pClimg ; //btNeg    -> OnMouseUp = btMtStop  ;
        btPos -> Caption   = "CCW"     ; btPos -> Glyph = m_pAcimg ; //btPos    -> OnMouseUp = btMtStop  ;
    }

    if(_iType == mdLR || _iType == mdFB || _iType == mdDU || _iType == mdCA )
    {
        //btPos -> OnMouseDown = btBwMove ; //이게 틀릴가능성이 매우 높음.
        //btNeg -> OnMouseDown = btFwMove ;
    }
    else if(_iType == mdRL || _iType == mdBF || _iType == mdUD || _iType == mdAC )
    {
        //btPos -> OnMouseDown = btFwMove ;
        //btNeg -> OnMouseDown = btBwMove ;
    }
    else return false ;

    tmUpdate -> Enabled = true ;

    delete m_pLtimg ; m_pLtimg = NULL ;
    delete m_pRtimg ; m_pRtimg = NULL ;
    delete m_pUpimg ; m_pUpimg = NULL ;
    delete m_pDnimg ; m_pDnimg = NULL ;
    delete m_pClimg ; m_pClimg = NULL ;
    delete m_pAcimg ; m_pAcimg = NULL ;

    return true ;
}
void TFraMotr::SetPitch(double _dUnit)
{
    dPitch = _dUnit ;
}
void TFraMotr::SetUnit(EN_UNIT_TYPE _iUnitType , double _dUnit)
{
    dUnit     = _dUnit     ;
    iUnitType = _iUnitType ;

}

void __fastcall TFraMotr::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    LbStat1 -> Color   = MT_GetNLimSnsr (m_iId) ? clLime : clSilver ;
    LbStat2 -> Color   = MT_GetHomeSnsr (m_iId) ? clLime : clSilver ;
    LbStat3 -> Color   = MT_GetPLimSnsr (m_iId) ? clLime : clSilver ;
    LbStat4 -> Color   = MT_GetAlarm    (m_iId) ? clLime : clSilver ;
    LbStat5 -> Color   = MT_GetServo    (m_iId) ? clLime : clSilver ;
    LbStat6 -> Color   = MT_GetStop     (m_iId) ? clLime : clSilver ;
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
    //나중에 외부에서 콜백으로 엮기   지금은 귀찮다.]

    //if(_iId == (int)miHED_XHed ) {
    //    if(!AT_MoveCyl(aiHED_PenMrkDU   , ccBwd)) return false ;
    //    if(!AT_MoveCyl(aiHED_PenCoverUD , ccBwd)) return false ;
    //}
    //else if(_iId == (int)miHED_YHed ) {
    //    if(!AT_MoveCyl(aiHED_PenMrkDU ,   ccBwd)) return false ;
    //    if(!AT_MoveCyl(aiHED_PenCoverUD , ccBwd)) return false ;
    //}








//    if(_iId == (int)miWRD_ZExd) {
//        if(OM.MstOptn.dStgYMaxExdAble <= MT_GetCmdPos(miWRD_YStg) bRet = false ;
//    }
//    if(_iId == miWRD_XStg || _iId == miWRD_YStg || _iId == miWRD_TStg ||
//       _iId == miWRD_XTrs || _iId == miWRD_ZCst || _iId == miWRD_ZExd) {
//        if(!STG.CheckSafe((EN_MOTR_ID)_iId)) bRet = false ;
//    }

/*
         if(_iId == (int)miLDR_Z) bRet = true ;
    else if(_iId == (int)miPCK_Y) {
        if(!AT_Complete(aiPCK_UpDn,ccBwd)) {sMsg = "During Picker Down"; bRet = false ;}
    }
    else if(_iId == (int)miRAL_X) bRet = true ;
    else if(_iId == (int)miSRT_X) {
        if(!AT_Complete(aiSRT_Flip , ccBwd)) { sMsg = "Flipper is not Bwd" ; bRet = false ; }
    }
*/
//    if(!bRet){
//        Trace(MT_GetName(_iId).c_str(), sMsg.c_str());
//        FM_MsgOk("CheckSafe",sMsg);
//    }
    return bRet ;
}


void __fastcall TFraMotr::btPosMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(iUnitType == utJog  ) MT_Stop(m_iId) ;
}
//---------------------------------------------------------------------------

void __fastcall TFraMotr::btNegMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(iUnitType == utJog  ) MT_Stop(m_iId) ;
}
//---------------------------------------------------------------------------


void __fastcall TFraMotr::btNegMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    AnsiString sTemp;
    if(!CheckSafe(m_iId)) return ;

    MT_Stop(m_iId) ;

    sTemp = m_iId;

    Trace("Operator", ("FrameMotr Form_" + sTemp + "Motor Neg Move Button Click").c_str());

         if(iUnitType == utJog   ) MT_JogN    (m_iId) ;
    else if(iUnitType == utMove  ) MT_GoIncMan(m_iId , -dUnit ) ;
    else if(iUnitType == utPitch ) MT_GoIncMan(m_iId , -dPitch) ;
    else return ;
}
//---------------------------------------------------------------------------

void __fastcall TFraMotr::btPosMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    AnsiString sTemp;
    if(!CheckSafe(m_iId)) return ;

    MT_Stop(m_iId) ;

    sTemp = m_iId ;

    Trace("Operator", ("FrameMotr Form_" + sTemp + "Motor Pos Move Button Click").c_str());

         if(iUnitType == utJog   ) MT_JogP    (m_iId) ;
    else if(iUnitType == utMove  ) MT_GoIncMan(m_iId , dUnit ) ;
    else if(iUnitType == utPitch ) MT_GoIncMan(m_iId , dPitch) ;
    else return ;


}
//---------------------------------------------------------------------------





