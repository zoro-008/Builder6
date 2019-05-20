//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormCamLightPara.h"
//#include "FormVisnMain.h"
#include "VisionBase.h"
#include "SLogUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrmCamLightPara *FrmCamLightPara = NULL;
//---------------------------------------------------------------------------
__fastcall TFrmCamLightPara::TFrmCamLightPara(TComponent* Owner)
        : TForm(Owner)
{
    Left = 100 ;
    Top  = 100 ;
}

//---------------------------------------------------------------------------
//__fastcall TFrmCamLightPara::TFrmCamLightPara(TComponent* Owner , int _iVisnId)
//        : TForm(Owner)
//{
//}


void TFrmCamLightPara::ApplyPara(bool _bToTable)
{
    TCamLightPara   DPara = VISN[m_iVisnId]->GetCamLightPara();
    TVisnStaticPara SPara = VISN[m_iVisnId]->GetVisnStaticPara();

    if(_bToTable) {
        edUseCamId    -> Text = SPara.iUseCamId     ;
        edCamExposure -> Text = DPara.iCamExposure  ;
        edCntOffsetX  -> Text = DPara.iCntOffsetX   ;
        edCntOffsetY  -> Text = DPara.iCntOffsetY   ;

        edUseLightId  -> Text = SPara.iUseLightId   ;
        edUseLightCh1 -> Text = DPara.iUseLightCh1  ;
        edLtBright1   -> Text = DPara.iLtBright1    ;
        edUseLightCh2 -> Text = DPara.iUseLightCh2  ;
        edLtBright2   -> Text = DPara.iLtBright2    ;
        edUseLightCh3 -> Text = DPara.iUseLightCh3  ;
        edLtBright3   -> Text = DPara.iLtBright3    ;

    }
    else {
        //SPara.iUseCamId    = edUseCamNo    -> Text.ToIntDef(0   ) ;
        DPara.iCamExposure = edCamExposure -> Text.ToIntDef(1000) ;
        DPara.iCntOffsetX  = edCntOffsetX  -> Text.ToIntDef(0   ) ;
        DPara.iCntOffsetY  = edCntOffsetY  -> Text.ToIntDef(0   ) ;

        //SPara.iUseLightId  = edUseLightNo  -> Text.ToIntDef(0  ) ;
        DPara.iUseLightCh1 = edUseLightCh1 -> Text.ToIntDef(1  ) ;
        DPara.iLtBright1   = edLtBright1   -> Text.ToIntDef(100) ;
        DPara.iUseLightCh2 = edUseLightCh2 -> Text.ToIntDef(2  ) ;
        DPara.iLtBright2   = edLtBright2   -> Text.ToIntDef(100) ;
        DPara.iUseLightCh3 = edUseLightCh3 -> Text.ToIntDef(3  ) ;
        DPara.iLtBright3   = edLtBright3   -> Text.ToIntDef(100) ;

        VISN[m_iVisnId]->SetCamLightPara(DPara);

        ApplyPara(true);
    }




}

void TFrmCamLightPara::Show(EN_VISN_ID _iVisnId)
{
    m_iVisnId = _iVisnId ;

    Caption = VISN[_iVisnId]->GetVisnStaticPara().sVisnName + AnsiString(" Cam&&Light");

    TForm::Show();

    ApplyPara(true);
}








void __fastcall TFrmCamLightPara::btCloseClick(TObject *Sender)
{
    //Trace("Operator", "FormCamLightPara Form_Lot End Button Click");
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmCamLightPara::btSaveClick(TObject *Sender)
{
    ApplyPara(false);
    VISN[m_iVisnId]->LoadCamLightPara(false);
    //Trace("Operator", "FormCamLightPara Form_SaveDevice Button Click");
}
//---------------------------------------------------------------------------



