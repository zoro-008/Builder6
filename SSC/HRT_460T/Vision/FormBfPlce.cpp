//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormBfPlce.h"
#include "BfPlce.h"
#include "VOptnMan.h"
#include "SLogUnit.h"
#include "FormImage.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFrmBfPlce *FrmBfPlce;
//---------------------------------------------------------------------------
__fastcall TFrmBfPlce::TFrmBfPlce(TComponent* Owner)
        : TForm(Owner)
{

}

void TFrmBfPlce::Init()
{
    Left = 0 ;
    Top  = 0 ;

    m_iVisnId = viBfPlce ;

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

void TFrmBfPlce::Close()
{
    //pnMag->WindowProc = OriginalProc ;
    //IMG_Del(m_pMagImg);

    FrmTrain -> Close();
    delete FrmTrain ;

    TForm::Close();
}

/*
void __fastcall TFrmEpoxy::PanelProc(Messages::TMessage &msg)
{
    OriginalProc(msg);
    if (msg.Msg == WM_PAINT ) {
        OnPaintMag();
    }
}*/

/*void TFrmEpoxy::OnPaintMag()
{

    if(m_pMagImg  == NULL) return ;


    RECT R ;
    R.left   = 0 ;
    R.top    = 0 ;
    R.right  = pnMag -> Width  ;
    R.bottom = pnMag -> Height ;


    // Create buffer with memory DC and bitmap, then clear it with background.
    HDC     hDc ;
    HDC     hMemDc ;
    HBITMAP hMemBm , hOldMemBm ;


    HWND Temphwnd =  pnMag->Handle ;

    hDc       = GetDC(pnMag->Handle); //imDraw -> Canvas -> Handle ;  //GetDC(pnMag->Handle);
    hMemDc    = CreateCompatibleDC(hDc);
    hMemBm    = CreateCompatibleBitmap(hDc, R.right, R.bottom);
    hOldMemBm = SelectObject(hMemDc, hMemBm);

    m_pMagImg ->ShowImgGrey  (hMemDc, R.left , R.top , R.right , R.bottom);

    //GDI ±×¸².
    IGdi * Gdi = GDI_New(hMemDc);
    Gdi -> SetScale(pnMag->Width/m_pMagImg->GetWidth() , pnMag->Height / m_pMagImg->GetHeight());
    int iPx = 0 ;
    int iThresholdLow  = sbThresholdLow  -> Position ;
    int iThresholdHigh = sbThresholdHigh -> Position ;
    int iPxMinPer      = sbPxMinPer      -> Position ;
    int iPxInCnt = 0 ;
    int iAllPxCnt = m_pMagImg->GetHeight() * m_pMagImg->GetHeight() ;
    TColor tColor ;

    for(int y = 0 ; y < m_pMagImg->GetHeight() ; y++) {
        for(int x = 0 ; x < m_pMagImg->GetWidth() ; x++) {
            iPx = m_pMagImg-> GetPixel(x,y);
            if(iThresholdLow < iPx && iPx <=iThresholdHigh){
                iPxInCnt++;
            }
        }
    }

    tColor = (iPxInCnt * 100 / iAllPxCnt ) < iPxMinPer ? clRed : clLime ;

    for(int y = 0 ; y < m_pMagImg->GetHeight() ; y++) {
        for(int x = 0 ; x < m_pMagImg->GetWidth() ; x++) {
            iPx = m_pMagImg-> GetPixel(x,y);
            if(iThresholdLow < iPx && iPx <=iThresholdHigh){
                Gdi -> Pixel(x,y,tColor);
            }
        }
    }

    GDI_Del(Gdi) ;

    //  Copy buffer bitmap into window dc.
    BitBlt(hDc, 0, 0, R.right, R.bottom, hMemDc, 0, 0, SRCCOPY);

    // Delete object
    SelectObject(hMemDc,hOldMemBm);
    DeleteObject(hMemBm);
    DeleteDC(hMemDc);
    ReleaseDC(pnMag->Handle,hDc);

}*/
//---------------------------------------------------------------------------

void TFrmBfPlce::ApplyPara(bool _bToTable)
{
    TCamLightPara   DPara = VISN[m_iVisnId]->GetCamLightPara();
    TVisnStaticPara SPara = VISN[m_iVisnId]->GetVisnStaticPara();

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
        cbImageSave     -> ItemIndex = VOM.VCmnOptn.iBfPlceImageSave      ;

        //Dev Optn
        edCountX        -> Value     = ((CBfPlce *)VISN[m_iVisnId])->GetInspColCnt() ;
        edCountY        -> Value     = ((CBfPlce *)VISN[m_iVisnId])->GetInspRowCnt() ;

        edOriSinc       -> Value     = VOM.VDevOptn.dBfPlceOriSinc        ;

        cbOverUnder     -> ItemIndex = VOM.VDevOptn.bBfPlceOverUnder ? 1 : 0 ;

        //DevPara.
        sbThresholdLow  -> Position  = VOM.VDevPara.BfPlceTsd.iThresholdLow  ;
        sbThresholdHigh -> Position  = VOM.VDevPara.BfPlceTsd.iThresholdHigh ;
        sbPxMinPer      -> Position  = VOM.VDevOptn.dBfPlcePxMinPer          ;
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
        VOM.VCmnOptn.iBfPlceImageSave         = cbImageSave     -> ItemIndex ;

        //Dev Optn
        ((CBfPlce *)VISN[m_iVisnId])->SetInspColCnt(edCountX        -> Value);
        ((CBfPlce *)VISN[m_iVisnId])->SetInspRowCnt(edCountY        -> Value);

        VOM.VDevOptn.dBfPlceOriSinc           = edOriSinc       -> Value     ;

        VOM.VDevOptn.bBfPlceOverUnder         = cbOverUnder     -> ItemIndex ;

        //DevPara.
        VOM.VDevPara.BfPlceTsd.iThresholdLow  = sbThresholdLow  -> Position  ;
        VOM.VDevPara.BfPlceTsd.iThresholdHigh = sbThresholdHigh -> Position  ;
        VOM.VDevOptn.dBfPlcePxMinPer          = sbPxMinPer      -> Position  ;

        ApplyPara(true);
    }
}


