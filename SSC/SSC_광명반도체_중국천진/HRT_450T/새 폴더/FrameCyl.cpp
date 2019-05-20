//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrameCyl.h"
//---------------------------------------------------------------------------
//Dll Reference Header
//Log Manager
//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "SlogUnit.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Head.h"
#include "Stage.h"
#include "UnLoader430t.h"
#include "PostBuff.h"
#include "Loader.h"
#include "SortingTool.h"
#include "WorkZone.h"
#include "PreBuff.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFraCyl *FraCyl;
//---------------------------------------------------------------------------
//__fastcall TFrameCyl::TFrameCyl(TComponent* Owner)
//        : TFrame(Owner)
__fastcall TFraCyl::TFraCyl(void) : TFrame(Owner)
{
    m_iId   = 0 ;
    m_iType = 0 ;
}
//---------------------------------------------------------------------------
bool TFraCyl::BitBtnVisible(bool _bBool) //술이 안깨서 별짓 다하고 있음. dd
{
    String sComName ;

    for (int i = 0; i < ComponentCount; i++)
    {
        if (Components[i]->ClassNameIs("TBitBtn"))
        {
            sComName = Components[i]->ClassName();
            ((TBitBtn*)FindComponent(Components[i]->Name))->Visible = _bBool  ;
            ((TBitBtn*)FindComponent(Components[i]->Name))->Left    = 5       ; //넌 덤이다.dd
            ((TBitBtn*)FindComponent(Components[i]->Name))->Top     = 5       ;
            ((TBitBtn*)FindComponent(Components[i]->Name))->Tag     = 0       ;
            ((TBitBtn*)FindComponent(Components[i]->Name))->OnClick = btClick ;
        }
    }
    return true ;
}
bool TFraCyl::SetIdType(EN_ACTR_ID _iId , EN_ACTR_DIRECTION _iType )
{
    m_iId   = (int)_iId  ;
    m_iType = _iType ;

    BitBtnVisible(false);

    //Direction (Bwd , Fwd)
    if(_iType == adLR || _iType == adRL ) {
        bt1_Left     -> Visible = true ;
        bt2_Right    -> Visible = true ;
        bt2_Right    -> Left    = 86   ;
        if(_iType == adLR) bt2_Right    -> Tag  = 1  ;
        if(_iType == adRL) bt1_Left     -> Tag  = 1  ;
    }
    else if(_iType == adBF || _iType == adFB ) {
        bt1_Backward  -> Visible = true ;
        bt2_Forward   -> Visible = true ;
        bt2_Forward   -> Top     = 74   ;
        if(_iType == adBF) bt2_Forward   -> Tag  = 1  ;
        if(_iType == adFB) bt1_Backward  -> Tag  = 1  ;
    }
    else if(_iType == adUD || _iType == adDU ) {
        bt1_Up       -> Visible = true ;
        bt2_Dn       -> Visible = true ;
        bt2_Dn       -> Top     = 74   ;
        if(_iType == adUD) bt2_Dn       -> Tag  = 1  ;
        if(_iType == adDU) bt1_Up       -> Tag  = 1  ;
    }
    else if(_iType == adCA || _iType == adAC ) {
        bt1_RLeft    -> Visible = true ;
        bt2_RRight   -> Visible = true ;
        bt2_RRight   -> Left    = 86   ;
        if(_iType == adCA) bt2_RRight   -> Tag  = 1  ;
        if(_iType == adAC) bt1_RLeft    -> Tag  = 1  ;
    }
    else if(_iType == adBW) {
        bt3_Backward -> Visible = true ;
        bt3_Backward -> Left    = 86   ;
        bt3_Backward -> Tag     = 1    ;
    }
    else if(_iType == adFw) {
        bt3_Forward  -> Visible = true ;
        bt3_Forward  -> Left    = 86   ;
        bt3_Forward  -> Tag     = 1    ;
    }


    else return false ;

    Width  = 170 ;
    Height = 150 ;

    tmUpdate -> Enabled = true ;

    return true ;
}
void __fastcall TFraCyl::btClick (TObject *Sender)
{
    int iTag ;
    iTag = ((TBitBtn *)Sender) -> Tag ;
    if(!CheckSafe(m_iId , iTag)) return ;

    AT_MoveCyl(m_iId , iTag) ;
    Trace("Operator", "FrameCyl Form_Cylinder Move Button Click");
}
void __fastcall TFraCyl::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;
    //For 문 돌리려다가...봐줌...그래도 직접 접근하는게 빠르겟지...dd
    bt1_Left    ->Font->Color = AT_Complete(m_iId ,bt1_Left    ->Tag) ? clLime : clBlack ;
    bt2_Right   ->Font->Color = AT_Complete(m_iId ,bt2_Right   ->Tag) ? clLime : clBlack ;
    bt1_RLeft   ->Font->Color = AT_Complete(m_iId ,bt1_RLeft   ->Tag) ? clLime : clBlack ;
    bt2_RRight  ->Font->Color = AT_Complete(m_iId ,bt2_RRight  ->Tag) ? clLime : clBlack ;
    bt1_Up      ->Font->Color = AT_Complete(m_iId ,bt1_Up      ->Tag) ? clLime : clBlack ;
    bt2_Dn      ->Font->Color = AT_Complete(m_iId ,bt2_Dn      ->Tag) ? clLime : clBlack ;
    bt1_Backward->Font->Color = AT_Complete(m_iId ,bt1_Backward->Tag) ? clLime : clBlack ;
    bt2_Forward ->Font->Color = AT_Complete(m_iId ,bt2_Forward ->Tag) ? clLime : clBlack ;
    bt3_Backward->Font->Color = AT_Complete(m_iId ,bt3_Backward->Tag) ? clLime : clBlack ;
    bt3_Forward ->Font->Color = AT_Complete(m_iId ,bt3_Forward ->Tag) ? clLime : clBlack ;


    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------
bool TFraCyl::CheckSafe(int _iId , bool _bFwd)
{
    bool bRet = true ;
    AnsiString sMsg = "";

         if(_iId == (int)aiPRB_Cmp   ) { bRet = PRB.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }
    else if(_iId == (int)aiPRB_IdChk ) { bRet = PRB.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }
    else if(_iId == (int)aiWRK_Cmp   ) { bRet = WRK.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }
    else if(_iId == (int)aiPSB_Cmp   ) { bRet = PSB.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }
    else if(_iId == (int)aiLDR_Cmp   ) { bRet = LDR.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }
    else if(_iId == (int)aiLDR_Psh   ) { bRet = LDR.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }
    else if(_iId == (int)aiSTL_Top   ) { bRet = STL.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }
    else if(_iId == (int)aiSTL_Btm   ) { bRet = STL.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }
    else if(_iId == (int)aiPSB_Psh   ) { bRet = PSB.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }
    else if(_iId == (int)aiULD_Cmp   ) { bRet = ULD.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }



    return bRet ;
}















