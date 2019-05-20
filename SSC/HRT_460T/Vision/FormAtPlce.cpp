//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormAtPlce.h"
#include "AtPlce.h"
#include "VOptnMan.h"
#include "SLogUnit.h"
#include "FormImage.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TFrmAtPlce *FrmAtPlce;
//---------------------------------------------------------------------------
__fastcall TFrmAtPlce::TFrmAtPlce(TComponent* Owner)
        : TForm(Owner)
{

}

void TFrmAtPlce::Init()
{
    Left = 0 ;
    Top  = 0 ;

    m_iVisnId = viAtPlce ;

    //OriginalProc = pnMag->WindowProc ;
    //pnMag->WindowProc = PanelProc   ;
    //m_pMagImg = IMG_New() ;


    FrmTrain = new TFrmTrainChk (pnTrain);
    FrmTrain -> SetTrainArea ( ((CAtPlce *)VISN[m_iVisnId])-> Orig.pTrArea );
    FrmTrain -> SetTrainImg  ( ((CAtPlce *)VISN[m_iVisnId])-> Orig.pTrImage);
    FrmTrain -> BorderStyle = bsNone ;
    FrmTrain -> Align = alClient ;
    FrmTrain -> Parent = pnTrain ;
    //FrmTrain -> Left = 0 ;
    //FrmTrain -> Top  = 0 ;

    FrmTrain -> Show() ;

    tmUpdate -> Enabled = true ;

}

void TFrmAtPlce::Close()
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

void TFrmAtPlce::ApplyPara(bool _bToTable)
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
        cbImageSave     -> ItemIndex = VOM.VCmnOptn.iAtPlceImageSave      ;

        //Dev Optn
        edCountX        -> Value     = ((CAtPlce *)VISN[m_iVisnId])->GetInspColCnt() ;
        edCountY        -> Value     = ((CAtPlce *)VISN[m_iVisnId])->GetInspRowCnt() ;

        edOriSinc       -> Value     = VOM.VDevOptn.dAtPlceOriSinc        ;

        cbOverUnder     -> ItemIndex = VOM.VDevOptn.bAtPlceOverUnder ? 1 : 0 ;

        //DevPara.
        sbThresholdLow  -> Position  = VOM.VDevPara.AtPlceTsd.iThresholdLow  ;
        sbThresholdHigh -> Position  = VOM.VDevPara.AtPlceTsd.iThresholdHigh ;
        sbPxMinPer      -> Position  = VOM.VDevOptn.dAtPlcePxMinPer          ;
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
        VOM.VCmnOptn.iAtPlceImageSave         = cbImageSave     -> ItemIndex ;

        //Dev Optn
        ((CAtPlce *)VISN[m_iVisnId])->SetInspColCnt(edCountX        -> Value);
        ((CAtPlce *)VISN[m_iVisnId])->SetInspRowCnt(edCountY        -> Value);

        VOM.VDevOptn.dAtPlceOriSinc           = edOriSinc       -> Value     ;

        VOM.VDevOptn.bAtPlceOverUnder         = cbOverUnder     -> ItemIndex ;

        //DevPara.
        VOM.VDevPara.AtPlceTsd.iThresholdLow  = sbThresholdLow  -> Position  ;
        VOM.VDevPara.AtPlceTsd.iThresholdHigh = sbThresholdHigh -> Position  ;
        VOM.VDevOptn.dAtPlcePxMinPer          = sbPxMinPer      -> Position  ;

        ApplyPara(true);
    }
}


void __fastcall TFrmAtPlce::btSaveClick(TObject *Sender)
{
    ApplyPara(false);

    VOM.LoadJobFile(false);

    //Trace("Operator", "FormVisionPara Form_Save Button Click");
}
//---------------------------------------------------------------------------

void __fastcall TFrmAtPlce::FormShow(TObject *Sender)
{

    ApplyPara(true);

}
//---------------------------------------------------------------------------

void TFrmAtPlce::FormShow(EN_PAGE_ID _iPage)
{
    pcSet -> ActivePageIndex = _iPage ;
    Show();
}