void __fastcall TFrmBfPlce::btSaveClick(TObject *Sender)
{
    ApplyPara(false);

    VOM.LoadJobFile(false);

    //Trace("Operator", "FormVisionPara Form_Save Button Click");
}
//---------------------------------------------------------------------------

void __fastcall TFrmBfPlce::FormShow(TObject *Sender)
{

    ApplyPara(true);

}
//---------------------------------------------------------------------------

void TFrmBfPlce::FormShow(EN_PAGE_ID _iPage)
{
    pcSet -> ActivePageIndex = _iPage ;
    Show();
}








void __fastcall TFrmBfPlce::sbThresholdLowChange(TObject *Sender)
{
    edThresholdLow -> Text = sbThresholdLow -> Position ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmBfPlce::sbThresholdHighChange(TObject *Sender)
{
    edThresholdHigh -> Text = sbThresholdHigh -> Position ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmBfPlce::sbPxMinPerChange(TObject *Sender)
{
    edPxMinPer -> Text = sbPxMinPer -> Position ;
}
//---------------------------------------------------------------------------




void __fastcall TFrmBfPlce::pcSetChange(TObject *Sender)
{
    VISN[m_iVisnId]->SetActivPage((EN_PAGE_ID)pcSet -> ActivePageIndex) ;
    btTrain -> Visible = pcSet -> ActivePageIndex == piBfPlceOcv ;

    FrmImage[m_iVisnId] -> OnPaint();
}
//---------------------------------------------------------------------------

void __fastcall TFrmBfPlce::btTrainClick(TObject *Sender)
{
    if(VISN[m_iVisnId]->GetActivPage() == piBfPlceLCS) {
    }
    else if(VISN[m_iVisnId]->GetActivPage() == piBfPlceOcv) {
        VISN[m_iVisnId]->Train();
        FrmTrain -> OnPaint() ;
    }
    else if(VISN[m_iVisnId]->GetActivPage() == piBfPlceTsd) {
    }
}

//---------------------------------------------------------------------------


void __fastcall TFrmBfPlce::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    bool bChangedTracker = false ;

    static int iPreX = 0 ;
    static int iPreY = 0 ;
    static int iPreW = 0 ;
    static int iPreH = 0 ;

    if(iPreX != ((CBfPlce *)VISN[m_iVisnId]) -> Insp.pTrckInsp1 -> Left  ) bChangedTracker = true ;
    if(iPreY != ((CBfPlce *)VISN[m_iVisnId]) -> Insp.pTrckInsp1 -> Top   ) bChangedTracker = true ;
    if(iPreW != ((CBfPlce *)VISN[m_iVisnId]) -> Insp.pTrckInsp1 -> Width ) bChangedTracker = true ;
    if(iPreH != ((CBfPlce *)VISN[m_iVisnId]) -> Insp.pTrckInsp1 -> Height) bChangedTracker = true ;

    /*
    if(bChangedTracker) {
        VISN[m_iVisnId]->GetImg()->Trim(((CEpoxy *)VISN[m_iVisnId]) -> Insp.pTrckInsp1 -> GetTRect() , m_pMagImg);
        iPreX = ((CEpoxy *)VISN[m_iVisnId]) -> Insp.pTrckInsp1 -> Left   ;
        iPreY = ((CEpoxy *)VISN[m_iVisnId]) -> Insp.pTrckInsp1 -> Top    ;
        iPreW = ((CEpoxy *)VISN[m_iVisnId]) -> Insp.pTrckInsp1 -> Width  ;
        iPreH = ((CEpoxy *)VISN[m_iVisnId]) -> Insp.pTrckInsp1 -> Height ;
        OnPaintMag();
    }
    */
    tmUpdate -> Enabled = true ;        
}
//---------------------------------------------------------------------------

void __fastcall TFrmBfPlce::btLtClick(TObject *Sender)
{
    int iSelButton = ((TButton *)Sender) -> Tag ;

    float fWidth  ;
    float fHeight ;
    float fLeft   ;
    float fTop    ;

    float fXPitch   = (((CBfPlce *)VISN[m_iVisnId])->Insp.pTrckInsp2 -> Left - ((CBfPlce *)VISN[m_iVisnId])->Insp.pTrckInsp1 -> Left) / (float)VOM.VDevOptn.iCountX ;
    float fYPitch   = (((CBfPlce *)VISN[m_iVisnId])->Insp.pTrckInsp2 -> Top  - ((CBfPlce *)VISN[m_iVisnId])->Insp.pTrckInsp1 -> Top ) / (float)VOM.VDevOptn.iCountY ;
    float fTrackerW =  ((CBfPlce *)VISN[m_iVisnId])->Insp.pTrckInsp1 -> Width  ;
    float fTrackerH =  ((CBfPlce *)VISN[m_iVisnId])->Insp.pTrckInsp1 -> Height ;

    if(iSelButton == 0 ) {//left top
        fWidth  = fXPitch * 2;
        fHeight = fYPitch * 2;
        fLeft   = ((CBfPlce *)VISN[m_iVisnId])->Insp.pTrckInsp1 -> Left - fWidth /2 + fTrackerW/2;
        fTop    = ((CBfPlce *)VISN[m_iVisnId])->Insp.pTrckInsp1 -> Top  - fHeight/2 + fTrackerH/2;
    }
    else if(iSelButton == 1) { //right top
        fWidth  = fXPitch * 2;
        fHeight = fYPitch * 2;
        fLeft   = ((CBfPlce *)VISN[m_iVisnId])->Insp.pTrckInsp2 -> Left - fWidth /2 + fTrackerW/2;
        fTop    = ((CBfPlce *)VISN[m_iVisnId])->Insp.pTrckInsp1 -> Top  - fHeight/2 + fTrackerH/2;
    }
    else if(iSelButton == 2) { //left bottom
        fWidth  = fXPitch * 2;
        fHeight = fYPitch * 2;
        fLeft   = ((CBfPlce *)VISN[m_iVisnId])->Insp.pTrckInsp1 -> Left - fWidth /2 + fTrackerW/2;
        fTop    = ((CBfPlce *)VISN[m_iVisnId])->Insp.pTrckInsp2 -> Top  - fHeight/2 + fTrackerH/2;
    }
    else if(iSelButton == 3) { //right bottom
        fWidth  = fXPitch * 2;
        fHeight = fYPitch * 2;
        fLeft   = ((CBfPlce *)VISN[m_iVisnId])->Insp.pTrckInsp2 -> Left - fWidth /2 + fTrackerW/2;
        fTop    = ((CBfPlce *)VISN[m_iVisnId])->Insp.pTrckInsp2 -> Top  - fHeight/2 + fTrackerH/2;
    }
    else { //Stratch
        FrmImage[m_iVisnId] -> SetViewMode(true);
        return ;
    }

    FrmImage[m_iVisnId] -> SetMagView(fLeft , fTop , fWidth ,fHeight );
}
//---------------------------------------------------------------------------

