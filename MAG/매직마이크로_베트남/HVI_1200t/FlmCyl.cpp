//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FlmCyl.h"
//---------------------------------------------------------------------------
//Dll Reference Header
//Log Manager
//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
#include "SlogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrameCyl *FrameCyl;
//---------------------------------------------------------------------------
//__fastcall TFrameCyl::TFrameCyl(TComponent* Owner)
//        : TFrame(Owner)
__fastcall TFrameCyl::TFrameCyl(void) : TFrame(Owner)
{
    m_iId   = 0 ;
    m_iType = 0 ;
}
//---------------------------------------------------------------------------
bool TFrameCyl::BitBtnVisible(bool _bBool) //술이 안깨서 별짓 다하고 있음. dd
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

}
bool TFrameCyl::SetIdType(EN_ACTR_ID _iId , EN_DISP_TYPE _iType )
{
    m_iId   = (int)_iId  ;
    m_iType = _iType ;

    BitBtnVisible(false);                

    switch(_iType){
      default : break ;
      case dtCCwCw_X : //CCw 가 백이라고 생각해줘.dd
            bt1_Left     -> Visible = true  ;
            bt2_Right    -> Visible = true  ;
            bt2_Right    -> Left = 66 ;
            bt2_Right    -> Tag  = 1  ;
      break ;
      case dtCwCCw_X :
            bt1_Left     -> Visible = true  ;
            bt2_Right    -> Visible = true  ;
            bt2_Right    -> Left = 66 ;
            bt1_Left     -> Tag=1;
      break ;
      case dtCCwCw_Y :
            bt1_Forward  -> Visible = true ;
            bt2_Backward -> Visible = true ;
            bt2_Backward -> Top  = 60;
            bt2_Backward -> Tag = 1;
      break ;
      case dtCwCCw_Y :
            bt1_Forward  -> Visible = true ;
            bt2_Backward -> Visible = true ;
            bt2_Backward -> Top  = 60;
            bt1_Forward  -> Tag = 1;
      break ;
      case dtCCwCw_Z :
            bt1_Up -> Visible = true ;
            bt2_Dn -> Visible = true ;
            bt2_Dn -> Top = 60;
            bt2_Dn -> Tag = 1;
      break ;
      case dtCwCCw_Z :
            bt1_Up -> Visible = true ;
            bt2_Dn -> Visible = true ;
            bt2_Dn -> Top = 60 ;
            bt1_Up -> Tag = 1;
      break ;
      case dtCCwCw_L :
            bt1_RLeft  -> Visible = true ;
            bt2_RRight -> Visible = true ;
            bt2_RRight -> Left = 66 ;
            bt2_RRight -> Tag  = 1;
      break ;
        case dtCwCCw_L :
            bt1_RLeft  -> Visible = true ;
            bt2_RRight -> Visible = true ;
            bt2_RRight -> Left = 66 ;
            bt1_RLeft  -> Tag  = 1;
        break ;
    }

    Width  = 130 ;
    Height = 130 ;

    tmUpdate -> Enabled = true ;

    return true ;
}
void __fastcall TFrameCyl::btClick (TObject *Sender)
{
    if(!CheckSafe(m_iId))
    {
        String sTemp = (String)m_iId +" Cylinder CheckSafe Fail" ;
        Trace("TFrameCyl",sTemp.c_str());
        return ;
    }

    int iTag ;
    iTag = ((TBitBtn *)Sender) -> Tag ;
    AT_MoveCyl(m_iId , iTag) ;
}
void __fastcall TFrameCyl::tmUpdateTimer(TObject *Sender)
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
bool TFrameCyl::CheckSafe(int _iId)
{
    bool bRet = true ;

         if(_iId == aiLDR_ClmpUpDn   ) bRet = true ;
    else if(_iId == aiLDR_PshrFwBw   ) bRet = true ;
    else if(_iId == aiPRB_StprUpDn   ) bRet = true ;
    else if(_iId == aiWK1_StprUpDn   ) bRet = true ;
    else if(_iId == aiWK1_AlgnFwBw   ) bRet = true ;
    else if(_iId == aiWK1_SttnUpDn   ) bRet = true ;
    else if(_iId == aiWK2_StprUpDn   ) bRet = true ;
    else if(_iId == aiWK2_AlgnFwBw   ) bRet = true ;
    else if(_iId == aiWK2_SttnUpDn   ) bRet = true ;
    else if(_iId == aiWK3_StprUpDn   ) bRet = true ;
    else if(_iId == aiWK3_AlgnFwBw   ) bRet = true ;
    else if(_iId == aiWK3_SttnUpDn   ) bRet = true ;
    else if(_iId == aiPSB_AlgnFwBw   ) bRet = true ;
    else if(_iId == aiPSB_IndxUpDn   ) bRet = true ;
    else if(_iId == aiPSB_FlprOpCl   ) bRet = true ;
    else if(_iId == aiPSB_GripUpDn   ) bRet = true ;
    else if(_iId == aiPSB_PnchDnUp   ) bRet = true ;
    else if(_iId == aiPSB_SortFwBw   ) bRet = true ;
    else if(_iId == aiPSB_SrtStprFwBw) bRet = true ;
    else if(_iId == aiPSB_PshrFwBw   ) bRet = true ;
    else if(_iId == aiPSB_PshrDnUp   ) bRet = true ;
    else if(_iId == aiULD_ClmpUpDn   ) bRet = true ;

    return bRet ;
}



