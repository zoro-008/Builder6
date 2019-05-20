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
#include "Loader.h"
#include "Rail.h"
#include "UnLoader.h"
#include "Picker.h"
#include "Sorter.h"
#include "Supply.h"
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
bool TFraCyl::SetIdType(EN_ACTR_ID _iId , EN_CYL_DISP_TYPE _iType )
{
    m_iId   = (int)_iId  ;
    m_iType = _iType ;

    BitBtnVisible(false);                

    //Direction (Bwd , Fwd)
    if(_iType == cdtBwFw_X || _iType == cdtFwBw_X ) {
        bt1_Left     -> Visible = true  ;
        bt2_Right    -> Visible = true  ;
        bt2_Right    -> Left = 66 ;
        if(_iType == cdtBwFw_X) bt2_Right -> Tag  = 1  ;
        if(_iType == cdtFwBw_X) bt1_Left  -> Tag  = 1  ;
    }
    else if(_iType == cdtBwFw_Y || _iType == cdtFwBw_Y ) {
        bt1_Forward  -> Visible = true ;
        bt2_Backward -> Visible = true ;
        bt2_Backward -> Top  = 60;
        if(_iType == cdtBwFw_Y) bt2_Backward -> Tag  = 1  ;
        if(_iType == cdtFwBw_Y) bt1_Forward  -> Tag  = 1  ;
    }
    else if(_iType == cdtBwFw_Z || _iType == cdtFwBw_Z ) {
        bt1_Up -> Visible = true ;
        bt2_Dn -> Visible = true ;
        bt2_Dn -> Top = 60 ;
        if(_iType == cdtBwFw_Z) bt2_Dn  -> Tag  = 1  ;
        if(_iType == cdtFwBw_Z) bt1_Up  -> Tag  = 1  ;
    }
    else if(_iType == cdtBwFw_L || _iType == cdtFwBw_L ) {
        bt1_RLeft  -> Visible = true ;
        bt2_RRight -> Visible = true ;
        bt2_RRight -> Left = 66 ;
        if(_iType == cdtBwFw_L) bt2_RRight -> Tag  = 1  ;
        if(_iType == cdtFwBw_L) bt1_RLeft  -> Tag  = 1  ;
    }
    else return false ;

    Width  = 130 ;
    Height = 130 ;

    tmUpdate -> Enabled = true ;

    return true ;
}
void __fastcall TFraCyl::btClick (TObject *Sender)
{
    int iTag ;
    iTag = ((TBitBtn *)Sender) -> Tag ;
    if(!CheckSafe(m_iId , iTag)) return ;

    AT_MoveCyl(m_iId , iTag) ;
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
    bt1_Forward ->Font->Color = AT_Complete(m_iId ,bt1_Forward ->Tag) ? clLime : clBlack ;
    bt2_Backward->Font->Color = AT_Complete(m_iId ,bt2_Backward->Tag) ? clLime : clBlack ;

    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------
bool TFraCyl::CheckSafe(int _iId , bool _bFwd)
{
    bool bRet = true ;
    AnsiString sMsg = "";

         if(_iId == (int)aiLDR_Sptr) {
             if(_bFwd = ccBwd && IO_GetX(xLDR_TrayDetect)) {
                 if(FM_MsgYesNo("Confirm","Loader Tray is Exist . Are you sure ?") != mrYes) bRet = false ;
             }
             if(!LDR.CheckSafe((EN_ACTR_ID)_iId,_bFwd)) bRet = false ;
         }
    else if(_iId == (int)aiPCK_UpDn) {if(!PCK.CheckSafe((EN_ACTR_ID)_iId,_bFwd)) bRet = false ; }
    else if(_iId == (int)aiRAL_Indx) {if(!RAL.CheckSafe((EN_ACTR_ID)_iId,_bFwd)) bRet = false ; }
    else if(_iId == (int)aiSRT_Grip) {if(!SRT.CheckSafe((EN_ACTR_ID)_iId,_bFwd)) bRet = false ; }
    else if(_iId == (int)aiSRT_Flip) {if(!SRT.CheckSafe((EN_ACTR_ID)_iId,_bFwd)) bRet = false ; }
    else if(_iId == (int)aiSPL_LBn0) {if(!SPL.CheckSafe((EN_ACTR_ID)_iId,_bFwd)) bRet = false ; }
    else if(_iId == (int)aiSPL_LBn1) {if(!SPL.CheckSafe((EN_ACTR_ID)_iId,_bFwd)) bRet = false ; }
    else if(_iId == (int)aiSPL_LBn2) {if(!SPL.CheckSafe((EN_ACTR_ID)_iId,_bFwd)) bRet = false ; }
    else if(_iId == (int)aiSPL_LBn3) {if(!SPL.CheckSafe((EN_ACTR_ID)_iId,_bFwd)) bRet = false ; }
    else if(_iId == (int)aiSPL_LBn4) {if(!SPL.CheckSafe((EN_ACTR_ID)_iId,_bFwd)) bRet = false ; }
    else if(_iId == (int)aiSPL_LBn5) {if(!SPL.CheckSafe((EN_ACTR_ID)_iId,_bFwd)) bRet = false ; }
    else if(_iId == (int)aiSPL_LBn6) {if(!SPL.CheckSafe((EN_ACTR_ID)_iId,_bFwd)) bRet = false ; }
    else if(_iId == (int)aiSPL_LBn7) {if(!SPL.CheckSafe((EN_ACTR_ID)_iId,_bFwd)) bRet = false ; }
    else if(_iId == (int)aiSPL_RBn0) {if(!SPL.CheckSafe((EN_ACTR_ID)_iId,_bFwd)) bRet = false ; }
    else if(_iId == (int)aiSPL_RBn1) {if(!SPL.CheckSafe((EN_ACTR_ID)_iId,_bFwd)) bRet = false ; }
    else if(_iId == (int)aiSPL_RBn2) {if(!SPL.CheckSafe((EN_ACTR_ID)_iId,_bFwd)) bRet = false ; }
    else if(_iId == (int)aiSPL_RBn3) {if(!SPL.CheckSafe((EN_ACTR_ID)_iId,_bFwd)) bRet = false ; }
    else if(_iId == (int)aiSPL_RBn4) {if(!SPL.CheckSafe((EN_ACTR_ID)_iId,_bFwd)) bRet = false ; }
    else if(_iId == (int)aiSPL_RBn5) {if(!SPL.CheckSafe((EN_ACTR_ID)_iId,_bFwd)) bRet = false ; }
    else if(_iId == (int)aiSPL_RBn6) {if(!SPL.CheckSafe((EN_ACTR_ID)_iId,_bFwd)) bRet = false ; }
    else if(_iId == (int)aiSPL_RBn7) {if(!SPL.CheckSafe((EN_ACTR_ID)_iId,_bFwd)) bRet = false ; }
    else if(_iId == (int)aiULD_Stck) {if(!ULD.CheckSafe((EN_ACTR_ID)_iId,_bFwd)) bRet = false ; }

//    if(!bRet){
//        Trace(MT_GetName(_iId).c_str(), sMsg.c_str());
//        FM_MsgOk("CheckSafe",sMsg);
//    }
    return bRet ;
}



