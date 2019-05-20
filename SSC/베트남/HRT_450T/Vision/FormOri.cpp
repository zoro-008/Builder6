//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormOri.h"
#include "BfPlce.h"
#include "VOptnMan.h"
#include "SLogUnit.h"
#include "FormImage.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFrmOri *FrmOri;
//---------------------------------------------------------------------------
__fastcall TFrmOri::TFrmOri(TComponent* Owner)
        : TForm(Owner)
{

}

void TFrmOri::Init()
{
    Left = 0 ;
    Top  = 0 ;

    m_iVisnId = viOri ;

    //OriginalProc = pnMag->WindowProc ;
    //pnMag->WindowProc = PanelProc   ;
    //m_pMagImg = IMG_New() ;


    FrmTrain = new TFrmTrainChk (pnTrain);
    FrmTrain -> SetTrainArea ( ((CBfPlce *)VISN[m_iVisnId])-> Orig.pTrArea );
    FrmTrain -> SetTrainImg  ( ((CBfPlce *)VISN[m_iVisnId])-> Orig.pTrImage);
    FrmTrain -> BorderStyle = bsNone ;
    FrmTrain -> Align = alClient ;
    FrmTrain -> Parent = pnTrain ;
    //FrmTrain -> Left = 0 ;
    //FrmTrain -> Top  = 0 ;

    FrmTrain -> Show() ;

    tmUpdate -> Enabled = true ;

}

void TFrmOri::Close()
{
    //pnMag->WindowProc = OriginalProc ;
    //IMG_Del(m_pMagImg);

    FrmTrain -> Close();
    delete FrmTrain ;

    TForm::Close();
}


//---------------------------------------------------------------------------

void TFrmOri::ApplyPara(bool _bToTable)
{
    TCamLightPara   DPara = VISN[m_iVisnId]->GetCamLightPara();
    TVisnStaticPara SPara = VISN[m_iVisnId]->GetVisnStaticPara();

    lbCh1 -> Visible = DPara.iUseLightCh1 != 0;
    lbCh2 -> Visible = DPara.iUseLightCh2 != 0;
    lbCh3 -> Visible = DPara.iUseLightCh3 != 0;

    lbUseLightCh1 -> Visible = DPara.iUseLightCh1 != 0;
    lbUseLightCh2 -> Visible = DPara.iUseLightCh2 != 0;
    lbUseLightCh3 -> Visible = DPara.iUseLightCh3 != 0;

    edLtBright1 -> Visible = DPara.iUseLightCh1 != 0;
    edLtBright2 -> Visible = DPara.iUseLightCh2 != 0;
    edLtBright3 -> Visible = DPara.iUseLightCh3 != 0;




    if(_bToTable) {
        //Cam Light Para
        edUseCamId    -> Text    = SPara.iUseCamId    ;
        edCamExposure -> Value   = DPara.iCamExposure ;
        edCntOffsetX  -> Value   = DPara.iCntOffsetX  ;
        edCntOffsetY  -> Value   = DPara.iCntOffsetY  ;

        edUseLightId  -> Text    = SPara.iUseLightId  ;

        lbUseLightCh1 -> Caption = DPara.iUseLightCh1 ;
        edLtBright1   -> Value   = DPara.iLtBright1   ;
        lbUseLightCh2 -> Caption = DPara.iUseLightCh2 ;
        edLtBright2   -> Value   = DPara.iLtBright2   ;
        lbUseLightCh3 -> Caption = DPara.iUseLightCh3 ;
        edLtBright3   -> Value   = DPara.iLtBright3   ;

        //Common Option
        cbImageSave   -> ItemIndex = VOM.VCmnOptn.iOriImageSave      ;

        //Dev Optn
        edOriSinc       -> Value     = VOM.VDevOptn.dOriOriSinc        ;

    }
    else {
        //Cam Light Para
        DPara.iCamExposure = edCamExposure -> Value ;
        DPara.iCntOffsetX  = edCntOffsetX  -> Value ;
        DPara.iCntOffsetY  = edCntOffsetY  -> Value ;

        DPara.iLtBright1   = edLtBright1   -> Value ;
        DPara.iLtBright2   = edLtBright2   -> Value ;
        DPara.iLtBright3   = edLtBright3   -> Value ;

        VISN[m_iVisnId]->SetCamLightPara(DPara);




        //Common Option
        VOM.VCmnOptn.iOriImageSave         = cbImageSave     -> ItemIndex ;

        VOM.VDevOptn.dOriOriSinc           = edOriSinc       -> Value     ;

        ApplyPara(true);
    }
}


void __fastcall TFrmOri::btSaveClick(TObject *Sender)
{
    ApplyPara(false);

    VOM.LoadJobFile(false);

    //Trace("Operator", "FormVisionPara Form_Save Button Click");
}
//---------------------------------------------------------------------------

void __fastcall TFrmOri::FormShow(TObject *Sender)
{

    ApplyPara(true);

}
//---------------------------------------------------------------------------

void TFrmOri::FormShow(EN_PAGE_ID _iPage)
{
    pcSet -> ActivePageIndex = _iPage ;
    Show();
}














void __fastcall TFrmOri::pcSetChange(TObject *Sender)
{
    VISN[m_iVisnId]->SetActivPage((EN_PAGE_ID)pcSet -> ActivePageIndex) ;
    btTrain -> Visible = pcSet -> ActivePageIndex == piOriOcv ;

    FrmImage[m_iVisnId] -> OnPaint();
}
//---------------------------------------------------------------------------

void __fastcall TFrmOri::btTrainClick(TObject *Sender)
{
    if(VISN[m_iVisnId]->GetActivPage() == piOriLCS) {
    }
    else if(VISN[m_iVisnId]->GetActivPage() == piOriOcv) {
        VISN[m_iVisnId]->Train();
        FrmTrain -> OnPaint() ;
    }
}

//---------------------------------------------------------------------------


void __fastcall TFrmOri::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    tmUpdate -> Enabled = true ;
}
//---------------------------------------------------------------------------


void __fastcall TFrmOri::btTrackerInitClick(TObject *Sender)
{
    VISN[m_iVisnId]-> TrackerInit() ;
}
//---------------------------------------------------------------------------