void __fastcall TFrmAtPlce::sbThresholdLowChange(TObject *Sender)
{
    edThresholdLow -> Text = sbThresholdLow -> Position ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmAtPlce::sbThresholdHighChange(TObject *Sender)
{
    edThresholdHigh -> Text = sbThresholdHigh -> Position ;
}
//---------------------------------------------------------------------------

void __fastcall TFrmAtPlce::sbPxMinPerChange(TObject *Sender)
{
    edPxMinPer -> Text = sbPxMinPer -> Position ;
}
//---------------------------------------------------------------------------




void __fastcall TFrmAtPlce::pcSetChange(TObject *Sender)
{
    VISN[m_iVisnId]->SetActivPage((EN_PAGE_ID)pcSet -> ActivePageIndex) ;
    btTrain -> Visible = pcSet -> ActivePageIndex == piAtPlceOcv ;

    FrmImage[m_iVisnId] -> OnPaint();
}
//---------------------------------------------------------------------------

void __fastcall TFrmAtPlce::btTrainClick(TObject *Sender)
{
    if(VISN[m_iVisnId]->GetActivPage() == piAtPlceLCS) {
    }
    else if(VISN[m_iVisnId]->GetActivPage() == piAtPlceOcv) {
        VISN[m_iVisnId]->Train();
        FrmTrain -> OnPaint() ;
    }
    else if(VISN[m_iVisnId]->GetActivPage() == piAtPlceTsd) {
    }
}

//---------------------------------------------------------------------------


void __fastcall TFrmAtPlce::tmUpdateTimer(TObject *Sender)
{
    tmUpdate -> Enabled = false ;

    bool bChangedTracker = false ;

    static int iPreX = 0 ;
    static int iPreY = 0 ;
    static int iPreW = 0 ;
    static int iPreH = 0 ;

    if(iPreX != ((CAtPlce *)VISN[m_iVisnId]) -> Insp.pTrckInsp1 -> Left  ) bChangedTracker = true ;
    if(iPreY != ((CAtPlce *)VISN[m_iVisnId]) -> Insp.pTrckInsp1 -> Top   ) bChangedTracker = true ;
    if(iPreW != ((CAtPlce *)VISN[m_iVisnId]) -> Insp.pTrckInsp1 -> Width ) bChangedTracker = true ;
    if(iPreH != ((CAtPlce *)VISN[m_iVisnId]) -> Insp.pTrckInsp1 -> Height) bChangedTracker = true ;

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

void __fastcall TFrmAtPlce::btLtClick(TObject *Sender)
{
    int iSelButton = ((TButton *)Sender) -> Tag ;

    float fWidth  ;
    float fHeight ;
    float fLeft   ;
    float fTop    ;

    float fXPitch   = (((CAtPlce *)VISN[m_iVisnId])->Insp.pTrckInsp2 -> Left - ((CAtPlce *)VISN[m_iVisnId])->Insp.pTrckInsp1 -> Left) / (float)VOM.VDevOptn.iCountX ;
    float fYPitch   = (((CAtPlce *)VISN[m_iVisnId])->Insp.pTrckInsp2 -> Top  - ((CAtPlce *)VISN[m_iVisnId])->Insp.pTrckInsp1 -> Top ) / (float)VOM.VDevOptn.iCountY ;
    float fTrackerW =  ((CAtPlce *)VISN[m_iVisnId])->Insp.pTrckInsp1 -> Width  ;
    float fTrackerH =  ((CAtPlce *)VISN[m_iVisnId])->Insp.pTrckInsp1 -> Height ;

    if(iSelButton == 0 ) {//left top
        fWidth  = fXPitch * 2;
        fHeight = fYPitch * 2;
        fLeft   = ((CAtPlce *)VISN[m_iVisnId])->Insp.pTrckInsp1 -> Left - fWidth /2 + fTrackerW/2;
        fTop    = ((CAtPlce *)VISN[m_iVisnId])->Insp.pTrckInsp1 -> Top  - fHeight/2 + fTrackerH/2;
    }
    else if(iSelButton == 1) { //right top
        fWidth  = fXPitch * 2;
        fHeight = fYPitch * 2;
        fLeft   = ((CAtPlce *)VISN[m_iVisnId])->Insp.pTrckInsp2 -> Left - fWidth /2 + fTrackerW/2;
        fTop    = ((CAtPlce *)VISN[m_iVisnId])->Insp.pTrckInsp1 -> Top  - fHeight/2 + fTrackerH/2;
    }
    else if(iSelButton == 2) { //left bottom
        fWidth  = fXPitch * 2;
        fHeight = fYPitch * 2;
        fLeft   = ((CAtPlce *)VISN[m_iVisnId])->Insp.pTrckInsp1 -> Left - fWidth /2 + fTrackerW/2;
        fTop    = ((CAtPlce *)VISN[m_iVisnId])->Insp.pTrckInsp2 -> Top  - fHeight/2 + fTrackerH/2;
    }
    else if(iSelButton == 3) { //right bottom
        fWidth  = fXPitch * 2;
        fHeight = fYPitch * 2;
        fLeft   = ((CAtPlce *)VISN[m_iVisnId])->Insp.pTrckInsp2 -> Left - fWidth /2 + fTrackerW/2;
        fTop    = ((CAtPlce *)VISN[m_iVisnId])->Insp.pTrckInsp2 -> Top  - fHeight/2 + fTrackerH/2;
    }
    else { //Stratch
        FrmImage[m_iVisnId] -> SetViewMode(true);
        return ;
    }

    FrmImage[m_iVisnId] -> SetMagView(fLeft , fTop , fWidth ,fHeight );
}
//---------------------------------------------------------------------------

