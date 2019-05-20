//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormCamLightPara.h"
#include "FormVisnMain.h"
#include "Vision.h"
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

    if(_bToTable) {
        edUseCamNo    -> Text = VISN[m_iVisnId]->Para.iUseCamNo     ;
        edCamExposure -> Text = VISN[m_iVisnId]->Para.iCamExposure  ;
        edCntOffsetX  -> Text = VISN[m_iVisnId]->Para.iCntOffsetX   ;
        edCntOffsetY  -> Text = VISN[m_iVisnId]->Para.iCntOffsetY   ;

        edUseLightNo  -> Text = VISN[m_iVisnId]->Para.iUseLightNo   ;
        edUseLightCh1 -> Text = VISN[m_iVisnId]->Para.iUseLightCh1  ;
        edLtBright1   -> Text = VISN[m_iVisnId]->Para.iLtBright1    ;
        edUseLightCh2 -> Text = VISN[m_iVisnId]->Para.iUseLightCh2  ;
        edLtBright2   -> Text = VISN[m_iVisnId]->Para.iLtBright2    ;
        edUseLightCh3 -> Text = VISN[m_iVisnId]->Para.iUseLightCh3  ;
        edLtBright3   -> Text = VISN[m_iVisnId]->Para.iLtBright3    ;

    }
    else {
        int iTemp ;

        iTemp = edUseCamNo    -> Text.ToIntDef(0   ); if (VISN[m_iVisnId]->Para.iUseCamNo    != iTemp) { Trace("Operator" , "Both VISN_UseCamNo values are different."   ); }
        iTemp = edCamExposure -> Text.ToIntDef(1000); if (VISN[m_iVisnId]->Para.iCamExposure != iTemp) { Trace("Operator" , "Both VISN_CamExposure values are different."); }
        iTemp = edCntOffsetX  -> Text.ToIntDef(0   ); if (VISN[m_iVisnId]->Para.iCntOffsetX  != iTemp) { Trace("Operator" , "Both VISN_CntOffsetX values are different." ); }
        iTemp = edCntOffsetY  -> Text.ToIntDef(0   ); if (VISN[m_iVisnId]->Para.iCntOffsetY  != iTemp) { Trace("Operator" , "Both VISN_CntOffsetY values are different." ); }

        iTemp = edUseLightNo  -> Text.ToIntDef(0  ) ; if (VISN[m_iVisnId]->Para.iUseLightNo  != iTemp) { Trace("Operator" , "Both VISN_UseLightNo values are different." ); }
        iTemp = edUseLightCh1 -> Text.ToIntDef(1  ) ; if (VISN[m_iVisnId]->Para.iUseLightCh1 != iTemp) { Trace("Operator" , "Both VISN_UseLightCh1 values are different."); }
        iTemp = edLtBright1   -> Text.ToIntDef(100) ; if (VISN[m_iVisnId]->Para.iLtBright1   != iTemp) { Trace("Operator" , "Both VISN_LtBright1 values are different."  ); }
        iTemp = edUseLightCh2 -> Text.ToIntDef(2  ) ; if (VISN[m_iVisnId]->Para.iUseLightCh2 != iTemp) { Trace("Operator" , "Both VISN_UseLightCh2 values are different."); }
        iTemp = edLtBright2   -> Text.ToIntDef(100) ; if (VISN[m_iVisnId]->Para.iLtBright2   != iTemp) { Trace("Operator" , "Both VISN_LtBright2 values are different."  ); }
        iTemp = edUseLightCh3 -> Text.ToIntDef(3  ) ; if (VISN[m_iVisnId]->Para.iUseLightCh3 != iTemp) { Trace("Operator" , "Both VISN_UseLightCh3 values are different."); }
        iTemp = edLtBright3   -> Text.ToIntDef(100) ; if (VISN[m_iVisnId]->Para.iLtBright3   != iTemp) { Trace("Operator" , "Both VISN_LtBright3 values are different."  ); }

        VISN[m_iVisnId]->Para.iUseCamNo    = edUseCamNo    -> Text.ToIntDef(0   ) ;
        VISN[m_iVisnId]->Para.iCamExposure = edCamExposure -> Text.ToIntDef(1000) ;
        VISN[m_iVisnId]->Para.iCntOffsetX  = edCntOffsetX  -> Text.ToIntDef(0   ) ;
        VISN[m_iVisnId]->Para.iCntOffsetY  = edCntOffsetY  -> Text.ToIntDef(0   ) ;

        VISN[m_iVisnId]->Para.iUseLightNo  = edUseLightNo  -> Text.ToIntDef(0  ) ;
        VISN[m_iVisnId]->Para.iUseLightCh1 = edUseLightCh1 -> Text.ToIntDef(1  ) ;
        VISN[m_iVisnId]->Para.iLtBright1   = edLtBright1   -> Text.ToIntDef(100) ;
        VISN[m_iVisnId]->Para.iUseLightCh2 = edUseLightCh2 -> Text.ToIntDef(2  ) ;
        VISN[m_iVisnId]->Para.iLtBright2   = edLtBright2   -> Text.ToIntDef(100) ;
        VISN[m_iVisnId]->Para.iUseLightCh3 = edUseLightCh3 -> Text.ToIntDef(3  ) ;
        VISN[m_iVisnId]->Para.iLtBright3   = edLtBright3   -> Text.ToIntDef(100) ;

        ApplyPara(true);
    }




}

void TFrmCamLightPara::Show(int _iVisnId)
{
    m_iVisnId = _iVisnId ;

    Caption = AnsiString(_iVisnId) + AnsiString(" Vision");

    TForm::Show();


}







void __fastcall TFrmCamLightPara::FormShow(TObject *Sender)
{

    ApplyPara(true);
}
//---------------------------------------------------------------------------

void __fastcall TFrmCamLightPara::btLotEndClick(TObject *Sender)
{
    Trace("Operator", "FormCamLightPara Form_Lot End Button Click");
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmCamLightPara::btSaveDeviceClick(TObject *Sender)
{
    ApplyPara(false);
    VISN[m_iVisnId]->LoadCamPara(false);
    Trace("Operator", "FormCamLightPara Form_SaveDevice Button Click");
}
//---------------------------------------------------------------------------


void __fastcall TFrmCamLightPara::FormDestroy(TObject *Sender)
{
    static int i = 0 ;
    Trace("",AnsiString(i).c_str());
    i++;

}
//---------------------------------------------------------------------------

