//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FrameCyl.h"
//---------------------------------------------------------------------------
//Dll Reference Header
//Log Manager
//---------------------------------------------------------------------------
#include "SMInterfaceUnit.h"
//#include "SlogUnit.h"
//---------------------------------------------------------------------------
//Part Reference
//---------------------------------------------------------------------------
#include "Loader.h"
#include "PreBuff.h"
#include "RailEpx.h"
#include "Epoxy.h"
#include "RailDie.h"
#include "Head.h"
#include "Stage.h"
#include "PostBuff.h"
#include "UnLoader.h"
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
bool TFraCyl::BitBtnVisible(bool _bBool) //���� �ȱ��� ���� ���ϰ� ����. dd
{
    String sComName ;

    for (int i = 0; i < ComponentCount; i++)
    {
        if (Components[i]->ClassNameIs("TBitBtn"))
        {
            sComName = Components[i]->ClassName();
            ((TBitBtn*)FindComponent(Components[i]->Name))->Visible = _bBool  ;
            ((TBitBtn*)FindComponent(Components[i]->Name))->Left    = 5       ; //�� ���̴�.dd
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
    /*
    enum EN_ACTR_DIRECTION {
    adLR = 0 , //���鿡��   ������ Left �� - Right�� +
    adRL     , //���鿡��   ������ Right�� - Left �� +
    adBF     , //���鿡��   ������ Bwd  �� - Fwd  �� +
    adFB     , //���鿡��   ������ Fwd  �� - Bwd  �� +
    adUD     , //���鿡��   ������ Up   �� - Down �� +
    adDU     , //���鿡��   ������ Down �� - Up   �� +
    adCA     , //ȸ���࿡�� ������ Clock�� - AntiC�� +
    adAC       //ȸ���࿡�� ������ AntiC�� - Clock�� +
    };

    */





    //Direction (Bwd , Fwd)
    if(_iType == adLR || _iType == adRL ) {
        bt1_Left     -> Visible = true  ;
        bt2_Right    -> Visible = true  ;
        bt2_Right    -> Left = 66 ;
        if(_iType == adLR) bt2_Right -> Tag  = 1  ;
        if(_iType == adRL) bt1_Left  -> Tag  = 1  ;
    }
    else if(_iType == adBF || _iType == adFB ) {
        bt1_Forward  -> Visible = true ;
        bt2_Backward -> Visible = true ;
        bt2_Backward -> Top  = 60;
//        bt1_Forward -> Top  = 60;

        //if(_iType == adBF) bt2_Backward -> Tag  = 1  ;
        //if(_iType == adFB) bt1_Forward  -> Tag  = 1  ;

        if(_iType == adBF) bt2_Backward -> Tag  = 1  ;
        if(_iType == adFB) bt1_Forward  -> Tag  = 1  ;
    }
    else if(_iType == adUD || _iType == adDU ) {
        bt1_Up -> Visible = true ;
        bt2_Dn -> Visible = true ;
        bt2_Dn -> Top = 60 ;
        if(_iType == adUD) bt2_Dn  -> Tag  = 1  ;
        if(_iType == adDU) bt1_Up  -> Tag  = 1  ;
    }
    else if(_iType == adCA || _iType == adAC ) {
        bt1_RLeft  -> Visible = true ;
        bt2_RRight -> Visible = true ;
        bt2_RRight -> Left = 66 ;
        if(_iType == adCA) bt2_RRight -> Tag  = 1  ;
        if(_iType == adAC) bt1_RLeft  -> Tag  = 1  ;
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
    //For �� �������ٰ�...����...�׷��� ���� �����ϴ°� ��������...dd
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

         if(_iId == (int)aiLDR_Stopper  ) { bRet = LDR.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }
    else if(_iId == (int)aiLDR_MgzGrip  ) { bRet = LDR.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }
    else if(_iId == (int)aiLDR_Out      ) { bRet = LDR.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }
    else if(_iId == (int)aiLDR_Pusher   ) { bRet = LDR.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }
    else if(_iId == (int)aiPRB_Stopper  ) { bRet = PRB.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }
    else if(_iId == (int)aiPRB_Clamp    ) { }//bRet = PRB.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; } //���� �̤�
    //else if(_iId == (int)aiWRE_BAlign   ) { bRet = RAE.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }
    else if(_iId == (int)aiWRE_Clamp    ) { }//bRet = RAE.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }
    else if(_iId == (int)aiWRD_Clamp    ) { }//bRet = RAD.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }
    else if(_iId == (int)aiPSB_Clamp    ) { }//bRet = PSB.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }
    else if(_iId == (int)aiEJT_UpDn     ) { bRet = STG.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }
    else if(_iId == (int)aiTRS_Chuck    ) { bRet = STG.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }
    else if(_iId == (int)aiULD_Stopper  ) { bRet = ULD.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }
    else if(_iId == (int)aiULD_MgzGrip  ) { bRet = ULD.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }
    else if(_iId == (int)aiULD_Out      ) { bRet = ULD.CheckSafe((EN_ACTR_ID)_iId,_bFwd) ; }

    return bRet ;
}


